

> https://wikidocs.net/book/11567

   tistory login 후 아래 자료 참조
> https://ppsicd.tistory.com/45
> https://ppsicd.tistory.com/43
> https://ppsicd.tistory.com/33
> https://ppsicd.tistory.com/29



#### archiso 진입

f12 => Arch Linux install medium... 에서 tab 키 누르고 명령어 맨 끝에 nomodeset 붙여주기

혹은 아치리눅스 부팅전 화면에서 esc 누른 후 아래 명령어 입력

```
boot: arch nomodeset
```

---

=> nomodeset이 없으면, 
부팅 메뉴에서 엔터를 누르고 리눅스 커널이 로딩되다가 모니터에 No Signal이 뜸. 
(아치리눅스가 드라이버를 잡으면서 **해상도나 그래픽 출력 포트를 잘못 찾아 화면 출력을 잃어버렸을 때** 발생하는 문제)

nomodeset 을 사용하면 리눅스가 그래픽드라이버 제어 못하게 막아서 기본 그래픽모드로 부팅할 수 있기에 화면이 잘 나옴.



#### ping : 인터넷 연결 확인

```bash
ping -c 3 google.com
```

https://wikidocs.net/224858#ping

유선랜(이더넷)이 꽃혀있으면 자동으로 잡힐 가능성 높음.
(와이파이인 경우 iwctl 도구 사용해야 함. 노트북 듀얼부팅 할 때 실습해볼 것)

#### lsblk 

list block devices. (IO - block device 맞음)

```bash
lsblk
```



```
NAME   SIZE  TYPE
sda    111.8G  disk      ← 아마 디스크 0 (윈도우)
├─sda1  ...    part
├─sda2  ...    part
└─sda3  ...    part
sdb    931.5G  disk      ← 아마 디스크 1 (F: + 미할당 공간)
└─sdb1  ...    part      ← F: 파티션 (축소된 크기)
```


lsblk 컬럼 설명

| 컬럼                  | 뜻                                                                                                                  | 예시                                                                                                                                               |
| ------------------- | ------------------------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------------------------------------------------------------------------------------ |
| **MAJ:MIN**         | Major:Minor 디바이스 번호. 리눅스 커널이 장치를 식별하는 내부 번호예요. Major는 드라이버 종류(8=SCSI/SATA 디스크, 7=loop), Minor는 그 안에서 몇 번째 장치/파티션인지 | `8:0` = SATA 디스크 첫 번째, `8:16` = SATA 디스크 두 번째, `7:0` = loop 첫 번째                                                                                 |
| **RM**              | Removable. 이동식 매체인지 여부. `1`이면 USB/CD 같은 분리 가능 장치, `0`이면 내장 디스크                                                     | USB → 1, HDD → 0                                                                                                                                 |
| **SIZE**            | 디스크/파티션 용량                                                                                                         | `931.5G`                                                                                                                                         |
| **RO**              | Read-Only. `1`이면 읽기 전용, `0`이면 읽기+쓰기 가능                                                                             | 보통 다 0                                                                                                                                           |
| **TYPE**            | `disk`(물리 디스크 전체), `part`(파티션), <br>`loop`(루프 디바이스)                                                                | 루프 디바이스 = 파일을 디스크처럼 가상으로 마운트하는 장치.                                                                                                               |
| **MOUNT<br>POINTS** | 현재 마운트된 경로. 비어있으면 마운트 안 된 상태                                                                                       | loop0 → `/run/archiso/airootfs`<br>지금 USB에서 부팅한 Arch ISO의 루트 파일시스템이 여기 마운트된 것! : squashfs 이미지 <br>파일을 loop 디바이스를 통해 마치 디스크처럼 마운트해서 쓰고 있는 것임.<br> |

참고로 sdb가 `8:16`인 이유 — Major 8(SATA 디스크)은 Minor를 **16 단위로** 할당해요. 첫 번째 디스크가 `8:0`, 두 번째가 `8:16`, 세 번째가 `8:32`. 그리고 파티션은 그 사이 번호를 써요 (`8:1`은 sda의 첫 번째 파티션, `8:17`은 sdb의 첫 번째 파티션).

- **sda (111.8G)** = 디스크 0 = 윈도우 → 절대 안 건드림
- **sdb (931.5G)** = 디스크 1 = F: + 미할당 공간 → 여기에 Arch 설치




#### fdisk -l

```bash
fdisk -l /dev/sdb
```

