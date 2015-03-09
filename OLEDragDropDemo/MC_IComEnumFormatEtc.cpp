//----------------------------------------------------------------------------
/*
 *	@Author		:		chenyifu
 *	@Date		:		2015-2-27
 *	@Descrption :		拖拽对象列表的格式枚举对象
 */
//----------------------------------------------------------------------------
#include "MC_IComEnumFormatEtc.h"
//----------------------------------------------------------------------------
MC_IComEnumFormatEtc::MC_IComEnumFormatEtc(FORMATETC* vFormatEtc, size_t vCount)
{
	//初始化成员变量
	m_RefCount 	   = 1;
	m_Index    	   = 0;
	m_FormatsCount = vCount;

	m_FormatEtc = new FORMATETC[vCount];

    for(int i=0; i<(int)vCount; i++)
        DeepCopyFormatEtc(&m_FormatEtc[i], &vFormatEtc[i]);
}
//----------------------------------------------------------------------------
MC_IComEnumFormatEtc::~MC_IComEnumFormatEtc(void)
{
    //释放所有的DVTARGETDEVICE对象
    for(int i=0; i<(int)m_FormatsCount; i++)
		{
        if(0 != m_FormatEtc[i].ptd)
			CoTaskMemFree(m_FormatEtc[i].ptd);
    	}

    delete[] m_FormatEtc;
}
//----------------------------------------------------------------------------
ULONG __stdcall MC_IComEnumFormatEtc::AddRef(void)
{
	return InterlockedIncrement(&m_RefCount);
}
//----------------------------------------------------------------------------
ULONG __stdcall MC_IComEnumFormatEtc::Release(void)
{
	ULONG tRet = InterlockedDecrement(&m_RefCount);

	if (0 == tRet)
		delete this;

	return tRet;
}
//----------------------------------------------------------------------------
STDMETHODIMP MC_IComEnumFormatEtc::QueryInterface(REFIID vRIID, void** vPpvObject) 
{
	//设置返回值默认值
	*vPpvObject = 0;

	//存在ID为vRIID的接口
	if(IID_IUnknown != vRIID)
		if(IID_IDataObject != vRIID)
			{
			//告知调用者接口不存在
			return E_NOINTERFACE;
			}

	//设置返回值*vPpvObject
	*vPpvObject = this;
	AddRef();

	return S_OK;
}
//----------------------------------------------------------------------------
HRESULT __stdcall MC_IComEnumFormatEtc::Next(ULONG vCelt, FORMATETC* vFormatEtc, ULONG* vCeltFetched)
{
	ULONG tCopied = 0;

    //将对应的FORMATETC结构体复制到vFormatEtc
	while(true)
		{
		if(tCopied >= vCelt) 			break;
		if(m_Index >= m_FormatsCount)   break;

        DeepCopyFormatEtc(&vFormatEtc[tCopied], &m_FormatEtc[m_Index]);
        tCopied++;
        m_Index++;
		}

    //设置vCeltFetched告知调用者实际上被复制结构体的个数
    if(0 != vCeltFetched) 
        *vCeltFetched = tCopied;

	//vCelt用于指定要复制到返回值vFormatEtc中的引用的个数
	return (tCopied == vCelt) ? S_OK : S_FALSE;
}
//----------------------------------------------------------------------------
void MC_IComEnumFormatEtc::DeepCopyFormatEtc(FORMATETC* vDest, FORMATETC* vSrc)
{
	if(0 == vSrc)	return;
	if(0 == vDest)	return;

	*vDest = *vSrc;

	//若vSrc没有设置渲染设备，复制完成
	if(0 == vSrc->ptd)
		return;

	//拷贝格式的渲染设备设置
	vDest->ptd = (DVTARGETDEVICE*)CoTaskMemAlloc(sizeof(DVTARGETDEVICE));
	*(vDest->ptd) = *(vSrc->ptd);

	return;
}
//----------------------------------------------------------------------------
HRESULT __stdcall MC_IComEnumFormatEtc::Skip(ULONG vCelt)
{
    m_Index += vCelt;
    return (m_Index <= m_FormatsCount) ? S_OK : S_FALSE;
}
//----------------------------------------------------------------------------
HRESULT __stdcall MC_IComEnumFormatEtc::Reset(void)
{
    m_Index = 0;
    return S_OK;
}
//----------------------------------------------------------------------------
HRESULT __stdcall MC_IComEnumFormatEtc::Clone(IEnumFORMATETC** vPpEnumFormatEtc)
{
    HRESULT tResult = CreateEnumFormatEtc(m_FormatsCount, m_FormatEtc, vPpEnumFormatEtc);
    if(S_OK == tResult)
        ((MC_IComEnumFormatEtc*)*vPpEnumFormatEtc)->m_Index = m_Index;

    return tResult;
}
//----------------------------------------------------------------------------
HRESULT MC_IComEnumFormatEtc::CreateEnumFormatEtc(size_t vCount, FORMATETC* vFmtList, IEnumFORMATETC** vPpEnumFormatEtc)
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