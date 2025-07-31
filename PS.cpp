https://plzrun.tistory.com/entry/%EC%95%8C%EA%B3%A0%EB%A6%AC%EC%A6%98-%EB%AC%B8%EC%A0%9C%ED%92%80%EC%9D%B4PS-%EC%8B%9C%EC%9E%91%ED%95%98%EA%B8%B0
https://steady-coding.tistory.com/260

ps 공부방법

  https://www.acmicpc.net/board/view/116825

// 분할정복 : 재귀 하노이 탑
#include <iostream>
#include <vector>
#include <string>
using namespace std;

vector<int> v[3];
vector<int> result;
int count=0;

// 각 단계를 출력해야 함
void hanoi_top(int N, int from, int to, int via){
    if(N==1) {
        // from => to 로 N 이동
        v[to].push_back(v[from].back()); // 수 저장
        v[from].pop_back(); // 제거
        result.push_back(from+1); 
        result.push_back(to+1);
        count ++;
    }
    else {
        hanoi_top(N-1, from, via, to); // 위의값 임시로 옮김
        
        v[to].push_back(v[from].back()); // 수 저장
        v[from].pop_back(); // 제거
        result.push_back(from+1); 
        result.push_back(to+1);
        
        count ++;
        hanoi_top(N-1, via, to, from); // 옮긴 값 복원
        // N-1 을 via => to 로 이동
    }

}

int main() {
    int N;
    cin >> N;

    for(int i=N; i>=1; i--){
        v[0].push_back(i);
    }
    
    hanoi_top(N,0,2,1);
    
    cout << count << "\n";

    for(int i=0; i<result.size(); i++){
        if((i+1)%2 != 0)cout << result[i] << " ";
        else cout << result[i];
        if((i+1)%2 == 0 && i != result.size())
            cout << "\n";
    }
}

