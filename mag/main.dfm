object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 495
  ClientWidth = 588
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
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
    Width = 137
    Height = 13
    Caption = #1055#1088#1086#1092#1080#1083#1100' '#1076#1077#1081#1089#1090#1074#1080#1090#1077#1083#1100#1085#1099#1093' '
  end
  object Label2: TLabel
    Left = 8
    Top = 380
    Width = 127
    Height = 13
    Caption = #1079#1085#1072#1095#1077#1085#1080#1081' '#1089#1086#1087#1088#1086#1090#1080#1074#1083#1077#1085#1080#1103
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
    Text = '3600'
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
      #1054#1073#1099#1095#1085#1099#1081
      #1055#1088#1086#1092#1080#1083#1100
      #1048#1082#1088#1077#1084#1077#1085#1090)
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
  end
  object ButtonFProfileOpen: TButton
    Left = 8
    Top = 399
    Width = 57
    Height = 25
    Caption = #1054#1090#1082#1088#1099#1090#1100
    TabOrder = 6
  end
  object ButtonFProfileEdit: TButton
    Left = 71
    Top = 399
    Width = 57
    Height = 25
    Caption = #1048#1079#1084#1077#1085#1080#1090#1100
    TabOrder = 7
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
  end
  object RadioOtklonOm: TRadioButton
    Left = 8
    Top = 164
    Width = 41
    Height = 17
    Caption = #1074' Om'
    TabOrder = 10
  end
  object RadioOtklonPercent: TRadioButton
    Left = 65
    Top = 164
    Width = 80
    Height = 17
    Caption = #1074' '#1087#1088#1086#1094#1077#1085#1090#1072#1093
    TabOrder = 11
  end
  object CheckCorrection: TCheckBox
    Left = 8
    Top = 187
    Width = 137
    Height = 17
    Caption = #1055#1086#1076#1075#1086#1085#1082#1072' '#1082' '#1085#1086#1084#1080#1085#1072#1083#1091
    TabOrder = 12
  end
  object ComboBoxType: TComboBox
    Left = 8
    Top = 133
    Width = 94
    Height = 21
    TabOrder = 13
    Text = #1053#1057#1061' '#1058#1057
    Visible = False
    OnChange = ComboBoxTypeChange
    Items.Strings = (
      #1055#1083#1072#1090#1080#1085#1086#1074#1099#1081' '#1072'=0,00385'
      #1055#1083#1072#1090#1080#1085#1086#1074#1099#1081' '#1072'=0,00391'
      #1052#1077#1076#1085#1099#1081
      #1053#1080#1082#1077#1083#1077#1074#1099#1081)
  end
  object ComboBoxR0: TComboBox
    Left = 8
    Top = 160
    Width = 94
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
  object TemperaturePlat: TComboBox
    Left = 8
    Top = 187
    Width = 94
    Height = 21
    TabOrder = 15
    Text = #1044#1080#1072#1087#1072#1079#1086#1085' C'
    Visible = False
    Items.Strings = (
      #1086#1090' -200 '#1076#1086' 0'
      #1086#1090' 0 '#1076#1086' 850')
  end
  object TemperatureCop: TComboBox
    Left = 8
    Top = 187
    Width = 94
    Height = 21
    TabOrder = 16
    Text = #1044#1080#1072#1087#1072#1079#1086#1085' C'
    Visible = False
    Items.Strings = (
      #1086#1090' -180 '#1076#1086' 0'
      #1086#1090' 0 '#1076#1086' 200')
  end
  object TemperatureNik: TComboBox
    Left = 8
    Top = 187
    Width = 94
    Height = 21
    TabOrder = 17
    Text = #1044#1080#1072#1087#1072#1079#1086#1085' C'
    Visible = False
    Items.Strings = (
      #1086#1090' -60 '#1076#1086' 100'
      #1086#1090' 100 '#1076#1086' 180')
  end
  object Info: TMemo
    Left = 219
    Top = 228
    Width = 358
    Height = 227
    Lines.Strings = (
      'Info')
    ReadOnly = True
    TabOrder = 18
  end
  object ButtonDiagnostic: TButton
    Left = 460
    Top = 461
    Width = 120
    Height = 25
    Caption = #1044#1080#1072#1075#1085#1086#1089#1090#1080#1082#1072
    TabOrder = 19
  end
  object ProfileInputGroup: TGroupBox
    Left = 219
    Top = 38
    Width = 225
    Height = 166
    TabOrder = 21
    Visible = False
    object ProfileInpButtonChange: TButton
      Left = 14
      Top = 42
      Width = 107
      Height = 25
      Caption = #1048#1079#1084#1077#1085#1080#1090#1100' '#1087#1088#1086#1092#1080#1083#1100
      TabOrder = 0
    end
    object ProfileInpButtonCreate: TButton
      Left = 14
      Top = 90
      Width = 107
      Height = 25
      Caption = #1057#1086#1079#1076#1072#1090#1100' '#1087#1088#1086#1092#1080#1083#1100
      TabOrder = 1
    end
    object ProfileInpButtonEnter: TButton
      Left = 126
      Top = 138
      Width = 91
      Height = 25
      Caption = #1042#1074#1086#1076' '#1079#1085#1072#1095#1077#1085#1080#1103
      TabOrder = 2
    end
    object ProfileInpButtonOpen: TButton
      Left = 14
      Top = 11
      Width = 107
      Height = 25
      Caption = #1054#1090#1082#1088#1099#1090#1100' '#1087#1088#1086#1092#1080#1083#1100
      TabOrder = 3
    end
    object ProfileValuesList: TListBox
      Left = 127
      Top = 11
      Width = 91
      Height = 121
      ItemHeight = 13
      TabOrder = 4
    end
  end
  object ConnectionButton: TButton
    Left = 219
    Top = 461
    Width = 166
    Height = 25
    Caption = #1055#1086#1076#1082#1083#1102#1095#1077#1085#1080#1077' '#1082' '#1091#1089#1090#1088#1086#1081#1089#1090#1074#1091
    TabOrder = 23
  end
  object IncrInputGroup: TGroupBox
    Left = 219
    Top = 49
    Width = 361
    Height = 132
    TabOrder = 22
    Visible = False
    object IncInpLabel: TLabel
      Left = 11
      Top = 68
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
      Top = 68
      Width = 25
      Height = 16
      Caption = '0,00'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = 16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object startLabel: TLabel
      Left = 11
      Top = 3
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
      Left = 149
      Top = 3
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
      Left = 275
      Top = 3
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
      Left = 168
      Top = 25
      Width = 65
      Height = 21
      TabOrder = 0
      OnKeyPress = EnterOm
    end
    object IncInpEditStart: TEdit
      Left = 35
      Top = 25
      Width = 65
      Height = 21
      TabOrder = 1
      OnKeyPress = EnterOm
    end
    object IncInpEditStep: TEdit
      Left = 283
      Top = 25
      Width = 62
      Height = 21
      TabOrder = 2
      OnKeyPress = EnterOm
    end
    object IncrInpButtonEnter: TButton
      Left = 201
      Top = 95
      Width = 57
      Height = 25
      Caption = #1042#1074#1086#1076
      TabOrder = 3
    end
  end
  object StandartInputGroup: TGroupBox
    Left = 219
    Top = 68
    Width = 286
    Height = 59
    TabOrder = 20
    Visible = False
    StyleElements = [seFont, seClient]
    object StandartInpButtonDel: TButton
      Left = 212
      Top = 15
      Width = 57
      Height = 25
      Caption = #1057#1073#1088#1086#1089
      TabOrder = 0
    end
    object StandartInpEdit: TEdit
      Left = 14
      Top = 17
      Width = 129
      Height = 21
      TabOrder = 1
      OnKeyPress = EnterOm
    end
    object StandartInpButtonEnter: TButton
      Left = 149
      Top = 15
      Width = 57
      Height = 25
      Caption = #1042#1074#1086#1076
      TabOrder = 2
    end
  end
end