# 02. TST (Ternary Search Tree)

노드마다 링크를 **3개만** 두는 트라이. 노드는 문자 하나를 갖고, 찾는 문자가
그보다 작으면 왼쪽, 크면 오른쪽으로 **같은 자리에서** 옮겨 가고, 같으면
가운데로 내려가 **다음 문자**를 본다. 삽입(한 단어는 비교 한 번씩), 검색,
접두사로 모두 찾기를 돌리고, 같은 키를 담는 데 링크가 몇 개 드는지 센다.
같은 절차를 의사코드 하나에 C와 Python 두 구현으로 담았다.

| 파일 | 내용 |
| --- | --- |
| `tst.pseudo` | 기준이 되는 절차(노드, 삽입, 검색, 접두사). 두 구현은 이것을 옮긴 것이다 |
| `tst.c` | C 구현 |
| `tst.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-10-trie/02_tst
make -f /work/Makefile tst.out && ./tst.out
python3 tst.py
```

- 결과

```console
insert apple=10 application=20 apply=30 applaud=40 applet=50 appliance=60 applicable=70
insert appliance=60
  a = a   middle
  p = p   middle
  p = p   middle
  l = l   middle
  i > e   right
  i = i   middle
  a < c   left
  a       new node
  n       new node
  c       new node
  e       new node = 60

a
  M p
    M p
      M l
        M e = 10
          L a
            M u
              M d = 40
          M t = 50
          R i
            M c
              L a
                M n
                  M c
                    M e = 60
              M a
                M t
                  L b
                    M l
                      M e = 70
                  M i
                    M o
                      M n = 20
            R y = 30

search
  applaud     -> 40     visited 8
  apply       -> 30     visited 7
  application -> 20     visited 12
  app         -> none   visited 3
  apples      -> none   visited 6

getWordsWithPrefix(app): applaud(40) apple(10) applet(50) appliance(60) applicable(70) application(20) apply(30)
getWordsWithPrefix(appli): appliance(60) applicable(70) application(20)
getWordsWithPrefix(apple): apple(10) applet(50)
getWordsWithPrefix(b): (none)

space (3 links per node)
  keys      N  nodes  links   null  height
  apple..   7     24     72     49      12
  sorted    7     24     72     49      15
  by..the   6     16     48     33       8
```

두 구현이 같은 결과를 낸다. `appliance`를 넣는 과정을 보자. `appl`까지는
같은 문자라 가운데로 내려간다. 다음 `i`는 `e`보다 커서 **같은 자리에서**
오른쪽으로 가고, 거기 있던 `i`(`application`이 만든 노드)와 같으니 가운데로
내려간다. `a`는 `c`보다 작아 왼쪽, 그 자리가 비어 있어 나머지 `a n c e`를
새 노드로 단다.

트리 출력에서 `L`·`R`은 같은 자리의 다른 문자, `M`은 다음 자리다. `L`·`M`·`R`
순서로 읽으면 키가 사전 순으로 나오고, 접두사 검색의 결과도 그래서 사전
순이다. `apple`처럼 접두사 자체가 키이면 그것이 먼저 들어간다.

`space` 표의 `apple..`은 노드 24개에 링크 72개다. 같은 키를 트라이에
넣으면 노드 25개(루트 포함)에 링크 650개다(`01_trie` 출력의 `apple..` 줄).
노드 수는 거의 같고, 링크가 노드마다 26개에서 3개로 줄었다. 대신 TST는
**넣는 순서에 따라 모양이 바뀐다.** 같은 키를 사전 순으로 넣으면(`sorted`)
높이가 12에서 15로 늘어난다.

## 바꿔 보기

`words`의 순서를 섞어 보자. 키와 노드 수는 그대로인데 트리 모양과 높이가
바뀐다. 트라이에서는 순서를 바꿔도 모양이 같다.

## 다음

다음 주제(K-평균 클러스터링)에서는 문자열을 떠나 점들을 묶는다.