- **fdisk**: **f**ixed **disk**의 약자. 디스크의 파티션 테이블을 보거나 편집하는 도구
- **-l**: **l**ist. "파티션 목록을 보여줘" (읽기만 하고 수정은 안 함)
- **/dev/sdb**: 어떤 디스크를 볼지 지정. `/dev/`는 리눅스에서 모든 장치 파일이 들어있는 디렉토리이고, `sdb`는 아까 확인한 두 번째 SATA 디스크
https://ppsicd.tistory.com/43 보면 이미 배운 내용임

정보 해석

| 출력                                   | 뜻                                                                                                              |
| ------------------------------------ | -------------------------------------------------------------------------------------------------------------- |
| **Disk /dev/sdb: 931 GiB**           | 이 디스크 전체 크기                                                                                                    |
| **1002...bytes, 1953525168 sectors** | 같은 용량을 바이트와 섹터 수로 표현한 것                                                                                        |
| **Units: sectors of 1 * 512**        | "섹터 1개 = 512바이트" 단위로 표시하겠다는 뜻                                                                                  |
| **Sector size: 512 / 4096**          | **논리 섹터 512B / 물리 섹터 4096B**. 실제 HDD 내부는 4KB 단위로 읽고 쓰지만, OS한테는 호환성을 위해 512B 단위로 보이게 해주는 거예요 (==512e 방식==이라고 함) |
| **I/O size: 4096 / 4096**            | 최적 I/O 크기. 4KB 정렬해서 읽고 쓰는 게 성능이 좋다는 의미                                                                         |
| **Disklabel type: dos**              | **dos = MBR 파티션 방식**이라는 뜻. 윈도우 디스크 관리에서 확인한 것과 일치                                                              |

이 아래에 보면 
Device     Boot  Start       End    Sectors   Size  Id  Type
/dev/sdb1         2048  xxxxxxx  xxxxxxx   xxxG   7  HPFS/NTFS/exFAT

이렇게 파티션 목록도 함께 나오는데
여기서 **sdb1(축소된 F: 파티션)이 어디서 끝나는지** 확인해야 그 뒤의 미할당 공간에 Arch 파티션을 만들 수 있음.

- **sdb1**: 섹터 2048부터 시작, 약 785GB, NTFS = 윈도우에서 축소한 F: 드라이브
- **sdb1 뒤의 빈 공간**: 931GB - 785GB = **약 146GB 미할당** = 여기에 Arch 설치


#### fdisk

 `-l` 없이 실행하면  파티션을 직접 만들 수 있는 인터랙티브 모드로 들어감.

```bash
fdisk /dev/sdb
```

프롬프트가 `Command (m for help):` 로 바뀔 거예요. 여기서 **한 글자씩 입력**하면서 진행합니다:

**1) 새 파티션 만들기:**

```
n
```

→ **n**ew partition

**2) 파티션 타입 선택:**

```
p
```

→ **p**rimary (주 파티션)

**3) 파티션 번호:**  
그냥 **Enter** (기본값 2가 뜰 거예요 — sdb2가 됨)

**4) 시작 섹터:**  
그냥 **Enter** (자동으로 sdb1 끝 바로 다음부터 시작)

**5) 끝 섹터:**  
그냥 **Enter** (남은 공간 전부 사용)

**6) 파티션 타입을 Linux로 변경:**

```
t
```

→ **t**ype 변경

파티션 번호 물으면:

```
2
```

Hex code 물으면:

```
83
```

→ 83 = Linux 파일시스템 타입

**7) 저장하고 나가기:**

```
w
```

→ **w**rite. 여기서 Enter 치는 순간 **실제로 파티션 테이블이 디스크에 써져요**

=> 오소이 RV (+tistory)



#### mkfs.ext4 

파일시스템 포맷.

fdisk에서 `w`로 저장하고 나왔으면, 
이제 새로 만든 sdb2 파티션에 **ext4 파일시스템** 만들어줌.

```bash
mkfs.ext4 /dev/sdb2
```

- **mkfs**: **m**a**k**e **f**ile**s**ystem
- **ext4**: 리눅스에서 가장 널리 쓰이는 파일시스템 종류
- **/dev/sdb2**: 방금 fdisk로 만든 새 파티션

#### mount 

포맷 완료됐으면, 이제 이 파티션을 **마운트**해야 해요:

```bash
mount /dev/sdb2 /mnt
```

