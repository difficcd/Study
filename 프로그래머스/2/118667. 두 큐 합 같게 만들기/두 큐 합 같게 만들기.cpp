#include <string>
#include <vector>
#include <deque>

using namespace std;

int solution(vector<int> queue1, vector<int> queue2) {
    int answer = 0;

    // 두 큐의 합 큰 큐 원소를 pop (FIFO) 해서 작은쪽에 push
    // long 고려 필요.. 큐 길이가 30만, 합계산 ov 조심
    // 10^5 * 10^9 => 10^16 ~= 2^48 정도일 수 있으니 long사용
    
    // 1 1   1 3 5
    // 2 9 => 3 8 => 6 8 => 11 0 
    // 불가능한 경우 : 무조건 한쪽 큐의 합이 0이 됨.
    
    deque<int> dq1;
    deque<int> dq2;
    
    for (int i=0; i<queue1.size(); i++){
        dq1.push_back(queue1[i]);
        dq2.push_back(queue2[i]); 
    }
    
    long long sum1=0, sum2=0; 
    
    for(int i=0; i<dq1.size(); i++) 
        sum1 += (long long)dq1[i];

    for(int i=0; i<dq2.size(); i++) 
        sum2 += (long long)dq2[i];

    while(1){
        
        if(sum1 == sum2) break;
        
        if(sum1 > sum2) {
            int temp = dq1.front();
            dq1.pop_front();
            dq2.push_back(temp);
            sum1-= temp;
            sum2+= temp;
        }
        else {
            int temp = dq2.front();
            dq2.pop_front();
            dq1.push_back(temp);
            sum2-= temp;
            sum1+= temp;
        }
        
         answer++;
        
        if(dq1.empty() || dq2.empty()
           || answer > queue1.size()*4) {
            answer = -1;
            break;
        }
        
    }
    
    return answer;
}