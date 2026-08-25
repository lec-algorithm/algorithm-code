# CLAUDE.md

2026-2 고급알고리즘(SIT2001-01)의 **강의 예제 코드 저장소**. 이 문서는 이
저장소에서 작업하는 AI 도구를 위한 가이드다.

## 이 저장소의 범위

- 여기는 **주제별 강의 예제**를 담는다. 학기 중에 주제가 하나씩 추가된다.
- **수강생은 이 저장소를 복사하지 않는다.** 여기서 바로 codespace를 만들거나
  클론해서 `git pull`로 새 주제를 받는다. 사본을 만들면 갱신을 받을 수 없다.
  자료의 안내가 그렇게 되어 있는지 확인한다.
- 과제·개인프로젝트의 출발점은 `algorithm-env`(template)다. 교안·슬라이드는
  `lecture` 저장소다.
- **설명을 길게 쓰지 않는다.** README는 "어떻게 돌리는가"까지만 담고,
  "왜 이렇게 되는가"는 강의 자료가 맡는다.
- 환경 파일(`compose.yml`·`Dockerfile`·`Makefile`·`.devcontainer`·`.vscode`)은
  `algorithm-env`에서 온 것이다. 고쳐야 할 일이 생기면 **`algorithm-env`를 먼저
  고치고** 여기로 가져온다. 반대 방향으로 흐르게 두지 않는다.

## 구조와 규약

```plaintext
src/    tests/                 환경 확인용 예제 (algorithm-env에서 온 버블 정렬)
topic-NN-<슬러그>/             주제별 예제 (추가 예정)
```

주제 폴더 이름은 `lecture` 저장소의 주제 슬러그와 **글자 그대로 일치**한다.
대응표는 `lecture` 저장소의 `docs/course-plan.md`에 있다.

- **외부 라이브러리를 쓰지 않는다.** C는 표준 라이브러리만, Python은 표준
  모듈만. C 테스트도 프레임워크 없이 직접 쓴다. 이미지에 무언가를 더 깔아야
  하는 코드는 넣지 않는다.
- **실행 파일은 `*.out`으로 만든다.** `.gitignore`가 그것만 걸러낸다.
  확장자 없는 이름으로 컴파일하면 Linux 바이너리가 커밋에 섞인다.
- C와 Python은 같은 알고리즘을 같은 이름의 함수로 구현한다. 파일명은 각
  언어의 관례를 따른다(C는 camelCase, Python은 snake_case).
- **컴파일 경고 없이 빌드되어야 한다.** `CFLAGS`에 `-Wall -Wextra`가 켜져
  있다. 수강생이 복사할 저장소이므로 경고를 남긴 채 커밋하지 않는다.

## 실행 환경

수강생이 설치하는 것은 없다. Codespaces가 주 경로이고, 로컬은 Git과 Docker만
있으면 된다. 컴파일러와 Python은 이미지 안에 있다.

```sh
docker compose up -d
docker compose exec lab bash
make test
```

- 서비스는 `lab` 하나다. 컨테이너를 나누지 않는다.
- 저장소 폴더가 컨테이너의 `/work`에 바인드 마운트된다. **이미지에 소스를
  굽지 않는다.** 수강생이 고쳐 가며 쓰는 코드라 마운트가 단일 원본이어야 한다.
- `.devcontainer/devcontainer.json`은 별도 이미지를 정의하지 않고
  `compose.yml`의 `lab` 서비스를 그대로 쓴다. 컨테이너 정의의 단일 원본은
  `compose.yml`이다. 이미지에 무언가 추가할 일이 생기면 `Dockerfile`만 고친다.

## Git 워크플로: git flow

- 새 작업은 `develop`에서 `feature/*` 브랜치를 만들어 시작한다.
  `main`에는 직접 커밋하지 않는다.
- 한 커밋에는 한 가지 주제만 담는다.
- **커밋 전에 `make test`가 통과해야 한다.** C와 Python 두 구현이 같은
  결과를 내야 한다.
- 커밋 메시지 제목은 영어 명령형 한 줄
