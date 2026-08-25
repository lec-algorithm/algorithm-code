# Changelog

이 저장소(실습 환경 template)의 변경 기록. 형식은
[Keep a Changelog](https://keepachangelog.com/ko/1.1.0/)를 따르고,
버전은 [유의적 버전](https://semver.org/lang/ko/)을 따른다.

## [1.0.0] - 2026-08-25

첫 릴리스. 2026-2 고급알고리즘의 실습 환경 template이다.
`Use this template`으로 사본을 만들어 과제와 개인프로젝트를 시작한다.

### Added

- **실습 컨테이너**: `debian:trixie-slim` 기반 이미지에 gcc 14.2, gdb, make,
  Python 3.13, git을 담았다. 서비스는 `lab` 하나이고, 저장소 폴더를 컨테이너의
  `/work`에 바인드 마운트한다. 이미지에 소스를 굽지 않으므로 호스트에서 고친
  코드가 바로 반영된다.
- **Codespaces 지원**: `.devcontainer/devcontainer.json`이 `compose.yml`의
  `lab` 서비스를 그대로 쓴다. 브라우저에서 열든 로컬에서 `docker compose`로
  띄우든 같은 컨테이너다.
- **`src`/`tests` 뼈대**: 버블 정렬을 C와 Python으로 같은 함수 이름으로 구현하고,
  각각에 유닛 테스트를 붙였다. C 테스트는 외부 프레임워크 없이 표준 C로만 쓴다.
- **`Makefile`**: `make run`으로 두 구현을 실행하고 `make test`로 두 테스트를
  돌린다. 테스트가 실패하면 0이 아닌 코드로 끝난다.
- **`.gitignore`**: 실행 파일은 `*.out`으로 만드는 규약을 두고 그것만 걸러낸다.
  컨테이너에서 컴파일한 Linux 바이너리가 커밋에 섞이지 않는다.

### Notes

- 이 저장소에는 **강의 예제 코드를 넣지 않는다.** 주제별 예제는
  [algorithm-code](https://github.com/lec-algorithm/algorithm-code)에 있고,
  교안과 슬라이드는 [lecture](https://github.com/lec-algorithm/lecture)에 있다.
- `src`의 버블 정렬은 환경이 도는지 보여 주는 예제다. 사본을 만든 뒤에는 지우고
  자기 코드를 넣으면 된다.

[1.0.0]: https://github.com/lec-algorithm/algorithm-env/releases/tag/v1.0.0
