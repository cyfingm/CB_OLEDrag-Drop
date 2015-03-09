//----------------------------------------------------------------------------
/*
 *	@Author		:		chenyifu
 *	@Date		:		2015-2-27
 *	@Descrption :		实现OLE向其他程序拖拽需要实现IDropSource接口,表明
						一个拖放源
 */
//----------------------------------------------------------------------------
#include "MC_IComDropSource.h"
//----------------------------------------------------------------------------
MC_IComDropSource::MC_IComDropSource(void)
{
	m_RefCount = 1;
}
//----------------------------------------------------------------------------
MC_IComDropSource::~MC_IComDropSource(void)
{
}
//----------------------------------------------------------------------------
ULONG __stdcall MC_IComDropSource::AddRef(void)
{
	return InterlockedIncrement(&m_RefCount);
}
//----------------------------------------------------------------------------
ULONG __stdcall MC_IComDropSource::Release(void)
{
	ULONG tRet = InterlockedDecrement(&m_RefCount);

	if(0 == tRet)
		delete this;

	return tRet;
}
//----------------------------------------------------------------------------
STDMETHODIMP MC_IComDropSource::QueryInterface(REFIID vRIID, void** vPpvObject)
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
STDMETHODIMP MC_IComDropSource::QueryContinueDrag(BOOL vEscPressed, DWORD vMousseState)
{
	//若是按下ESC键，则返回RAGDROP_S_CANCEL告知拖拽操作被取消
	if(true == vEscPressed)
		return DRAGDROP_S_CANCEL;

	//若是鼠标左键被松开，则返回DRAGDROP_S_DROP表示文件已经完成拖拽操作
	if(0 == (vMousseState & MK_LBUTTON))
		return DRAGDROP_S_DROP;

	return S_OK;
}
//----------------------------------------------------------------------------
STDMETHODIMP MC_IComDropSource::GiveFeedback(DWORD vEffect)
{
	UNREFERENCED_PARAMETER(vEffect);

	return DRAGDROP_S_USEDEFAULTCURSORS;
}
//----------------------------------------------------------------------------
