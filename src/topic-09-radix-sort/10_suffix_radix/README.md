# 10. 접미사 배열: 짝을 기수 정렬로

더블링의 짝 (순위, k/2칸 뒤의 순위)을 두 자리 수로 보고, `03_suffix_array`의 `qsort` 대신
**LSD 기수 정렬 두 패스**로 정렬한다. 결과가 접미사를 그대로 정렬한 것과 같은지 확인한다.

| 파일 | 내용 |
| --- | --- |
| `suffixRadix.pseudo` | 기준이 되는 절차 |
| `suffixRadix.c` | C 구현 |
| `suffix_radix.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-09-radix-sort/10_suffix_radix
make -f /work/Makefile suffixRadix.out && ./suffixRadix.out
python3 suffix_radix.py
```

- 결과

```console
banana$ (n = 7)
  k = 2  distinct ranks =  5 of 7
  k = 4  distinct ranks =  7 of 7
  SA = 6 5 3 1 0 4 2
  passes = 4, same as plain sort = yes
mississippi$ (n = 12)
  k = 2  distinct ranks =  9 of 12
  k = 4  distinct ranks = 11 of 12
  k = 8  distinct ranks = 12 of 12
  SA = 11 10 7 4 1 0 9 8 6 3 5 2
  passes = 6, same as plain sort = yes
aaaaaaaa$ (n = 9)
  k = 2  distinct ranks =  3 of 9
  k = 4  distinct ranks =  5 of 9
  k = 8  distinct ranks =  9 of 9
  SA = 8 7 6 5 4 3 2 1 0
  passes = 6, same as plain sort = yes
```

두 구현이 같은 결과를 낸다. 라운드마다 패스가 두 번이다.
