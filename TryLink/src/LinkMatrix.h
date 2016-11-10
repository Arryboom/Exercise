//LinkMatrix.h
#pragma once

typedef	unsigned char ICONDATA;

class CLinkPath
{
public:
	CLinkPath();
	void	Empty();
	void	AddPoint(int x,int y);
public:
	int		m_Count;
	int		m_x[4];
	int		m_y[4];
};

class CLinkMatrix  
{
public:
	enum
	{
		MOVE_None,		//���仯
		MOVE_Up,		//����
		MOVE_Down,		//����
		MOVE_Left,		//����
		MOVE_Right,		//����
		MOVE_LU,		//������
		MOVE_LD,		//������
		MOVE_RU,		//������
		MOVE_RD,		//������
		MOVE_ULDR,		//��������
		MOVE_URDL,		//��������
		MOVE_LDRU,		//��������
		MOVE_LURD,		//��������
		MOVE_FenLiH,	//�������
		MOVE_FenLiV,	//�������
		MOVE_JiZhongH,	//������
		MOVE_JiZhongV,	//������
		MOVE_DengFenH,	//����ȷ�
		MOVE_DengFenV,	//����ȷ�
		MOVE_JuZhongH,	//�������
		MOVE_JuZhongV,	//�������
		MOVE_HCuo,		//�����(����ż��)
		MOVE_VCuo,		//�����(����ż��)
		MOVE_HCuoFen,	//�����(�����ż����)
		MOVE_VCuoFen,	//�����(�����ż����)
		MOVE_HCuoDeng,	//�����(��ȷ�ż����)
		MOVE_VCuoDeng,	//�����(��ȷ�ż����)
		MOVE_KuoSanHV,	//��ɢ(���к���)
		MOVE_KuoSanVH,	//��ɢ(���к���)
		MOVE_JiZhongHV,	//����(���к���)
		MOVE_JiZhongVH,	//����(���к���)
		MOVE_ZUp,		//Z ������
		MOVE_ZDown,		//Z ������
		MOVE_NLeft,		//N ������
		MOVE_NRight,	//N ������
		MOVE_SUp,		//S ������
		MOVE_SDown,		//S ������
		MOVE_RotL,		//��ʱ�������ת
		MOVE_RotR,		//˳ʱ�������ת
		MOVE_RotLR,		//���������ת
		MOVE_RotNLuoW,	//��ʱ����������
		MOVE_RotNLuoN,	//��ʱ����������
		MOVE_RotSLuoW,	//˳ʱ����������
		MOVE_RotSLuoN,	//˳ʱ����������
		MOVE_COUNT,		//�仯����
	};
public:
	CLinkMatrix();
	~CLinkMatrix();
	void	Init(int nRows,int nCols,int nIconCount=0);
	void	Empty();
	BOOL	FindPath(int x1, int y1, int x2, int y2,CLinkPath*pPath=NULL);
	int		GetHelp(CLinkPath*pPath=NULL);
	int		GetHelp(int x0,int y0,CLinkPath*pPath=NULL);
	void	ChangeLayout();
	void	RemoveIcon(int x1,int y1,int x2,int y2);
	CString	GetMoveModeStr();
	void	SetMoveMode(int iMode);
	ICONDATA&	operator()(int x,int y)
	{
		return m_pMatrix[x + y * (m_iCols + 2)];
	}
	CLinkMatrix&operator=(CLinkMatrix&src);
	int		XYtoIndex(int x,int y)
	{
		return x + y * (m_iCols + 2);
	}
	void	IndexToXY(int nIndex,int&x,int&y)
	{
		x	=	nIndex % (m_iCols + 2);
		y	=	nIndex / (m_iCols + 2);
	}
private:
	void	MoveUp(int x1,int y1,int x2,int y2);		//����
	void	MoveDown(int x1,int y1,int x2,int y2);		//����
	void	MoveLeft(int x1,int y1,int x2,int y2);		//����
	void	MoveRight(int x1,int y1,int x2,int y2);		//����
	void	MoveLU(int x1,int y1,int x2,int y2);		//������
	void	MoveLD(int x1,int y1,int x2,int y2);		//������
	void	MoveRU(int x1,int y1,int x2,int y2);		//������
	void	MoveRD(int x1,int y1,int x2,int y2);		//������
	void	MoveULDR(int x1,int y1,int x2,int y2);		//��������
	void	MoveURDL(int x1,int y1,int x2,int y2);		//��������
	void	MoveLDRU(int x1,int y1,int x2,int y2);		//��������
	void	MoveLURD(int x1,int y1,int x2,int y2);		//��������
	void	MoveFenLiH(int x1,int y1,int x2,int y2);	//�з���
	void	MoveFenLiV(int x1,int y1,int x2,int y2);	//�з���
	void	MoveJiZhongH(int x1,int y1,int x2,int y2);	//�м���
	void	MoveJiZhongV(int x1,int y1,int x2,int y2);	//�м���
	void	MoveDengH(int x1,int y1,int x2,int y2);		//�еȷ�
	void	MoveDengV(int x1,int y1,int x2,int y2);		//�еȷ�
	void	MoveJuZhongH(int x1,int y1,int x2,int y2);	//�о���
	void	MoveJuZhongV(int x1,int y1,int x2,int y2);	//�о���
	void	MoveCuoH(int x1,int y1,int x2,int y2);		//�д�(����ż��)
	void	MoveCuoV(int x1,int y1,int x2,int y2);		//�д�(����ż��)
	void	MoveCuoFenH(int x1,int y1,int x2,int y2);	//�д�(�����ż����)
	void	MoveCuoFenV(int x1,int y1,int x2,int y2);	//�д�(�����ż����)
	void	MoveCuoDengH(int x1,int y1,int x2,int y2);	//�д�(��ȷ�ż����)
	void	MoveCuoDengV(int x1,int y1,int x2,int y2);	//�д�(��ȷ�ż����)
	void	MoveKuoSanHV(int x1,int y1,int x2,int y2);	//��ɢ(���к���)
	void	MoveKuoSanVH(int x1,int y1,int x2,int y2);	//��ɢ(���к���)
	void	MoveJiZhongHV(int x1,int y1,int x2,int y2);	//����(���к���)
	void	MoveJiZhongVH(int x1,int y1,int x2,int y2);	//����(���к���)
	void	MoveZUpDown(BOOL bUp);						//Z ������/Z ������
	void	MoveNLeftRight(BOOL bLeft);					//N ������/N ������
	void	MoveSUpDown(BOOL bUp);						//S ������/S ������
	void	MoveRotL(int x1,int y1,int x2,int y2);		//��ʱ�������ת
	void	MoveRotR(int x1,int y1,int x2,int y2);		//˳ʱ�������ת
	void	MoveRotLR(int x1,int y1,int x2,int y2);		//������ת
	void	MoveRotLuo1(BOOL bNi);						//˳ʱ����������/��ʱ����������
	void	MoveRotLuo2(BOOL bNi);						//˳ʱ����������/��ʱ����������
private:
	void	MoveUp(int x);				//����(һ��)
	void	MoveDown(int x);			//����(һ��)
	void	MoveLeft(int y);			//����(һ��)
	void	MoveRight(int y);			//����(һ��)
	void	MoveLU(int c);				//������
	void	MoveLD(int c);				//������
	void	MoveRU(int c);				//������
	void	MoveRD(int c);				//������
	void	MoveFenLiRow(int y);		//����һ��
	void	MoveFenLiCol(int x);		//����һ��
	void	MoveJiZhongRow(int y);		//����һ��
	void	MoveJiZhongCol(int x);		//����һ��
	void	MoveDengRow(int y);			//�ȷ�һ��
	void	MoveDengCol(int x);			//�ȷ�һ��
	void	MoveJuZhongRow(int y);		//����һ��
	void	MoveJuZhongCol(int x);		//����һ��
	void	MoveRotL(int nLoop);		//��ʱ����תһȦ
	void	MoveRotR(int nLoop);		//˳ʱ����תһȦ
private:
	BOOL	IsBlankH(int y,int xFrom,int xTo);
	BOOL	IsBlankV(int x,int yFrom,int yTo);
	int		CountIconRow(int y);						//ͳ��һ�е���
	int		CountIconCol(int x);						//ͳ��һ�е���
	int		GetLoopNumber(int x,int y);
	void	FillIcon(CDWordArray&arrIndex,CDWordArray&arrIcon,BOOL bLeft);
public:
	ICONDATA*	m_pMatrix;
	int			m_iRows;	//����
	int			m_iCols;	//����
	int			m_xSel;		//��ǰѡ��ͼƬ������
	int			m_ySel;		//
	int			m_iMoveMode;//����ģʽ
};
