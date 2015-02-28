//----------------------------------------------------------------------------
/*
* 	@Author		:		陈艺夫
* 	@Date		:		2015-2-27
* 	@Descrption :		实现OLE向其他程序拖拽需要实现IDataObject接口,用于存储
						将被传输的文件
*/
//----------------------------------------------------------------------------
#include "MC_DataObject.h"
//----------------------------------------------------------------------------
MC_DataObject::MC_DataObject() : m_RefCount(1)
{
}
//----------------------------------------------------------------------------
MC_DataObject::~MC_DataObject()
{
}
//----------------------------------------------------------------------------
ULONG __stdcall MC_DataObject::AddRef()
{
	return ++m_RefCount;
}
//----------------------------------------------------------------------------
ULONG __stdcall MC_DataObject::Release()
{
	--m_RefCount;

	if (m_RefCount == 0)
		delete this;

	return m_RefCount;
}
//----------------------------------------------------------------------------
STDMETHODIMP MC_DataObject::QueryInterface(REFIID vRIID, void** vPpvObject) 
{
	if (!vPpvObject)
		return E_POINTER;

	if (vRIID == IID_IUnknown || vRIID == IID_IDataObject)
		{
        *vPpvObject = this;
        AddRef();
        return S_OK;
		}

        *vPpvObject = 0;
        return E_NOINTERFACE;
}
//----------------------------------------------------------------------------
HRESULT __stdcall MC_DataObject::GetData(FORMATETC *vFormatEtc, STGMEDIUM *vStorageMedium)
{
	if (!vFormatEtc || !vStorageMedium)
		return E_INVALIDARG;

	//Lookup for requierd format data object
	int tIndex = LookupFormatEtc(vFormatEtc);
	if (tIndex == -1)
		return DV_E_FORMATETC;

	vStorageMedium->tymed           = m_FormatEtc[tIndex].tymed;
	vStorageMedium->pUnkForRelease  = 0;

	//Store data object to vStorageMedium
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
HRESULT __stdcall MC_DataObject::GetDataHere(FORMATETC *, STGMEDIUM *)
{
	return DATA_E_FORMATETC;
}
//----------------------------------------------------------------------------
HRESULT __stdcall MC_DataObject::QueryGetData(FORMATETC *vFormatEtc)
{
	if (!vFormatEtc)
		return E_INVALIDARG;

	return (-1 == LookupFormatEtc(vFormatEtc)) ? DV_E_FORMATETC : S_OK;
}
//----------------------------------------------------------------------------
HRESULT __stdcall MC_DataObject::GetCanonicalFormatEtc(FORMATETC *, FORMATETC *vFormatEtcOut)
{
    vFormatEtcOut->ptd = 0;
    return E_NOTIMPL;
}
//----------------------------------------------------------------------------
HRESULT __stdcall MC_DataObject::SetData(FORMATETC *, STGMEDIUM *, BOOL)
{
    return E_NOTIMPL;
}
//----------------------------------------------------------------------------
HRESULT __stdcall MC_DataObject::EnumFormatEtc(DWORD vDirection, IEnumFORMATETC **vPpEnumFormatEtc)
{
	if (vDirection == DATADIR_GET)
		return vPpEnumFormatEtc ? CreateEnumFormatEtc(m_FormatEtc.size(), &m_FormatEtc[0], vPpEnumFormatEtc) : E_INVALIDARG;
	else
	        return E_NOTIMPL;
}
//----------------------------------------------------------------------------
HRESULT __stdcall MC_DataObject::DAdvise(FORMATETC *, DWORD, IAdviseSink *, DWORD *)
{
    return E_NOTIMPL;
}
//----------------------------------------------------------------------------
HRESULT __stdcall MC_DataObject::DUnadvise(DWORD)
{
    return E_NOTIMPL;
}
//----------------------------------------------------------------------------
HRESULT __stdcall MC_DataObject::EnumDAdvise(IEnumSTATDATA **)
{
    return E_NOTIMPL;
}
//----------------------------------------------------------------------------
int MC_DataObject::LookupFormatEtc(FORMATETC* vFormatEtc) const
{
	for (size_t i = 0; i < m_FormatEtc.size(); ++i)
		{
		if (	(m_FormatEtc[i].tymed & vFormatEtc->tymed)   &&
			m_FormatEtc[i].cfFormat == vFormatEtc->cfFormat &&
			m_FormatEtc[i].dwAspect == vFormatEtc->dwAspect)
			{
				return i;
			}
		}
	return -1;
}
//----------------------------------------------------------------------------
void MC_DataObject::Add(FORMATETC *vFormatEtc, STGMEDIUM *vStorageMedium, UINT vCount = 1)
{
    for(UINT i=0; i<vCount; i++)
		{
        m_FormatEtc.push_back(vFormatEtc[i]);
        m_StgMedium.push_back(vStorageMedium[i]);
		}
}
//----------------------------------------------------------------------------
void MC_DataObject::Add(CLIPFORMAT vClipboardFormat,DWORD vTymed,void *vUnionContent)
{
	FORMATETC tFormatEtc = {vClipboardFormat, 0, DVASPECT_CONTENT, -1, vTymed};
	STGMEDIUM tStorageMedium = {vTymed, {(HBITMAP)vUnionContent}, 0};

	Add(&tFormatEtc, &tStorageMedium, 1);
}
//----------------------------------------------------------------------------
size_t MC_DataObject::Size(void)
{
    return m_FormatEtc.size();
}
//----------------------------------------------------------------------------
bool MC_DataObject::Empty(void)
{
    return m_FormatEtc.empty();
}
//----------------------------------------------------------------------------
void MC_DataObject::Clear(void)
{
    m_FormatEtc.clear();
    m_StgMedium.clear();
}
//----------------------------------------------------------------------------
void MC_DataObject::ClearAndRelease(void)
{
    for(size_t i=0;i<m_FormatEtc.size();i++)
        ReleaseStgMedium(&m_StgMedium[i]);
    Clear();
}
//----------------------------------------------------------------------------
HGLOBAL MC_DataObject::DupGlobalMem(HGLOBAL vSourceMem)
{
	DWORD tLen    = GlobalSize(vSourceMem);
	PVOID tSource = GlobalLock(vSourceMem);
	PVOID tDest   = GlobalAlloc(GMEM_FIXED, tLen);
	std::memcpy (tDest, tSource, tLen);
    GlobalUnlock(vSourceMem);
	return tDest;
}
//----------------------------------------------------------------------------
HRESULT MC_DataObject::CreateEnumFormatEtc(UINT vFormatCount, FORMATETC* vFormat, IEnumFORMATETC** vPpEnumFormatEtc)
{
	if(vFormatCount == 0||vFormat == 0||vPpEnumFormatEtc == 0)
        return E_INVALIDARG;

    *vPpEnumFormatEtc = new MC_EnumFormatEtc(vFormat, vFormatCount);

	return (*vPpEnumFormatEtc) ? S_OK : E_OUTOFMEMORY;
}
//----------------------------------------------------------------------------