- **mount**: 파티션을 디렉토리에 "연결"하는 명령. 이후 `/mnt` 경로로 접근하면 sdb2 파티션에 읽고 쓸 수 있게 됨
- **/mnt**: Arch 설치 관례상 여기에 마운트해요. 나중에 이 `/mnt`가 설치된 Arch의 루트(`/`)가 됨!!

참고로 한 번 재부팅하면 마운트가 풀리니,
ls /mnt 해보고 아무것도 나오지 않으면 마운트 해주기.


#### pacstrap

마운트한 다음, **Arch 베이스 시스템 설치**:

```bash
pacstrap -K /mnt base linux linux-firmware
```

- **pacstrap**: Arch 설치 전용 도구. 지정한 경로(`/mnt`)에 패키지를 다운로드해서 설치해줌
- **-K**: 새 pacman 키링을 초기화 (패키지 서명 검증용)
- **base**: 최소한의 시스템 패키지 묶음 (bash, coreutils, systemd 등)
- **linux**: 커널
- **linux-firmware**: 하드웨어 드라이버/펌웨어


#### genfstab

##### ① fstab 생성

```bash
genfstab -U /mnt >> /mnt/etc/fstab
```

- **genfstab**: **gen**erate **fstab**. 현재 마운트 상태를 기반으로 fstab 파일을 자동 생성
- **-U**: UUID로 파티션을 식별 (디바이스 이름 `/dev/sdb2`는 부팅할 때마다 바뀔 수 있지만, UUID는 고유하니까 더 안전)
- **>> /mnt/etc/fstab**: 결과를 fstab 파일에 추가(append)  (>>는 이어쓰기, >는 파일 덮어쓰기 혹은 새로 만들기.)

- \*RV : fstab = 리눅스 부팅 시 **어떤 디스크 파티션을, 시스템의 어느 폴더(마운트 포인트)에, 어떤 옵션으로 연결할지** 기록해 둔 설정 파일
- `mount -a` 명령어를 칠 때 이 파일을 읽어서 디스크를 제 위치에 척척 붙여줌.

> 재부팅된 새 시스템이 "아, 내 루트 디스크는 저기 `/dev/nvme0n1p2`에 있었지!" 하고 찾아갈 수 있도록 **"부팅할 때 이 설정을 꼭 읽어라"** 하고 새 시스템의 영구적인 설정 파일( `/mnt/etc/fstab`)로 남겨두는 것입니다.

fstab은 **"부팅할 때 어떤 파티션을 어디에 마운트할지"를 적어둔 설정 파일**이에요. 이게 없으면 Arch가 부팅돼도 자기 루트 파티션을 어디서 찾아야 하는지 모르게 돼요.
(부팅할 때 컴퓨터가 저장장치들을 어떤 순서로, 어디에, 어떤 방식으로 연결(마운트)해야 할지 알려주는 설계도를 작성하는 것)

```bash
cat /mnt/etc/fstab
```

UUID로 시작하는 줄이 하나 보이고, 마운트 포인트가 `/`로 되어있으면 정상.
(rv : **cat**: 파일 내용을 화면에 출력하는 명령 (**c**onc**at**enate의 약자))


#### chroot

##### ② chroot — 설치된 시스템 안으로 진입

```bash
arch-chroot /mnt
```

- **chroot**: **ch**ange **root**. 루트 디렉토리(`/`)를 `/mnt`로 바꾸는 명령
- 이걸 실행하면 **지금 방금 설치한 Arch 시스템 안으로 들어가는 거예요.** 프롬프트가 바뀔 거예요
- 이 안에서 치는 모든 명령은 USB의 라이브 환경이 아니라 **디스크에 설치된 Arch에 적용**됨

쉽게 말하면 "아직 건물(Arch)이 완공은 안 됐지만, 일단 건물 안으로 들어가서 내부 공사(설정)를 하는 것"이에요.

나가고 싶으면 exit 하면 됨.



#### pacman

chroot 진입하고, nano 설치

```bash
arch-chroot /mnt
pacman -S nano
```

- **pacman**: Arch의 패키지 매니저 (**pac**kage **man**ager)
- **-S**: **S**ync, "이 패키지를 서버에서 다운받아서 설치해줘"




#### ln

시간대 설정.

```bash
ln -sf /usr/share/zoneinfo/Asia/Seoul /etc/localtime
```

