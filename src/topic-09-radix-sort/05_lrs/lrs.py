"""가장 긴 반복 부분 문자열 (LRS) — 한 문자열에 두 번 이상 나오는 가장 긴 부분 문자열.

접미사를 정렬하면 같은 부분 문자열로 시작하는 접미사들이 이웃하게 모인다.
그래서 LCP 배열의 최댓값이 답이다. 모든 위치 쌍을 비교하는 방법과 견준다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 lrs.py`
"""


def build_suffix_array(text):
    """접미사 배열 (03_suffix_array). 여기서는 단순한 방법으로 만든다."""
    return sorted(range(len(text)), key=lambda i: text[i:])


def build_lcp(text, sa):
    """Kasai 알고리즘 (04_lcp). lcp[0]은 -1."""
    n = len(text)
    rank = [0] * n
    for j in range(n):
        rank[sa[j]] = j
    lcp = [0] * n
    lcp[0] = -1
    h = 0
    for i in range(n):
        if rank[i] == 0:
            h = 0
            continue
        j = sa[rank[i] - 1]
        while i + h < n and j + h < n and text[i + h] == text[j + h]:
            h += 1
        lcp[rank[i]] = h
        if h > 0:
            h -= 1
    return lcp


def find_longest_repeated_substring(text, sa, lcp):
    """LCP의 최댓값과 그 자리 j(1 이상)를 돌려준다. 같은 최댓값이면 앞의 것."""
    best, at = -1, 0
    for j in range(1, len(text)):
        if lcp[j] > best:
            best, at = lcp[j], j
    return best, at


def lrs_brute_force(text):
    """모든 위치 쌍 (i, j)의 공통 접두사를 센다. (길이, 시작, 쌍 수, 글자 비교 수)."""
    n = len(text)
    best, start, pairs, compares = 0, 0, 0, 0
    for i in range(n):
        for j in range(i + 1, n):
            pairs += 1
            h = 0
            while j + h < n:
                compares += 1
                if text[i + h] != text[j + h]:
                    break
                h += 1
            if h > best:
                best, start = h, i
    return best, start, pairs, compares


def report(text):
    n = len(text)
    sa = build_suffix_array(text)
    lcp = build_lcp(text, sa)

    print(f"text = {text} (n = {n})")
    print("   j  SA[j]  LCP[j]  suffix")
    for j, s in enumerate(sa):
        value = "-" if j == 0 else str(lcp[j])
        print(f"  {j:2d}  {s:5d}  {value:>6s}  {text[s:]}")

    best, at = find_longest_repeated_substring(text, sa, lcp)
    found = text[sa[at]:sa[at] + best]
    print(f"suffix sort: max LCP = {best} at j = {at} -> \"{found}\""
          f" (positions {sa[at - 1]}, {sa[at]}), {n - 1} adjacent pairs")

    length, start, pairs, compares = lrs_brute_force(text)
    print(f"brute force: \"{text[start:start + length]}\", {pairs} pairs,"
          f" {compares} char compares")


if __name__ == "__main__":
    report("banana$")
    print()
    report("aacaagtttacaagc$")
