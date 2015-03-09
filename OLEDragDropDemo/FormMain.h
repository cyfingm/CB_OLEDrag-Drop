//---------------------------------------------------------------------------
#ifndef FormMainH
#define FormMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Ole2.h>
#include <windows.h>
#include <Shlobj.h>
#include <ComCtrls.hpp>
#include "MC_ListViewDragKernel.h"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TLabel* 		m_TipLabel;
	TListView* 		m_FileListView;

	void __fastcall FormClose		(TObject* Sender, TCloseAction& Action);
	void __fastcall FormCreate		(TObject* Sender);

private:	// User declarations
	MC_ListViewDragKernel* m_DragKernel;

public:		// User declarations
	__fastcall 	TForm1			(TComponent* Owner);

	void 		ShowFileInDisk	(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
