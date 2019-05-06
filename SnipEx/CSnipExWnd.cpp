#include "stdafx.h"
#include "CSnipExWnd.h"
#include "SnipEx.h"
#include "SnipExDlg.h"

//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
CSnipExWnd::CSnipExWnd()
{
}

//---------------------------------------------------------
// デストラクタ
//---------------------------------------------------------
CSnipExWnd::~CSnipExWnd()
{
}

//---------------------------------------------------------
// トレイアイコンでの左ボタンクリック
//---------------------------------------------------------
void CSnipExWnd::DoLButtonClick()
{
}

//---------------------------------------------------------
// トレイアイコンでの右ボタンクリック
//---------------------------------------------------------
void CSnipExWnd::DoRButtonClick()
{
	POINT pt;
	GetCursorPos(&pt);

	CMenu menu;
	menu.LoadMenu(IDR_R_CLK_MENU);

	CMenu*	pPopup = menu.GetSubMenu(0);

	// SetForgroundWindowとPostMessageが必要な理由は、
	// Knowledge Base (Q135788)参照のこと
	SetForegroundWindow();
	pPopup->TrackPopupMenu(TPM_RIGHTBUTTON, pt.x, pt.y, this);
	PostMessage(WM_NULL);

}

//---------------------------------------------------------
// トレイアイコンでの左ボタンのダブルクリック
//---------------------------------------------------------
void CSnipExWnd::DoLButtonDoubleClick()
{
}

//---------------------------------------------------------
// メッセージマップ
//---------------------------------------------------------
BEGIN_MESSAGE_MAP(CSnipExWnd, CTaskTrayApp)
	ON_COMMAND(ID_APP_EXIT, &CSnipExWnd::OnAppExit)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_HOTKEY()
	ON_WM_CHANGECBCHAIN()
	ON_WM_DRAWCLIPBOARD()
	ON_COMMAND(ID_M_SETTING, &CSnipExWnd::OnMSetting)
END_MESSAGE_MAP()

//---------------------------------------------------------
// アプリケーションの終了
// (メニューのイベントハンドラ)
//---------------------------------------------------------
void CSnipExWnd::OnAppExit()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	PostMessage(WM_CLOSE);
}

//---------------------------------------------------------
// Create処理
//---------------------------------------------------------
int CSnipExWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTaskTrayApp::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: ここに特定な作成コードを追加してください。
	// アイコンのロードとタスクトレイへの登録
	m_hIcon_main = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	NotifyIcon(NIM_ADD, m_hIcon_main, _T("SnipEx"));
	
	// INIファイルの読み込み
	ReadINIFile();

	return 0;
}

//---------------------------------------------------------
// Destroy処理
//---------------------------------------------------------
void CSnipExWnd::OnDestroy()
{
	CTaskTrayApp::OnDestroy();

	NotifyIcon(NIM_DELETE, m_hIcon_main, _T("SnipEx"));

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	ChangeClipboardChain(m_hNextViewr);
}

//---------------------------------------------------------
// HotKeyが押されたときのイベントハンドラ
//---------------------------------------------------------
void CSnipExWnd::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	CTaskTrayApp::OnHotKey(nHotKeyId, nKey1, nKey2);

	// 押下したホットキーが「SnippingStart」だった場合
	if (nHotKeyId == SNIP_START_KEY)
	{
		// 現在の状態を切り替える。
		if (m_bIsClipboardChk == TRUE)
		{
			EndClipBoardChk();
		}
		else
		{
			StartClipBoardChk();
		}
	}
}

//---------------------------------------------------------
// クリップボードの監視を開始する
//---------------------------------------------------------
void CSnipExWnd::StartClipBoardChk()
{
	// クリップボードのビューアチェインに自信を登録する。
	m_hNextViewr = SetClipboardViewer();

	// クリップボード監視中にする
	m_bIsClipboardChk = TRUE;
}

//---------------------------------------------------------
// クリップボードの監視を終了する
//---------------------------------------------------------
void CSnipExWnd::EndClipBoardChk()
{
	// クリップボードのビューアチェインから削除する。
	ChangeClipboardChain(m_hNextViewr);

	// クリップボード非監視状態にする。
	m_bIsClipboardChk = FALSE;
}

