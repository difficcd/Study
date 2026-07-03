---
id: agent-basics
title: 에이전트 기초 (Agentic AI) 정리본
tags: [agent, agentic-ai, react, mcp, a2a, multi-agent]
created: 2026-06-24
updated: 2026-06-24
status: active
links: [multi-agent-setup, knowledge-base-design]
---

# 에이전트 기초 (Agentic AI) — 정리본

<small>04 에이전트 트랙 · Lesson 1</small>

## 1. 에이전트란?

**1 ) 정의**
- **AI 에이전트(agent)** : 환경을 **인식(Perceive)** 하고, **계획(Plan)** 을 세우며, **도구(Tool)** 를 활용해 **자율적으로 목표를 달성**하는 소프트웨어 시스템
- 맡겨진 일을 자율적으로 처리 — LLM을 **Brain**으로 활용(LLM 기반 에이전트)

**2 ) 핵심 4요소** (정의 안에 다 들어있음)
- ① LLM을 두뇌로 ② 도구(외부 프로그램) 사용 ③ 문제해결 계획수립(planning) ④ 에이전트 간 협력(다중 에이전트)

### # 일반 LLM vs Agentic LLM

| 구분 | 일반 LLM (챗봇) | Agentic LLM (에이전트) |
|---|---|---|
| 기본 동작 | 한 번의 응답 생성 | **여러 단계**를 거쳐 작업 수행 |
| 외부 도구 | 불가/제한적 | **가능** (검색·계산·코드실행·API) |
| 문제 해결 | 입력에 **반응**(단발성) | 목표 세우고 **능동적** 진행 |
| 실행 흐름 | 단발성 | **목표 달성까지 반복 루프** |

> 한 줄 요약 : *일반 LLM에 **도구 사용 + 다단계 추론 + 루프**를 더한 것* = Agentic LLM.

## 2. 에이전트의 형태 (고전 → LLM 기반 진화)

| 형태 | 핵심 | 특징 |
|---|---|---|
| Simple Reflex | 현재 입력 → 조건-행동 규칙 | 과거·상태 무시 |
| Model-based Reflex | 내부 **세계 모델(상태)** 유지 | 부분 관측 대응 |
| Goal-based | **목표** 향해 행동 선택 | 탐색·계획 필요 |
| Utility-based | 목표 + **효용(좋음의 정도)** | 트레이드오프 판단 |
| **LLM-based (Agentic LLM)** | LLM 두뇌 + 도구 + 다단계 추론 | 우리가 다루는 형태(OpenClaw) |

> 고전 AI의 Reflex/Goal/Utility 능력을 **LLM 한 덩어리로 통합**한 것이 오늘날의 에이전트. 새 개념 아님.

## 3. 에이전트의 5대 구성요소

**1 ) 추론엔진 (LLM)** — 자연어 이해·추론·생성. 전체의 "두뇌".

**2 ) 도구 사용 (Tool Use)**
- 웹검색·코드실행·파일 I/O·API 호출 등 외부 기능
- 각 도구를 **JSON Schema**로 정의해 호출 (= function calling)
- **도구목록(tool list)** : 실행진입점(함수명)·매개변수·문서 → 에이전트의 활동 범위 결정

**3 ) 메모리 (Memory)** — 과거 입력·사고·행동의 이력
- **단기(short-term)** : 현재 작업 맥락 / **장기(long-term)** : 이력·취향

**4 ) 계획수립 (Planning)** — 복잡한 목표 → 하위 태스크 분해, 실행 순서·의존성 결정 (§4)

**5 ) 실행환경 (Runtime)** — perceive→plan→act **루프 제어**, 이벤트 큐·비동기·오류복구 (§6)

> **도구가 없으면** 에이전트는 "말만 하는 챗봇"으로 추락. 도구 = 행동력의 원천.

## 4. 계획수립 (Planning)

