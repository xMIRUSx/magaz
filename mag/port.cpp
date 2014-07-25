//---------------------------------------------------------------------------

#pragma hdrstop

#include <vcl.h>

#include <io.h>         //для работы с файлами
#include <fcntl.h>      //для работы с файлами
#include <sys\stat.h>   //для работы с файлами

#include "port.h"
#include <stdio.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#define BUFSIZE 255

unsigned long bytecnt;
unsigned char bufrd[BUFSIZE], bufwr[BUFSIZE]; //приёмный и передающий буферы

HANDLE COMport;		//дескриптор порта
HANDLE reader;	//дескриптор потока чтения из порта
HANDLE writer;	//дескриптор потока записи в порт

OVERLAPPED olread;
OVERLAPPED olwrite;

bool fl = false;


DWORD WINAPI ReceiveFromDeviceThread(LPVOID)
{
	 COMSTAT comstat;		//структура текущего состояния порта, в данной программе используется
					//для определения количества принятых в порт байтов
	 DWORD btr, temp, mask, signal;	//переменная temp используется в качестве заглушки

	 //создать сигнальный объект-событие для асинхронных операций
	 olread.hEvent = CreateEvent(NULL, true, true, NULL);

	 //установить маску на срабатывание по событию приёма байта в порт
	 SetCommMask(COMport, EV_RXCHAR);
	 while(true)	//пока поток не будет прерван, выполняем цикл
	  {
	   //ожидать события приёма байта (это и есть перекрываемая операция)
	   WaitCommEvent(COMport, &mask, &olread);

		//приостановить поток до прихода байта
	   signal = WaitForSingleObject(olread.hEvent, INFINITE);
		//если событие прихода байта произошло
	   if(signal == WAIT_OBJECT_0)
		{
		//проверяем, успешно ли завершилась
		//перекрываемая операция WaitCommEvent
		 if(GetOverlappedResult(COMport, &olread, &temp, true))
		  //если произошло именно событие прихода байта
		  if((mask & EV_RXCHAR)!=0)
		   {
			//нужно заполнить структуру COMSTAT
			ClearCommError(COMport, &temp, &comstat);
			//и получить из неё количество принятых байтов
			btr = comstat.cbInQue;
			//если действительно есть байты для чтения
			if(btr)
			{
			 //прочитать байты из порта в буфер программы
			 ReadFile(COMport, bufrd, btr, &temp, &olread);
			 bytecnt+=btr; //увеличиваем счётчик байтов
//			 ReadPrinting(); //вызываем функцию для вывода данных на экран и в файл
			}
		   }
		}
	  }
	// CloseHandle(olread.hEvent);		//перед выходом из потока закрыть объект-событие
}

DWORD WINAPI SendToDeviceThread(LPVOID)
{
    DWORD temp, signal;	//temp - переменная-заглушка

	olwrite.hEvent = CreateEvent(NULL, true, true, NULL);   	  //создать событие
	while(1)
	{
		//записать байты в порт (перекрываемая операция!)
		WriteFile(COMport, bufwr, strlen(bufwr), &temp, &olwrite);
		 signal = WaitForSingleObject(olwrite.hEvent, INFINITE);	//приостановить поток, пока не завершится
									//перекрываемая операция WriteFile
		//если операция завершилась успешно, установить соответствующий флажок
		if((signal == WAIT_OBJECT_0) && (GetOverlappedResult(COMport, &olwrite, &temp, true)))
		{
			fl = true;
			//вывод сообщения об успешной установке
		}
		else
		{
			fl = false;
			//сообщение об ошибке
		}
    	SuspendThread(/*имя потока */);
	}
}


void ClosePort()
{
//Примечание: так как при прерывании потоков, созданных с помощью функций WinAPI, функцией TerminateThread
//	      поток может быть прерван жёстко, в любом месте своего выполнения, то освобождать дескриптор
//	      сигнального объекта-события, находящегося в структуре типа OVERLAPPED, связанной с потоком,
//	      следует не внутри кода потока, а отдельно, после вызова функции TerminateThread.
//	      После чего нужно освободить и сам дескриптор потока.

 //если поток записи работает, завершить его; проверка if(writer) обязательна, иначе возникают ошибки
 if(writer)
 {
	TerminateThread(writer,0);
	CloseHandle(olwrite.hEvent);	//нужно закрыть объект-событие
	CloseHandle(writer);
 }

 //если поток чтения работает, завершить его; проверка if(reader) обязательна, иначе возникают ошибки
 if(reader)
 {
   TerminateThread(reader,0);
   CloseHandle(olread.hEvent);	//нужно закрыть объект-событие
   CloseHandle(reader);
  }

 CloseHandle(COMport);  //закрыть порт
 COMport=0;		//обнулить переменную для дескриптора порта
// close(handle);		//закрыть файл, в который велась запись принимаемых данных
// handle=0;		//обнулить переменную для дескриптора файла
}

