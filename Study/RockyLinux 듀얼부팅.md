
# 삼성 T7 → Rocky Linux 외장 부팅 설치 매뉴얼

**대상 환경**

- 노트북: 갤럭시북 프로3 (i5-1340P / RAM 16GB / 내장 238GB, 윈도우)
- 외장: 삼성 T7 1TB (931.4GB, exFAT 단일 파티션, 데이터 438.6GB)
- 연결: C to C 직결
- 목표: 내장 윈도우는 그대로 두고, T7에 Rocky Linux를 네이티브 설치해 부팅 메뉴로 선택 사용

**핵심 원칙 3가지**

1. 내장 238GB 디스크는 설치 과정에서 **절대 선택하지 않는다**
2. 부트로더(ESP)는 **T7 안에** 만든다
3. T7을 뽑았을 때 윈도우가 평소대로 부팅되어야 한다

---

## 준비물 체크리스트

- [ ] USB 메모리 8GB 이상 (설치 미디어용, T7과 별개)
- [ ] 노트북 충전기
- [ ] BitLocker 복구키 (아래 0-3 참조)
- [ ] 인터넷 연결

---

# Phase 0 — 사전 작업

## 0-1. T7 Security Mode 해제 ⚠️ 필수

현재 매지션에서 Security Mode가 **ON** 상태다. 이건 T7의 하드웨어 암호화 잠금인데, 해제하려면 삼성 포터블 SSD 소프트웨어가 실행되어야 한다. 리눅스 부팅 시점에는 그런 게 없으므로 **드라이브가 잠긴 채로 남아 부팅 자체가 불가능**하다.

1. Samsung Magician 실행
2. 좌측 `Security Setting` 클릭
3. `Security Mode` 토글을 **OFF**
4. 기존 비밀번호 입력 → 적용
5. T7 뽑았다 다시 꽂아서, 비밀번호 창 없이 바로 E: 드라이브가 열리는지 확인

> 이 확인을 안 하고 넘어가면 Phase 3에서 원인 모를 부팅 실패로 시간 날린다.

## 0-2. 파티션 스킴 확인 (GPT인지 MBR인지) ⚠️ 중요

UEFI 부팅을 하려면 T7이 **GPT**여야 한다. T7은 공장 출하 시 MBR인 경우가 있으니 반드시 확인.

1. 시작 버튼 우클릭 → `터미널(관리자)` 또는 `PowerShell(관리자)`
2. 아래 입력:

```
diskpart
list disk
```

3. 출력 표에서 T7에 해당하는 디스크(931GB짜리, 아마 `디스크 1`)의 맨 오른쪽 **`Gpt` 열**을 본다
    - `*` 표시 있음 → **GPT**. 정상. `exit` 치고 Phase 1로 진행
    - `*` 표시 없음 → **MBR**. 아래 0-2-a로

```
exit
```

### 0-2-a. MBR이었을 경우

MBR → GPT 변환은 데이터를 날리지 않고도 가능하지만 실패 리스크가 있다. 두 갈래:

- **A안 (권장, 안전)**: 438GB를 어딘가로 옮기고 → T7 전체를 GPT로 초기화 → 파티션 새로 구성
    - 이 경우 Phase 1의 "축소" 과정이 통째로 불필요해지고, 그냥 원하는 대로 나누면 된다
- **B안**: MiniTool의 `Convert MBR Disk to GPT Disk` 기능 사용 (데이터 유지)
    - 무료판에서 데이터 디스크 변환은 지원됨. 다만 변환 실패 시 파티션 테이블이 깨질 수 있으므로 유일본 파일은 먼저 대피

## 0-3. BitLocker 복구키 확보

부팅 순서나 Secure Boot 설정을 건드리면 윈도우가 복구키를 요구할 수 있다.

1. 브라우저에서 `account.microsoft.com/devices/recoverykey` 접속
2. 노트북에 해당하는 48자리 키를 **휴대폰으로 촬영하거나 메모**
3. 또는: 관리자 터미널에서 `manage-bde -protectors -get C:` 실행 후 Numerical Password 값 저장

## 0-4. 유일본 파일 대피

T7 안 438GB 중 **다른 곳에 원본이 없는 파일**만 골라서 C: 또는 클라우드에 임시 복사. (백업본만 들어있다면 이 단계는 생략 가능)

## 0-5. 물리적 준비

- 노트북 **충전기 연결** (파티션 작업 중 전원 차단 = 최악의 시나리오)
- T7은 **C to C로 노트북에 직결**. USB 허브나 도킹 스테이션 경유 금지
- 다른 USB 장치는 모두 제거

