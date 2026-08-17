
# Primary Skills

## **[andrej-karpathy-skills](https://github.com/multica-ai/andrej-karpathy-skills)** 

https://github.com/multica-ai/andrej-karpathy-skills
Claude.md 한 장으로 화제가 되었던 그 스킬

### 설치 방법
[skills install README](https://github.com/multica-ai/andrej-karpathy-skills#install)

**Option A: Claude Code Plugin (recommended)**
From within Claude Code, first add the marketplace:

```
/plugin marketplace add forrestchang/andrej-karpathy-skills
```

Then install the plugin:

```
/plugin install andrej-karpathy-skills@karpathy-skills
```

This installs the guidelines as a Claude Code plugin, making the skill available across all your projects.

=> 그냥 터미널 전역에서 claude 열고 두개 붙여넣기 하면 됨.
/plugin 의 원리는 [[05 Claude Extension Understanding]] 참조

(참고 : user scope 로 설치하면 아예 모든 claude.md에 자동 적용됨.)
![[Pasted image 20260716012905 1.png]]

바로 나옴!


**Option B: CLAUDE.md (per-project)**

New project:

```shell
curl -o CLAUDE.md https://raw.githubusercontent.com/forrestchang/andrej-karpathy-skills/main/CLAUDE.md
```

Existing project (append):

```shell
echo "" >> CLAUDE.md
curl https://raw.githubusercontent.com/forrestchang/andrej-karpathy-skills/main/CLAUDE.md >> CLAUDE.md
```



---

### andrej karpathy 지적 문제점

- "모델들은 개발자를 대신해 잘못된 가정을 내리고, 이를 검증하지 않은 채 그대로 진행해 버립니다. 혼란스러운 부분을 스스로 관리하지 못하고, 명확한 설명을 요구하지 않으며, 모순점을 드러내거나 트레이드오프를 제시하지도 않고, 정작 밀어붙여야 할 때 그러지 못합니다."

- "코드와 API를 과도하게 복잡하게 만들고, 불필요한 추상화를 남발하며, 쓰이지 않는 코드(dead code)를 정리하지 않는 경향이 있습니다. 100줄이면 충분할 것을 1000줄짜리 비대한 구조로 구현하곤 합니다."

- "본래 작업과 직접적인 관련이 없더라도, 충분히 이해하지 못한 기존 주석이나 코드를 사이드 이펙트로 변경하거나 삭제해 버리는 일이 여전히 발생합니다."


### 해결책 (4대 원칙)

이러한 문제들을 직접 해결하기 위해 하나의 파일에 담은 4가지 원칙입니다.

| **원칙**                                   | **해결하는 문제**                      |
| ---------------------------------------- | -------------------------------- |
| **코딩 전 생각하기 <br>(Think Before Coding)**  | 잘못된 가정, 숨겨진 혼란, 누락된 트레이드오프       |
| **단순함 최우선 (Simplicity First)**           | 과도한 복잡성, 비대한 추상화                 |
| **외과 수술식 변경 <br>(Surgical Changes)**     | 무관한 코드 수정, 건드리지 말아야 할 코드 수정      |
| **목표 중심 실행 <br>(Goal-Driven Execution)** | 테스트 우선주의 및 검증 가능한 성공 기준을 통한 레버리지 |

### 4대 원칙 상세 안내

### ① 코딩 전 생각하기 (Think Before Coding)

> **가정하지 마세요. 혼란을 숨기지 마세요. 트레이드오프를 제시하세요.**

LLM은 종종 혼자서 특정 해석을 내리고 바로 실행에 옮깁니다. 
이 원칙은 명확한 추론을 강제합니다.

- **가정을 명시적으로 밝히기** — 확실하지 않다면 추측하지 말고 질문하세요.

- **다양한 해석 제시하기** — 모호함이 존재할 때 혼자서 결정을 내리지 마세요.

- **필요할 땐 반론 제기하기** — 더 간단한 접근 방식이 있다면 제안하세요.

- **헷갈릴 땐 멈추기** — 불명확한 부분을 명시하고 명확한 설명을 요구하세요.


### ② 단순함 최우선 (Simplicity First)

> **문제를 해결하는 최소한의 코드만 작성하세요. 추측성 코드는 금지합니다.**

과도한 엔지니어링(오버엔지니어링) 경향에 맞섭니다.

- 요청받지 않은 기능은 추가하지 않습니다.

- 단발성 코드에 추상화를 적용하지 않습니다.

- 요청받지 않은 "유연성"이나 "설정 가능성(Configurability)"은 배제합니다.

- 일어날 수 없는 시나리오에 대한 예외 처리는 하지 않습니다.

- 200줄짜리 코드가 50줄로 줄어들 수 있다면, 다시 작성하세요.

- _테스트 방법:_ "시니어 엔지니어가 보기에 이 코드가 과하게 복잡한가?" 만약 그렇다면 단순화하세요.


### ③ 외과 수술식 변경 (Surgical Changes)

> **반드시 건드려야 하는 곳만 수정하세요. 본인이 만든 흔적만 정리하세요.**

기존 코드를 수정할 때:

- 주변 코드, 주석, 포맷팅을 "개선"하려고 하지 마세요.

- 망가지지 않은 부분을 리팩토링하지 마세요.

- 본인의 스타일과 다르더라도 기존 스타일을 따르세요.

- 무관한 데드 코드를 발견했다면 언급만 하고, 직접 삭제하지 마세요.


내가 변경함으로써 발생한 잔재:

- 본인의 수정 사항으로 인해 더 이상 사용되지 않게 된 `import`, 변수, 함수는 제거합니다.

- 요청받지 않았다면 기존에 존재하던 데드 코드는 건드리지 마세요.

- _테스트 방법:_ 변경된 모든 라인은 사용자의 요청 사항과 직접 연결되어야 합니다.


### ④ 목표 중심 실행 (Goal-Driven Execution)

> **성공 기준을 정의하세요. 검증될 때까지 루프를 도세요.**

명령형 작업을 검증 가능한 목표로 전환합니다.

- "유효성 검사 추가해줘" $\rightarrow$ **"유효하지 않은 입력에 대한 테스트를 작성하고, 이를 통과시키세요."**

- "버그 고쳐줘" $\rightarrow$ **"버그를 재현하는 테스트를 작성하고, 이를 통과시키세요."**

- "X를 리팩토링해줘" $\rightarrow$ **"리팩토링 전후로 테스트가 통과하는지 확인하세요."**


### 핵심 인사이트

> "LLM은 특정 목표를 달성할 때까지 반복(looping)하는 작업에 매우 뛰어납니다... 무엇을 해야 하는지 일일이 지시하지 말고, **성공 기준**을 제시한 뒤 지켜보세요." — 안드레 카파시

=> 심층 사고는 결국 맥락을 전체적으로 던져줘야 하므로.. 토큰 소모는 크고 세부적인 요소를 놓칠 가능성이 커질 수밖에 없음. (compaction 하는 과정에서 정밀한 정보는 사라질 수밖에 없기에) => 단기목표 sprint 하듯 짧은단위의 task 를 잡고 looping 시키는것으로 효율성 높이기.



# 
# HOT ISSUE / REPO

## Multica-ai

https://github.com/multica-ai 

그냥 가볍게 정리

[andrej-karpathy-skills](https://github.com/multica-ai/andrej-karpathy-skills) 도 여기거임, 
https://github.com/multica-ai/multica 

### Multica가 뭐냐

한 줄로 — **AI 에이전트를 팀원처럼 관리하는 오픈소스 플랫폼**

Claude Code, Codex, Gemini 같은 에이전트한테 GitHub 이슈 던지듯 태스크 할당하면, 에이전트가 알아서 코드 짜고, 진행상황 보고하고, 막히면 블로커 리포트해요. 사람 팀원이랑 같은 보드에서 같이 일하는 구조.

---

### 핵심 기능

- 이슈 만들고 에이전트한테 assign → 알아서 실행
- 실시간 진행상황 WebSocket 스트리밍
- **Skills 시스템** — 한 번 해결한 작업이 재사용 가능한 스킬로 누적됨
- Claude Code, Codex, Gemini, Cursor Agent 등 멀티 에이전트 지원
- 로컬 데몬 + 클라우드 런타임 통합

## MCP : playwright

적어두고 
카카오톡, 옵시디언, x 등에대한 MCP 하나하나 시도..