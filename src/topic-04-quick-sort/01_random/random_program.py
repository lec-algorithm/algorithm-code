"""유사난수 — 랜덤처럼 보이는 수열도 함수의 출력이다.

random 모듈 대신 같은 종류의 생성기(minstd, 곱수 16807)를 직접 만든다.
라이브러리 생성기는 언어마다 수열이 다르지만, 이렇게 하면 C와 Python이
같은 수열을 만들어 "같은 시드 = 같은 수열"이 재현된다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 random_program.py`
"""

state = 1


def set_seed(seed):
    """srand처럼 시드를 바꾼다."""
    global state
    state = seed


def next_random():
    """rand처럼 다음 수를 만든다. x <- x * 16807 mod (2^31 - 1)"""
    global state
    state = state * 16807 % 2147483647
    return state


def print_sequence(seed, count):
    set_seed(seed)
    print(f"seed = {seed} :", *[next_random() for _ in range(count)])


if __name__ == "__main__":
    print_sequence(1, 3)
    print_sequence(1, 3)  # 같은 시드: 완전히 같은 수열
    print_sequence(2, 3)  # 시드를 바꿔야 수열이 바뀐다
