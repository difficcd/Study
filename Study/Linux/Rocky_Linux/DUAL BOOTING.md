

# Arch dual booting vs. Rocky dual booting

>환경 차이 이해하기


이렇게 설치하는건 개인 PC에서 OS 깔았던거랑 뭐가 다른가?
아치 깔때는 USB에다가 미니리눅스배포판으로 시작해서 아치리눅스 본체를 까는 방식이었는데 지금은 USB라는 temp OS는 필요하지만 SSD가 F:(1TB HDD,  PC에 아치 듀얼부팅할 때 파티셔닝했던 디스크) 역할을 대신 해주는 느낌인건가?

SSD 여도 어차피 연결을 해버리면 노트북의 일원처럼 쓰고 조회하고 할 수 있으니까

=>

| 아치 (조립식 PC)             | 지금 (노트북)                 |
| ----------------------- | ------------------------ |
| USB의 아치 라이브 = 임시 OS     | USB의 Rocky 설치미디어 = 임시 OS |
| F: (내장 1TB HDD) = 설치 대상 | T7 = 설치 대상               |
| `fdisk`로 수동 파티셔닝        | Anaconda GUI로 파티셔닝       |
| `pacstrap`으로 시스템 설치     | 설치 프로그램이 자동              |

핵심 차이는 **설치 대상이 내부냐 외부냐** 하나뿐임. 그리고 그건 사실 차이가 아님 — 네가 말한 대로 USB로 연결된 순간 커널 입장에선 그냥 블록 디바이스 하나임. 내장 NVMe는 `/dev/nvme0n1`, T7은 `/dev/sda`. 이름만 다르고 취급이 같음.

부팅 시점도 마찬가지. UEFI 펌웨어가 부팅 장치를 훑을 때 USB 저장장치도 후보에 포함됨. F10 눌러서 T7 고르면 거기 EFI 파티션 읽고 GRUB 띄우는 것뿐임.






### 아치 설치랑 비교

**네 이해가 정확함.** 구조가 똑같음.

|아치 (조립식 PC)|지금 (노트북)|
|---|---|
|USB의 아치 라이브 = 임시 OS|USB의 Rocky 설치미디어 = 임시 OS|
|F: (내장 1TB HDD) = 설치 대상|T7 = 설치 대상|
|`fdisk`로 수동 파티셔닝|Anaconda GUI로 파티셔닝|
|`pacstrap`으로 시스템 설치|설치 프로그램이 자동|

핵심 차이는 **설치 대상이 내부냐 외부냐** 하나뿐임. 그리고 그건 사실 차이가 아님 — 네가 말한 대로 USB로 연결된 순간 커널 입장에선 그냥 블록 디바이스 하나임. 내장 NVMe는 `/dev/nvme0n1`, T7은 `/dev/sda`. 이름만 다르고 취급이 같음.

부팅 시점도 마찬가지. UEFI 펌웨어가 부팅 장치를 훑을 때 USB 저장장치도 후보에 포함됨. F10 눌러서 T7 고르면 거기 EFI 파티션 읽고 GRUB 띄우는 것뿐임.

### clean이 뭐냐

**포맷보다 더 근본적임.**

- **포맷**: 파티션은 그대로 두고, 그 안의 파일 목록만 초기화
- **clean**: 파티션 테이블 자체를 지움 → 파티션이라는 개념이 통째로 사라짐

방 안 짐만 비우는 게 포맷이면, clean은 집 도면을 찢는 거임.

**데이터 있는 상태로 해도 되냐** — 명령은 정상 실행되지만 **데이터는 못 씀.** 파일 본체는 물리적으로 남아있는데 어디에 뭐가 있는지 적힌 목차가 사라져서 접근이 불가능해짐. 그래서 백업을 먼저 뜬 거고, 순서가 반대였으면 4시간짜리 복구 작업이 됐을 거임.

