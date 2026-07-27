# Arch Linux 듀얼부팅 계획서

> 작성일: 2026-07-23
> 대상 PC: Gigabyte H110M-DS2V / i5-7400 / RAM 8GB / GTX 1050
> **이 문서는 계획서입니다. 실행은 본인이 직접 진행.**

---

## 0. 한 줄 요약

이 PC는 **Legacy BIOS + MBR** 환경이다. 요즘 인터넷에 널린 Arch 설치 가이드는
거의 다 **UEFI + GPT** 기준이라 **그대로 따라하면 부팅 안 된다.**
반드시 이 문서의 BIOS/MBR 방식(`grub-install --target=i386-pc`)으로 진행할 것.

---

## 1. 현재 시스템 상태 (실측값)

### 1-1. 하드웨어

| 항목 | 값 | 비고 |
|------|-----|------|
| 메인보드 | Gigabyte H110M-DS2V | LGA1151 |
| CPU | Intel Core i5-7400 (Kaby Lake) | → `intel-ucode` 필요 |
| RAM | 8 GB | swap 계획에 영향 |
| GPU | NVIDIA GeForce GTX 1050 (Pascal) | → 독점 `nvidia` 드라이버 |
| 펌웨어 부팅 모드 | **Legacy BIOS (CSM)** | UEFI 아님. 확정됨 |
| OS | Windows 10 Pro 19045 | |

### 1-2. 디스크 구성

**Disk 0 — TS120GSSD220S (120GB SSD, SATA, MBR)**

| 파티션 | 드라이브 | 크기 | 역할 |
|--------|----------|------|------|
| 1 | (없음) | 0.5 GB | 시스템 예약 (active) |
| 2 | **C:** | 110.8 GB | Windows (boot) / 여유 **30.3 GB** |
| 3 | (없음) | 0.5 GB | 복구 파티션 |

→ MBR 주 파티션 **3/4 사용. 남은 슬롯 1개.**

**Disk 1 — WDC WD10EZEX-00MFCA0 (1TB HDD, SATA, MBR)**

| 파티션 | 드라이브 | 크기 | 사용량 |
|--------|----------|------|--------|
| 1 | **F:** | 931.5 GB | 68 GB 사용 / **863 GB 여유** |

→ MBR 주 파티션 **1/4 사용. 남은 슬롯 3개.**

**Disk 2 — Samsung PSSD T7 (1TB, USB, exFAT)** = E: 드라이브
→ 백업 전용. **설치 중에는 반드시 물리적으로 분리.**

### 1-3. ⚠️ 가장 위험한 함정

**WD 1TB HDD와 T7 외장 SSD가 둘 다 정확히 931.5GB다.**
설치 화면에서 용량만 보고 고르면 **100% 헷갈린다.**
반드시 **모델명으로 구분**하고, 설치 중엔 T7을 뽑아둘 것.

```bash
lsblk -o NAME,SIZE,MODEL,TYPE    # 반드시 MODEL 컬럼 확인
```

---

## 2. 설치 전 필수 준비

### 2-1. 백업 (최우선)

