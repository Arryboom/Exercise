Attribute VB_Name = "modCal"
Option Explicit   'Ҫ���������
Option Base 1     '�����±��1��ʼ

Public gDij(21) As Double    '����۲�ֵ
Public gB(21, 7) As Double
Public gL(21) As Double

Public gN(7, 7) As Double
Public gU(7) As Double
Public gX(7) As Double
Public gQ(7, 7) As Double
Public gM0 As Double      '��λȨ�����

Public Sub CalK()
  Call MakeBL
  Call MakeNU
  Call CalQ
  Call CalX
  Call CalM0
End Sub

'�������
Private Sub MakeBL()
  Dim i As Integer, j As Integer, k As Integer
  'B��ʼ��Ϊ0,���L
  For i = 1 To 21
    For j = 1 To 7
      gB(i, j) = 0
    Next
    gL(i) = gDij(i)
  Next
  '���B
  k = 1
  For i = 0 To 5
    For j = i + 1 To 6
      gB(k, 7) = 1
      gB(k, j) = -1
      If i > 0 Then
        gB(k, i) = 1
      End If
      k = k + 1
    Next
  Next
End Sub

Private Sub MakeNU()
  Dim i As Integer, j As Integer, k As Integer
  Dim dbl As Double
  '���N
  For i = 1 To 7
    For j = i To 7
      dbl = 0
      For k = 1 To 21
        dbl = dbl + gB(k, i) * gB(k, j)
      Next
      gN(i, j) = dbl
      If i <> j Then
        gN(j, i) = dbl
      End If
    Next
  Next
  '���U
  For i = 1 To 7
    dbl = 0
    For k = 1 To 21
      dbl = dbl + gB(k, i) * gL(k)
    Next
    gU(i) = dbl
  Next
End Sub

Private Sub CalQ()
  Dim i As Integer, j As Integer, k As Integer
  Dim dbl As Double
  Dim dblN(1 To 7, 1 To 7) As Double
  '��ʼ��Ϊ��λ��
  For i = 1 To 7
    gQ(i, i) = 1
    For j = i + 1 To 7
      gQ(i, j) = 0
      gQ(j, i) = 0
    Next
  Next
  '����N
  For i = 1 To 7
    For j = 1 To 7
      dblN(i, j) = gN(i, j)
    Next
  Next
  '������
  For k = 1 To 7
    dbl = 1# / dblN(k, k)
    For j = k + 1 To 7
      dblN(k, j) = dblN(k, j) * dbl
    Next
    For j = 1 To 7
      gQ(k, j) = gQ(k, j) * dbl
    Next
    For i = 1 To 7
      If i <> k Then
        dbl = -dblN(i, k)
        For j = k + 1 To 7
          dblN(i, j) = dblN(i, j) + dblN(k, j) * dbl
        Next
        For j = 1 To 7
          gQ(i, j) = gQ(i, j) + gQ(k, j) * dbl
        Next
      End If
    Next
  Next
End Sub

Private Sub CalX()
  Dim i As Integer, j As Integer
  Dim dbl As Double
  
  For i = 1 To 7
    dbl = 0
    For j = 1 To 7
      dbl = dbl + gQ(i, j) * gU(j)
    Next
    gX(i) = -dbl
  Next
End Sub

Private Sub CalM0()
  Dim i As Integer, j As Integer, k As Integer
  Dim v As Double, VTV As Double
  '����[VV]
  VTV = 0
  k = 1
  For i = 0 To 5
    For j = i + 1 To 6
      v = gX(7) + gL(k) - gX(j)
      If i > 0 Then
        v = v + gX(i)
      End If
      k = k + 1
      VTV = VTV + v * v
    Next
  Next
  gM0 = Sqr(VTV / 14)
End Sub