(참고로 이 성질 때문에 TestDisk 같은 도구로 되살릴 여지는 있음. 하지만 그거 믿고 백업 생략하는 건 도박임.)



## USB 세팅에서의 차이점 : iso vs. dd

(DD모드 == Data Duplicator모드. iso파일 내부 데이터를 파일단위로 해석하지 않고
1B도 빠짐없이 있는 그대로 USB 드라이브에 raw sector(바이너리 수준)으로 덮어쓰는 방식.

- **ISO 모드 (파일 시스템 추출 방식):** ISO 내부의 파일들을 추출하여 USB를 FAT32/NTFS 등으로 포맷한 뒤 파일 형태로 복사합니다. 그 후 별도의 부트로더(GRUB, Syslinux 등)를 덮어씌워 부팅 가능하게 만듭니다. Windows나 일부 일반적인 리눅스에서 잘 작동합니다.
    
- **DD 모드 (바이너리 원본 복사 방식):** ISO 파일 자체가 이미 완전한 파티션 구조와 부팅 레코드를 포함하도록 만들어진 **하이브리드 ISO(ISOHybrid)** 일 때 사용합니다. USB 전체를 ISO 구조 그대로 덮어쓰므로 포맷 과정이나 파일 추출 없이 '원판 그대로' 복제됩니다.

록키 리눅스(Red Hat 계열) ISO는 **아나콘다(Anaconda) 설치 프로그램**과 **특수한 부팅 파티션 구조**를 사용하기 때문에 DD모드로 구워야함:

- **라벨(Volume Label) 및 인스톨러 미스매치:** ISO 모드로 구우면 툴이 파일 시스템이나 볼륨 라벨을 변경할 수 있습니다. 록키 설치 프로그램은 부팅 시 특정 라벨(예: `LABEL=Rocky-9-x86_64-dvd`)을 찾아 설치 미디어를 마운트하는데, ISO 모드로 가공되면 이 라벨을 찾지 못해 `dracut` 셸 에러가 발생하며 부팅에 실패합니다.

- **하이브리드 구조 보존:** 록키 개발진은 ISO 자체를 USB에 바로 `dd`로 복사해도 부팅되도록 특수하게 패키징해둡니다. DD 모드로 구워야 록키 개발팀이 의도한 EFI/MBR 부트 로더와 라벨이 훼손 없이 정확히 들어가 부팅 오류가 생기지 않습니다.

아치 리눅스도 최근 이미지들은 하이브리드 형태라 DD 모드를 권장하지만, Rufus 등에서 ISO 모드 호환 처리가 잘 되어있어 그냥 넘어갔을 수 있습니다. 반면 록키/RHEL 계열은 ISO 모드 변형에 매우 민감하여 DD 모드가 필수적입니다.
(간단히 말해서 로키리눅스자체가 DD모드로 구워질걸 상정하고 ISO파일을 배포한다고 생각하면 된다는 것.  + 인스톨러의 특성임.)


### 핵심 원인: 설치 디바이스를 찾아서 마운트하는 방식의 차이

**1. 아치 리눅스 (Arch Linux)**

- **설치 방식:** 별도의 복잡한 그래픽 인스톨러가 없으며, 부팅 후 라이브 쉘 환경으로 들어가 사용자가 직접 파티션을 잡고 명령어로 설치합니다.
    
- **마운트 방식:** 부팅 시 파티션 라벨이 약간 달라져도, ISO 내부의 자그마한 초기 램디스크(archiso)가 알아서 USB 디바이스를 찾아 라이브 환경을 띄워줍니다.
    
- Syslinux/GRUB 등 어떤 부트로더를 쓰든 **Rufus 같은 툴이 USB를 FAT32로 재포맷하고 파일만 추출해 옮겨 담아도(ISO 모드)** 부팅 과정이 유연하게 넘어가줍니다.
    

**2. 록키 리눅스 (Rocky Linux)**

- **설치 방식:** Anaconda(아나콘다)라는 거대한 엔터프라이즈 전용 그래픽/텍스트 인스톨러가 실행됩니다.
    
- **마운트 방식:** 부팅 시 하드웨어 호환성과 보안, 설치 파일 무결성을 매우 엄격하게 체크합니다. 부트로더 설정에 아예 **`inst.stage2=hd:LABEL=Rocky-9-x86_64-dvd`** 처럼 "정확히 이 볼륨 라벨을 가진 디바이스에서 설치 파일 패키지를 로드해라"라고 **하드코딩**되어 있습니다.
    
- **ISO 모드의 문제:** Rufus 등에서 ISO 모드로 구우면 USB를 FAT32/NTFS로 포맷하면서 **볼륨 라벨이 잘리거나 변경**되기도 하고, 부트로더 설정을 툴 임의로 수정합니다. 이 때문에 아나콘다가 설치 패키지 위치를 못 찾고 `dracut-initqueue timeout` 에러를 뿜으며 멈춥니다.
    
- **DD 모드가 필수인 이유:** ISO 바이너리 상태 그대로 USB 섹터에 1:1로 밀어 넣어야만, 록키 개발팀이 지정해둔 파티션 레이아웃과 볼륨 라벨이 1비트의 오차도 없이 유지되어 인스톨러가 정확히 설치 미디어를 인식합니다.



# SSD: MBR to GPT 

> 내부 백업 파일 잠시 PC HDD로 옮겨두고 진행

`diskpart`
`list disk`

두 가지 명령어로 현재 연결된 disk 상태 볼 수 있음.
GPT 에 \*이 있으면 GPT고, \*이 없으면 MBR.
(\*이 없으면 파티션 테이블 자체가 없거나(RAW, Status가 Not Initialized로 뜸) 
  동적 디스트(Dyn : \*)인 것이 아닌 경우 MBR임.)


![[Pasted image 20260814131942.png|367]]

### MBR을 GPT로 바꾸는 이유

**1. UEFI가 MBR 디스크에서 부팅을 안 함**

요즘 펌웨어(UEFI)는 OS를 이렇게 찾음: 디스크의 파티션 목록을 읽고 → 그중 "EFI 시스템 파티션"이라고 표시된 걸 찾아서 → 그 안의 `.efi` 파일을 실행함.

문제는 "EFI 시스템 파티션"이라는 **표식이 GPT에만 있는 개념**이라는 것. MBR 파티션 항목에는 그 표식을 적을 칸이 없음. 그래서 UEFI가 MBR 디스크를 보면 "부팅 가능한 게 없네" 하고 넘어감.

MBR로도 부팅하려면 펌웨어를 CSM(레거시 호환 모드)으로 돌려야 하는데, 그러면 내장 윈도우가 안 켜짐. 윈도우는 GPT + UEFI로 설치돼 있으니까. 둘 중 하나만 골라야 하는 상황이 됨.

(+ Rocky 자체는 BIOS, UEFI 둘 다 지원하는데, C:의 윈도우가 UEFI라서 둘을 통일해야함 : 레거시로가면 BIOS에서 CSM 켜야하는데 그러면 윈도우를 못 킨다고 함.)


**2. 파티션 개수 제한**

MBR은 디스크 맨 앞 512바이트 안에 파티션 정보를 적는데, 거기 배정된 공간이 16바이트 × 4개뿐임. 즉 **파티션 4개가 물리적 한계**.

우리 계획은 6개임:

```
exFAT(데이터) / EFI / boot / swap / root / home
```

확장 파티션이라는 우회책이 있긴 한데, 리눅스 부팅 파티션을 거기 넣으면 골치 아파짐.

**덤:** GPT는 파티션 테이블 백업본을 디스크 맨 뒤에도 둠. MBR은 앞의 512바이트가 깨지면 끝. 안정성도 더 나음.

파티션 6개를 역할별로 나눠보면:

| 파티션       | FS    | 소유  | 용도                            |
| --------- | ----- | --- | ----------------------------- |
| **exFAT** | exFAT | 공용  | **네 백업 데이터.** 윈도우/리눅스 양쪽에서 읽힘 |
| EFI       | FAT32 | 펌웨어 | 부트로더 `.efi` 파일                |
| /boot     | ext4  | 리눅스 | 커널, initramfs                 |
| swap      | swap  | 리눅스 | 메모리 부족 시 대피처                  |
| /         | xfs   | 리눅스 | OS 본체                         |
| /home     | xfs   | 리눅스 | 사용자 파일                        |

즉 T7 하나를 **"백업 창고 531GB" + "리눅스 400GB"** 로 쪼개는 거고, exFAT는 앞쪽 창고 담당임. 리눅스 설치와는 무관하고, 원래 쓰던 백업 드라이브 역할을 계속 하는 거.

---

## T7 GPT 전환

**노트북에서** 진행. T7 연결하고 탐색기 E: 창은 다 닫기.

1. 시작 우클릭 → `터미널(관리자)`
2. `diskpart` 입력

```
list disk
```

3. **Disk 1이 931GB인지 눈으로 확인.** Disk 0(238GB)이면 절대 안 됨

```
select disk 1
detail disk
```

4. 출력 첫 줄에 **`Samsung PSSD T7`** 뜨는지 확인. 아니면 여기서 멈추고 알려줘

```
clean
convert gpt

create partition primary size=543744
format fs=exfat quick label=T7
assign letter=E

list partition
exit
```

clean = 파티션 테이블 자체를 지워, 파티션 개념을 통째로 지워버리는 것
format = 파티션은 그대로 두고 그 안의 파일목록만 초기화하는 것
(참고로 clean 은 그냥 데이터 있는 상태로 해도 되지만 개념 그대로 다 날리는 명령어이니 백업을 해두고 세팅을 해주자.)

convert gpt 한 줄로 MBR=>GPT
primary partition = 컴퓨터 disk 를 사용하기 위해 나누는 파티션의 가장 기본적인 형태.
OS 설치하거나, 데이터 저장하기 위한 일반 볼륨으로 사용가능한기본 단위.
GPT에서는 최대 128개까지 만들 수 있음. (MBR에선 알고있듯 4개가 최대고)

format 은 fs를 exfat (MS개발 fs, 대용량파일저장에 최적화. ext4랑 헷갈리지말기..)로 만드는거고
quick 은 format의 옵션으로 빠른 포맷(드라이브의 기존 파일목록, 주소록:파일할당테이블)만 싹 지우고 새틀 짜는 방식. 실재 데이터가저장된 공간까지 밀지 않는 방식.. 그냥 포맷을 하면 디스크 전체를 하나하나 훑으면서 배드섹터 검사/기존데이터를 완전히 덮어씌우기를 해야 해서 시간이 더 오래 걸림.

5. 결과 확인:
    - 파티션 1이 약 531GB
    - 탐색기에 E: 드라이브가 빈 상태로 뜸




# Setting USB 

- `rockylinux.org/download` → **Rocky Linux 10.x, x86_64, DVD ISO** 다운로드
- `rufus.ie` → Rufus 다운로드 (포터블판 권장)
- USB 메모리(iso 보다 큰 용량이어야 함) 연결

### Rufus 설정

1. **장치**: `NO_LABEL (D:) [16 GB]` 맞는지 확인
    - T7(E:)이 아닌지 재확인
2. **부팅 선택** → `선택` 버튼 → 받은 Rocky ISO 지정
3. ISO 선택 후 아래 항목 확인:
    - 파티션 방식: **`GPT`**
    - 대상 시스템: **`UEFI (CSM 지원 안 함)`**
    - 파일 시스템: 기본값 그대로
4. `시작` 클릭
5. **모드 선택 창이 뜨면 → `DD 이미지 모드로 쓰기` 선택** → 확인
6. 데이터 삭제 경고 → 확인
7. 완료(`준비` 표시) 대기, 10~20분 정도 (준비 표시 되면 닫기)


### BIOS 설정

노트북 **완전 종료** (재시작 아님)

전원 버튼 누른 직후 **F2(F10) 연타** → BIOS 진입 
	(fast booting 때문에 안되면 윈도우 시작메뉴에서 shift 누르고 다시 시작.)

- **Secure Boot Key Set** (또는 자격 키셋) → `Secure Boot Supported OS`
- **Secure Boot Control** → `Enabled` 유지
- **Fast BIOS Mode** → `Disabled`
- `F10` → Save & Exit
- BitLocker 복구키 물으면 48자리 입력

재부팅 후 USB 꽂고 F10(또는 Shift+다시 시작 → 장치 사용)으로 부팅 시도.
Install Rocky Linux 10.2 선택하면 됨. (test media 는 몇 분 더 걸리고, fips는 정부 금융용 암호화 규격 모드)

해상도 문제가 있으면 arch의 nomodeset처럼 

- `Install Rocky Linux 10.2`에 커서 올린 상태에서 **`e`** 키
- 편집 화면에서 **`linux` 또는 `linuxefi`로 시작하는 줄** 찾기 (보통 두 번째~세 번째 줄)
- `End` 키로 그 줄 맨 끝으로 이동
-  스페이스 한 칸 띄우고 아래 입력:

```
inst.resolution=1600x900
```

 -  **`Ctrl + X`** 로 부팅

이렇게 해주면 됨 (1280x1024도 괜찮고..)
근데 나는 이렇게 해도 안 먹어서 그냥 했음. 어차피 arch처럼 계속 cli 만 보고있을 것도 아니니..

이렇게 해서 부팅을 해주면 
arch와는 다르게 무려 ubuntu vm에서 설치했을 때처럼 GUI가 뜸!!
iso파일이 좀 더 무겁게 느껴졌던 거에 (아마 실제로 더 무거웠을듯) GUI 의 비중이 좀 기여했을거같음

여튼 여기서 설정으로 넘어감.


---

ref ) 디스크 축소 등 준비과정에서 충전기 사용해야 하는 이유: 
- **절전 개입.** 배터리 모드에서 윈도우가 USB 포트 전원을 줄이거나 디스크를 잠깐 재우는 경우가 있음. 파티션 테이블 쓰는 중에 T7이 순간적으로 끊기면 그게 제일 나쁜 시나리오임. 충전기 꽂으면 전원 계획이 고성능 쪽으로 붙어서 이 개입이 안 일어남.
- **T7 전력.** T7은 USB 포트에서 전력을 다 끌어씀. 배터리 모드에선 포트 출력이 제한될 수 있음.


