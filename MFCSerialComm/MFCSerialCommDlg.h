
// MFCSerialCommDlg.h: 헤더 파일
//

#pragma once


// CMFCSerialCommDlg 대화 상자
class CMFCSerialCommDlg : public CDialogEx
{
private:
	HANDLE mh_program_state;
// 생성입니다.
public:
	CMFCSerialCommDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCSERIALCOMM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
