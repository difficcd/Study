---
id: knowledge-base-design
title: AI 최적화 지식베이스 설계 (멀티에이전트 합심 결과)
tags: [kb, knowledge-base, rag, frontmatter, retrieval]
created: 2026-06-24
updated: 2026-06-24
status: active
links: [multi-agent-setup, index]
---

# AI 최적화 지식베이스 설계 (멀티에이전트 합심 결과)

<small>2026-06-24 · 패널 1회전: ollama gemma3:4b + gemini CLI + claude → 종합 ｜ [[multi-agent-setup (testing)|멀티에이전트 셋업]]</small>

> 세 에이전트가 같은 과제를 풀고, 오케스트레이터(claude)가 합쳐 만든 "최적 효율 지식 창고" 설계.

## 1. 구조 (Structure)
- **주제 계층** `주제/하위주제/` + **날짜 저널** `journal/YYYY-MM-DD.md` 병용 (맥락 + 시점 둘 다)
- **원자성**: 1파일 = 1개념/1아이디어, **안정적 고유 ID**(접두어)
- **소스 vs 파생 분리**: 손으로 쓴 `.md`(원본) ↔ 자동생성 `index/`·임베딩(파생)

## 2. 포맷 & 메타데이터 (Format)
- 전부 **Markdown(.md)** — LLM 친화, 텍스트 기반
- 상단 **YAML front matter**: `id, title, tags, created, updated, status, links`
- 파일명: `ID/kebab + 핵심키워드` (예: `agent-react-loop.md`)

## 3. 검색·인출 (Retrieval) — LLM 효율 핵심
- **컴팩트 인덱스 + 지연 로딩**: AI가 작은 인덱스만 먼저 읽고 → 필요한 원자 노트만 fetch (토큰 절약)
- front matter 태그/ID로 **1차 필터** → 본문에서 핵심 추출
- **ID/위키링크로 지식그래프** → 연관 추적
- **헤딩 = 청크 경계**: 임베딩(RAG) 시 깔끔하게 분할

## 4. 핵심 원칙 (Principles)
1. **일관성·정형화** (표준 포맷 + YAML)
2. **원자성** (작게 쪼갬)
3. **연결성** (ID 링크 → 그래프)
4. **확장성** (추가·갱신 용이)
5. **토큰 효율** (인덱스 먼저, 디테일은 지연 로딩) — LLM 특화

## 5. 메타 관찰
우리 **Claude 메모리 시스템이 이미 이 패턴**: `MEMORY.md`(컴팩트 인덱스) + 원자 fact 파일 + frontmatter + `[[links]]`.
→ 이 설계 = 그 방식을 학습 Vault 전체로 확장한 것.

## 6. 적용 후보 (우리 AI_training Vault)
- [ ] 각 노트 상단에 YAML front matter 추가(id/tags/updated)
- [ ] 루트에 `INDEX.md`(전 노트 1줄 요약) 자동/수동 유지 → AI가 먼저 읽는 지도
- [ ] 큰 노트는 원자 단위로 분할

## 부록 A. frontmatter란? (2026-06-24 Q&A)
- 파일 맨 위 `---` 사이의 **메타데이터 딱지**. 본문(사람용)과 별개로 **도구/AI가 분류·검색·연결**에 사용.
- 필드 예: `id`(고유이름) · `tags`(필터/검색) · `created/updated` · `source` · `links`(노트 연결).
- 비유: 도서관 책의 **분류카드/바코드**. 내용은 그대로, 카드 덕에 빨리 찾음.
- 효과: AI가 INDEX의 태그만 보고 열 노트를 결정 → **토큰 절약**. 옵시디언 태그/그래프도 이걸로.
- **velog엔 안 씀**(velog는 본문 마크다운만). frontmatter는 *AI 친화 창고*용 장치.