# Anaconda install setting

## 유닛 6-1 — 언어 선택

1. 목록에서 **`English`** 선택 (오류 검색할 때 편함. 한국어 원하면 `한국어`도 무방)
2. 우하단 **`Continue`** 클릭

## 유닛 6-2 — 설치 디스크 선택 ⚠️ 가장 중요

1. **`Installation Destination`** 클릭
2. `Local Standard Disks` 목록에서:
    - **`Samsung PSSD T7` (931.51 GiB) 만** 클릭해서 체크 표시 넣기
    - **내장 디스크(238.47 GiB, SAMSUNG MZVL2256)는 체크 해제** — 기본으로 체크돼 있으면 클릭해서 반드시 해제
3. 하단 `Storage Configuration`에서 **`Custom`** 선택
4. 우상단 **`Done`**

## 유닛 6-3 — 파티션 생성

`MANUAL PARTITIONING` 화면일 거임.

1. 좌측 상단 드롭다운을 **`Standard Partition`** 으로 변경 (LVM 아님)
2. 좌측에 `Unknown` 아래 기존 exFAT 파티션이 보이면 **건드리지 말 것**
3. 좌하단 **`+`** 버튼으로 아래를 순서대로 생성:

|순서|Mount Point|Desired Capacity|
|---|---|---|
|1|`/boot/efi`|`1024 MiB`|
|2|`/boot`|`2048 MiB`|
|3|`swap`|`8192 MiB`|
|4|`/`|`120 GiB`|
|5|`/home`|**비워두기**|

