//---------------------------------------------------------------------------

#ifndef FProfileEditH
#define FProfileEditH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include "main.h"
#include <cstdio>
#include <cstdlib>
#include <string.h>
//---------------------------------------------------------------------------
class TForm3 : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TSaveDialog *SaveDialog;
	TButton *SaveButton;
	TLabeledEdit *R0;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall SaveButtonClick(TObject *Sender);
	void __fastcall EnterVal(TObject *Sender, System::WideChar &Key);
	void __fastcall AddZero(TObject *Sender);
private:	// User declarations
	char* editFile;

//	void AddZero(TObject *Sender);
public:		// User declarations
	TLabeledEdit* inputs[7][9];
	__fastcall TForm3(TComponent* Owner);
	__fastcall TForm3(TComponent* Owner, char* filepath);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm3 *Form3;
//---------------------------------------------------------------------------
#endif
