//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainWindow.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Label1StartDrag(TObject *Sender, TDragObject *&DragObject)
{
	//file to copy
	char tFileName[256] = "C:\\windows\\explorer.exe";

	//initialize OLE
	OleInitialize(0);

	//Prepare a file list, if you have more than one file to copy, just do push_back
	MC_OLEDragHelper tDragHelper;
	MC_DropFileListA tFiles;
	std::string tTargetFile(tFileName);
	tFiles.push_back(tTargetFile);

	//DropFiles will call DoDragDrop for us
	if(false == tDragHelper.DropFiles(tFiles, DROPEFFECT_COPY))
		MessageBoxA(NULL, "Drop file failed!!", "Error", 0);

	//uninitialize OLE
	OleUninitialize();

	return;
}
//---------------------------------------------------------------------------
