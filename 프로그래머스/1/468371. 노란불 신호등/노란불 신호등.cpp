/* #include <string>
#include <vector>
#include <iostream>

using namespace std;


int solution(vector<vector<int>> signals) {
    int answer = -1;

    // 노란색시간 = 출발점 + 주기 ~ 범위적으로 처리

    // 만약 지금시간 > 출발점이면
    // 지금시간 - 출발점 = 주기(G+Y+R)*반복횟수 이면 노란색.
    // 반복횟수 = 지금시간 - 출발점 을 주기로 나눈 것.

    int time = 10000000;
    
    for (int t=1; t<time; t++){
        int finish = 1;
        for (int j=0; j<signals.size(); j++){
            int start = signals[j][0];
            int cycle = start + signals[j][1] 
                              + signals[j][2];
                
            if(t > start) {
                int flag = (t - start - 1) % cycle;
                    if(flag >= signals[j][1]){
                        finish = 0;
                        break;
                    } // 하나라도 노란 불이 아니면 반복
            }
            else { finish = 0; break; }
        }
        if(finish == 1) {
            answer = t;
            break;
        }
    }

    return answer;
}
*/



#include <vector>
using namespace std;
// 개선 코드

long long mygcd(long long a, long long b){ 
    return b==0 ? a : mygcd(b, a%b);  
}
long long mylcm(long long a, long long b){ 
    return a / mygcd(a,b) * b;  // LCM (유클리드 호제법 RV)
}

int solution(vector<vector<int>> signals) {
    long long period = 1;
    
    for (auto& s : signals) {
        long long cycle = s[0] + s[1] + s[2]; // cycle = g+y+r
        period = mylcm(period, cycle);   
        // 모든 신호등 주기의 LCM = 진짜 전체 주기
    }

    // 확인할 시간을 period 로 정확하게 계산 가능 (magic number X)
    for (long long t = 1; t <= period; t++) {
        bool allYellow = true;
        for (auto& s : signals) {
            long long start = s[0];
            long long cycle = s[0] + s[1] + s[2];
            
            if (t > start) { // 로직은 동일함
                long long flag = (t - start - 1) % cycle;
                if (flag >= s[1]) { 
                    allYellow = false; break; 
                }
            } else {
                allYellow = false; break;
            }
        }
        if (allYellow) return (int)t;
    }
    return -1;
}


