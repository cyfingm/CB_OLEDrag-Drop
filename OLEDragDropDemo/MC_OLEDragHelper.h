//----------------------------------------------------------------------------
/*
 *	@Author		:		chenyifu
 *	@Date		:		2015-2-27
 *	@Descrption :		提供OLE拖拽的各种操作(未完成)
 */
//----------------------------------------------------------------------------
#ifndef MC_OLEDragHelperH
#define MC_OLEDragHelperH
//----------------------------------------------------------------------------
#include "MC_OLEDragHeaders.h"
#include "MC_DropSource.h"
#include "MC_DataObject.h"
#include "MC_EnumFormatEtc.h"
#include <string>
//----------------------------------------------------------------------------
struct MC_DropFileListA : std::vector<std::string>
{
public:
	//Help with fill the STGMEDIUM`s hGlobal member
	void* GetFileDescription(void);

	MC_DropFileListA(void){;};
	~MC_DropFileListA(void){;};
};
//----------------------------------------------------------------------------
//This class is useful when doing ole drag&drop operation, but it`s not completely implement,by mow just support file drag&drop
class MC_OLEDragHelper
{
public:
	MC_OLEDragHelper(void){;};
	~MC_OLEDragHelper(void){;};

	bool IsDropSuccess(DWORD vDropResult, DWORD vDropEffect);

	bool DropFiles(MC_DropFileListA vFileList, DWORD vDesireEffect); //For file drag&drop

	//bool DropText();
};
//----------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------




