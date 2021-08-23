#ifndef __WJ_LAYOUT_MANAGER_H__
#define __WJ_LAYOUT_MANAGER_H__

#pragma once

#define MAX_GROUP_COUNT  8     // ��� ������ �׷� �ִ� ����
#define MAX_ITEM_COUNT   20    // �׷쿡 ���Ե� �������� �ִ� ����

// �׷쿡 ���Ե� �������� ����
struct LayoutItemData
{
	HWND h_wnd;           // ����� ��ġ�� ������ �������� �ڵ鰪
	RECT relative_pos;    // ���� �������� ������ �Ǵ� ������ �������� ����� ����� �Ÿ�
	UINT8 x_move_type;    // 0: �̵� ����, 1: left ����, 2: right����
	UINT8 y_move_type;    // 0: �̵� ����, 1: top ����, 2: bottom����
	UINT8 cx_resize_type; // 0: resize ����, 1: left����, 2: right����, 3: ���� ����
	UINT8 cy_resize_type; // 0: resize ����, 1: top����, 2: bottom����, 3: ���� ����
};

// �׷� �׸��� ����
struct LayoutGroupData
{
	HWND h_wnd; // ���� �������� �ڵ鰪
	RECT pos;   // ���� �������� ��ǥ
	int item_count;  // �׷쿡 ���Ե� �������� ��
	LayoutItemData item[MAX_ITEM_COUNT];  // �������� ����
};

class WJ_LayoutManager
{
protected:
	// �׷� ������ ����� �޸𸮸� ����ų ������
	LayoutGroupData *mp_group;
	// ������ ��ϵ� �׷��� ������ ������ ����
	int m_group_count;
	// Layout�� ���� ������ ��ü�� �ּ�
	CWnd *mp_parent_wnd;

public:
	WJ_LayoutManager(CWnd *ap_parent_wnd);  // ������
	virtual ~WJ_LayoutManager();  // �ı���

	// �׷��� �߰��ϴ� �Լ�
	LayoutGroupData *AddGroup(HWND ah_wnd);
	// ������ �ڵ�� �׷��� ã�� �Լ�
	LayoutGroupData *FindGroupData(HWND ah_wnd);
	// Ư�� �׷쿡 �׸��� �߰��ϴ� �Լ�
	int AddItem(LayoutGroupData *ap_group, HWND ah_wnd, UINT8 a_x_type,
		UINT8 a_y_type, UINT8 a_cx_type, UINT8 a_cy_type);
	// �׷��� ������ �Ǵ� �������� ��ǥ�� ����ؼ� Layout�� �����ϴ� �Լ�
	void UpdateGroupPos(HWND ah_wnd);
};

#endif