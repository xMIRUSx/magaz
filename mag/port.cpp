//---------------------------------------------------------------------------

#pragma hdrstop

#include <vcl.h>

#include <io.h>         //��� ������ � �������
#include <fcntl.h>      //��� ������ � �������
#include <sys\stat.h>   //��� ������ � �������

#include "port.h"
#include <stdio.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#define BUFSIZE 255

unsigned long bytecnt;
unsigned char bufrd[BUFSIZE], bufwr[BUFSIZE]; //������� � ���������� ������

HANDLE COMport;		//���������� �����
HANDLE reader;	//���������� ������ ������ �� �����
HANDLE writer;	//���������� ������ ������ � ����

OVERLAPPED olread;
OVERLAPPED olwrite;

bool fl = false;


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
			 bytecnt+=btr; //����������� ������� ������
//			 ReadPrinting(); //�������� ������� ��� ������ ������ �� ����� � � ����
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
			fl = true;
			//����� ��������� �� �������� ���������
		}
		else
		{
			fl = false;
			//��������� �� ������
		}
    	SuspendThread(/*��� ������ */);
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
	 COMport = CreateFile(portname.c_str(),GENERIC_READ | GENERIC_WRITE, 0,
				  NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	 //�����:
	 // - portname.c_str() - ��� ����� � �������� ����� �����,
	 //   c_str() ����������� ������ ���� String � ������ � ���� ������� ���� char, ����� ������� �� ������
	 // - GENERIC_READ | GENERIC_WRITE - ������ � ����� �� ������/�������
	 // - 0 - ���� �� ����� ���� ������������� (shared)
	 // - NULL - ���������� ����� �� �����������, ������������ ���������� ������������ �� ���������
	 // - OPEN_EXISTING - ���� ������ ����������� ��� ��� ������������ ����
	 // - FILE_FLAG_OVERLAPPED - ���� ���� ��������� �� ������������� ����������� ��������
	 // - NULL - ��������� �� ���� ������� �� ������������ ��� ������ � �������

	 if(COMport == INVALID_HANDLE_VALUE)            //���� ������ �������� �����
	   return;

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
	 dcb.BaudRate = 9600; //����� �������� �������� 115200 ���
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
	 timeouts.ReadIntervalTimeout = 0;	 	//������� ����� ����� ���������
	 timeouts.ReadTotalTimeoutMultiplier = 0;	//����� ������� �������� ������
	 timeouts.ReadTotalTimeoutConstant = 0;       //��������� ��� ������ �������� �������� ������
	 timeouts.WriteTotalTimeoutMultiplier = 0;    //����� ������� �������� ������
	 timeouts.WriteTotalTimeoutConstant = 0;      //��������� ��� ������ �������� �������� ������

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
			 /*
			 handle = open("test.txt", O_CREAT | O_APPEND | O_BINARY | O_WRONLY, S_IREAD | S_IWRITE);

			 if(handle==-1)		//���� ��������� ������ �������� �����
			  {
			   //������� ��������� �� ���� � ��������� ������
			   Form1->StatusBar1->Panels->Items[1]->Text = "������ �������� �����";
			   Form1->Label6->Hide();                                               //�������� ������� � ������ �����
			   Form1->CheckBox3->Checked = false;                                   //�������� � ��������� �������
			   Form1->CheckBox3->Enabled = false;
			  }
			 //����� ������� � ������ ��������� ��������� �� �������� �������� �����
			 else { Form1->StatusBar1->Panels->Items[0]->Text = "���� ������ �������"; }
			 */
	 PurgeComm(COMport, PURGE_RXCLEAR);	//�������� ����������� ����� �����

	 reader = CreateThread(NULL, 0, ReceiveFromDeviceThread, NULL, 0, NULL);  //������� � ��������� ����� ������ ������
	 reader->FreeOnTerminate = true;  //���������� ��� �������� ������,
					  //����� �� ������������� ����������� ����� ����������


	//�������� ����� �� ������
	bytecnt = 0;

}


//������� ������ � ����������� ����������
void FindDevice()
{
	String port = "\\\\.\\COM";
	for (int i = 0; i < 255; i++)
	{
		OpenPort(port + IntToStr(i));
		if(COMport != INVALID_HANDLE_VALUE)
			break;
	}
	if (COMport == INVALID_HANDLE_VALUE)
	{
		//���������� �� �������
	}
}
