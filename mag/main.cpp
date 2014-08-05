//---------------------------------------------------------------------------

#include <vcl.h>
#include <string.h>
#include <stdio.h>
#pragma hdrstop

#include "main.h"
#include "FProfileEdit.h"
#include "ProfileEdit.h"

#include <io.h>         //��� ������ � �������
#include <fcntl.h>      //��� ������ � �������
#include <sys\stat.h>   //��� ������ � �������
//****************************
#define BUFSIZE 25

unsigned char bufrd[BUFSIZE], bufwr[BUFSIZE]; //������� � ���������� ������
unsigned char bufI[1];

HANDLE COMport;		//���������� �����

HANDLE reader;	//���������� ������ ������ �� �����
HANDLE writer;	//���������� ������ ������ � ����

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
	//������ ��� ����������� ���������� ���������� (�� ��������� ��3070��)
	devID = new char [1];
	devID = "M";

	//������������� ������� ��� �������� ������� ���������
	Fprofile = new double*[7];
	for(size_t i = 0; i < 7; ++i)
		Fprofile[i] = new double[9];

	//������������� ����� � ������������ ������ ��������
	FprofilePath = new char;
	FprofilePath = '\0';
	profilePath = '\0';

	//������������� ������ �������� ��� �������������/������������� �����
	//������ ������������ ����������, ����� ����������,
	//�������� �� ������� ����� ��������� �������� ������
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

	if(ConnectionButton->Caption != "����������� � ����������")
	{
		strcpy(bufwr,"E");
		ResumeThread(writer);
		ClosePort();
    }

}
//---------------------------------------------------------------------------

//########################################
//��������� �������� ����� � �����������##
//########################################

//����� �������������� � ����������� �� ���������� ����������
void __fastcall TForm1::RadioDeviceClick(TObject *Sender)
{
	if (RadioDevice->ItemIndex == 0)
		devID = "M";
	else
		devID = "K";
}

//����������� ��������, ��������������� ��������� ���������� ��������
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
			ShowMessage("��� ��������� ������� ���������");
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

//��������� ���� ����� ��������� ������� ��� ������ ��������������� ������
void __fastcall TForm1::RadioWorkModeClick(TObject *Sender)
{
	if (RadioWorkMode->ItemIndex == 0)
	{
		EditAuto->Enabled = false;
		LabelAuto->Enabled = false;
		LabelAuto2->Enabled = false;

		InpType->Items->Strings[1] = "�������";
	}
	else
	{
		EditAuto->Enabled = true;
		LabelAuto->Enabled = true;
		LabelAuto2->Enabled = true;

		InpType->Items->Strings[1] = "���������";
	}
}

//����������� ���������, ��������������� ���������� ������ �����
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
//�������� �������� �� ������##
//#############################
//������� ��������� �� ����� ������ �������� � ��������� �������
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
			ShowMessage("���� �������� ������������ ������");
			FprofilePath = '\0';
			fclose(fl);
			return;
		}
		rewind(fl);

		fscanf(fl, "%s", tmp);
		if (strlen(tmp) > 10)
		{
			ShowMessage("���� �������� ������������ ������");
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
			ShowMessage("���� �������� ������������ ������");
			FprofilePath = '\0';
			fclose(fl);
			return;
		}
		const char* postComma = strchr(tmp, ',');
		if (strlen(postComma) > 5)
		{
			ShowMessage("���� �������� ������������ ������");
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
					ShowMessage("���� �������� ������������ ������");
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
					ShowMessage("���� �������� ������������ ������");
					FprofilePath = '\0';
					fclose(fl);
					return;
				}
				postComma = strchr(tmp, ',');
				if (strlen(postComma) > 5)
				{
					ShowMessage("���� �������� ������������ ������");
					FprofilePath = '\0';
					fclose(fl);
					return;
				}
			}
		delete[] tmp;
		fclose(fl);
	}
}

//������� ������� �� ����� ������ �������� � ���������
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
				ShowMessage("���� �������� ������������ ������");
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
				ShowMessage("���� �������� ������������ ������");
				return;
			}
			if (tmp.Length() - tmp.Pos(",") > 2)
			{
				ProfileValuesList->Clear();
				ShowMessage("���� �������� ������������ ������");
				return;
			}
		}

		delete[] path;
	  }
}