- **ln**: **l**i**n**k. 링크(바로가기) 만드는 명령
- **-s**: **s**ymbolic link (심볼릭 링크, 윈도우의 "바로가기"와 비슷)
- **-f**: **f**orce. 이미 있으면 덮어쓰기
- `/usr/share/zoneinfo/Asia/Seoul`: 서울 시간대 정보 파일
- `/etc/localtime`: OS가 "현재 시간대가 뭐지?" 할 때 읽는 파일
- 즉 "서울 시간대 파일을 /etc/localtime으로 연결해줘"라는 뜻

#### hwclock 

```bash
hwclock --systohc
```

- **hwclock**: **h**ard**w**are **clock**. 메인보드의 RTC(하드웨어 시계) 관리 도구
- **--systohc**: **sys**tem **to** **h**ardware **c**lock. "현재 시스템 시간을 RTC에 써줘"

#### nano & echo,   locale.gen, hostname

```bash
nano /etc/locale.gen
```

- 시스템에서 사용할 **언어/인코딩 목록**이 적힌 파일을 편집기로 여는 거예요
- `#en_US.UTF-8 UTF-8` 앞의 `#`을 지우면 "이 로케일을 활성화해줘"라는 뜻
- nano는 설명이 다 있으니 설명대로 사용하면 됨. 매우 직관적임.

```bash
locale-gen
```

- **locale-gen**erate. 방금 주석 해제한 로케일을 실제로 **생성(컴파일)** 하는 명령

```bash
echo LANG=en_US.UTF-8 > /etc/locale.conf
```

- **echo**: 뒤에 오는 텍스트를 출력
- **>**: 출력을 파일로 보내기 (덮어쓰기)
- 즉 `LANG=en_US.UTF-8`이라는 한 줄짜리 텍스트를 `/etc/locale.conf` 파일에 써넣는 거예요
- 이 파일을 OS가 부팅할 때 읽고 "시스템 기본 언어는 영어 UTF-8이구나" 하고 적용


```bash
echo myarch > /etc/hostname
```

- 위와 같은 원리. `myarch`라는 텍스트를 `/etc/hostname`에 써넣음
- 네트워크에서 "이 컴퓨터 이름이 뭐야?" 하면 이 파일을 읽어서 응답



#### passwd

```bash
passwd
```

- **passw**or**d**. << root 계정의 비밀번호를 설정 >> 하는 명령
- 보안상 **타이핑해도 화면에 아무것도 안 보여요** (별표도 안 뜸). 정상이니 그냥 치고 Enter (backup : 1234)

#### grub install

##### ① GRUB 패키지 설치

```bash
pacman -S grub os-prober
```

- **grub**: 부트로더 프로그램 자체
- **os-prober**: 다른 OS(윈도우)를 자동으로 찾아서 GRUB 메뉴에 추가해주는 도구. 지금 당장은 안 쓰지만(F12로 선택할 거니까) 나중에 필요할 수 있어서 같이 설치


##### ② GRUB을 디스크 1(sdb)의 MBR에 설치

```bash
grub-install --target=i386-pc /dev/sdb
```

- **grub-install**: GRUB 부트 코드를 디스크에 써넣는 명령
- **--target=i386-pc**: "Legacy BIOS + MBR 방식으로 설치해줘"라는 뜻 (우리 환경이 이거예요)
- **/dev/sdb**: **sdb2가 아니라 sdb** — 파티션이 아니라 **디스크 자체의 MBR(첫 섹터)**에 쓰는 거예요

여기가 핵심이에요. `/dev/sda`(윈도우 디스크)가 아니라 \*\*`/dev/sdb`\*\*인 걸 꼭 확인하세요. 이게 "디스크 0(윈도우)은 안 건드린다"를 보장하는 부분이에요.


##### ③ GRUB 설정 파일 생성

```bash
grub-mkconfig -o /boot/grub/grub.cfg
```

- **grub-mkconfig**: GRUB 설정 파일을 자동 생성
- **-o**: **o**utput. 결과를 이 경로에 저장
- **/boot/grub/grub.cfg**: GRUB이 부팅할 때 읽는 설정 파일 (어떤 커널을 어디서 로드할지 등)





#### systemctl  & network tool install  & USB => HDD


##### ① 네트워크 도구 설치

지금 인터넷이 되는 건 USB 라이브 환경 덕분이에요. 
설치된 Arch로 부팅하면 네트워크 도구가 없어서 인터넷이 안 돼요. 
미리 설치해둬야 해요:

