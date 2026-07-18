# Claude Agent SDK

[[AI_external/Claude/README|← Claude 활용법]] · [[01 Claude Code 동작 방식]]에서 이어짐

> 한 줄: **Claude Code를 라이브러리로 포장한 것.**
> [[01 Claude Code 동작 방식]]에서 본 하네스(에이전트 루프 + 내장 도구 + 권한 + 세션)를
> 내 프로그램 안에서 `query()` 한 줄로 돌릴 수 있다.

## 1. 에이전트를 만드는 4가지 방법 (헷갈리기 쉬운 지형도)

두 가지 질문으로 구분된다: **하네스(루프)를 누가 주는가** / **배포(인프라)를 누가 주는가**.

| # | 방법 | 내가 짜는 것 | 하네스 | 배포 | 내장 도구 |
|---|---|---|---|---|---|
| 1 | API 수동 루프 | `while stop_reason=="tool_use"` 루프 전부 | 나 | 나 | 없음 (내 도구만) |
| 2 | API **Tool Runner** | 도구 함수만 | SDK가 줌 | 나 | 없음 (내 도구만) |
| 3 | **Managed Agents** | 에이전트 설정만 | Anthropic | **Anthropic** (세션별 샌드박스) | bash·파일·코드실행 등 |
| 4 | **Claude Agent SDK** | 프롬프트 + 옵션 | Claude Code 하네스 | 나 | Read/Write/Edit/Bash/Glob/Grep/웹검색 + MCP + 서브에이전트 |

- **Tool Runner ≠ Agent SDK** — 이름이 비슷해서 자주 혼동됨.
  Tool Runner는 일반 API SDK(`anthropic` 패키지)의 헬퍼로, **내가 정의한 도구**의 호출 루프만 자동화.
  Agent SDK는 **별도 패키지**(`claude-agent-sdk`)로, Claude Code 전체(내장 도구·권한·훅·세션 포함)가 들어있다.
- Managed Agents만 배포까지 해줌 (서버에서 상시 실행, cron 스케줄, 세션별 컨테이너).
  나머지는 전부 내 인프라에서 돈다.

## 2. Agent SDK 기본 사용

```bash
pip install claude-agent-sdk        # Python
npm install @anthropic-ai/claude-agent-sdk   # TypeScript
```

```python
import anyio
from claude_agent_sdk import query

async def main():
    # query() 하나로 Claude Code와 동일한 에이전트 루프가 돈다:
    # 파일 읽기 → 수정 → 명령 실행을 알아서 반복
    async for message in query(prompt="이 저장소의 실패하는 테스트를 찾아서 고쳐줘"):
        print(message)

anyio.run(main)
```

옵션으로 제어할 수 있는 것 (Claude Code에서 봤던 개념들이 그대로 파라미터가 됨):

| 옵션 | Claude Code에서의 대응물 |
|---|---|
| `allowed_tools`, `permission_mode` | 권한 시스템 (`settings.json`의 allowlist) |
| `system_prompt`, `append_system_prompt` | CLAUDE.md / 시스템 프롬프트 |
| `mcp_servers` | `claude mcp add` ([[04 MCP]]) |
| `agents` (서브에이전트 정의) | Agent 도구 / `.claude/agents/` |
| hooks | 훅 (도구 호출 가로채기) |
| `cwd`, `max_turns` | 작업 디렉토리, 루프 상한 |
| 세션 resume | `--resume` / 세션 이어가기 |

## 3. 언제 뭘 쓰나

- **분류·요약·추출 등 단발 호출** → 그냥 Messages API. 에이전트가 과함.
- **내 앱의 자체 도구 몇 개로 도는 에이전트** (예: 사내 DB 조회 봇) → **Tool Runner**.
  도구 함수만 쓰면 루프는 SDK가 돌려주고, turn마다 개입(승인 게이트, 로깅)도 가능.
- **파일시스템·셸을 다루는 코딩/자동화 에이전트를 내 서버에서** → **Agent SDK**.
  예: CI에서 실패 로그 분석 후 수정 PR 만드는 봇, 리포지토리 감사 스크립트.
- **호스팅·스케줄·세션 관리까지 맡기고 싶다** → **Managed Agents** (베타).
  에이전트 설정을 등록해두면 Anthropic이 루프와 샌드박스를 돌려줌. cron 배포도 지원.

판단 기준 4가지 (에이전트가 정말 필요한가): 다단계이고 사전에 완전히 명세하기 어려운가 /
결과가 비용·지연을 정당화하는가 / Claude가 그 작업 유형에 능한가 / 오류를 잡고 복구할 수 있는가.
하나라도 "아니오"면 단발 호출이나 코드로 제어하는 워크플로우로 충분.

## 4. MANAGE와의 연결 (상상 연습)

지금은 내가 Claude Code를 **대화로** 조종하지만, Agent SDK를 쓰면:
- "매일 밤 MANAGE 저장소를 열어 TODO 주석을 수집해 리포트 생성" 같은 걸 **스크립트로** 자동화 가능
- manage-ui 스킬 같은 파일도 `system_prompt`에 주입해서 동일한 규칙으로 동작시킬 수 있음
- 단, 실행 비용(토큰)과 권한 설계는 내 몫

---
_관련: [[04 MCP]] · [[02 프롬프트 캐싱]] (에이전트 루프 비용의 핵심) · 공식 문서: code.claude.com/docs/en/agent-sdk_
