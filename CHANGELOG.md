# Changelog

이 저장소(강의 예제 코드)의 변경 기록. 형식은
[Keep a Changelog](https://keepachangelog.com/ko/1.1.0/)를 따른다.

주제는 진도에 맞춰 추가된다. 새 주제가 올라오면 여기에 적는다.

## [Unreleased]

### Added

- **주제 01의 첫 예제**: 순차 검색. `src/topic-01-intro-search/01_sequential_search/`에
  의사코드와 C·Python 구현이 들어 있다.
- 예제 하나가 폴더 하나인 구조를 정했다. 같은 폴더의 `.c`가 함께 빌드되므로
  한 폴더에 `main`은 하나만 둔다.

### Removed

- `algorithm-env`에서 온 버블 정렬 데모(`src/main.*`·`src/sort.*`·`tests/`).
  환경이 도는지 보여 주는 예제였고, 이 저장소에는 필요 없다. 그에 따라
  `Makefile`의 `run`·`test`·`debug` 타겟과 관련 작업도 뺐다. 이 저장소에는
  고정된 진입점이 없고, 실행은 열려 있는 파일을 대상으로 한다.

- 저장소 생성. [algorithm-env](https://github.com/lec-algorithm/algorithm-env)
  v1.0.0 template에서 만들었다.
- 실습 환경을 `algorithm-env` v1.0.3에 맞췄다. VS Code의 빌드·디버그 설정
  (`.vscode/`), `make debug`·`%.out` 규칙, 파일 하나만 디버그하는 구성,
  Code Runner의 ▶ 버튼이 들어왔다.
