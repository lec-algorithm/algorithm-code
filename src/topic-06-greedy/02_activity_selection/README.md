# 02. 활동 선택 (Activity Selection)

회의실 하나에 서로 겹치지 않는 회의를 가장 많이 넣는 문제.
끝 시각으로 정렬한 뒤, 언제나 **가장 일찍 끝나는 회의부터** 고른다.
같은 절차를 의사코드 하나에 C와 Python 두 구현으로 담았다.

| 파일 | 내용 |
| --- | --- |
| `activitySelection.pseudo` | 기준이 되는 절차. 두 구현은 이것을 옮긴 것이다 |
| `activitySelection.c` | C 구현 |
| `activity_selection.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-06-greedy/02_activity_selection
make -f /work/Makefile activitySelection.out && ./activitySelection.out
python3 activity_selection.py
```

- 결과

```console
input   : (5,8) (1,3) (8,9) (2,5) (6,7) (4,6)
by end  : (1,3) (2,5) (4,6) (6,7) (5,8) (8,9)
selected: (1,3) (4,6) (6,7) (8,9)
count = 4
```

두 구현이 같은 결과를 낸다. 괄호는 (시작, 끝)이다. (4,6) 다음에
(6,7)이 들어간 것을 보자. 앞 회의가 끝나는 시각에 다음 회의가
시작해도 겹치지 않는 것으로 친다.

## 바꿔 보기

기준을 "가장 일찍 시작하는 회의"로 바꿔 보자. 정렬 키만 바꾸면 된다.
`(0,10)` 같은 긴 회의를 하나 넣으면 그 회의 하나가 다른 회의를 전부
밀어내 1개만 고르게 된다. 그리디는 **무엇을 먼저 고르느냐**가 전부다.

## 다음

다음 예제(분할 배낭)는 물건을 쪼갤 수 있을 때의 그리디다.
