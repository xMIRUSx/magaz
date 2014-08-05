//---------------------------------------------------------------------------

#include <vcl.h>
#include <string.h>
#include <stdio.h>
#pragma hdrstop

#include "main.h"
#include "FProfileEdit.h"
#include "ProfileEdit.h"

#include <io.h>         //для работы с файлами
#include <fcntl.h>      //для работы с файлами
#include <sys\stat.h>   //для работы с файлами
//****************************
#define BUFSIZE 25

unsigned char bufrd[BUFSIZE], bufwr[BUFSIZE]; //приёмный и передающий буферы
unsigned char bufI[1];

HANDLE COMport;		//дескриптор порта

HANDLE reader;	//дескриптор потока чтения из порта
HANDLE writer;	//дескриптор потока записи в порт

OVERLAPPED olread;
OVERLAPPED olwrite;

bool connected = false;
bool autoinp = false;
bool signalI = true;
const char* ID;
//###############

void FindDevice(const char* ID);

void OpenPort(String portname);
void ClosePort();

DWORD WINAPI SendToDeviceThread(LPVOID);
DWORD WINAPI SendToDeviceThread(LPVOID buf);
DWORD WINAPI ReceiveFromDeviceThread(LPVOID);
//****************************

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	//символ для опознавания выбранного устройства (по умолчанию МС3070ПУ)
	devID = new char [1];
	devID = "M";

	//инициализация массива для хранения профиля номиналов
	Fprofile = new double*[7];
	for(size_t i = 0; i < 7; ++i)
		Fprofile[i] = new double[9];

	//инициализация путей к используемым файлам профилей
	FprofilePath = new char;
	FprofilePath = '\0';
	profilePath = '\0';

	//инициализация нового значения дли инкриментного/декрементного ввода
	//меньше минимального возможного, чтобы определять,
	//является ли текущий вызов процедуры отправки первым
	incrNextVal = 0,0001;

	memset(bufwr, 0, BUFSIZE);
	memset(bufrd, 0, BUFSIZE);
	strcpy(bufI, "I");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
	for(size_t i = 0; i < 7; ++i)
		delete [] Fprofile[i];
	delete [] Fprofile;
	delete [] FprofilePath;

	if(ConnectionButton->Caption != "Подключение к устройству")
	{
		strcpy(bufwr,"E");
		ResumeThread(writer);
		ClosePort();
    }

}
//---------------------------------------------------------------------------

//########################################
//УСТАНОВКА НАСТРОЕК ВВОДА И ОТОБРАЖЕНИЯ##
//########################################

//смена идентификатора в зависимости от выбранного устройства
void __fastcall TForm1::RadioDeviceClick(TObject *Sender)
{
	if (RadioDevice->ItemIndex == 0)
		devID = "M";
	else
		devID = "K";
}

//отображение настроек, соответствующих выбранной физической величине
void __fastcall TForm1::RadioPhysValClick(TObject *Sender)
{
	if (RadioPhysVal->ItemIndex == 0)
	{
		CheckBoxOtklon->Visible = true;
		RadioOtklonOm->Visible = true;
		RadioOtklonPercent->Visible = true;
		CheckCorrection->Visible = true;

		ComboBoxType->Visible = false;
		ComboBoxR0->Visible = false;
	}
	else
	{
		CheckBoxOtklon->Visible = false;
		RadioOtklonOm->Visible = false;
		RadioOtklonPercent->Visible = false;
		CheckCorrection->Visible = false;

		ComboBoxType->Visible = true;
		ComboBoxR0->Visible = true;
	}
}

void __fastcall TForm1::CheckBoxOtklonClick(TObject *Sender)
{
	if (CheckBoxOtklon->Checked)
	{
		if (FprofilePath == '\0')
		{
            CheckBoxOtklon->Checked = false;
			ShowMessage("Нет открытого профиля номиналов");
		}
		else
		{
			RadioOtklonOm->Enabled = true;
			RadioOtklonPercent->Enabled = true;
			CheckCorrection->Enabled = true;
        }
	}
	else
	{
		RadioOtklonOm->Enabled = false;
		RadioOtklonPercent->Enabled = false;
		CheckCorrection->Enabled = false;
    }
}

