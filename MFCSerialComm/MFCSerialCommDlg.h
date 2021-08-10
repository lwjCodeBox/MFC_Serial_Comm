﻿
// MFCSerialCommDlg.h: 헤더 파일
//

#pragma once
#include "WJ_ToolBar.h"

// CMFCSerialCommDlg 대화 상자
class CMFCSerialCommDlg : public CDialogEx
{
private:
	HANDLE mh_program_state;
	WJ_ToolBar m_tool_bar;

public:
	CComboBox m_serial_port_list;

// 생성입니다.
public:
	CMFCSerialCommDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	
	void OnDropdownComboPortName();

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
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL PreTranslateMessage(MSG *pMsg);
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedSearch();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	afx_msg void OnFileSave();
};
