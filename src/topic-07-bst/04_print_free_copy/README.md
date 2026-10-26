# 04. 출력·해제·복사 (Print, Free, Copy)

트리 전체를 다루는 일은 순회 하나로 끝난다. 어느 순회를 쓰느냐는 **부모와
자식 중 누구를 먼저 다뤄야 하느냐**로 정한다. 모양 출력과 복사는 부모가
먼저인 전위, 해제는 자식이 먼저인 후위, 정렬된 출력은 중위다. 끝으로 순회
결과를 저장했다가 그 순서대로 다시 넣으면 어떤 트리가 되는지 본다.
같은 절차를 의사코드 하나에 C와 Python 두 구현으로 담았다.

| 파일 | 내용 |
| --- | --- |
| `treeOps.pseudo` | 기준이 되는 절차. 두 구현은 이것을 옮긴 것이다 |
| `treeOps.c` | C 구현 |
| `tree_ops.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-07-bst/04_print_free_copy
make -f /work/Makefile treeOps.out && ./treeOps.out
python3 tree_ops.py
```

- 결과

```console
insert PFBHGSRYTWZ

P
├── F
│   ├── B
│   └── H
│       ├── G
│       └── -
└── S
    ├── R
    └── Y
        ├── T
        │   ├── -
        │   └── W
        └── Z

sorted (inorder)  : B F G H P R S T W Y Z
copy   (preorder) : P F B H G S R Y T W Z
copy is same      : yes
free   (postorder): B G H F R W T Z Y S P
copy after free   : NULL

rebuild from preorder: height 5, same shape: yes
rebuild from inorder : height 11, same shape: no
```

두 구현이 같은 결과를 낸다.

- **모양 출력**: 노드를 적고 그 아래에 자식을 들여 적는다. 위가 왼쪽 자식,
  아래가 오른쪽 자식이고, 형제가 있는데 비어 있는 자식은 `-`로 적는다.
- **복사**: 부모를 먼저 만들어야 자식을 매달 수 있으므로 전위다. 복사본은
  원래 트리와 모양까지 같다.
- **해제**: 부모를 먼저 지우면 자식으로 가는 주소를 잃는다. 그래서 자식부터
  지우는 후위다. 해제가 끝나면 루트 포인터를 `NULL`로 돌려받는다.
- **다시 만들기**: 전위 순서대로 다시 넣으면 높이 5의 같은 트리가 된다.
  중위 순서(정렬된 순서)로 넣으면 한쪽으로만 자라 높이가 11, 노드 수와 같다.

Python은 쓰지 않는 객체를 알아서 거두므로 `free`가 없다. 이 구현은 C와 같은
순서로 링크를 끊어 해제 순서를 보여 준다.

## 바꿔 보기

`rebuild`에 후위 순서 `B G H F R W T Z Y S P`를 넣어 보자. 루트 P가 맨
마지막에 들어가므로 B가 루트가 되고 모양이 완전히 달라진다. 레벨 순서
`P F S B H R Y G T Z W`는 어떨지도 예상해 보고 돌려 보자.

## 다음

다음 주제(해시 테이블)는 비교 없이 키에서 바로 자리를 계산해, 검색을 평균
O(1)로 줄인다.