//активация поля ввода интервала времени при выборе автоматического режима
void __fastcall TForm1::RadioWorkModeClick(TObject *Sender)
{
	if (RadioWorkMode->ItemIndex == 0)
	{
		EditAuto->Enabled = false;
		LabelAuto->Enabled = false;
		LabelAuto2->Enabled = false;

		InpType->Items->Strings[1] = "Обычный";
	}
	else
	{
		EditAuto->Enabled = true;
		LabelAuto->Enabled = true;
		LabelAuto2->Enabled = true;

		InpType->Items->Strings[1] = "Инкремент";
	}
}

//отображение элементов, соответствующих выбранному методу ввода
void __fastcall TForm1::InpTypeChange(TObject *Sender)
{
	switch (InpType->ItemIndex)
	{
		case 0:
		    StandartInputGroup->Visible = false;
			ProfileInputGroup->Visible = true;
			IncrInputGroup->Visible = false;
			Timer->OnTimer = TimerProfile;
			break;
		case 1:
			if (RadioWorkMode->ItemIndex == 0)
			{
				StandartInputGroup->Visible = true;
				ProfileInputGroup->Visible = false;
				IncrInputGroup->Visible = false;
			}
			else
			{
				StandartInputGroup->Visible = false;
				ProfileInputGroup->Visible = false;
				IncrInputGroup->Visible = true;
				Timer->OnTimer = TimerIncr;
			}
			break;
	}
}

//---------------------------------------------------------------------------

//#############################
//ЗАГРУЗКА ПРОФИЛЕЙ ИЗ ФАЙЛОВ##
//#############################
//профиль номиналов во время работы хранится в двумерном массиве
void __fastcall TForm1::ButtonFProfileOpenClick(TObject *Sender)
{
	if (OpenDialog->Execute())
	{
		FprofilePath = new char[100];
		wcstombs(FprofilePath, OpenDialog->FileName.c_str(), 100);

		FILE* fl;
		fl = fopen(FprofilePath, "r+");

		char* tmp;
		tmp = new char[10];


		int cnt = 0;
		while (!feof(fl))
		{
			fscanf(fl, "%s", tmp);
			cnt++;
		}

		if (cnt != 64)
		{
			ShowMessage("Файл содержит некорректные данные");
			FprofilePath = '\0';
			fclose(fl);
			return;
		}
		rewind(fl);

		fscanf(fl, "%s", tmp);
		if (strlen(tmp) > 10)
		{
			ShowMessage("Файл содержит некорректные данные");
			FprofilePath = '\0';
			fclose(fl);
			return;
		}
		else
		try
		{
			FprofileR0 = StrToFloat(tmp);
		}
		catch(const EConvertError &excep)
		{
			ShowMessage("Файл содержит некорректные данные");
			FprofilePath = '\0';
			fclose(fl);
			return;
		}
		const char* postComma = strchr(tmp, ',');
		if (strlen(postComma) > 5)
		{
			ShowMessage("Файл содержит некорректные данные");
			FprofilePath = '\0';
			fclose(fl);
			return;
		}


		for (int i = 0; i < 7; i++)
			for (int j = 0; j < 9; j++)
			{
				fscanf(fl, "%s", tmp);
				if (strlen(tmp) > 10)
				{
					ShowMessage("Файл содержит некорректные данные");
					FprofilePath = '\0';
					fclose(fl);
					return;
				}
				else
				try
				{
					Fprofile[i][j] = StrToFloat(tmp);
				}
				catch(const EConvertError &excep)
				{
					ShowMessage("Файл содержит некорректные данные");
					FprofilePath = '\0';
					fclose(fl);
					return;
				}
				postComma = strchr(tmp, ',');
				if (strlen(postComma) > 5)
				{
					ShowMessage("Файл содержит некорректные данные");
					FprofilePath = '\0';
					fclose(fl);
					return;
				}
			}
		delete[] tmp;
		fclose(fl);
	}
}

//рабочий профиль во время работы хранится в листбоксе
void __fastcall TForm1::ProfileInpButtonOpenClick(TObject *Sender)
{
	  if (OpenDialog->Execute())
	  {
		char* path;
		path = new char[100];
		wcstombs(path, OpenDialog->FileName.c_str(), 100);
		profilePath = path;

		ProfileValuesList->Items->LoadFromFile(profilePath);


		for (int i = 0; i < ProfileValuesList->Items->Count; i++)
		{
			String tmp = ProfileValuesList->Items->Strings[i];
			if (tmp.Length() > 8)
			{
				ProfileValuesList->Clear();
				ShowMessage("Файл содержит некорректные данные");
				return;
			}
			else
			try
			{
				ProfileValuesList->ItemIndex = 0;
				StrToFloat(tmp);
			}
			catch(const EConvertError &excep)
			{
				ProfileValuesList->Clear();
				ShowMessage("Файл содержит некорректные данные");
				return;
			}
			if (tmp.Length() - tmp.Pos(",") > 2)
			{
				ProfileValuesList->Clear();
				ShowMessage("Файл содержит некорректные данные");
				return;
			}
		}

		delete[] path;
	  }
}


