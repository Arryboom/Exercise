//Undo.h
#pragma once

#include "LinkMatrix.h"

class CUndo  
{
public:
	class CState
	{
	public:
		CLinkPath	m_path;			//�ò�������·��
		CLinkMatrix	m_Matrix;		//�ò�ִ�к��״̬
	};
public:
	CUndo();
	~CUndo();
public:
	void	Empty();
	void	AddState(CState*pState,BOOL bInit=FALSE);
	BOOL	CanUndo();
	BOOL	CanRedo();
	CState*	Undo(CLinkPath*pPath=NULL);
	CState*	Redo(CLinkPath*pPath=NULL);
public:
	CTypedPtrArray<CPtrArray,CState*>m_arrState;
	int		m_iHaveDoStep;	//�Ѿ����Ĳ�����
};