//---------------------------------------------------------
// クリップボードビューア削除時のイベントハンドラ
//---------------------------------------------------------
void CSnipExWnd::OnChangeCbChain(HWND hWndRemove, HWND hWndAfter)
{
	CTaskTrayApp::OnChangeCbChain(hWndRemove, hWndAfter);

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	// 次にチェインから抜けるウィンドウが、このアプリの次の位置に存在するウィンドウだった場合
	if (m_hNextViewr == hWndRemove)
	{
		// イベントの渡し先を更新する。
		m_hNextViewr = hWndAfter;
	}
	else
	{
		//　そのまま次のチェーンに流す
		if (m_hNextViewr != NULL)
		{
			CWnd::FromHandle(m_hNextViewr)->SendMessage(WM_CHANGECBCHAIN, (WPARAM)hWndRemove, (LPARAM)hWndAfter);
		}
	}

	return;
}

//---------------------------------------------------------
// クリップボード更新時のイベントハンドラ
//---------------------------------------------------------
void CSnipExWnd::OnDrawClipboard()
{
	HANDLE hClipData = NULL;
	
	CTaskTrayApp::OnDrawClipboard();

	// TODO: ここにメッセージ ハンドラー コードを追加します。

	OpenClipboard();

	// クリップボードの更新値がイメージだったら取得する。
	hClipData = GetClipboardData(CF_BITMAP);
	
	if (hClipData != NULL)
	{
		// 取得した場合はクリップボードの内容を画像として保存する。
		SaveBitmap((HBITMAP)hClipData);
	}

	// クリップボードを閉じる
	CloseClipboard();
}

//---------------------------------------------------------
// jpgファイル保存処理
//---------------------------------------------------------
void CSnipExWnd::SaveBitmap(HBITMAP hBitMap)
{
	// イメージを保存させる

	CImage image;
	image.Attach(hBitMap);

	CTime cTime = CTime::GetCurrentTime();
	CString time = cTime.Format("%Y-%m-%d_%H-%M-%S");

	CString path = m_strSaveDir;
	path.Append(time);
	path.Append(TEXT(".jpg"));

	image.Save(path);
	image.Detach();
}

//---------------------------------------------------------
// 「設定」メニュー押下時のイベントハンドラ
//---------------------------------------------------------
void CSnipExWnd::OnMSetting()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	CSnipExDlg pWnd = new CSnipExDlg;

	if (pWnd.DoModal() == IDOK)
	{
		
	}
}

//---------------------------------------------------------
// 実行ファイルの存在するディレクトリパスを取得する
//---------------------------------------------------------
void CSnipExWnd::GetModulePath(CString &modDir)
{
	TCHAR path[MAX_PATH];   //パス取得用
	GetModuleFileName(NULL, path, MAX_PATH);

	CString c_path = path;
	modDir = c_path.Left(c_path.ReverseFind(_T('\\')) + 1);
}

//---------------------------------------------------------
// INIファイルを読み込む
//---------------------------------------------------------
void CSnipExWnd::ReadINIFile()
{
	TCHAR	ss[256];
	TCHAR	tcDir[256];
	CString	inipath;

	GetModulePath(inipath);

	inipath.Append(TEXT("CONFIG.INI"));

	_tcscpy_s(ss, inipath);

	GetPrivateProfileString(TEXT("drive"), TEXT("save_dir"), TEXT("C:\\temp\\"), tcDir, sizeof(tcDir), ss);

	m_strSaveDir = tcDir;

	FormatDirPath(m_strSaveDir);

	// フォルダが存在しなかった場合
	if (PathFileExists(m_strSaveDir) == FALSE)
	{
		CreateDirectory(m_strSaveDir, NULL);
	}

}

//---------------------------------------------------------
// path文字列の終端を「\」に整える
//---------------------------------------------------------
void CSnipExWnd::FormatDirPath(CString &ss)
{
	long	_lYenPos;
	_lYenPos = ss.ReverseFind(_T('\\'));

	if (_lYenPos != ss.GetLength())
	{
		ss.AppendChar(_T('\\'));
	}
}
