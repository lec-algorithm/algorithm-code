"""정규식 매칭 — 재귀와 백트래킹으로 '.'과 '*'를 처리한다.

match는 패턴을 한 글자씩 맞춰 나가고, 'x*'를 만나면 match_star가
x를 0개, 1개, 2개, ... 먹어 보며 나머지를 다시 시도한다.
비교 횟수(match 호출 수)와 최대 재귀 깊이를 함께 센다.
re 모듈은 쓰지 않는다. 같은 절차를 직접 구현한다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 regex_match.py`
"""

comparisons = 0        # match 호출 수
current_recursion = 0  # 지금 재귀 깊이
max_recursion = 0      # 가장 깊었던 재귀 깊이
trace = False          # True면 match 호출마다 한 줄 찍는다


def enter():
    global current_recursion, max_recursion
    current_recursion += 1
    if current_recursion > max_recursion:
        max_recursion = current_recursion


def match(pattern, text):
    """text의 앞부분이 pattern에 맞으면 True"""
    global comparisons, current_recursion
    comparisons += 1
    enter()
    if trace:
        print(f"  {'':{2 * current_recursion}}match(\"{pattern}\", \"{text}\")")

    if pattern == "":                              # 패턴이 끝나면 성공
        result = True
    elif len(pattern) > 1 and pattern[1] == "*":   # 다음 글자가 '*' (x* 패턴)
        result = match_star(pattern[0], pattern[2:], text)
    elif text != "" and (pattern[0] == "." or pattern[0] == text[0]):
        result = match(pattern[1:], text[1:])      # 한 글자 맞추고 전진
    else:
        result = False
    current_recursion -= 1
    return result


def match_star(c, pattern, text):
    """c를 0개 이상 먹은 뒤 나머지 pattern이 맞으면 True"""
    global current_recursion
    enter()
    result = False
    if match(pattern, text):                       # 0개 매칭 시도
        result = True
    else:
        while text != "" and (text[0] == c or c == "."):   # 1개 이상
            text = text[1:]
            if match(pattern, text):
                result = True
                break
    current_recursion -= 1
    return result


def search(pattern, text):
    """pattern이 처음 맞는 위치. 없으면 -1. 정규식 .*pattern 과 같다."""
    for i in range(len(text) + 1):
        if trace:
            print(f"  start {i}")
        if match(pattern, text[i:]):
            return i
    return -1


if __name__ == "__main__":
    # 1) 호출 하나하나를 찍어 본다. 들여쓰기가 재귀 깊이다.
    print('search("ab*c", "aabbc")')
    trace = True
    comparisons = 0
    at = search("ab*c", "aabbc")
    trace = False
    print(f"  -> found at {at}, comparisons = {comparisons}")
    print()

    # 2) 여러 패턴과 텍스트
    tests = [
        ("ab*c", "ac"), ("ab*c", "abbbbc"), ("ab*c", "abd"),
        ("a.c", "xxabcxx"), (".*ab*c", "zzabbbcz"),
    ]
    print("tests")
    print(f"  {'pattern':<8} {'text':<10} {'at':>3} {'comparisons':>12}")
    for pattern, text in tests:
        comparisons = 0
        found = search(pattern, text)
        print(f"  {pattern:<8} {text:<10} {found:>3} {comparisons:>12}")

    # 3) 'a*'를 n번 이은 뒤 b. 텍스트는 a를 n개. 맞지 않는데, 그걸 알기까지
    print()
    print("blow-up: match((a*)^n b, a^n)")
    print(f"  {'n':<3} {'result':<12} {'comparisons':>12} {'max depth':>10}")
    for n in range(2, 11, 2):
        pattern = "a*" * n + "b"
        text = "a" * n
        comparisons = 0
        max_recursion = 0
        ok = match(pattern, text)
        result = "match" if ok else "no match"
        print(f"  {n:<3} {result:<12} {comparisons:>12} {max_recursion:>10}")