- [x] F: 유저데이터 → `E:\MyPCBackUp\`
- [x] 그림 작업물 → `E:\DRAW\`
- [ ] **백업본 무결성 직접 눈으로 확인** (DRAW 폴더 열어서 파일 열리는지 확인)
- [ ] 확인 끝나면 **T7 물리적으로 분리**

### 2-2. Windows 빠른 시작 끄기 (필수)

`제어판 → 전원 옵션 → 전원 단추 작동 설정 → 현재 사용할 수 없는 설정 변경`
→ **"빠른 시작 켜기" 체크 해제**

**안 끄면**: Windows가 종료 시 완전히 안 꺼지고 최면 상태로 들어간다.
이 상태에서 Linux가 NTFS(F:)에 쓰기를 하면 **파일시스템이 깨진다.**

### 2-3. BitLocker 확인

```powershell
manage-bde -status
```
켜져 있으면 파티션 건드리기 전에 반드시 해제. (Pro 에디션이라 켜져 있을 수 있음)

### 2-4. BIOS 설정 확인

- **CSM / Legacy Support: Enabled 유지**
- Secure Boot: Disabled (Legacy면 어차피 해당 없음)
- **UEFI 모드로 바꾸지 말 것** — 바꾸면 기존 Windows가 부팅 불가능해진다

---

## 3. 파티션 계획 (3가지 안)

### ⭐ 안 A — Arch를 HDD에 설치 (가장 안전 / 추천)

F:를 줄여서 Disk 1에 Arch를 설치한다.

```
Disk 1 (1TB HDD)
├─ F: (NTFS)         200~400 GB   ← Windows용 데이터
└─ Arch              500~700 GB
   ├─ /              (ext4)
   └─ swapfile       8 GB (파티션 아님, 파일로)
```

**장점**
- Windows 디스크(Disk 0)를 **전혀 안 건드림** → Windows 깨질 위험 최소
- 공간이 압도적으로 여유로움 (F:는 931GB 중 68GB만 씀)
- MBR 슬롯 3개 남아있어 여유

**단점**
- HDD라 SSD 대비 부팅/실행 체감 느림

---

### 안 B — Arch를 SSD에 설치 (빠르지만 빡빡)

C:를 줄여서 Disk 0의 마지막 MBR 슬롯에 설치.

```
Disk 0 (120GB SSD)
├─ 시스템 예약   0.5 GB
├─ C: (NTFS)    85 GB
├─ 복구         0.5 GB
└─ Arch         25 GB   ← 마지막 슬롯
```

**장점**: SSD라 빠름
**단점**
- C: 여유가 30.3GB뿐이라 실제로는 **20~25GB밖에 못 뺀다**
- Windows 축소는 이동 불가 파일 때문에 표시된 만큼 안 줄어드는 경우 많음
- **MBR 슬롯을 다 써버림** → 나중에 파티션 추가 불가
- 25GB는 Arch + NVIDIA 드라이버 + 데스크탑 환경 넣으면 빡빡함

---

### 안 C — 하이브리드 (성능/용량 절충, 난이도 상)

```
Disk 0 (SSD)  └─ /        25 GB   ← 시스템만, 빠름
Disk 1 (HDD)  ├─ F:       300 GB
              └─ /home    600 GB  ← 데이터, 용량 여유
```

**장점**: 부팅/프로그램 실행은 SSD 속도, 데이터는 HDD 용량
**단점**: 디스크 2개에 걸쳐 있어 설정 복잡, 둘 중 하나 고장나면 전체 영향

> **결론: 처음이면 안 A로 가자.** Windows 디스크를 안 건드리는 게
> 사고 확률을 가장 크게 낮춘다. 속도가 아쉬우면 나중에 안 C로 옮기면 됨.

---

## 4. 파티션 축소 (Windows에서 먼저)

**Linux 설치기에서 줄이지 말고, Windows 디스크 관리에서 먼저 줄일 것.**
NTFS를 가장 잘 아는 건 Windows 자신이다.

1. `Win + X` → **디스크 관리**
2. **F: 우클릭 → 볼륨 축소**
3. 축소할 공간 입력 (예: 600GB = `614400` MB)
4. 축소 후 생긴 공간은 **"할당되지 않음" 상태로 그대로 둘 것**
   (Linux 설치 때 사용)

축소가 잘 안 되면 (이동 불가 파일 때문):
- 조각 모음 실행
- 시스템 보호/복원 지점 임시 해제
- 페이지파일을 F:에서 C:로 이동 (F:에 `pagefile.sys` 있음 — 확인됨)

> ⚠️ F:에 `pagefile.sys`가 있다. 축소 전에
> `시스템 속성 → 고급 → 성능 → 가상 메모리`에서 **F: 페이지파일을 없음으로** 설정하고
> 재부팅해야 축소가 제대로 된다.

---

## 5. 설치 USB 만들기

1. https://archlinux.org/download/ 에서 ISO 다운로드
2. **서명 검증** (권장)
3. **Rufus** 설정:
   - 파티션 구성: **MBR**
   - 대상 시스템: **BIOS 또는 UEFI-CSM**
   - 쓰기 모드: **DD 이미지 모드**
4. 부팅 시 부팅 메뉴에서 **"UEFI:" 접두사가 없는 항목** 선택
   (UEFI로 부팅하면 안 됨)

---

## 6. 설치 절차

### 6-1. 부팅 모드 확인 (가장 먼저!)

```bash
ls /sys/firmware/efi
```

- **"No such file or directory" → 정상 (BIOS 모드).** 계속 진행
- 디렉토리가 나오면 → UEFI로 부팅된 것. **중단하고 재부팅해서 Legacy로 다시 부팅**

### 6-2. 네트워크

```bash
# 유선이면 보통 자동
ping -c 3 archlinux.org