---

# Phase 1 — 파티션 공간 확보

## 왜 필요한가

현재 T7의 상태:

```
디스크 1 (931.4GB)
┌──────────────────────────────────────────────┐
│           exFAT 파티션 931.4GB                │
│ [데이터 438.6GB][ 파일시스템 내 빈칸 492.8GB ] │
└──────────────────────────────────────────────┘
                 미할당 영역: 0 바이트
```

"사용 가능 492.8GB"는 **파일시스템 안의 여유 공간**이지, 디스크의 빈 자리가 아니다. 파티션 하나가 디스크 전체를 점유하고 있어서 리눅스가 들어갈 자리가 물리적으로 없다.

목표 상태:

```
┌────────────────────┬─────────────────────────┐
│  exFAT 450GB       │   미할당 약 481GB       │
│  (데이터 그대로)    │   (여기에 Rocky)        │
└────────────────────┴─────────────────────────┘
```

## 1-1. 도구 설치

MiniTool Partition Wizard Free (또는 AOMEI Partition Assistant Standard) 설치.

- 설치 중 번들 소프트웨어 체크박스 해제할 것

## 1-2. 축소 실행

1. MiniTool 실행 → 하단 디스크 맵에서 **디스크 1의 E: (T7)** 확인
    - 내장 디스크(238GB)와 헷갈리지 말 것. 용량과 드라이브 문자로 반드시 재확인
2. E: 파티션 우클릭 → `Move/Resize`
3. 다이얼로그에서:
    - **왼쪽 핸들은 절대 건드리지 않는다** (파티션 시작 위치 고정)
    - 오른쪽 핸들만 왼쪽으로 당기거나, `Partition Size` 칸에 **450 GB** 직접 입력
    - `Unallocated Space After` 가 약 481GB로 표시되는지 확인
4. `OK`
5. 좌측 하단 `Apply` 클릭 → 확인
6. 완료까지 대기 (**중간에 절대 취소하거나 케이블 뽑지 말 것**)

> 데이터가 파티션 앞쪽에 있고 뒤쪽 빈 공간만 잘라내는 작업이라, 파일 이동이 발생하지 않아 리사이즈 중 가장 안전한 케이스다. 보통 수 분 내 끝난다.

## 1-3. 검증

- 윈도우 탐색기에서 E: 열어서 파일들이 멀쩡한지 확인
- `디스크 관리`(Win+X → 디스크 관리)에서 디스크 1에 **미할당** 영역이 보이는지 확인

## 트러블슈팅

**Move/Resize가 회색으로 비활성화된 경우** → 무료판이 exFAT 리사이즈를 막아둔 버전이다. 대안:

1. AOMEI Partition Assistant Standard로 재시도
2. 그래도 안 되면: 438GB를 대피시키고 → 파티션 삭제 → 원하는 크기로 exFAT 재생성 → 데이터 복원

---

# Phase 2 — 설치 미디어 준비

## 2-1. ISO 다운로드

1. `rockylinux.org/download` 접속
2. **Rocky Linux 10.x**, 아키텍처 **x86_64**, 이미지 종류 **DVD ISO** 선택
    - Minimal ISO는 GUI가 없다. 데스크톱 환경 쓸 거면 DVD
    - 수업에서 특정 버전을 지정했다면 그걸 따를 것
3. 다운로드 후 체크섬 검증 (선택이지만 권장):

```powershell
Get-FileHash .\Rocky-10.x-x86_64-dvd.iso -Algorithm SHA256
```

다운로드 페이지의 CHECKSUM 값과 대조.

## 2-2. Rufus로 굽기

1. `rufus.ie` 에서 Rufus 다운로드 (포터블판이면 설치 불필요)
2. **USB 메모리** 연결 (T7 아님. 절대 T7 선택 금지)
3. Rufus 실행:
    - 장치: USB 메모리
    - 부트 선택: 다운로드한 ISO
    - 파티션 방식: **GPT**
    - 대상 시스템: **UEFI (non CSM)**
4. `시작` 클릭 → 모드 선택 창이 뜨면 **`DD 이미지 모드로 쓰기`** 선택
    - ISO 모드는 RHEL 계열에서 설치 소스를 못 찾는 오류가 자주 난다
5. 완료 대기

---

# Phase 3 — BIOS 설정

## 3-1. 진입

1. 노트북 완전 종료
2. 전원 버튼 누른 직후 **F2 연타** (안 되면 `Esc`)
3. BIOS(Aptio Setup) 진입

