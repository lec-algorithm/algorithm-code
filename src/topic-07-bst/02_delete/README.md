# 02. 삭제 (Delete)

이진 탐색 트리에서 노드를 지울 때는 **왼쪽이 작고 오른쪽이 크다**는 성질이
깨지지 않아야 한다. 지울 노드의 자식 수에 따라 세 경우로 나뉜다.
자식이 없으면 떼어 내고, 하나면 그 자식을 부모에 바로 잇고, 둘이면
**후속자**(오른쪽 서브트리의 가장 작은 값)를 그 자리에 복사한 뒤 원래
후속자를 지운다.
같은 절차를 의사코드 하나에 C와 Python 두 구현으로 담았다.

| 파일 | 내용 |
| --- | --- |
| `bstDelete.pseudo` | 기준이 되는 절차. 두 구현은 이것을 옮긴 것이다 |
| `bstDelete.c` | C 구현 |
| `bst_delete.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-07-bst/02_delete
make -f /work/Makefile bstDelete.out && ./bstDelete.out
python3 bst_delete.py
```

- 결과

```console
insert BADCGFE
  tree    = B(A,D(C,G(F(E,-),-)))
  inorder = A B C D E F G
delete E: no child, just remove it
  tree    = B(A,D(C,G(F,-)))
  inorder = A B C D F G
delete F: one child E, link it to the parent
  tree    = B(A,D(C,G(E,-)))
  inorder = A B C D E G
delete D: two children, successor E takes its place
  tree    = B(A,E(C,G(F,-)))
  inorder = A B C E F G

insert JBQLRKNZMP
  tree    = J(B,Q(L(K,N(M,P)),R(-,Z)))
  inorder = B J K L M N P Q R Z
delete Q: two children, successor R takes its place
  tree    = J(B,R(L(K,N(M,P)),Z))
  inorder = B J K L M N P R Z
delete J: two children, successor K takes its place
  tree    = K(B,Q(L(-,N(M,P)),R(-,Z)))
  inorder = B K L M N P Q R Z
```

두 구현이 같은 결과를 낸다. `tree`는 모양을 한 줄로 적은 것이다.
`키(왼쪽,오른쪽)`이고 빈 자식은 `-`다. 삭제는 매번 처음 트리에서 한다.

- **E**: 잎이라 그냥 떼어 냈다.
- **F**: 자식이 E 하나다. E가 F의 자리, 곧 G의 왼쪽으로 올라왔다.
- **D**: 자식이 둘이다. 오른쪽 서브트리 `G(F(E,-),-)`에서 왼쪽 끝까지 가면
  E가 나온다. E를 D 자리에 복사하고, 원래 E(잎)를 지웠다.
- **Q**: 후속자는 R이다. R은 왼쪽 자식이 없으니 오른쪽 서브트리의 가장
  작은 값이 바로 R 자신이다. R을 복사하고 원래 R을 지우면 그 자식 Z가 올라온다.
- **J**: 루트도 같다. 후속자 K를 찾아 오른쪽 서브트리를 Q, L, K까지 내려갔다.

어느 경우든 중위 순서는 지운 값 하나만 빠지고 그대로 정렬되어 있다.

## 바꿔 보기

`deleteNode`의 경우 3을 **선행자**(왼쪽 서브트리의 가장 큰 값)로 바꿔 보자.
`findMin(root->right)` 대신 왼쪽 서브트리의 오른쪽 끝을 찾으면 된다. D를
지우면 C가, Q를 지우면 P가 그 자리에 온다. 모양은 달라지지만 중위 순서는
같다. 둘 다 맞는 삭제다.

## 다음

다음 예제(순회)는 트리의 모든 노드를 한 번씩 방문하는 세 가지 순서다.
