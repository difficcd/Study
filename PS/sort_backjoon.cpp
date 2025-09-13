https://www.acmicpc.net/problem/24090
이거랑 수 정렬하기 시리즈에 각 알고리즘들 다 구현 연습하기
>> 각 언어들의 라이브러리도 사용해 볼 것
https://www.acmicpc.net/problem/15688
해당 문제는 중복값이 많은 경우 퀵정렬 기반의 sort 성능이 떨어지므로
시간 초과가 나옴 => 개선책 마련해서 해결해보는것을 정렬 학습 목표로 삼아볼것

https://www.acmicpc.net/problem/11729
하노이 탑
1번째 탑 가장 위의 원판을 3번째 탑의 위로 올림


7
1 3
1 2
3 2
1 3
2 1
2 3
1 3
...


#include <iostream>
#include <vector>
#include <algorithm> 
using namespace std;


int main() {
    int N; cin >> N;
    vector<pair<int,int>> v;
    
    for(int i=0; i<N; i++){
        int temp;
        cin >> temp;
        v.push_back({temp, i});
        // 인덱스와 함께 저장
    }

    sort(v.begin(), v.end());

    // 벡터 정렬방식
    // arr 의 경우 arr, arr+N 

    int max_count=0;
    for(int i=0; i<N; i++){
        if(max_count < v[i].second-i)
            max_count = v[i].second-i;
    }

    cout << max_count+1;
  
}

// 퀵 정렬 알고리즘 학습 C++ 예제 코드 (랜덤 피벗)
#include <iostream>
#include <vector>
#include <time.h>
#include <random>

using namespace std;

random_device rd;
mt19937 mt(rd());

template <typename T>
void quickSort_(vector<T>& vec, const int& left, const int& right){
  if (right <= left){
    return;
  }
  uniform_int_distribution<int> randInt(left, right);
  int leftArrow = left + 1;
  int rightArrow = right;
  swap(vec[left], vec[randInt(mt)]);
  int current = left;
  bool focusedOnRight = true;
    
  do {
    if (focusedOnRight){
      if (vec[current] >= vec[rightArrow]){
        swap(vec[current], vec[rightArrow]);
        current = rightArrow;
        focusedOnRight = !focusedOnRight;
      }
      --rightArrow;
    }
    else {
      if (vec[current] <= vec[leftArrow]){
        swap(vec[current], vec[leftArrow]);
        current = leftArrow;
        focusedOnRight = !focusedOnRight;
      }
      ++leftArrow;
    }
  } while (rightArrow >= leftArrow);
    
  quickSort_(vec, left, current - 1);
  quickSort_(vec, current + 1, right);
}


int main(){
    time_t start = time(nullptr);
    vector<pair<int,int>> v;
    
    int N; cin >> N;
    for (int i=0; i<N; i++){
        int temp;
        cin >> temp;
        v.push_back({temp, i});
    }

    quickSort_(v,0, v.size()-1);
    
    for(int i=0; i<N; i++){
        cout << v[i].first << "\n";
    }
}


// 위 코드의 랜덤 피벗을 중간값으로 바꾼 버전
#include <iostream>
#include <vector>
#include <time.h>
#include <random>

using namespace std;

template<typename T>
int medianOfThree(std::vector<T>& arr, int left, int right) {
    int mid = left + (right - left) / 2;

    // 정렬되지 않은 세 값 중 중간값의 인덱스를 반환
    if (arr[left] > arr[mid]) std::swap(arr[left], arr[mid]);
    if (arr[left] > arr[right]) std::swap(arr[left], arr[right]);
    if (arr[mid] > arr[right]) std::swap(arr[mid], arr[right]);

    // 이제 arr[mid]가 중간값
    return mid;
}


template <typename T>
void quickSort_(vector<T>& vec, const int& left, const int& right){
  if (right <= left){
    return;
  }
  uniform_int_distribution<int> randInt(left, right);
  int leftArrow = left + 1;
  int rightArrow = right;
  int pivot = medianOfThree(vec, left, right);
  swap(vec[left], vec[pivot]);
  int current = left;
  bool focusedOnRight = true;
    
  do {
    if (focusedOnRight){
      if (vec[current] >= vec[rightArrow]){
        swap(vec[current], vec[rightArrow]);
        current = rightArrow;
        focusedOnRight = !focusedOnRight;
      }
      --rightArrow;
    }
    else {
      if (vec[current] <= vec[leftArrow]){
        swap(vec[current], vec[leftArrow]);
        current = leftArrow;
        focusedOnRight = !focusedOnRight;
      }
      ++leftArrow;
    }
  } while (rightArrow >= leftArrow);
    
  quickSort_(vec, left, current - 1);
  quickSort_(vec, current + 1, right);
}


int main(){
    vector<pair<int,int>> v;
    
    int N; cin >> N;
    for (int i=0; i<N; i++){
        int temp;
        cin >> temp;
        v.push_back({temp, i});
    }

    quickSort_(v,0, v.size()-1);
    
    for(int i=0; i<N; i++){
        cout << v[i].first << "\n";
    }
}

