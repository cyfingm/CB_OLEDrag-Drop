//----------------------------------------------------------------------------
/*
* 	@Author		:		陈艺夫
* 	@Date		:		2015-2-27
* 	@Descrption :		实现OLE向其他程序拖拽需要实现IDataObject接口,用于存储
						将被传输的文件
*/
//----------------------------------------------------------------------------
#ifndef MC_DataObjectH
#define MC_DataObjectH
//----------------------------------------------------------------------------
#include "MC_OLEDragHeaders.h"
#include "MC_EnumFormatEtc.h"
#include <vector>
//----------------------------------------------------------------------------
class MC_DataObject : public IDataObject
{
public:
	//COM interface
	ULONG __stdcall AddRef			();
	ULONG __stdcall Release			();
	STDMETHODIMP	QueryInterface	(REFIID vRIID, void** vPpvObject);

	//IDataObject interface
	STDMETHODIMP GetData				(FORMATETC* vFormatEtc, STGMEDIUM* vStorageMedium);
	STDMETHODIMP GetDataHere			(FORMATETC* vFormatEtc, STGMEDIUM* vStorageMedium);
	STDMETHODIMP QueryGetData			(FORMATETC* vFormatEtc);
	STDMETHODIMP GetCanonicalFormatEtc	(FORMATETC* vFormatEctIn, FORMATETC* vFormatEtcOut);
	STDMETHODIMP SetData				(FORMATETC* vFormatEct, STGMEDIUM* vStorageMedium, BOOL vRelease);
	STDMETHODIMP EnumFormatEtc			(DWORD vDirection, IEnumFORMATETC** vPpEnumFormatEtc);
	STDMETHODIMP DAdvise				(FORMATETC* vFormatEct, DWORD vAdvf, IAdviseSink* vAdvSink, DWORD* vConnection);
	STDMETHODIMP DUnadvise				(DWORD vConnection);
	STDMETHODIMP EnumDAdvise			(IEnumSTATDATA** vPpEnumAdvise);

	//Cons/Destructors
	MC_DataObject();
	~MC_DataObject();

	void	Add				(FORMATETC* vFormatEtc, STGMEDIUM* vStorageMedium, UINT vCount);
	void	Add				(CLIPFORMAT vClipboardFormat, DWORD vTymed, void* vUnionContent);
	size_t	Size			();
	bool	Empty			();
	void	Clear			();
	void	ClearAndRelease	();

private:
	HGLOBAL DupGlobalMem	(HGLOBAL vSourceMem);
	int		LookupFormatEtc	(FORMATETC* vFormatEtc);

	HRESULT CreateEnumFormatEtc(UINT vFormatCount, FORMATETC* vFormat, IEnumFORMATETC** vPpEnumFormatEtc);

	ULONG m_RefCount;										// COM reference count

	std::vector<FORMATETC> m_FormatEtc;
	std::vector<STGMEDIUM> m_StgMedium;
	
};
#endif
//----------------------------------------------------------------------------
