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
        hanoi_top(N-1, from, via, to); // 3232.. 자리 지정 (L)
        
        v[to].push_back(v[from].back()); //V
        v[from].pop_back(); // 제거
        result.push_back(from+1); 
        result.push_back(to+1);
        
        count ++;
        hanoi_top(N-1, via, to, from); // 큰 수 위에 작은 수 올리기 (R)
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




// 피보나치 수열
#include <iostream>
#include <vector>
using namespace std;

vector<pair<int, int>> v;


void fibonacci(int n) {

  // 5이면 f(4), f(3), f(2) 만들어야 함
  // 2 3 4 5" 끝. 2부터 시작.
  
  
  for (int i=2; i<=n; i++){
    v[i].first = v[i-1].first + v[i-2].first;
    v[i].second = v[i-1].second + v[i-2].second;
  }
  
  cout << v[n].first << " " << v[n].second << "\n";
}

int main(){

  int N, M;
  cin >> N;

  // 0 개수, 1 개수 입력
  // v[0] = f(0)
  
  v.push_back({1,0}); // f(0)
  v.push_back({0,1}); // f(1)

  for (int i=0; i<40; i++)
    v.push_back({0,0}); // 나머지 공간할당
  
  for(int i=0; i<N; i++){
    cin >> M;
    fibonacci(M);
  }
}


#include <iostream>
#include <vector>
using namespace std;

// 리스트를 활용한 피보나치 수열 구현

int main(){

  vector<long long> v(3);
  int N; cin >> N;
  
  v[0] = 1;
  v[1] = 0;
  v[2] = 1;

  for(int i=2; i<=N; i++){
  v[0] = v[1];
  v[1] = v[2];
  v[2] = v[0] + v[1];
  }

  cout << v[2];

}

// 1788번 피보나치 수의 확장
// 큰 수를 나머지 처리하기  
// 음수까지 피보나치 수 확장하기
#include <iostream>
#include <vector>
#define MAX 1000000000
using namespace std;

int main(){

  vector<long long> v(3);
  int N; cin >> N; 
  // 절댓값이 1,000,000 이내

  if(N==0) cout << "0\n0\n";  
  else if(N<0) {
    if(N % 2 == 0) cout << "-1\n";
    else cout << "1\n";
    N = -N;
  }
  else cout << "1\n";
  
  
  v[0] = 1; v[1] = 0; v[2] = 1;

  for(int i=2; i<=N; i++){
  if(v[1] > MAX) v[0] = v[1] - MAX;
  else v[0] = v[1];
    
  if(v[2] > MAX) v[1] = v[2] - MAX;
  else v[1] = v[2];
    
  if(v[0]+v[1] > MAX) v[2] = v[0] + v[1] - MAX;
  else v[2] = v[0] + v[1];
    
  }

  if(N!=0) cout << v[2];
  

}




https://www.acmicpc.net/problem/2086 
// 피보나치 확장문제****

