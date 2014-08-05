//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <fstream>
#include <cmath>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TEdit *EditAuto;
	TComboBox *InpType;
	TRadioGroup *RadioDevice;
	TRadioGroup *RadioPhysVal;
	TLabel *LabelAuto;
	TRadioGroup *RadioWorkMode;
	TLabel *LabelAuto2;
	TLabel *Label1;
	TLabel *Label2;
	TButton *ButtonFProfileCreate;
	TButton *ButtonFProfileOpen;
	TButton *ButtonFProfileEdit;
	TButton *ButtonFProfileSend;
	TEdit *StandartInpEdit;
	TButton *StandartInpButtonEnter;
	TButton *StandartInpButtonDel;
	TEdit *IncInpEditStart;
	TEdit *IncInpEditEnd;
	TEdit *IncInpEditStep;
	TButton *IncrInpButtonEnter;
	TLabel *IncInpLabel;
	TLabel *IncrInpNextVal;
	TListBox *ProfileValuesList;
	TButton *ProfileInpButtonEnter;
	TButton *ProfileInpButtonOpen;
	TButton *ProfileInpButtonCreate;
	TButton *ProfileInpButtonChange;
	TCheckBox *CheckBoxOtklon;
	TRadioButton *RadioOtklonOm;
	TRadioButton *RadioOtklonPercent;
	TCheckBox *CheckCorrection;
	TComboBox *ComboBoxType;
	TComboBox *ComboBoxR0;
	TMemo *Info;
	TButton *ButtonDiagnostic;
	TGroupBox *StandartInputGroup;
	TGroupBox *ProfileInputGroup;
	TGroupBox *IncrInputGroup;
	TLabel *startLabel;
	TLabel *endLabel;
	TLabel *Label3;
	TButton *ConnectionButton;
	TOpenDialog *OpenDialog;
	TListBox *StandartInpList;
	TButton *StandartInpButtonSaveAsProfile;
	TSaveDialog *SaveDialog;
	TTimer *Timer;
	TLabel *StandartInpOtklon1;
	TLabel *StandartInpOtklon;
	void __fastcall RadioPhysValClick(TObject *Sender);
	void __fastcall EnterOm(TObject *Sender, System::WideChar &Key);
	void __fastcall RadioWorkModeClick(TObject *Sender);
	void __fastcall InpTypeChange(TObject *Sender);
	void __fastcall ButtonFProfileCreateClick(TObject *Sender);
	void __fastcall ProfileInpButtonCreateClick(TObject *Sender);
	void __fastcall ButtonFProfileOpenClick(TObject *Sender);
	void __fastcall ButtonFProfileEditClick(TObject *Sender);
	void __fastcall ProfileInpButtonOpenClick(TObject *Sender);
	void __fastcall ProfileInpButtonChangeClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall StandartInpButtonDelClick(TObject *Sender);
	void __fastcall ButtonDiagnosticClick(TObject *Sender);
	void __fastcall StandartInpButtonEnterClick(TObject *Sender);
	void __fastcall StandartInpButtonSaveAsProfileClick(TObject *Sender);
	void __fastcall IncrInpButtonEnterClick(TObject *Sender);
	void __fastcall TimerIncr(TObject *Sender);
	void __fastcall TimerProfile(TObject *Sender);
	void __fastcall ProfileInpButtonEnterClick(TObject *Sender);
	void __fastcall CheckBoxOtklonClick(TObject *Sender);
	void __fastcall EditAutoKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall ConnectionButtonClick(TObject *Sender);
	void __fastcall RadioDeviceClick(TObject *Sender);
	void __fastcall ButtonFProfileSendClick(TObject *Sender);
	void __fastcall AddZero(TObject *Sender);


private:	// User declarations

	//путь к файлу, в котором хранится текущий рабочий профиль
    String profilePath;

	//для инкрементного/декрементного ввода
	double incrStrt, incrEnd, incrStep, incrNextVal;

	bool checkIncVals();
	String CalcOtkl(String val);
	double CalcNominal(double val);
	String CorrectVal(String str);

	//загрузка профиля номинальных значений из файла

	void LoadFprofile(char* path);
	void SendToDevice(String val);

	const char* devID;

public:		// User declarations

	//значения текущего профиля номинальных значений и путь к файлу
	double** Fprofile;
	double FprofileR0;
	char* FprofilePath;

	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
