
// MFCSerialCommDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFCSerialComm.h"
#include "MFCSerialCommDlg.h"
#include "afxdialogex.h"
 
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCSerialCommDlg 대화 상자



CMFCSerialCommDlg::CMFCSerialCommDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCSERIALCOMM_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCSerialCommDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERIAL_PORT_LIST, m_serial_port_list);
}

BEGIN_MESSAGE_MAP(CMFCSerialCommDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMFCSerialCommDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMFCSerialCommDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CMFCSerialCommDlg 메시지 처리기

BOOL CMFCSerialCommDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	mh_program_state = CreateMutex(NULL, FALSE, L"프로젝트명");
	if (ERROR_ALREADY_EXISTS == GetLastError()) {
		MessageBox(L"이미 열려 있음.");
		DestroyWindow();
		return TRUE;
	}

	// 대화상자 배경색 변경
	SetBackgroundColor(RGB(128, 128, 128)); // 87, 111, 156

	m_tool_bar.AddButton(L"파일 열기", 28001);
	m_tool_bar.AddButton(L"파일 저장", 28002);
	m_tool_bar.AddButton(L"선택 내용 복사", 28003);
	m_tool_bar.AddButton(L"선택 내용 지우기", 28004);
	m_tool_bar.AddButton(L"프로그램 버전", 28005);
	m_tool_bar.AddButton(L"프로그램 종료", 28006);

	m_tool_bar.CreateToolBar(IDC_TOOL_BAR_RECT, this, 31000); //IDC_TOOL_BAR_RECT은 picture control의 ID 이름.

	OnDropdownComboPortName();
	
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCSerialCommDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCSerialCommDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

void CMFCSerialCommDlg::OnBnClickedOk()
{
	//CDialogEx::OnOK();
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

void CMFCSerialCommDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

BOOL CMFCSerialCommDlg::DestroyWindow()
{

	return CDialogEx::DestroyWindow();
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

BOOL CMFCSerialCommDlg::PreTranslateMessage(MSG *pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_ESCAPE)
		{
			return true; // true면 동작을 안먹게 함. false면 동작을 먹게 함.
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

// SerialDlg 메시지 처리기입니다.
void CMFCSerialCommDlg::OnDropdownComboPortName()
{
	HKEY  hSerialCom;
	TCHAR buffer[_MAX_PATH];
	char data[_MAX_PATH];
	DWORD len, type, dataSize;
	long  i;
	
	// http://littletrue.egloos.com/4294013
	if ( RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"HARDWARE\\DEVICEMAP\\SERIALCOMM", 0L, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hSerialCom)
		== ERROR_SUCCESS )
	{
		/*for (i = 0, len = dataSize = _MAX_PATH; 
			::RegEnumValue(hSerialCom,
				i,
				buffer,
				&len,
				NULL,
				&type,
				(unsigned char *)data,
				&dataSize) == ERROR_SUCCESS; i++, len = dataSize = _MAX_PATH)
		{
			data[dataSize - 1] = NULL;
			if (strncmp(data, "COM", 3) == 0)
				m_serial_port_list.AddString(CString(data));
		}*/
		
		m_serial_port_list.AddString(L"com1");
		m_serial_port_list.AddString(L"com2");
		m_serial_port_list.SetCurSel(0);

		// 오픈한 키를 닫는다.
		RegCloseKey(hSerialCom);
	} 
	else {
		MessageBox(L"키 열기 실패");
	}

}

// https://poorman.tistory.com/41
//void SerialDlg::OnDropdownComboPortName()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	HKEY hKey;
//	CString lpSubKey = _T("HARDWARE\\DEVICEMAP\\SERIALCOMM");
//	//HKEY_LOCAL_MACHINE\HARDWARE\DEVICEMAP\SERIALCOMM에 시리얼포트 번호들이 등록되어 있음.  
//	RegOpenKey(HKEY_LOCAL_MACHINE, lpSubKey, &hKey);
//	TCHAR szData[100], szName[100];
//	DWORD index = 0, dwSize = 100, dwSize2 = 100, dwType = REG_SZ;
//	memset(szData, 0x00, sizeof(szData));
//	memset(szName, 0x00, sizeof(szName));
//
//	// reset content of combobox
//	m_combo_port_num.ResetContent();
//
//	//hKey - 레지스터키 핸들  
//	//index - 값을 가져올 인덱스.. 다수의 값이 있을 경우 필요  
//	//szName - 항목값이 저장될 배열  
//	//dwSize - 배열의 크기  
//	while (ERROR_SUCCESS == RegEnumValue(hKey, index, szName, &dwSize, NULL, NULL, NULL, NULL))
//	{
//		index++;
//
//		//szName-레지터스터 항목의 이름  
//		//dwType-항목의 타입, 여기에서는 널로 끝나는 문자열  
//		//szData-항목값이 저장될 배열  
//		//dwSize2-배열의 크기
//		RegQueryValueEx(hKey, szName, NULL, &dwType, (LPBYTE)szData, &dwSize2);
//
//		//Port Number
//		m_iSerialPort = pDisplayUnitTesterDlg->toIdxComPort(CString(szData));
//		// 포트가 닫혀 있을 경우에만 포트를 열기 위해
//		if (m_ComuPort.m_bConnected == FALSE)
//		{
//			if (m_ComuPort.OpenPort(pDisplayUnitTesterDlg->byIndexComPort(m_iSerialPort), pDisplayUnitTesterDlg->byIndexBaud(m_iBaudRate), pDisplayUnitTesterDlg->byIndexData(m_iDataBit), pDisplayUnitTesterDlg->byIndexStop(m_iStopBit), pDisplayUnitTesterDlg->byIndexParity(m_iParity)) == TRUE)
//			{
//				// 포트를 여는데 성공했다면
//				if (m_ComuPort.m_bConnected == TRUE)
//				{
//					m_combo_port_num.AddString(CString(szData));
//					m_ComuPort.ClosePort();
//				}
//			}
//		}
//
//
//		memset(szData, 0x00, sizeof(szData));
//		memset(szName, 0x00, sizeof(szName));
//		dwSize = 100;
//		dwSize2 = 100;
//	}
//	RegCloseKey(hKey);
//	m_combo_port_num.SetCurSel(0);	//combo box select content
//}
