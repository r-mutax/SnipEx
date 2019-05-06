#pragma once
#include "CTaskTrayApp.h"

#define SNIP_START_KEY	0


class CSnipExWnd :	public CTaskTrayApp
{
private:
	HICON	m_hIcon_main;
	HWND	m_hNextViewr;
	BOOL	m_bIsClipboardChk;
	CString	m_strSaveDir;

public:
	CSnipExWnd();
	~CSnipExWnd();
	void DoLButtonClick();
	void DoRButtonClick();
	void DoLButtonDoubleClick();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnAppExit();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	void StartClipBoardChk();
	void EndClipBoardChk();
	afx_msg void OnChangeCbChain(HWND hWndRemove, HWND hWndAfter);
	afx_msg void OnDrawClipboard();
	void SaveBitmap(HBITMAP hBitMap);
	afx_msg void OnMSetting();
	void GetModulePath(CString & modDir);

	void ReadINIFile();

	void FormatDirPath(CString &ss);

};

