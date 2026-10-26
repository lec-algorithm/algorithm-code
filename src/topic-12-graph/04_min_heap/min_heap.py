"""최소 힙 — 우선순위 큐의 바탕이 되는 완전 이진 트리.

1부터 9가 든 힙에 10과 0을 넣고(sift up), 최솟값을 하나 꺼낸다(sift down).
비교와 교환을 한 줄씩 찍는다. 끝으로 남은 것을 모두 꺼내 오름차순으로
나오는지 본다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 min_heap.py`
"""

CAPACITY = 16
verbose = True  # False이면 비교·교환을 찍지 않는다(마지막 확인용)


def log(text):
    if verbose:
        print(text)


def heap_str(a):
    return "heap [" + " ".join(map(str, a)) + "]"


def heapify_up(a, i):
    """i에 있는 값을 부모보다 작지 않을 때까지 올린다."""
    while i > 0:
        p = (i - 1) // 2
        if a[i] >= a[p]:
            log(f"  {a[i]} (index {i}) vs parent {a[p]} (index {p}): stay")
            return
        log(f"  {a[i]} (index {i}) vs parent {a[p]} (index {p}): swap")
        a[i], a[p] = a[p], a[i]
        i = p
    log("  reached the root")


def insert(a, x):
    if len(a) == CAPACITY:
        return False
    a.append(x)
    log(f"insert {x} at index {len(a) - 1}")
    heapify_up(a, len(a) - 1)
    return True


def heapify_down(a, i):
    """i에 있는 값을 두 자식보다 크지 않을 때까지 내린다."""
    n = len(a)
    while True:
        m = i
        l, r = 2 * i + 1, 2 * i + 2
        if l < n and a[l] < a[m]:
            m = l
        if r < n and a[r] < a[m]:
            m = r
        line = f"  {a[i]} (index {i}) vs children"
        if l < n:
            line += f" {a[l]} (index {l})"
        if r < n:
            line += f", {a[r]} (index {r})"
        if l >= n:
            line += " none"
        if m == i:
            log(line + ": stay")
            return
        log(line + f": swap with {a[m]}")
        a[i], a[m] = a[m], a[i]
        i = m


def extract_min(a):
    smallest = a[0]
    last = a.pop()
    if a:
        a[0] = last
    log(f"extract min {smallest}, move last {last} to the root")
    if a:
        heapify_down(a, 0)
    return smallest


if __name__ == "__main__":
    h = [1, 2, 3, 4, 5, 6, 7, 8, 9]
    print(heap_str(h))
    print()

    insert(h, 10)
    print(heap_str(h))
    print()

    insert(h, 0)
    print(heap_str(h))
    print()

    extract_min(h)
    print(heap_str(h))
    print()

    verbose = False
    out = []
    while h:
        out.append(extract_min(h))
    print("extract all:", *out)