//вызов окон создания/редактирования профилей

//при редактировании в окно создания заносятся значения из открытого профиля
//при сохранении не появляется диалог с выбором файла/места,
//а происходит перезапись файла, указанного в profilePath или FProfilePath.

//профиль номиналов
void __fastcall TForm1::ButtonFProfileCreateClick(TObject *Sender)
{
	TForm3* Form3 = new TForm3(this);
	Form3->ShowModal();
	Form3->Free();
}

void __fastcall TForm1::ButtonFProfileEditClick(TObject *Sender)
{
	if (FprofilePath == '\0')
	{
		ShowMessage("Нет открытого профиля номиналов");
	}
	else
	{
		TForm3* Form3 = new TForm3(this, FprofilePath);

		//перенос значений в окно редактирования
		Form3->R0->Text = FloatToStr(FprofileR0);
		for(int i = 0; i < 7; i++)
				for (int j = 0; j < 9; j++)
					Form3->inputs[i][j]->Text = FloatToStr(Fprofile[i][j]);

		Form3->ShowModal();
		Form3->Free();
    }
}

//рабочий профиль
void __fastcall TForm1::ProfileInpButtonCreateClick(TObject *Sender)
{
   Application->CreateForm(__classid(TForm2), &Form2);
   Form2->ShowModal();
   Form2->Free();
}

void __fastcall TForm1::ProfileInpButtonChangeClick(TObject *Sender)
{
	if (FprofilePath == '\0')
	{
        ShowMessage("Нет открытого рабочего профиля");
	}
	else
	{
	   TForm2* Form2 = new TForm2(this, profilePath);

	   Form2->ListBox->Items = ProfileValuesList->Items;

	   Form2->ShowModal();
	   Form2->Free();
	}
}

//---------------------------------------------------------------------------

//#######################
//НАСТРОЙКА ПОЛЕЙ ВВОДА##
//#######################
//Ограничение на время. Нельзя установить более 3600.
void __fastcall TForm1::EditAutoKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (EditAuto->Text!= "")
	{
		int tmp = StrToInt(EditAuto->Text);
		if ((isdigit(Key)) && (tmp*10+StrToInt(Key) > 3600))
			Key = 0;
	}
}

//добавление незначащих нулей в дробную часть
void __fastcall TForm1::AddZero(TObject *Sender)
{
	TEdit *txtbox = static_cast<TEdit*>(Sender);
	AnsiString tmp = txtbox->Text;
	int comma = tmp.Pos(",");
	if (comma == 0)
	{
		tmp += ",00";
		txtbox->Text = tmp;
		txtbox->SelStart = tmp.Length();
	}
	else
	{
		int afterComma = tmp.Length() - comma;
		if (afterComma < 2)
			for (int i = afterComma; i < 2; i++)
				tmp += "0";
	}
	txtbox->Text = tmp;
}

//ограничение на ввод символов в поле входных данных:
//только цифры, не более 2х знаков после запятой
void __fastcall TForm1::EnterOm(TObject *Sender, System::WideChar &Key)
{
	TEdit *txtbox = static_cast<TEdit*>(Sender);
	AnsiString tmp = txtbox->Text;
	if (Key != 13)
	{
		if (isdigit(Key) || Key == 8 || Key == ',' || Key == '-')
		{
			if ((Key == '-') && !tmp.IsEmpty())
				Key = 0;

			if (Key == ',')
				if (tmp.IsEmpty())
				{
					tmp = "0";
					txtbox->Text = tmp;
					txtbox->SelStart=2;
				}
				else
				if (tmp.Pos(",") != 0)
					Key = 0;
			if ((tmp.Length() == 5) & (tmp.Pos(",") == 0) & (Key != ',') & (Key != 8))
			{
				tmp += ",";
				txtbox->Text = tmp;
				txtbox->SelStart = 6;
			}

			if (isdigit(Key) && (tmp.Pos(",") != 0))
				if (tmp.Length() - tmp.Pos(",") > 1)
					Key = 0;
		}
		else
			Key = 0;
	}
	else
	{
		Key = 0;
		if(txtbox->Name == "StandartInpEdit")
			StandartInpButtonEnterClick(this);
		else
		{
			AddZero(txtbox);
			//табуляция на след окно ввода
			SelectNext(txtbox, true, true);
		}
	}
}
//---------------------------------------------------------------------------


