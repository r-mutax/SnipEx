
// SnipEx.cpp : アプリケーションのクラス動作を定義します。
//

#include "stdafx.h"
#include "SnipEx.h"
#include "SnipExDlg.h"
#include "CSnipExWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SNIP_START_KEY	0

// CSnipExApp

BEGIN_MESSAGE_MAP(CSnipExApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSnipExApp の構築

CSnipExApp::CSnipExApp()
{
	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}


// 唯一の CSnipExApp オブジェクト

CSnipExApp theApp;


// CSnipExApp の初期化

BOOL CSnipExApp::InitInstance()
{
	CWinApp::InitInstance();


	// ダイアログにシェル ツリー ビューまたはシェル リスト ビュー コントロールが
	// 含まれている場合にシェル マネージャーを作成します。
	CShellManager *pShellManager = new CShellManager;

	// MFC コントロールでテーマを有効にするために、"Windows ネイティブ" のビジュアル マネージャーをアクティブ化
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 標準初期化
	// これらの機能を使わずに最終的な実行可能ファイルの
	// サイズを縮小したい場合は、以下から不要な初期化
	// ルーチンを削除してください。
	// 設定が格納されているレジストリ キーを変更します。
	// TODO: 会社名または組織名などの適切な文字列に
	// この文字列を変更してください。
	SetRegistryKey(_T("アプリケーション ウィザードで生成されたローカル アプリケーション"));

	CSnipExWnd* pWnd = new CSnipExWnd();
	pWnd->Create();
	m_pMainWnd = pWnd;
	
	// HotKeyの登録
	RegisterHotKey(m_pMainWnd->m_hWnd, SNIP_START_KEY, MOD_ALT, VK_SPACE);
	
	return TRUE;


}

