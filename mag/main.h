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
#include <vector>
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
	void __fastcall RadioPhysValClick(TObject *Sender);
	void __fastcall EnterOm(TObject *Sender, System::WideChar &Key);
	void __fastcall RadioWorkModeClick(TObject *Sender);
	void __fastcall InpTypeChange(TObject *Sender);
	void __fastcall ButtonFProfileCreateClick(TObject *Sender);
	void __fastcall ProfileInpButtonCreateClick(TObject *Sender);
	void __fastcall ButtonFProfileOpenClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);

private:	// User declarations
public:		// User declarations

	//��������� ������ + ��������� ���� ��� ������� ����������� ��������
	//������ ��� �������� �������
	double* profile;
	double** Fprofile;
	double FprofileR0;

	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
