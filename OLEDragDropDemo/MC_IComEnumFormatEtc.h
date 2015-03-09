//----------------------------------------------------------------------------
/*
 *	@Author		:		chenyifu
 *	@Date		:		2015-2-27
 *	@Descrption :		拖拽对象列表的格式枚举对象
 */
//----------------------------------------------------------------------------
#ifndef MC_IComEnumFormatEtcH
#define MC_IComEnumFormatEtcH
//----------------------------------------------------------------------------
#include <Objidl.h>
//----------------------------------------------------------------------------
class MC_IComEnumFormatEtc : public IEnumFORMATETC
{
private:
	size_t      m_Index;           		//枚举器指针索引
	size_t      m_FormatsCount;    		//FORMATETC对象个数
	FORMATETC*  m_FormatEtc;       		//FORMATETC对象集合

	void		DeepCopyFormatEtc	 	(FORMATETC* vDest, FORMATETC* vSrc);

	//产生数据对象的格式枚举器实例
	HRESULT 	CreateEnumFormatEtc	 	(size_t vCount, FORMATETC* vFmtList, IEnumFORMATETC** vPpEnumFormatEtc);

public:
	// IEnumFormatEtc接口成员
	HRESULT __stdcall  Next  			(ULONG vCelt, FORMATETC* vFormatEtc, ULONG* vCeltFetched);
	HRESULT __stdcall  Skip  			(ULONG vCelt);
	HRESULT __stdcall  Reset 			(void);
	HRESULT __stdcall  Clone 			(IEnumFORMATETC** vPpEnumFormatEtc);

	//COM接口成员
	ULONG __stdcall    AddRef			(void);
	ULONG __stdcall    Release			(void);
	STDMETHODIMP 	   QueryInterface	(REFIID vRIID, void** vPpvObject);

	volatile LONG      m_RefCount;        // COM接口引用计数变量

public:
	MC_IComEnumFormatEtc(FORMATETC* vFormatEtc, size_t vCount);
	~MC_IComEnumFormatEtc(void);
};
//----------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------