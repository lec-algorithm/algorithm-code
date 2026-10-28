"""접미사 배열 — 문자열의 모든 접미사를 사전순으로 정렬한 시작 위치의 배열.

단순한 방법(접미사 문자열을 그대로 정렬)과 더블링(앞 2, 4, 8, ...글자의
순위를 두 배씩 늘려 가며 정렬) 두 가지로 만들고 결과가 같은지 본다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 suffix_array.py`
"""


def build_suffix_array_naive(text):
    """접미사 문자열끼리 비교해 정렬한다."""
    return sorted(range(len(text)), key=lambda i: text[i:])


def build_suffix_array_doubling(text):
    """앞 k글자의 순위를 k = 2, 4, 8, ...로 늘려 가며 정렬한다. 라운드마다 표를 찍는다."""
    n = len(text)
    rank = [ord(c) for c in text]
    sa = list(range(n))

    k = 2
    while True:
        half = k // 2
        key = [(rank[i], rank[i + half] if i + half < n else -1) for i in range(n)]
        # 짝 순. 같으면 시작 위치 순으로 두어 C와 Python의 순서를 맞춘다.
        sa.sort(key=lambda i: (key[i], i))

        new_rank = [0] * n
        for j in range(1, n):
            same = key[sa[j]] == key[sa[j - 1]]
            new_rank[sa[j]] = new_rank[sa[j - 1]] + (0 if same else 1)
        rank = new_rank

        print(f"k = {k}")
        print("   j  SA[j]  key         rank  suffix")
        for j, s in enumerate(sa):
            k0, k1 = key[s]
            print(f"  {j:2d}  {s:5d}  ({k0:3d}, {k1:3d})  {rank[s]:4d}  {text[s:]}")
        distinct = rank[sa[n - 1]] + 1
        print(f"  distinct ranks = {distinct} of {n}")

        if distinct == n:
            break
        k *= 2
    return sa


if __name__ == "__main__":
    text = "banana$"
    n = len(text)

    print(f"text = {text} (n = {n})")
    for i in range(n):
        print(f"  {i} {text[i:]}")
    print()

    naive = build_suffix_array_naive(text)
    print("naive    SA =", *naive)
    print()

    doubling = build_suffix_array_doubling(text)
    print()
    print("doubling SA =", *doubling)
    print("same =", "yes" if naive == doubling else "no")
