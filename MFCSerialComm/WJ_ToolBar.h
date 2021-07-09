#pragma once


// WJ_ToolBar

// 툴바에 사용가능한 최대 버튼의 수
#define MAX_COMMAND_COUNT			24

#define TOOL_BAR_BK_COLOR			RGB(15, 55, 100) 
#define TOOL_BAR_OUTER_BODER_COLOR	RGB(0, 255, 255) 
#define TOOL_BAR_INNER_BODER_COLOR	RGB(0, 255, 255) 

#define OUTER_TEXT_COLOR			RGB(15, 55, 100)   
#define INNER_TEXT_COLOR			RGB(255, 255, 255) 

#define MOVE_OUTER_BODER_COLOR		RGB(0, 128, 64)  
#define MOVE_INNER_BODER_COLOR		RGB(128, 255, 0) 

// 툴바에 사용된 버튼별 정보를 정의한 구조체
struct ToolBar_CommandData {
	wchar_t *p_name;  // 툴바 버튼에 사용될 이름
	int name_len;  // 이름 문자열의 길이 (NULL 문자 제외 길이)
	// 툴바 버튼을 발생했을때 발생한 명령번호
	// ( WM_COMMAND 메시지 사용, WPARAM 에 command_id 가 저장되어 전송됨)
	int command_id;
	int x;  // 버튼의 시작 위치
	int width;  // 버튼의 폭
};

class WJ_ToolBar : public CWnd
{
	DECLARE_DYNAMIC(WJ_ToolBar)
private:
	CRect m_rect;  // 툴바의 영역 좌표
	CFont m_tool_font;  // 툴바에서 사용할 글꼴
	ToolBar_CommandData m_btn_list[MAX_COMMAND_COUNT];   // 툴바의 버튼 정보
	// 툴바에 추가된 버튼의 갯수와 선택된 버튼의 위치
	int m_btn_count = 0, m_select_index = -1;

public:
	WJ_ToolBar();
	virtual ~WJ_ToolBar();

	// 툴바를 생성하는 함수
	void CreateToolBar(int a_rect_id, CWnd *ap_parent, int a_ctrl_id);
	// 툴바에 버튼을 추가하는 함수 (CreateToolBar 함수를 호출하기 전에 사용해야 함!!)
	void AddButton(const wchar_t *ap_name, int a_command_id);
	// 등록된 버튼 정보를 사용해서 버튼이 위치할 정보를 구성한다.
	void UpdateButtonInfo();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
};


