//LinkMatrix.cpp
#include "stdafx.h"
#include "LinkMatrix.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CLinkPath::CLinkPath()
{
	Empty();
}

void CLinkPath::Empty()
{
	m_Count	=	0;
}

void CLinkPath::AddPoint(int x,int y)
{
	BOOL	bAdd	=	FALSE;

	if(m_Count >= 4)
	{
	}
	else if(m_Count)
	{
		bAdd	=	(x!=m_x[m_Count - 1] || y!=m_y[m_Count - 1]);
	}
	else
	{
		bAdd	=	TRUE;
	}
	if(bAdd)
	{
		m_x[m_Count]	=	x;
		m_y[m_Count]	=	y;
		m_Count++;
	}
}

CLinkMatrix::CLinkMatrix()
{
	m_pMatrix	=	NULL;
	m_iRows		=
	m_iCols		=	0;
	m_xSel		=
	m_ySel		=	0;
	m_iMoveMode	=	0;
}

CLinkMatrix::~CLinkMatrix()
{
	Empty();
}

void CLinkMatrix::Empty()
{
	if(m_pMatrix)
	{
		delete[] m_pMatrix;
		m_pMatrix	=	NULL;
	}
	m_iRows		=	0;
	m_iCols		=	0;
}

/*************************************************\
��ʼ���������������֣�
nRows		[in]	����
nCols		[in]	����
nIconCount	[in]	ͼ���� (С�� 1 ��ϴ��)
\*************************************************/
void CLinkMatrix::Init(int nRows,int nCols,int nIconCount)
{
	if(nRows < 1 || nCols < 1)
	{//����������̫С
		Empty();
		return;
	}
	if((nRows & 1) && (nCols & 1))
	{//������������Ϊ����
		Empty();
		return;
	}
	DWORD	dwCountOld	=	(m_iRows + 2) * (m_iCols + 2);
	DWORD	dwCountNew	=	(nRows + 2) * (nCols + 2);
	if(NULL==m_pMatrix || dwCountOld < dwCountNew)
	{//��Ҫ���·����ڴ�
		if(m_pMatrix)
		{
			delete[] m_pMatrix;
		}
		m_pMatrix	=	new ICONDATA[dwCountNew];
	}
	memset(m_pMatrix,0,dwCountNew * sizeof(ICONDATA));
	m_iRows		=	nRows;
	m_iCols		=	nCols;
	m_xSel		=
	m_ySel		=	0;
	if(nIconCount > 0)
	{//ϴ��
		srand(time(NULL) + GetTickCount());
		int			i,j,n,p;
		int			nIconTotal;
		CDWordArray arrIcon;

		do 
		{
			nIconTotal	=	m_iRows * m_iCols;						//�ܵ�ͼƬ��
			n			=	(nIconTotal / (nIconCount << 1)) << 1;	//ÿ��ͼƬ����� n ��
			p			=	0;
			arrIcon.SetSize(nIconTotal);
			for(i=1;i <= nIconCount;i++)
			{
				for(j=n;j>0;j--)
				{
					arrIcon[p++]=i;
				}
			}
			i	=	1;
			while(p < nIconTotal)
			{
				arrIcon[p++]	=	i;
				arrIcon[p++]	=	i;
				i++;
			}
			for(i=1;i<=m_iRows;i++)
			{
				for(j=1;j<=m_iCols;j++)
				{
					p				=	rand() % (nIconTotal--);
					operator()(j,i)	=	(ICONDATA)arrIcon.GetAt(p);
					arrIcon.RemoveAt(p);
				}
			}
		}while(0==GetHelp());
	}
}

/*************************************************\
ˮƽ�����Ƿ���ͨ
[in]	y		ˮƽ�� y ����
[in]	xFrom	ˮƽ����ʼ x ����
[in]	xTo		ˮƽ����ֹ x ����
\*************************************************/
BOOL CLinkMatrix::IsBlankH(int y,int xFrom,int xTo)
{
	for(int x=xFrom;x<=xTo;x++)
	{
		if(operator()(x,y))
		{
			return FALSE;
		}
	}
	return TRUE;
}