- Mount Point는 직접 타이핑하면 됨 (`swap`은 드롭다운에 있음)
- 5번은 용량 칸 비워두면 남은 공간 전부 할당됨. (다른 곳들은 용량 단위까지 꼭 써주기)

-  각 파티션 만든 뒤 우측 패널에서 **`Device(s)`가 `Samsung PSSD T7`인지** 확인
-  `/`와 `/home`의 File System은 **`xfs`**, `/boot`는 **`ext4`** 로 지정



## 유닛 6-4 — 변경사항 확인

1. 좌상단 **`Done`** 클릭
2. `SUMMARY OF CHANGES` 창이 뜸
3. **여기서 확인:** 모든 항목이 **`sda`** 로 시작하는지. `nvme0n1`이 하나라도 있으면 즉시 `Cancel`
	(nvme0n1이 있으면 윈도우를 건드린다는거임.. C:가 NVMe SSD 라서, 리눅스가 C:를 이런식으로 잡음.)
4. sda만 있으면 **`Accept Changes`**

## 유닛 7 — 나머지 설정

`INSTALLATION SUMMARY` 화면에서 순서대로:

**1. Software Selection**

- 좌측에서 **`Workstation`** 선택 (GUI 데스크톱 환경 포함)
- `Done`

**2. Network & Host Name**

