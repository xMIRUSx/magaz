object Form3: TForm3
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Form3'
  ClientHeight = 460
  ClientWidth = 700
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Scaled = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 24
    Top = 8
    Width = 299
    Height = 13
    Caption = #1053#1086#1074#1099#1081' '#1087#1088#1086#1092#1080#1083#1100' '#1076#1077#1081#1089#1090#1074#1080#1090#1077#1083#1100#1085#1099#1093' '#1079#1085#1072#1095#1077#1085#1080#1081' '#1089#1086#1087#1088#1086#1090#1080#1074#1083#1077#1085#1080#1103
  end
  object SaveButton: TButton
    Left = 600
    Top = 416
    Width = 75
    Height = 25
    Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
    TabOrder = 0
    OnClick = SaveButtonClick
  end
  object R0: TLabeledEdit
    Left = 170
    Top = 418
    Width = 73
    Height = 21
    EditLabel.Width = 142
    EditLabel.Height = 13
    EditLabel.BiDiMode = bdLeftToRight
    EditLabel.Caption = #1053#1072#1095#1072#1083#1100#1085#1086#1077' '#1089#1086#1087#1088#1086#1090#1080#1074#1083#1077#1085#1080#1077': '
    EditLabel.ParentBiDiMode = False
    EditLabel.Layout = tlBottom
    LabelPosition = lpLeft
    TabOrder = 1
  end
  object SaveDialog: TSaveDialog
    Filter = #1058#1077#1082#1089#1090#1086#1074#1099#1081' '#1092#1072#1081#1083'|*.txt'
    Left = 680
    Top = 424
  end
end
