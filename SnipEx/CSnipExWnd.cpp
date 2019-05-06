#include "stdafx.h"
#include "CSnipExWnd.h"
#include "SnipEx.h"
#include "SnipExDlg.h"

//---------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------
CSnipExWnd::CSnipExWnd()
{
}

//---------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------
CSnipExWnd::~CSnipExWnd()
{
}

//---------------------------------------------------------
// �g���C�A�C�R���ł̍��{�^���N���b�N
//---------------------------------------------------------
void CSnipExWnd::DoLButtonClick()
{
}

//---------------------------------------------------------
// �g���C�A�C�R���ł̉E�{�^���N���b�N
//---------------------------------------------------------
void CSnipExWnd::DoRButtonClick()
{
	POINT pt;
	GetCursorPos(&pt);

	CMenu menu;
	menu.LoadMenu(IDR_R_CLK_MENU);

	CMenu*	pPopup = menu.GetSubMenu(0);

	// SetForgroundWindow��PostMessage���K�v�ȗ��R�́A
	// Knowledge Base (Q135788)�Q�Ƃ̂���
	SetForegroundWindow();
	pPopup->TrackPopupMenu(TPM_RIGHTBUTTON, pt.x, pt.y, this);
	PostMessage(WM_NULL);

}

//---------------------------------------------------------
// �g���C�A�C�R���ł̍��{�^���̃_�u���N���b�N
//---------------------------------------------------------
void CSnipExWnd::DoLButtonDoubleClick()
{
}

//---------------------------------------------------------
// ���b�Z�[�W�}�b�v
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
// �A�v���P�[�V�����̏I��
// (���j���[�̃C�x���g�n���h��)
//---------------------------------------------------------
void CSnipExWnd::OnAppExit()
{
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂��B
	PostMessage(WM_CLOSE);
}

//---------------------------------------------------------
// Create����
//---------------------------------------------------------
int CSnipExWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTaskTrayApp::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: �����ɓ���ȍ쐬�R�[�h��ǉ����Ă��������B
	// �A�C�R���̃��[�h�ƃ^�X�N�g���C�ւ̓o�^
	m_hIcon_main = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	NotifyIcon(NIM_ADD, m_hIcon_main, _T("SnipEx"));
	
	// INI�t�@�C���̓ǂݍ���
	ReadINIFile();

	return 0;
}

//---------------------------------------------------------
// Destroy����
//---------------------------------------------------------
void CSnipExWnd::OnDestroy()
{
	CTaskTrayApp::OnDestroy();

	NotifyIcon(NIM_DELETE, m_hIcon_main, _T("SnipEx"));

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	ChangeClipboardChain(m_hNextViewr);
}

//---------------------------------------------------------
// HotKey�������ꂽ�Ƃ��̃C�x���g�n���h��
//---------------------------------------------------------
void CSnipExWnd::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	CTaskTrayApp::OnHotKey(nHotKeyId, nKey1, nKey2);

	// ���������z�b�g�L�[���uSnippingStart�v�������ꍇ
	if (nHotKeyId == SNIP_START_KEY)
	{
		// ���݂̏�Ԃ�؂�ւ���B
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
// �N���b�v�{�[�h�̊Ď����J�n����
//---------------------------------------------------------
void CSnipExWnd::StartClipBoardChk()
{
	// �N���b�v�{�[�h�̃r���[�A�`�F�C���Ɏ��M��o�^����B
	m_hNextViewr = SetClipboardViewer();

	// �N���b�v�{�[�h�Ď����ɂ���
	m_bIsClipboardChk = TRUE;
}

//---------------------------------------------------------
// �N���b�v�{�[�h�̊Ď����I������
//---------------------------------------------------------
void CSnipExWnd::EndClipBoardChk()
{
	// �N���b�v�{�[�h�̃r���[�A�`�F�C������폜����B
	ChangeClipboardChain(m_hNextViewr);

	// �N���b�v�{�[�h��Ď���Ԃɂ���B
	m_bIsClipboardChk = FALSE;
}

//---------------------------------------------------------
// �N���b�v�{�[�h�r���[�A�폜���̃C�x���g�n���h��
//---------------------------------------------------------
void CSnipExWnd::OnChangeCbChain(HWND hWndRemove, HWND hWndAfter)
{
	CTaskTrayApp::OnChangeCbChain(hWndRemove, hWndAfter);

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	// ���Ƀ`�F�C�����甲����E�B���h�E���A���̃A�v���̎��̈ʒu�ɑ��݂���E�B���h�E�������ꍇ
	if (m_hNextViewr == hWndRemove)
	{
		// �C�x���g�̓n������X�V����B
		m_hNextViewr = hWndAfter;
	}
	else
	{
		//�@���̂܂܎��̃`�F�[���ɗ���
		if (m_hNextViewr != NULL)
		{
			CWnd::FromHandle(m_hNextViewr)->SendMessage(WM_CHANGECBCHAIN, (WPARAM)hWndRemove, (LPARAM)hWndAfter);
		}
	}

	return;
}

//---------------------------------------------------------
// �N���b�v�{�[�h�X�V���̃C�x���g�n���h��
//---------------------------------------------------------
void CSnipExWnd::OnDrawClipboard()
{
	HANDLE hClipData = NULL;
	
	CTaskTrayApp::OnDrawClipboard();

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B

	OpenClipboard();

	// �N���b�v�{�[�h�̍X�V�l���C���[�W��������擾����B
	hClipData = GetClipboardData(CF_BITMAP);
	
	if (hClipData != NULL)
	{
		// �擾�����ꍇ�̓N���b�v�{�[�h�̓��e���摜�Ƃ��ĕۑ�����B
		SaveBitmap((HBITMAP)hClipData);
	}

	// �N���b�v�{�[�h�����
	CloseClipboard();
}

//---------------------------------------------------------
// jpg�t�@�C���ۑ�����
//---------------------------------------------------------
void CSnipExWnd::SaveBitmap(HBITMAP hBitMap)
{
	// �C���[�W��ۑ�������

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
// �u�ݒ�v���j���[�������̃C�x���g�n���h��
//---------------------------------------------------------
void CSnipExWnd::OnMSetting()
{
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂��B
	CSnipExDlg pWnd = new CSnipExDlg;

	if (pWnd.DoModal() == IDOK)
	{
		
	}
}

//---------------------------------------------------------
// ���s�t�@�C���̑��݂���f�B���N�g���p�X���擾����
//---------------------------------------------------------
void CSnipExWnd::GetModulePath(CString &modDir)
{
	TCHAR path[MAX_PATH];   //�p�X�擾�p
	GetModuleFileName(NULL, path, MAX_PATH);

	CString c_path = path;
	modDir = c_path.Left(c_path.ReverseFind(_T('\\')) + 1);
}

//---------------------------------------------------------
// INI�t�@�C����ǂݍ���
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

	// �t�H���_�����݂��Ȃ������ꍇ
	if (PathFileExists(m_strSaveDir) == FALSE)
	{
		CreateDirectory(m_strSaveDir, NULL);
	}

}

//---------------------------------------------------------
// path������̏I�[���u\�v�ɐ�����
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
