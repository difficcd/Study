# 05. 리눅스 공부

 > VM 우분투 / 아치리눅스 / 멀티부팅 / 리눅스 커널 코드 이해 

[[README|← 대시보드로]]


외장 SSD 정리 후 VM 열면 .. :

딱 하나 알아두실 것: `Files\openclaw_test`, `Files\VM_openclaw_test` 안의 **VMware 가상머신**을 나중에 열 때 "이동했습니까, 복사했습니까?" 물어보면 **"이동했습니다(I Moved It)"** 를 선택하시면 됩니다.


## 다룰 주제(초안)
- [ ] 커널이 하는 일 큰 그림 (프로세스/메모리/파일/드라이버)
- [ ] 소스 트리 구조 둘러보기
- [ ] 작은 서브시스템 하나 골라 코드 따라가기
- [ ] 모듈 5(Linux 운영)와 연결해서 이해 → [[wishlist/Cloud_basic/README]]

**실제 컴퓨터(가상머신 X)에 깔아보기** : 남는 다른 컴퓨터가 있다면 무조건 실물 컴퓨터에 까는 걸 추천해. 가상머신은 하드웨어 호환성 문제를 다 알아서 우회해 주거든. 실물 컴퓨터에 직접 깔면서 그래픽카드 드라이버 설치하고, 와이파이 안 잡혀서 고생해 봐야 리눅스 커널과 하드웨어가 어떻게 상호작용하는지 뼈저리게 배울 수 있어.


## 자료

https://namu.wiki/w/Linux

https://namu.wiki/w/X11?from=Wayland#Wayland

https://wikidocs.net/238658

https://ppsicd.tistory.com/26

https://ppsicd.tistory.com/29

https://ppsicd.tistory.com/27


_선행 추천: 모듈 5(Linux 서버 운영)로 사용자 관점부터 익히고 진입_