- Wi-Fi 켜고 연결
- 좌하단 Host Name(네트워크에서 PC부르는이름)에 원하는 이름 입력 → `Apply`
- `Done`

**3. Time & Date**

- 지도에서 `Asia/Seoul` 선택
- `Done`

**4. Root Password**

- 비밀번호 설정 (또는 `Lock root account` 체크)
- `Done`

**5. User Creation**

- 사용자명 / 비밀번호 입력
- **`Make this user administrator` 체크** ← 중요
- `Done`

ref ) **username을 `admin`으로 하는 건 비추.** 동작은 하지만:
- 무차별 대입 공격이 제일 먼저 시도하는 이름임
- 일부 서비스가 `admin` 계정을 자체적으로 만들면서 충돌할 여지가 있음

위 사항들 모두 설정해준 다음
**`Begin Installation`** 누르면 됨.

---
이후에는 재부팅.

**재부팅 순서:**

1. `Reboot System` 클릭
2. 화면이 꺼지고 삼성 로고 나오기 직전에 **USB 뽑기**
3. 그대로 두면 자동으로 T7의 GRUB이 뜰 수도 있고, 윈도우로 갈 수도 있음

**윈도우로 넘어가버리면** — 정상임. 놀라지 말고:

1. 다시 종료
2. USB 없이, T7만 연결한 채로 전원
3. **F10 연타** → 부팅 메뉴 → `UEFI: Samsung PSSD T7` 선택
(F10 : fast bios mode 꺼야 먹히는 거일듯)

