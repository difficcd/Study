#include <iostream>
#include <vector>
#include <algorithm> // 퀵정렬 제공
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

