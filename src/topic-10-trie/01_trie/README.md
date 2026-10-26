# 01. 트라이 (Trie)

키를 **문자 하나씩 경로로** 저장하는 트리. 노드마다 a-z 26개의 링크를 두고,
키의 마지막 문자에 해당하는 노드에 값을 둔다. 삽입, 검색, 접두사로 모두
찾기, 삭제를 차례로 돌리고, 링크가 몇 개나 비어 있는지와 검색 실패가 노드
몇 개에서 끝나는지 센다. 같은 절차를 의사코드 하나에 C와 Python 두 구현으로
담았다.

| 파일 | 내용 |
| --- | --- |
| `trie.pseudo` | 기준이 되는 절차(노드, 삽입, 검색, 접두사, 삭제). 두 구현은 이것을 옮긴 것이다 |
| `trie.c` | C 구현 |
| `trie.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-10-trie/01_trie
make -f /work/Makefile trie.out && ./trie.out
python3 trie.py
```

- 결과

```console
insert by=4 sea=2 sells=1 she=0 shells=3 the=5
  b
    y = 4
  s
    e
      a = 2
      l
        l
          s = 1
    h
      e = 0
        l
          l
            s = 3
  t
    h
      e = 5
nodes = 17 (with root), links = 17 x 26 = 442, non-null = 16

search
  shells -> 3      visited 7: root s h e l l s
  shell  -> none   visited 6: root s h e l l (no value)
  she    -> 0      visited 4: root s h e
  shore  -> none   visited 3: root s h (no link 'o')

insert shore=7
getWordsWithPrefix(sh): she(0) shells(3) shore(7)
getWordsWithPrefix(se): sea(2) sells(1)
getWordsWithPrefix(t): the(5)
getWordsWithPrefix(x): (none)

delete shells: removed s l l
  shells -> none   visited 4: root s h e (no link 'l')
  she    -> 0      visited 4: root s h e
  shore  -> 7      visited 6: root s h o r e
nodes = 17

space (R = 26, w = average key length)
  keys      N      w  nodes  links   null     RN    RNw
  by..the   6   3.67     17    442    426    156    572
  chain     6   3.50      7    182    176    156    546
  spread    6   3.50     22    572    551    156    546
  apple..   7   7.57     25    650    626    182   1378

search miss, random keys of length 7, 1000 searches
  N = 100    average visited = 2.15
  N = 1000   average visited = 2.82
  N = 10000  average visited = 3.45
```

두 구현이 같은 결과를 낸다. 키 6개에 노드가 루트까지 17개다. `sea`와
`sells`는 `se`를, `she`와 `shells`는 `she`를 함께 쓴다. 링크는 442개인데
쓰는 것은 16개뿐이고 426개가 비어 있다.

검색은 문자마다 링크를 하나씩 따라간다. 실패는 두 가지다. `shell`은 끝까지
갔지만 그 노드에 값이 없고, `shore`는 `sh` 다음에 `o` 링크가 없다.
`shells`를 지우면 값도 자식도 없는 `s`, `l`, `l`이 아래에서부터 떨어져 나가고,
값(0)이 있는 `e`에서 멈춘다.

`space` 표에서 루트의 링크 26개를 빼면 `links`는 늘 `RN`과 `RNw` 사이에
있다. 키가 서로 접두사를 많이 공유하면(`chain`) `RN`에 붙고, 공유하지
않으면(`spread`) `RNw`에 붙는다. 마지막 표는 검색 실패가 키 길이(7)와 상관없이
노드 두세 개에서 끝난다는 것을 보여 준다. N이 10배가 될 때마다 0.7개쯤
(log₂₆ 10 ≈ 0.71) 늘어난다.

## 바꿔 보기

`main`의 `words`에 자기 이름이나 좋아하는 단어를 넣고 `getWordsWithPrefix`로
접두사 검색을 해 보자. `searchMiss`의 키 길이 7을 20으로 바꿔도 검색 실패의
평균은 거의 그대로다.

## 다음

다음 예제(TST)는 노드마다 링크를 26개가 아니라 **3개만** 둔다.
