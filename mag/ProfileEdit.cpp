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
	String tmp = Edit->Text;
	//если в строке не найдена ',', приписать в конец ",00"
	ListBox->Items->Add(Edit->Text);
}

void __fastcall TForm2::ButtonDeleteClick(TObject *Sender)
{
	if (ListBox->ItemIndex != -1)
		ListBox->Items->Delete(ListBox->ItemIndex);
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

