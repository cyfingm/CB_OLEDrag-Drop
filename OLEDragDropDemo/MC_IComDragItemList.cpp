//----------------------------------------------------------------------------
/*
  	@Author		:		陈艺夫
	@Date		:		2015-2-27
	@Descrption :		OLE拖拽功能的IDataObject接口,表明一个数据对象，可理解为
 						被拖拽文件的集合。
 */
//----------------------------------------------------------------------------
#include "MC_IComDragItemList.h"
//----------------------------------------------------------------------------
MC_IComDragItemList::MC_IComDragItemList(void)
{
	m_RefCount = 1;
	InitializeCriticalSection(&m_CriticalSection);
}
//----------------------------------------------------------------------------
MC_IComDragItemList::~MC_IComDragItemList(void)
{
	DeleteCriticalSection(&m_CriticalSection);
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
ULONG __stdcall MC_IComDragItemList::AddRef(void)
{
	return InterlockedIncrement(&m_RefCount);
}
//----------------------------------------------------------------------------
ULONG __stdcall MC_IComDragItemList::Release(void)
{
	ULONG tRet = InterlockedDecrement(&m_RefCount);

	if(0 == tRet)
		delete this;

	return tRet;
}
//----------------------------------------------------------------------------
STDMETHODIMP MC_IComDragItemList::QueryInterface(REFIID vRIID, void** vPpvObject)
{
	if(0 == vPpvObject)	return E_POINTER;

	//设置返回值默认值
	*vPpvObject = 0;

	//存在ID为vRIID的接口
	if((IID_IUnknown == vRIID)||(IID_IDataObject == vRIID))
		*vPpvObject = static_cast<IDataObject*>(this);

	if(IID_IAsyncOperation == vRIID)
	 	*vPpvObject = static_cast<IAsyncOperation*>(this);

	if(0 == *vPpvObject)
		return E_NOINTERFACE;

	AddRef();

	return S_OK;
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
HRESULT __stdcall MC_IComDragItemList::GetData(FORMATETC* vFormatEtc, STGMEDIUM* vStorageMedium)
{
	if(0 == vFormatEtc)
		return E_INVALIDARG;

	//查找是否存在格式为vFormatEtc的数据对象
    int tIndex = LookupFormatEtc(vFormatEtc);
	if(-1 == tIndex)
		return DV_E_FORMATETC;

	//将查找到的数据对象内容赋值给vFormatEtc返回
    vStorageMedium->tymed           = m_FormatEtc[tIndex].tymed;
    vStorageMedium->pUnkForRelease  = 0;

	//将查询到的数据对象的hGlobal成员赋值给vStorageMedium的hGlobal
    switch(m_FormatEtc[tIndex].tymed)
		{
		case TYMED_HGLOBAL:
			vStorageMedium->hGlobal = DupGlobalMem(m_StgMedium[tIndex].hGlobal);
			break;

		case TYMED_GDI:
			vStorageMedium->hBitmap = (HBITMAP)CopyImage(m_StgMedium[tIndex].hBitmap, IMAGE_BITMAP, 0, 0, 0);
			break;

		default:
			return DV_E_FORMATETC;
		}

    return S_OK;
}
//----------------------------------------------------------------------------
HRESULT __stdcall MC_IComDragItemList::GetDataHere(FORMATETC* vFormatEtc, STGMEDIUM* vStorageMedium)
{
	return E_NOTIMPL;
}
//----------------------------------------------------------------------------
HRESULT __stdcall MC_IComDragItemList::QueryGetData(FORMATETC *vFormatEtc)
{
	int tIndex = LookupFormatEtc(vFormatEtc);
	return (-1 == tIndex) ? DV_E_FORMATETC : S_OK;
}
//----------------------------------------------------------------------------
HRESULT __stdcall MC_IComDragItemList::GetCanonicalFormatEtc(FORMATETC* vFormatEctIn, FORMATETC* vFormatEtcOut)
{
	return DATA_S_SAMEFORMATETC;
}
//----------------------------------------------------------------------------
HRESULT __stdcall MC_IComDragItemList::SetData(FORMATETC* vFormatEct, STGMEDIUM* vStorageMedium, BOOL vRelease)
{
	if(0 == vFormatEct)		return E_INVALIDARG;
	if(0 == vStorageMedium)	return E_INVALIDARG;

	m_FormatEtc.push_back(*vFormatEct);
	m_StgMedium.push_back(*vStorageMedium);

	if(true == vRelease)
		ReleaseStgMedium(vStorageMedium);

    return S_OK;
}
//----------------------------------------------------------------------------
HRESULT __stdcall MC_IComDragItemList::EnumFormatEtc(DWORD vDirection, IEnumFORMATETC** vPpEnumFormatEtc)
{
	//OLE仅仅支持数据方向为DATADIR_GET的操作
	if(DATADIR_GET != vDirection)
		return E_NOTIMPL;

	return CreateEnumFormatEtc(m_FormatEtc.size(), &m_FormatEtc[0], vPpEnumFormatEtc);	//CreateEnumFormatEtc返回对应的枚举对象实例
}
//----------------------------------------------------------------------------
HRESULT __stdcall MC_IComDragItemList::DAdvise(FORMATETC* vFormatEct, DWORD vAdvf, IAdviseSink* vAdvSink, DWORD* vConnection)
{
	UNREFERENCED_PARAMETER(vFormatEct);
	UNREFERENCED_PARAMETER(vAdvf);
	UNREFERENCED_PARAMETER(vAdvSink);
	UNREFERENCED_PARAMETER(vConnection);

    return OLE_E_ADVISENOTSUPPORTED;
}
//----------------------------------------------------------------------------
HRESULT __stdcall MC_IComDragItemList::DUnadvise(DWORD vConnection)
{
	UNREFERENCED_PARAMETER(vConnection);

    return E_NOTIMPL;
}
//----------------------------------------------------------------------------
HRESULT __stdcall MC_IComDragItemList::EnumDAdvise(IEnumSTATDATA** vPpEnumAdvise)
{
	UNREFERENCED_PARAMETER(vPpEnumAdvise);

    return OLE_E_ADVISENOTSUPPORTED;
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
int MC_IComDragItemList::LookupFormatEtc(FORMATETC* vFormatEtc)
{
	//设置默认返回值，-1代表失败
	int tRetVal = -1;

	if(0 == vFormatEtc)
		return -1;

	//查找符合格式描述vFormatEtc的数据对象的下标
	int tSize = (int)m_FormatEtc.size();
	for(int i=0; i<tSize; i++)
		{
		//两者的tymed必须有同一位为1，否则代表存储介质指示符不同
		if(0 == (m_FormatEtc[i].tymed & vFormatEtc->tymed))	continue;
		if(m_FormatEtc[i].cfFormat != vFormatEtc->cfFormat)	continue;
		if(m_FormatEtc[i].dwAspect != vFormatEtc->dwAspect) continue;

		tRetVal = i;
		}

    return tRetVal;
}
//----------------------------------------------------------------------------
void MC_IComDragItemList::Add(FORMATETC* vFormatEtc, STGMEDIUM* vStorageMedium, int vCount)
{
	if(0 == vFormatEtc)		return;
	if(0 == vStorageMedium)	return;

	EnterCriticalSection(&m_CriticalSection);		//确保push_back的线程安全

    for(int i=0; i<vCount; i++)
		{
        m_FormatEtc.push_back(vFormatEtc[i]);
        m_StgMedium.push_back(vStorageMedium[i]);
		}

	LeaveCriticalSection(&m_CriticalSection);
}
//----------------------------------------------------------------------------
void MC_IComDragItemList::Add(CLIPFORMAT vFormat, DWORD vTymed, void *vUnionContent)
{
	FORMATETC tFormatEtc 	 = {vFormat, 0, DVASPECT_CONTENT, -1, vTymed};
	STGMEDIUM tStorageMedium = {vTymed, {(HBITMAP)vUnionContent}, 0};

	Add(&tFormatEtc, &tStorageMedium, 1);
}
//----------------------------------------------------------------------------
size_t MC_IComDragItemList::Size(void)
{
    return m_FormatEtc.size();
}
//----------------------------------------------------------------------------
void MC_IComDragItemList::Clear(void)
{
    m_FormatEtc.clear();
    m_StgMedium.clear();
}
//----------------------------------------------------------------------------
void MC_IComDragItemList::ClearAndRelease(void)
{
	int tSize = (int)m_FormatEtc.size();
    for(int i=0; i<tSize; i++)
		ReleaseStgMedium(&m_StgMedium[i]);

    Clear();
}
//----------------------------------------------------------------------------
HGLOBAL MC_IComDragItemList::DupGlobalMem(HGLOBAL vSourceMem)
{
	if(0 == vSourceMem)
		return 0;

	DWORD tLen = GlobalSize(vSourceMem);
	if(0 == tLen)
		return 0;

	void* tSrcGlobalMem = GlobalLock(vSourceMem);
	if(0 == tSrcGlobalMem)
		return 0;

	void* tDstGlobalMem = GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT, tLen);
	if(0 == tDstGlobalMem)
		{
		GlobalUnlock(vSourceMem);
		return 0;
		}

	memcpy(tDstGlobalMem, tSrcGlobalMem, tLen);

	GlobalUnlock(vSourceMem);

	return tDstGlobalMem;
}
//----------------------------------------------------------------------------
HRESULT MC_IComDragItemList::CreateEnumFormatEtc(size_t vCount, FORMATETC* vFmtList, IEnumFORMATETC** vPpEnumFormatEtc)
{
	if(0 == vCount)				return E_INVALIDARG;
	if(0 == vFmtList)       	return E_INVALIDARG;
	if(0 == vPpEnumFormatEtc)	return E_INVALIDARG;

	//设置返回值为一个数据对象(可理解为被拖拽对象集合)的格式枚举器实例
    *vPpEnumFormatEtc = new MC_IComEnumFormatEtc(vFmtList, vCount);
	if(0 == *vPpEnumFormatEtc)
		return E_OUTOFMEMORY;

	return S_OK;
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
STDMETHODIMP MC_IComDragItemList::SetAsyncMode(BOOL fDoOpAsync)
{
	m_IsAsync = fDoOpAsync;
    return S_OK;
}
//----------------------------------------------------------------------------
STDMETHODIMP MC_IComDragItemList::StartOperation(IBindCtx *pbcReserved)
{
	m_InOperation = VARIANT_TRUE;
    return S_OK;
}
//----------------------------------------------------------------------------
STDMETHODIMP MC_IComDragItemList::InOperation(BOOL *pfInAsyncOp)
{
	if(0 == pfInAsyncOp)
		return E_FAIL;

	*pfInAsyncOp = m_InOperation;

	return (m_InOperation == VARIANT_FALSE) ? E_FAIL : S_OK;
}
//----------------------------------------------------------------------------
STDMETHODIMP MC_IComDragItemList::GetAsyncMode(BOOL *pfIsOpAsync)
{
	if(!pfIsOpAsync)
		return E_FAIL;

    *pfIsOpAsync = m_IsAsync;

    return S_OK;
}
//----------------------------------------------------------------------------
STDMETHODIMP MC_IComDragItemList::EndOperation(HRESULT hResult,IBindCtx *pbcReserved,DWORD dwEffects)
{
	m_InOperation = VARIANT_FALSE;
    return S_OK;
}
//----------------------------------------------------------------------------
