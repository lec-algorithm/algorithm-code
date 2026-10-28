# 02. 거스름돈 (Coin Change)

금액을 만드는 **동전 개수의 최솟값**을 동적 계획법으로 구한다.
마지막에 낸 동전이 `c`라면 나머지 `amount - c`는 다시 같은 문제다.
같은 점화식을 메모이제이션(하향식)과 타뷸레이션(상향식) 두 가지로 풀고,
타뷸레이션에서는 쓴 동전까지 거꾸로 따라가 복원한다.

| 파일 | 내용 |
| --- | --- |
| `coinChange.pseudo` | 점화식과 두 방식의 절차 |
| `coinChange.c` | C 구현 |
| `coin_change.py` | Python 구현 |

## 실행

파일을 열고 편집기 오른쪽 위 **▶ 버튼**을 누릅니다. C와 Python 모두 됩니다.

터미널에서 직접 돌리려면 이렇게 합니다.

- 실행

```sh
cd src/topic-14-dynamic-programming/02_coin_change
make -f /work/Makefile coinChange.out && ./coinChange.out
python3 coin_change.py
```

- 결과

```console
coins = {2, 3, 5}, amount = 6
  solve 6
  solve 4
  solve 2
  solve 1
  solve 3
memo -> 2 coins, solved 5 amounts
  memo = [-1, INF, 1, 1, 2, -1, 2]
tab  -> 2 coins: 3 + 3, filled 6 amounts
  dp   = [0, INF, 1, 1, 2, 1, 2]

coins = {50, 40, 10}, amount = 80
memo -> 2 coins, solved 8 amounts
tab  -> 2 coins: 40 + 40, filled 80 amounts
```

두 구현이 같은 결과를 낸다.

- **메모**는 6원에서 내려가며 6, 4, 2, 1, 3원만 풀었다. `memo[5]`는 -1로
  남았다. 5원은 6원을 만드는 데 필요하지 않았기 때문이다 (lazy).
- **표**는 1원부터 6원까지 전부 채웠다. `dp[5] = 1`도 구했지만 답에는 쓰지
  않았다 (eager).
- `INF`는 만들 수 없는 금액이다. 1원은 어떤 동전으로도 못 만든다.

둘째 묶음은 주제 06에서 그리디가 **4개**(50 + 10 + 10 + 10)를 낸 입력이다.
동적 계획법은 **40 + 40, 2개**를 찾는다. 메모는 10원 단위 금액 8개만 풀었고,
표는 80칸을 전부 채웠다.

## 바꿔 보기

- 동전을 `{5, 3}`, 금액을 7로 바꿔 보자. 7원은 못 만들므로 `INF`가 나온다.
- `{50, 40, 10}`에서 그리디와 답이 갈리는 금액을 더 찾아보자. 주제 06의
  그리디 코드와 나란히 돌리면 바로 비교된다.

## 다음

다음 예제(최장 증가 부분수열)는 칸 하나를 채울 때 앞의 칸 여러 개를 본다.
