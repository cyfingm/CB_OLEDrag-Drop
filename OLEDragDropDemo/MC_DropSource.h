//----------------------------------------------------------------------------
/*
 *	@Author		:		chenyifu
 *	@Date		:		2015-2-27
 *	@Descrption :		实现OLE向其他程序拖拽需要实现IDropSource接口,表明本程序
						是一个拖放源
 */
//----------------------------------------------------------------------------
#ifndef MC_DropSourceH
#define MC_DropSourceH
//----------------------------------------------------------------------------
#include "MC_OLEDragHeaders.h"
//----------------------------------------------------------------------------
class MC_DropSource : public IDropSource
{
public:
	//COM intreface
	ULONG __stdcall AddRef			(void);
	ULONG __stdcall Release			(void);
	STDMETHODIMP 	QueryInterface	(REFIID vRIID, void **vPpvObject);

    //IDropSource interface
	STDMETHODIMP QueryContinueDrag	(BOOL vEscapePressed, DWORD vKeyState);
    STDMETHODIMP GiveFeedback		(DWORD vEffect);

	//Cons/Destructors
	MC_DropSource();
	~MC_DropSource();

private:
    LONG m_RefCount;	// COM reference count
};
//----------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------