**1 ) 계획수립 프롬프트의 예**
- 시스템: "작업을 해결할 **계획**을 제안하라. 사용 가능 도구 5개:
  `get_today_date()`, `fetch_top_products(start,end,n)`, `fetch_product_info(name)`, `generate_query(history, output)`, `generate_response(query)`. 계획은 **유효한 작업들의 순서**여야 함."
- 예) "지난주 최다 판매 제품?" → `[fetch_top_products, generate_query, generate_response]`

**2 ) 계획과 실행의 분리** (안전 설계)
- ① **Planner** : 계획 생성
- ② **Evaluator** : 계획 검증(위험·타당성)
- ③ **Executor** : **검증된 계획만** 실행

> 왜 분리? 잘못된 행동을 **실행 전에** 거르기 위해. → 우리 "시드 10만원" 실험에선 **사람이 Evaluator**(결제 전 승인).

## 5. 추론 패턴 (Reasoning Pattern)

| 패턴 | 아이디어 | 적합 |
|---|---|---|
| **ReAct** (Reason+Act) | 추론↔행동 반복(도구 호출) | 도구 쓰는 에이전트 기본 |
| **CoT** (Chain-of-Thought) | "차근차근" 단계적 추론 | 단순·논리 문제 |
| **ToT** (Tree-of-Thoughts) | 가설을 **트리**로 탐색 | 복잡·창의(비용↑) |
| **MCTS** | Monte Carlo Tree Search | 게임·최적화(평가함수 필요) |

**# ReAct 루프** (자율성의 심장) : `목표 → ①Reason → ②Act(도구) → ③Observe → ④달성?(아니오→①)`

**# ReAct 실제 흐름 예**
```
Thought   '날씨 API를 먼저 호출하자'
Action    get_weather(city='Seoul')
Observation {'temp':22, 'sky':'맑음'}
Thought   '결과로 답을 만들자'
Answer    '서울은 22도, 맑음입니다.'
```

## 6. 실행환경 (Runtime)

**1 ) 정의** — LLM 호출·도구 실행·메모리 관리·오류처리·**루프 제어**를 조율하는 인프라

**2 ) 주요 요소**
- 이벤트 시스템(event loop, queue) · 컨텍스트 관리 · 라우팅
- 도구 사용 = **MCP**(§8) · 능력 확장 = Skills·Hooks · 협업 = 외부 채널·**A2A/ACP**(§9)

**3 ) 로컬 실행의 장점** (← OpenClaw가 로컬인 이유)
- **환경 접근성** : 이메일·로컬파일·터미널 직접 접근 → 실제 업무 수행
- **보안** : 로컬 실행 → 민감정보 보호 유리
- **지속성** : 며칠짜리 장기 프로젝트도 상주 실행

## 7. 단일 vs 다중 에이전트 (Multi-Agent)

- **단일 에이전트** : 혼자 작업
- **다중 에이전트(MAS)** : 복수의 **상호작용**하는 에이전트, **환경 공유** + 역할 분담 (예: Researcher + Router + Executor)

> 너의 목표와 직결 — ollama+Claude+Codex+Gemini 역할 분담 예) **Planner=Claude / Coder=Codex / 로컬요약=ollama / 크로스체크=Gemini**. 이들을 잇는 표준이 §8·§9.

## 8. 통신 ① MCP (Model Context Protocol) — 수직 연결

**1 ) 개요** — **Anthropic** 공개 오픈 표준(2024.11). LLM 앱 ↔ 외부 데이터·도구 연결을 표준화. 비유: **USB-C**.

**2 ) 구성**
- **Host** : LLM 구동 환경(Claude Desktop, Cursor, Codex…)
- **Client** : Host 내부, 서버와 1:1 연결 관리
- **Server** : 기능 노출 경량 프로세스 → **Tools / Resources / Prompts** 제공

**3 ) 채널** — `stdio`(로컬 spawn 상주) · `SSE`(HTTP 푸시)

```
Host(Claude Desktop/Codex/Cursor)
 └ MCP Client  ↕ JSON-RPC 2.0 (stdio/SSE)
 MCP Server ├ Tools(함수실행) ├ Resources(데이터) └ Prompts(템플릿)
```
> Claude Code 등록 예: `claude mcp add weather python /path/weather_server.py` → `claude mcp list`

