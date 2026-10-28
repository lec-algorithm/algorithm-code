"""정규식 → NFA — 정규식에서 ε 전이 그래프를 만들고 상태 집합으로 시뮬레이션한다.

상태 i는 정규식의 i번째 글자다. 글자 상태는 그 글자를 읽고 i+1로 가고,
괄호·'*'·'|'는 ε 전이를 만든다. 텍스트를 읽을 때는 "있을 수 있는 상태
전부"를 집합으로 들고 다니므로 되돌아가지 않는다. re 모듈은 쓰지 않는다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 nfa_regex.py`
"""

re_ = ""     # 괄호로 감싼 정규식. 상태 i = re_[i]
M = 0        # 수락 상태 번호 = 정규식 길이
edges = []   # ε 전이 목록 (v, w)
work = 0     # 집합에 넣은 상태 수의 합


def build_nfa(regex):
    """regex를 괄호로 감싸 re_에 두고 ε 전이 그래프를 만든다."""
    global re_, M, edges
    re_ = "(" + regex + ")"
    M = len(re_)
    edges = []
    ops = []
    for i in range(M):
        lp = i                                   # '*'가 붙을 덩어리의 시작
        if re_[i] in "(|":
            ops.append(i)
        elif re_[i] == ")":
            or_pos = ops.pop()
            if re_[or_pos] == "|":
                lp = ops.pop()                   # 짝이 되는 '('
                edges.append((lp, or_pos + 1))   # 왼쪽 갈래를 건너뛰고 오른쪽으로
                edges.append((or_pos, i))        # 왼쪽 갈래 끝에서 ')'로
            else:
                lp = or_pos
        if i < M - 1 and re_[i + 1] == "*":
            edges.append((lp, i + 1))            # 0번 (건너뛰기)
            edges.append((i + 1, lp))            # 한 번 더 (되풀이)
        if re_[i] in "(*)":
            edges.append((i, i + 1))             # 기호는 글자 없이 다음으로


def dfs(v, marked):
    global work
    marked.add(v)
    work += 1
    for a, b in edges:
        if a == v and b not in marked:
            dfs(b, marked)


def closure(states):
    """states에서 ε 전이만 따라 갈 수 있는 상태 전부"""
    out = set()
    for v in range(M + 1):
        if v in states and v not in out:
            dfs(v, out)
    return out


def is_literal(c):
    return c not in "()*|"


def print_set(label, states):
    print(f"  {label:<10} {{{' '.join(str(v) for v in sorted(states))}}}")


def recognizes(text, trace):
    """text 전체가 정규식에 맞으면 True. trace면 글자마다 상태 집합을 찍는다."""
    pc = closure({0})
    if trace:
        print_set("start", pc)
    for ch in text:
        match = {i + 1 for i in range(M)
                 if i in pc and is_literal(re_[i]) and (re_[i] == ch or re_[i] == ".")}
        pc = closure(match)
        if trace:
            print_set(f"read '{ch}'", match)
            print_set("closure", pc)
    return M in pc


if __name__ == "__main__":
    # 1) 3의 배수를 이진수로 나타내는 정규식
    regex = "(0|1(01*0)*1)*"
    build_nfa(regex)
    print(f"regex = {regex}")
    print(f"nfa   = {re_}, states 0..{M}, accept {M}")
    print()
    print("  state" + "".join(f"{i:3d}" for i in range(M + 1)))
    print("  char " + "".join(f"{c:>3}" for c in re_))
    print()
    for k in range(0, len(edges), 9):               # 9개마다 줄을 바꾼다
        head = "  eps  " if k == 0 else "       "
        print(head + "".join(f" {v}->{w}" for v, w in edges[k:k + 9]))
    print()
    print("text = 1001")
    ok = recognizes("1001", True)
    print(f"  -> {'accept' if ok else 'reject'}")
    print()

    texts = ["11", "110", "1001", "1100", "10", "1011", "10000"]
    print(f"  {'text':<6} {'value':>5}  result")
    for t in texts:
        value = int(t, 2)
        result = "accept" if recognizes(t, False) else "reject"
        print(f"  {t:<6} {value:>5}  {result}")

    # 2) 01의 백트래킹이 폭발하던 입력: (a*)^n b 와 a^n
    print()
    print("blow-up: (a*)^n b on a^n")
    print(f"  {'n':<3} {'M':>4} {'N':>4}  {'result':<9} {'work':>6}")
    for n in range(2, 11, 2):
        build_nfa("a*" * n + "b")
        work = 0
        found = recognizes("a" * n, False)
        print(f"  {n:<3} {M:>4} {n:>4}  {'match' if found else 'no match':<9} {work:>6}")