void OpenPort(String portname)
{
//	 String portname     	//имя порта (например, "COM1", "COM2" и т.д.)
	 DCB dcb;           	//структура для общей инициализации порта DCB
	 COMMTIMEOUTS timeouts;  	//структура для установки таймаутов

//	 portname = ;	//получить имя выбранного порта

	 //открыть порт, для асинхронных операций обязательно нужно указать флаг FILE_FLAG_OVERLAPPED
	 COMport = CreateFile(portname.c_str(),GENERIC_READ | GENERIC_WRITE, 0,
				  NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	 //здесь:
	 // - portname.c_str() - имя порта в качестве имени файла,
	 //   c_str() преобразует строку типа String в строку в виде массива типа char, иначе функция не примет
	 // - GENERIC_READ | GENERIC_WRITE - доступ к порту на чтение/записть
	 // - 0 - порт не может быть общедоступным (shared)
	 // - NULL - дескриптор порта не наследуется, используется дескриптор безопасности по умолчанию
	 // - OPEN_EXISTING - порт должен открываться как уже существующий файл
	 // - FILE_FLAG_OVERLAPPED - этот флаг указывает на использование асинхронных операций
	 // - NULL - указатель на файл шаблона не используется при работе с портами

	 if(COMport == INVALID_HANDLE_VALUE)            //если ошибка открытия порта
	   return;

	 //инициализация порта

	 dcb.DCBlength = sizeof(DCB); 	//в первое поле структуры DCB необходимо занести её длину,
					//она будет использоваться функциями настройки порта
					//для контроля корректности структуры

	 //считать структуру DCB из порта
	 if(!GetCommState(COMport, &dcb))
	  {
	   //если не удалось - закрыть порт и вывести сообщение об ошибке в строке состояния
	   ClosePort();
	   ShowMessage("Ошибка соединения: не удалось считать DCB!");
	   return;
	  }

	 //инициализация структуры DCB
	 dcb.BaudRate = 9600; //задаём скорость передачи 115200 бод
	 dcb.fBinary = TRUE;                              //включаем двоичный режим обмена
	 dcb.fOutxCtsFlow = FALSE;                        //выключаем режим слежения за сигналом CTS
	 dcb.fOutxDsrFlow = FALSE;                        //выключаем режим слежения за сигналом DSR
	 dcb.fDtrControl = DTR_CONTROL_DISABLE;           //отключаем использование линии DTR
	 dcb.fDsrSensitivity = FALSE;                     //отключаем восприимчивость драйвера к состоянию линии DSR
	 dcb.fNull = FALSE;                               //разрешить приём нулевых байтов
	 dcb.fRtsControl = RTS_CONTROL_DISABLE;           //отключаем использование линии RTS
	 dcb.fAbortOnError = FALSE;                       //отключаем остановку всех операций чтения/записи при ошибке
	 dcb.ByteSize = 8;                                //задаём 8 бит в байте
	 dcb.Parity = 0;                                  //отключаем проверку чётности
	 dcb.StopBits = 0;                                //задаём один стоп-бит

	 //загрузить структуру DCB в порт
	 if(!SetCommState(COMport, &dcb))
	  {
	   //если не удалось - закрыть порт и вывести сообщение об ошибке в строке состояния
	   ClosePort();
	   ShowMessage("Ошибка соединения: не удалось установить DCB!");
	   return;
	  }

	 //установить таймауты
	 timeouts.ReadIntervalTimeout = 0;	 	//таймаут между двумя символами
	 timeouts.ReadTotalTimeoutMultiplier = 0;	//общий таймаут операции чтения
	 timeouts.ReadTotalTimeoutConstant = 0;       //константа для общего таймаута операции чтения
	 timeouts.WriteTotalTimeoutMultiplier = 0;    //общий таймаут операции записи
	 timeouts.WriteTotalTimeoutConstant = 0;      //константа для общего таймаута операции записи

	 //записать структуру таймаутов в порт
	 //если не удалось - закрыть порт и вывести сообщение об ошибке в строке состояния
	 if(!SetCommTimeouts(COMport, &timeouts))
	  {
	   ClosePort();
	   ShowMessage("Ошибка соединения:  не удалось установить тайм-ауты!");
	   return;
	  }

	 //установить размеры очередей приёма и передачи
	 SetupComm(COMport,2000,2000);

			 //создать или открыть существующий файл для записи принимаемых данных
			 /*
			 handle = open("test.txt", O_CREAT | O_APPEND | O_BINARY | O_WRONLY, S_IREAD | S_IWRITE);

			 if(handle==-1)		//если произошла ошибка открытия файла
			  {
			   //вывести сообщение об этом в командной строке
			   Form1->StatusBar1->Panels->Items[1]->Text = "Ошибка открытия файла";
			   Form1->Label6->Hide();                                               //спрятать надпись с именем файла
			   Form1->CheckBox3->Checked = false;                                   //сбросить и отключить галочку
			   Form1->CheckBox3->Enabled = false;
			  }
			 //иначе вывести в строке состояния сообщение об успешном открытии файла
			 else { Form1->StatusBar1->Panels->Items[0]->Text = "Файл открыт успешно"; }
			 */
	 PurgeComm(COMport, PURGE_RXCLEAR);	//очистить принимающий буфер порта

	 reader = CreateThread(NULL, 0, ReceiveFromDeviceThread, NULL, 0, NULL);  //создать и запустить поток чтения байтов
	 reader->FreeOnTerminate = true;  //установить это свойство потока,
					  //чтобы он автоматически уничтожался после завершения


	//поменять капшн на кнопке
	bytecnt = 0;

}


//функция поиска и подключения устройства
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
		//устройство не найдено
	}
}
