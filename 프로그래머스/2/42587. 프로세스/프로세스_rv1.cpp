#include <string>
#include <queue>
#include <iostream>
#include <vector>
using namespace std;

int solution(vector<int> priorities, int location) {
    
    int answer = 0;
    
    // 모범 답안 : priority_queue 사용 (O(nlogn))

    // 결국 현재 남아있는 전체 요소 중 최대 우선순위값 도출이 주 목적.
    // remain max + queue 문제.
    
    queue <pair<int,int>>q;
    priority_queue <int>pq;
    
    for(int i = 0; i < priorities.size(); i++){
        pq.push(priorities[i]); 
        q.push({i, priorities[i]});
    }

    pair<int,int> tmp;
    while(!q.empty()){
        
        if(q.front().second == pq.top() 
           && q.front().first == location) { break; }
        
        else if(q.front().second == pq.top()) {
            q.pop(); 
            pq.pop();
            answer++;
        }
        else {
            tmp = q.front(); 
            q.pop(); 
            q.push(tmp);
        }
    }

    return answer + 1;
}

