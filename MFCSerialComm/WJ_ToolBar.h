#pragma once


// WJ_ToolBar

// ���ٿ� ��밡���� �ִ� ��ư�� ��
#define MAX_COMMAND_COUNT			24

#define TOOL_BAR_BK_COLOR			RGB(15, 55, 100) 
#define TOOL_BAR_OUTER_BODER_COLOR	RGB(0, 255, 255) 
#define TOOL_BAR_INNER_BODER_COLOR	RGB(0, 255, 255) 

#define OUTER_TEXT_COLOR			RGB(15, 55, 100)   
#define INNER_TEXT_COLOR			RGB(255, 255, 255) 

#define MOVE_OUTER_BODER_COLOR		RGB(0, 128, 64)  
#define MOVE_INNER_BODER_COLOR		RGB(128, 255, 0) 

// ���ٿ� ���� ��ư�� ������ ������ ����ü
struct ToolBar_CommandData {
	wchar_t *p_name;  // ���� ��ư�� ���� �̸�
	int name_len;  // �̸� ���ڿ��� ���� (NULL ���� ���� ����)
	// ���� ��ư�� �߻������� �߻��� ��ɹ�ȣ
	// ( WM_COMMAND �޽��� ���, WPARAM �� command_id �� ����Ǿ� ���۵�)
	int command_id;
	int x;  // ��ư�� ���� ��ġ
	int width;  // ��ư�� ��
};

class WJ_ToolBar : public CWnd
{
	DECLARE_DYNAMIC(WJ_ToolBar)
private:
	CRect m_rect;  // ������ ���� ��ǥ
	CFont m_tool_font;  // ���ٿ��� ����� �۲�
	ToolBar_CommandData m_btn_list[MAX_COMMAND_COUNT];   // ������ ��ư ����
	// ���ٿ� �߰��� ��ư�� ������ ���õ� ��ư�� ��ġ
	int m_btn_count = 0, m_select_index = -1;

public:
	WJ_ToolBar();
	virtual ~WJ_ToolBar();

	// ���ٸ� �����ϴ� �Լ�
	void CreateToolBar(int a_rect_id, CWnd *ap_parent, int a_ctrl_id);
	// ���ٿ� ��ư�� �߰��ϴ� �Լ� (CreateToolBar �Լ��� ȣ���ϱ� ���� ����ؾ� ��!!)
	void AddButton(const wchar_t *ap_name, int a_command_id);
	// ��ϵ� ��ư ������ ����ؼ� ��ư�� ��ġ�� ������ �����Ѵ�.
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


