object Form1: TForm1
  Left = 0
  Top = 0
  BorderStyle = bsSingle
  Caption = 'Form1'
  ClientHeight = 505
  ClientWidth = 500
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object LabelAuto: TLabel
    Left = 8
    Top = 293
    Width = 110
    Height = 16
    Caption = #1048#1085#1090#1077#1088#1074#1072#1083' '#1074#1088#1077#1084#1077#1085#1080
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = 16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LabelAuto2: TLabel
    Left = 84
    Top = 315
    Width = 19
    Height = 16
    Caption = #1089#1077#1082
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = 16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label1: TLabel
    Left = 8
    Top = 364
    Width = 134
    Height = 13
    Caption = #1055#1088#1086#1092#1080#1083#1100' '#1076#1077#1081#1089#1090#1074#1080#1090#1077#1083#1100#1085#1099#1093
  end
  object Label2: TLabel
    Left = 8
    Top = 380
    Width = 127
    Height = 13
    Caption = #1079#1085#1072#1095#1077#1085#1080#1081' '#1089#1086#1087#1088#1086#1090#1080#1074#1083#1077#1085#1080#1103
  end
  object ComboBoxR0: TComboBox
    Left = 8
    Top = 178
    Width = 137
    Height = 21
    TabOrder = 14
    Text = 'R0'
    Visible = False
    Items.Strings = (
      '10'
      '50'
      '100'
      '500'
      '1000')
  end
  object ComboBoxType: TComboBox
    Left = 8
    Top = 151
    Width = 137
    Height = 21
    TabOrder = 13
    Text = #1053#1057#1061' '#1058#1057
    Visible = False
    Items.Strings = (
      #1055#1083#1072#1090#1080#1085#1086#1074#1099#1081' '#1072'=0,00385'
      #1055#1083#1072#1090#1080#1085#1086#1074#1099#1081' '#1072'=0,00391'
      #1052#1077#1076#1085#1099#1081
      #1053#1080#1082#1077#1083#1077#1074#1099#1081)
  end
  object EditAuto: TEdit
    Left = 16
    Top = 315
    Width = 49
    Height = 21
    Alignment = taCenter
    Enabled = False
    NumbersOnly = True
    TabOrder = 0
    OnKeyPress = EditAutoKeyPress
  end
  object InpType: TComboBox
    Left = 219
    Top = 11
    Width = 94
    Height = 21
    TabOrder = 1
    Text = #1057#1087#1086#1089#1086#1073' '#1074#1074#1086#1076#1072
    OnChange = InpTypeChange
    Items.Strings = (
      #1055#1088#1086#1092#1080#1083#1100
      #1054#1073#1099#1095#1085#1099#1081)
  end
  object RadioDevice: TRadioGroup
    Left = 8
    Top = 8
    Width = 137
    Height = 65
    Caption = #1058#1080#1087' '#1087#1088#1080#1073#1086#1088#1072
    Items.Strings = (
      #1052#1057'3070'#1055#1059'-01'#1042
      #1050#1057#1069'-01'#1042)
    TabOrder = 2
  end
  object RadioPhysVal: TRadioGroup
    Left = 8
    Top = 79
    Width = 137
    Height = 48
    Caption = #1060#1080#1079'. '#1074#1077#1083#1080#1095#1080#1085#1072
    Columns = 2
    ItemIndex = 0
    Items.Strings = (
      'Om'
      'C')
    TabOrder = 3
    OnClick = RadioPhysValClick
  end
  object RadioWorkMode: TRadioGroup
    Left = 8
    Top = 222
    Width = 137
    Height = 65
    Caption = #1056#1077#1078#1080#1084' '#1088#1072#1073#1086#1090#1099
    ItemIndex = 0
    Items.Strings = (
      #1056#1091#1095#1085#1086#1081
      #1040#1074#1090#1086#1084#1072#1090#1080#1095#1077#1089#1082#1080#1081)
    TabOrder = 4
    OnClick = RadioWorkModeClick
  end
  object ButtonFProfileCreate: TButton
    Left = 8
    Top = 430
    Width = 120
    Height = 25
    Caption = #1057#1086#1079#1076#1072#1090#1100' '#1085#1086#1074#1099#1081
    TabOrder = 5
    OnClick = ButtonFProfileCreateClick
  end
  object ButtonFProfileOpen: TButton
    Left = 8
    Top = 399
    Width = 57
    Height = 25
    Caption = #1054#1090#1082#1088#1099#1090#1100
    TabOrder = 6
    OnClick = ButtonFProfileOpenClick
  end
  object ButtonFProfileEdit: TButton
    Left = 71
    Top = 399
    Width = 57
    Height = 25
    Caption = #1048#1079#1084#1077#1085#1080#1090#1100
    TabOrder = 7
    OnClick = ButtonFProfileEditClick
  end
  object ButtonFProfileSend: TButton
    Left = 8
    Top = 461
    Width = 120
    Height = 25
    Caption = #1047#1072#1087#1080#1089#1072#1090#1100' '#1074' '#1087#1088#1080#1073#1086#1088
    TabOrder = 8
  end
  object CheckBoxOtklon: TCheckBox
    Left = 8
    Top = 133
    Width = 137
    Height = 17
    Caption = #1054#1090#1082#1083#1086#1085#1077#1085#1080#1077' '#1086#1090' R '#1085#1086#1084#1080#1085#1072#1083#1100#1085#1086#1075#1086
    TabOrder = 9
    OnClick = CheckBoxOtklonClick
  end
  object RadioOtklonOm: TRadioButton
    Left = 8
    Top = 164
    Width = 41
    Height = 17
    Caption = #1074' Om'
    Checked = True
    Enabled = False
    TabOrder = 10
    TabStop = True
  end
  object RadioOtklonPercent: TRadioButton
    Left = 65
    Top = 164
    Width = 80
    Height = 17
    Caption = #1074' '#1087#1088#1086#1094#1077#1085#1090#1072#1093
    Enabled = False
    TabOrder = 11
  end
  object CheckCorrection: TCheckBox
    Left = 8
    Top = 187
    Width = 137
    Height = 17
    Caption = #1055#1086#1076#1075#1086#1085#1082#1072' '#1082' '#1085#1086#1084#1080#1085#1072#1083#1091
    Enabled = False
    TabOrder = 12
  end
  object Info: TMemo
    Left = 219
    Top = 228
    Width = 271
    Height = 227
    Lines.Strings = (
      'Info')
    ReadOnly = True
    ScrollBars = ssVertical
    TabOrder = 15
  end
  object ButtonDiagnostic: TButton
    Left = 404
    Top = 461
    Width = 86
    Height = 25
    Caption = #1044#1080#1072#1075#1085#1086#1089#1090#1080#1082#1072
    TabOrder = 16
    OnClick = ButtonDiagnosticClick
  end
  object ConnectionButton: TButton
    Left = 219
    Top = 461
    Width = 158
    Height = 25
    Caption = #1055#1086#1076#1082#1083#1102#1095#1077#1085#1080#1077' '#1082' '#1091#1089#1090#1088#1086#1081#1089#1090#1074#1091
    TabOrder = 20
  end
  object ProfileInputGroup: TGroupBox
    Left = 219
    Top = 38
    Width = 230
    Height = 166
    TabOrder = 18
    Visible = False
    object ProfileInpButtonChange: TButton
      Left = 112
      Top = 42
      Width = 107
      Height = 25
      Caption = #1048#1079#1084#1077#1085#1080#1090#1100' '#1087#1088#1086#1092#1080#1083#1100
      TabOrder = 0
      OnClick = ProfileInpButtonChangeClick
    end
    object ProfileInpButtonCreate: TButton
      Left = 112
      Top = 95
      Width = 107
      Height = 25
      Caption = #1057#1086#1079#1076#1072#1090#1100' '#1087#1088#1086#1092#1080#1083#1100
      TabOrder = 1
      OnClick = ProfileInpButtonCreateClick
    end
    object ProfileInpButtonEnter: TButton
      Left = 15
      Top = 138
      Width = 91
      Height = 25
      Caption = #1057#1090#1072#1088#1090
      TabOrder = 2
      OnClick = ProfileInpButtonEnterClick
    end
    object ProfileInpButtonOpen: TButton
      Left = 112
      Top = 11
      Width = 107
      Height = 25
      Caption = #1054#1090#1082#1088#1099#1090#1100' '#1087#1088#1086#1092#1080#1083#1100
      TabOrder = 3
      OnClick = ProfileInpButtonOpenClick
    end
    object ProfileValuesList: TListBox
      Left = 15
      Top = 11
      Width = 91
      Height = 121
      ItemHeight = 13
      TabOrder = 4
    end
  end
  object StandartInputGroup: TGroupBox
    Left = 219
    Top = 38
    Width = 271
    Height = 166
    TabOrder = 17
    Visible = False
    StyleElements = [seFont, seClient]
    object StandartInpButtonDel: TButton
      Left = 197
      Top = 14
      Width = 57
      Height = 25
      Caption = #1057#1073#1088#1086#1089
      TabOrder = 0
      OnClick = StandartInpButtonDelClick
    end
    object StandartInpEdit: TEdit
      Left = 14
      Top = 16
      Width = 108
      Height = 21
      TabOrder = 1
      OnKeyPress = EnterOm
    end
    object StandartInpButtonEnter: TButton
      Left = 128
      Top = 14
      Width = 57
      Height = 25
      Caption = #1042#1074#1086#1076
      TabOrder = 2
      OnClick = StandartInpButtonEnterClick
    end
    object StandartInpList: TListBox
      Left = 14
      Top = 44
      Width = 108
      Height = 109
      ItemHeight = 13
      TabOrder = 3
    end
    object StandartInpButtonSaveAsProfile: TButton
      Left = 128
      Top = 126
      Width = 132
      Height = 27
      Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1082#1072#1082' '#1087#1088#1086#1092#1080#1083#1100
      TabOrder = 4
      OnClick = StandartInpButtonSaveAsProfileClick
    end
  end
  object IncrInputGroup: TGroupBox
    Left = 219
    Top = 49
    Width = 254
    Height = 143
    TabOrder = 19
    Visible = False
    object IncInpLabel: TLabel
      Left = 11
      Top = 88
      Width = 195
      Height = 16
      Caption = #1057#1083#1077#1076#1091#1102#1097#1077#1077' '#1074#1074#1086#1076#1080#1084#1086#1077' '#1079#1085#1072#1095#1077#1085#1080#1077':'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = 16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object IncrInpNextVal: TLabel
      Left = 212
      Top = 88
      Width = 10
      Height = 16
      Caption = '--'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = 16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object startLabel: TLabel
      Left = 82
      Top = 9
      Width = 123
      Height = 16
      Caption = #1053#1072#1095#1072#1083#1100#1085#1086#1077' '#1079#1085#1072#1095#1077#1085#1080#1077
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = 16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object endLabel: TLabel
      Left = 82
      Top = 34
      Width = 115
      Height = 16
      Caption = #1050#1086#1085#1077#1095#1085#1086#1077' '#1079#1085#1072#1095#1077#1085#1080#1077
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = 16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label3: TLabel
      Left = 82
      Top = 62
      Width = 24
      Height = 16
      Caption = #1064#1072#1075
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = 16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object IncInpEditEnd: TEdit
      Left = 11
      Top = 33
      Width = 65
      Height = 21
      TabOrder = 1
      OnKeyPress = EnterOm
    end
    object IncInpEditStart: TEdit
      Left = 11
      Top = 6
      Width = 65
      Height = 21
      TabOrder = 0
      OnKeyPress = EnterOm
    end
    object IncInpEditStep: TEdit
      Left = 11
      Top = 60
      Width = 65
      Height = 21
      TabOrder = 2
      OnKeyPress = EnterOm
    end
    object IncrInpButtonEnter: TButton
      Left = 11
      Top = 110
      Width = 57
      Height = 25
      Caption = #1057#1090#1072#1088#1090
      TabOrder = 3
      OnClick = IncrInpButtonEnterClick
    end
  end
  object OpenDialog: TOpenDialog
    DefaultExt = 'txt'
    Filter = #1058#1077#1082#1089#1090#1086#1074#1099#1081' '#1092#1072#1081#1083'|*.txt'
    Left = 176
    Top = 224
  end
  object SaveDialog: TSaveDialog
    DefaultExt = 'txt'
    FileName = #1053#1086#1074#1099#1081' '#1087#1088#1086#1092#1080#1083#1100'.txt'
    Filter = #1058#1077#1082#1089#1090#1086#1074#1099#1081' '#1092#1072#1081#1083'|*.txt'
    Left = 176
    Top = 176
  end
  object Timer: TTimer
    Enabled = False
    Left = 120
    Top = 312
  end
end