//����� ���� ��������/�������������� ��������

//��� �������������� � ���� �������� ��������� �������� �� ��������� �������
//��� ���������� �� ���������� ������ � ������� �����/�����,
//� ���������� ���������� �����, ���������� � profilePath ��� FProfilePath.

//������� ���������
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
		ShowMessage("��� ��������� ������� ���������");
	}
	else
	{
		TForm3* Form3 = new TForm3(this, FprofilePath);

		//������� �������� � ���� ��������������
		Form3->R0->Text = FloatToStr(FprofileR0);
		for(int i = 0; i < 7; i++)
				for (int j = 0; j < 9; j++)
					Form3->inputs[i][j]->Text = FloatToStr(Fprofile[i][j]);

		Form3->ShowModal();
		Form3->Free();
    }
}

//������� �������
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
        ShowMessage("��� ��������� �������� �������");
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
//��������� ����� �����##
//#######################
//����������� �� �����. ������ ���������� ����� 3600.
void __fastcall TForm1::EditAutoKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (EditAuto->Text!= "")
	{
		int tmp = StrToInt(EditAuto->Text);
		if ((isdigit(Key)) && (tmp*10+StrToInt(Key) > 3600))
			Key = 0;
	}
}

//���������� ���������� ����� � ������� �����
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

//����������� �� ���� �������� � ���� ������� ������:
//������ �����, �� ����� 2� ������ ����� �������
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
			//��������� �� ���� ���� �����
			SelectNext(txtbox, true, true);
		}
	}
}
//---------------------------------------------------------------------------


//####################################
//      ������ ������� �����        ##
//��������� �������� ����� ���������##
//   � �������� �������� � ������   ##
//####################################




double TForm1::CalcNominal(double val)
{
		//����� ��� �������� ��������� ����� �������� �������
		val *= 100;
		//�������� ��� ��������� ����� ������������ �������
		double divider = 1000000;
		//����� �������
		int num;
		//�������� � ������ ����������
		double trueVal = FprofileR0;


		//�������� �� ��������� �����,
		//������ ������� ������ ��������� ������, ������� �� ��������
		//���� ������� ��� (num == 0) ������� ������� ���������� ������
		//� ������� ������� ����������� ��������, ��������������� �������� �������
		//(num ������������ ��� ������ ��� ��������� � ������� ������� ������. ��������)
		//���������� �������� �� ������� � trueVal


		for (int i = 0; i < 7; i++)
		{
			//��������� ����� �������
			num = fmod(val, 10*divider);
			num = floor(num / divider);
			//���� ���� �����, ��������� � ������� ����������� ��������
			if (num != 0)
				trueVal += Fprofile[i][num-1];
			//���������� �������� ��� ��������� ����������� �������
			divider /= 10;
		}
		return trueVal;
}

