# 예제 하나를 빌드한다. 이 저장소에는 고정된 진입점이 없고 주제 폴더마다
# 예제가 들어 있으므로, 대상 파일을 지정해서 부른다.
#
#   make src/topic-01-intro-search/01_sequential_search/sequentialSearch.out
#   make src/topic-01-intro-search/01_sequential_search/sequentialSearch.debug.out
#   make clean
#
# 편집기에서는 직접 칠 일이 거의 없다. 파일을 열고 ▶ 버튼(실행)이나
# F5(디버그)를 누르면 아래 규칙이 대신 불린다.
#
# 실행 파일은 `*.out`으로 만든다. .gitignore가 그것만 걸러내므로 컨테이너에서
# 컴파일한 Linux 바이너리가 커밋에 섞이지 않는다.

CC ?= gcc
CFLAGS ?= -std=c17 -Wall -Wextra -O2
# 디버그 빌드: 최적화를 끄고 심볼을 남긴다.
DEBUGFLAGS ?= -std=c17 -Wall -Wextra -g -O0

.PHONY: clean

# 파일 하나를 그 자리에서 빌드한다. 같은 폴더의 .c를 함께 링크하므로 헤더에
# 선언만 있고 구현이 옆 파일에 있어도 된다. 대신 **한 폴더에 main은 하나만** 둔다.
%.out: %.c
	$(CC) $(CFLAGS) -I$(@D) -o $@ $(wildcard $(@D)/*.c)

%.debug.out: %.c
	$(CC) $(DEBUGFLAGS) -I$(@D) -o $@ $(wildcard $(@D)/*.c)

clean:
	find . -name '*.out' -delete
	find . -name '__pycache__' -type d -prune -exec rm -rf {} +
