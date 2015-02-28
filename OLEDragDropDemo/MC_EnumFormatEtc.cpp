//----------------------------------------------------------------------------
/*
 *	@Author		:		chenyifu
 *	@Date		:		2015-2-27
 *	@Descrption :		用于枚举已被选中将要拖拽的文件
 */
//----------------------------------------------------------------------------
#include "MC_EnumFormatEtc.h"
//----------------------------------------------------------------------------
MC_EnumFormatEtc::MC_EnumFormatEtc(FORMATETC* vFormatEtc, ULONG vFormatsCount)
: m_RefCount(1), m_Index(0), m_FormatsCount(vFormatsCount)
{
    m_FormatEtc = new FORMATETC[vFormatsCount];

    for(ULONG i = 0; i < vFormatsCount; i++)
        DeepCopyFormatEtc(&m_FormatEtc[i], &vFormatEtc[i]);
}
//----------------------------------------------------------------------------
MC_EnumFormatEtc::~MC_EnumFormatEtc()
{
    //release all DVTARGETDEVICE structures
    for(ULONG i = 0; i < m_FormatsCount; i++)
		{
        if(m_FormatEtc[i].ptd)
			CoTaskMemFree(m_FormatEtc[i].ptd);
    	}

    delete[] m_FormatEtc;
}
//----------------------------------------------------------------------------
ULONG __stdcall MC_EnumFormatEtc::AddRef()
{
	return ++m_RefCount;
}
//----------------------------------------------------------------------------
ULONG __stdcall MC_EnumFormatEtc::Release()
{
	--m_RefCount;

	if (m_RefCount == 0)
		delete this;

	return m_RefCount;
}
//----------------------------------------------------------------------------
STDMETHODIMP MC_EnumFormatEtc::QueryInterface(REFIID vRIID, void** vPpvObject) 
{
	if (vRIID == IID_IUnknown || vRIID == IID_IDataObject)
		{
        *vPpvObject = this;
        AddRef();
        return S_OK;
		}
    else
		{
        *vPpvObject = 0;
        return E_NOINTERFACE;
		}
}
//----------------------------------------------------------------------------
HRESULT __stdcall MC_EnumFormatEtc::Next(ULONG vCelt, FORMATETC* vFormatEtc, ULONG* vCeltFetched)
{
    ULONG tCopied = 0;
	// copy required object to vFormatEtc
    while (m_Index < m_FormatsCount && tCopied < vCelt)
		{
        DeepCopyFormatEtc(&vFormatEtc[tCopied], &m_FormatEtc[m_Index]);
        tCopied++;
        m_Index++;
    	}
	//return copied files count to vCeltFetched
    if(0 != vCeltFetched) 
        *vCeltFetched = tCopied;

	//Is all object required copied?
    return (tCopied == vCelt) ? S_OK : S_FALSE;
}
//----------------------------------------------------------------------------
void MC_EnumFormatEtc::DeepCopyFormatEtc(FORMATETC* vDest, FORMATETC* vSource)
{
	*vDest = *vSource;
	if(vSource->ptd)
		{
		vDest->ptd = (DVTARGETDEVICE*)CoTaskMemAlloc(sizeof(DVTARGETDEVICE));
		*(vDest->ptd) = *(vSource->ptd);
		}
}
//----------------------------------------------------------------------------
HRESULT __stdcall MC_EnumFormatEtc::Skip(ULONG vCelt)
{
    m_Index += vCelt;
    return (m_Index <= m_FormatsCount) ? S_OK : S_FALSE;
}
//----------------------------------------------------------------------------
HRESULT __stdcall MC_EnumFormatEtc::Reset()
{
    m_Index = 0;
    return S_OK;
}
//----------------------------------------------------------------------------
HRESULT __stdcall MC_EnumFormatEtc::Clone(IEnumFORMATETC** vPpEnumFormatEtc)
{
    HRESULT tResult;
    tResult = CreateEnumFormatEtc(m_FormatsCount, m_FormatEtc, vPpEnumFormatEtc);
    if(tResult == S_OK)
        ((MC_EnumFormatEtc *)*vPpEnumFormatEtc)->m_Index = m_Index;

    return tResult;
}
//----------------------------------------------------------------------------
HRESULT MC_EnumFormatEtc::CreateEnumFormatEtc(UINT vFormatCount, FORMATETC* vFormat, IEnumFORMATETC** vPpEnumFormatEtc)
{
	if (vFormatCount == 0||vFormat == 0||vPpEnumFormatEtc == 0)
        return E_INVALIDARG;

    *vPpEnumFormatEtc = new MC_EnumFormatEtc(vFormat, vFormatCount);

	return (*vPpEnumFormatEtc) ? S_OK : E_OUTOFMEMORY;
}
//----------------------------------------------------------------------------