```bash
pacman -S networkmanager
systemctl enable NetworkManager
```

- **networkmanager**: 네트워크 연결을 자동으로 관리해주는 서비스
- **systemctl enable**: "부팅할 때 이 서비스를 자동으로 시작해줘"라고 등록하는 명령. **enable은 지금 실행하는 게 아니라 "다음 부팅부터 자동 실행" 예약**이에요



##### ② chroot에서 나가기 + 재부팅

```bash
exit
umount /mnt
reboot
```

- **exit**: chroot에서 나가기
- **umount**: **u**n**mount**. 마운트 해제 (안전하게 디스크 연결을 끊는 것)
- **reboot**: 재부팅

재부팅되면 **USB를 빼세요.** 그리고:

1. **그냥 켜지면** → 윈도우가 평소처럼 뜰 거예요 (정상)
2. **F12 눌러서 부팅 메뉴** → 디스크 1(또는 sdb 관련 항목) 선택 → **Arch Linux 로그인 화면**이 뜨면 성공!
3. 로그인: 유저명 `root`, 비밀번호는 아까 설정한 거


재부팅 시 boot device select : 
- **P1: TS120...** → 아마 **Transcend 120GB SSD** = 디스크 0 (윈도우). TS120이 Transcend SSD 모델명이고 용량도 111GB랑 맞아요
- **P3: WDC...** → **Western Digital** = 디스크 1 (1TB HDD, F: + Arch). WDC는 Western Digital의 약자


#### GRUB nomodeset 

다시 F12로 Arch 부팅해서 **GRUB 메뉴가 뜰 때**:

1. `Arch Linux` 항목이 선택된 상태에서 **Enter 치지 말고 `e` 키**를 누르세요 (edit)
2. 텍스트 편집 화면이 나오는데, `linux` 으로 시작하는 줄을 찾으세요
3. 그 줄 맨 끝에 커서를 이동해서 **한 칸 띄고** `nomodeset` 추가

```
linux  /boot/vmlinuz-linux root=UUID=xxxxx ... quiet nomodeset
```

4. **Ctrl + X** 또는 **F10**으로 부팅

- **nomodeset**: "커널아, GPU 모드 설정(해상도 변경 등)을 하지 마"라는 옵션. 기본 저해상도 텍스트 모드로 부팅하게 돼요.
---

### nomodeset 영구 적용

로그인한 상태에서 (`root` + 비밀번호):

```bash
nano /etc/default/grub
```

`GRUB_CMDLINE_LINUX_DEFAULT=` 줄을 찾으세요. 아마 이렇게 생겼을 거예요:

```
GRUB_CMDLINE_LINUX_DEFAULT="loglevel=3 quiet"
```

이 따옴표 안에 `nomodeset`을 추가:

```
GRUB_CMDLINE_LINUX_DEFAULT="loglevel=3 quiet nomodeset"
```

`Ctrl + O` → Enter → `Ctrl + X`로 저장하고 나온 다음:

```bash
grub-mkconfig -o /boot/grub/grub.cfg
```

이러면 앞으로 GRUB이 매번 자동으로 `nomodeset`을 넣어서 부팅해줘요.


- **/etc/default/grub**: GRUB의 기본 설정 파일. 여기 값을 바꾸면 `grub-mkconfig`가 반영해줌
- **GRUB_CMDLINE_LINUX_DEFAULT**: "커널한테 매번 전달할 옵션 목록"
- **loglevel=3**: 부팅 시 커널 로그를 경고(warning) 이상만 표시
- **quiet**: 부팅 로그를 조용하게 (안 보여줌)
- **nomodeset**: GPU 모드 설정 비활성화
- **grub-mkconfig**: 이 설정 파일을 읽어서 실제 GRUB 설정(`grub.cfg`)을 다시 생성


####
####
####



#### system 종료 명령어 

(예전에는 전자: 프로그램정리과정 좀 생략, 후자: 좀 더 안전하게 작업중데이터/프로그램 안전하게 커버해준다음 OS 종료 뒤 메인보드에 파워오프 명령내리는거 였는데  ,  현재 시점에서는 둘이 별 차이 없음. : sys관리도구방식인 systemd 환경에서 내부적으로 심볼릭 링크를 걸어 systemctl poweroff라는 동일한 sys 종료 명령어로 연결해뒀기 때문임. : 안전종료로 끝냄)

```bash
poweroff 
```

```
shutdown -h now
```

