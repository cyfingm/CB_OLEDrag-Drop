object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'DragOutDemo'
  ClientHeight = 283
  ClientWidth = 590
  Color = clBtnFace
  DockSite = True
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object m_TipLabel: TLabel
    Left = 96
    Top = 64
    Width = 376
    Height = 13
    Caption = #35831#23558#25991#20214#25302#25341#33267#19979#26041'ListView'#20013#25110#32773#20174'ListView'#20013#25302#25341#25991#20214#21040#36164#28304#31649#29702#22120#20013
    DragMode = dmAutomatic
  end
  object m_FileListView: TListView
    Left = 96
    Top = 83
    Width = 407
    Height = 150
    Columns = <
      item
        AutoSize = True
        Caption = #25991#20214#21015#34920
      end>
    MultiSelect = True
    TabOrder = 0
    ViewStyle = vsReport
  end
end
