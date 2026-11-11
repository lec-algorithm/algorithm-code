"""접미사 배열 — 더블링의 짝을 정렬 함수 대신 LSD 기수 정렬 두 패스로 정렬한다.

짝 (순위, k/2칸 뒤의 순위)을 두 자리 수로 보고 뒷자리, 앞자리 순으로 안정 카운팅 정렬한다.
결과가 접미사를 그대로 정렬한 것과 같은지 확인한다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 suffix_radix.py`
"""

MAX_VALUE = 258   # 값의 범위: -1을 0으로 민 문자 코드 또는 순위


def counting_pass(sa, key):
    """sa를 key[sa[j]]로 안정 카운팅 정렬한다. key는 0 이상이다."""
    count = [0] * (MAX_VALUE + 1)
    for s in sa:
        count[key[s]] += 1
    for v in range(1, MAX_VALUE + 1):
        count[v] += count[v - 1]
    output = [0] * len(sa)
    for s in reversed(sa):
        count[key[s]] -= 1
        output[count[key[s]]] = s
    return output


def build_suffix_array_radix(text):
    """라운드마다 k와 서로 다른 순위의 수를 찍는다. 접미사 배열과 패스 수를 돌려준다."""
    n = len(text)
    rank = [ord(c) for c in text]
    passes = 0
    k = 2
    while True:
        half = k // 2
        key0 = [r + 1 for r in rank]
        key1 = [rank[i + half] + 1 if i + half < n else 0 for i in range(n)]
        sa = list(range(n))
        sa = counting_pass(sa, key1)   # 뒷자리
        sa = counting_pass(sa, key0)   # 앞자리
        passes += 2

        new_rank = [0] * n
        for j in range(1, n):
            same = key0[sa[j]] == key0[sa[j - 1]] and key1[sa[j]] == key1[sa[j - 1]]
            new_rank[sa[j]] = new_rank[sa[j - 1]] + (0 if same else 1)
        rank = new_rank

        distinct = rank[sa[n - 1]] + 1
        print(f"  k = {k:<2d} distinct ranks = {distinct:2d} of {n}")
        if distinct == n:
            return sa, passes
        k *= 2


def check(text):
    n = len(text)
    print(f"{text} (n = {n})")
    sa, passes = build_suffix_array_radix(text)
    naive = sorted(range(n), key=lambda i: text[i:])
    print("  SA = " + " ".join(map(str, sa)))
    print(f"  passes = {passes}, same as plain sort = {'yes' if sa == naive else 'no'}")


if __name__ == "__main__":
    check("banana$")
    check("mississippi$")
    check("aaaaaaaa$")
