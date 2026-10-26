"""해시 함수 — 키를 배열 인덱스 [0, M-1]로 바꾼다.

정수 키는 나머지(모듈러 해싱), 문자열 키는 글자를 숫자로 바꿔 섞은 뒤
나머지를 취한다. 마지막으로 키가 몇 개면 충돌이 생기는지 확률을 셈한다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 hash_function.py`
"""


def modular_hash(k, m):
    """모듈러 해싱: 양의 정수 k를 m으로 나눈 나머지"""
    return k % m


def sum_hash(s, m):
    """나쁜 예: 글자 코드를 더하기만 한다. 순서가 다른 같은 글자들은 모두 같은 값"""
    h = 0
    for c in s:
        h = (h + ord(c)) % m
    return h


def horner_hash(s, m):
    """호너 방식: 자리마다 R을 곱해 순서를 반영한다. 매번 나머지를 취한다."""
    R = 31
    h = 0
    for c in s:
        h = (R * h + ord(c)) % m
    return h


def collision_chance(n, m):
    """키 n개를 m칸에 고르게 무작위로 넣을 때 적어도 한 번 충돌할 확률"""
    none = 1.0
    for i in range(n):
        none = none * (m - i) / m
    return 1.0 - none


def print_buckets(keys, m):
    hashes = [modular_hash(k, m) for k in keys]
    used = len(set(hashes))
    print(f"  M = {m}:", *hashes, f" -> {used} of {m} buckets used")


if __name__ == "__main__":
    # 1. 글자 코드를 M = 8로 나눈 나머지
    m = 8
    letters = ["A", "B", "C", "K"]
    print(f"modular hashing, M = {m}")
    for i, c in enumerate(letters):
        h = modular_hash(ord(c), m)
        line = f"  {c} ({ord(c)}) -> {h}"
        for prev in letters[:i]:
            if modular_hash(ord(prev), m) == h:
                line += f"  collides with {prev}"
                break
        print(line)

    # 2. 4의 배수 키를 M = 8과 M = 7에
    keys = [4 * i for i in range(8)]
    print()
    print("keys", *keys)
    print_buckets(keys, 8)
    print_buckets(keys, 7)

    # 3. 같은 글자로 된 문자열 여섯 개, M = 97
    words = ["stop", "pots", "tops", "spot", "opts", "post"]
    m = 97
    print(f"\nstring keys, M = {m}")
    print("  key   sum  horner")
    for w in words:
        print(f"  {w:<4} {sum_hash(w, m):4d} {horner_hash(w, m):7d}")

    # 4. 생일 문제
    print("\nchance of at least one collision")
    for n, m in [(3, 16), (5, 16), (10, 16), (23, 365)]:
        print(f"  N = {n:2d}, M = {m:3d}: {collision_chance(n, m) * 100:5.1f}%")
