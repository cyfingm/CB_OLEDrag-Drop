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
//    //��ȡ�ļ����ļ���
//	char FileName[256];
//	DragQueryFile((HDROP)msg.WParam,0,FileName,256);
//
//	//��ʾ��Label1����
//	MessageBoxA(NULL, "HandleDropFiles", "Hey", 0);
//
//    //�ͷ�Ӧ�ó���Ϊ�����ļ��������ٵ��ڴ�ռ�
//    DragFinish( HDROP(msg.WParam));
//}

