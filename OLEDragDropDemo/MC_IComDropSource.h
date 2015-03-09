//----------------------------------------------------------------------------
/*
 *	@Author		:		chenyifu
 *	@Date		:		2015-2-27
 *	@Descrption :		实现OLE向其他程序拖拽需要实现IDropSource接口,表明
						一个拖放源
 */
//----------------------------------------------------------------------------
#ifndef MC_IComDropSourceH
#define MC_IComDropSourceH
//----------------------------------------------------------------------------
#include <ole2.h>
//----------------------------------------------------------------------------
class MC_IComDropSource : public IDropSource
{
public:
    //IDropSource接口成员
	STDMETHODIMP 	QueryContinueDrag	(BOOL vEscPressed, DWORD vMousseState);  //定义拖拽过程中的对鼠标按键和键盘的响应
	STDMETHODIMP	GiveFeedback		(DWORD vEffect);                         //定义拖拽时的鼠标的样式

	//COM接口成员
	ULONG __stdcall AddRef				(void);
	ULONG __stdcall Release				(void);
	STDMETHODIMP 	QueryInterface		(REFIID vRIID, void **vPpvObject);

	volatile LONG 	m_RefCount;		//COM引用计数变量

public:
	MC_IComDropSource(void);
	~MC_IComDropSource(void);
};
//----------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------