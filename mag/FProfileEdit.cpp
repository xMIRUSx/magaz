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

			inputs[i][j]->EditLabel->Caption = FloatToStr(pow(10, (4 - i))*(j + 1));
		}
}
//---------------------------------------------------------------------------

void __fastcall TForm3::SaveButtonClick(TObject *Sender)
{
	if (SaveDialog->Execute()){

		FILE *fl;
		char* path;
		std::wcstombs(path, SaveDialog->FileName.c_str(), 100);
		fl = fopen(path, "w");

/*		fw.open(SaveDialog->FileName);
		for(int i = 0; i < 7; i++)
			for (int j = 0; j < 9; j++) {

			}  */
		fclose(fl);
	}
}
//---------------------------------------------------------------------------

