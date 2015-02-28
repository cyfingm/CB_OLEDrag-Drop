//----------------------------------------------------------------------------
/*
 *	@Author		:		chenyifu
 *	@Date		:		2015-2-27
 *	@Descrption :		用于枚举已被选中将要拖拽的文件
 */
//----------------------------------------------------------------------------
#ifndef MC_EnumFormatEtcH
#define MC_EnumFormatEtcH
//----------------------------------------------------------------------------
#include "MC_OLEDragHeaders.h"
//----------------------------------------------------------------------------
class MC_EnumFormatEtc : public IEnumFORMATETC
{
public:
	//COM interface
	ULONG __stdcall AddRef			(void);
	ULONG __stdcall Release			(void);
	STDMETHODIMP 	QueryInterface	(REFIID vRIID, void** vPpvObject);

	// IEnumFormatEtc interface
    HRESULT __stdcall  Next  (ULONG vCelt, FORMATETC* vFormatEtc, ULONG* vCeltFetched);
    HRESULT __stdcall  Skip  (ULONG vCelt);
    HRESULT __stdcall  Reset (void);
    HRESULT __stdcall  Clone (IEnumFORMATETC** vPpEnumFormatEtc);

	void	DeepCopyFormatEtc	 (FORMATETC* vDest, FORMATETC* vSource);
	HRESULT CreateEnumFormatEtc	 (UINT vFormatCount, FORMATETC* vFormat, IEnumFORMATETC** vPpEnumFormatEtc);

    //Cons/Destructors
    MC_EnumFormatEtc(FORMATETC* vFormatEtc, ULONG vFormatsCount);
    ~MC_EnumFormatEtc(void);

private:
    LONG        m_RefCount;        // COM reference count
	ULONG       m_Index;
	ULONG       m_FormatsCount;
	FORMATETC*  m_FormatEtc;
};
//----------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------