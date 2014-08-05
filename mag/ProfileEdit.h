//---------------------------------------------------------------------------

#ifndef ProfileEditH
#define ProfileEditH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>
#include "main.h"
#include <cstdio>
#include <cstdlib>
#include <string.h>
//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE-managed Components
	TListBox *ListBox;
	TButton *ButtonUp;
	TButton *ButtonDown;
	TEdit *Edit;
	TButton *ButtonAdd;
	TButton *ButtonSave;
	TButton *ButtonDelete;
	TSaveDialog *SaveDialog;
	TButton *ButtonReplace;
	TEdit *EditReplace;
	void __fastcall ButtonUpClick(TObject *Sender);
	void __fastcall ButtonDownClick(TObject *Sender);
	void __fastcall ButtonDeleteClick(TObject *Sender);
	void __fastcall ButtonSaveClick(TObject *Sender);
	void __fastcall ButtonReplaceClick(TObject *Sender);
	void __fastcall EnterVal(TObject *Sender, System::WideChar &Key);
	void __fastcall ButtonAddClick(TObject *Sender);
private:	// User declarations
	String editFile;
public:		// User declarations
	__fastcall TForm2(TComponent* Owner);
	__fastcall TForm2(TComponent* Owner, String filepath);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
#endif
