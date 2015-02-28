//----------------------------------------------------------------------------
/*
 *	@Author		:		chenyifu
 *	@Date		:		2015-2-27
 *	@Descrption :		提供OLE拖拽的各种操作(未完成)
 */
//----------------------------------------------------------------------------
#include "MC_OLEDragHelper.h"
//----------------------------------------------------------------------------
void* MC_DropFileListA::GetFileDescription(void)
{
	struct TMyDropFiles{
		DWORD pFiles;
		POINT pt;
		BOOL  fNC;
		BOOL  fWide;
        	char  szFiles[1];
	};

	//Get file descrpition length
	size_t tFileDescriptionLength=0;
	for(const_iterator i = begin(); i != end(); ++i)
		{
		tFileDescriptionLength += i->length();
		++tFileDescriptionLength;
		}

	//Ready a TMyDropFiles(A structure like DROPFILES, but I always get failed when using DROPFILES)
	void* tSpace=GlobalAlloc(GMEM_FIXED, sizeof(TMyDropFiles) + tFileDescriptionLength);
	ZeroMemory(tSpace, sizeof(TMyDropFiles) + tFileDescriptionLength);

	TMyDropFiles *tFileNames = new (tSpace)TMyDropFiles;
	tFileNames->pFiles = (unsigned)tFileNames->szFiles - (unsigned)(&tFileNames->pFiles);

	//Fill the filenames
	char *fileListString = tFileNames->szFiles;
	size_t tLen = 0;
	for(const_iterator i = begin();i != end(); ++i)
		{
		tLen = i->length();
		CopyMemory(fileListString, i->c_str(), tLen);
		fileListString += tLen;
		*(fileListString++) = 0;
		}
	//Shoul terminate with two 0
	*fileListString = 0;

	return tSpace;
}
//----------------------------------------------------------------------------
bool MC_OLEDragHelper::DropFiles(MC_DropFileListA vFileList, DWORD vDesireEffect)
{
	FORMATETC tFormatEtc	 = {CF_HDROP, 0, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
	STGMEDIUM tStorageMedium = {TYMED_HGLOBAL, {(HBITMAP)vFileList.GetFileDescription()}, 0};

	IDropSource *tDropSource   = new MC_DropSource();
	MC_DataObject *tDataObject = new MC_DataObject();

	//Operation like setData
	tDataObject->Add(&tFormatEtc, &tStorageMedium, 1);
	DWORD tEffect;

	//Invoke DoDragDrop
	DWORD tDragResult = DoDragDrop(tDataObject, tDropSource, vDesireEffect, &tEffect);

	//Check DoDragDrop
	IsDropSuccess(tDragResult, tEffect);

	//Cleaning
	ReleaseStgMedium(&tStorageMedium);
	tDropSource->Release();
	tDataObject->Release();

	return true;
}
//----------------------------------------------------------------------------
bool MC_OLEDragHelper::IsDropSuccess(DWORD vDropResult, DWORD vDropEffect)
{
   //Check return value of DoDragDrop
	if(vDropResult != DRAGDROP_S_DROP)
		{
		if(vDropResult == DRAGDROP_S_CANCEL)
			MessageBoxA(0, "DRAGDROP_S_CANCEL!", "DRAGDROP_S_DROP", 0);
		else
			MessageBoxA(0, "E_UNSPEC!", "DRAGDROP_S_DROP", 0);
		return false;
		}

	//Check drop effect
	if(vDropEffect == DROPEFFECT_NONE)
		{
		MessageBoxA(0, "Ole data not accepted!!", "DRAGDROP_S_DROP", 0);
		return false;
		}
	else if((vDropEffect & DROPEFFECT_MOVE) == DROPEFFECT_MOVE)
		MessageBoxA(0, "Ole data moved!!", "DRAGDROP_S_DROP", 0);

	else if((vDropEffect & DROPEFFECT_COPY) == DROPEFFECT_COPY)
		MessageBoxA(0, "Ole data copied!!", "DRAGDROP_S_DROP", 0);

	return true;
}
