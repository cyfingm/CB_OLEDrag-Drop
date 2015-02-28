//----------------------------------------------------------------------------
/*
 *	@Author		:		chenyifu
 *	@Date		:		2015-2-27
 *	@Descrption :		实现OLE向其他程序拖拽需要实现IDropSource接口,表明本程序
						是一个拖放源
 */
//----------------------------------------------------------------------------
#include "MC_DropSource.h"
//----------------------------------------------------------------------------
MC_DropSource::MC_DropSource(void)
:m_RefCount(1)
{
	;
}
//----------------------------------------------------------------------------
MC_DropSource::~MC_DropSource(void)
{
	;
}
//----------------------------------------------------------------------------
//COM 接口成员:AddRef
ULONG __stdcall MC_DropSource::AddRef(void)
{
	return ++m_RefCount;
}
//----------------------------------------------------------------------------
//COM 接口成员:Release
ULONG __stdcall MC_DropSource::Release(void)
{
	--m_RefCount;

	if (m_RefCount == 0)
		delete this;

	return m_RefCount;
}
//----------------------------------------------------------------------------
//COM 接口成员:QueryInterface
STDMETHODIMP MC_DropSource::QueryInterface(REFIID vRIID, void** vPpvObject) 
{
	if((vRIID == IID_IUnknown)||(vRIID == IID_IDataObject))
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
STDMETHODIMP MC_DropSource::QueryContinueDrag(BOOL vEscapePressed, DWORD vKeyState)
{
	return vEscapePressed ? DRAGDROP_S_CANCEL : ((vKeyState&MK_LBUTTON) ? S_OK : DRAGDROP_S_DROP);
}
//----------------------------------------------------------------------------
STDMETHODIMP MC_DropSource::GiveFeedback(DWORD vEffect)
{
	UNREFERENCED_PARAMETER(vEffect);
	return DRAGDROP_S_USEDEFAULTCURSORS;
}
//----------------------------------------------------------------------------