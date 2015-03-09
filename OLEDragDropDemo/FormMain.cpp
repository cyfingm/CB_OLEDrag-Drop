//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include "FormMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TForm1::ShowFileInDisk(void)
{
	//获取D盘文件列表及个数
	MC_KKFilePathTrack tFileTrack(L"D:\\", L"*");
	int tFileCount = (int)tFileTrack.m_FileList.size();

	//添加文件列
	m_FileListView->Items->BeginUpdate();        //BeginUpdate和EndUpdate()用以防止添加多个TListItem时的闪烁问题
	m_FileListView->Items->Clear();

	//添加Item
	TListItem* tItem;
	for(int i=0; i<tFileCount; i++)
		{
		tItem = m_FileListView->Items->Add();
		KKSTR tFileName = avx_KKSCombine(L"D:\\%s", KKStringW(tFileTrack.m_FileList[i].Name));
		tItem->Caption  = KKStringW(tFileName);
		}

	m_FileListView->Items->EndUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
	delete m_FileListView;
	delete m_DragKernel;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
   	m_DragKernel = new MC_ListViewDragKernel();
	if(0 == m_DragKernel)
		return;

	if(false == m_DragKernel->BindListView(m_FileListView))
		return;

	ShowFileInDisk();
}
//---------------------------------------------------------------------------


