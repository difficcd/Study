---
id: index
title: Study 지식베이스 인덱스
tags: [index, map]
updated: 2026-06-24
---

# INDEX — AI가 먼저 읽는 지도

> 컴팩트 인덱스. AI/사람이 이걸 먼저 읽고 → 필요한 노트만 지연 로딩(토큰 절약).
> 설계 근거: [[knowledge-base-design]]

## 트랙 (대단원)
- [[README]] — 대시보드 / 진도판
- 01 · 클라우드 OCI → [[Cloud_basic/_index]]
- 02 · Claude 활용 → [[Claude/_index]]
- 03 · Transformer/minGPT → [[AI_basic/_index]]
- 04 · 에이전트 설계 → [[04_agent-design/_index]]
- 05 · 리눅스 커널 → [[Linux_kernel/_index]]
- 06 · OpenClaw·멀티에이전트 → [[Openclaw_VM/_index]]

## 노트 (한 줄 요약)
| id | 제목 | tags |
|---|---|---|
| agent-basics | 에이전트 기초(정의·ReAct·구성요소·MCP/A2A) | agent, react, mcp, a2a |
| multi-agent-setup | 멀티에이전트 셋업·하이브리드 아키텍처 | multi-agent, ollama, gemini, claude |
| knowledge-base-design | AI 최적화 지식베이스 설계(합심 결과) | kb, rag, frontmatter, retrieval |

## 규칙 (frontmatter 표준)
모든 노트 상단에: `id, title, tags, created, updated, status, links`
- **원자성**(1파일=1개념) · **연결성**(`[[id]]` 링크) · **토큰효율**(이 INDEX 먼저)
