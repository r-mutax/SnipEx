#include "stdafx.h"
#include "CTaskTrayApp.h"

// �J�X�^�����b�Z�[�W
#define WM_USER_TRAYNOTIFYICON	(WM_USER+1)

// ���\�[�X�̃A�C�R��ID?
#define ID_ICON					(100)

// �_�u���N���b�N����p�̃^�C�}
#define ID_CLICKTIMER			(4)

//---------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------
CTaskTrayApp::CTaskTrayApp()
{
}

//---------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------
CTaskTrayApp::~CTaskTrayApp()
{
}

//---------------------------------------------------------
// �N���G�C�g
//---------------------------------------------------------
BOOL CTaskTrayApp::Create()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B

	return CreateEx(WS_EX_TOOLWINDOW,	// �E�B���h�E��Ex�X�^�C��
		AfxRegisterWndClass(0),			// �E�B���h�E�N���X�̖��O�@�f�t�H���g�^�C�v�Ő������Ďw��
		_T("TrayWnd"),					// �E�B���h�E�^�C�g��
		WS_OVERLAPPED,					// �E�B���h�E�̃X�^�C��
		0,								// �����Ƃ����Ƃ�
		0,								// �\�����Ȃ��̂łS�Ƃ��O
		0,								// 
		0,								// 
		NULL,							// �e�E�B���h�E�̃n���h��
		NULL);							// �q�E�B���h�E�⃁�j���[�̎��ʎq
}

//---------------------------------------------------------
// �E�B���h�E�̃f���[�g�˗�
//---------------------------------------------------------
void CTaskTrayApp::PostNcDestroy()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B

	CWnd::PostNcDestroy();

	delete this;
}

//---------------------------------------------------------
// �^�X�N�g���C�ւ̃A�C�R���̒ǉ��A�폜�A�ύX���s���B
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
	{	// �A�C�R���ύX
		nid.uFlags |= NIF_ICON;
		nid.hIcon = hIcon;
		m_hIcon_current = hIcon;
	}

	if (NULL != pszTip)
	{	// �c�[���`�b�v�\��
		nid.uFlags |= NIF_TIP;
		_tcsncpy_s(nid.szTip, sizeof(nid.szTip), pszTip, _TRUNCATE);
	}

	return Shell_NotifyIcon(dwMessage, &nid);
}

//---------------------------------------------------------
// ���b�Z�[�W�}�b�v
//---------------------------------------------------------
BEGIN_MESSAGE_MAP(CTaskTrayApp, CWnd)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONUP()
	ON_WM_TIMER()
	ON_MESSAGE(WM_USER_TRAYNOTIFYICON, &CTaskTrayApp::OnUserTraynotifyicon)
END_MESSAGE_MAP()

//---------------------------------------------------------
// �}�E�X�̍��{�^���A�b�v�ł̃C�x���g�n���h��
//---------------------------------------------------------
void CTaskTrayApp::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	DoLButtonClick();

	//	CWnd::OnLButtonUp(nFlags, point);
}

//---------------------------------------------------------
// �}�E�X�̍��{�^���̃_�u���N���b�N�ł̃C�x���g�n���h��
//---------------------------------------------------------
void CTaskTrayApp::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	DoLButtonDoubleClick();

	//	CWnd::OnLButtonDblClk(nFlags, point);
}

//---------------------------------------------------------
// �}�E�X�̉E�{�^���A�b�v�ł̃C�x���g�n���h��
//---------------------------------------------------------
void CTaskTrayApp::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	DoRButtonClick();

	//	CWnd::OnRButtonUp(nFlags, point);
}

//---------------------------------------------------------
// �^�C�}�C�x���g�̃n���h��
//---------------------------------------------------------
void CTaskTrayApp::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	
	// OnUserTraynotifyicon()�ŃZ�b�g�����^�C�}���o�߂�����
	// �^�C�}�Z�b�g�����Ƃ��̃{�^���_�E�����_�u���N���b�N�łȂ��Ɣ��f���ă��b�Z�[�W�𓊂���B
	if (ID_CLICKTIMER == nIDEvent)
	{
		KillTimer(nIDEvent);
		PostMessage(WM_LBUTTONUP);
	}

//	CWnd::OnTimer(nIDEvent);
}

//---------------------------------------------------------
// �^�X�N�g���C�̃A�C�R�����N���b�N�������ɔ�������C�x���g
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
