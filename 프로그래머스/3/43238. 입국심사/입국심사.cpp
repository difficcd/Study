#include <string>
#include <vector>
#include <algorithm>
using namespace std;

long long solution(int n, vector<int> times) {
    long long answer = 0;
    long long m = 0;
    for(int i=0; i<times.size(); i++){
        if(m < times[i]) m = times[i];
    }
    
    long long max = m * n;
    
    long long left = 0;
    long long right = max; 
    long long tmp = max;
    
    while(left <= right) {
        long long mid = left + ( right - left ) / 2;
        
        long long cnt = 0;
        for(int i=0; i<times.size(); i++)
            cnt += mid / times[i];

        if(cnt >= n){
            answer = mid;
            right = mid - 1;
        }
            
        else if(cnt < n)
            left = mid + 1;
    }
    
    return answer;
}