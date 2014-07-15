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





}
//---------------------------------------------------------------------------

//����������� ��������, ��������������� ��������� ���������� ��������
void __fastcall TForm1::RadioPhysValClick(TObject *Sender)
{
	if (RadioPhysVal->ItemIndex == 0) {
		CheckBoxOtklon->Visible = true;
		RadioOtklonOm->Visible = true;
		RadioOtklonPercent->Visible = true;
		CheckCorrection->Visible = true;

		ComboBoxType->Visible = false;
		ComboBoxR0->Visible = false;
	}
	else {
		CheckBoxOtklon->Visible = false;
		RadioOtklonOm->Visible = false;
		RadioOtklonPercent->Visible = false;
		CheckCorrection->Visible = false;

		ComboBoxType->Visible = true;
		ComboBoxR0->Visible = true;
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
		if (isdigit(Key) || Key == 8 || Key == ','){
			if (Key == ',')
				if (tmp.IsEmpty()){
					tmp = "0";
					txtbox->Text = tmp;
					txtbox->SelStart=2;
				}
				else
				if (tmp.Pos(",") != 0)
					Key = 0;
			if ((tmp.Length() == 5) & (tmp.Pos(",") == 0) & (Key != ',') & (Key != 8)){
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
	else{
		if (tmp.Pos(",") == 0) {
			tmp += ",00";
			txtbox->Text = tmp;
			txtbox->SelStart = tmp.Length();
		}
		//�������� �������� ��������
	}
}

//---------------------------------------------------------------------------

//��������� ���� ����� ��������� ������� ��� ������ ��������������� ������
void __fastcall TForm1::RadioWorkModeClick(TObject *Sender)
{
	if (RadioWorkMode->ItemIndex == 0) {
		EditAuto->Enabled = false;
		LabelAuto->Enabled = false;
		LabelAuto2->Enabled = false;
	}
	else{
		EditAuto->Enabled = true;
		LabelAuto->Enabled = true;
		LabelAuto2->Enabled = true;
    }
}

//---------------------------------------------------------------------------

//����������� ���������, ��������������� ���������� ������ �����
void __fastcall TForm1::InpTypeChange(TObject *Sender)
{
	switch (InpType->ItemIndex)
	{
		case 0:
		   StandartInputGroup->Visible = true;
		   ProfileInputGroup->Visible = false;
		   IncrInputGroup->Visible = false;
		   break;
		case 1:
		   StandartInputGroup->Visible = false;
		   ProfileInputGroup->Visible = true;
		   IncrInputGroup->Visible = false;
		   break;
		case 2:
		   StandartInputGroup->Visible = false;
		   ProfileInputGroup->Visible = false;
		   IncrInputGroup->Visible = true;
		   break;
	}
}

//---------------------------------------------------------------------------
//����� ���� �������������� ��������

void __fastcall TForm1::ButtonFProfileCreateClick(TObject *Sender)
{
	Application->CreateForm(__classid(TForm3), &Form3);
	Form3->ShowModal();
	Form3->Free();
}

void __fastcall TForm1::ProfileInpButtonCreateClick(TObject *Sender)
{
   Application->CreateForm(__classid(TForm2), &Form2);
   Form2->ShowModal();
   Form2->Free();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ButtonFProfileOpenClick(TObject *Sender)
{
	FILE* fl;
	if (OpenDialog->Execute()) {
		char* path;
		path = new char[100];
		wcstombs(path, OpenDialog->FileName.c_str(), 100);
		fl = fopen(path, "r+");

		delete[] path;

		char* tmp;
		tmp = new char[8];

		fscanf(fl, "%s", tmp);
		FprofileR0 = StrToFloat(tmp);

		for(int i = 0; i < 7; i++)
			for (int j = 0; j < 9; j++) {
				fscanf(fl, "%s", tmp);
				Fprofile[i][j] = StrToFloat(tmp);
//				Info->Lines->Add(FloatToStr(Fprofile[i][j]));
			}
		delete[] tmp;
		fclose(fl);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
	Fprofile = new double*[7];
	for(size_t i = 0; i < 7; ++i)
		Fprofile[i] = new double[9];
}
//---------------------------------------------------------------------------

