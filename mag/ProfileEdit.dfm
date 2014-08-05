object Form2: TForm2
  Left = 0
  Top = 0
  Caption = 'Form2'
  ClientHeight = 271
  ClientWidth = 266
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object ListBox: TListBox
    Left = 15
    Top = 56
    Width = 97
    Height = 161
    ItemHeight = 13
    ScrollWidth = 1
    TabOrder = 0
  end
  object ButtonUp: TButton
    Left = 136
    Top = 56
    Width = 113
    Height = 25
    Caption = #1055#1077#1088#1077#1084#1077#1089#1090#1080#1090#1100' '#1074#1099#1096#1077
    TabOrder = 1
    OnClick = ButtonUpClick
  end
  object ButtonDown: TButton
    Left = 136
    Top = 87
    Width = 113
    Height = 25
    Caption = #1055#1077#1088#1077#1084#1077#1089#1090#1080#1090#1100' '#1085#1080#1078#1077
    TabOrder = 2
    OnClick = ButtonDownClick
  end
  object Edit: TEdit
    Left = 15
    Top = 8
    Width = 97
    Height = 21
    TabOrder = 3
    OnKeyPress = EnterVal
  end
  object ButtonAdd: TButton
    Left = 136
    Top = 8
    Width = 113
    Height = 25
    Caption = #1044#1086#1073#1072#1074#1080#1090#1100' '#1079#1085#1072#1095#1077#1085#1080#1077
    TabOrder = 4
    OnClick = ButtonAddClick
  end
  object ButtonSave: TButton
    Left = 80
    Top = 240
    Width = 113
    Height = 25
    Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
    TabOrder = 5
    OnClick = ButtonSaveClick
  end
  object ButtonDelete: TButton
    Left = 136
    Top = 192
    Width = 113
    Height = 25
    Caption = #1059#1076#1072#1083#1080#1090#1100
    TabOrder = 6
    OnClick = ButtonDeleteClick
  end
  object ButtonReplace: TButton
    Left = 136
    Top = 118
    Width = 113
    Height = 25
    Caption = #1047#1072#1084#1077#1085#1080#1090#1100' '#1085#1072
    TabOrder = 7
    OnClick = ButtonReplaceClick
  end
  object EditReplace: TEdit
    Left = 152
    Top = 149
    Width = 89
    Height = 21
    TabOrder = 8
    OnKeyPress = EnterVal
  end
  object SaveDialog: TSaveDialog
    DefaultExt = 'txt'
    FileName = #1053#1086#1074#1099#1081' '#1087#1088#1086#1092#1080#1083#1100'.txt'
    Filter = #1058#1077#1082#1089#1090#1086#1074#1099#1081' '#1092#1072#1081#1083'|*.txt'
    Left = 208
    Top = 208
  end
end