## 3-2. 설정 변경

|항목|값|위치(대략)|
|---|---|---|
|Fast Boot / Fast BIOS Mode|**Disabled**|Advanced|
|Secure Boot|**Enabled 유지**|Boot 또는 Security|
|USB Boot / USB S3 Wake-up|Enabled|Advanced|

**Secure Boot에 대해**: Rocky Linux의 shim은 마이크로소프트 서명이 되어 있어서 Secure Boot를 켠 상태로도 정상 부팅된다. 켜둔 채로 먼저 시도하고, 설치 USB 자체가 인식되지 않을 때만 임시로 Disabled로 바꿀 것. (끄면 BitLocker 복구키를 물어볼 수 있다 — 0-3에서 확보한 키 사용)

3. `F10` → Save & Exit

## 3-3. USB로 부팅

1. 설치 USB와 T7을 **둘 다** 연결
2. 전원 켠 직후 **F10 연타** → 부팅 장치 선택 메뉴
    - 안 뜨면 F12 또는 Esc 시도
3. `UEFI: [USB 메모리 이름]` 항목 선택
    - **`UEFI:` 접두사가 붙은 항목**을 골라야 한다. 없는 항목은 레거시 부팅이라 안 됨
4. GRUB 화면에서 `Install Rocky Linux 10.x` 선택 (Test media는 건너뛰어도 무방)

---

# Phase 4 — Anaconda 설치 (가장 중요한 단계)

## 4-1. 초기 화면

1. 언어: `한국어` 또는 `English` (터미널 오류 메시지 검색 편의상 English 추천)
2. 설치 요약 화면 진입

## 4-2. Installation Destination — 디스크 선택 ⚠️

1. `Installation Destination` 클릭
2. **Local Standard Disks** 목록에서:
    - **`Samsung PSSD T7` (931.51 GiB) 만 클릭해서 체크**
    - **내장 238GB 디스크는 체크 해제 상태 유지** — 이미 체크되어 있다면 반드시 클릭해서 해제
3. 하단 `Storage Configuration` → **`Custom`** 선택
4. `Done`

> 이 화면이 전체 작업의 분기점이다. 내장 디스크가 선택된 채로 진행하면 윈도우 부트로더가 덮어써질 수 있다. 체크 상태를 두 번 확인할 것.

## 4-3. Manual Partitioning — 파티션 생성

좌측 상단 드롭다운을 **`Standard Partition`** 으로 변경 (LVM 아님. 단순하고 문제 적음).

기존 `exFAT` 파티션이 목록에 보이면 **손대지 말고 그대로 둔다.**

`+` 버튼으로 아래를 순서대로 생성:

|#|Mount Point|Desired Capacity|파일시스템|
|---|---|---|---|
|1|`/boot/efi`|`1024 MiB`|EFI System Partition|
|2|`/boot`|`2048 MiB`|ext4|
|3|`swap`|`8192 MiB`|swap|
|4|`/`|`120 GiB`|xfs|
|5|`/home`|(비워두기 = 나머지 전부)|xfs|

**각 파티션 생성 후 오른쪽 패널에서 확인할 것:**

- `Device(s)` 항목이 **`Samsung PSSD T7`** 로 되어 있는지
- 다른 디스크가 잡혀 있으면 `Modify...` 로 T7만 지정

특히 `/boot/efi`가 T7에 만들어지는 게 이 계획의 핵심이다. 이게 내장 디스크의 기존 ESP를 재사용하도록 잡히면 윈도우 부팅에 영향을 준다.

완료 후 `Done` → 변경 요약(Summary of Changes) 창에서:

- **T7 파티션에 대한 Create 항목만** 있는지 확인
- 내장 디스크(nvme0n1 등)에 대한 Delete/Format 항목이 하나라도 있으면 **즉시 Cancel** 하고 4-2로 돌아갈 것

`Accept Changes`

## 4-4. 나머지 설정

- `Software Selection`: `Server with GUI` 또는 `Workstation` 선택
- `Root Password`: 설정 (또는 잠금 후 관리자 사용자만 생성)
- `User Creation`: 사용자 생성 + **`Make this user administrator` 체크**
- `Network & Host Name`: Wi-Fi 연결, 호스트명 설정
- `Time & Date`: Asia/Seoul

`Begin Installation` → 완료까지 대기 → `Reboot System`

---

# Phase 5 — 설치 후 검증

## 5-1. 첫 부팅

재부팅 시 **F10** → `UEFI: Samsung PSSD T7` 선택 → GRUB → Rocky 부팅

