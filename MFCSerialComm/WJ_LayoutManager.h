#ifndef __WJ_LAYOUT_MANAGER_H__
#define __WJ_LAYOUT_MANAGER_H__

#pragma once

#define MAX_GROUP_COUNT  8     // 사용 가능한 그룹 최대 개수
#define MAX_ITEM_COUNT   20    // 그룹에 포함된 아이템의 최대 개수

// 그룹에 포함된 아이템의 정보
struct LayoutItemData
{
	HWND h_wnd;           // 상대적 위치를 적용할 윈도우의 핸들값
	RECT relative_pos;    // 기준 윈도우의 시작점 또는 끝점을 기준으로 계산한 상대적 거리
	UINT8 x_move_type;    // 0: 이동 없음, 1: left 기준, 2: right기준
	UINT8 y_move_type;    // 0: 이동 없음, 1: top 기준, 2: bottom기준
	UINT8 cx_resize_type; // 0: resize 없음, 1: left방향, 2: right방향, 3: 양쪽 방향
	UINT8 cy_resize_type; // 0: resize 없음, 1: top방향, 2: bottom방향, 3: 양쪽 방향
};

// 그룹 항목의 정보
struct LayoutGroupData
{
	HWND h_wnd; // 기준 윈도우의 핸들값
	RECT pos;   // 기준 윈도우의 좌표
	int item_count;  // 그룹에 포함된 아이템의 수
	LayoutItemData item[MAX_ITEM_COUNT];  // 아이템의 정보
};

class WJ_LayoutManager
{
protected:
	// 그룹 정보가 저장된 메모리를 가리킬 포인터
	LayoutGroupData *mp_group;
	// 실제로 등록된 그룹의 개수를 저장할 변수
	int m_group_count;
	// Layout이 사용될 윈도우 객체의 주소
	CWnd *mp_parent_wnd;

public:
	WJ_LayoutManager(CWnd *ap_parent_wnd);  // 생성자
	virtual ~WJ_LayoutManager();  // 파괴자

	// 그룹을 추가하는 함수
	LayoutGroupData *AddGroup(HWND ah_wnd);
	// 윈도우 핸들로 그룹을 찾는 함수
	LayoutGroupData *FindGroupData(HWND ah_wnd);
	// 특정 그룹에 항목을 추가하는 함수
	int AddItem(LayoutGroupData *ap_group, HWND ah_wnd, UINT8 a_x_type,
		UINT8 a_y_type, UINT8 a_cx_type, UINT8 a_cy_type);
	// 그룹의 기준이 되는 윈도우의 좌표를 사용해서 Layout을 수정하는 함수
	void UpdateGroupPos(HWND ah_wnd);
};

#endif