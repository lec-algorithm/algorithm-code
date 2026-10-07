"""분할 배낭 — 용량이 정해진 배낭에 가치를 최대로 담는다.

물건을 쪼갤 수 있다. 단위 무게당 가치가 높은 물건부터 담는다 (그리디).
쪼갤 수 없게 바꾸면 같은 순서가 최적을 놓치는 것도 함께 본다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 fractional_knapsack.py`
"""


def by_ratio(item):
    """단위 무게당 가치 내림차순으로 정렬하는 키. 각 물건은 (무게, 가치)다."""
    weight, value = item
    return -value / weight


def fractional_knapsack(items, capacity):
    """items를 단위 무게당 가치로 정렬하고 (가치의 합, 담은 비율 목록)을 돌려준다."""
    items.sort(key=by_ratio)

    total = 0.0
    taken = [0.0] * len(items)
    for i, (weight, value) in enumerate(items):
        if capacity == 0:
            break
        if weight <= capacity:
            taken[i] = 1.0
            total += value
            capacity -= weight
        else:
            taken[i] = capacity / weight
            total += value * taken[i]
            capacity = 0
    return total, taken


def zero_one_greedy(items, capacity):
    """같은 순서로 담되 쪼개지 않는다 (0/1 배낭에 그리디를 쓴 경우)."""
    items.sort(key=by_ratio)

    total = 0
    for weight, value in items:
        if weight <= capacity:
            total += value
            capacity -= weight
    return total


def run(items, capacity):
    total, taken = fractional_knapsack(items, capacity)
    print(f"capacity = {capacity}")
    for (weight, value), part in zip(items, taken):
        if part > 0:
            print(f"  take (w={weight}, v={value}, v/w={value / weight:.1f}) x {part:.2f}")
    print(f"  total value = {total:.2f}")


if __name__ == "__main__":
    items = [(20, 100), (30, 120), (10, 60)]

    run(items, 15)
    print()
    run(items, 50)
    print("\n0/1 greedy, capacity = 50")
    print(f"  total value = {zero_one_greedy(items, 50)}")
