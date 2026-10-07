"""활동 선택 — 회의실 하나에 겹치지 않는 회의를 가장 많이 넣는다.

언제나 가장 일찍 끝나는 회의부터 고른다 (그리디).

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 activity_selection.py`
"""


def activity_selection(activities):
    """activities를 끝 시각으로 정렬하고, 고른 활동의 목록을 돌려준다.

    각 활동은 (시작 시각, 끝 시각)이다. 끝이 같으면 시작 시각 순이다.
    """
    activities.sort(key=lambda act: (act[1], act[0]))

    selected = [activities[0]]
    last_end = activities[0][1]
    for start, end in activities[1:]:
        if start >= last_end:
            selected.append((start, end))
            last_end = end
    return selected


def print_activities(label, activities):
    print(f"{label}:", *(f"({s},{e})" for s, e in activities))


if __name__ == "__main__":
    activities = [(5, 8), (1, 3), (8, 9), (2, 5), (6, 7), (4, 6)]

    print_activities("input   ", activities)
    selected = activity_selection(activities)
    print_activities("by end  ", activities)
    print_activities("selected", selected)
    print(f"count = {len(selected)}")
