"""가장 긴 공통 부분 문자열 (LCS) — 두 문자열에 함께 나오는 가장 긴 부분 문자열.

두 문자열을 구분자 '#'로 이어 붙인 문자열 하나의 접미사 배열과 LCP 배열을
만든다. 이웃한 두 접미사가 **서로 다른 문자열에서 시작할 때만** LCP를
후보로 삼는다. 같은 문자열 안의 반복은 공통 부분 문자열이 아니다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 lcs.py`
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


def origin(pos, len1, n):
    """접미사가 어느 문자열에서 시작하는지: 1, 2. 구분자 '#'나 끝 '$'에서 시작하면 0."""
    if pos < len1:
        return 1
    if pos == len1 or pos == n - 1:
        return 0
    return 2


def find_longest_common_substring(s1, s2):
    """(LCS 문자열, 그 자리 j)를 돌려준다. 이웃 쌍마다 한 줄씩 찍는다."""
    text = s1 + "#" + s2 + "$"
    n, len1 = len(text), len(s1)
    sa = build_suffix_array(text)
    lcp = build_lcp(text, sa)

    print(f"text = {text} (n = {n}, s1 = 0..{len1 - 1}, s2 = {len1 + 1}..{n - 2})")
    print("   j  SA[j]  from  LCP[j]  suffix")
    best, at = 0, 0
    for j in range(n):
        f = origin(sa[j], len1, n)
        tag = "-" if f == 0 else str(f)
        if j == 0:
            print(f"  {j:2d}  {sa[j]:5d}  {tag:>4s}  {'-':>6s}  {text[sa[j]:]}")
            continue
        g = origin(sa[j - 1], len1, n)
        mark = ""
        if f != 0 and g != 0 and f != g:      # 서로 다른 문자열에서 시작한 이웃
            mark = "  *"
            if lcp[j] > best:
                best, at = lcp[j], j
        print(f"  {j:2d}  {sa[j]:5d}  {tag:>4s}  {lcp[j]:6d}  {text[sa[j]:]}{mark}")

    top = max(lcp[1:])
    print(f"max LCP over all pairs = {top}")
    return text[sa[at]:sa[at] + best], at


if __name__ == "__main__":
    s1, s2 = "mississippi", "kiss"
    print(f"s1 = {s1}, s2 = {s2}")
    answer, at = find_longest_common_substring(s1, s2)
    print(f"LCS = \"{answer}\" (length {len(answer)}) at j = {at}")
