//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ProfileEdit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{
	editFile = '\0';
}

__fastcall TForm2::TForm2(TComponent* Owner, String filepath)
	: TForm(Owner)
{
	editFile = filepath;
}

//---------------------------------------------------------------------------
void __fastcall TForm2::ButtonAddClick(TObject *Sender)
{
	Form1->AddZero(Edit);
	ListBox->Items->Add(Edit->Text);
}

void __fastcall TForm2::ButtonDeleteClick(TObject *Sender)
{
	if (ListBox->ItemIndex != -1)
	{
        Form1->AddZero(EditReplace);
		ListBox->Items->Delete(ListBox->ItemIndex);
	}
}

void __fastcall TForm2::ButtonReplaceClick(TObject *Sender)
{
	 if (ListBox->ItemIndex != -1)
	 {
		ListBox->Items->Strings[ListBox->ItemIndex] = EditReplace->Text;
	 }
}
//---------------------------------------------------------------------------
//изменение позиции элемента в списке

void __fastcall TForm2::ButtonUpClick(TObject *Sender)
{
	if (ListBox->ItemIndex > 0)
	{
		int pos = ListBox->ItemIndex;
		String tmp = ListBox->Items->Strings[pos];
		ListBox->Items->Delete(pos);
		ListBox->Items->Insert(pos-1, tmp);
		ListBox->ItemIndex = pos-1;
	}
}

void __fastcall TForm2::ButtonDownClick(TObject *Sender)
{
	if (ListBox->ItemIndex < ListBox->Items->Count - 1)
	{
		int pos = ListBox->ItemIndex;
		String tmp = ListBox->Items->Strings[pos];
		ListBox->Items->Delete(pos);
		ListBox->Items->Insert(pos+1, tmp);
		ListBox->ItemIndex = pos+1;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm2::ButtonSaveClick(TObject *Sender)
{

	if (editFile != '\0')
	{
		ListBox->Items->SaveToFile(editFile);
		Form1->ProfileValuesList->Items = ListBox->Items;
		ShowMessage("Done!");
	}
	else
	if (SaveDialog->Execute())
	{
		char* path;
		path = new char[100];
		wcstombs(path, SaveDialog->FileName.c_str(), 100);

		ListBox->Items->SaveToFile(path);

		delete[] path;
		String defaultName = "Новый профиль.txt";
		SaveDialog->FileName = defaultName.c_str();

		ShowMessage("Done!");
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm2::EnterVal(TObject *Sender, System::WideChar &Key)
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
		if (txtbox->Name == "Edit")
			ButtonAddClick(this);
		else
			ButtonReplaceClick(this);
	}
}
//---------------------------------------------------------------------------

