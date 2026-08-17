# /btw 클로드

### `/btw` 란?

"By The Way"의 줄임말로, 2026년 3월 10일 Claude Code v2.1.72에서 추가된 공식 명령어예요. [Towards AI](https://pub.towardsai.net/mastering-claude-codes-btw-fork-and-rewind-the-context-hygiene-toolkit-5ceefa59623d?gi=49697b2c5651)

---

### 핵심 개념

대화 히스토리에 추가되지 않는 사이드 채널이에요. 질문하고 답변받아도 메인 대화 스레드에 흔적이 남지 않아요. [MindStudio](https://www.mindstudio.ai/blog/claude-code-btw-command-save-tokens)

컨텍스트를 소비하지 않고, 프롬프트 캐시를 재사용하기 때문에 추가 비용이 최소예요. [Youmind](https://youmind.com/landing/x-viral-articles/claude-code-commands-shortcuts-guide)

---

### 사용법

```
/btw [질문]
```

예시:

```
/btw calculate_metrics 함수가 뭘 리턴하는 거야?
/btw 아까 그 파일 이름이 뭐였지?
/btw idempotent가 정확히 뭔 뜻이야?
```

Claude Code가 작업 중일 때도 사용할 수 있어요. Space / Enter / Esc로 답변창 닫으면 원래 작업으로 복귀해요. [Youmind](https://youmind.com/landing/x-viral-articles/claude-code-commands-shortcuts-guide)

---

### 동작 원리

`/btw`는 현재 대화 전체를 볼 수 있지만 툴(파일 수정, bash 실행 등)은 사용할 수 없는 임시 에이전트를 생성해요. 창 닫으면 답변은 사라져요. [Howdoiuseai](https://www.howdoiuseai.com/blog/2026-03-12-the-simple-btw-command-that-cuts-claude-code-costs)

서브에이전트랑 정반대예요:

```
/btw        → 현재 대화 전체 보임 + 툴 없음
서브에이전트 → 빈 컨텍스트로 시작 + 툴 있음
```

---

### 쓰면 안 되는 경우

질문 결과가 메인 작업 방향을 바꿀 수 있는 거라면 메인 스레드에서 물어봐야 해요. 그냥 궁금한 거, 개념 확인, 뭔가 기억하고 싶을 때만 써요. [MindStudio](https://www.mindstudio.ai/blog/claude-code-btw-command-save-tokens)

---

### 토큰 절약 효과

긴 세션에서 자주 쓰면 총 토큰 소비를 최대 50%까지 줄일 수 있어요. 20,000토큰짜리 히스토리에 질문 하나 추가하면 그 질문도 20,000토큰 위에 쌓이는 구조인데, `/btw`는 그걸 우회해요.


## 



# /reload-skills , /reload-plugins

설정 새로고침 하는것 (추가하고 리스트보기전에 한번 해주는 용도)

참고로 v2.1.64 이상에서 지원하니 버전이 낮으면 claude --version 으로 확인하고
claude update 해준 다음 해보면 됨.


# /plugin
### `/plugin` 전체 명령어 레퍼런스

#### 기본 UI

```
/plugin
```

그냥 치면 4개 탭이 있는 플러그인 매니저 열려요:

- **Discover** — 마켓플레이스 탐색
- **Installed** — 설치된 플러그인 관리
- **f** 키로 즐겨찾기 추가 가능

---
#### 마켓플레이스 관리

```bash
# 마켓플레이스 추가
/plugin marketplace add anthropics/claude-plugins-community
/plugin marketplace add forrestchang/andrej-karpathy-skills

# 마켓플레이스 카탈로그 갱신
/plugin marketplace update claude-plugins-official
```

참고:
`claude-plugins-official`은 Claude Code 설치 시 자동 등록.

claude-plugins-community 
= **Anthropic 내부 리뷰 파이프라인의 읽기 전용 미러**. 매일 밤 동기화.

외부 개발자가 플러그인을 제출하면:

```
clau.de/plugin-directory-submission 에 제출
        ↓
자동 보안 스캔
        ↓
승인되면 특정 커밋 SHA로 고정해서 marketplace.json에 추가
        ↓
매일 밤 claude-plugins-community 레포에 동기화
```

SHA로 고정하기 때문에 
누군가 플러그인 소스를 악의적으로 변경해도 마켓플레이스에는 반영 안 됨.

```
anthropics/claude-plugins-communit ← Anthropic 운영 커뮤니티 마켓플레이스
        안에 있는 플러그인들: code-review, github, supabase 등등

forrestchang/andrej-karpathy-skills   ← 개인 개발자(forrestchang)가
                                        만든 별도 마켓플레이스
```

이런 차이가 있음. (`andrej-karpathy-skills`는 community에 제출 안 된 거예요. 그냥 개인 GitHub 레포를 마켓플레이스로 직접 등록하는 거임.)

참고로 `add` 자체는 아무것도 설치 안 해요. "여기서 플러그인 찾을 수 있다"고 알려주는 것뿐임.

---

#### 설치 / 제거

```bash
# 설치
/plugin install code-review@claude-plugins-official
/plugin install andrej-karpathy-skills@karpathy-skills

# 범위 지정 설치 (기본은 user)
/plugin install [플러그인]@[마켓] --scope user     # 전체 프로젝트 공통
/plugin install [플러그인]@[마켓] --scope project  # 현재 프로젝트만
/plugin install [플러그인]@[마켓] --scope local    # 로컬 전용 (git 제외)

# 제거
/plugin uninstall code-review@claude-plugins-official

# 제거 + 의존성 정리
/plugin uninstall [플러그인] --prune
```

---

#### 활성화 / 비활성화

```bash
# 비활성화 (삭제 없이)
/plugin disable code-review@claude-plugins-official

# 재활성화
/plugin enable code-review@claude-plugins-official
```

---

#### 정보 확인

```bash
# 플러그인 상세 정보
/plugin show andrej-karpathy-skills@karpathy-skills

# 고아 의존성 정리
/plugin autoremove
```

---

#### 리로드

```bash
/reload-plugins    # 전체 리로드 (v2.1.64+)
/reload-skills     # 스킬만 리로드 (v2.1.152+)
```


#