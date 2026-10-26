# 03. 순회 (Traversal)

트리의 모든 노드를 한 번씩 방문하는 순서는 여럿이다. 재귀로 쓰는 세 순회는
**현재 노드를 언제 출력하느냐**만 다르다. 중위(inorder)는 왼쪽 다음, 전위
(preorder)는 맨 먼저, 후위(postorder)는 맨 나중이다. 레벨 순서(level order)는
큐를 써서 위 레벨부터 한 층씩 읽는다.
같은 절차를 의사코드 하나에 C와 Python 두 구현으로 담았다.

| 파일 | 내용 |
| --- | --- |
| `traversal.pseudo` | 기준이 되는 절차. 두 구현은 이것을 옮긴 것이다 |
| `traversal.c` | C 구현 |
| `traversal.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-07-bst/03_traversal
make -f /work/Makefile traversal.out && ./traversal.out
python3 traversal.py
```

- 결과

```console
insert DBFACEG
  tree       = D(B(A,C),F(E,G))
  inorder    = A B C D E F G
  preorder   = D B A C F E G
  postorder  = A C B E G F D
  levelorder = D B F A C E G

insert PFBHGSRYTWZ
  tree       = P(F(B,H(G,-)),S(R,Y(T(-,W),Z)))
  inorder    = B F G H P R S T W Y Z
  preorder   = P F B H G S R Y T W Z
  postorder  = B G H F R W T Z Y S P
  levelorder = P F S B H R Y G T Z W
```

두 구현이 같은 결과를 낸다. `tree`는 모양을 한 줄로 적은 것이다
(`키(왼쪽,오른쪽)`, 빈 자식은 `-`).

- **중위**는 두 트리 모두 알파벳 순서다. BST를 중위로 읽으면 언제나
  정렬된 순서가 나온다.
- **전위**는 루트가 맨 앞이다. 두 번째 트리의 전위 `P F B H G S R Y T W Z`는
  이 트리를 만들 때 넣은 순서와 같다.
- **후위**는 루트가 맨 뒤다. 자식이 모두 끝나야 부모 차례가 온다.
- **레벨 순서**는 위에서 아래로, 같은 레벨은 왼쪽에서 오른쪽으로 읽는다.
  첫 번째 트리는 넣은 순서와 같다.

## 바꿔 보기

`trees`에 `ABCDEFG`를 넣어 보자. 한쪽으로 기운 트리라 중위와 전위가
같아지고, 후위는 그 반대 순서가 된다. 레벨 순서도 전위와 같다. 모양이
모든 순회의 결과를 정한다.

## 다음

다음 예제(출력·삭제·복사)는 이 순회들을 트리 전체를 다루는 일에 쓴다.
