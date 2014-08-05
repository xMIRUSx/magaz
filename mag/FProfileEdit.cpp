//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FProfileEdit.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm3 *Form3;
//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner)
	: TForm(Owner)
{
	editFile = new char[1];
	editFile = '\0';
}

__fastcall TForm3::TForm3(TComponent* Owner, char* filepath)
	: TForm(Owner)
{
	editFile = filepath;
}
//---------------------------------------------------------------------------

void __fastcall TForm3::AddZero(TObject *Sender)
{
	TEdit *txtbox = static_cast<TEdit*>(Sender);
    AnsiString tmp = txtbox->Text;
	int comma = tmp.Pos(",");
	if (comma == 0)
	{
		tmp += ",0000";
		txtbox->Text = tmp;
		txtbox->SelStart = tmp.Length();
	}
	else
	{
		int afterComma = tmp.Length() - comma;
		if (afterComma < 4)
			for (int i = afterComma; i < 4; i++)
				tmp += "0";
	}
	txtbox->Text = tmp;
}

//---------------------------------------------------------------------------

void __fastcall TForm3::FormCreate(TObject *Sender)
{
	int TabOrd = 0;
	for(int i = 0; i < 7; i++)
		for (int j = 0; j < 9; j++)
		{
			inputs[i][j] = new TLabeledEdit (Form3);
			inputs[i][j]->Parent = this;
			inputs[i][j]->Width = 73;
			inputs[i][j]->Height = 21;
			inputs[i][j]->Top = 48 + 40*j;
			inputs[i][j]->Left = 24 + 96*i;
			inputs[i][j]->Text = "";

			inputs[i][j]->OnKeyPress = EnterVal;
            inputs[i][j]->OnExit = AddZero;

			inputs[i][j]->EditLabel->Caption = FloatToStr(pow(10, (4 - i))*(j + 1));

			inputs[i][j]->TabOrder = TabOrd;
			TabOrd++;
		}
}
//---------------------------------------------------------------------------

void __fastcall TForm3::EnterVal(TObject *Sender, System::WideChar &Key)
{
	TEdit *txtbox = static_cast<TEdit*>(Sender);
	AnsiString tmp = txtbox->Text;
	if (Key != 13 && Key != VK_TAB)
	{
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
				if (tmp.Length() - tmp.Pos(",") > 3)
					Key = 0;
		}
		else
			Key = 0;
	}
	else
	{
		Key = 0;
		SelectNext(txtbox, true, true);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm3::SaveButtonClick(TObject *Sender)
{
	if (R0->Text == "")
	{
		ShowMessage("Не все поля заполнены");
		return;
	}
	for(int i = 0; i < 7; i++)
		for (int j = 0; j < 9; j++)
			if (inputs[i][j]->Text == "")
			{
                ShowMessage("Не все поля заполнены");
				return;
			}

	FILE *fl;

	char* tmp;
	tmp = new char[12];

	if (editFile == '\0')
	{
		if (SaveDialog->Execute())
		{
			char* path;
			wchar_t* convertMe;
			path = new char[100];
			wcstombs(path, SaveDialog->FileName.c_str(), 100);
			fl = fopen(path, "w+");

			wcstombs(tmp, R0->Text.c_str(), 12);
			fprintf(fl, "%s\n", tmp);

			for(int i = 0; i < 7; i++)
				for (int j = 0; j < 9; j++)
				{
					std::wcstombs(tmp, inputs[i][j]->Text.c_str(), 12);
					fprintf(fl, "%s\n", tmp);
				}
			fclose(fl);
			delete[] path;

			String defaultName = "new_profile.txt";
			SaveDialog->FileName = defaultName.c_str();

			ShowMessage("Профиль сохранён!");
		}
	}
	else
	{
		fl = fopen(editFile, "w+");

		wcstombs(tmp, R0->Text.c_str(), 12);
		//если в строке не найдена ',', приписать в конец ",0000"
		fprintf(fl, "%s\n", tmp);

		Form1->FprofileR0 = StrToFloat(R0->Text);

		for(int i = 0; i < 7; i++)
			for (int j = 0; j < 9; j++)
			{
				Form1->Fprofile[i][j] = StrToFloat(inputs[i][j]->Text);
				std::wcstombs(tmp, inputs[i][j]->Text.c_str(), 8);
				//если в строке не найдена ',', приписать в конец ",0000"
				fprintf(fl, "%s\n", tmp);
			}
		fclose(fl);
		ShowMessage("Done!");
	}
	delete[] tmp;
}
//---------------------------------------------------------------------------

