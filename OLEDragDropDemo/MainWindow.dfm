object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 243
  ClientWidth = 527
  Color = clBtnFace
  DragMode = dmAutomatic
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 72
    Top = 88
    Width = 390
    Height = 13
    Caption = 
      'Drag this label to explorer will copy C:\windows\explorer.exe to' +
      ' correspond path '
    DragMode = dmAutomatic
    OnStartDrag = Label1StartDrag
  end
end
