//---------------------------------------------------------------------------
#include "MC_ListViewDragCallback.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
MC_ListViewDragCallback::MC_ListViewDragCallback(void)
{
}
//---------------------------------------------------------------------------
MC_ListViewDragCallback::~MC_ListViewDragCallback(void)
{
}
//---------------------------------------------------------------------------
void MC_ListViewDragCallback::OnBindListView(TListView* vListView)
{
   //ToDo
}
//---------------------------------------------------------------------------
KKSTR MC_ListViewDragCallback::OnItemDragOut(TListItem* vItem)
{
	if(0 == vItem)
		return L"";

	return UnicodeString(vItem->Caption).c_str();
}
//---------------------------------------------------------------------------
void MC_ListViewDragCallback::OnItemDragIn(KKSTR vFrom)
{
	ShowMessage(vFrom.c_str());
}
//---------------------------------------------------------------------------

