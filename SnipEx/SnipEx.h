
// SnipEx.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CSnipExApp:
// このクラスの実装については、SnipEx.cpp を参照してください
//

class CSnipExApp : public CWinApp
{
public:
	CSnipExApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CSnipExApp theApp;