/*************************************************\
��ֱ�����Ƿ���ͨ
[in]	x		��ֱ�� x ����
[in]	yFrom	��ֱ����ʼ y ����
[in]	yTo		��ֱ����ֹ y ����
\*************************************************/
BOOL CLinkMatrix::IsBlankV(int x,int yFrom,int yTo)
{
	for(int y=yFrom;y<=yTo;y++)
	{
		if(operator()(x,y))
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CLinkMatrix::FindPath(int x1, int y1, int x2, int y2,CLinkPath*pPath)
{
	if(pPath)
	{
		pPath->Empty();
	}
	if(x1 < 1 || x1 > m_iCols
	|| x2 < 1 || x2 > m_iCols
	|| y1 < 1 || y1 > m_iRows
	|| y2 < 1 || y2 > m_iRows)
	{//�ڷ�Χ�⣬��ֱ�ӷ��� FALSE
		return FALSE;
	}
	RECT	rect;		//��Ӿ���
	int		iFlag	=	0;

	if(x1 <= x2)
	{
		rect.left	=	x1;
		rect.right	=	x2;
	}
	else
	{
		iFlag	   |=	1;
		rect.left	=	x2;
		rect.right	=	x1;
	}
	if(y1 <= y2)
	{
		rect.top	=	y1;
		rect.bottom	=	y2;
	}
	else
	{
		iFlag	   |=	2;
		rect.top	=	y2;
		rect.bottom	=	y1;
	}
	BOOL	b45		=	(0==iFlag || 3==iFlag);	//��㵽�յ�������Ƿ�Ϊ��\��(���п���Ϊ��/��)
	int		x,y;

	if(x1==x2)
	{//��㵽�յ�����Ϊ��|��
		if(y1==y2)
		{//�����յ��غ�
			return FALSE;
		}
		if(IsBlankV(x1,rect.top + 1,rect.bottom - 1))
		{
			if(pPath)
			{
				pPath->AddPoint(x1,rect.top);
				pPath->AddPoint(x1,rect.bottom);
			}
			return TRUE;
		}
	}
	if(y1==y2)
	{//��㵽�յ�����Ϊ��-��
		if(IsBlankH(y1,rect.left + 1,rect.right - 1))
		{
			if(pPath)
			{
				pPath->AddPoint(rect.left,y1);
				pPath->AddPoint(rect.right,y1);
			}
			return TRUE;
		}
	}
	if(x1!=x2 && y1!=y2)
	{//�ڲ����
		if(b45)
		{//��㵽�յ�����Ϊ��\��
			//������
			for(x=rect.left;x<=rect.right;x++)
			{
				if(IsBlankH(rect.top,rect.left + 1,x)
				&& IsBlankV(x,rect.top + 1,rect.bottom - 1)
				&& IsBlankH(rect.bottom,x,rect.right - 1))
				{
					if(pPath)
					{
						pPath->AddPoint(rect.left,rect.top);
						pPath->AddPoint(x,rect.top);
						pPath->AddPoint(x,rect.bottom);
						pPath->AddPoint(rect.right,rect.bottom);
					}
					return TRUE;
				}
			}
			//���ϵ���
			for(y=rect.top;y<=rect.bottom;y++)
			{
				if(IsBlankV(rect.left,rect.top + 1,y)
				&& IsBlankH(y,rect.left + 1,rect.right - 1)
				&& IsBlankV(rect.right,y,rect.bottom - 1))
				{
					if(pPath)
					{
						pPath->AddPoint(rect.left,rect.top);
						pPath->AddPoint(rect.left,y);
						pPath->AddPoint(rect.right,y);
						pPath->AddPoint(rect.right,rect.bottom);
					}
					return TRUE;
				}
			}
		}
		else
		{//��㵽�յ�����Ϊ��/��
			//������
			for(x=rect.left;x<=rect.right;x++)
			{
				if(IsBlankH(rect.bottom,rect.left + 1,x)
				&& IsBlankV(x,rect.top + 1,rect.bottom - 1)
				&& IsBlankH(rect.top,x,rect.right - 1))
				{
					if(pPath)
					{
						pPath->AddPoint(rect.right,rect.top);
						pPath->AddPoint(x,rect.top);
						pPath->AddPoint(x,rect.bottom);
						pPath->AddPoint(rect.left,rect.bottom);
					}
					return TRUE;
				}
			}
			//���ϵ���
			for(y=rect.top;y<=rect.bottom;y++)
			{
				if(IsBlankV(rect.right,rect.top + 1,y)
				&& IsBlankH(y,rect.left + 1,rect.right - 1)
				&& IsBlankV(rect.left,y,rect.bottom - 1))
				{
					if(pPath)
					{
						pPath->AddPoint(rect.left,rect.bottom);
						pPath->AddPoint(rect.left,y);
						pPath->AddPoint(rect.right,y);
						pPath->AddPoint(rect.right,rect.top);
					}
					return TRUE;
				}
			}
		}
	}
	if(b45)
	{//��㵽�յ�����Ϊ��\��
		if(y1!=y2)
		{
			//������
			for(x=rect.right + 1;x<=m_iCols+1;x++)
			{
				if(IsBlankH(rect.top,rect.left + 1,x)
				&& IsBlankV(x,rect.top + 1,rect.bottom - 1)
				&& IsBlankH(rect.bottom,rect.right + 1,x))
				{
					if(pPath)
					{
						pPath->AddPoint(rect.left,rect.top);
						pPath->AddPoint(x,rect.top);
						pPath->AddPoint(x,rect.bottom);
						pPath->AddPoint(rect.right,rect.bottom);
					}
					return TRUE;
				}
			}
			//������
			for(x=rect.left - 1;x>=0;x--)
			{
				if(IsBlankH(rect.top,x,rect.left - 1)
				&& IsBlankV(x,rect.top + 1,rect.bottom - 1)
				&& IsBlankH(rect.bottom,x,rect.right - 1))
				{
					if(pPath)
					{
						pPath->AddPoint(rect.left,rect.top);
						pPath->AddPoint(x,rect.top);
						pPath->AddPoint(x,rect.bottom);
						pPath->AddPoint(rect.right,rect.bottom);
					}
					return TRUE;
				}
			}
		}
		if(x1!=x2)
		{
			//������
			for(y=rect.top-1;y>=0;y--)
			{
				if(IsBlankV(rect.left,y,rect.top - 1)
				&& IsBlankH(y,rect.left + 1,rect.right - 1)
				&& IsBlankV(rect.right,y,rect.bottom - 1))
				{
					if(pPath)
					{
						pPath->AddPoint(rect.left,rect.top);
						pPath->AddPoint(rect.left,y);
						pPath->AddPoint(rect.right,y);
						pPath->AddPoint(rect.right,rect.bottom);
					}
					return TRUE;
				}
			}
			//������
			for(y=rect.bottom + 1;y<=m_iRows + 1;y++)
			{
				if(IsBlankV(rect.left,rect.top + 1,y)
				&& IsBlankH(y,rect.left + 1,rect.right - 1)
				&& IsBlankV(rect.right,rect.bottom + 1,y))
				{
					if(pPath)
					{
						pPath->AddPoint(rect.left,rect.top);
						pPath->AddPoint(rect.left,y);
						pPath->AddPoint(rect.right,y);
						pPath->AddPoint(rect.right,rect.bottom);
					}
					return TRUE;
				}
			}
		}
	}
	else
	{//��㵽�յ�����Ϊ��/��
		if(y1!=y2)
		{
			//������
			for(x=rect.right + 1;x<=m_iCols+1;x++)
			{
				if(IsBlankH(rect.bottom,rect.left + 1,x)
				&& IsBlankV(x,rect.top + 1,rect.bottom - 1)
				&& IsBlankH(rect.top,rect.right + 1,x))
				{
					if(pPath)
					{
						pPath->AddPoint(rect.right,rect.top);
						pPath->AddPoint(x,rect.top);
						pPath->AddPoint(x,rect.bottom);
						pPath->AddPoint(rect.left,rect.bottom);
					}
					return TRUE;
				}
			}
			//������
			for(x=rect.left - 1;x>=0;x--)
			{
				if(IsBlankH(rect.bottom,x,rect.left - 1)
				&& IsBlankV(x,rect.top + 1,rect.bottom - 1)
				&& IsBlankH(rect.top,x,rect.right - 1))
				{
					if(pPath)
					{
						pPath->AddPoint(rect.right,rect.top);
						pPath->AddPoint(x,rect.top);
						pPath->AddPoint(x,rect.bottom);
						pPath->AddPoint(rect.left,rect.bottom);
					}
					return TRUE;
				}
			}
		}
		if(x1!=x2)
		{
			//������
			for(y=rect.top-1;y>=0;y--)
			{
				if(IsBlankV(rect.right,y,rect.top - 1)
				&& IsBlankH(y,rect.left + 1,rect.right - 1)
				&& IsBlankV(rect.left,y,rect.bottom - 1))
				{
					if(pPath)
					{
						pPath->AddPoint(rect.left,rect.bottom);
						pPath->AddPoint(rect.left,y);
						pPath->AddPoint(rect.right,y);
						pPath->AddPoint(rect.right,rect.top);
					}
					return TRUE;
				}
			}
			//������
			for(y=rect.bottom + 1;y<=m_iRows + 1;y++)
			{
				if(IsBlankV(rect.right,rect.top + 1,y)
				&& IsBlankH(y,rect.left + 1,rect.right - 1)
				&& IsBlankV(rect.left,rect.bottom + 1,y))
				{
					if(pPath)
					{
						pPath->AddPoint(rect.left,rect.bottom);
						pPath->AddPoint(rect.left,y);
						pPath->AddPoint(rect.right,y);
						pPath->AddPoint(rect.right,rect.top);
					}
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

/**************************************************\
��ȡ��ʾ
����
-1		ʧ�ܣ��Ѿ�û��ͼ���ˣ�
0		ʧ��
1		�ɹ�����ʾ·���� pPath ��
\**************************************************/
int CLinkMatrix::GetHelp(CLinkPath*pPath)
{
	int	nHaveIcon = -1;
	if(pPath)
	{
		pPath->Empty();
	}
	int	x1,y1;	//���
	int	x2,y2;	//�յ�
	int	f	=	XYtoIndex(1,1);
	int	t	=	XYtoIndex(m_iCols,m_iRows);
	int	i,j;
	ICONDATA nIcon;

	for(i=f;i<=t;i++)
	{
		if(nIcon=m_pMatrix[i])
		{
			nHaveIcon	=	0;
			IndexToXY(i,x1,y1);
			for(j=i+1;j<=t;j++)
			{
				if(nIcon==m_pMatrix[j])
				{
					IndexToXY(j,x2,y2);
					if(FindPath(x1,y1,x2,y2,pPath))
						return 1;
				}
			}
		}
	}
	return nHaveIcon;
}

/**************************************************\
���²���
\**************************************************/
void CLinkMatrix::ChangeLayout()
{
	CDWordArray	arrX;
	CDWordArray	arrY;
	CDWordArray	arrIcon;
	int			x,y,nIcon;
	int			i,p;

	do 
	{
		for(y=1;y<=m_iRows;y++)
		{
			for(x=1;x<=m_iCols;x++)
			{
				if(nIcon=operator()(x,y))
				{
					arrIcon.Add(nIcon);
					arrX.Add(x);
					arrY.Add(y);
				}
			}
		}
		for(i=arrIcon.GetSize() - 1;i>=0;i--)
		{
			p	=	rand() % (i + 1);
			operator()(arrX[i],arrY[i])	=	(ICONDATA)arrIcon[p];
			arrIcon.RemoveAt(p);
		}
		arrX.RemoveAll();
		arrY.RemoveAll();
	}while(0==GetHelp());
}

/**************************************************\
��ȡ��ʾ
����
0		ʧ��
1		�ɹ�����ʾ·���� pPath ��
\**************************************************/
int CLinkMatrix::GetHelp(int x0,int y0,CLinkPath*pPath)
{
	if(pPath)
	{
		pPath->Empty();
	}
	if(x0 < 1 || x0 > m_iCols
	|| y0 < 1 || y0 > m_iRows)
	{//�ڷ�Χ�⣬��ֱ�ӷ��� FALSE
		return 0;
	}
	ICONDATA nIcon	=	operator()(x0,y0);
	if(nIcon)
	{
		int x,y;
		int	f	=	XYtoIndex(1,1);
		int	t	=	XYtoIndex(m_iCols,m_iRows);
		int	i;
		for(i=f;i<=t;i++)
		{
			if(nIcon==m_pMatrix[i])
			{
				IndexToXY(i,x,y);
				if(FindPath(x0,y0,x,y,pPath))
					return 1;
			}
		}
	}
	return 0;
}

CString CLinkMatrix::GetMoveModeStr()
{
	switch(m_iMoveMode)
	{
		case MOVE_Up:			return _T("����");
		case MOVE_Down:			return _T("����");
		case MOVE_Left:			return _T("����");
		case MOVE_Right:		return _T("����");
		case MOVE_LU:			return _T("������");
		case MOVE_LD:			return _T("������");
		case MOVE_RU:			return _T("������");
		case MOVE_RD:			return _T("������");
		case MOVE_ULDR:			return _T("��������");
		case MOVE_URDL:			return _T("��������");
		case MOVE_LDRU:			return _T("��������");
		case MOVE_LURD:			return _T("��������");
		case MOVE_FenLiH:		return _T("�������");
		case MOVE_FenLiV:		return _T("�������");
		case MOVE_JiZhongH:		return _T("������");
		case MOVE_JiZhongV:		return _T("������");
		case MOVE_DengFenH:		return _T("����ȷ�");
		case MOVE_DengFenV:		return _T("����ȷ�");
		case MOVE_JuZhongH:		return _T("�������");
		case MOVE_JuZhongV:		return _T("�������");
		case MOVE_HCuo:			return _T("�����(����������,ż��������)");
		case MOVE_VCuo:			return _T("�����(����������,ż��������)");
		case MOVE_HCuoFen:		return _T("�����(�����з���,ż���м���)");
		case MOVE_VCuoFen:		return _T("�����(�����з���,ż���м���)");
		case MOVE_HCuoDeng:		return _T("�����(�����еȷ�,ż���о���)");
		case MOVE_VCuoDeng:		return _T("�����(�����еȷ�,ż���о���)");
		case MOVE_KuoSanHV:		return _T("��ɢ(ÿ�еȷ�,Ȼ��ÿ�еȷ�)");
		case MOVE_KuoSanVH:		return _T("��ɢ(ÿ�еȷ�,Ȼ��ÿ�еȷ�)");
		case MOVE_JiZhongHV:	return _T("����(ÿ�о���,Ȼ��ÿ�о���)");
		case MOVE_JiZhongVH:	return _T("����(ÿ�о���,Ȼ��ÿ�о���)");
		case MOVE_ZUp:			return _T("Z ������");
		case MOVE_ZDown:		return _T("Z ������");
		case MOVE_NLeft:		return _T("N ������");
		case MOVE_NRight:		return _T("N ������");
		case MOVE_SUp:			return _T("S ������");
		case MOVE_SDown:		return _T("S ������");
		case MOVE_RotL:			return _T("��ʱ�������ת");
		case MOVE_RotR:			return _T("˳ʱ�������ת");
		case MOVE_RotLR:		return _T("���������ת");
		case MOVE_RotNLuoW:		return _T("��ʱ����������");
		case MOVE_RotNLuoN:		return _T("��ʱ����������");
		case MOVE_RotSLuoW:		return _T("˳ʱ����������");
		case MOVE_RotSLuoN:		return _T("˳ʱ����������");
	}
	return _T("���仯");
}

void CLinkMatrix::RemoveIcon(int x1,int y1,int x2,int y2)
{
	m_xSel	=
	m_ySel	=	0;
	operator()(x1,y1)=0;
	operator()(x2,y2)=0;
	switch(m_iMoveMode)
	{
		case MOVE_Up:		//����
			MoveUp(x1,y1,x2,y2);	break;
		case MOVE_Down:		//����
			MoveDown(x1,y1,x2,y2);	break;
		case MOVE_Left:		//����
			MoveLeft(x1,y1,x2,y2);	break;
		case MOVE_Right:	//����
			MoveRight(x1,y1,x2,y2);	break;
		case MOVE_LU:		//������
			MoveLU(x1,y1,x2,y2);	break;
		case MOVE_LD:		//������
			MoveLD(x1,y1,x2,y2);	break;
		case MOVE_RU:		//������
			MoveRU(x1,y1,x2,y2);	break;
		case MOVE_RD:		//������
			MoveRD(x1,y1,x2,y2);	break;
		case MOVE_ULDR:		//��������
			MoveULDR(x1,y1,x2,y2);	break;
		case MOVE_URDL:		//��������
			MoveURDL(x1,y1,x2,y2);	break;
		case MOVE_LDRU:		//��������
			MoveLDRU(x1,y1,x2,y2);	break;
		case MOVE_LURD:		//��������
			MoveLURD(x1,y1,x2,y2);	break;
		case MOVE_FenLiH:	//�з���
			MoveFenLiH(x1,y1,x2,y2);break;
		case MOVE_FenLiV:	//�з���
			MoveFenLiV(x1,y1,x2,y2);break;
		case MOVE_JiZhongH:	//�м���
			MoveJiZhongH(x1,y1,x2,y2);break;
		case MOVE_JiZhongV:	//�м���
			MoveJiZhongV(x1,y1,x2,y2);break;
		case MOVE_DengFenH:	//�еȷ�
			MoveDengH(x1,y1,x2,y2);break;
		case MOVE_DengFenV:	//�еȷ�
			MoveDengV(x1,y1,x2,y2);break;
		case MOVE_JuZhongH:	//�о���
			MoveJuZhongH(x1,y1,x2,y2);break;
		case MOVE_JuZhongV:	//�о���
			MoveJuZhongV(x1,y1,x2,y2);break;
		case MOVE_HCuo:		//�д�(����ż��)
			MoveCuoH(x1,y1,x2,y2);break;
		case MOVE_VCuo:		//�д�(����ż��)
			MoveCuoV(x1,y1,x2,y2);break;
		case MOVE_HCuoFen:	//�д�(�����ż����)
			MoveCuoFenH(x1,y1,x2,y2);break;
		case MOVE_VCuoFen:	//�д�(�����ż����)
			MoveCuoFenV(x1,y1,x2,y2);break;
		case MOVE_HCuoDeng:	//�д�(��ȷ�ż����)
			MoveCuoDengH(x1,y1,x2,y2);break;
		case MOVE_VCuoDeng:	//�д�(��ȷ�ż����)
			MoveCuoDengV(x1,y1,x2,y2);break;
		case MOVE_KuoSanHV:	//��ɢ(���к���)
			MoveKuoSanHV(x1,y1,x2,y2);break;
		case MOVE_KuoSanVH:	//��ɢ(���к���)
			MoveKuoSanVH(x1,y1,x2,y2);break;
		case MOVE_JiZhongHV://����(���к���)
			MoveJiZhongHV(x1,y1,x2,y2);break;
		case MOVE_JiZhongVH://����(���к���)
			MoveJiZhongVH(x1,y1,x2,y2);break;
		case MOVE_ZUp:		//Z ������
			MoveZUpDown(TRUE);break;
		case MOVE_ZDown:	//Z ������
			MoveZUpDown(FALSE);break;
		case MOVE_NLeft:	//N ������
			MoveNLeftRight(TRUE);break;
		case MOVE_NRight:	//N ������
			MoveNLeftRight(FALSE);break;
		case MOVE_SUp:		//S ������
			MoveSUpDown(TRUE);break;
		case MOVE_SDown:	//S ������
			MoveSUpDown(FALSE);break;
		case MOVE_RotL:		//��ʱ�������ת
			MoveRotL(x1,y1,x2,y2);break;
		case MOVE_RotR:		//˳ʱ�������ת
			MoveRotR(x1,y1,x2,y2);break;
		case MOVE_RotLR:	//���������ת
			MoveRotLR(x1,y1,x2,y2);break;
		case MOVE_RotNLuoW:	//��ʱ����������
			MoveRotLuo2(TRUE);break;
		case MOVE_RotNLuoN:	//��ʱ����������
			MoveRotLuo1(TRUE);break;
		case MOVE_RotSLuoW:	//˳ʱ����������
			MoveRotLuo1(FALSE);break;
		case MOVE_RotSLuoN:	//˳ʱ����������
			MoveRotLuo2(FALSE);break;
	}
}

void CLinkMatrix::MoveDown(int x1,int y1,int x2,int y2)
{
	MoveDown(x1);
	if(x1!=x2)
	{
		MoveDown(x2);
	}
}

void CLinkMatrix::MoveLeft(int x1,int y1,int x2,int y2)
{
	MoveLeft(y1);
	if(y1!=y2)
	{
		MoveLeft(y2);
	}
}

/***********************************************\
�����ƶ�һ��
\***********************************************/
void CLinkMatrix::MoveUp(int x)
{
	int	p,y;

	for(p = y = 1;y <= m_iRows;y++)
	{
		ICONDATA&nIcon=operator()(x,y);
		if(nIcon)
		{
			if(y > p)
			{
				operator()(x,p)	=	nIcon;
				nIcon			=	0;
			}
			p++;
		}
	}
}

/***********************************************\
�����ƶ�һ��
\***********************************************/
void CLinkMatrix::MoveDown(int x)
{
	int	p,y;

	for(p = y = m_iRows;y > 0;y--)
	{
		ICONDATA&nIcon=operator()(x,y);
		if(nIcon)
		{
			if(y < p)
			{
				operator()(x,p)	=	nIcon;
				nIcon			=	0;
			}
			p--;
		}
	}
}

/***********************************************\
�����ƶ�һ��
\***********************************************/
void CLinkMatrix::MoveLeft(int y)
{
	int	p,x;

	for(p = x = 1;x <= m_iCols;x++)
	{
		ICONDATA&nIcon=operator()(x,y);
		if(nIcon)
		{
			if(x > p)
			{
				operator()(p,y)	=	nIcon;
				nIcon			=	0;
			}
			p++;
		}
	}
}

/***********************************************\
�����ƶ�һ��
\***********************************************/
void CLinkMatrix::MoveRight(int y)
{
	int	p,x;

	for(p = x = m_iCols;x > 0;x--)
	{
		ICONDATA&nIcon=operator()(x,y);
		if(nIcon)
		{
			if(x < p)
			{
				operator()(p,y)	=	nIcon;
				nIcon			=	0;
			}
			p--;
		}
	}
}

void CLinkMatrix::MoveUp(int x1,int y1,int x2,int y2)
{
	MoveUp(x1);
	if(x1!=x2)
	{
		MoveUp(x2);
	}
}

void CLinkMatrix::MoveRight(int x1,int y1,int x2,int y2)
{
	MoveRight(y1);
	if(y1!=y2)
	{
		MoveRight(y2);
	}
}

void CLinkMatrix::SetMoveMode(int iMode)
{
	m_iMoveMode = iMode % MOVE_COUNT;
	if(m_iMoveMode < 0)
	{
		m_iMoveMode += MOVE_COUNT;
	}
}

//����һ��
void CLinkMatrix::MoveFenLiRow(int y)
{
	int	p,x;
	int	xm	=	(m_iCols >> 1);

	for(p = x = 1;x <= xm;x++)
	{
		ICONDATA&nIcon=operator()(x,y);
		if(nIcon)
		{
			if(x > p)
			{
				operator()(p,y)	=	nIcon;
				nIcon			=	0;
			}
			p++;
		}
	}
	for(p = x = m_iCols;x > xm;x--)
	{
		ICONDATA&nIcon=operator()(x,y);
		if(nIcon)
		{
			if(x < p)
			{
				operator()(p,y)	=	nIcon;
				nIcon			=	0;
			}
			p--;
		}
	}
}

//����һ��
void CLinkMatrix::MoveFenLiCol(int x)
{
	int	p,y;
	int	ym	=	(m_iRows >> 1);

	for(p = y = m_iRows;y > ym;y--)
	{//������
		ICONDATA&nIcon=operator()(x,y);
		if(nIcon)
		{
			if(y < p)
			{
				operator()(x,p)	=	nIcon;
				nIcon			=	0;
			}
			p--;
		}
	}
	for(p = y = 1;y <= ym;y++)
	{//������
		ICONDATA&nIcon=operator()(x,y);
		if(nIcon)
		{
			if(y > p)
			{
				operator()(x,p)	=	nIcon;
				nIcon			=	0;
			}
			p++;
		}
	}
}

//����һ��
void CLinkMatrix::MoveJiZhongRow(int y)
{
	int	p,x;
	int	xm	=	(m_iCols >> 1);

	for(p = x = xm;x >= 1;x--)
	{
		ICONDATA&nIcon=operator()(x,y);
		if(nIcon)
		{
			if(x < p)
			{
				operator()(p,y)	=	nIcon;
				nIcon			=	0;
			}
			p--;
		}
	}
	for(p = x = xm + 1;x <= m_iCols;x++)
	{
		ICONDATA&nIcon=operator()(x,y);
		if(nIcon)
		{
			if(x > p)
			{
				operator()(p,y)	=	nIcon;
				nIcon			=	0;
			}
			p++;
		}
	}
}

//����һ��
void CLinkMatrix::MoveJiZhongCol(int x)
{
	int	p,y;
	int	ym	=	(m_iRows >> 1);

	for(p = y = ym + 1;y <= m_iRows;y++)
	{//������
		ICONDATA&nIcon=operator()(x,y);
		if(nIcon)
		{
			if(y > p)
			{
				operator()(x,p)	=	nIcon;
				nIcon			=	0;
			}
			p++;
		}
	}
	for(p = y = ym;y >= 1;y--)
	{//������
		ICONDATA&nIcon=operator()(x,y);
		if(nIcon)
		{
			if(y < p)
			{
				operator()(x,p)	=	nIcon;
				nIcon			=	0;
			}
			p--;
		}
	}
}

//ͳ��һ�е���
int CLinkMatrix::CountIconRow(int y)
{
	int count	=	0;
	int	p		=	XYtoIndex(1,y);

	for(int x=1;x<=m_iCols;x++)
	{
		if(m_pMatrix[p++])
		{
			count++;
		}
	}
	return count;
}

//ͳ��һ�е���
int CLinkMatrix::CountIconCol(int x)
{
	int count	=	0;
	int	p		=	XYtoIndex(x,1);

	for(int y=1;y<=m_iRows;y++)
	{
		if(m_pMatrix[p])
		{
			count++;
		}
		p+=m_iCols+2;
	}
	return count;
}

//������(x-y=c)
void CLinkMatrix::MoveLU(int x1,int y1,int x2,int y2)
{
	int	c1	=	x1 - y1;
	int	c2	=	x2 - y2;
	MoveLU(c1);
	if(c1!=c2)
	{
		MoveLU(c2);
	}
}

//������(x-y=c)
void CLinkMatrix::MoveRD(int x1,int y1,int x2,int y2)
{
	int	c1	=	x1 - y1;
	int	c2	=	x2 - y2;
	MoveRD(c1);
	if(c1!=c2)
	{
		MoveRD(c2);
	}
}

//������(x+y=c)
void CLinkMatrix::MoveLD(int x1,int y1,int x2,int y2)
{
	int	c1	=	x1 + y1;
	int	c2	=	x2 + y2;
	MoveLD(c1);
	if(c1!=c2)
	{
		MoveLD(c2);
	}
}

//������(x+y=c)
void CLinkMatrix::MoveRU(int x1,int y1,int x2,int y2)
{
	int	c1	=	x1 + y1;
	int	c2	=	x2 + y2;
	MoveRU(c1);
	if(c1!=c2)
	{
		MoveRU(c2);
	}
}

//������(x-y=c)
void CLinkMatrix::MoveLU(int c)
{
	int	xMin	=	max(1,c + 1);
	int xMax	=	min(m_iCols,c + m_iRows);

	int x,y,p;
	for(y = p = (x = xMin) - c;x <= xMax;x++,y++)
	{
		ICONDATA&nIcon	=	operator()(x,y);
		if(nIcon)
		{
			if(y > p)
			{
				operator()(c+p,p)	=	nIcon;
				nIcon				=	0;
			}
			p++;
		}
	}
}

//������(x-y=c)
void CLinkMatrix::MoveRD(int c)
{
	int	xMin	=	max(1,c + 1);
	int xMax	=	min(m_iCols,c + m_iRows);

	int x,y,p;

	for(y = p = (x = xMax) - c;x >= xMin;x--,y--)
	{
		ICONDATA&nIcon	=	operator()(x,y);
		if(nIcon)
		{
			if(y < p)
			{
				operator()(c+p,p)	=	nIcon;
				nIcon				=	0;
			}
			p--;
		}
	}
}

//������(x+y=c)
void CLinkMatrix::MoveLD(int c)
{
	int	xMin	=	max(1,c - m_iRows);
	int xMax	=	min(m_iCols,c - 1);

	int x,y,p;

	for(y = p = c - (x = xMin);x <= xMax;x++,y--)
	{
		ICONDATA&nIcon	=	operator()(x,y);
		if(nIcon)
		{
			if(y < p)
			{
				operator()(c-p,p)	=	nIcon;
				nIcon				=	0;
			}
			p--;
		}
	}
}

//������(x+y=c)
void CLinkMatrix::MoveRU(int c)
{
	int	xMin	=	max(1,c - m_iRows);
	int xMax	=	min(m_iCols,c - 1);

	int x,y,p;

	for(y = p = c - (x = xMax);x >= xMin;x--,y++)
	{
		ICONDATA&nIcon	=	operator()(x,y);
		if(nIcon)
		{
			if(y > p)
			{
				operator()(c-p,p)	=	nIcon;
				nIcon				=	0;
			}
			p++;
		}
	}
}

//��������
void CLinkMatrix::MoveULDR(int x1,int y1,int x2,int y2)
{
	int	ym	=	(m_iRows >> 1);
	int y;
	int i	=	(y1==y2 ? 1 : 2);

	while(i--)
	{
		if(i)
		{
			y	=	y1;
		}
		else
		{
			y	=	y2;
		}
		if(y <= ym)
		{
			MoveLeft(y);
		}
		else
		{
			MoveRight(y);
		}
	}
}

//��������
void CLinkMatrix::MoveURDL(int x1,int y1,int x2,int y2)
{
	int	ym	=	(m_iRows >> 1);
	int y;
	int i	=	(y1==y2 ? 1 : 2);

	while(i--)
	{
		if(i)
		{
			y	=	y1;
		}
		else
		{
			y	=	y2;
		}
		if(y <= ym)
		{
			MoveRight(y);
		}
		else
		{
			MoveLeft(y);
		}
	}
}

//��������
void CLinkMatrix::MoveLDRU(int x1,int y1,int x2,int y2)
{
	int	xm	=	(m_iCols >> 1);
	int x;
	int i	=	(x1==x2 ? 1 : 2);

	while(i--)
	{
		if(i)
		{
			x	=	x1;
		}
		else
		{
			x	=	x2;
		}
		if(x <= xm)
		{
			MoveDown(x);
		}
		else
		{
			MoveUp(x);
		}
	}
}

//��������
void CLinkMatrix::MoveLURD(int x1,int y1,int x2,int y2)
{
	int	xm	=	(m_iCols >> 1);
	int x;
	int i	=	(x1==x2 ? 1 : 2);

	while(i--)
	{
		if(i)
		{
			x	=	x1;
		}
		else
		{
			x	=	x2;
		}
		if(x <= xm)
		{
			MoveUp(x);
		}
		else
		{
			MoveDown(x);
		}
	}
}

//�������
void CLinkMatrix::MoveFenLiH(int x1,int y1,int x2,int y2)
{
	MoveFenLiRow(y1);
	if(y1!=y2)
	{
		MoveFenLiRow(y2);
	}
}

//�������
void CLinkMatrix::MoveFenLiV(int x1,int y1,int x2,int y2)
{
	MoveFenLiCol(x1);
	if(x1!=x2)
	{
		MoveFenLiCol(x2);
	}
}

//������
void CLinkMatrix::MoveJiZhongH(int x1,int y1,int x2,int y2)
{
	MoveJiZhongRow(y1);
	if(y1!=y2)
	{
		MoveJiZhongRow(y2);
	}
}

//������
void CLinkMatrix::MoveJiZhongV(int x1,int y1,int x2,int y2)
{
	MoveJiZhongCol(x1);
	if(x1!=x2)
	{
		MoveJiZhongCol(x2);
	}
}

//����ȷ�
void CLinkMatrix::MoveDengH(int x1,int y1,int x2,int y2)
{
	MoveDengRow(y1);
	if(y1!=y2)
	{
		MoveDengRow(y2);
	}
}

//����ȷ�
void CLinkMatrix::MoveDengV(int x1,int y1,int x2,int y2)
{
	MoveDengCol(x1);
	if(x1!=x2)
	{
		MoveDengCol(x2);
	}
}

//�о���
void CLinkMatrix::MoveJuZhongH(int x1,int y1,int x2,int y2)
{
	MoveJuZhongRow(y1);
	if(y1!=y2)
	{
		MoveJuZhongRow(y2);
	}
}

//�о���
void CLinkMatrix::MoveJuZhongV(int x1,int y1,int x2,int y2)
{
	MoveJuZhongCol(x1);
	if(x1!=x2)
	{
		MoveJuZhongCol(x2);
	}
}

//�ȷ�һ��
void CLinkMatrix::MoveDengRow(int y)
{
	int	x;
	int p		=	XYtoIndex(1,y);
	int count	=	0;

	for(x = 1;x <= m_iCols;x++)
	{
		ICONDATA&nIcon	=	m_pMatrix[p++];
		if(nIcon)
		{
			m_pMatrix[count++]	=	nIcon;
		}
	}
	if(!count)
	{//���ƿɷ�
		return;
	}
	if(count < m_iCols)
	{
		int	CountL	=	(count >> 1);	//��ߵ�����
		int	CountR	=	count - CountL;	//�ұߵ�����
		p			=	XYtoIndex(1,y);

		if(CountL)
		{
			memcpy(m_pMatrix + p,m_pMatrix,sizeof(ICONDATA) * CountL);
		}
		memset(m_pMatrix + p + CountL,0,sizeof(ICONDATA) * (m_iCols - count));
		if(CountR)
		{
			memcpy(m_pMatrix + (p + CountL + m_iCols - count),m_pMatrix + CountL,sizeof(ICONDATA) * CountR);
		}
	}
	memset(m_pMatrix,0,sizeof(ICONDATA) * count);
}

//�ȷ�һ��
void CLinkMatrix::MoveDengCol(int x)
{
	int	y;
	int p		=	XYtoIndex(x,1);
	int count	=	0;

	for(y = 1;y <= m_iRows;y++)
	{
		ICONDATA&nIcon	=	m_pMatrix[p];
		if(nIcon)
		{
			m_pMatrix[count++ * (m_iCols + 2)]	=	nIcon;
		}
		p+=(m_iCols + 2);
	}
	if(!count)
	{//���ƿɷ�
		return;
	}
	if(count < m_iRows)
	{
		int	CountU	=	(count >> 1);	//�ϱߵ�����
		int	CountD	=	count - CountU;	//�±ߵ�����
		p			=	0;

		for(y=1;y <= CountU;y++)
		{
			operator()(x,y) =	m_pMatrix[p];
			p			   +=	(m_iCols + 2);
		}
		for(y = m_iRows - count;y > 0;y--)
		{
			operator()(x,CountU + y)	=	0;
		}
		for(y = CountD - 1;y >= 0;y--)
		{
			operator()(x,m_iRows - y) =	m_pMatrix[p];
			p			   +=	(m_iCols + 2);
		}
	}
	for(p = y = 0;y < count;y++)
	{
		m_pMatrix[p]	=	0;
		p			   +=	(m_iCols + 2);
	}
}

//����һ��
void CLinkMatrix::MoveJuZhongRow(int y)
{
	int	x;
	int p		=	XYtoIndex(1,y);
	int count	=	0;

	for(x = 1;x <= m_iCols;x++)
	{
		ICONDATA&nIcon	=	m_pMatrix[p++];
		if(nIcon)
		{
			m_pMatrix[count++]	=	nIcon;
		}
	}
	if(!count)
	{//���ƿɷ�
		return;
	}
	if(count < m_iCols)
	{
		p	=	XYtoIndex(1,y);
		memset(m_pMatrix + p,0,sizeof(ICONDATA) * m_iCols);
		memcpy(m_pMatrix + p + ((m_iCols - count)>>1)
			,m_pMatrix,sizeof(ICONDATA) * count);
	}
	memset(m_pMatrix,0,sizeof(ICONDATA) * count);
}

//����һ��
void CLinkMatrix::MoveJuZhongCol(int x)
{
	int	y;
	int p		=	XYtoIndex(x,1);
	int count	=	0;

	for(y = 1;y <= m_iRows;y++)
	{
		ICONDATA&nIcon	=	m_pMatrix[p];
		if(nIcon)
		{
			m_pMatrix[count++ * (m_iCols + 2)]	=	nIcon;
		}
		p+=(m_iCols + 2);
	}
	if(!count)
	{//���ƿɷ�
		return;
	}
	if(count < m_iRows)
	{
		p	=	0;
		int	yf	=	((m_iRows + 2 - count) >> 1);
		int	yt	=	yf + count - 1;
		for(y=1;y <= m_iRows;y++)
		{
			ICONDATA&nIcon	=	operator()(x,y);
			if(y < yf || y > yt)
			{
				nIcon	=	0;
			}
			else
			{
				nIcon	=	m_pMatrix[p];
				p	   +=	(m_iCols + 2);
			}
		}
	}
	for(p = y = 0;y < count;y++)
	{
		m_pMatrix[p]	=	0;
		p			   +=	(m_iCols + 2);
	}
}

//�д�(����ż��)
void CLinkMatrix::MoveCuoH(int x1,int y1,int x2,int y2)
{
	int y;
	int	i	=	(y1==y2 ? 1 : 2);

	while(i--)
	{
		if(i)
		{
			y	=	y1;
		}
		else
		{
			y	=	y2;
		}
		if(y & 1)
		{
			MoveLeft(y);
		}
		else
		{
			MoveRight(y);
		}
	}
}

//�д�(����ż��)
void CLinkMatrix::MoveCuoV(int x1,int y1,int x2,int y2)
{
	int x;
	int	i	=	(x1==x2 ? 1 : 2);

	while(i--)
	{
		if(i)
		{
			x	=	x1;
		}
		else
		{
			x	=	x2;
		}
		if(x & 1)
		{
			MoveUp(x);
		}
		else
		{
			MoveDown(x);
		}
	}
}

//�д�(�����ż����)
void CLinkMatrix::MoveCuoFenH(int x1,int y1,int x2,int y2)
{
	int y;
	int	i	=	(y1==y2 ? 1 : 2);

	while(i--)
	{
		if(i)
		{
			y	=	y1;
		}
		else
		{
			y	=	y2;
		}
		if(y & 1)
		{
			MoveFenLiRow(y);
		}
		else
		{
			MoveJiZhongRow(y);
		}
	}
}

//�д�(�����ż����)
void CLinkMatrix::MoveCuoFenV(int x1,int y1,int x2,int y2)
{
	int x;
	int	i	=	(x1==x2 ? 1 : 2);

	while(i--)
	{
		if(i)
		{
			x	=	x1;
		}
		else
		{
			x	=	x2;
		}
		if(x & 1)
		{
			MoveFenLiCol(x);
		}
		else
		{
			MoveJiZhongCol(x);
		}
	}
}

//�д�(��ȷ�ż����)
void CLinkMatrix::MoveCuoDengH(int x1,int y1,int x2,int y2)
{
	int y;
	int	i	=	(y1==y2 ? 1 : 2);

	while(i--)
	{
		if(i)
		{
			y	=	y1;
		}
		else
		{
			y	=	y2;
		}
		if(y & 1)
		{
			MoveDengRow(y);
		}
		else
		{
			MoveJuZhongRow(y);
		}
	}
}

//�д�(��ȷ�ż����)
void CLinkMatrix::MoveCuoDengV(int x1,int y1,int x2,int y2)
{
	int x;
	int	i	=	(x1==x2 ? 1 : 2);

	while(i--)
	{
		if(i)
		{
			x	=	x1;
		}
		else
		{
			x	=	x2;
		}
		if(x & 1)
		{
			MoveDengCol(x);
		}
		else
		{
			MoveJuZhongCol(x);
		}
	}
}

//��ɢ(���к���)
void CLinkMatrix::MoveKuoSanHV(int x1,int y1,int x2,int y2)
{
	for(int y=1;y<=m_iRows;y++)
	{
		MoveDengRow(y);
	}
	for(int x=1;x<=m_iCols;x++)
	{
		MoveDengCol(x);
	}
}

//��ɢ(���к���)
void CLinkMatrix::MoveKuoSanVH(int x1,int y1,int x2,int y2)
{
	for(int x=1;x<=m_iCols;x++)
	{
		MoveDengCol(x);
	}
	for(int y=1;y<=m_iRows;y++)
	{
		MoveDengRow(y);
	}
}

//����(���к���)
void CLinkMatrix::MoveJiZhongHV(int x1,int y1,int x2,int y2)
{
	for(int y=1;y<=m_iRows;y++)
	{
		MoveJuZhongRow(y);
	}
	for(int x=1;x<=m_iCols;x++)
	{
		MoveJuZhongCol(x);
	}
}

//����(���к���)
void CLinkMatrix::MoveJiZhongVH(int x1,int y1,int x2,int y2)
{
	for(int x=1;x<=m_iCols;x++)
	{
		MoveJuZhongCol(x);
	}
	for(int y=1;y<=m_iRows;y++)
	{
		MoveJuZhongRow(y);
	}
}

/************************************************\
���Ȧ�ı��(����һȦ���Ϊ 1,����������)
���� 0 ��ʾ����Ȧ��
\************************************************/
int CLinkMatrix::GetLoopNumber(int x,int y)
{
	if(x <= ((m_iCols + 1) >> 1))
	{//���
		if(y >= x && y <= m_iRows + 1 - x)
		{
			return x;
		}
	}
	else
	{//�ұ�
		if(y >= m_iCols + 1 - x && y <= x + m_iRows - m_iCols)
		{
			return m_iCols + 1 - x;
		}
	}
	if(y <= ((m_iRows + 1) >> 1))
	{//�ϱ�
		if(x >= y && x <= m_iCols + 1 - y)
		{
			return y;
		}
	}
	else
	{//�±�
		if(x >= m_iRows + 1 - y && x <= y + m_iCols - m_iRows)
		{
			return m_iRows + 1 - y;
		}
	}
	return 0;
}

//��ʱ�������ת
void CLinkMatrix::MoveRotL(int x1,int y1,int x2,int y2)
{
	int	L1	=	GetLoopNumber(x1,y1);
	if(L1)
	{
		MoveRotL(L1);
	}
	int	L2	=	GetLoopNumber(x2,y2);
	if(L2 && L1!=L2)
	{
		MoveRotL(L2);
	}
}

//˳ʱ�������ת
void CLinkMatrix::MoveRotR(int x1,int y1,int x2,int y2)
{
	int	L1	=	GetLoopNumber(x1,y1);
	if(L1)
	{
		MoveRotR(L1);
	}
	int	L2	=	GetLoopNumber(x2,y2);
	if(L2 && L1!=L2)
	{
		MoveRotR(L2);
	}
}

void CLinkMatrix::MoveRotL(int nLoop)
{
	int			xMin	=	nLoop;
	int			xMax	=	m_iCols + 1 - nLoop;
	int			yMin	=	nLoop;
	int			yMax	=	m_iRows + 1 - nLoop;
	int			x,y;
	int			nIndex;
	CDWordArray	arrIcon;
	CDWordArray	arrIndex;

	x	=	(m_iRows + m_iCols) << 1;
	arrIndex.SetSize(0,x);
	arrIcon.SetSize(0,x);

	//��ȡ��---------------------------------
	//��
	for(x = xMin,y = yMin;x <= xMax;x++)
	{
		arrIndex.Add(nIndex = XYtoIndex(x,y));
		ICONDATA&nIcon = m_pMatrix[nIndex];
		if(nIcon)
		{
			arrIcon.Add(nIcon);
		}
	}
	//��
	for(x = xMax,y = yMin + 1;y < yMax;y++)
	{
		arrIndex.Add(nIndex = XYtoIndex(x,y));
		ICONDATA&nIcon = m_pMatrix[nIndex];
		if(nIcon)
		{
			arrIcon.Add(nIcon);
		}
	}
	//��
	if(yMax > yMin)
	{
		for(x = xMax,y = yMax;x >= xMin;x--)
		{
			arrIndex.Add(nIndex = XYtoIndex(x,y));
			ICONDATA&nIcon = m_pMatrix[nIndex];
			if(nIcon)
			{
				arrIcon.Add(nIcon);
			}
		}
	}
	//��
	if(xMax > xMin)
	{
		for(x = xMin,y = yMax - 1;y > yMin;y--)
		{
			arrIndex.Add(nIndex = XYtoIndex(x,y));
			ICONDATA&nIcon = m_pMatrix[nIndex];
			if(nIcon)
			{
				arrIcon.Add(nIcon);
			}
		}
	}
	int	nCountIndex	=	arrIndex.GetSize();
	int	nCountIcon	=	arrIcon.GetSize();
	if(nCountIndex > nCountIcon)
	{
		int i;
		for(i=0;i<nCountIcon;i++)
		{
			m_pMatrix[arrIndex[i]]	=	(ICONDATA)arrIcon[i];
		}
		while(i < nCountIndex)
		{
			m_pMatrix[arrIndex[i++]]	=	0;
		}
	}
}

void CLinkMatrix::MoveRotR(int nLoop)
{
	int			xMin	=	nLoop;
	int			xMax	=	m_iCols + 1 - nLoop;
	int			yMin	=	nLoop;
	int			yMax	=	m_iRows + 1 - nLoop;
	int			x,y;
	int			nIndex;
	CDWordArray	arrIcon;
	CDWordArray	arrIndex;

	x	=	(m_iRows + m_iCols) << 1;
	arrIndex.SetSize(0,x);
	arrIcon.SetSize(0,x);

	//��ȡ��---------------------------------
	//��
	for(x = xMin,y = yMin;y <= yMax;y++)
	{
		arrIndex.Add(nIndex = XYtoIndex(x,y));
		ICONDATA&nIcon = m_pMatrix[nIndex];
		if(nIcon)
		{
			arrIcon.Add(nIcon);
		}
	}
	//��
	for(x = xMin + 1,y = yMax;x < xMax;x++)
	{
		arrIndex.Add(nIndex = XYtoIndex(x,y));
		ICONDATA&nIcon = m_pMatrix[nIndex];
		if(nIcon)
		{
			arrIcon.Add(nIcon);
		}
	}
	//��
	if(xMax > xMin)
	{
		for(x = xMax,y = yMax;y >= yMin;y--)
		{
			arrIndex.Add(nIndex = XYtoIndex(x,y));
			ICONDATA&nIcon = m_pMatrix[nIndex];
			if(nIcon)
			{
				arrIcon.Add(nIcon);
			}
		}
	}
	//��
	if(yMax > yMin)
	{
		for(x = xMax - 1,y = yMin;x > xMin;x--)
		{
			arrIndex.Add(nIndex = XYtoIndex(x,y));
			ICONDATA&nIcon = m_pMatrix[nIndex];
			if(nIcon)
			{
				arrIcon.Add(nIcon);
			}
		}
	}
	int	nCountIndex	=	arrIndex.GetSize();
	int	nCountIcon	=	arrIcon.GetSize();
	if(nCountIndex > nCountIcon)
	{
		int i;
		for(i=0;i<nCountIcon;i++)
		{
			m_pMatrix[arrIndex[i]]	=	(ICONDATA)arrIcon[i];
		}
		while(i < nCountIndex)
		{
			m_pMatrix[arrIndex[i++]]	=	0;
		}
	}
}

//���������ת
void CLinkMatrix::MoveRotLR(int x1,int y1,int x2,int y2)
{
	int	L1	=	GetLoopNumber(x1,y1);
	int	L2	=	GetLoopNumber(x2,y2);
	int L;
	int	i	=	L1 == L2 ? 1 : 2;

	while(i--)
	{
		if(i)
		{
			L	=	L1;
		}
		else
		{
			L	=	L2;
		}

		if(L)
		{
			if(L & 1)
			{
				MoveRotL(L);
			}
			else
			{
				MoveRotR(L);
			}
		}
	}
}

void CLinkMatrix::FillIcon(CDWordArray&arrIndex,CDWordArray&arrIcon,BOOL bLeft)
{
	int	p;
	int	CountIcon	=	arrIcon.GetSize();
	int	CountIndex	=	arrIndex.GetSize();

	if(0==CountIcon || CountIcon==CountIndex)
	{
		return;
	}
	if(bLeft)
	{
		for(p=0;p < CountIcon;p++)
		{
			m_pMatrix[arrIndex[p]]=(ICONDATA)arrIcon[p];
		}
		while(p < CountIndex)
		{
			m_pMatrix[arrIndex[p++]]=0;
		}
	}
	else
	{
		for(p=1;p <= CountIcon;p++)
		{
			m_pMatrix[arrIndex[CountIndex - p]]	=	(ICONDATA)arrIcon[CountIcon - p];
		}
		for(p = CountIndex - CountIcon - 1;p>=0;p--)
		{
			m_pMatrix[arrIndex[p]]=0;
		}
	}
}

//S ������
void CLinkMatrix::MoveSUpDown(BOOL bUp)
{
	CDWordArray	arrIndex;
	CDWordArray	arrIcon;
	int			c	=	m_iRows * m_iCols;

	arrIndex.SetSize(0,c);
	arrIcon.SetSize(0,c);
	int x,y,nIndex;
	for(y=1;y<=m_iRows;y++)
	{
		if(y & 1)
		{
			for(x=m_iCols;x>=1;x--)
			{
				nIndex		=	XYtoIndex(x,y);
				arrIndex.Add(nIndex);
				ICONDATA&nIcon	=	m_pMatrix[nIndex];
				if(nIcon)
				{
					arrIcon.Add(nIcon);
				}
			}
		}
		else
		{
			for(x=1;x<=m_iCols;x++)
			{
				nIndex		=	XYtoIndex(x,y);
				arrIndex.Add(nIndex);
				ICONDATA&nIcon	=	m_pMatrix[nIndex];
				if(nIcon)
				{
					arrIcon.Add(nIcon);
				}
			}
		}
	}
	FillIcon(arrIndex,arrIcon,bUp);
}

//Z ������
void CLinkMatrix::MoveZUpDown(BOOL bUp)
{
	CDWordArray	arrIndex;
	CDWordArray	arrIcon;
	int			c	=	m_iRows * m_iCols;

	arrIndex.SetSize(0,c);
	arrIcon.SetSize(0,c);
	int x,y,nIndex;
	for(y=1;y<=m_iRows;y++)
	{
		for(x=1;x<=m_iCols;x++)
		{
			nIndex		=	XYtoIndex(x,y);
			arrIndex.Add(nIndex);
			ICONDATA&nIcon	=	m_pMatrix[nIndex];
			if(nIcon)
			{
				arrIcon.Add(nIcon);
			}
		}
	}
	FillIcon(arrIndex,arrIcon,bUp);
}

//N ������
void CLinkMatrix::MoveNLeftRight(BOOL bLeft)
{
	CDWordArray	arrIndex;
	CDWordArray	arrIcon;
	int			c	=	m_iRows * m_iCols;

	arrIndex.SetSize(0,c);
	arrIcon.SetSize(0,c);
	int x,y,nIndex;
	for(x=1;x<=m_iCols;x++)
	{
		for(y=m_iRows;y>=1;y--)
		{
			nIndex		=	XYtoIndex(x,y);
			arrIndex.Add(nIndex);
			ICONDATA&nIcon	=	m_pMatrix[nIndex];
			if(nIcon)
			{
				arrIcon.Add(nIcon);
			}
		}
	}
	FillIcon(arrIndex,arrIcon,bLeft);
}

//˳ʱ����������/��ʱ����������
void CLinkMatrix::MoveRotLuo1(BOOL bNi)
{
	CDWordArray	arrIndex;
	CDWordArray	arrIcon;
	int			c	=	m_iRows * m_iCols;

	arrIndex.SetSize(0,c);
	arrIcon.SetSize(0,c);

	int x,y,nIndex;
	int xMin	=	1;
	int xMax	=	m_iCols;
	int yMin	=	1;
	int yMax	=	m_iRows;

	while(1)
	{
		if(xMin > xMax || yMin > yMax)
		{
			break;
		}
		if(xMin==xMax)
		{
			x	=	xMin;
			for(y=yMin;y<=yMax;y++)
			{
				nIndex		=	XYtoIndex(x,y);
				arrIndex.Add(nIndex);
				ICONDATA&nIcon	=	m_pMatrix[nIndex];
				if(nIcon)
				{
					arrIcon.Add(nIcon);
				}
			}
			break;
		}
		if(yMin==yMax)
		{
			y	=	yMin;
			for(x=xMin;x<=xMax;x++)
			{
				nIndex		=	XYtoIndex(x,y);
				arrIndex.Add(nIndex);
				ICONDATA&nIcon	=	m_pMatrix[nIndex];
				if(nIcon)
				{
					arrIcon.Add(nIcon);
				}
			}
			break;
		}
		//��
		x	=	xMin;
		for(y=yMin;y<=yMax;y++)
		{
			nIndex		=	XYtoIndex(x,y);
			arrIndex.Add(nIndex);
			ICONDATA&nIcon	=	m_pMatrix[nIndex];
			if(nIcon)
			{
				arrIcon.Add(nIcon);
			}
		}
		//��
		y	=	yMax;
		for(x=xMin + 1;x<=xMax;x++)
		{
			nIndex		=	XYtoIndex(x,y);
			arrIndex.Add(nIndex);
			ICONDATA&nIcon	=	m_pMatrix[nIndex];
			if(nIcon)
			{
				arrIcon.Add(nIcon);
			}
		}
		//��
		x	=	xMax;
		for(y=yMax-1;y>=yMin;y--)
		{
			nIndex		=	XYtoIndex(x,y);
			arrIndex.Add(nIndex);
			ICONDATA&nIcon	=	m_pMatrix[nIndex];
			if(nIcon)
			{
				arrIcon.Add(nIcon);
			}
		}
		//��
		y	=	yMin;
		for(x=xMax - 1;x > xMin;x--)
		{
			nIndex		=	XYtoIndex(x,y);
			arrIndex.Add(nIndex);
			ICONDATA&nIcon	=	m_pMatrix[nIndex];
			if(nIcon)
			{
				arrIcon.Add(nIcon);
			}
		}
		xMin++;
		yMin++;
		xMax--;
		yMax--;
	}
	FillIcon(arrIndex,arrIcon,!bNi);
}

//˳ʱ����������/��ʱ����������
void CLinkMatrix::MoveRotLuo2(BOOL bNi)
{
	CDWordArray	arrIndex;
	CDWordArray	arrIcon;
	int			c	=	m_iRows * m_iCols;

	arrIndex.SetSize(0,c);
	arrIcon.SetSize(0,c);

	int x,y,nIndex;
	int xMin	=	1;
	int xMax	=	m_iCols;
	int yMin	=	1;
	int yMax	=	m_iRows;

	while(1)
	{
		if(xMin > xMax || yMin > yMax)
		{
			break;
		}
		if(xMin==xMax)
		{
			x	=	xMin;
			for(y=yMin;y<=yMax;y++)
			{
				nIndex		=	XYtoIndex(x,y);
				arrIndex.Add(nIndex);
				ICONDATA&nIcon	=	m_pMatrix[nIndex];
				if(nIcon)
				{
					arrIcon.Add(nIcon);
				}
			}
			break;
		}
		if(yMin==yMax)
		{
			y	=	yMin;
			for(x=xMin;x<=xMax;x++)
			{
				nIndex		=	XYtoIndex(x,y);
				arrIndex.Add(nIndex);
				ICONDATA&nIcon	=	m_pMatrix[nIndex];
				if(nIcon)
				{
					arrIcon.Add(nIcon);
				}
			}
			break;
		}
		//��
		y	=	yMin;
		for(x=xMin;x <= xMax;x++)
		{
			nIndex		=	XYtoIndex(x,y);
			arrIndex.Add(nIndex);
			ICONDATA&nIcon	=	m_pMatrix[nIndex];
			if(nIcon)
			{
				arrIcon.Add(nIcon);
			}
		}
		//��
		x	=	xMax;
		for(y=yMin + 1;y<=yMax;y++)
		{
			nIndex		=	XYtoIndex(x,y);
			arrIndex.Add(nIndex);
			ICONDATA&nIcon	=	m_pMatrix[nIndex];
			if(nIcon)
			{
				arrIcon.Add(nIcon);
			}
		}
		//��
		y	=	yMax;
		for(x=xMax - 1;x>=xMin;x--)
		{
			nIndex		=	XYtoIndex(x,y);
			arrIndex.Add(nIndex);
			ICONDATA&nIcon	=	m_pMatrix[nIndex];
			if(nIcon)
			{
				arrIcon.Add(nIcon);
			}
		}
		//��
		x	=	xMin;
		for(y=yMax-1;y>yMin;y--)
		{
			nIndex		=	XYtoIndex(x,y);
			arrIndex.Add(nIndex);
			ICONDATA&nIcon	=	m_pMatrix[nIndex];
			if(nIcon)
			{
				arrIcon.Add(nIcon);
			}
		}
		xMin++;
		yMin++;
		xMax--;
		yMax--;
	}
	FillIcon(arrIndex,arrIcon,bNi);
}

CLinkMatrix& CLinkMatrix::operator=(CLinkMatrix&src)
{
	if(this!=&src)
	{
		Init(src.m_iRows,src.m_iCols);
		if(m_pMatrix)
		{
			memcpy(m_pMatrix,src.m_pMatrix,(m_iRows + 2) * (m_iCols + 2) * sizeof(ICONDATA));
		}
		m_xSel		=	src.m_xSel;
		m_ySel		=	src.m_ySel;
		m_iMoveMode	=	src.m_iMoveMode;
	}
	return *this;
}