(설치 USB는 이제 뽑아도 된다)

## 5-2. 부트로더 위치 확인

터미널 열고:

```bash
sudo efibootmgr -v
```

`Rocky Linux` 항목의 경로에 T7의 파티션 UUID가 있는지 확인. 윈도우 부트 항목(`Windows Boot Manager`)도 그대로 남아 있어야 정상이다.

```bash
lsblk -f
```

파티션 배치와 마운트 상태 확인.

## 5-3. 윈도우 정상 여부 확인 ⚠️

1. Rocky 종료
2. **T7 케이블 분리**
3. 전원 켜기 → 윈도우가 평소처럼 뜨는지 확인

여기까지 통과하면 설치 성공이다.

## 5-4. 일상 사용법

- **윈도우**: T7 뽑고 그냥 켜기 (또는 꽂은 채로 부팅해도 기본은 윈도우)
- **Rocky**: T7 꽂고 부팅 → `F10` → T7 선택

---

# Phase 6 — 선택 설정

## 6-1. exFAT 데이터 파티션 마운트

T7에 남겨둔 450GB exFAT를 Rocky에서도 쓰려면:

```bash
sudo dnf install -y exfatprogs
sudo mkdir -p /mnt/data
sudo blkid | grep exfat        # UUID 확인
```

`/etc/fstab`에 추가 (UUID는 위에서 확인한 값으로):

```
UUID=XXXX-XXXX  /mnt/data  exfat  defaults,uid=1000,gid=1000,umask=022,nofail  0 0
```

`nofail` 옵션이 중요하다. 없으면 마운트 실패 시 부팅이 멈춘다.

```bash
sudo mount -a
```

## 6-2. USB 자동절전 비활성화 (권장)

외장 부팅 시 USB 전원 절약 때문에 드라이브가 잠깐 끊기면 시스템이 멎을 수 있다.

```bash
sudo grubby --update-kernel=ALL --args="usbcore.autosuspend=-1"
sudo reboot
```

## 6-3. 다른 PC 호환 (지금은 불필요)

노트북 전용이면 건너뛰어도 된다. 다만 나중에 조립식 PC에 꽂아볼 가능성이 있다면 한 줄이라 미리 넣어둬도 손해는 없다:

```bash
echo 'hostonly=no' | sudo tee /etc/dracut.conf.d/no-hostonly.conf
sudo dracut -f --regenerate-all
```

RHEL 계열은 기본적으로 설치한 하드웨어에만 맞춘 initramfs를 만들기 때문에, 이 설정 없이는 다른 기기에서 부팅이 실패한다.

---

# 트러블슈팅

|증상|원인 / 대처|
|---|---|
|부팅 메뉴에 T7이 안 뜸|Security Mode가 안 꺼졌거나, Fast Boot가 켜져 있음. 허브 경유 여부도 확인|
|설치 USB가 인식 안 됨|Rufus를 DD 모드로 다시 굽기. Secure Boot 임시 해제 시도|
|Anaconda에 T7이 안 보임|케이블 재연결 후 화면 우측 상단 새로고침. C to C 직결인지 확인|
|설치 후 윈도우가 안 뜸|BIOS 부팅 순서 1순위를 `Windows Boot Manager`로 변경|
|부팅 중 멈춤 / I/O 에러|6-2의 usbcore.autosuspend 적용|
|윈도우가 복구키 요구|0-3에서 확보한 48자리 키 입력|
|GRUB이 뜨는데 커널 로딩 실패|설치 시 `/boot`가 T7에 제대로 생성됐는지 재확인|

---

# 요약 체크리스트

- [ ] 0-1. Security Mode OFF + 재연결 확인
- [ ] 0-2. GPT 여부 확인 (diskpart)
- [ ] 0-3. BitLocker 복구키 저장
- [ ] 0-4. 유일본 파일 대피
- [ ] 0-5. 충전기 + C to C 직결
- [ ] 1-2. exFAT를 450GB로 축소, 미할당 481GB 확보
- [ ] 2-2. Rufus DD 모드로 설치 USB 제작
- [ ] 3-2. Fast Boot 끄기
- [ ] 4-2. **T7만 체크**, Custom 선택
- [ ] 4-3. `/boot/efi` 포함 5개 파티션 생성, 전부 T7에 지정
- [ ] 4-3. Summary of Changes에 내장 디스크 항목 없음 확인
- [ ] 5-2. efibootmgr 확인
- [ ] 5-3. T7 뽑고 윈도우 정상 부팅 확인
