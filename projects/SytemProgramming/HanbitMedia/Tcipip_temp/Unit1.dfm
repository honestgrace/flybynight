object Form1: TForm1
  Left = 192
  Top = 107
  Width = 264
  Height = 190
  Caption = 'Winsock TCP Message Client'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 54
    Height = 13
    Caption = 'IP Address:'
  end
  object Label2: TLabel
    Left = 8
    Top = 32
    Width = 22
    Height = 13
    Caption = 'Port:'
  end
  object Label3: TLabel
    Left = 8
    Top = 56
    Width = 86
    Height = 13
    Caption = 'Message to Send:'
  end
  object Label4: TLabel
    Left = 8
    Top = 80
    Width = 95
    Height = 13
    Caption = 'Message Received:'
  end
  object Edit1: TEdit
    Left = 104
    Top = 8
    Width = 145
    Height = 21
    TabOrder = 0
    Text = '127.0.0.1'
  end
  object Edit2: TEdit
    Left = 104
    Top = 32
    Width = 145
    Height = 21
    TabOrder = 1
    Text = '4000'
  end
  object Edit3: TEdit
    Left = 104
    Top = 56
    Width = 145
    Height = 21
    TabOrder = 2
    Text = 'Hello, Winsock!'
  end
  object Button1: TButton
    Left = 8
    Top = 112
    Width = 75
    Height = 25
    Caption = 'Connect'
    TabOrder = 3
    OnClick = Button1Click
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 144
    Width = 256
    Height = 19
    Panels = <>
    SimplePanel = False
  end
  object Button2: TButton
    Left = 88
    Top = 112
    Width = 75
    Height = 25
    Caption = 'Disconnect'
    Enabled = False
    TabOrder = 5
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 168
    Top = 112
    Width = 75
    Height = 25
    Caption = 'Send'
    Enabled = False
    TabOrder = 6
    OnClick = Button3Click
  end
  object Button4: TButton
    Left = 168
    Top = 16
    Width = 75
    Height = 25
    Caption = 'Button4'
    TabOrder = 7
    Visible = False
    OnMouseUp = Button4MouseUp
  end
  object Edit4: TEdit
    Left = 104
    Top = 80
    Width = 145
    Height = 21
    TabOrder = 8
  end
end
