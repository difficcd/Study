https://www.acmicpc.net/problem/11279 
 0726 최대 힙
https://namu.wiki/w/%EC%A0%95%EB%A0%AC%20%EC%95%8C%EA%B3%A0%EB%A6%AC%EC%A6%98#s-4.4.2
  정렬 알고리즘 학습하기! ~8월 첫째주까지.
  JAVA 프로젝트를 위한 시간 마련 포함
   + hwp 참고 & 프로젝트 진행
  
  https://www.acmicpc.net/problem/5177
  https://www.acmicpc.net/problem/5186
  **

// 1158번 요세푸스 : 순환 로직 복습 필수!!!
  #include <iostream>
#include <vector>
using namespace std;


int main() {
  vector<int> v, j;
  // v -> j 

  int N, K;
  cin >> N >> K;
  for(int i=0; i<N; i++)
    v.push_back(i+1);
  for(int i=0; i<N; i++)
    j.push_back(0);


  int index=K-1;

  
  // 1 2 "3" 4 5 6 7 => 3 소거후 index 는 2.
  // 1 2 4 5 "6" 7 => index 에서 +2칸 +(K-1)
  // 1 2 4 5 7 => 6 소거후 index = 4
  // 1 2 4 5 7 => index 에서 +2칸 인데, 6 > 5(원소수)
  // 1 4 5 7 => index = 

  j[0] = v[index];
  
  for(int i=0; i<N-1; i++){
    v.erase(v.begin() + index);

    index = (index + (K - 1)) % v.size();
    // 원형 큐 로직과 유사

   /* for(int i=0; i<N-1; i++){
    v.erase(v.begin() + index);

    if(index >= v.size())
      index = 0;
        
    index += (K-1);
    
    if(index > N-2-i)
      index = index-(N-1-i);

    이 부분을 위의 한 줄로 대체한 것.
    복잡하고 긴 과정을 간단히 추리는 걸 연습하자!!
      */
 

    j[i+1] = v[index];
  }

  cout << "<";
  for(int k=0; k<N-1; k++)
      cout << j[k] << ", ";
  cout << j[N-1] << ">";
  
}



#include <iostream>
#include <string>
using namespace std;


int main() {

  // 2675: 문자열 반복
  int T, R;
  cin >> T;
  
  string S;

  for (int i=0; i<T; i++){
    cin >> R >> S;

    for(int j=0; j<; j++)
      for(int k=0; k<R; k++)
        cout << S[j];
    
    cout <<"\n";
  }


  // 10809: 알파벳 찾기
  
  string str;
  int alphabet[26];

  for(int i=0; i<26; i++)
   alphabet[i] = -1;

  
  getline(cin, str, '\n');
  
  for(int i=0; i<str.length(); i++){
   if(alphabet[int(str[i])-97] == -1)
    alphabet[int(str[i])-97] = i;
  }

  for(int i=0; i<26; i++)
    cout << alphabet[i] << " ";

  
}

// 1436번 : 영화감독 숌

#include <iostream>
#include <string>
#define MAX 1000000
using namespace std;

int main() {

  int arr[MAX]; 
  int N=0;
  int temp=0;

   cin >> N; 
  
  for(int i=666; i<MAX; i++){
    string str = to_string(i);

    // 해당 숫자의 666개수여부 count
                           
    for(int j=0; j<str.length()-2; j++)
      if(str.substr(j, 3) == "666"){
        arr[temp] = i;
        temp++;
        break;
      }
      
  }

  cout << arr[N-1];

  
}

// 배열은 용량 문제로 백준에서 틀렸습니다 처리.
// 아래는 개선 코드
#include <iostream>
#include <string>
#define MAX 1000000
using namespace std;

int main() {

  int N;
  int count = 0;
  int num = 666;

  int temp=1;

   cin >> N; 

  while(temp){
    string str = to_string(num);
    
    for(int j=0; j<str.length()-2; j++)
      if(str.substr(j, 3) == "666"){
        count++; // 몇 번째 666인지 나타냄
        if(count == N){
          cout << num;
          temp = 0;
          break;
          }
        break;
      }
          
    num++;
  }


}


// 1312 번 : 소수
#include <iostream>
using namespace std;

int main() {

 int A, B;
 unsigned long long  N;

  cin >> A >> B >> N;
  
  unsigned long long T;
 int temp=1, count=0;

 int NN=N;
  
  N = A%B;
  T = N*10 / B;
  
  while(temp){
    if(count == NN-1) {
      temp=0;
      cout << T;
      break;
    }
    
    N = N*10 - T*B;
    T = N*10 / B;
    
    count++;
  }


}
// GPT 보고 개선


