//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "main.h"
#include "FProfileEdit.h"
#include "ProfileEdit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	//������������� ������� ��� �������� ������� ���������
	Fprofile = new double*[7];
	for(size_t i = 0; i < 7; ++i)
		Fprofile[i] = new double[9];

	//������������� ����� � ������������ ������ ��������
	FprofilePath = new char;
	FprofilePath = '\0';
	profilePath = '\0';

	//������������� ���������� �������� ��� �������������/������������� �����
	//������ ������������ ����������, ����� ����������,
	//�������� �� ������� ����� ��������� �������� ������
	incrStrt = 0,001;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
	for(size_t i = 0; i < 7; ++i)
		delete [] Fprofile[i];
	delete [] Fprofile;
	delete [] FprofilePath;
}
//---------------------------------------------------------------------------

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
		RadioOtklonOm->Enabled = true;
		RadioOtklonPercent->Enabled = true;
		CheckCorrection->Enabled = true;
	}
	else
	{
		RadioOtklonOm->Enabled = false;
		RadioOtklonPercent->Enabled = false;
		CheckCorrection->Enabled = false;
    }
}
//---------------------------------------------------------------------------

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

//---------------------------------------------------------------------------

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

//�������� �������� �� ������

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
		tmp = new char[12];

		fscanf(fl, "%s", tmp);
		FprofileR0 = StrToFloat(tmp);

		for(int i = 0; i < 7; i++)
			for (int j = 0; j < 9; j++)
			{
				fscanf(fl, "%s", tmp);
				Fprofile[i][j] = StrToFloat(tmp);
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
		ProfileValuesList->ItemIndex = 0;
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

//����������� �� ���� �������� � ���� ������� ������:
//������ �����, �� ����� 2� ������ ����� �������
void __fastcall TForm1::EnterOm(TObject *Sender, System::WideChar &Key)
{
	TEdit *txtbox = static_cast<TEdit*>(Sender);
	AnsiString tmp = txtbox->Text;
	if (Key != 13) {
		if (isdigit(Key) || Key == 8 || Key == ',')
		{
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
		if (tmp.Pos(",") == 0)
		{
			tmp += ",00";
			txtbox->Text = tmp;
			txtbox->SelStart = tmp.Length();
		}
		if(txtbox->Name == "StandartInpEdit")
			SendToDevice(StrToFloat(tmp));
		else
			//��������� �� ���� ���� �����
			SelectNext(txtbox, true, true);
	}
}

//---------------------------------------------------------------------------


//�������� �������� ���������

//���������� ���������� ���������������� ��������
//double TForm1::CalcTrueVal(double val)
  /*
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
  */
String TForm1::SendToDevice(double val)
{
 	//���� ����� ��������� ����������
	if (CheckBoxOtklon->Checked)
	{
		if (FprofilePath == '\0')
		{
			ShowMessage("��� ��������� ������� ���������");
			return "error";
		}
		else
		{
			//����� ��� �������� ��������� ����� �������� �������
			val *= 100;
            //�������� ��� ��������� ����� ������������ �������
			double divider = 1000000;
            //����� �������
			int num;
			//�������� � ������ ����������
			double trueVal = 0;

			//##���������� ���������� �� ��������##
			//#####################################
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

			//����� trueVal �����������, ������� ���������� �� ��������� ��������
			double otklon = val / 100 - trueVal;

			if (RadioOtklonOm->Checked)
				Info->Lines->Add("���������� = " + FormatFloat("0.0000",otklon));

            //���������� � ���������
			else if (RadioOtklonPercent->Checked)
			{
				otklon /= trueVal / 100;
				Info->Lines->Add("���������� = " + FormatFloat("0.00000",otklon) + "%");
			}

			//�������� � ��������
			if (CheckCorrection->Checked)
			{
				val = val /100 + otklon;			
				Info->Lines->Add("�������� � ��������� = " + FormatFloat("0.00",val));
			}
		}
	}
	else
	{
    	Info->Lines->Add(FloatToStr(val));
    }
	//��������������� ��������
}
//---------------------------------------------------------------------------

//������ ������������ �����
void __fastcall TForm1::StandartInpButtonDelClick(TObject *Sender)
{
	StandartInpEdit->Text = "";
}

void __fastcall TForm1::StandartInpButtonEnterClick(TObject *Sender)
{
	//���������� �������� � ���� ������� �����
	StandartInpList->Items->Add(StandartInpEdit->Text);

	//�������� � ������
	SendToDevice(StrToFloat(StandartInpEdit->Text));
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
void __fastcall TForm1::IncrInpButtonEnterClick(TObject *Sender)
{
	if (!Timer->Enabled)
	{
		Timer->Interval = StrToInt(EditAuto->Text)*1000;

		//���� �������� 1�� ��������
		if ((incrStrt == 0,001))
		{
			//������������� �������� ����������
			incrStrt = StrToFloat(IncInpEditStart->Text);
			incrEnd = StrToFloat(IncInpEditEnd->Text);
			incrStep = StrToFloat(IncInpEditStep->Text);
			incrNextVal = incrStrt + incrStep;

			//����� ���������� ��������� ��������
			IncrInpNextVal->Caption = FloatToStr(incrNextVal);

			if (SendToDevice(incrStrt) != "error")
			{
				IncInpEditStart->Enabled = false;
				IncInpEditEnd->Enabled = false;
				IncInpEditStep->Enabled = false;

				IncrInpButtonEnter->Caption = "����";
				Timer->Enabled = true;
			}
		}
	}
	else
	{
		IncInpEditStart->Enabled = true;
		IncInpEditEnd->Enabled = true;
		IncInpEditStep->Enabled = true;

		Timer->Enabled = false;
		IncrInpButtonEnter->Caption = "�����";
		IncrInpNextVal->Caption = "--";
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TimerIncr(TObject *Sender)
{
	SendToDevice(incrNextVal);
	if (incrNextVal != incrEnd)
	{
		incrNextVal += incrStep;
		IncrInpNextVal->Caption = FloatToStr(incrNextVal);
	}
	else
	{
        IncInpEditStart->Enabled = true;
		IncInpEditEnd->Enabled = true;
		IncInpEditStep->Enabled = true;

		Timer->Enabled = false;
		IncrInpButtonEnter->Caption = "�����";
		IncrInpNextVal->Caption = "--";
    }
}
//---------------------------------------------------------------------------
//���� ����� �������
void __fastcall TForm1::TimerProfile(TObject *Sender)
{
	String tmp;
	int pos = ProfileValuesList->ItemIndex;
	int cnt = ProfileValuesList->Items->Count - 1;

	if (pos == cnt)
	{
		tmp = ProfileValuesList->Items->Strings[pos];
		SendToDevice(StrToFloat(tmp));
		Timer->Enabled = false;
		ProfileInpButtonEnter->Caption = "�����";
		ProfileValuesList->ItemIndex = 0;
	}
	else
	{
		tmp = ProfileValuesList->Items->Strings[pos];
		SendToDevice(StrToFloat(tmp));
		ProfileValuesList->ItemIndex++;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ProfileInpButtonEnterClick(TObject *Sender)
{
	//������ ����� �����
	if (RadioWorkMode->ItemIndex == 0)
	{
		TimerProfile(this);
	}
	//��������������
	else
	if (!Timer->Enabled)
	{
		Timer->Interval = StrToInt(EditAuto->Text)*1000;
		Timer->Enabled = true;
		ProfileInpButtonEnter->Caption = "����";
	}
	else
	{
		Timer->Enabled = false;
		ProfileInpButtonEnter->Caption = "�����";
	}
}
//---------------------------------------------------------------------------


void __fastcall TForm1::ButtonDiagnosticClick(TObject *Sender)
{

	//���� ������:
	//��� - ����������� ������ ��

	//�00 - ������ ���
}
//---------------------------------------------------------------------------
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
//---------------------------------------------------------------------------

