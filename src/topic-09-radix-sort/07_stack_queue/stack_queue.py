"""스택과 큐 — LIFO와 FIFO. 그리고 LSD 기수 정렬의 버킷으로 썼을 때의 차이.

버킷을 큐로 쓰면 같은 숫자끼리 들어온 순서가 지켜져(안정) 정렬이 되고,
스택으로 쓰면 순서가 뒤집혀(불안정) 정렬이 깨진다.

실행: 편집기 오른쪽 위 ▶ 버튼, 또는 `python3 stack_queue.py`
"""

RADIX = 10
WIDTH = 3


class Stack:
    def __init__(self):
        self.items = []

    def push(self, x):
        self.items.append(x)

    def pop(self):
        return self.items.pop()          # 마지막에 넣은 것

    def empty(self):
        return not self.items


class Queue:
    def __init__(self):
        self.items = []
        self.head = 0                    # 다음에 꺼낼 자리

    def enqueue(self, x):
        self.items.append(x)

    def dequeue(self):
        x = self.items[self.head]        # 가장 먼저 넣은 것
        self.head += 1
        return x

    def empty(self):
        return self.head == len(self.items)


def print_numbers(label, a):
    print(f"{label}:", *(f"{v:0{WIDTH}d}" for v in a))


def bucket_radix_sort(a, use_queue):
    """use_queue가 True면 큐 버킷, False면 스택 버킷으로 LSD 기수 정렬을 한다."""
    kind = "queue" if use_queue else "stack"
    max_value = max(a)
    exp = 1
    while max_value // exp > 0:
        if use_queue:
            buckets = [Queue() for _ in range(RADIX)]
        else:
            buckets = [Stack() for _ in range(RADIX)]
        for v in a:                                  # 흩기
            d = (v // exp) % RADIX
            if use_queue:
                buckets[d].enqueue(v)
            else:
                buckets[d].push(v)
        k = 0
        for d in range(RADIX):                       # 모으기
            while not buckets[d].empty():
                a[k] = buckets[d].dequeue() if use_queue else buckets[d].pop()
                k += 1
        print_numbers(f"{kind} exp {exp:<3d}", a)
        exp *= RADIX


def is_sorted(a):
    return all(a[i - 1] <= a[i] for i in range(1, len(a)))


if __name__ == "__main__":
    letters = ["A", "B", "C", "D"]
    s = Stack()
    q = Queue()

    for x in letters:
        s.push(x)
    print("push   :", *letters)
    out = []
    while not s.empty():
        out.append(s.pop())
    print("pop    :", *out)

    for x in letters:
        q.enqueue(x)
    print("enqueue:", *letters)
    out = []
    while not q.empty():
        out.append(q.dequeue())
    print("dequeue:", *out)
    print()

    data = [437, 42, 531, 8, 74]
    a = data[:]
    b = data[:]

    print_numbers("input        ", data)
    bucket_radix_sort(a, True)
    bucket_radix_sort(b, False)
    print(f"queue sorted = {'yes' if is_sorted(a) else 'no'},"
          f" stack sorted = {'yes' if is_sorted(b) else 'no'}")
