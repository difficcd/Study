# GitHub 협업 워크플로우 — git 기초만 아는 사람을 위한

`add / commit / push`만 써봤다면 여기서 시작하면 됩니다.

---

## 0. 큰 그림

혼자 작업할 때:
```
코드 수정 → add → commit → push       (main에 바로)
```

실무:
```
이슈 등록 → 브랜치 → 커밋 여러 개 → PR → CI 통과 → 리뷰 → merge → 브랜치 삭제
```

**혼자인데 왜 이 귀찮은 걸?** 솔로에서도 실제로 남는 이득:

| 얻는 것           | 설명                                                      |
| -------------- | ------------------------------------------------------- |
| **되돌리기 단위**    | main에 직접 커밋하면 "이 기능만 빼기"가 어렵습니다. PR은 통째로 revert 가능      |
| **프리뷰 배포**     | Vercel은 **PR마다 별도 URL**을 만듭니다. main을 안 건드리고 실제로 써볼 수 있음 |
| **CI 게이트**     | 테스트 깨진 코드가 main에 못 들어옵니다                                |
| **왜의 기록**      | PR 본문 = 배경·측정값·트레이드오프. 6개월 뒤의 나에게                       |
| **에이전트 지시 단위** | "이슈 #12 해줘"가 됩니다. 범위가 고정돼 스코프 크립이 줄어듦                   |

---

## 1. `gh` — GitHub을 터미널에서

```
git  = 내 컴퓨터의 히스토리 관리 (GitHub 몰라도 동작)
gh   = github.com과 대화 (이슈, PR, 리뷰, 릴리스, Actions)
```

### 설치와 로그인

```powershell
winget install --id GitHub.cli
```

**설치 후 열려 있던 터미널은 PATH가 갱신되지 않습니다.** 새 터미널에서:

```powershell
gh auth login      # GitHub.com → HTTPS → 브라우저 인증
gh auth status     # 확인
```

---

## 2. 이슈 (Issue) — 할 일을 문서로

번호가 붙고, 검색되고, PR과 연결됩니다.

```powershell
gh issue list
gh issue create                          # 템플릿 선택 → 에디터
gh issue create --title "..." --body "..."
gh issue view 12
gh issue view 12 --web
```

**좋은 이슈:** 증상 / 재현 방법 / 기대 동작 / (있으면) 원인 추정

에이전트에게 시킬 거라면 **재현 절차가 곧 스펙**입니다.

> **관례: 이슈·PR·커밋은 영어로.** 공개 저장소의 기본이고, 나중에 남이 보거나
> 도구가 파싱할 때 편합니다. 코드 주석도 마찬가지.

### 라벨

```powershell
gh label list
gh label create perf --description "속도, 메모리" --color FBCA04
gh issue create --title "..." --label bug,perf
```

이 저장소 라벨: `bug` `enhancement` `refactor` `perf` `test` `deploy` `ux` `documentation`

---

## 3. 브랜치 (Branch) — 작업 격리

main을 건드리지 않는 평행 세계입니다.

```powershell
git switch -c fix/12-text-size-focus   # 만들고 이동
git switch main                        # 이동
git branch                             # 목록
```

> `git switch`는 최신 명령. 옛 자료의 `git checkout -b`와 같습니다.

**이름 규칙:** `fix/` `feat/` `refactor/` `perf/` `docs/` `chore/`
이슈 번호를 넣으면 추적이 쉽습니다.

**브랜치는 짧게.** 오래 살수록 main과 벌어져 병합이 아파집니다.

---

## 4. 커밋 단위 — 제일 헷갈리는 것

**하나의 커밋 = 하나의 논리적 변경.** 파일 개수나 줄 수가 아닙니다.

**판단 기준: 이 커밋만 revert했을 때 말이 되는가?**

이 프로젝트 규칙: 100줄 이상 변경 또는 의미 있는 기능 하나.

```
<type>: <명령형 한 줄>

<왜 이렇게 했는지. 무엇을 했는지는 diff가 말해준다.>
```

type: `feat` `fix` `refactor` `perf` `docs` `test` `chore`

성능 변경이면 **측정값**을 본문에. 6개월 뒤 그게 유일한 근거입니다.

---

## 5. PR (Pull Request)

"이 브랜치를 main에 넣어주세요" + 토론 공간 + 리뷰 단위.

