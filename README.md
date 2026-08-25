# algorithm-code

2026-2 **고급알고리즘**(SIT2001-01)의 **강의 예제 코드**입니다.
주제마다 의사코드 하나에 C와 Python 두 구현이 붙습니다.

- 강의 자료: [lec-algorithm.github.io/lecture](https://lec-algorithm.github.io/lecture/)
- 실습 환경 template: [lec-algorithm/algorithm-env](https://github.com/lec-algorithm/algorithm-env)
- 시각화 자료: [lec-algorithm/algorithm-viz](https://github.com/lec-algorithm/algorithm-viz)

## 시작하기: 이 저장소에서 바로 Codespace 만들기

**복사하지 마세요.** 이 저장소는 학기 중에 주제가 계속 추가됩니다. 사본을
만들면 새 주제를 받을 방법이 없습니다.

1. 이 저장소의 초록색 **Code** 버튼
2. **Codespaces** 탭 → **Create codespace on main**

잠시 기다리면 브라우저에 VS Code가 뜹니다. 컴파일러와 Python이 준비된 컨테이너
안이므로 터미널에서 바로 돌려 보면 됩니다. 설치할 것은 없고 GitHub 계정만
있으면 됩니다.

만들어 둔 codespace는 [github.com/codespaces](https://github.com/codespaces)에서
다시 열 수 있습니다.

### 새 주제가 올라오면

codespace 터미널에서 받으면 됩니다.

```sh
git pull
```

## 로컬에서 하기

```sh
git clone https://github.com/lec-algorithm/algorithm-code.git
cd algorithm-code
docker compose up -d
docker compose exec lab bash
```

**이후 모든 `docker compose` 명령은 이 폴더에서 칩니다.**

## 돌려보기

**파일을 열고 편집기 오른쪽 위 ▶ 버튼**을 누르면 됩니다. C든 Python이든
그 파일이 실행됩니다.

- 실행 (`src/topic-01-intro-search/01_sequential_search/sequentialSearch.c`)

```console
n = 15, key = 51
index = 6, comparisons = 7
```

Python 구현도 같은 결과를 냅니다.

| 하고 싶은 것 | 방법 |
| --- | --- |
| 파일 하나 실행 | 편집기 오른쪽 위 **▶ 버튼** (Code Runner) |
| 파일 하나 빌드만 | `Cmd/Ctrl + Shift + B` |
| 디버그 | `F5` → **C 디버그 (현재 파일)** |

전부 `Makefile`을 거치므로 같은 폴더의 `.c`가 함께 빌드됩니다. Python은
`python3`로 실행됩니다.

터미널에서 직접 부를 수도 있습니다.

```sh
cd src/topic-01-intro-search/01_sequential_search
make -f /work/Makefile sequentialSearch.out && ./sequentialSearch.out
python3 sequential_search.py
```

`make clean`으로 빌드 산출물을 지웁니다.

## 내 코드를 남기고 싶다면

이 저장소에는 push 권한이 없습니다. codespace에서 고친 내용을 커밋하고
`git push`하면 VS Code가 **fork를 만들어 거기에 올릴지** 물어봅니다. 그렇게
하면 본인 저장소에 남습니다.

**과제와 개인프로젝트는 여기가 아니라
[algorithm-env](https://github.com/lec-algorithm/algorithm-env)에서 시작하세요.**
`Use this template`으로 본인 저장소를 만들면 같은 환경에서 깨끗하게 출발할 수
있습니다.

## 저장소 구조

주제 폴더는 진도에 맞춰 하나씩 추가됩니다. 폴더 이름은 강의 자료의 주제
슬러그와 그대로 일치합니다.

```plaintext
algorithm-code/
├── .devcontainer/  .vscode/          # Codespaces · 빌드 · 디버그 설정
├── compose.yml  Dockerfile  Makefile
└── src/
    └── topic-01-intro-search/
        └── 01_sequential_search/
            ├── README.md
            ├── sequentialSearch.pseudo   # 기준이 되는 절차
            ├── sequentialSearch.c        # C 구현
            └── sequential_search.py      # Python 구현
```

예제 하나가 폴더 하나입니다. 같은 폴더의 `.c`가 함께 빌드되므로 헤더를 나눠도
되지만, **한 폴더에 `main`은 하나만** 둡니다.

전체 주제 목록은 [강의 목록](https://lec-algorithm.github.io/lecture/course/)에
있습니다.

## 규약

- **의사코드(`.pseudo`)가 기준입니다.** C와 Python은 그것을 옮긴 것이고,
  같은 이름의 함수를 씁니다.
- **실행 파일은 `*.out`으로 만듭니다.** `.gitignore`가 그것만 걸러냅니다.
- **외부 라이브러리를 쓰지 않습니다.** C는 표준 라이브러리만, Python은 표준
  모듈만 씁니다.
- **왜 그렇게 되는지는 강의 자료가 설명합니다.** 여기 README와 주석은 어떻게
  돌리는가까지만 담습니다.

## 정리

```sh
docker compose down
```
