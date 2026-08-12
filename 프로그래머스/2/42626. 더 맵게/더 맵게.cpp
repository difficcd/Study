#include <string>
#include <vector>
#include <queue>
using namespace std;

int solution(vector<int> scoville, int K) {
    int answer = 0;
    
    priority_queue<int, vector<int>, greater<int>> pq; 
    
    for(int i : scoville) pq.push(i);
    
    while(pq.top() < K){

        if(pq.size() == 1) {
            answer = -1;
            break;
        }
        
        int temp = 0; 
        
        answer++;
        temp = pq.top();     pq.pop();
        temp += 2*pq.top();  pq.pop();
        
        pq.push(temp);
        
    }
    
    return answer;
}