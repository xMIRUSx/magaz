//---------------------------------------------------------------------------

#pragma hdrstop

#include <vcl.h>

#include "port.h"
#include "main.h"
#include <stdio.h>
#include <string.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
int handle;

void ReadBuf()
{

	String rdt = (UnicodeString)(char*)bufrd;


	if (*bufrd == *ID)
	{
        connected = true;
		strcpy(bufwr,"D");
		ResumeThread(writer);
		Form1->Info->Lines->Add("���������� ����������!");
		Form1->ConnectionButton->Caption = "���������� ����������";
	}

	if( *bufrd=='I')
	{
		strcpy(bufwr,"I");
		ResumeThread(writer);
	}
	else
	{
        Form1->Info->Lines->Add("##BUFRD##");
		Form1->Info->Lines->Add(rdt);
		Form1->Info->Lines->Add("#########");

    }
	if( *bufrd=='B')
	{
		Form1->Info->Lines->Add("������� ����������");
		Form1->Info->Lines->Add("������� �������");
		ResumeThread(writer);
	}

	memset(bufrd, 0, BUFSIZE);	        //�������� �����
}

DWORD WINAPI ReceiveFromDeviceThread(LPVOID)
{
	 COMSTAT comstat;		//��������� �������� ��������� �����, � ������ ��������� ������������
					//��� ����������� ���������� �������� � ���� ������
	 DWORD btr, temp, mask, signal;	//���������� temp ������������ � �������� ��������

	 //������� ���������� ������-������� ��� ����������� ��������
	 olread.hEvent = CreateEvent(NULL, true, true, NULL);

	 //���������� ����� �� ������������ �� ������� ����� ����� � ����
	 SetCommMask(COMport, EV_RXCHAR);
	 while(true)	//���� ����� �� ����� �������, ��������� ����
	  {
	   //������� ������� ����� ����� (��� � ���� ������������� ��������)
	   WaitCommEvent(COMport, &mask, &olread);

		//������������� ����� �� ������� �����
	   signal = WaitForSingleObject(olread.hEvent, INFINITE);
		//���� ������� ������� ����� ���������
	   if(signal == WAIT_OBJECT_0)
		{
		//���������, ������� �� �����������
		//������������� �������� WaitCommEvent
		 if(GetOverlappedResult(COMport, &olread, &temp, true))
		  //���� ��������� ������ ������� ������� �����
		  if((mask & EV_RXCHAR)!=0)
		   {
			//����� ��������� ��������� COMSTAT
			ClearCommError(COMport, &temp, &comstat);
			//� �������� �� �� ���������� �������� ������
			btr = comstat.cbInQue;
			//���� ������������� ���� ����� ��� ������
			if(btr)
			{
			 //��������� ����� �� ����� � ����� ���������
			 ReadFile(COMport, bufrd, btr, &temp, &olread);
			 ReadBuf(); //�������� ������� ��� ������ ������ �� ����� � � ����
			}
		   }
		}
	  }
	// CloseHandle(olread.hEvent);		//����� ������� �� ������ ������� ������-�������
}

DWORD WINAPI SendToDeviceThread(LPVOID)
{
    DWORD temp, signal;	//temp - ����������-��������

	olwrite.hEvent = CreateEvent(NULL, true, true, NULL);   	  //������� �������
	while(1)
	{
		//�������� ����� � ���� (������������� ��������!)
		WriteFile(COMport, bufwr, strlen(bufwr), &temp, &olwrite);
		 signal = WaitForSingleObject(olwrite.hEvent, INFINITE);	//������������� �����, ���� �� ����������
									//������������� �������� WriteFile
		//���� �������� ����������� �������, ���������� ��������������� ������
		if((signal == WAIT_OBJECT_0) && (GetOverlappedResult(COMport, &olwrite, &temp, true)))
		{
		 ;;	//����� ��������� �� �������� ���������
		}
		else
		{
			Form1->Info->Lines->Add("�������� �� ����������");
		}
    	SuspendThread(writer);
	}
}