//####################################
//      МЕТОДЫ РЕЖИМОВ ВВОДА        ##
//ОБРАБОТКИ ЗНАЧЕНИЯ ПЕРЕД ОТПРАВКОЙ##
//   И ОТПРАВКИ ЗНАЧЕНИЯ В ПРИБОР   ##
//####################################




double TForm1::CalcNominal(double val)
{
		//нужно для удобного выделения цифры дробного разряда
		val *= 100;
		//делитель для выделения цифры определённого разряда
		double divider = 1000000;
		//цифра разряда
		int num;
		//величина с учётом отклонений
		double trueVal = FprofileR0;


		//проходим по введённому числу,
		//пробуя извлечь каждый возможный разряд, начиная со старшего
		//если разряда нет (num == 0) пробуем извлечь предыдущий разряд
		//в таблице находим фактическое значение, соответствующее значению разряда
		//(num используется как индекс для адресации в массиве профиля фактич. значений)
		//прибавляем значение из массива к trueVal


		for (int i = 0; i < 7; i++)
		{
			//выделение цифры разряда
			num = fmod(val, 10*divider);
			num = floor(num / divider);
			//если есть цифра, обращение к массиву фактических значений
			if (num != 0)
				trueVal += Fprofile[i][num-1];
			//уменьшение делителя для выделения предыдущего разряда
			divider /= 10;
		}
		return trueVal;
}

String TForm1::CalcOtkl(String str)
 {
		double val = StrToFloat(str);

		double trueVal = CalcNominal(val);

		//когда trueVal сформирован, считаем отклонение от введённого значения
		double otklon = trueVal - val;

		if (RadioOtklonOm->Checked)
		{
			Info->Lines->Add("Отклонение = " + FormatFloat("0.0000",otklon) + " Ом");
			if ((RadioWorkMode->ItemIndex == 0) &&(InpType->ItemIndex == 1))
				StandartInpOtklon->Caption = FormatFloat("0.0000",otklon) + " Ом";

		}

		//отклонение в процентах
		else if (RadioOtklonPercent->Checked)
		{
			otklon /= trueVal / 100;
			Info->Lines->Add("Отклонение = " + FormatFloat("0.00000",otklon) + "%");
			if ((RadioWorkMode->ItemIndex == 0) &&(InpType->ItemIndex == 1))
				StandartInpOtklon->Caption = FormatFloat("0.00000",otklon) + "%";
		}
		return otklon;
 }

String TForm1::CorrectVal(String str)
 {
	double val = StrToFloat(str);
	double nom = CalcNominal(val);
	double dif = nom - val;

	double nomDown = CalcNominal(val - 0.01);
	double nomUp = CalcNominal(val + 0.01);
	double difDown = val - nomDown;
	double difUp = val - nomUp;

	if ((fabs(difDown) < fabs(dif)) || (fabs(difUp) < fabs(dif)))
	{
		if (fabs(difDown) < fabs(difUp))
		{
			//циклично продолжить занижение, пока не будет достигнуто наименьшее отклонение
			double corrected = val - 0.01;
			double newDif = difDown;
			double newnom = nomDown;
			while (fabs(newDif) <= fabs(dif))
			{
				corrected -= 0.01;
				nom = newnom;
				dif = newDif;
				newnom = CalcNominal(corrected);
				newDif = val - newnom;
			}
			return FloatToStr(corrected + 0.01);
		}
		else
		{
			//циклично продолжить завышение, пока не будет достигнуто наименьшее отклонение
			double corrected = val + 0.01;
			double newDif = difUp;
			double newnom = nomUp;
			while (fabs(newDif) <= fabs(dif))
			{
				corrected += 0.01;
				nom = newnom;
				dif = newDif;
				newnom = CalcNominal(corrected);
				newDif = val - newnom;
			}
			return FloatToStr(corrected - 0.01);
		}
    }
	else
		return FloatToStr(-1);
 }