## 9. 통신 ② A2A + 비교 — 수평 연결

**1 ) A2A (Agent-to-Agent, Google, 2025.4)**
- 서로 다른 조직/플랫폼 에이전트 간 **P2P 통신** 표준
- 각 에이전트가 `/.well-known/agent.json`에 **Agent Card** 게시 (이름·설명·capabilities·엔드포인트·보안·skills)

**2 ) MCP vs A2A**

| | 연결 방향 | 잇는 대상 |
|---|---|---|
| **MCP** | **수직** ↕ | AI ↔ 도구/데이터 |
| **A2A** | **수평** ↔ | AI ↔ AI 에이전트 |

> 경쟁이 아니라 **보완** : 에이전트는 MCP로 도구를 쓰고, A2A로 다른 에이전트와 협업.

## 10. 개발 프레임워크

**1 ) LangChain** — LLM 앱 빌드 구성요소 모음(오픈소스). 여러 작업을 **체인(chain)** 으로 연속 수행.
- RAG 부품 : Document Loader · Splitter · **Vector DB**(임베딩 검색) · Memory

**2 ) LangGraph** — 상태 유지 **다중 에이전트** 앱. **그래프**로 워크플로우 설계.
- **노드**=LLM/도구, **에지**=통신 채널. 각 단계 후 **상태 자동 저장**.

> 우리 트랙: 04에서 작은 ReAct 에이전트 직접 제작 → 06에서 OpenClaw로 완성형 운전. 프레임워크는 그 중간 다리.

## 11. 자율성의 양날 — 가드레일

- **강점** : 사람 개입 없이 장기 작업 완수, 도구로 실제 세계에 영향
- **위험** : 잘못된 판단도 **자율 실행**(파일 삭제·오결제·정보 유출·비용 폭주)
- **가드레일(필수)** : ① 격리(VM) ② 지출 하드리밋 ③ 휴먼 승인(=Evaluator) ④ 합법/ToS ⑤ 전량 로깅

> §11 가드레일 = §4 "계획-실행 분리"의 현실판. 안전은 나중에 붙이는 게 아니라 **구성요소**.

## 12. OpenClaw = 모든 개념의 제품화

| 에이전트 이론 | OpenClaw에서 |
|---|---|
| 추론엔진(LLM) | Claude / GPT / **ollama(로컬)** 선택 |
| 도구(Tool Use) | 셸 · 파일 · **브라우저 자동화** · 스킬 |
| 메모리 | 지속 메모리(단기+장기) |
| 실행환경(Runtime) | 24/7 백그라운드 + cron + 이벤트 |
| 도구 연결(MCP) | MCP 서버로 기능 확장 |
| 다중 에이전트(A2A) | 여러 모델·에이전트 역할 분담 |
| 인터페이스 | 카톡·텔레그램·디스코드 |

> 우리는 에이전트를 **처음부터 코딩하지 않고**, 완성품을 **운전**하며 §1~§11을 눈으로 확인.

## 13. 복습 퀴즈

1. 일반 LLM과 Agentic LLM의 차이를 **실행 흐름** 관점에서 한 줄로.
2. ReAct 루프 4단계와 각 단계에서 일어나는 일.
3. ReAct / CoT / ToT / MCTS를 (도구호출 / 단계추론 / 트리탐색 / 게임·최적화)에 매칭.
4. "계획-실행 분리"에서 **Evaluator** 역할? 우리 시드실험에선 누가?
5. **MCP**와 **A2A**의 차이를 "수직/수평"으로 설명 + 각 예.
6. 로컬 런타임 3가지 장점이 왜 **OpenClaw**의 강점인지.
7. ollama+Claude+Codex+Gemini 다중 에이전트의 **역할 분담** 설계.

> 막히면 → §1 정의 · §3 구성요소 · §5 추론 · §8·§9 통신 으로 복귀.
