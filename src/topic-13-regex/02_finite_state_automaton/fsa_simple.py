"""유한 상태 오토마타 — 패턴 ab*c를 상태와 전이로 인식한다.

입력을 한 글자씩 읽으며 현재 상태를 옮긴다. S1에서는 입력 없이
S2로 넘어간다(ε 전이). 입력이 끝났을 때 S4에 있으면 수락한다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 fsa_simple.py`
"""

# 상태 정의
S0 = 0  # 시작 상태
S1 = 1  # 'a'를 읽은 상태
S2 = 2  # b* 시작 상태 (0개의 'b' 허용)
S3 = 3  # 'b'를 1개 이상 읽은 상태
S4 = 4  # 'c'를 읽어서 패턴 매칭 완료 (Accept 상태)


def accept(text):
    """text 전체가 ab*c에 맞으면 True. 전이마다 한 줄 찍는다."""
    current_state = S0
    print(f"  start S{current_state}")

    for ch in text:
        # 상태 전이 규칙
        if current_state == S0:
            if ch == "a":
                next_state = S1
                print(f"  '{ch}' -> S{next_state}")
                current_state = S2            # S1에서 ε 전이로 S2로 자동 이동
                print(f"  eps -> S{current_state}")
                continue
            print(f"  '{ch}' -> no move from S{current_state}: reject")
            return False
        elif current_state in (S2, S3):       # b* 시작 상태, 'b'를 1개 이상 읽은 상태
            if ch == "b":
                next_state = S3               # S3에서는 제자리
            elif ch == "c":
                next_state = S4
            else:
                print(f"  '{ch}' -> no move from S{current_state}: reject")
                return False
        else:                                 # S4: 수락 후에는 더 이상 입력 불가
            print(f"  '{ch}' -> no move from S{current_state}: reject")
            return False

        print(f"  '{ch}' -> S{next_state}")
        current_state = next_state

    # 최종 상태가 수락 상태(S4)인지 확인
    result = current_state == S4
    print(f"  final S{current_state}: {'accept' if result else 'reject'}")
    return result


if __name__ == "__main__":
    inputs = ["abc", "abbbbc", "ac", "ab", "abd", "abcc"]
    results = []

    print("pattern ab*c")
    for text in inputs:
        print()
        print(f'input "{text}"')
        results.append(accept(text))

    print()
    print("summary")
    for text, ok in zip(inputs, results):
        print(f"  {text:<8} {'accept' if ok else 'reject'}")
