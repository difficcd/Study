#include <string>
#include <vector>
#include <algorithm>
using namespace std;

int solution(vector<int> citations) {
    int answer = 0;
    int n = citations.size();
    sort(citations.begin(), citations.end());

    // n<1000 O(n) 10e6
    for(int h=0; h<=n; h++){
        int cnt=0;
        for(int i=0; i<n; i++){
            if(citations[i]>=h) cnt++;
        }
        
        if(h<=cnt && answer<h) answer=h;
        if(h>cnt) break;
    }
    
    return answer;
}