String TForm1::CalcOtkl(String str)
 {
		double val = StrToFloat(str);

		double trueVal = CalcNominal(val);

		//����� trueVal �����������, ������� ���������� �� ��������� ��������
		double otklon = trueVal - val;

		if (RadioOtklonOm->Checked)
		{
			Info->Lines->Add("���������� = " + FormatFloat("0.0000",otklon) + " ��");
			if ((RadioWorkMode->ItemIndex == 0) &&(InpType->ItemIndex == 1))
				StandartInpOtklon->Caption = FormatFloat("0.0000",otklon) + " ��";

		}

		//���������� � ���������
		else if (RadioOtklonPercent->Checked)
		{
			otklon /= trueVal / 100;
			Info->Lines->Add("���������� = " + FormatFloat("0.00000",otklon) + "%");
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
			//�������� ���������� ���������, ���� �� ����� ���������� ���������� ����������
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
			//�������� ���������� ���������, ���� �� ����� ���������� ���������� ����������
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

// 'C' <��������> 'B'
void TForm1::SendToDevice(String val)
{
	//signalI = false;

	//���� ������� �����, ������� ���������� �/��� ����������� ��������
	Form1->Info->Lines->Add(val);
	if (CheckBoxOtklon->Checked)
		CalcOtkl(val);
	if (CheckCorrection->Checked)
	{
		String corrVal = CorrectVal(val);
		if (corrVal != "-1")
		{
			val = corrVal;
			Form1->Info->Lines->Add("�������� � ���������: " + val);
		}
		else
			Form1->Info->Lines->Add("�������� � ���������: �������� �� ���������.");
	}
	Form1->Info->Lines->Add("-**--**-");

 //	memset(bufwr,0,BUFSIZE); //�������� ����������� ���������� �����,
			  //����� ������ �� ������������� ���� �� �����
 //	PurgeComm(COMport, PURGE_TXCLEAR);
	strcpy(bufwr, "C");     //������� � ����������� ���������� ����� ������
	ResumeThread(writer);               //������������ ����� ������ ������ � ����
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

//������ ������������ �����

void __fastcall TForm1::StandartInpButtonDelClick(TObject *Sender)
{
	StandartInpEdit->Text = "";
}

void __fastcall TForm1::StandartInpButtonEnterClick(TObject *Sender)
{
    AddZero(StandartInpEdit);

	//���������� �������� � ���� ������� �����
	StandartInpList->Items->Add(StandartInpEdit->Text);

	//�������� � ������
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
		String defaultName = "����� �������.txt";
		SaveDialog->FileName = defaultName.c_str();

		ShowMessage("Done!");
	}
}

//---------------------------------------------------------------------------

//������������/������������ ����

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
		ShowMessage("�������� ������� �� ������!");
		return;
	}

	//������������� ���������� � �������� �� ������������
	if (!checkIncVals())
	{
		ShowMessage("��������� ����� ������ �����������!");
		return;
	}
	else
	if (IncrInpButtonEnter->Caption == "�����")
	{
		Timer->Interval = StrToInt(EditAuto->Text)*1000;

		//���� �������� 1�� ��������
		if ((incrNextVal == 0,0001))
		{
			//������������� �������� ����������

			incrNextVal = incrStrt + incrStep;

			//����� ���������� ��������� ��������
			IncrInpNextVal->Caption = FloatToStr(incrNextVal);

				IncInpEditStart->Enabled = false;
				IncInpEditEnd->Enabled = false;
				IncInpEditStep->Enabled = false;
				IncrInpButtonEnter->Caption = "����";

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
		IncrInpButtonEnter->Caption = "�����";
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
		IncrInpButtonEnter->Caption = "�����";
		IncrInpNextVal->Caption = "--";
    }
}
//---------------------------------------------------------------------------

//���� ����� �������

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
		ProfileInpButtonEnter->Caption = "�����";
		ProfileValuesList->ItemIndex = 0;
	}
	else
		ProfileValuesList->ItemIndex++;
}

void __fastcall TForm1::ProfileInpButtonEnterClick(TObject *Sender)
{
	//������ ����� �����
	if (RadioWorkMode->ItemIndex == 0)
	{
		TimerProfile(this);
	}
	//��������������
	else
	if (EditAuto->Text.IsEmpty())
	{
		ShowMessage("�������� ������� �� ������!");
		return;
	}
	else
	if (!Timer->Enabled)
	{
		Timer->Interval = StrToInt(EditAuto->Text)*1000;
		autoinp = true;
		ProfileInpButtonEnter->Caption = "����";
		TimerProfile(this);
	}
	else
	{
		Timer->Enabled = false;
		ProfileInpButtonEnter->Caption = "�����";
	}
}
//---------------------------------------------------------------------------

//###################################
//���������� � �������� � ���������##
//###################################

void __fastcall TForm1::ButtonDiagnosticClick(TObject *Sender)
{

	//���� ������:
	//��� - ����������� ������ ��

	//�00 - ������ ���
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ConnectionButtonClick(TObject *Sender)
{
	if (ConnectionButton->Caption == "����������� � ����������")
		FindDevice(devID);
	else
	{
		strcpy(bufwr,"E");
		ResumeThread(writer);
		ClosePort();
		ConnectionButton->Caption = "����������� � ����������";
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ButtonFProfileSendClick(TObject *Sender)
{
	if(FprofilePath = '\0')
	{
	  ShowMessage("��� ��������� ������� ���������");
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
	Info->Lines->Add("������� ��������� ������� �������.");
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
		Form1->Info->Lines->Add("���������� ����������!");
		Form1->ConnectionButton->Caption = "���������� ����������";
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
