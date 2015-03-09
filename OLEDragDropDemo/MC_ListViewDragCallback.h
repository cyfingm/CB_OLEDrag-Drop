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
	virtual KKSTR 	OnItemDragOut	(TListItem* vItem);		  //当有DragItem拖动至外部, 获取指定Item的实际文件路径，再交由系统予以接管
	virtual void 	OnItemDragIn	(KKSTR vFrom);			  //当有DragItem拖动至内部, 投递外部的文件路径到这里
};
//---------------------------------------------------------------------------
#endif
