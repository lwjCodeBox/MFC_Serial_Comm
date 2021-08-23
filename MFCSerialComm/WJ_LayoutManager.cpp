#include "pch.h"
#include "WJ_LayoutManager.h"

WJ_LayoutManager::WJ_LayoutManager(CWnd *ap_parent_wnd)
{
	// 그룹 정보를 저장할 메모리를 생성한다.
	mp_group = new LayoutGroupData[MAX_GROUP_COUNT];
	// 등록된 그룹의 개수를 초기화 한다.
	m_group_count = 0;
	// Layout 개념을 사용할 윈도우 객체의 주소를 저장한다.
	mp_parent_wnd = ap_parent_wnd;
}

WJ_LayoutManager::~WJ_LayoutManager()
{
	// 그룹 정보를 저장하던 메모리를 해제한다.
	delete[] mp_group;
}

// 그룹을 추가하는 함수
LayoutGroupData *WJ_LayoutManager::AddGroup(HWND ah_wnd)
{
	// 그룹 등록이 가능한지 체크한다.
	if (m_group_count < MAX_GROUP_COUNT) {
		// 등록할 위치의 주소를 얻는다.
		LayoutGroupData *p_group = mp_group + m_group_count;
		// 그룹의 기준 윈도우 핸들을 저장한다.
		p_group->h_wnd = ah_wnd;
		// 그룹에 속한 아이템의 개수를 초기화한다.
		p_group->item_count = 0;
		// 기준 윈도우가 Layout 개념을 사용하는 윈도우인지 체크해서
		// 동일한 윈도우면 클라이언트 영역을 얻어서 사용하고
		// Layout 개념을 사용하는 윈도우의 자식 윈도우라면 
		// 해당 자식 윈도우의 좌표를 얻어서 사용한다.
		if (ah_wnd == mp_parent_wnd->m_hWnd) {
			// Layout 윈도우가 그룹의 기준이 된 경우!
			::GetClientRect(ah_wnd, &p_group->pos);
		}
		else {
			// Layout 윈도우의 자식 윈도우가 그룹의 기준이 된 경우!
			::GetWindowRect(ah_wnd, &p_group->pos);
			mp_parent_wnd->ScreenToClient(&p_group->pos);
		}
		// 그룹을 추가한다.
		m_group_count++;
		// 저장된 그룹의 주소를 반환한다.
		return p_group;
	}
	// 그룹 등록에 실패한 경우!
	return NULL;
}

// 윈도우 핸들로 그룹을 찾는 함수
LayoutGroupData *WJ_LayoutManager::FindGroupData(HWND ah_wnd)
{
	LayoutGroupData *p = mp_group, *p_limit = mp_group + m_group_count;
	// 그룹의 기준 윈도우와 매개 변수로 전달된 윈도우가 일치하는지 체크해서
	// 일치하는 그룹의 주소를 반환한다.
	while (p < p_limit) {
		if (p->h_wnd == ah_wnd) return p;  // 일치하는 그룹을 찾음!
		p++; // 다음 그룹으로 이동
	}
	// 일치하는 그룹이 없는 경우!
	return NULL;
}

// 특정 그룹에 항목을 추가하는 함수
int WJ_LayoutManager::AddItem(LayoutGroupData *ap_group, HWND ah_wnd, UINT8 a_x_type, UINT8 a_y_type, UINT8 a_cx_type, UINT8 a_cy_type)
{
	// 그룹에 항목을 추가할 수 있는지 체크한다.
	if (ap_group->item_count < MAX_ITEM_COUNT) {
		// 항목을 추가할 메모리의 주소를 얻는다.
		LayoutItemData *p_item = ap_group->item + ap_group->item_count;
		// 항목의 윈도우 핸들을 저장한다.
		p_item->h_wnd = ah_wnd;

		RECT r;
		// 항목의 좌표를 얻는다.
		::GetWindowRect(ah_wnd, &r);
		// 얻은 좌표를 Layout 윈도우 기준으로 변환한다.
		mp_parent_wnd->ScreenToClient(&r);

		if (a_x_type == 1) {  // x 좌표를 left 기준으로 이동 사용!
			p_item->relative_pos.left = ap_group->pos.left - r.left;
			p_item->relative_pos.right = ap_group->pos.left - r.right;
		}
		else if (a_x_type == 2) {  // x 좌표를 right 기준으로 이동 사용!
			p_item->relative_pos.left = ap_group->pos.right - r.left;
			p_item->relative_pos.right = ap_group->pos.right - r.right;
		}
		else { // 이동을 하지 않는 경우
			if (a_cx_type == 2) {  // x축 크기 변경이 있고, right를 기준으로 하는 경우!
				p_item->relative_pos.left = r.left;
				p_item->relative_pos.right = ap_group->pos.right - r.right;
			}
			else {  // 이동 또는 크기 변경이 없는 경우!
				p_item->relative_pos.left = r.left;
				p_item->relative_pos.right = r.right;
			}
		}

		if (a_y_type == 1) {  // y 좌표를 top 기준으로 이동 사용!
			p_item->relative_pos.top = ap_group->pos.top - r.top;
			p_item->relative_pos.bottom = ap_group->pos.top - r.bottom;
		}
		else if (a_y_type == 2) {  // y 좌표를 bottom 기준으로 이동 사용!
			p_item->relative_pos.top = ap_group->pos.bottom - r.top;
			p_item->relative_pos.bottom = ap_group->pos.bottom - r.bottom;
		}
		else {  // 이동을 하지 않는 경우
			if (a_cy_type == 2) {  // y축 크기 변경이 있고, bottom 기준으로 하는 경우!
				p_item->relative_pos.top = r.top;
				p_item->relative_pos.bottom = ap_group->pos.bottom - r.bottom;
			}
			else {  // 이동 또는 크기 변경이 없는 경우!
				p_item->relative_pos.top = r.top;
				p_item->relative_pos.bottom = r.bottom;
			}
		}

		// 이동 또는 크기 변화 상태를 저장한다.
		p_item->x_move_type = a_x_type;
		p_item->y_move_type = a_y_type;
		p_item->cx_resize_type = a_cx_type;
		p_item->cy_resize_type = a_cy_type;
		// 그룹에 추가된 항목 수를 증가시킨다.
		ap_group->item_count++;
		return 1;  // 정상적으로 추가!
	}
	return 0;  // 그룹에 아이템 추가 실패!
}

