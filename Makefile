# 빌드와 테스트를 한 단어로 돌리기 위한 Makefile.
# 컨테이너 안에서 실행한다 (docker compose exec lab bash).
#
#   make run     예제 실행 (C, Python)
#   make test    유닛 테스트 (C, Python)
#   make debug   디버그 심볼을 넣어 빌드 (VS Code의 F5가 쓴다)
#   make clean   빌드 산출물 정리
#
# 실행 파일은 `*.out`으로 만든다. .gitignore가 그것만 걸러낸다.

CC ?= gcc
CFLAGS ?= -std=c17 -Wall -Wextra -O2 -Isrc
# 디버그 빌드: 최적화를 끄고 심볼을 남긴다 (VS Code의 F5가 이 결과물을 쓴다).
DEBUGFLAGS ?= -std=c17 -Wall -Wextra -g -O0 -Isrc

.PHONY: all run run-c run-py test test-c test-py debug clean

all: test

run: run-c run-py

run-c: src/main.out
	@./src/main.out

run-py:
	@python3 src/main.py

test: test-c test-py

test-c: tests/test_sort.out
	@./tests/test_sort.out

test-py:
	@python3 -m unittest discover -s tests -v

debug: src/main.debug.out

src/main.out: src/main.c src/sort.c src/sort.h
	$(CC) $(CFLAGS) -o $@ src/main.c src/sort.c

src/main.debug.out: src/main.c src/sort.c src/sort.h
	$(CC) $(DEBUGFLAGS) -o $@ src/main.c src/sort.c

tests/test_sort.out: tests/test_sort.c src/sort.c src/sort.h
	$(CC) $(CFLAGS) -o $@ tests/test_sort.c src/sort.c

clean:
	rm -f src/*.out tests/*.out
	rm -rf src/__pycache__ tests/__pycache__