// 'C' <значение> 'B'
void TForm1::SendToDevice(String val)
{
	//signalI = false;

	//если галочки висят, выводим отклонение и/или подогнанное значение
	Form1->Info->Lines->Add(val);
	if (CheckBoxOtklon->Checked)
		CalcOtkl(val);
	if (CheckCorrection->Checked)
	{
		String corrVal = CorrectVal(val);
		if (corrVal != "-1")
		{
			val = corrVal;
			Form1->Info->Lines->Add("Значение с поправкой: " + val);
		}
		else
			Form1->Info->Lines->Add("Значение с поправкой: поправка не требуется.");
	}
	Form1->Info->Lines->Add("-**--**-");

 //	memset(bufwr,0,BUFSIZE); //очистить программный передающий буфер,
			  //чтобы данные не накладывались друг на друга
 //	PurgeComm(COMport, PURGE_TXCLEAR);
	strcpy(bufwr, "C");     //занести в программный передающий буфер строку
	ResumeThread(writer);               //активировать поток записи данных в порт
	Sleep(95);

	memset(bufwr,0,BUFSIZE);
	PurgeComm(COMport, PURGE_TXCLEAR);
	char* cVal = new char[25];
	wcstombs(cVal, val.c_str(), 25);
	strcpy(bufwr, cVal);
	ResumeThread(writer);
	Sleep(95);



//	memset(bufwr,0,BUFSIZE);
//	PurgeComm(COMport, PURGE_TXCLEAR);
	strcpy(bufwr, "B");
	ResumeThread(writer);
}
//---------------------------------------------------------------------------

//МЕТОДЫ СТАНДАРТНОГО ВВОДА

void __fastcall TForm1::StandartInpButtonDelClick(TObject *Sender)
{
	StandartInpEdit->Text = "";
}

void __fastcall TForm1::StandartInpButtonEnterClick(TObject *Sender)
{
    AddZero(StandartInpEdit);

	//добавление значения в окно истории ввода
	StandartInpList->Items->Add(StandartInpEdit->Text);

	//отправка в прибор
	SendToDevice(StandartInpEdit->Text);
}

void __fastcall TForm1::StandartInpButtonSaveAsProfileClick(TObject *Sender)
{
	if (SaveDialog->Execute())
	{
		char* path;
		path = new char[100];
		wcstombs(path, SaveDialog->FileName.c_str(), 100);

		StandartInpList->Items->SaveToFile(path);

		delete[] path;
		String defaultName = "Новый профиль.txt";
		SaveDialog->FileName = defaultName.c_str();

		ShowMessage("Done!");
	}
}

//---------------------------------------------------------------------------

//ИНКРЕМЕНТНЫЙ/ДЕКРЕМЕНТНЫЙ ВВОД

bool TForm1::checkIncVals()
{
	incrStrt = StrToFloat(IncInpEditStart->Text);
	incrEnd = StrToFloat(IncInpEditEnd->Text);
	incrStep = StrToFloat(IncInpEditStep->Text);

	if (incrStep == 0 || incrStrt == incrEnd)
		return false;
	if (((incrStep > 0) && (incrStrt > incrEnd))
		||
		((incrStep < 0) && (incrStrt < incrEnd)))
		return false;

	if (IncInpEditStart->Text.Pos(",") == 0)
	{
		String tmp = IncInpEditStart->Text + ",00";
		IncInpEditStart->Text = tmp;
	}

	if (IncInpEditEnd->Text.Pos(",") == 0)
	{
		String tmp = IncInpEditEnd->Text + ",00";
		IncInpEditEnd->Text = tmp;
	}

	if (IncInpEditStep->Text.Pos(",") == 0)
	{
		String tmp = IncInpEditStep->Text + ",00";
		IncInpEditStep->Text = tmp;
	}

	return true;
}