// 그룹의 기준이 되는 윈도우의 좌표를 사용해서 Layout을 수정하는 함수
void WJ_LayoutManager::UpdateGroupPos(HWND ah_wnd)
{
	// 윈도우 핸들값을 기준으로 그룹 정보를 얻는다.
	LayoutGroupData *p_group = FindGroupData(ah_wnd);
	if (p_group) {
		// 기준 윈도우가 Layout 개념을 사용하는 윈도우인지 체크해서
		// 동일한 윈도우면 클라이언트 영역을 얻어서 사용하고
		// Layout 개념을 사용하는 윈도우의 자식 윈도우라면 
		// 해당 자식 윈도우의 좌표를 얻어서 사용한다.
		if (ah_wnd == mp_parent_wnd->m_hWnd) {
			// Layout 윈도우!
			::GetClientRect(ah_wnd, &p_group->pos);
		}
		else {
			// Layout 윈도우의 자식 윈도우!
			::GetWindowRect(ah_wnd, &p_group->pos);
			mp_parent_wnd->ScreenToClient(&p_group->pos);
		}

		RECT *p_rect = &p_group->pos, *p_r_pos, r;
		LayoutItemData *p = p_group->item, *p_limit = p_group->item + p_group->item_count;
		// 그룹에 포함된 모든 항목을 탐색하면서 좌표를 수정한다.
		while (p < p_limit) {
			p_r_pos = &p->relative_pos;
			if (p->x_move_type == 1) { // x 좌표를 left 기준으로 이동 사용!
				r.left = p_rect->left - p_r_pos->left;
				r.right = p_rect->left - p_r_pos->right;
			}
			else if (p->x_move_type == 2) { // x 좌표를 right 기준으로 이동 사용!
				r.left = p_rect->right - p_r_pos->left;
				r.right = p_rect->right - p_r_pos->right;
			}
			else { // 이동을 하지 않는 경우
				if (p->cx_resize_type == 2) { // x축 크기 변경이 있고, right를 기준으로 하는 경우!
					r.left = p_r_pos->left;
					r.right = p_rect->right - p_r_pos->right;
				}
				else { // 이동 또는 크기 변경이 없는 경우!
					r.left = p_r_pos->left;
					r.right = p_r_pos->right;
				}
			}

			if (p->y_move_type == 1) {  // y 좌표를 top 기준으로 이동 사용!
				r.top = p_rect->top - p_r_pos->top;
				r.bottom = p_rect->top - p_r_pos->bottom;
			}
			else if (p->y_move_type == 2) {  // y 좌표를 bottom 기준으로 이동 사용!
				r.top = p_rect->bottom - p_r_pos->top;
				r.bottom = p_rect->bottom - p_r_pos->bottom;
			}
			else {  // 이동을 하지 않는 경우
				if (p->cy_resize_type == 2) {  // y축 크기 변경이 있고, bottom 기준으로 하는 경우!
					r.top = p_r_pos->top;
					r.bottom = p_rect->bottom - p_r_pos->bottom;
				}
				else {  // 이동 또는 크기 변경이 없는 경우!
					r.top = p_r_pos->top;
					r.bottom = p_r_pos->bottom;
				}
			}

			// 계산된 좌표를 사용해서 아이템 윈도우를 이동시킨다.
			::MoveWindow(p->h_wnd, r.left, r.top, r.right - r.left, r.bottom - r.top, 1);
			// 해당 윈도우의 내부까지도 다시 그리기를 한다.
			::InvalidateRect(p->h_wnd, NULL, 1);
			// 다음 아이템으로 이동한다.
			p++;
		}
	}
}
