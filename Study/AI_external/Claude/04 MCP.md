# MCP (Model Context Protocol)

[[AI_external/Claude/README|← Claude 활용법]] · [[01 Claude Code 동작 방식]]에서 이어짐

> 한 줄: **AI 도구 연결의 표준 규격 — "AI계의 USB-C".**
> [[01 Claude Code 동작 방식]]의 도구(Read/Bash 등)는 하네스에 내장된 것이고,
> MCP는 **외부의 도구·데이터를 표준 방식으로 꽂는** 프로토콜이다.

## 1. 왜 필요한가

MCP 이전에는 "Claude에 GitHub 연동", "GPT에 GitHub 연동"을 각각 따로 만들어야 했다 (N개 모델 × M개 서비스 = N×M 개발).
MCP는 Anthropic이 공개한 오픈 표준으로, 서비스가 **MCP 서버 하나**만 만들면
MCP를 지원하는 모든 클라이언트(Claude Code, Claude 앱, 타사 에이전트…)가 그걸 쓸 수 있다 (N+M).

## 2. 구조

```
┌─ 호스트 (Claude Code, Claude 앱 등) ─────────┐
│   MCP 클라이언트 ←──── 모델은 그냥 "도구"로 봄  │
└──────┬───────────────┬───────────────────────┘
       │ stdio          │ HTTP(S)
┌──────▼─────┐   ┌──────▼──────────┐
│ 로컬 서버   │   │ 원격 서버        │
│ (내 PC 프로 │   │ (mcp.linear.app,│
│  세스로 실행)│   │  GitHub MCP 등) │
└────────────┘   └─────────────────┘
```

MCP 서버가 제공하는 것 세 가지:
| 종류 | 설명 | 예 |
|---|---|---|
| **Tools** | 모델이 호출하는 함수 | `create_issue`, `search_docs` |
| **Resources** | 읽을 수 있는 데이터 | 파일, DB 레코드 |
| **Prompts** | 미리 정의된 프롬프트 템플릿 | 정형화된 리뷰 요청 |

핵심: 모델 입장에서 MCP 도구는 **내장 도구와 똑같은 tool call**이다.
차이는 실행 위치 — 내장 도구는 하네스가, MCP 도구는 연결된 서버가 실행.

## 3. Claude Code에서 쓰기

```sh
# 로컬 서버 추가 (stdio — 내 PC에서 프로세스로 실행됨)
claude mcp add my-server -- npx -y @some/mcp-server

# 원격 서버 추가 (HTTP)
claude mcp add --transport http linear https://mcp.linear.app/mcp

claude mcp list   # 확인
```

- 설정은 프로젝트 `.mcp.json`(팀 공유) 또는 유저 설정에 저장.
- 연결하면 그 서버의 도구들이 도구 목록에 추가되고, 권한 시스템도 동일하게 적용됨.
- MANAGE 같은 앱 개발에 응용 예: 브라우저 조작 MCP(Playwright 등)를 붙이면
  Claude Code가 실제 화면을 열고 클릭해서 UI를 검증할 수 있다.

## 4. API에서 쓰기 (MCP 커넥터)

내 코드에서 Messages API를 쓸 때도 원격 MCP 서버를 붙일 수 있다 (베타 `mcp-client-2025-11-20`).
서버 선언(`mcp_servers`)과 도구 활성화(`mcp_toolset`) **둘 다** 필요:

```python
client.beta.messages.create(
    model="claude-opus-4-8", max_tokens=1024,
    betas=["mcp-client-2025-11-20"],
    mcp_servers=[{"type": "url", "url": "https://example/mcp", "name": "example"}],
    tools=[{"type": "mcp_toolset", "mcp_server_name": "example"}],
    messages=[...],
)
```

이때 MCP 연결 자체를 Anthropic 서버가 대신 해준다 (내 코드는 선언만).

## 5. 주의점 (실전에서 자주 걸리는 것)

- **MCP 인증 토큰 ≠ 서비스 API 키.** 호스팅 MCP 서버(Notion, Linear 등)는 보통
  OAuth 베어러 토큰을 요구한다. Notion REST API 키(`ntn_...`)를 넣으면 안 됨 — 다른 인증 체계.
- 원격 MCP 서버에 보내는 데이터는 그 서버 운영자에게 간다 — 신뢰할 수 있는 서버만 연결.
- 도구가 많아지면 컨텍스트를 잡아먹는다 — 필요한 서버만 켜두기.

---
_관련: [[02 프롬프트 캐싱]] (도구 목록 변경 = 캐시 전체 무효화) · [[03 Agent SDK]]_