void __fastcall TForm1::IncrInpButtonEnterClick(TObject *Sender)
{
    if (EditAuto->Text.IsEmpty())
	{
		ShowMessage("Интервал времени не указан!");
		return;
	}

	//инициализация переменных и проверка на корректность
	if (!checkIncVals())
	{
		ShowMessage("Параметры ввода заданы некорректно!");
		return;
	}
	else
	if (IncrInpButtonEnter->Caption == "Старт")
	{
		Timer->Interval = StrToInt(EditAuto->Text)*1000;

		//если отправка 1го значения
		if ((incrNextVal == 0,0001))
		{
			//инициализация значений переменных

			incrNextVal = incrStrt + incrStep;

			//вывод следующего вводимого значения
			IncrInpNextVal->Caption = FloatToStr(incrNextVal);

				IncInpEditStart->Enabled = false;
				IncInpEditEnd->Enabled = false;
				IncInpEditStep->Enabled = false;
				IncrInpButtonEnter->Caption = "Стоп";

				autoinp = true;
				SendToDevice(IncInpEditStart->Text);
		}
	}
	else
	{
		IncInpEditStart->Enabled = true;
		IncInpEditEnd->Enabled = true;
		IncInpEditStep->Enabled = true;

        autoinp = false;
		Timer->Enabled = false;
		IncrInpButtonEnter->Caption = "Старт";
		IncrInpNextVal->Caption = "--";
	}
}

void __fastcall TForm1::TimerIncr(TObject *Sender)
{
	Timer->Enabled = false;
	SendToDevice(FloatToStr(incrNextVal));

	if (incrNextVal != incrEnd)
	{
		incrNextVal += incrStep;

		if(((incrStep > 0) && (incrEnd < incrNextVal))
			||
			((incrStep < 0) && (incrEnd > incrNextVal)))
		{
			IncrInpNextVal->Caption = FloatToStr(incrEnd);
			incrNextVal = incrEnd;
		}
		else
			IncrInpNextVal->Caption = FloatToStr(incrNextVal);


	}
	else
	{
        IncInpEditStart->Enabled = true;
		IncInpEditEnd->Enabled   = true;
		IncInpEditStep->Enabled = true;

		autoinp = false;
		IncrInpButtonEnter->Caption = "Старт";
		IncrInpNextVal->Caption = "--";
    }
}
//---------------------------------------------------------------------------

//ВВОД ЧЕРЕЗ ПРОФИЛЬ

void __fastcall TForm1::TimerProfile(TObject *Sender)
{
	Timer->Enabled = false;

	int pos = ProfileValuesList->ItemIndex;
	int cnt = ProfileValuesList->Items->Count - 1;

	String tmp = ProfileValuesList->Items->Strings[pos];
	SendToDevice(tmp);

	if (pos == cnt)
	{
		autoinp = false;
		ProfileInpButtonEnter->Caption = "Старт";
		ProfileValuesList->ItemIndex = 0;
	}
	else
		ProfileValuesList->ItemIndex++;
}

void __fastcall TForm1::ProfileInpButtonEnterClick(TObject *Sender)
{
	//ручной режим ввода
	if (RadioWorkMode->ItemIndex == 0)
	{
		TimerProfile(this);
	}
	//автоматический
	else
	if (EditAuto->Text.IsEmpty())
	{
		ShowMessage("Интервал времени не указан!");
		return;
	}
	else
	if (!Timer->Enabled)
	{
		Timer->Interval = StrToInt(EditAuto->Text)*1000;
		autoinp = true;
		ProfileInpButtonEnter->Caption = "Стоп";
		TimerProfile(this);
	}
	else
	{
		Timer->Enabled = false;
		ProfileInpButtonEnter->Caption = "Старт";
	}
}
//---------------------------------------------------------------------------

//###################################
//СОЕДИНЕНИЕ С ПРИБОРОМ И НАСТРОЙКА##
//###################################

