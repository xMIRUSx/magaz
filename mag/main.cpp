//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "main.h"
#include <string.h>
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


//---------------------------------------------------------------------------

void __fastcall TForm1::RadioPhysValClick(TObject *Sender)
{
	if (RadioPhysVal->ItemIndex == 0) {
		CheckBoxOtklon->Visible = true;
		RadioOtklonOm->Visible = true;
		RadioOtklonPercent->Visible = true;
		CheckCorrection->Visible = true;

		ComboBoxType->Visible = false;
		ComboBoxR0->Visible = false;
		TemperaturePlat->Visible = false;
	}
	else {
		CheckBoxOtklon->Visible = false;
		RadioOtklonOm->Visible = false;
		RadioOtklonPercent->Visible = false;
		CheckCorrection->Visible = false;

		ComboBoxType->Visible = true;
		ComboBoxR0->Visible = true;
		TemperaturePlat->Visible = true;
	}
}

//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBoxTypeChange(TObject *Sender)
{
	switch (ComboBoxType->ItemIndex)
	{
		case 0:
		case 1:
		   TemperaturePlat->Visible = true;
		   TemperatureCop->Visible = false;
		   TemperatureNik->Visible = false;
		   break;
		case 2:
		   TemperaturePlat->Visible = false;
		   TemperatureCop->Visible = true;
		   TemperatureNik->Visible = false;
		   break;
		case 3:
		   TemperaturePlat->Visible = false;
		   TemperatureCop->Visible = false;
		   TemperatureNik->Visible = true;
		   break;
	}
}

//---------------------------------------------------------------------------

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