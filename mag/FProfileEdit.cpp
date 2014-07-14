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
}
//---------------------------------------------------------------------------

void __fastcall TForm3::FormCreate(TObject *Sender)
{
	for(int i = 0; i < 7; i++)
		for (int j = 0; j < 9; j++) {
			inputs[i][j] = new TLabeledEdit (Form3);
			inputs[i][j]->Parent = Form3;
			inputs[i][j]->Width = 73;
			inputs[i][j]->Height = 21;
			inputs[i][j]->Top = 48 + 40*j;
			inputs[i][j]->Left = 24 + 96*i;

			inputs[i][j]->Text = "12";

			inputs[i][j]->EditLabel->Caption = FloatToStr(pow(10, (4 - i))*(j + 1));
		}
}
//---------------------------------------------------------------------------


void __fastcall TForm3::SaveButtonClick(TObject *Sender)
{
	if (SaveDialog->Execute()){

		FILE *fl;
		char* path;
		wchar_t* convertMe;
		path = new char[100];
		wcstombs(path, SaveDialog->FileName.c_str(), 100);
		fl = fopen(path, "w+");


		char* tmp;
		tmp = new char[8];
		wcstombs(tmp, R0->Text.c_str(), 8);
		fprintf(fl, "%s\n", tmp);

		for(int i = 0; i < 7; i++)
			for (int j = 0; j < 9; j++) {
				std::wcstombs(tmp, inputs[i][j]->Text.c_str(), 8);
				fprintf(fl, "%s\n", tmp);
			}
		fclose(fl);
		delete[] path;
		delete[] tmp;
		String defaultName = "����� �������.txt";
		SaveDialog->FileName = defaultName.c_str();

		ShowMessage("Done!");
	}
}
//---------------------------------------------------------------------------

