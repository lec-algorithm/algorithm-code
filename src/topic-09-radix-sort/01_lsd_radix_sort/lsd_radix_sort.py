"""LSD 기수 정렬 — 가장 낮은 자리부터 한 자리씩 안정 카운팅 정렬을 반복한다.

정수(10진 자리)와 길이가 같은 문자열(문자 한 글자가 한 자리) 두 가지를 정렬한다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 lsd_radix_sort.py`
"""

RADIX = 10  # 10진수 한 자리의 값: 0~9
R = 256     # 문자 한 글자의 값: 0~255

width = 3   # 출력할 때 맞출 자릿수


def print_numbers(label, a):
    print(f"{label}:", *(f"{v:0{width}d}" for v in a))


def print_strings(label, s):
    print(f"{label}:", *s)


def counting_sort_by_digit(a, exp):
    """exp 자리(1, 10, 100, ...)의 숫자를 키로 a를 안정 카운팅 정렬한다."""
    n = len(a)
    output = [0] * n
    count = [0] * RADIX
    for v in a:                             # 1) 세기
        count[(v // exp) % RADIX] += 1
    for d in range(1, RADIX):               # 2) 누적
        count[d] += count[d - 1]
    for i in range(n - 1, -1, -1):          # 3) 뒤에서부터 배치: 안정
        d = (a[i] // exp) % RADIX
        count[d] -= 1
        output[count[d]] = a[i]
    a[:] = output


def lsd_radix_sort(a):
    """가장 낮은 자리부터 자리마다 counting_sort_by_digit을 부른다. 패스마다 한 줄 찍는다."""
    max_value = max(a)
    exp = 1
    while max_value // exp > 0:
        counting_sort_by_digit(a, exp)
        print_numbers(f"exp {exp:<4d}", a)
        exp *= RADIX


def lsd_string_sort(s, w):
    """길이가 모두 w인 문자열 s를 마지막 문자부터 한 글자씩 안정 카운팅 정렬한다."""
    n = len(s)
    for d in range(w - 1, -1, -1):
        count = [0] * R
        for word in s:
            count[ord(word[d])] += 1
        for c in range(1, R):
            count[c] += count[c - 1]
        aux = [""] * n
        for i in range(n - 1, -1, -1):
            c = ord(s[i][d])
            count[c] -= 1
            aux[count[c]] = s[i]
        s[:] = aux
        print_strings(f"char {d}", s)


if __name__ == "__main__":
    a = [437, 42, 531, 8, 74]

    print_numbers("input   ", a)
    lsd_radix_sort(a)
    print_numbers("sorted  ", a)
    print()

    s = ["dab", "add", "cab", "fad", "fee", "bad",
         "dad", "bee", "fed", "bed", "ebb", "ace"]

    print_strings("input ", s)
    lsd_string_sort(s, 3)
    print_strings("sorted", s)
