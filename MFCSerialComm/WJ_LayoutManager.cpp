#include "pch.h"
#include "WJ_LayoutManager.h"

WJ_LayoutManager::WJ_LayoutManager(CWnd *ap_parent_wnd)
{
	// �׷� ������ ������ �޸𸮸� �����Ѵ�.
	mp_group = new LayoutGroupData[MAX_GROUP_COUNT];
	// ��ϵ� �׷��� ������ �ʱ�ȭ �Ѵ�.
	m_group_count = 0;
	// Layout ������ ����� ������ ��ü�� �ּҸ� �����Ѵ�.
	mp_parent_wnd = ap_parent_wnd;
}

WJ_LayoutManager::~WJ_LayoutManager()
{
	// �׷� ������ �����ϴ� �޸𸮸� �����Ѵ�.
	delete[] mp_group;
}

// �׷��� �߰��ϴ� �Լ�
LayoutGroupData *WJ_LayoutManager::AddGroup(HWND ah_wnd)
{
	// �׷� ����� �������� üũ�Ѵ�.
	if (m_group_count < MAX_GROUP_COUNT) {
		// ����� ��ġ�� �ּҸ� ��´�.
		LayoutGroupData *p_group = mp_group + m_group_count;
		// �׷��� ���� ������ �ڵ��� �����Ѵ�.
		p_group->h_wnd = ah_wnd;
		// �׷쿡 ���� �������� ������ �ʱ�ȭ�Ѵ�.
		p_group->item_count = 0;
		// ���� �����찡 Layout ������ ����ϴ� ���������� üũ�ؼ�
		// ������ ������� Ŭ���̾�Ʈ ������ �� ����ϰ�
		// Layout ������ ����ϴ� �������� �ڽ� �������� 
		// �ش� �ڽ� �������� ��ǥ�� �� ����Ѵ�.
		if (ah_wnd == mp_parent_wnd->m_hWnd) {
			// Layout �����찡 �׷��� ������ �� ���!
			::GetClientRect(ah_wnd, &p_group->pos);
		}
		else {
			// Layout �������� �ڽ� �����찡 �׷��� ������ �� ���!
			::GetWindowRect(ah_wnd, &p_group->pos);
			mp_parent_wnd->ScreenToClient(&p_group->pos);
		}
		// �׷��� �߰��Ѵ�.
		m_group_count++;
		// ����� �׷��� �ּҸ� ��ȯ�Ѵ�.
		return p_group;
	}
	// �׷� ��Ͽ� ������ ���!
	return NULL;
}

// ������ �ڵ�� �׷��� ã�� �Լ�
LayoutGroupData *WJ_LayoutManager::FindGroupData(HWND ah_wnd)
{
	LayoutGroupData *p = mp_group, *p_limit = mp_group + m_group_count;
	// �׷��� ���� ������� �Ű� ������ ���޵� �����찡 ��ġ�ϴ��� üũ�ؼ�
	// ��ġ�ϴ� �׷��� �ּҸ� ��ȯ�Ѵ�.
	while (p < p_limit) {
		if (p->h_wnd == ah_wnd) return p;  // ��ġ�ϴ� �׷��� ã��!
		p++; // ���� �׷����� �̵�
	}
	// ��ġ�ϴ� �׷��� ���� ���!
	return NULL;
}

// Ư�� �׷쿡 �׸��� �߰��ϴ� �Լ�
int WJ_LayoutManager::AddItem(LayoutGroupData *ap_group, HWND ah_wnd, UINT8 a_x_type, UINT8 a_y_type, UINT8 a_cx_type, UINT8 a_cy_type)
{
	// �׷쿡 �׸��� �߰��� �� �ִ��� üũ�Ѵ�.
	if (ap_group->item_count < MAX_ITEM_COUNT) {
		// �׸��� �߰��� �޸��� �ּҸ� ��´�.
		LayoutItemData *p_item = ap_group->item + ap_group->item_count;
		// �׸��� ������ �ڵ��� �����Ѵ�.
		p_item->h_wnd = ah_wnd;

		RECT r;
		// �׸��� ��ǥ�� ��´�.
		::GetWindowRect(ah_wnd, &r);
		// ���� ��ǥ�� Layout ������ �������� ��ȯ�Ѵ�.
		mp_parent_wnd->ScreenToClient(&r);

		if (a_x_type == 1) {  // x ��ǥ�� left �������� �̵� ���!
			p_item->relative_pos.left = ap_group->pos.left - r.left;
			p_item->relative_pos.right = ap_group->pos.left - r.right;
		}
		else if (a_x_type == 2) {  // x ��ǥ�� right �������� �̵� ���!
			p_item->relative_pos.left = ap_group->pos.right - r.left;
			p_item->relative_pos.right = ap_group->pos.right - r.right;
		}
		else { // �̵��� ���� �ʴ� ���
			if (a_cx_type == 2) {  // x�� ũ�� ������ �ְ�, right�� �������� �ϴ� ���!
				p_item->relative_pos.left = r.left;
				p_item->relative_pos.right = ap_group->pos.right - r.right;
			}
			else {  // �̵� �Ǵ� ũ�� ������ ���� ���!
				p_item->relative_pos.left = r.left;
				p_item->relative_pos.right = r.right;
			}
		}

		if (a_y_type == 1) {  // y ��ǥ�� top �������� �̵� ���!
			p_item->relative_pos.top = ap_group->pos.top - r.top;
			p_item->relative_pos.bottom = ap_group->pos.top - r.bottom;
		}
		else if (a_y_type == 2) {  // y ��ǥ�� bottom �������� �̵� ���!
			p_item->relative_pos.top = ap_group->pos.bottom - r.top;
			p_item->relative_pos.bottom = ap_group->pos.bottom - r.bottom;
		}
		else {  // �̵��� ���� �ʴ� ���
			if (a_cy_type == 2) {  // y�� ũ�� ������ �ְ�, bottom �������� �ϴ� ���!
				p_item->relative_pos.top = r.top;
				p_item->relative_pos.bottom = ap_group->pos.bottom - r.bottom;
			}
			else {  // �̵� �Ǵ� ũ�� ������ ���� ���!
				p_item->relative_pos.top = r.top;
				p_item->relative_pos.bottom = r.bottom;
			}
		}

		// �̵� �Ǵ� ũ�� ��ȭ ���¸� �����Ѵ�.
		p_item->x_move_type = a_x_type;
		p_item->y_move_type = a_y_type;
		p_item->cx_resize_type = a_cx_type;
		p_item->cy_resize_type = a_cy_type;
		// �׷쿡 �߰��� �׸� ���� ������Ų��.
		ap_group->item_count++;
		return 1;  // ���������� �߰�!
	}
	return 0;  // �׷쿡 ������ �߰� ����!
}

