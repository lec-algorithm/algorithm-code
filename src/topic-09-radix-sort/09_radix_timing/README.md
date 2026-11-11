# 09. 기수 정렬 시간 재기

LSD, MSD, 크기 16 이하를 삽입 정렬로 넘기는 MSD의 시간을 잰다. 모두 R = 256이고, 값은 밀리초다.
값은 컴퓨터마다 다르므로 비율을 본다.

| 파일 | 내용 |
| --- | --- |
| `radixTiming.pseudo` | 재는 절차 |
| `radixTiming.c` | C 구현. 1,000,000개까지 잰다 |
| `radix_timing.py` | Python 구현. 100,000개까지 잰다 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

```sh
cd src/topic-09-radix-sort/09_radix_timing
make -f /work/Makefile radixTiming.out && ./radixTiming.out
python3 radix_timing.py
```

시간을 재는 예제라 두 구현의 출력 숫자는 같지 않다. 입력은 시드가 고정된 같은 값이다.
