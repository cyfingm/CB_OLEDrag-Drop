// ----------------------------------------------------------------------------
/*
  	@Author		:		陈艺夫
	@Date		:		2015-2-27
	@Descrption :		OLE拖拽功能的IDataObject接口,表明一个数据对象，可理解为
 						被拖拽文件的集合。
 */
//----------------------------------------------------------------------------
#ifndef MC_IComDragItemListH
#define MC_IComDragItemListH
//----------------------------------------------------------------------------
#include "MC_IComEnumFormatEtc.h"
#include <vector>
#include <Shldisp.h>
//----------------------------------------------------------------------------
class MC_IComDragItemList : public IDataObject, public IAsyncOperation
{
private:
	std::vector<FORMATETC>				m_FormatEtc; 				 //拖拽对象格式列表
	std::vector<STGMEDIUM>				m_StgMedium; 				 //拖拽对象的存储媒介

	CRITICAL_SECTION 					m_CriticalSection;			 //用该变量确保Add函数中vector的push_back线程安全

	HGLOBAL 	DupGlobalMem	   		(HGLOBAL vSourceMem); 	 	//用于复制GlobalAlloc分配的内存
	int 		LookupFormatEtc			(FORMATETC* vFormatEtc); 	//查找m_FormatEtc中是否存在匹配格式，成功返回对象下标，否则返回-1

	//产生数据对象的格式枚举器实例
	HRESULT 	CreateEnumFormatEtc		(size_t vCount, FORMATETC* vFmtList, IEnumFORMATETC** vPpEnumFormatEtc);

public:
	//IDataObject接口成员,GetData用于获取数据对象,由拖拽接受程序调用,SetData用于添加要传输的数据对象,由拖拽源调用
	STDMETHODIMP GetData				(FORMATETC* vFormatEtc, STGMEDIUM* vStorageMedium);
	STDMETHODIMP GetDataHere			(FORMATETC* vFormatEtc, STGMEDIUM* vStorageMedium);
	STDMETHODIMP QueryGetData			(FORMATETC* vFormatEtc);
	STDMETHODIMP GetCanonicalFormatEtc	(FORMATETC* vFormatEctIn, FORMATETC* vFormatEtcOut);
	STDMETHODIMP SetData				(FORMATETC* vFormatEct, STGMEDIUM* vStorageMedium, BOOL vRelease);
	STDMETHODIMP EnumFormatEtc			(DWORD vDirection, IEnumFORMATETC** vPpEnumFormatEtc);
	STDMETHODIMP DAdvise				(FORMATETC* vFormatEct, DWORD vAdvf, IAdviseSink* vAdvSink, DWORD* vConnection);
	STDMETHODIMP DUnadvise				(DWORD vConnection);
	STDMETHODIMP EnumDAdvise			(IEnumSTATDATA** vPpEnumAdvise);

	//COM接口成员
	ULONG __stdcall AddRef				(void);
	ULONG __stdcall Release				(void);
	STDMETHODIMP 	QueryInterface		(REFIID vRIID, void** vPpvObject);

	//IAsyncOperation接口成员
	STDMETHODIMP SetAsyncMode				(BOOL fDoOpAsync);
	STDMETHODIMP StartOperation				(IBindCtx *pbcReserved);
	STDMETHODIMP InOperation				(BOOL *pfInAsyncOp);
	STDMETHODIMP GetAsyncMode				(BOOL *pfIsOpAsync);
	STDMETHODIMP EndOperation				(HRESULT hResult,IBindCtx *pbcReserved,DWORD dwEffects);

	BOOL	m_IsAsync;
	BOOL	m_InOperation;

	volatile LONG m_RefCount; //COM接口引用计数变量

public:
	MC_IComDragItemList(void);
	~MC_IComDragItemList(void);

	//一次拖拽对应一个存储媒介(STGMEDIUM)和一个通用剪贴板格式(FORMATETC)
	void 	Add		(FORMATETC* vFormatEtc, STGMEDIUM* vStorageMedium, int vCount=1);	//向数据对象添加vCount个元素
	void 	Add		(CLIPFORMAT vFormat, DWORD vTymed, void* vUnionContent);			//向数据对象中添加1个指定格式和内容的元素

	//数据对象相关操作
	size_t 	Size			(void);
	void 	Clear			(void);
	void 	ClearAndRelease	(void); 			//清空队列并释放存储媒介的空间
};
//----------------------------------------------------------------------------
#endif
