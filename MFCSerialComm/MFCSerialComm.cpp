
// MFCSerialComm.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "MFCSerialComm.h"
#include "MFCSerialCommDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCSerialCommApp

BEGIN_MESSAGE_MAP(CMFCSerialCommApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMFCSerialCommApp 생성

CMFCSerialCommApp::CMFCSerialCommApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CMFCSerialCommApp 개체입니다.

CMFCSerialCommApp theApp;


// CMFCSerialCommApp 초기화

BOOL CMFCSerialCommApp::InitInstance()
{
	CWinApp::InitInstance();

	CMFCSerialCommDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	
	return FALSE;
}

