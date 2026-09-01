# 01. 랜덤 (Random)

랜덤처럼 보이는 수열도 사실은 함수의 출력이라는 것을 보이는 예제.
rand() 같은 라이브러리 생성기 대신, 같은 종류의 생성기(minstd, 곱수
16807)를 두 언어에 직접 만들었다. 그래서 C와 Python이 같은 수열을 낸다.

| 파일 | 내용 |
| --- | --- |
| `randomProgram.c` | C 구현 (setSeed / nextRandom) |
| `random_program.py` | Python 구현 (set_seed / next_random) |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-04-quick-sort/01_random
make -f /work/Makefile randomProgram.out && ./randomProgram.out
python3 random_program.py
```

- 결과

```console
seed = 1 : 16807 282475249 1622650073
seed = 1 : 16807 282475249 1622650073
seed = 2 : 33614 564950498 1097816499
```

두 구현이 같은 결과를 낸다. **시드가 같으면 수열이 완전히 같다.**
바뀌는 것은 시드를 바꿀 때뿐이다.

## 바꿔 보기

시드를 시계(`time(NULL)`, `time.time()`)로 넣으면 돌릴 때마다 다른
수열이 나온다. "예측하기 어려운 시드"가 실전 난수의 전부다.
C의 `rand()`와 Python의 `random`으로 바꿔 보면 언어끼리 수열이
달라진다. 생성기가 다르기 때문이다.

## 다음

[02_quick_sort](../02_quick_sort/)의 피벗 선택에서 이 랜덤이 무기가
됩니다. 어떤 입력이 와도 최악을 피하는 방법입니다.
