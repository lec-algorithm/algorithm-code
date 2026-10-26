# 01. 검색과 삽입 (Search and Insert)

이진 탐색 트리에서 값을 찾을 때는 노드와 비교해 **작으면 왼쪽, 크면
오른쪽**으로 내려간다. 삽입은 검색이 실패한 바로 그 자리(비어 있는 자식)에
새 노드를 매다는 것이다. 같은 키라도 넣는 순서에 따라 트리의 높이가
달라지고, 검색 비용은 그 높이를 따른다.
같은 절차를 의사코드 하나에 C와 Python 두 구현으로 담았다.

| 파일 | 내용 |
| --- | --- |
| `bstSearchInsert.pseudo` | 기준이 되는 절차. 두 구현은 이것을 옮긴 것이다 |
| `bstSearchInsert.c` | C 구현 |
| `bst_search_insert.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-07-bst/01_search_insert
make -f /work/Makefile bstSearchInsert.out && ./bstSearchInsert.out
python3 bst_search_insert.py
```

- 결과

```console
insert 5 9 7 3 8 12 6 4 20
  insert  5: root
  insert  9: 5 -> right of 5
  insert  7: 5 9 -> left of 9
  insert  3: 5 -> left of 5
  insert  8: 5 9 7 -> right of 7
  insert 12: 5 9 -> right of 9
  insert  6: 5 9 7 -> left of 7
  insert  4: 5 3 -> right of 3
  insert 20: 5 9 12 -> right of 12
inorder = 3 4 5 6 7 8 9 12 20
height = 4, comparisons = 17

tree 7 4 14 1 6 16 2 5 15
  search 6: 7 4 6 -> found (3 comparisons)
  search 13: 7 14 -> not found (2 comparisons)
  min = 1, max = 16

same keys, two shapes
  tree S R X M H E C A: height 7
    search E: S R M H E -> found (5 comparisons)
  tree H C R A E M S X: height 4
    search E: H C E -> found (3 comparisons)

insert order -> shape
  D B F A C E G: height 3, comparisons 10
  B A D C G F E: height 5, comparisons 13
  A B C D E F G: height 7, comparisons 21
```

두 구현이 같은 결과를 낸다.

- **삽입**: `insert 8`의 길 `5 9 7`은 8을 찾다가 실패한 길이다. 7의 오른쪽이
  비어 있어서 거기에 매달렸다. 중위 순서로 읽으면 넣은 순서와 상관없이
  정렬되어 나온다.
- **검색**: 6은 비교 3번에 찾는다. 13은 14의 왼쪽이 비어 있으므로 비교 2번
  만에 없다고 답한다. 가장 작은 값은 왼쪽 끝(1), 가장 큰 값은 오른쪽 끝(16)이다.
- **모양**: 같은 키 8개가 높이 7로도, 높이 4로도 들어간다. E를 찾는 비교
  횟수가 5번과 3번으로 갈린다.
- **넣는 순서**: A부터 G까지 정렬된 순서로 넣으면 한쪽으로만 자라 높이가
  7, 키 개수와 같아진다. 연결 리스트와 다를 바가 없다.

높이는 레벨의 수로 센다. 루트만 있으면 1이다. 간선 수로 세는 교재도 있으니
다른 자료와 비교할 때는 1을 빼서 맞춘다.

## 바꿔 보기

1번 `keys`를 `20 12 9 8 7 6 5 4 3`처럼 내림차순으로 바꿔 보자. 높이가 9가
되고 비교 횟수는 0+1+…+8 = 36번으로 늘어난다. 같은 키를
`7 4 12 3 5 9 20 6 8`처럼 가운데 값부터 넣으면 높이가 4로 줄어든다.

## 다음

다음 예제(삭제)는 노드를 떼어 내면서도 BST의 성질을 지키는 방법이다.