void __fastcall TForm1::ButtonDiagnosticClick(TObject *Sender)
{

	//коды ошибок:
	//аХХ - неисправени датчик ХХ

	//а00 - ошибок нет
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ConnectionButtonClick(TObject *Sender)
{
	if (ConnectionButton->Caption == "Подключение к устройству")
		FindDevice(devID);
	else
	{
		strcpy(bufwr,"E");
		ResumeThread(writer);
		ClosePort();
		ConnectionButton->Caption = "Подключение к устройству";
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ButtonFProfileSendClick(TObject *Sender)
{
	if(FprofilePath = '\0')
	{
	  ShowMessage("Нет открытого профиля номиналов");
	  return;
    }

	memset(bufwr,0,BUFSIZE);
	PurgeComm(COMport, PURGE_TXCLEAR);
	strcpy(bufwr, "S");
	ResumeThread(writer);
	Sleep(600);

    for(int i = 6; i > -1; i--)
		for (int j = 0; j < 9; j++)
		{
			SendToDevice(FloatToStr(Fprofile[i][j]));
			Sleep(600);
		}
	memset(bufwr,0,BUFSIZE);
	PurgeComm(COMport, PURGE_TXCLEAR);
	strcpy(bufwr, "K");
	ResumeThread(writer);
   //	signalI = true;
	Info->Lines->Add("Профиль номиналов успешно записан.");
}
//---------------------------------------------------------------------------


//*********************************************************************

int handle;

void ReadBuf()
{

	String rdt = (UnicodeString)(char*)bufrd;


	if (*bufrd == *ID)
	{
        connected = true;
		strcpy(bufwr,"D");
		ResumeThread(writer);
		Form1->Info->Lines->Add("Устройство подключено!");
		Form1->ConnectionButton->Caption = "Отключение устройства";
	}

	if(*bufrd=='I')
	{
        strcpy(bufwr,"I");
		ResumeThread(writer);
	}
	else
	{
        Form1->Info->Lines->Add("##BUFRD##");
		Form1->Info->Lines->Add(rdt);
		Form1->Info->Lines->Add("########");

    }
	if( *bufrd=='B')
	{

		if (autoinp) {
		//	if (Form1->Timer->Interval > 4999)
        //    	signalI = true;
            Form1->Timer->Enabled = true;
		}
	  //	else
	  //		signalI = true;
		ResumeThread(writer);
	}

	memset(bufrd, 0, BUFSIZE);	        //очистить буфер
}

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
			 ReadBuf(); //вызываем функцию для вывода данных на экран и в файл
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
		 ;;	//вывод сообщения об успешной установке
		}
		else
		{
			Form1->Info->Lines->Add("Значение не отправлено");
		}
    	SuspendThread(writer);
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

 connected = false;
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
}

void OpenPort(String portname)
{
//	 String portname     	//имя порта (например, "COM1", "COM2" и т.д.)
	 DCB dcb;           	//структура для общей инициализации порта DCB
	 COMMTIMEOUTS timeouts;  	//структура для установки таймаутов

//	 portname = ;	//получить имя выбранного порта

	 //открыть порт, для асинхронных операций обязательно нужно указать флаг FILE_FLAG_OVERLAPPED
	char* port;
	port = new char[15];
	wcstombs(port, portname.c_str(), 15);

	COMport = CreateFile(port, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	 //здесь:
	 // - port - имя порта в качестве имени файла,
	 // - GENERIC_READ | GENERIC_WRITE - доступ к порту на чтение/записть
	 // - 0 - порт не может быть общедоступным (shared)
	 // - NULL - дескриптор порта не наследуется, используется дескриптор безопасности по умолчанию
	 // - OPEN_EXISTING - порт должен открываться как уже существующий файл
	 // - FILE_FLAG_OVERLAPPED - этот флаг указывает на использование асинхронных операций
	 // - NULL - указатель на файл шаблона не используется при работе с портами

	 if(COMport == INVALID_HANDLE_VALUE)            //если ошибка открытия порта
	 {
// 	   Form1->Info->Lines->Add(GetLastError());
	   return;
	 }

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
	 dcb.BaudRate = 9600; //задаём скорость передачи
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
	 timeouts.ReadIntervalTimeout = 1;	 	//таймаут между двумя символами
	 timeouts.ReadTotalTimeoutMultiplier = 1;	//общий таймаут операции чтения
	 timeouts.ReadTotalTimeoutConstant = 1;       //константа для общего таймаута операции чтения
	 timeouts.WriteTotalTimeoutMultiplier = 1;    //общий таймаут операции записи
	 timeouts.WriteTotalTimeoutConstant = 1;      //константа для общего таймаута операции записи

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


	 PurgeComm(COMport, PURGE_RXCLEAR);	//очистить принимающий буфер порта

 //  	 EscapeCommFunction(COMport, SETDTR);
 //	 EscapeCommFunction(COMport, SETRTS);

	 reader = CreateThread(NULL, 0, ReceiveFromDeviceThread, NULL, 0, NULL);  //создать и запустить поток чтения байтов
	 writer = CreateThread(NULL, 0, SendToDeviceThread, NULL, CREATE_SUSPENDED, NULL);	//создаём поток записи в остановленном состоянии (предпоследний параметр = CREATE_SUSPENDED)
}

//функция поиска и подключения устройства
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
		Form1->Info->Lines->Add("Устройство не найдено!!!");
	}
}