void ClosePort()
{
//����������: ��� ��� ��� ���������� �������, ��������� � ������� ������� WinAPI, �������� TerminateThread
//	      ����� ����� ���� ������� �����, � ����� ����� ������ ����������, �� ����������� ����������
//	      ����������� �������-�������, ������������ � ��������� ���� OVERLAPPED, ��������� � �������,
//	      ������� �� ������ ���� ������, � ��������, ����� ������ ������� TerminateThread.
//	      ����� ���� ����� ���������� � ��� ���������� ������.

 //���� ����� ������ ��������, ��������� ���; �������� if(writer) �����������, ����� ��������� ������

 connected = false;
 if(writer)
 {
	TerminateThread(writer,0);
	CloseHandle(olwrite.hEvent);	//����� ������� ������-�������
	CloseHandle(writer);
 }

 //���� ����� ������ ��������, ��������� ���; �������� if(reader) �����������, ����� ��������� ������
 if(reader)
 {
   TerminateThread(reader,0);
   CloseHandle(olread.hEvent);	//����� ������� ������-�������
   CloseHandle(reader);
  }

 CloseHandle(COMport);  //������� ����
 COMport=0;		//�������� ���������� ��� ����������� �����
// close(handle);		//������� ����, � ������� ������ ������ ����������� ������
// handle=0;		//�������� ���������� ��� ����������� �����
}

