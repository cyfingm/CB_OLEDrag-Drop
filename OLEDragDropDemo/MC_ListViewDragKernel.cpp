//----------------------------------------------------------------------------
/*
	@Author		:		chenyifu
	@Date		:		2015-2-27
 	@Descrption :		用于初始化一个TListView，使之具备接受外部拖拽物体和向外
 						拖拽其中TListItem的能力
 */
//----------------------------------------------------------------------------
#include "MC_ListViewDragKernel.h"
//----------------------------------------------------------------------------
MC_ListViewDragKernel::MC_ListViewDragKernel(void)
{
	CoInitializeEx(0, COINIT_MULTITHREADED);
//	CoInitializeEx(0, COINIT_APARTMENTTHREADED);

	m_InvokeCount = 0;
	m_Available = false;

	m_ListViewDragKit = new MC_ListViewDragCallback();
	if(0 == m_ListViewDragKit)
		return;

	m_DropSource = new MC_IComDropSource();
	if(0 == m_DropSource)
		return;

	m_DragItemList = new MC_IComDragItemList();
	if(0 == m_DragItemList)
		return;
	m_DragItemList->SetAsyncMode(VARIANT_TRUE);

//	for(int i=0; i<1024; i++)
//		{
//		m_Timers[i].m_Owner 	= this;
//		m_Timers[i].m_Interval 	= 1;
//		m_Timers[i].m_StartWait = 0;
//		}
//
//	m_Timers[0].Start();

	m_Available = true;
}
//----------------------------------------------------------------------------
MC_ListViewDragKernel::~MC_ListViewDragKernel(void)
{
	avx_SafeDeleteX(m_ListViewDragKit);
	m_DropSource->Release();
	m_DragItemList->Release();

	CoUninitialize();
}
//----------------------------------------------------------------------------
void* MC_ListViewDragKernel::CreateFileDespListItem(MT_FileList* vFileList)
{
	if(0 == vFileList)
		return 0;

	//vFileList若为空, 这完全不合理
	int tFileCount = (*vFileList).size();
	if(0 == tFileCount)
		return 0;

	//获取文件描述所需要的字符数
	int tCharCount = 1;
	for(int i=0; i<tFileCount; i++)
		tCharCount += ((*vFileList)[i].length() + 1);

	//分配文件描述所的空间大小, 所需大小需保留数据头DROPFILES的空间
	int tNeedLen = sizeof(wchar_t) * tCharCount + sizeof(DROPFILES);
	HGLOBAL tAllocMem = GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT, tNeedLen);
	if(0 == tAllocMem)
		return 0;

	//对数据头DROPFILES进行赋值
	DROPFILES* tDropfiles = (DROPFILES*)tAllocMem;
	tDropfiles->fWide  	  = true;							  //使用宽字符
	tDropfiles->pFiles 	  = sizeof(DROPFILES);				  //文件描述字符串起始偏移

	//进行文件描述的组装
	BYTE* tBuffer  = (BYTE*)tAllocMem + sizeof(DROPFILES);
	for(int i=0; i<tFileCount; i++)
		{
		UINT tLen = ((*vFileList)[i].length()+1) * sizeof(wchar_t);
		memcpy(tBuffer, (*vFileList)[i].c_str(), tLen);

		tBuffer += tLen;
		}

	//文件描述要求用两个0来标志着结束
	*(tBuffer++) = 0;

	return tAllocMem;
}
//----------------------------------------------------------------------------
void MC_ListViewDragKernel::DestroyFileDespListItem(HGLOBAL tItem)
{
	GlobalFree(tItem);
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
bool MC_ListViewDragKernel::DropFiles(MT_FileList* vFileList, DWORD vDesireEffect)
{
	if(false == m_Available)
		return false;

	if(0 == vFileList->size())
		return false;

	void* tFileDesp = CreateFileDespListItem(vFileList);
	if(0 == tFileDesp)
		return false;

	//产生被拖拽对象的格式描述和存储媒介
	FORMATETC tFormatEtc	 = {CF_HDROP, 0, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
	STGMEDIUM tStorageMedium = {TYMED_HGLOBAL, {(HBITMAP)tFileDesp}, 0};

	//将数据对象添加到待传输对象列表中
	m_DragItemList->Add(&tFormatEtc, &tStorageMedium, 1);

	//调用DoDragDrop(调用结果需要同时参考返回值和它的最后一个参数)
	DWORD tDropEffect;
	DWORD tDragResult = DoDragDrop(m_DragItemList, m_DropSource, vDesireEffect, &tDropEffect);

	BOOL tAsycResult;
	m_DragItemList->InOperation(&tAsycResult);

	//检查DoDragDrop调用结果
	bool tRetVal = false;
	if(tDragResult != DRAGDROP_S_DROP)
		goto WORK_END;

	//查看拖拽产生的效果
	if(tDropEffect == DROPEFFECT_NONE)
		goto WORK_END;

	tRetVal = true;

WORK_END:
	m_DragItemList->EndOperation(S_OK , 0, DROPEFFECT_COPY);

	//清理存储媒介
	ReleaseStgMedium(&tStorageMedium);
	DestroyFileDespListItem(tFileDesp);

	return tRetVal;
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
bool MC_ListViewDragKernel::BindListView(TListView* vListView)
{
	if(false == m_Available)
		return false;

	m_ListView = vListView;

	m_ListView->OnMouseMove = MyListMouseMove;

	//设置ListView的窗体消息响应函数
	m_OldListViewProc = m_ListView->WindowProc;
	m_ListView->WindowProc = MyListViewProc;

	//使m_ListView可以接受被拖拽到此处的文件
	DragAcceptFiles(m_ListView->Handle, true);

	return true;
}
//----------------------------------------------------------------------------
void __fastcall MC_ListViewDragKernel::MyListViewProc(TMessage& vMessage)
{
	switch(vMessage.Msg)
		{
		case WM_DROPFILES:                      //当有文件被拖拽进来时
			MyListDropFile(vMessage);
			break;

		case WM_LBUTTONUP:                      //当鼠标左键被释放时告知m_ListView拖拽已经结束
			m_ListView->EndDrag(true);
			break;

		default:
			break;
		}

	m_OldListViewProc(vMessage);

	return;
}
//----------------------------------------------------------------------------
bool MC_ListViewDragKernel::MyListDropFile(TMessage& vMessage)
{
	int tFileCount = (int)DragQueryFile((HDROP)vMessage.WParam, -1, 0, 0);

	for(int i=0; i<tFileCount; i++)
		{
		//获取文件的绝对路径
		wchar_t tFileNameBuf[MAX_PATH];
		DragQueryFileW((HDROP)vMessage.WParam, i, tFileNameBuf, MAX_PATH);
		KKSTR tSrcFileName = tFileNameBuf;

		m_ListViewDragKit->OnItemDragIn(tSrcFileName);
		}

	return true;
}
//---------------------------------------------------------------------------
void _fastcall MC_ListViewDragKernel::MyListStartDrag(void)
{
//	if(SEM_WAIT_SUCCESS != m_Sem.Wait())
//		return;

	int tCount = (int)m_ListView->Items->Count;

	//进行文件集合的组装
	MT_FileList tFileList;
	for(int i=0; i<tCount; i++)
		{
		TListItem* tItem = m_ListView->Items->Item[i];
		if(false == tItem->Selected)
			continue;

		//通过回调, 请求此Item相应的文件路径
		KKSTR tFileName = m_ListViewDragKit->OnItemDragOut(tItem);
		if(true == tFileName.empty())
			continue;

		tFileList.push_back(tFileName);
		}

	//进行拖拽至外部的处理
	DropFiles(&tFileList, DROPEFFECT_COPY);

	return;
}
//---------------------------------------------------------------------------
void _fastcall MC_ListViewDragKernel::MyListMouseMove(TObject *vSender, TShiftState vShift, int vX, int vY)
{
	//判断鼠标左键是否按下
	if(false == vShift.Contains(ssLeft))
		return;

	MyListStartDrag();

	return;
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
