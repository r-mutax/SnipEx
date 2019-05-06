#pragma once
#include <afxwin.h>
class CTaskTrayApp :
	public CWnd
{
private:
	HICON m_hIcon_current;
	BOOL m_bFireDoubleClick;

public:
	CTaskTrayApp();
	~CTaskTrayApp();
	virtual BOOL Create();
	virtual void PostNcDestroy();
	BOOL NotifyIcon(DWORD dwMessage, HICON hIcon, LPCTSTR pszTip);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	afx_msg LRESULT OnUserTraynotifyicon(WPARAM wParam, LPARAM lParam);
	virtual void DoLButtonClick() {}
	virtual void DoRButtonClick() {}
	virtual void DoLButtonDoubleClick() {}
};

