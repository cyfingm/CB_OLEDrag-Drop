//---------------------------------------------------------------------------
#ifndef MC_ListViewDragCallbackH
#define MC_ListViewDragCallbackH
//---------------------------------------------------------------------------
#include <vcl.h>
#include "..\..\zavx\windows\inc\ZAVXShareLib.h"
//---------------------------------------------------------------------------
class MC_ListViewDragCallback
{
private:

public:
	MC_ListViewDragCallback(void);
	~MC_ListViewDragCallback(void);

	virtual void 	OnBindListView 	(TListView* vListView);
	virtual KKSTR 	OnItemDragOut	(TListItem* vItem);		  //����DragItem�϶����ⲿ, ��ȡָ��Item��ʵ���ļ�·�����ٽ���ϵͳ���Խӹ�
	virtual void 	OnItemDragIn	(KKSTR vFrom);			  //����DragItem�϶����ڲ�, Ͷ���ⲿ���ļ�·��������
};
//---------------------------------------------------------------------------
#endif
