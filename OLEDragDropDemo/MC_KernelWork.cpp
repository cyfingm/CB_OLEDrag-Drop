////---------------------------------------------------------------------------
//#include "MC_KernelWork.h"
//#include "MC_InterfaceImport.h"
////---------------------------------------------------------------------------
//MC_KernelWork MyKernelWork;
////---------------------------------------------------------------------------
//MC_KernelWork::MC_KernelWork(void)
//{
//	m_Available	= false;
//}
////---------------------------------------------------------------------------
//MC_KernelWork::~MC_KernelWork(void)
//{
//}
////---------------------------------------------------------------------------
//bool MC_KernelWork::Initialize(void)
//{
////	if(true == m_Available)
////		return true;
////
////	if(false == MyInterfaceImport.Load())
////		return false;
////
////	m_Available	= true;
//	return true;
//}
////---------------------------------------------------------------------------
//bool MC_KernelWork::UnInitialize(void)
//{
////	if(false == m_Available)
////		return true;
////
////	MyInterfaceImport.Unload();
////
////	m_Available = false;
//	return true;
//}
////---------------------------------------------------------------------------
//bool MC_KernelWork::TestIXReserve(void)
//{
////	if(false == m_Available)
////		return false;
////
////	IXReserve_ExampleBC* tIXReserve = (IXReserve_ExampleBC*)MyInterfaceImport.m_IXFactoryExample.m_IXReserve;
////	if(0 == tIXReserve)
////		return false;
////
////	tIXReserve->xBoolRW	=	true;
////	tIXReserve->xBoolRW	|=	false;
////	if(true !=	(tIXReserve->xBoolRW | false))	return false;
////	if(false != (tIXReserve->xBoolRW & false))	return false;
////
////	tIXReserve->xBoolRW	&=	false;
////	if(false !=	(false | tIXReserve->xBoolRW))	return false;
////	if(false != (false & tIXReserve->xBoolRW))	return false;
////
////	tIXReserve->xBoolRW	= false;
////	bool tBool			= tIXReserve->xBoolRW;
////
////	tIXReserve->xIntRW	= 1;
////	tIXReserve->xIntRW++;
////	++tIXReserve->xIntRW;
////	if(3 != tIXReserve->xIntRW)					return false;
////	if(1 != (tIXReserve->xIntRW&1))				return false;
////	if(7 != (tIXReserve->xIntRW|4))				return false;
////
////	tIXReserve->xIntRW +=	1;
////	tIXReserve->xIntRW -=	1;
////	tIXReserve->xIntRW *=	2;
////	tIXReserve->xIntRW /=	2;
////	tIXReserve->xIntRW <<=	2;
////	tIXReserve->xIntRW >>=	2;
////	tIXReserve->xIntRW %=	2;
////	if(1 != tIXReserve->xIntRW)
////		return false;
////
////	int tInt = tIXReserve->xIntRW;
////
////	tIXReserve->xStringARW	= "HelloWorld";
////	char* tString			= tIXReserve->xStringARW;
////
////	MC_KKMemSpace tMemSpace;
////
////	tIXReserve->xPointerRW	= &tMemSpace;
////	if(0 == tIXReserve->xPointerRW->Alloc(1024))
////		return false;
////
////	memset(tIXReserve->xPointerRW->GetKKBuffer()->_dat, 1, 1024);
////
////	tIXReserve->xOnEvent.Bind(0, &_OnEventReserveB);
////	tIXReserve->xHelloWorld();
////
////	tIXReserve->xOnEvent.Bind(this, avx_GetClassFunctionAddr(&MC_KernelWork::_OnEventReserveA));
////	tIXReserve->xHelloWorld();
//	return true;
//}
////---------------------------------------------------------------------------
//void __stdcall MC_KernelWork::_OnEventReserveA(IXKKEvent_Default_Params)
//{
//	return;
//}
////---------------------------------------------------------------------------
//void __stdcall MC_KernelWork::_OnEventReserveB(IXKKEvent_Default_Params)
//{
//	return;
//}
////---------------------------------------------------------------------------

