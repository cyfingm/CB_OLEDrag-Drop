//#include "MC_FileListView.h"
//
//__fastcall MC_FileListView::MC_FileListView(TComponent *Owner)
//    : TListView(Owner)
//{
//}
//
//void __fastcall MC_FileListView::CreateWnd()
//{
//    inherited::CreateWnd();
//    DragAcceptFiles(Handle, TRUE);
//}
//
//void __fastcall MC_FileListView::DestroyWnd()
//{
//    DragAcceptFiles(Handle, FALSE);
//    inherited::DestroyWnd();
//}
//
//void __fastcall MC_FileListView::WndProc(TMessage& Msg)
//{
//    if (Msg.Msg == WM_DROPFILES)
//    {
//		;
//    }
//    else
//        inherited::WndProc(Msg);
//}
//
////---------------------------------------------------------------------------
//void __fastcall MC_FileListView::HandleDropFiles(TMessage &msg)
//{
//    //获取文件的文件名
//	char FileName[256];
//	DragQueryFile((HDROP)msg.WParam,0,FileName,256);
//
//	//显示到Label1里面
//	MessageBoxA(NULL, "HandleDropFiles", "Hey", 0);
//
//    //释放应用程序为传递文件名而开辟的内存空间
//    DragFinish( HDROP(msg.WParam));
//}

