"""최장 증가 부분수열(LIS) — 메모이제이션과 타뷸레이션.

메모는 원소마다 "고른다 / 안 고른다"로 갈라지는 재귀에 (i, prev)
표를 붙인다. 표가 실제로 몇 번 쓰이는지 찍어 본다. 타뷸레이션은
dp[i] = arr[i]로 끝나는 가장 긴 길이를 채우고, prev를 거꾸로 따라가
수열 하나를 복원한다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 lis.py`
"""

use_memo = False  # False이면 표 없이 순수 재귀
calls = 0  # lis_helper가 불린 횟수
hits = 0  # 표에서 바로 꺼낸 횟수


def lis_helper(arr, i, prev, memo):
    """arr[i:]에서 더 이을 수 있는 최대 길이. prev는 마지막으로 고른 원소"""
    global calls, hits
    calls += 1
    # 1. 기저
    if i == len(arr):
        return 0
    # 2. 메모 확인
    if use_memo and memo[i][prev + 1] != -1:
        hits += 1
        print(f"  hit memo[{i}][{prev + 1}] = {memo[i][prev + 1]}")
        return memo[i][prev + 1]
    # 3. 고르지 않는 경우와 고르는 경우
    exclude = lis_helper(arr, i + 1, prev, memo)
    include = 0
    if prev == -1 or arr[i] > arr[prev]:
        include = 1 + lis_helper(arr, i + 1, i, memo)
    # 4. 둘 중 큰 것을 적어 둔다
    best = max(include, exclude)
    if use_memo:
        memo[i][prev + 1] = best
    return best


def lis_memo(arr):
    """(길이, memo 표)를 돌려준다."""
    n = len(arr)
    memo = [[-1] * (n + 1) for _ in range(n)]
    return lis_helper(arr, 0, -1, memo), memo


def lis_tab(arr):
    """(끝 인덱스, dp, prev)를 돌려준다.

    dp[i]는 arr[i]로 끝나는 가장 긴 증가 부분수열의 길이다.
    """
    n = len(arr)
    # 1. 초기화
    dp = [1] * n
    prev = [-1] * n
    # 2. 앞의 원소를 모두 훑는다
    for i in range(1, n):
        for j in range(i):
            if arr[j] < arr[i] and dp[j] + 1 > dp[i]:
                dp[i] = dp[j] + 1
                prev[i] = j
    # 3. 최댓값 찾기 (같으면 앞의 것)
    end = 0
    for i in range(1, n):
        if dp[i] > dp[end]:
            end = i
    return end, dp, prev


def show(label, values):
    print(f"{label}[{', '.join(map(str, values))}]")


if __name__ == "__main__":
    arr = [10, 9, 2, 5, 3, 7, 101, 18]
    n = len(arr)
    show("arr = ", arr)
    print()

    use_memo = False
    calls = 0
    plain = lis_helper(arr, 0, -1, None)
    print(f"plain recursion = {plain}, calls = {calls}")

    print("memo hits (i, prev + 1)")
    use_memo = True
    calls = 0
    hits = 0
    m, memo = lis_memo(arr)
    filled = sum(1 for row in memo for v in row if v != -1)
    print(f"memo = {m}, calls = {calls}, hits = {hits}, filled {filled} of {n * (n + 1)}")
    print()

    end, dp, prev = lis_tab(arr)
    print(f"tab  = {dp[end]}")
    show("  dp   = ", dp)
    show("  prev = ", prev)
    # 4. 경로 복원: prev를 거꾸로 따라가며 모은다
    seq = []
    k = end
    while k != -1:
        seq.append(arr[k])
        k = prev[k]
    print("  one LIS: " + " ".join(map(str, reversed(seq))))
