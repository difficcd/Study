---
id: multi-agent-setup
title: 멀티에이전트 구성 (본 PC) 셋업 & 설계
tags: [multi-agent, ollama, gemini, claude, codex, orchestration]
created: 2026-06-24
updated: 2026-06-24
status: active
links: [agent-basics, knowledge-base-design]
---

# 멀티에이전트 구성 (본 PC) — 셋업 & 설계 노트

[[Openclaw_VM/_index|← 06 트랙]] · 시작 2026-06-24

## 1. 환경 확인 결과 (본 PC)
| 도구 | 상태 | 역할 후보 |
|---|---|---|
| ollama | 설치됨 — `gemma3:4b`, `llava` | 로컬·무료 일꾼(요약/분류/초안/비전) |
| claude (CLI) | 설치됨 | 기획·추론·코딩(오케스트레이터) |
| gemini (CLI) | 설치됨 | 크로스체크·대안 관점·긴 맥락 |
| codex | 설치됨 — `@openai/codex` v0.142 (gpt-5.5) | 코딩 전담(Coder) |
| API키(env) | 없음 | CLI 자체 로그인 사용 → 스크립트는 CLI 호출 방식 |
- node v22, python(msys2) 보유.

## 2. 역할 분담(초안)
- **Planner / Orchestrator** = Claude
- **Coder** = Codex(없으면 Claude 겸임)
- **Local Worker** = ollama `gemma3:4b`(텍스트), `llava`(이미지)
- **Critic / Cross-check** = Gemini

## 3. 진행 패턴(단계)
- (A) **패널 패턴**: 한 질문 → 여러 모델에 동시 질의 → 답 모아 비교·종합 (가장 단순, 첫걸음)
- (B) **역할 파이프라인**: Planner→Worker→Critic 순으로 전달
- (C) 점차 도구 호출(MCP)·자율 루프로 확장

## 4. 다음 결정사항
- [ ] 오케스트레이터 언어: Python vs Node
- [ ] codex 설치할지(OpenAI 계정 필요) or 생략
- [ ] 첫 패턴: A(패널)부터

## 5. 개념: 멀티에이전트 패턴 & 트렌드 (2026-06-24 Q&A)
- 핵심 = "여러 에이전트 엮기" + **토폴로지(어떻게) / 오케스트레이터(누가 조율) / 프로토콜(어떻게 소통)**
- 패턴: ① 패널·앙상블(병렬→종합) ② **슈퍼바이저-워커**(위임=AI회사) ③ 파이프라인(Planner→Coder→Critic) ④ 토론(debate)
- 대표 OSS: MetaGPT·ChatDev(SW회사 역할극), AutoGen·CrewAI·LangGraph(범용)
- 소통 표준: MCP(도구), A2A(에이전트간) — [[agent-basics|에이전트 기초 §8·§9]]

## 6. 오케스트레이터 = 2모드
- A. **Claude가 직접 조율**(지금, 코드0): Bash로 ollama/gemini 호출·취합
- B. **독립 스크립트**(나중, Node): 혼자 24/7 자동 실행 = 진짜 구축물

## 7. 자동화엔 CLI 필요 (GUI 앱은 수동)
- 자동: ollama / gemini CLI / claude CLI
- 수동(전문가): codex 앱, Antigravity(Gemini Pro IDE) — 자동화엔 codex CLI(`npm i -g @openai/codex`) 별도 필요
- 주의: gemini CLI와 Antigravity는 같은 Gemini → 자동화엔 하나면 충분

## 8. 확정 아키텍처(하이브리드)
- 자동 코어: Brain=Claude, 로컬일꾼=ollama(gemma3/llava), 크로스체크=gemini CLI
- 수동 전문가: codex 앱(코딩), Antigravity(무거운 코딩)

## 9. Q&A 로그
- **2026-06-24 패널 1회전** (모드 A, claude 오케스트레이터): 과제 "AI 최적화 지식베이스 설계". ollama gemma3:4b(구조)·gemini CLI(원칙)·claude(인출효율) 3종 응답 → 종합. 결과: [[knowledge-base-design]]. 성격차이: gemma3=실용/빠름, gemini=원칙(원자성·그래프), claude=토큰효율 인덱싱.

## 10. 인공지능 강의 속 멀티에이전트 (참고 위치, material/)
- **「Agentic AI I」**: §1.4 단일 vs 다중 에이전트 / A2A(에이전트 간 통신) / MCP vs A2A / **LangGraph**(다중에이전트, node=에이전트·edge=통신, 예 Researcher+Router)
- **「Coding Agent」**: 전체가 **오케스트레이터-서브에이전트** 구조이고 그 예시가 **Claude Code**.
  - 오케스트레이터가 작업 분해 → Subagent A/B/C(grader) **spawn**, 각자 **독립 context**, 완료시 **요약만** 반환
  - **Tool 제한으로 역할 강제**(Coder=Read/Write/Bash, Reviewer=Read/Bash)
  - 에이전트 간 정보 전달 = **파일 시스템** 경유
  - → 우리 "패널 → 슈퍼바이저-워커" 실습과 1:1 매핑. [[agent-basics]] §7 참고.

## 11. codex CLI 설치·합류 (2026-06-25)
- 설치: `npm i -g @openai/codex` → `codex-cli 0.142.1`. **로그인은 ChatGPT 계정 세션 자동 사용**(브라우저 따로 안 함).
- **모델**: ChatGPT 계정은 기본 `gpt-5.2` 거부됨("not supported") → `/model`로 **gpt-5.5** 선택. 자동화 땐 `-m gpt-5.5` 명시.
- **자동화 호출 정답** (멀티에이전트 스크립트에서 codex 부르는 법):
  ```bash
  codex exec -m gpt-5.5 -c model_reasoning_effort=low \
    --skip-git-repo-check --ephemeral -s read-only \
    "프롬프트" </dev/null
  ```
  - **`</dev/null` 필수**: 없으면 "Reading additional input from stdin..."에서 **무한 대기**(stdin 파이프가 안 닫혀서). 제일 큰 함정.
  - `-s read-only`(샌드박스) / `--ephemeral`(세션파일 안 남김) / `-c model_reasoning_effort=low`(잡일 속도↑, 검증·코딩엔 생략).
  - 답만 깔끔히: `--output-last-message out.txt` 추가 → 그 파일만 읽기.
- 이제 자동 코어 4종 완비: **Claude(조율) · ollama(로컬일꾼) · gemini(크로스체크) · codex(코딩)**. [[agent-basics]] §7 매핑대로.
