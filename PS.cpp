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



// 2086번 : 피보나치 수의 합
// # 피보나치 수를 행렬로 구하기
// # 분할 정복으로 제곱함수 구현하기 (log N)
// # 피보나치 합의 공식

#include <iostream>
#include <vector>
#define MAX 1000000000
using namespace std;

typedef vector<vector<long long>> matrix;
// 벡터 안에 벡터를 넣어 행렬 구현

const long long MOD = MAX; 

// 2x2 행렬 곱셈
matrix multiply_2(matrix &a, matrix &b) {
    matrix res = {{0, 0}, {0, 0}};
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++)
                res[i][j] = (res[i][j] + a[i][k] * b[k][j]) % MOD;
    return res;
}

// 2x1 행렬 곱셈
matrix multiply_1(matrix &a, matrix &b) {
    matrix temp = {{0},{0}};
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            temp[i][0] += (b[j][0] * a[i][j]) % MOD;
    return temp;
}


// 2x2 행렬 n 지수곱 a^n 구현
// 분할 정복을 이용한 빅오 logN 복잡도의 거듭제곱 함수 power
// 알고리즘 강의 참조. F(n) = F(n/2) +C
matrix power(matrix a, long long n) {
    matrix res = {{1, 0}, {0, 1}}; // 단위행렬
    while (n > 0) {
        if (n % 2 == 1) res = multiply_2(res, a);
        a = multiply_2(a, a);
        n /= 2;
    }
    return res;
}

int main(){

  vector<long long> v(3);
  unsigned long long a, b; 
  unsigned long long F_a, F_b, sum;
  cin >> a >> b; 

  matrix M = {{1, 1}, {1, 0}};
  matrix N = {{1},{0}};
  matrix result ={{0},{0}};


 // 1 1 2 3 5 8 

  matrix res = power(M, a); // a+1
  result = multiply_1(res, N);
  F_a = result[0][0];

  res = power(M, b+1); // b+2
  result = multiply_1(res, N);
  F_b = result[0][0];

  sum = (F_b - F_a + MOD) % MOD;
  // 피보나치 합의 공식 F(b+2) - F(a+1)
  // ex ) 2 3 5 8 13  에서 2+3+4 = 13-3 = 10
      
  cout << sum;
  

}


//  10830번 행렬 제곱 
// (1) 메모리 누수, C++ 스타일 파괴.. 등 문제가 많은 내 원본 코드
#include <iostream>
#define NUM 1000
using namespace std;

int N;



// NxN 행렬끼리의 행렬 곱 정의
int** multiply_N(int** A, int** B){

    int ** temp; 
    temp = (int**)malloc(sizeof(int*) * N);
    for(int i=0; i<N; i++)
        temp[i] = (int*)malloc(sizeof(int)*N);

    for(int i=0; i<N; i++)
        for(int j=0; j<N; j++)
             temp[i][j] = 0; // 초기화


    for(int i=0; i<N; i++)
        for(int j=0; j<N; j++)
            for(int k=0; k<N; k++)
                temp[i][j] = (temp[i][j] + A[i][k] * B[k][j]) % NUM;

    return temp;
}


int** power(int** A, long long M){

    int ** I; // N x N 단위행렬 
    I = (int**)malloc(sizeof(int*) * N);
    for(int i=0; i<N; i++)
        I[i] = (int*)malloc(sizeof(int)*N);

    // temp 단위행렬로 초기화
    for(int i=0; i<N; i++)
        for(int j=0; j<N; j++){
            if(i == j) I[i][j] = 1;
            else I[i][j] = 0;
        }


    while(M > 0){
        if(M % 2 == 1) I = multiply_N(I, A);
        A = multiply_N(A,A); // A = A * A
        M /= 2;
    }

    return I;
}

int main(){
    int **matrix;

    cin >> N;
    long long M; cin >> M;

    matrix = (int**)malloc(sizeof(int*) * N);

    for(int i=0; i<N; i++)
        matrix[i] = (int*)malloc(sizeof(int)*N);

    for(int i=0; i<N; i++)
        for(int j=0; j<N; j++)
            cin >> matrix[i][j];

    matrix = power(matrix, M);

    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
             cout << matrix[i][j];
            if(j != N-1) cout << " ";
        }
        if(i != N-1) cout <<"\n";
    }


}



// (2) 1차 개선 : 메모리 관리 개선 코드


#include <iostream>
#define NUM 1000
using namespace std;

int N;

void delete_M(int** mat) {
    for (int i = 0; i < N; ++i)
        delete[] mat[i];
    delete[] mat;
}

// NxN 행렬 곱
int** multiply_N(int** A, int** B) {
    int** temp = new int*[N];
    for (int i = 0; i < N; ++i)
        temp[i] = new int[N];

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            temp[i][j] = 0;

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            for (int k = 0; k < N; ++k)
                temp[i][j] = (temp[i][j] + A[i][k] * B[k][j]) % NUM;

    return temp;
}

int** power(int** A, long long M) {

    int** B = new int*[N];
    for (int i = 0; i < N; ++i) {
        B[i] = new int[N];
        for (int j = 0; j < N; ++j)
            B[i][j] = A[i][j];
    }


    int** I = new int*[N];
    for (int i = 0; i < N; ++i)
        I[i] = new int[N];

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            I[i][j] = (i == j) ? 1 : 0;

    while (M > 0) {
        if (M % 2 == 1) {
            int** temp = multiply_N(I, B);
            delete_M(I);
            I = temp;
        }
        int** temp = multiply_N(B, B);
        delete_M(B);
        B = temp;
        M /= 2;
    }

    return I;
}

