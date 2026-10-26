"""LCP 배열 (Kasai 알고리즘) — 정렬된 접미사 중 이웃한 둘의 최장 공통 접두사 길이.

텍스트 위치 순서로 접미사를 처리하면서, 앞 단계에서 센 길이 h를 하나만
줄여 이어 쓴다. 이웃 쌍마다 처음부터 비교하는 방법과 글자 비교 횟수를 견준다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 lcp.py`
"""


def build_suffix_array(text):
    """접미사 배열은 03_suffix_array에서 만들었다. 여기서는 단순한 방법으로 만든다."""
    return sorted(range(len(text)), key=lambda i: text[i:])


def build_lcp_kasai(text, sa):
    """Kasai 알고리즘. lcp[0]은 정의하지 않으므로 -1을 넣는다.

    (lcp, 글자 비교 횟수)를 돌려준다. 처리한 위치마다 한 줄씩 찍는다.
    """
    n = len(text)
    rank = [0] * n
    for j in range(n):
        rank[sa[j]] = j
    lcp = [0] * n
    lcp[0] = -1
    compares = 0

    print("   i  suffix    rank  j  start h  lcp  compares")
    h = 0
    for i in range(n):
        if rank[i] == 0:
            h = 0
            print(f"  {i:2d}  {text[i:]:<8s}  {rank[i]:4d}  -        -    -         -")
            continue
        j = sa[rank[i] - 1]
        start = h
        before = compares
        while i + h < n and j + h < n:
            compares += 1
            if text[i + h] != text[j + h]:
                break
            h += 1
        lcp[rank[i]] = h
        print(f"  {i:2d}  {text[i:]:<8s}  {rank[i]:4d}  {j}  {start:7d}  {h:3d}  {compares - before:8d}")
        if h > 0:
            h -= 1
    return lcp, compares


def count_naive_compares(text, sa):
    """비교용: 이웃 쌍마다 처음부터 센다. 글자 비교 횟수를 돌려준다."""
    n = len(text)
    compares = 0
    for j in range(1, n):
        a, b, h = sa[j - 1], sa[j], 0
        while a + h < n and b + h < n:
            compares += 1
            if text[a + h] != text[b + h]:
                break
            h += 1
    return compares


if __name__ == "__main__":
    text = "banana$"
    sa = build_suffix_array(text)
    print(f"text = {text}")
    print("SA   =", *sa)
    print()

    lcp, kasai = build_lcp_kasai(text, sa)
    print()

    print("   j  SA[j]  LCP[j]  suffix")
    for j, s in enumerate(sa):
        value = "-" if j == 0 else str(lcp[j])
        print(f"  {j:2d}  {s:5d}  {value:>6s}  {text[s:]}")
    print(f"char compares: kasai {kasai}, naive {count_naive_compares(text, sa)}")
