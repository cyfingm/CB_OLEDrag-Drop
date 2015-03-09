//----------------------------------------------------------------------------
/*
 	@Author		:		chenyifu
	@Date		:		2015-2-27
	@Descrption :		用于初始化一个TListView，使之具备接受外部拖拽物体和向外
 						拖拽其中TListItem的能力
 */
//----------------------------------------------------------------------------
#ifndef MC_ListViewDragKernelH
#define MC_ListViewDragKernelH
//----------------------------------------------------------------------------
#include "MC_IComDropSource.h"
#include "MC_IComDragItemList.h"
#include "MC_ListViewDragCallback.h"
#include <string>
//----------------------------------------------------------------------------
class MC_ListViewDragKernel
{
private:
	typedef std::vector<KKSW> 	MT_FileList;

	int 						m_InvokeCount;
	MC_KKSemaphore				m_Sem;

	bool    					m_Available;

	TListView*					m_ListView;
	TWndMethod 					m_OldListViewProc;		//用于保存m_ListView的原有窗体消息处理函数

	MC_IComDropSource*			m_DropSource;
	MC_IComDragItemList*		m_DragItemList;
	MC_ListViewDragCallback* 	m_ListViewDragKit;

	void __fastcall		MyListViewProc	(TMessage& vMessage);                            	//m_ListView的自定义窗体消息处理函数
	bool				MyListDropFile	(TMessage& vMessage);							 	//接收到拖拽进来文件时的处理程序
	void __fastcall		MyListMouseMove	(TObject *vSender, TShiftState vShift, int vX, int vY);
	void __fastcall		MyListStartDrag	(void);

	bool    DropFiles					(MT_FileList* vFileList, DWORD vDesireEffect);   	//执行OLE文件拖拽

	void*	CreateFileDespListItem		(MT_FileList* vFileList);
	void 	DestroyFileDespListItem 	(void* tItem);

//线程相关
	MC_KKTimerA(MC_Timer, MC_ListViewDragKernel, MyListStartDrag);
	MC_Timer	m_Timers[1024];

//	DWORD WINAPI ThreadProc(PVOID pParam);

public:
	MC_ListViewDragKernel(void);
	~MC_ListViewDragKernel(void);

	bool  BindListView 					(TListView* vListView);
};
//----------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------