# 설치 성공 후 확인

## 유닛 8 — 첫 부팅 검증

f10 GRUB 부팅 메뉴에서..
맨 위 `Rocky Linux (숫자...) 10.2` 선택 → Enter
(0-rescue는 복구용, 나머지 둘은 윈도우/BIOS 진입용)


로그인해서 터미널 열고 (Activities → Terminal 또는 `Ctrl+Alt+T`):

```bash
lsblk -f
```

→ sda3~sda7이 각각 마운트돼 있는지 확인


```bash
df -h /
```

→ 120G 루트가 잡혔는지


```bash
sudo efibootmgr -v
```

→ `Rocky Linux` 항목과 `Windows Boot Manager` 항목이 **둘 다** 있는지



# 블루투스 설정하기

참고로 모니터 설정은 super + p. 윈도우와 동일.


**블루투스**

터미널에서:

```bash
rfkill list
systemctl status bluetooth
```

`Soft blocked: yes`면:

```bash
sudo rfkill unblock bluetooth
sudo systemctl enable --now bluetooth
```

그 다음 `Settings` → `Bluetooth`에서 마우스 페어링.

---
아래는 블루투스 키보드 페어링을 못 하길래 처리한 과정 (수동 페어링)

터미널에서 수동 페어링:

bash

```bash
bluetoothctl
```