// �׷��� ������ �Ǵ� �������� ��ǥ�� ����ؼ� Layout�� �����ϴ� �Լ�
void WJ_LayoutManager::UpdateGroupPos(HWND ah_wnd)
{
	// ������ �ڵ鰪�� �������� �׷� ������ ��´�.
	LayoutGroupData *p_group = FindGroupData(ah_wnd);
	if (p_group) {
		// ���� �����찡 Layout ������ ����ϴ� ���������� üũ�ؼ�
		// ������ ������� Ŭ���̾�Ʈ ������ �� ����ϰ�
		// Layout ������ ����ϴ� �������� �ڽ� �������� 
		// �ش� �ڽ� �������� ��ǥ�� �� ����Ѵ�.
		if (ah_wnd == mp_parent_wnd->m_hWnd) {
			// Layout ������!
			::GetClientRect(ah_wnd, &p_group->pos);
		}
		else {
			// Layout �������� �ڽ� ������!
			::GetWindowRect(ah_wnd, &p_group->pos);
			mp_parent_wnd->ScreenToClient(&p_group->pos);
		}

		RECT *p_rect = &p_group->pos, *p_r_pos, r;
		LayoutItemData *p = p_group->item, *p_limit = p_group->item + p_group->item_count;
		// �׷쿡 ���Ե� ��� �׸��� Ž���ϸ鼭 ��ǥ�� �����Ѵ�.
		while (p < p_limit) {
			p_r_pos = &p->relative_pos;
			if (p->x_move_type == 1) { // x ��ǥ�� left �������� �̵� ���!
				r.left = p_rect->left - p_r_pos->left;
				r.right = p_rect->left - p_r_pos->right;
			}
			else if (p->x_move_type == 2) { // x ��ǥ�� right �������� �̵� ���!
				r.left = p_rect->right - p_r_pos->left;
				r.right = p_rect->right - p_r_pos->right;
			}
			else { // �̵��� ���� �ʴ� ���
				if (p->cx_resize_type == 2) { // x�� ũ�� ������ �ְ�, right�� �������� �ϴ� ���!
					r.left = p_r_pos->left;
					r.right = p_rect->right - p_r_pos->right;
				}
				else { // �̵� �Ǵ� ũ�� ������ ���� ���!
					r.left = p_r_pos->left;
					r.right = p_r_pos->right;
				}
			}

			if (p->y_move_type == 1) {  // y ��ǥ�� top �������� �̵� ���!
				r.top = p_rect->top - p_r_pos->top;
				r.bottom = p_rect->top - p_r_pos->bottom;
			}
			else if (p->y_move_type == 2) {  // y ��ǥ�� bottom �������� �̵� ���!
				r.top = p_rect->bottom - p_r_pos->top;
				r.bottom = p_rect->bottom - p_r_pos->bottom;
			}
			else {  // �̵��� ���� �ʴ� ���
				if (p->cy_resize_type == 2) {  // y�� ũ�� ������ �ְ�, bottom �������� �ϴ� ���!
					r.top = p_r_pos->top;
					r.bottom = p_rect->bottom - p_r_pos->bottom;
				}
				else {  // �̵� �Ǵ� ũ�� ������ ���� ���!
					r.top = p_r_pos->top;
					r.bottom = p_r_pos->bottom;
				}
			}

			// ���� ��ǥ�� ����ؼ� ������ �����츦 �̵���Ų��.
			::MoveWindow(p->h_wnd, r.left, r.top, r.right - r.left, r.bottom - r.top, 1);
			// �ش� �������� ���α����� �ٽ� �׸��⸦ �Ѵ�.
			::InvalidateRect(p->h_wnd, NULL, 1);
			// ���� ���������� �̵��Ѵ�.
			p++;
		}
	}
}
