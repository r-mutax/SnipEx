#include "stdafx.h"
#include "CTaskTrayApp.h"

// カスタムメッセージ
#define WM_USER_TRAYNOTIFYICON	(WM_USER+1)

// リソースのアイコンID?
#define ID_ICON					(100)

// ダブルクリック判定用のタイマ
#define ID_CLICKTIMER			(4)

//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
CTaskTrayApp::CTaskTrayApp()
{
}

//---------------------------------------------------------
// デストラクタ
//---------------------------------------------------------
CTaskTrayApp::~CTaskTrayApp()
{
}

//---------------------------------------------------------
// クリエイト
//---------------------------------------------------------
BOOL CTaskTrayApp::Create()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	return CreateEx(WS_EX_TOOLWINDOW,	// ウィンドウのExスタイル
		AfxRegisterWndClass(0),			// ウィンドウクラスの名前　デフォルトタイプで生成して指定
		_T("TrayWnd"),					// ウィンドウタイトル
		WS_OVERLAPPED,					// ウィンドウのスタイル
		0,								// 高さとか幅とか
		0,								// 表示しないので４つとも０
		0,								// 
		0,								// 
		NULL,							// 親ウィンドウのハンドル
		NULL);							// 子ウィンドウやメニューの識別子
}

//---------------------------------------------------------
// ウィンドウのデリート依頼
//---------------------------------------------------------
void CTaskTrayApp::PostNcDestroy()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	CWnd::PostNcDestroy();

	delete this;
}

//---------------------------------------------------------
// タスクトレイへのアイコンの追加、削除、変更を行う。
//---------------------------------------------------------
BOOL CTaskTrayApp::NotifyIcon(DWORD dwMessage, HICON hIcon, LPCTSTR pszTip /*= NULL*/)
{
	ASSERT(NIM_ADD == dwMessage
		|| NIM_DELETE == dwMessage
		|| NIM_MODIFY == dwMessage);

	NOTIFYICONDATA nid;
	ZeroMemory(&nid, sizeof(NOTIFYICONDATA));
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = GetSafeHwnd();
	nid.uID = ID_ICON;	
	nid.uCallbackMessage = WM_USER_TRAYNOTIFYICON;
	nid.uFlags = NIF_MESSAGE;

	if (NULL != hIcon
		&& m_hIcon_current != hIcon)
	{	// アイコン変更
		nid.uFlags |= NIF_ICON;
		nid.hIcon = hIcon;
		m_hIcon_current = hIcon;
	}

	if (NULL != pszTip)
	{	// ツールチップ表示
		nid.uFlags |= NIF_TIP;
		_tcsncpy_s(nid.szTip, sizeof(nid.szTip), pszTip, _TRUNCATE);
	}

	return Shell_NotifyIcon(dwMessage, &nid);
}

//---------------------------------------------------------
// メッセージマップ
//---------------------------------------------------------
BEGIN_MESSAGE_MAP(CTaskTrayApp, CWnd)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONUP()
	ON_WM_TIMER()
	ON_MESSAGE(WM_USER_TRAYNOTIFYICON, &CTaskTrayApp::OnUserTraynotifyicon)
END_MESSAGE_MAP()

//---------------------------------------------------------
// マウスの左ボタンアップでのイベントハンドラ
//---------------------------------------------------------
void CTaskTrayApp::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	DoLButtonClick();

	//	CWnd::OnLButtonUp(nFlags, point);
}

//---------------------------------------------------------
// マウスの左ボタンのダブルクリックでのイベントハンドラ
//---------------------------------------------------------
void CTaskTrayApp::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	DoLButtonDoubleClick();

	//	CWnd::OnLButtonDblClk(nFlags, point);
}

//---------------------------------------------------------
// マウスの右ボタンアップでのイベントハンドラ
//---------------------------------------------------------
void CTaskTrayApp::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	DoRButtonClick();

	//	CWnd::OnRButtonUp(nFlags, point);
}

//---------------------------------------------------------
// タイマイベントのハンドラ
//---------------------------------------------------------
void CTaskTrayApp::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	
	// OnUserTraynotifyicon()でセットしたタイマを経過したら
	// タイマセットしたときのボタンダウンがダブルクリックでないと判断してメッセージを投げる。
	if (ID_CLICKTIMER == nIDEvent)
	{
		KillTimer(nIDEvent);
		PostMessage(WM_LBUTTONUP);
	}

//	CWnd::OnTimer(nIDEvent);
}

//---------------------------------------------------------
// タスクトレイのアイコンをクリックした時に発生するイベント
//---------------------------------------------------------
afx_msg LRESULT CTaskTrayApp::OnUserTraynotifyicon(WPARAM wParam, LPARAM lParam)
{
	UINT uiIconID = (UINT)wParam;
	UINT uiMouseMsg = (UINT)lParam;

	if (ID_ICON != uiIconID)
	{
		return 0;
	}

	switch (uiMouseMsg)
	{
	case WM_LBUTTONDOWN:
		m_bFireDoubleClick = FALSE;
		SetTimer(ID_CLICKTIMER, GetDoubleClickTime(), NULL);
		break;

	case WM_LBUTTONUP:
		if (m_bFireDoubleClick)
		{
			PostMessage(WM_LBUTTONDBLCLK);
		}
		break;

	case WM_LBUTTONDBLCLK:
		m_bFireDoubleClick = TRUE;
		KillTimer(ID_CLICKTIMER);
		break;

	case WM_RBUTTONUP:
		PostMessage(WM_RBUTTONUP);
		break;
	}

	return 0;
}
