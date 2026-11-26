# 시험 전 연습 3개

| 순서 | 문제지 | 폴더 | 채울 파일 | 확인 (폴더 안에서) |
|:-:|---|---|---|---|
| 1 | `project/Problem 1.pdf` (파일 서버) | `project/` | `src/common.c`, `src/chunkd.c` | `make chunkd && bash test1.sh` |
| 2 | `Problem 3.pdf` (소수 세기 서버) | `problem3/` | `src/main.c` | `make && bash test.sh` |
| 3 | `project/Problem 2.pdf` (다운로드 클라이언트) | `project/` | `src/pget.c` | `make pget && bash test2.sh` |

- 3번은 순서 출력을 생략해도 됨 (테스트는 FAIL 이 나옴. 파일 내용이 같고 `done:` 이 나오면 충분)
- 시간이 없으면 1 → 2 까지만

`problem1/` (푼 것), `problem2/` : 기존 mock. 문제지는 이 폴더의 `Problem 1.pdf`, `Problem 2.pdf`, 확인은 `bash extra/_채점/test1.sh` (`test2.sh`). ③은 `project/` 와 `problem2/` 중 한 곳에서만 풀면 됨

`extra/` : 예전 P3·P4, 정답(`extra/_정답/`, 2번은 `prime/`), Test 1 복습. `extra/queue_server/` 는 어려워서 뺀 문제