# 무선이면
iwctl
> station wlan0 scan
> station wlan0 connect <SSID>
> exit
```

### 6-3. 디스크 확인 (신중하게!)

```bash
lsblk -o NAME,SIZE,MODEL,TYPE
```

예상 출력:
```
sda   111.8G  TS120GSSD220S          ← Windows. 건드리지 말 것
sdb   931.5G  WDC WD10EZEX-00MFCA0   ← 여기에 설치
```

> **⚠️ 반드시 MODEL 컬럼으로 확인.**
> `sda`/`sdb` 순서는 보장되지 않는다. 부팅할 때마다 바뀔 수 있다.

### 6-4. 파티션 생성

Windows에서 미리 축소해뒀으므로 빈 공간에 파티션만 만든다.

```bash
fdisk /dev/sdb        # ← 위에서 확인한 WD 디스크
```

```
n        새 파티션
p        primary
2        파티션 번호
(엔터)   시작 섹터 기본값
(엔터)   끝 섹터 기본값 (남은 공간 전부)
w        저장
```

> `p` 명령으로 **기존 F: 파티션(sdb1)이 그대로 살아있는지** 먼저 확인할 것.
> 만약 안 보이면 즉시 `q`로 저장 없이 종료.

### 6-5. 포맷 & 마운트

```bash
mkfs.ext4 /dev/sdb2
mount /dev/sdb2 /mnt
```

### 6-6. 베이스 설치

```bash
pacstrap -K /mnt base linux linux-firmware intel-ucode \
                 base-devel vim nano networkmanager grub os-prober ntfs-3g sudo
```

> `intel-ucode` 필수 (i5-7400)
> `os-prober`, `ntfs-3g` — Windows 감지 및 NTFS 접근용

### 6-7. fstab

```bash
genfstab -U /mnt >> /mnt/etc/fstab
cat /mnt/etc/fstab        # 확인
```

### 6-8. chroot 후 기본 설정

```bash
arch-chroot /mnt

# 시간대
ln -sf /usr/share/zoneinfo/Asia/Seoul /etc/localtime
hwclock --systohc

# 로케일 — /etc/locale.gen 에서 아래 두 줄 주석 해제
#   en_US.UTF-8 UTF-8
#   ko_KR.UTF-8 UTF-8
locale-gen
echo "LANG=en_US.UTF-8" > /etc/locale.conf

# 호스트명
echo "archbox" > /etc/hostname

# root 암호
passwd

# 사용자 추가
useradd -m -G wheel -s /bin/bash myname
passwd myname
EDITOR=nano visudo        # %wheel ALL=(ALL:ALL) ALL 주석 해제

