#include <string>
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