프롬프트가 `[bluetooth]#`로 바뀌면 한 줄씩:

```
power on
agent KeyboardDisplay
default-agent
scan on
```

키보드를 페어링 모드로 두고 10초쯤 기다리면 목록이 뜸. 키보드 MAC 주소(`XX:XX:XX:XX:XX:XX`) 확인 후:

```
scan off
pair XX:XX:XX:XX:XX:XX
```

**여기서 화면에 6자리 숫자가 뜨면, 그 숫자를 블루투스 키보드로 입력하고 Enter.** 이 단계가 핵심임 — GUI 설정에서는 이 프롬프트를 놓쳐서 실패하는 경우가 많음.

성공하면:

```
trust XX:XX:XX:XX:XX:XX
connect XX:XX:XX:XX:XX:XX
exit
```

---

`scan on`에서 키보드가 아예 안 보이면:

- 키보드가 다른 기기(노트북 윈도우, 폰)에 이미 연결돼 있는지 확인 — 대부분 한 번에 하나만 붙음
- 키보드 페어링 버튼 길게 눌러 LED 깜빡이는 상태 만들기
- 멀티페어링 지원 모델이면 빈 슬롯(Fn+1/2/3)으로 전환

(블루투스 키보드는 OS 끼리 페어링을 해두는걸 다르게, 멀티페어링 해둬야 킬때마다 잘 잡힘 (+trusted확인). 지금 윈도우가 BT2?인거로 알고있으니.. 혹시라도 로키에서 겹쳤으면 BT1로 해두면됨.)



# Rocky 에서 한국어 사용 세팅

터미널에서:

```bash
sudo dnf install -y langpacks-ko ibus-hangul
```

없다고 나오면 EPEL 먼저:

```bash
sudo dnf install -y epel-release && sudo dnf install -y ibus-hangul
```

설치 후 **로그아웃 → 다시 로그인** (필수).

그 다음:

1. `Settings` → `Keyboard` → `Input Sources` → `+`
2. `Korean` → **`Korean (Hangul)`** 선택 → Add
    - 그냥 `Korean`은 자판 배열만 바뀌고 한글 입력이 안 됨. **(Hangul)** 붙은 걸 골라야 함
3. 전환: `Super + Space`

**한/영 키로 전환하려면** 터미널에서:

```bash
ibus-setup
```

→ `Hangul` 탭 → `Hangul Key` → `Add` → 키보드의 한/영 키 누르기 (인식 안 되면 `Alt_R` 직접 입력)


참고로 로그아웃 방법 : UI도 있지만 터미널에서는 : 
```bash
gnome-session-quit --logout
```



# 기본 저장소에 없는 APP/SW 다운받기

**크롬 설치**

기본 저장소에 없어서 구글 저장소를 추가해야 함:

```bash
sudo tee /etc/yum.repos.d/google-chrome.repo <<'EOF'
[google-chrome]
name=google-chrome
baseurl=https://dl.google.com/linux/chrome/rpm/stable/x86_64
enabled=1
gpgcheck=1
gpgkey=https://dl.google.com/linux/linux_signing_key.pub
EOF

sudo dnf install -y google-chrome-stable
```