# 네트워크
systemctl enable NetworkManager
```

### 6-9. GRUB 설치 ⭐ 여기가 핵심

```bash
# Legacy BIOS 이므로 반드시 i386-pc 타겟
grub-install --target=i386-pc /dev/sdb
```

> **`/dev/sdb`** — 파티션(`sdb2`)이 아니라 **디스크 전체**를 지정.
>
> **`--target=i386-pc`** — 이게 BIOS용. UEFI 가이드의
> `--target=x86_64-efi --efi-directory=/boot` 는 **이 PC에서 작동 안 함.**

Windows를 GRUB 메뉴에 띄우려면:

```bash
# /etc/default/grub 편집
nano /etc/default/grub
```
아래 줄의 주석을 해제 (없으면 추가):
```
GRUB_DISABLE_OS_PROBER=false
```

```bash
grub-mkconfig -o /boot/grub/grub.cfg
```

출력에 **`Found Windows ... on /dev/sda1`** 같은 줄이 보여야 성공.
안 보이면 → 7-1 참고.

### 6-10. 재부팅

```bash
exit
umount -R /mnt
reboot
```

USB 뽑고, **BIOS 부팅 순서에서 WD 디스크(sdb)를 1순위로** 설정.

---

## 7. 부팅 관련 주의사항

### 7-1. GRUB에 Windows가 안 뜰 때

`grub-install`을 어느 디스크에 했느냐에 따라 다르다.

- **방법 1 (추천)**: BIOS 부팅 순서를 바꿔서 어느 OS로 갈지 선택.
  Windows 쓸 땐 SSD 1순위, Arch 쓸 땐 HDD 1순위.
  → **가장 안전.** Windows 부트섹터를 전혀 안 건드림.

- **방법 2**: `os-prober` 재실행
  ```bash
  sudo os-prober
  sudo grub-mkconfig -o /boot/grub/grub.cfg
  ```
  단, os-prober가 Windows를 찾으려면 Windows가 **빠른 시작 꺼진 상태로 정상 종료**돼 있어야 한다.

### 7-2. 시계가 9시간 어긋나는 문제

Windows는 하드웨어 시계를 **로컬타임**으로, Linux는 **UTC**로 읽는다.
듀얼부팅하면 오갈 때마다 시간이 틀어진다.

**해결 (Windows 쪽을 UTC로):**
```powershell
reg add "HKLM\SYSTEM\CurrentControlSet\Control\TimeZoneInformation" /v RealTimeIsUniversal /t REG_DWORD /d 1 /f
```

### 7-3. Arch에서 F: (NTFS) 접근

```bash
sudo mkdir -p /mnt/windata
sudo mount -t ntfs3 /dev/sdb1 /mnt/windata
```

> **Windows가 빠른 시작/최대 절전으로 종료된 상태면 쓰기 금지.**
> 파일시스템 깨진다. 2-2를 반드시 먼저 처리할 것.

---

## 8. 설치 후 (GPU / 데스크탑)

### 8-1. NVIDIA 드라이버 — GTX 1050은 주의 필요

GTX 1050은 **Pascal 세대**다.

- ❌ `nvidia-open` — **사용 불가.** Turing(RTX 20xx) 이상만 지원
- ✅ **독점 드라이버 사용**

```bash
sudo pacman -S nvidia nvidia-utils nvidia-settings
```

> ⚠️ NVIDIA가 Maxwell/Pascal/Volta를 레거시 분기로 넘기는 중이라,
> 설치 시점에 `nvidia` 대신 `nvidia-580xx` 같은 레거시 패키지가 필요할 수 있다.
> **설치 직전에 Arch Wiki의 NVIDIA 문서에서 Pascal 항목을 반드시 재확인할 것.**
> (이 문서 작성 시점 기준 정보이며, 드라이버 분기는 자주 바뀐다.)

### 8-2. 데스크탑 환경 (RAM 8GB 고려)

| 환경 | 특징 |
|------|------|
| **XFCE / LXQt** | 가벼움. 8GB에 적합 ⭐ |
| KDE Plasma | 기능 많고 무난. 8GB에서 쓸 만함 |
| GNOME | 무겁고 NVIDIA 독점 드라이버와 Wayland 궁합 이슈 있음 |

```bash
# 예: KDE
sudo pacman -S plasma-meta konsole dolphin sddm
sudo systemctl enable sddm
```

> Pascal + 독점 드라이버 조합은 **X11이 Wayland보다 안정적**이다.

### 8-3. 한글 입력

```bash
sudo pacman -S fcitx5 fcitx5-hangul fcitx5-configtool fcitx5-gtk fcitx5-qt noto-fonts-cjk
```

---

## 9. 위험 요소 정리

| 위험 | 결과 | 대응 |
|------|------|------|
| 설치 시 디스크 오선택 | **F: 데이터 전멸** | T7 분리 + `lsblk` MODEL로 확인 |
| WD와 T7 용량 동일(931.5GB) | 헷갈려서 오선택 | 설치 중 T7 물리적 분리 (필수) |
| 빠른 시작 켜진 채 NTFS 쓰기 | F: 파일시스템 손상 | 2-2 선행 |
| MBR 슬롯 초과 (Disk 0은 1개뿐) | 파티션 생성 실패 | 안 A 선택 시 해당 없음 |
| BIOS를 UEFI로 변경 | **Windows 부팅 불가** | CSM/Legacy 유지 |
| GRUB을 SSD MBR에 덮어씀 | Windows 부팅 깨질 수 있음 | HDD(`/dev/sdb`)에 설치 |

### 최후의 방어선

**설치 중 WD 1TB(F:)의 SATA 케이블도 물리적으로 뽑는 것.**
안 A를 쓰면 거기 설치해야 하니 불가능하지만,
안 B(SSD 설치)를 선택한다면 **F: 케이블을 뽑고 설치하는 게 가장 확실하다.**
물리적으로 없는 디스크는 실수로도 지울 수 없다.

---

## 10. 롤백 (되돌리기)

Arch를 지우고 Windows만 쓰려면:

1. Windows 설치 USB로 부팅 → 복구 → 명령 프롬프트
2. ```
   bootrec /fixmbr
   bootrec /fixboot
   bootrec /rebuildbcd
   ```
3. 디스크 관리에서 Arch 파티션 삭제 후 F:에 병합

> 단, 방법 1(BIOS 부팅 순서로 전환)을 썼다면 Windows MBR을 안 건드렸으므로
> **그냥 Arch 파티션만 지우면 끝.** 이것도 안 A를 추천하는 이유.

---

## 11. 체크리스트

**설치 전**
- [ ] 백업 완료 및 **파일 열어서 직접 확인**
- [ ] T7 물리적 분리
- [ ] 빠른 시작 해제
- [ ] BitLocker 해제 확인
- [ ] F: 페이지파일 제거 후 재부팅
- [ ] Windows 디스크 관리에서 F: 축소
- [ ] Arch ISO → Rufus (MBR / DD 모드)

**설치 중**
- [ ] `ls /sys/firmware/efi` → 없음 확인
- [ ] `lsblk -o NAME,SIZE,MODEL` → 모델명으로 디스크 확인
- [ ] `fdisk`에서 기존 F: 파티션 살아있는지 확인
- [ ] `grub-install --target=i386-pc /dev/sdb`
- [ ] `GRUB_DISABLE_OS_PROBER=false`

**설치 후**
- [ ] 양쪽 OS 부팅 확인
- [ ] `RealTimeIsUniversal` 시계 설정
- [ ] NVIDIA 드라이버 (Pascal용 패키지 Wiki 재확인)
- [ ] F: 데이터 정상 접근 확인

---

## 참고 링크

- Arch 설치 가이드: https://wiki.archlinux.org/title/Installation_guide
- GRUB (BIOS/MBR): https://wiki.archlinux.org/title/GRUB#BIOS
- 듀얼부팅: https://wiki.archlinux.org/title/Dual_boot_with_Windows
- NVIDIA: https://wiki.archlinux.org/title/NVIDIA
- 시계 문제: https://wiki.archlinux.org/title/System_time#UTC_in_Microsoft_Windows
