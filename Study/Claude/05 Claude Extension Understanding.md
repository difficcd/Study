# Multica claude skills -`/plugin`의 원리


원리를 뜯어보면 사실 굉장히 단순한 구조예요 
— **"git 저장소를 clone해서 정해진 폴더 구조를 읽어들이는 것"**이 전부입니다.

### 전체 구조

```
Anthropic
├── Claude Code (에이전트 CLI)
│   └── /plugin 명령어 (공식 기능)
│
├── claude-plugins-official (공식 마켓플레이스)
│   └── Anthropic이 직접 큐레이션한 플러그인들
│
└── claude-community (커뮤니티 마켓플레이스)
    └── 외부 제출 → 자동 검증 통과한 것들
```

---

### 플러그인 폴더 구조 (Anthropic 공식 관례)

플러그인은 이 구조를 따라요: [GitHub](https://github.com/anthropics/claude-plugins-official/blob/main/README.md)

```
plugin-name/
├── .claude-plugin/
│   └── plugin.json      ← 메타데이터 (필수)
├── .mcp.json            ← MCP 서버 설정 (선택)
├── commands/            ← 슬래시 명령어 (선택)
├── agents/              ← 에이전트 정의 (선택)
├── skills/              ← 스킬 정의 (선택)
└── README.md
```

---

### `/plugin` 명령어 흐름

[[Claude Code Slash Command]] 참조..

`claude-plugins-official`은 Claude Code를 처음 실행할 때 자동으로 등록돼요. [Claude Code Docs](https://code.claude.com/docs/en/plugins)

```
/plugin marketplace add anthropics/claude-community
        ↓
마켓플레이스(git 레포) 주소 로컬에 등록

/plugin install andrej-karpathy-skills@karpathy-skills
        ↓
해당 마켓플레이스 레포에서 플러그인 pull
        ↓
~/.claude/plugins/ 에 설치
        ↓
Claude Code가 자동 스캔해서 컨텍스트 주입
```

---

### 마켓플레이스 = git 레포

마켓플레이스는 git으로 배포돼요. GitHub, GitLab, Bitbucket, 프라이빗 서버도 마켓플레이스가 될 수 있어요. Anthropic 공식 디렉토리는 그 중 가장 큐레이션된 버전이에요. [Claude Camp](https://claudecamp.ai/blog/claude-code-plugins-official-directory)

### GitLab, Bitbucket도 마켓플레이스가 될 수 있다?

마켓플레이스가 결국 **git 레포 하나**예요.

```
마켓플레이스 = .claude-plugin/marketplace.json 파일이 있는 git 레포
```

GitHub에만 올릴 수 있는 게 아니라, << git 레포를 호스팅할 수 있는 곳이면 어디든 된다는 거예요. >> 

```
회사 내부용 예시:
gitlab.mycompany.com/infra/claude-plugins
        ↓
/plugin marketplace add gitlab.mycompany.com/infra/claude-plugins
        ↓
사내 전용 플러그인만 있는 비공개 마켓플레이스
```

외부에 공개하기 싫은 사내 도구, 
팀 전용 스킬 같은 걸 << 프라이빗하게 배포할 때 >> 쓰는 거예요.


---

### Multica 역할 재정정

Multica는 이 공식 시스템을 **그대로 활용**해서 팀 협업 레이어를 얹은 거예요.

```
Anthropic 공식     /plugin, .claude-plugin/, 마켓플레이스 구조
       ↓
Multica가 추가한 것  이슈 보드, 팀 에이전트 관리, 로컬 데몬
```

`/plugin` 명령어도, `.claude-plugin/` 폴더 구조도, 마켓플레이스 개념도 전부 Anthropic 공식이에요. Multica는 그 위에 팀 협업 도구를 얹은 거예요.



# 용어 및 자잘한 호기심 정리

## apt란 ? 

어디서 많이 본 단언데..
sudo apt? 리눅스!


=> **리눅스(Debian/Ubuntu 계열)의 패키지 관리자**입니다. Advanced Package Tool의 약자로, << 운영체제 수준에서 소프트웨어를 설치·업데이트·삭제하는 명령줄 도구 >> 예요.

### 기본 사용 꼴

bash

```
sudo apt update              # 패키지 목록(카탈로그) 최신화
sudo apt install python3     # 설치
sudo apt remove python3      # 삭제
sudo apt upgrade             # 설치된 것들 일괄 업그레이드
```

### 익숙한 것들과 비교하면

| 도구       | 관리 대상                       | 예시                   |
| -------- | --------------------------- | -------------------- |
| **apt**  | OS 레벨 소프트웨어 (Ubuntu/Debian) | `apt install ffmpeg` |
| **pip**  | Python 패키지                  | `pip install numpy`  |
| **npm**  | Node.js 패키지                 | `npm install react`  |
| **brew** | macOS 소프트웨어                 | `brew install git`   |

즉, pip이 Python 생태계의 패키지 관리자라면, apt는 **우분투 OS 자체의 패키지 관리자**입니다. Python 인터프리터 자체, CUDA 드라이버, Docker, git 같은 걸 까는 도구죠.

### 이전 대화의 비유 설명

제가 `/plugin`을 설명할 때 쓴 비유를 다시 풀면:

bash

```
# apt: 저장소(패키지 카탈로그)를 먼저 등록하고
sudo add-apt-repository ppa:some/repo
sudo apt install some-package

# Claude Code: 마켓플레이스(플러그인 카탈로그)를 먼저 등록하고
/plugin marketplace add anthropics/claude-code-plugins
/plugin install my-plugin@claude-code-plugins
```

둘 다 **"카탈로그 등록 → 카탈로그에서 골라 설치"** 라는 2단계 패턴이 동일하다는 뜻이었습니다. apt도 기본적으로 우분투 공식 저장소(카탈로그)에서 패키지를 받아오는데, 공식에 없는 소프트웨어는 서드파티 저장소를 먼저 추가해야 하거든요.

MLOps 쪽으로 가시면 apt는 필수로 만나게 됩니다 — Dockerfile 작성할 때 `RUN apt-get install -y ...`가 거의 모든 이미지에 등장하고, GPU 서버 세팅(CUDA, 드라이버)도 apt로 하니까요.

## multica-ai 과 Go 

[https://github.com/multica-ai/multica](https://github.com/multica-ai/multica)

보면 Go가 큰 비율로 잡히는 걸 알 수 있는데
이건 왜일까?? OS 에서 짧게나마 마주했던 Go.. 멀티스레딩..

![[Pasted image 20260716010358.png|236]]

OS 복습 겸 정리 :

### Go의 실제 모델: 스레드가 아니라 goroutine

Go는 OS 스레드를 직접 다루게 하지 않고,
**goroutine이라는 초경량 실행 단위**를 제공합니다:

| 구분       | OS 스레드 (Java/C++ 등) | goroutine            |
| -------- | ------------------- | -------------------- |
| 초기 스택 크기 | ~1MB                | **~2KB** (필요시 자동 확장) |
| 생성 비용    | 시스템 콜, 비쌈           | 함수 호출 수준, 쌈          |
| 현실적 개수   | 수천 개면 부담            | **수십만~수백만 개** 가능     |
| 스케줄링     | OS 커널               | Go 런타임 (M:N 스케줄러)    |

핵심은 **M:N 스케줄링**입니다 — Go 런타임이 M개의 goroutine을 N개의 OS 스레드(보통 CPU 코어 수, `GOMAXPROCS`) 위에 얹어서 스스로 스케줄링합니다. 그래서:

- **동시성(concurrency)**: 수십만 goroutine이 I/O 대기 중에도 스레드를 점유하지 않고 양보 → 네트워크 !!요청 폭탄 처리에 강함!!
- **병렬성(parallelism)**: 실행 가능한 goroutine들이 여러 코어에서 동시에 돎 → CPU 멀티코어 활용도 됨!

```go
for i := 0; i < 100000; i++ {
    go handleRequest(i)  // 10만 개 goroutine, 문제없음
}
```

### 뉘앙스: Go의 강점은 "병렬 연산"보다 "동시 대기"

- 순수 CPU 숫자 계산(number crunching)에서는 C++/Rust가 더 빠릅니다. Go의 진짜 강점은 **"수만 개의 요청이 각자 네트워크/DB를 기다리는 상황"을 코드 몇 줄로, 적은 메모리로 버텨내는 것**
- 그래서 이전 대화의 결론과 연결됩니다: AI 인프라에서 Go가 맡는 건 무거운 행렬 연산(→ GPU/CUDA)이 아니라, 수많은 동시 연결을 관리하는 **컨트롤 플레인**이라는 것

### 한 줄 정리

"Go = CPU 멀티스레딩 언어" → 대체로 맞지만, 더 정확히는 **"goroutine 기반 M:N 동시성 모델로, 멀티코어 병렬 실행과 대규모 동시 I/O 처리를 모두 언어 차원에서 지원하는 언어"**입니다. 구호로는 _"Concurrency is not parallelism"_ (Rob Pike, Go 설계자) — Go는 동시성을 우선 설계했고, 병렬성은 그 부산물로 따라옵니다.


언어 선택은 하드웨어 성능이 아니라 **워크로드의 성격**으로 결정됩니다:

|워크로드|병목|적합 언어|
|---|---|---|
|CPU-bound (숫자 계산, 인코딩, 커널)|연산 자체|C++/Rust — 오버헤드 제로에 가까움|
|I/O-bound (수만 개 요청이 네트워크/DB 대기)|대기와 스케줄링|Go — goroutine으로 싸게 처리|
|개발 속도·팀 생산성|사람|Go — 단순한 언어, 빠른 컴파일|
HW가 빈약할수록(임베디드 등) 런타임·GC가 없는 C++/Rust가 필요하고, HW가 넉넉하면 Go/Java의 GC 오버헤드쯤은 문제가 안 되죠. 
**"연산이 무거운가 vs 대기가 많은가, 그리고 개발 비용을 얼마나 감당할 것인가"** 축으로 봐야 합니다.

### C++은 멀티스레딩 잘 되나? → 성능은 최강, 대신 전부 수동

C++은 `std::thread`, atomics, 메모리 모델까지 갖춘 완전한 멀티스레딩 언어이고, OS 스레드를 직접 다루므로 **이론상 성능 상한이 가장 높습니다.** 문제는 안전장치가 없다는 것:

- 데이터 레이스, 데드락, use-after-free를 컴파일러가 전혀 안 잡아줌 — 런타임에 간헐적으로 터지는 최악의 버그 유형
- 스레드는 무겁기 때문에(~1MB 스택) 수만 개 동시 연결을 하려면 스레드풀 + epoll 기반 비동기 이벤트 루프를 **직접 구축**해야 함 ([nginx](https://soonmin.tistory.com/88) 가 이 방식)
- 즉 "잘 되는 언어"가 아니라 **"잘 되게 만들 수 있는 언어"** — 전문가 비용이 큼

### "Rust는 소유권 때문에 비효율적"? → ❌ 방향이 반대입니다

이건 중요한 오해예요. 
소유권/borrow checker는 **컴파일 타임 검사**라서 **런타임 비용이 0**입니다:

- 비효율적인 것은 실행 성능이 아니라 **개발자의 시간** — 컴파일러와 싸우는 학습곡선이 가파름
- 대가로 얻는 게 **"Fearless Concurrency"**: 데이터 레이스가 있는 코드는 아예 컴파일이 안 됨. C++에서 런타임에 터질 버그를 컴파일 타임에 차단
- 실행 성능은 C++과 동급이고, GC가 없어서 Go보다 지연이 예측 가능합니다. 그래서 최근 고성능 인프라(예: Cloudflare의 프록시 Pingora, Discord가 Go→Rust 전환한 서비스)가 Rust로 가는 추세

정리하면: **Rust = 런타임 효율 최상 + 개발 비용 높음**, 
"비효율"은 사람 쪽 이야기입니다.


언어 선택=f(런타임 성능 요구,  개발 비용,  안전성 요구)\text{언어 선택} = f(\text{런타임 성능 요구}, \; \text{개발 비용}, \; \text{안전성 요구})언어 선택=f(런타임 성능 요구,개발 비용,안전성 요구)

|C++|Rust|Go|
|---|---|---|---|
|런타임 성능|최상|최상|상 (GC 있음)|
|동시성 안전|수동 (위험)|컴파일러 보장|관례 + channel (레이스 가능은 함)|
|개발 속도|느림|느림|**빠름**|
|대표 영역|게임엔진, 추론 커널|프록시, OS, 고신뢰 인프라|API 서버, 게이트웨이, CLI, K8s 생태계|

실무 감각으로는: **성능이 "충분히 좋으면 되는" 컨트롤 플레인은 Go, 나노초 단위 지연이나 메모리 통제가 필요한 데이터 플레인 코어는 C++/Rust.** Discord 사례가 딱 이 경계인데, Go의 GC 스파이크(수 분마다 지연 튐)가 문제가 되자 해당 서비스만 Rust로 옮겼습니다 — 즉 Go로 시작해서 병목인 부분만 저수준으로 내리는 게 흔한 패턴입니다.



## 