void OpenPort(String portname)
{
//	 String portname     	//��� ����� (��������, "COM1", "COM2" � �.�.)
	 DCB dcb;           	//��������� ��� ����� ������������� ����� DCB
	 COMMTIMEOUTS timeouts;  	//��������� ��� ��������� ���������

//	 portname = ;	//�������� ��� ���������� �����

	 //������� ����, ��� ����������� �������� ����������� ����� ������� ���� FILE_FLAG_OVERLAPPED
	char* port;
	port = new char[15];
	wcstombs(port, portname.c_str(), 15);

	COMport = CreateFile(port, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	 //�����:
	 // - port - ��� ����� � �������� ����� �����,
	 // - GENERIC_READ | GENERIC_WRITE - ������ � ����� �� ������/�������
	 // - 0 - ���� �� ����� ���� ������������� (shared)
	 // - NULL - ���������� ����� �� �����������, ������������ ���������� ������������ �� ���������
	 // - OPEN_EXISTING - ���� ������ ����������� ��� ��� ������������ ����
	 // - FILE_FLAG_OVERLAPPED - ���� ���� ��������� �� ������������� ����������� ��������
	 // - NULL - ��������� �� ���� ������� �� ������������ ��� ������ � �������

	 if(COMport == INVALID_HANDLE_VALUE)            //���� ������ �������� �����
	 {
// 	   Form1->Info->Lines->Add(GetLastError());
	   return;
	 }

	 //������������� �����

	 dcb.DCBlength = sizeof(DCB); 	//� ������ ���� ��������� DCB ���������� ������� � �����,
					//��� ����� �������������� ��������� ��������� �����
					//��� �������� ������������ ���������

	 //������� ��������� DCB �� �����
	 if(!GetCommState(COMport, &dcb))
	  {
	   //���� �� ������� - ������� ���� � ������� ��������� �� ������ � ������ ���������
	   ClosePort();
	   ShowMessage("������ ����������: �� ������� ������� DCB!");
	   return;
	  }

	 //������������� ��������� DCB
	 dcb.BaudRate = 9600; //����� �������� ��������
	 dcb.fBinary = TRUE;                              //�������� �������� ����� ������
	 dcb.fOutxCtsFlow = FALSE;                        //��������� ����� �������� �� �������� CTS
	 dcb.fOutxDsrFlow = FALSE;                        //��������� ����� �������� �� �������� DSR
	 dcb.fDtrControl = DTR_CONTROL_DISABLE;           //��������� ������������� ����� DTR
	 dcb.fDsrSensitivity = FALSE;                     //��������� ��������������� �������� � ��������� ����� DSR
	 dcb.fNull = FALSE;                               //��������� ���� ������� ������
	 dcb.fRtsControl = RTS_CONTROL_DISABLE;           //��������� ������������� ����� RTS
	 dcb.fAbortOnError = FALSE;                       //��������� ��������� ���� �������� ������/������ ��� ������
	 dcb.ByteSize = 8;                                //����� 8 ��� � �����
	 dcb.Parity = 0;                                  //��������� �������� ��������
	 dcb.StopBits = 0;                                //����� ���� ����-���

	 //��������� ��������� DCB � ����
	 if(!SetCommState(COMport, &dcb))
	  {
	   //���� �� ������� - ������� ���� � ������� ��������� �� ������ � ������ ���������
	   ClosePort();
	   ShowMessage("������ ����������: �� ������� ���������� DCB!");
	   return;
	  }

	 //���������� ��������
	 timeouts.ReadIntervalTimeout = 1;	 	//������� ����� ����� ���������
	 timeouts.ReadTotalTimeoutMultiplier = 1;	//����� ������� �������� ������
	 timeouts.ReadTotalTimeoutConstant = 1;       //��������� ��� ������ �������� �������� ������
	 timeouts.WriteTotalTimeoutMultiplier = 1;    //����� ������� �������� ������
	 timeouts.WriteTotalTimeoutConstant = 1;      //��������� ��� ������ �������� �������� ������

	 //�������� ��������� ��������� � ����
	 //���� �� ������� - ������� ���� � ������� ��������� �� ������ � ������ ���������
	 if(!SetCommTimeouts(COMport, &timeouts))
	  {
	   ClosePort();
	   ShowMessage("������ ����������:  �� ������� ���������� ����-����!");
	   return;
	  }

	 //���������� ������� �������� ����� � ��������
	 SetupComm(COMport,2000,2000);

			 //������� ��� ������� ������������ ���� ��� ������ ����������� ������

			 handle = open("test.txt", O_CREAT | O_APPEND | O_BINARY | O_WRONLY, S_IREAD | S_IWRITE);

			 if(handle==-1)		//���� ��������� ������ �������� �����
			  {
			   //������� ��������� �� ���� � ��������� ������
			   Form1->Info->Lines->Add("������ �������� �����");
			  }
			 //����� ������� � ������ ��������� ��������� �� �������� �������� �����
			 else {
//				Form1->Info->Lines->Add("���-01� ���������");
				}

	 PurgeComm(COMport, PURGE_RXCLEAR);	//�������� ����������� ����� �����

 //  	 EscapeCommFunction(COMport, SETDTR);
 //	 EscapeCommFunction(COMport, SETRTS);

	 reader = CreateThread(NULL, 0, ReceiveFromDeviceThread, NULL, 0, NULL);  //������� � ��������� ����� ������ ������
	 writer = CreateThread(NULL, 0, SendToDeviceThread, NULL, CREATE_SUSPENDED, NULL);	//������ ����� ������ � ������������� ��������� (������������� �������� = CREATE_SUSPENDED)
}


//������� ������ � ����������� ����������
void FindDevice(const char* devID)
{
	ID = devID;
	strcpy(bufwr, ID);

	String port = "\\\\.\\COM";

	int i = 1;
	while ((!connected) && (i < 257))

	{
		OpenPort(port + IntToStr(i));

		if(COMport != INVALID_HANDLE_VALUE)
		{
			ResumeThread(writer);
			 Sleep(1000);
		}
		i++;
	}
	if (COMport == INVALID_HANDLE_VALUE)
	{
		Form1->Info->Lines->Add("���������� �� �������!!!");
	}
}