설치 후 `Super` → `chrome` 검색하면 뜸.

> RPM 직접 받아서 `sudo dnf install ./파일.rpm` 해도 되지만, 위 방식은 `dnf update`에 크롬이 같이 물려서 자동 업데이트됨. 이게 나음. <== !!


**VS Code**

```bash
sudo rpm --import https://packages.microsoft.com/keys/microsoft.asc
sudo tee /etc/yum.repos.d/vscode.repo <<'EOF'
[code]
name=Visual Studio Code
baseurl=https://packages.microsoft.com/yumrepos/vscode
enabled=1
gpgcheck=1
gpgkey=https://packages.microsoft.com/keys/microsoft.asc
EOF

sudo dnf install -y code
```

---

**옵시디언**

공식 저장소가 없어서 Flatpak이 제일 깔끔함:

```bash
sudo dnf install -y flatpak
flatpak remote-add --if-not-exists flathub https://dl.flathub.org/repo/flathub.flatpakrepo
flatpak install -y flathub md.obsidian.Obsidian
```

설치 후 `Super` → `obsidian`으로 실행.

---

오 보인다 보여

지금은 클릭할 때마다 임시로 마운트되는 상태임. 부팅할 때 자동으로 고정 경로에 붙게 해두자. 옵시디언 볼트나 VS Code 작업 폴더를 거기 둘 거면 이게 필요함.

**1. UUID 확인**


```bash
lsblk -f /dev/sda2
```

`UUID` 칸의 값(`XXXX-XXXX` 형태) 메모.

**2. 마운트 지점 만들기**


```bash
sudo mkdir -p /mnt/t7
```

**3. fstab 등록**


```bash
sudo nano /etc/fstab
```

맨 아래에 한 줄 추가 (UUID는 1번 값으로):

```
UUID=XXXX-XXXX  /mnt/t7  exfat  defaults,uid=1000,gid=1000,umask=022,nofail  0 0
```

`Ctrl+O` → Enter → `Ctrl+X`

**4. 적용 및 확인**

```bash
sudo mount -a
ls /mnt/t7
```

에러 없이 목록 나오면 끝. 다음 부팅부터 자동으로 붙음.

> `nofail`이 중요함. 이게 없으면 나중에 파티션에 문제가 생겼을 때 부팅이 멈춤.





# 온도 관리 등 기타 명령어

### 온도 확인

**CPU — lm_sensors**

```bash
sudo dnf install -y lm_sensors
sudo sensors-detect --auto
sensors
```

`Package id 0`이 CPU 전체 온도, `Core 0~N`이 코어별 온도임.

실시간으로 보려면:

```bash
watch -n 2 sensors
```

**T7 — smartctl**

USB 연결이라 `-d sat` 옵션이 필요함:

```bash
sudo dnf install -y smartmontools
sudo smartctl -d sat -a /dev/sda | grep -i temperature
```

이걸로 안 나오면:

```bash
sudo smartctl -d sntasmedia -a /dev/sda | grep -i temperature
```

**둘 다 한 번에**

```bash
sensors; echo "--- T7 ---"; sudo smartctl -d sat -a /dev/sda | grep -i temperature
```

---

### GUI 툴

**Mission Center** — 윈도우 작업관리자랑 제일 비슷함. 온도도 같이 나옴:

```bash
flatpak install -y flathub io.missioncenter.MissionCenter
```

**상단바에 온도 상시 표시** — GNOME 확장 `Vitals`:

1. 크롬으로 `extensions.gnome.org` 접속
2. `Vitals` 검색 → 토글 켜기
3. 브라우저 연동 안 되면: `sudo dnf install -y gnome-browser-connector` 후 재시도


free -h (available 메모리 볼 수 있음. 윈도우에서의 사용 가능 : xx... 이거랑 비교가능)

#