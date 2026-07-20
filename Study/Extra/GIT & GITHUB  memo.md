
> 자주 잊어버리는 github 명령어 기록
> + git 버전관리, 이슈 활용, 브랜치 관리 연습.


## remote 설정, branch 관리

git remote add origin <GitHub 레포지토리 주소>

git remote -v    로 현재 연결된 원격저장소 확인 가능

주소 바꾸기 =>   git remote set-url origin <새로운 GitHub 주소>       

연결 끊기 =>   git remote remove origin

브랜치 이름을 강제로 바꾸기  =>  git branch -M main(이름)

브랜치 목록과 현재 브랜치 확인하기  => git branch

remote 브랜치 확인하기 => git branch -a

remote 브랜치 정밀하게 커밋까지 확인하기 => git branch -v



## git pull (fetch & merge)

원격저장소 상태 로컬로 가져오기  =>  git pull <원격저장소이름> <브랜치>

(fetch = 원격저장소 새로운사항 데이터 확인. merge는 로컬브랜치 '자동' 합침임)

git pull --rebase   =>  히스토리를 깔끔하게 정리해서 가져오기


컨플릭트 나면 => 에러가난파일 열어서 이쁘게 정리해주면됨 
(수정후 스테이징 및 커밋은 당연히필수)


방금 git pull 하기 전 상태로 되돌리기  => git reset --hard ORIG_HEAD

병합 커밋 없이 최신 코드 가져오기 => git pull --rebase origin main

커밋 없이 병합하기  =>  git pull --no-commit origin main


당연하지만 pull 이후 커밋해주는거도 필수임.


```
PS C:\Users\diffi\Desktop\mine\Study_manage\Study> git fetch

remote: Enumerating objects: 8, done.
remote: Counting objects: 100% (8/8), done.
remote: Compressing objects: 100% (6/6), done.
remote: Total 7 (delta 1), reused 0 (delta 0), pack-reused 0 (from 0)

Unpacking objects: 100% (7/7), 1.62 KiB | 30.00 KiB/s, done.

From https://github.com/difficcd/Study

   3adb10c..9b61902  main       -> origin/main
```

=> 이렇게 나오면 원격 저장소데이터가 로컬기준인 3adb10c 버전에서 9b61902버전으로 업뎃된다는거



## commit 관리

방금 한 git add . 취소하기 => git restore --staged  (구버전이면 git reset)

특정 파일만 스테이징 취소하기 => git restore --staged <파일이름> 혹은 <폴더이름>/

방금 push한 커밋 취소하기  =>  git reset --soft HEAD~1

```bash
git reset --soft HEAD~1    # 커밋만 취소, 파일 staged 유지
git reset --mixed HEAD~1   # 커밋 취소, 파일 unstaged 유지 (기본값)
git reset --hard HEAD~1    # 커밋 + 파일 변경사항 전부 삭제
git reset --soft afe157f   # 특정 커밋으로 되돌리기
```



git reset 을 취소하기 => git reflog (취소를 취소)
	                 HEAD가 이동한 모든 기록 보여줌. reset으로 날린 것도 복구 가능.


git revert HEAD => 취소 커밋 새로 만들기.



지금 상태를 강제 반영하기  =>  git push origin main --force   (--force == -f)
(이미 올라간거지우기)  위험해서 권장하지는 않음..



git rebase -i '커밋 해시' => 특정 커밋 이후 커밋들을 수정/삭제/합치기 가능



git rebase --continue    =>  충돌 해결 후 rebase 계속
git rebase --abort        =>  rebase 전체 취소하고 원상복구


안전        revert, reflog
보통        reset --soft, reset --mixed
위험        reset --hard, push --force



## 원격 저장소 관리

git clone https://github.com/사용자명/저장소명.git   
=> 빈공간에 자동으로 원격저장소 내용 가져오기  
  (이미 파일이 있으면 init, remote add, pull)