int main() {
    cin >> N;
    long long M;
    cin >> M;

    int** matrix = new int*[N];
    for (int i = 0; i < N; ++i)
        matrix[i] = new int[N];

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            cin >> matrix[i][j];

    int** result = power(matrix, M);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << result[i][j];
            if (j != N - 1) cout << " ";
        }
        if (i != N - 1) cout << "\n";
    }

    delete_M(result);
    delete_M(matrix);
}


// (3) vector로 개선
// ** N x N 행렬을 vector 로 받는 방법

#include <iostream>
#include <vector>
using namespace std;

constexpr int MOD = 1000;
using Matrix = vector<vector<int>>;

int N;

// 단위행렬 반환
Matrix identity(int size) {
    Matrix I(size, vector<int>(size, 0));
    for (int i = 0; i < size; ++i)
        I[i][i] = 1;
    return I;
}

// 두 행렬 곱셈 (결과를 새로 생성)
Matrix multiply(const Matrix& A, const Matrix& B) {
    Matrix result(N, vector<int>(N, 0));
    for (int i = 0; i < N; ++i)
        for (int k = 0; k < N; ++k)
            for (int j = 0; j < N; ++j)
                result[i][j] = (result[i][j] + A[i][k] * B[k][j]) % MOD;
    return result;
}

// 행렬 거듭제곱 (분할 정복)
Matrix power(Matrix base, long long exp) {
    Matrix result = identity(N);
    while (exp > 0) {
        if (exp % 2 == 1)
            result = multiply(result, base);
        base = multiply(base, base);
        exp /= 2;
    }
    return result;
}

int main() {
    long long M;
    cin >> N >> M;

    Matrix A(N, vector<int>(N));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            cin >> A[i][j];

    Matrix result = power(A, M);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << result[i][j];
            if (j != N - 1) cout << ' ';
        }
        cout << '\n';
    }
}

// https://www.acmicpc.net/problem/13246
// 13246 행렬 제곱 합은 나중에 도전해보기.

// 이진 탐색 코드 ( 1920 수 찾기 )
// 초기 코드
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
vector<int> v;

void binarysearch(int N, int start, int end){
  int mid=0;
  int size = end - start + 1;

  if (size == 2) {
    if(v[start] == N || v[end] == N) cout << "1\n";
    else cout << "0\n"; // 모두 탐색함
    return;
  }
    
  else if(size % 2 == 0) mid = start + size / 2 - 1;
  else mid = start + size / 2;

  
  
  if(v[mid] == N) {
    cout << "1\n"; // 발견
    return;
  }
  else {
    if(N > v[mid])
      binarysearch(N, mid, end);
    else
      binarysearch(N, 0, mid);
  }
}

int main(){
  int N; cin >> N;
  int temp;
  
  for (int i=0; i<N; i++){
    cin >> temp;
    v.push_back(temp);
  }

  // 이진탐색 전 정렬
  sort(v.begin(), v.end());

  int M; cin >> M;
  
  for(int i=0; i<M; i++){
    cin >> temp;
    binarysearch(temp, 0, v.size()-1);
  }

  
}

// 개선 코드 (시간 초과 개선)

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

vector<int> v;

void binarysearch(int N, int start, int end) {
    if (start > end) { // 찾지 못함
        cout << "0\n";
        return;
    }

    int mid = (start + end) / 2;

    if (v[mid] == N) {
        cout << "1\n";
        return;
    } 
    else if (N > v[mid]) {
        binarysearch(N, mid + 1, end);
    } 
    else {
        binarysearch(N, start, mid - 1);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N; cin >> N;
    int temp;

    for (int i = 0; i < N; i++) {
        cin >> temp;
        v.push_back(temp);
    }

    sort(v.begin(), v.end());

    int M; cin >> M;
    for (int i = 0; i < M; i++) {
        cin >> temp;
        binarysearch(temp, 0, v.size() - 1);
    }
}


// 1654번 : 랜선 자르기 
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

vector<int> v;
int N;

// 참조가 훨씬 빠름!
// 큰 데이터를 매개변수로 넘길 때는 무조건 const & 로 넘기자.
long long Sum(const vector<int>& v, int len){
  long long sum = 0;
  
  for(int i=0; i<v.size(); i++)
    sum += v[i] / len;
  return sum; // len 일때 막대의 개수
}


// N개의 막대로 만들어야 함
void bin_search(long long start, long long end){

  long long result=0;
  
  while(start <= end) {
    long long mid = start + (end - start) / 2;
    
     if(Sum(v, mid) >= (long long)N) {
       result = mid;
       start = mid + 1;
     }
     else end = mid - 1;
  }

  cout << result << "\n";
  
}


int max_search(){
    int max=v[0];

    for(int i=1; i<v.size(); i++)
      if(max < v[i])
        max = v[i];

    return max;
}



int main() {
  ios::sync_with_stdio(false); 
  cin.tie(nullptr);
  
  int K; cin >> K >> N;
  
  for(int i=0; i<K; i++){
    int temp; cin >> temp;
    v.push_back(temp);
  }
  
  int max = max_search();

  bin_search(1, (long long)max);
  
}









// 도저히 야매로는 못풀겟으니 vector 나 python 을 활용해서 나중에 풀어보자 