```powershell
git push -u origin HEAD
gh pr create --fill              # 커밋 메시지로 채움
gh pr status / list
gh pr checks                     # CI 결과
gh pr view --web                 # 프리뷰 URL 확인
```

### `Closes #12`

PR 본문에 쓰면 **merge될 때 이슈가 자동으로 닫힙니다.** (`Fixes`/`Resolves`도 동일)

### 병합 방식

```powershell
gh pr merge --squash --delete-branch
```

| 방식 | 결과 | 언제 |
|---|---|---|
| `--squash` | 커밋들을 **하나로 합쳐** main에 | **기본.** main 히스토리가 깔끔 |
| `--merge` | 그대로 + 병합 커밋 | 커밋 단위가 이미 잘 나뉜 큰 PR |
| `--rebase` | main 위에 다시 얹음 | 병합 커밋을 싫어하는 팀 |

솔로라면 squash가 거의 항상 정답입니다. 작업 중 지저분한 커밋이 main에 안 남습니다.

병합되면 커밋 제목에 `(#1)`이 붙어 **PR과 영구히 연결**됩니다.

---

## 6. CI — 자동 검사

`.github/workflows/*.yml`. 이 저장소는 push/PR마다 `npm run check`
(타입체크 → 테스트 → 훅 기준선 → 빌드)를 돌립니다.

```powershell
gh run list
gh run watch
gh pr checks
```

**진짜 가치:** "내 컴퓨터에선 되는데"를 없앱니다.

---

## 7. Vercel과 PR — 프리뷰 배포

- **main에 merge** → 프로덕션 URL 갱신
- **PR을 열면** → 그 PR 전용 프리뷰 URL 자동 생성

**병합 전에 진짜 배포본으로 테스트할 수 있습니다.** 태블릿에서 열어보기에도 좋습니다.

> ⚠️ **이 앱 주의:** 서버 저장(`/api`)은 로컬 Express(:8787)를 씁니다. Vercel엔 없으므로
> 배포본에서 서버 저장은 동작하지 않습니다 (로컬 `.emv`·자동저장은 정상).
> 백엔드는 Render, DB는 TiDB로 따로 갈 계획 → 이슈 #2로 등록됨.

---

## 8. README와 문서

읽는 사람 세 부류: **처음 온 사람 / 써보려는 사람 / 고치려는 사람.**
이 저장소는 세 번째를 `ARCHITECTURE.md`로 분리했고, README는 1·2에 집중합니다.

**⚠️ 문서가 틀리면 없느니만 못합니다.** 이 저장소의 `ARCHITECTURE.md`는 한동안
캔버스를 854×480(실제 1920×1080)이라 적고, 이미 있는 비트맵 수집기를 없다고 적어뒀습니다.

---

## 9. 실전 한 바퀴

```powershell
gh issue create                                  # 1. 무엇을 왜
git switch -c fix/13-server-save-offline         # 2. 브랜치
npm run check                                    # 3. 작업 → 검증
git add -A; git commit -m "fix: ..."
git push -u origin HEAD                          # 4. push
gh pr create --fill                              #    PR (본문에 Closes #13)
gh pr checks                                     # 5. CI
gh pr view --web                                 #    프리뷰로 실제 테스트
gh pr merge --squash --delete-branch             # 6. 병합
git switch main; git pull                        # 7. 정리
```

---

## 10. 자주 쓰는 명령

```powershell
# 상태
git status                 # 지금 뭐가 바뀌었나
git log --oneline -10
git diff                   # add 안 한 변경
git diff --staged          # add한 변경

# 브랜치
git switch -c feat/x
git switch main
git branch -d feat/x       # 삭제 (병합된 것만)

# 되돌리기
git restore <파일>          # 변경 취소 (⚠️ 복구 불가)
git restore --staged <파일> # add만 취소
git revert <커밋>           # 되돌리는 새 커밋 (안전, push한 것에 사용)
git reset --hard <커밋>     # ⚠️ 히스토리 되감기. push한 것엔 금지

# gh
gh issue list / create / view
gh pr create / checks / merge
gh run list / watch
gh browse                  # 저장소를 브라우저로
```

**위험한 것 둘:** `git reset --hard`, `git push --force`.
**이미 push한 것에는 쓰지 마세요.** 되돌릴 땐 `git revert`.

---

*모르는 명령은 `gh <명령> --help`, `git help <명령>`.
`gh browse`로 눈으로 확인하는 게 제일 빠를 때가 많습니다.*
