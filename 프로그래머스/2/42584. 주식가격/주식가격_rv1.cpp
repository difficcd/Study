#include <string>
#include <vector>
#include <stack>

using namespace std;

vector<int> solution(vector<int> prices) {
    int n = prices.size();
    vector<int> answer(n);
    stack<int> s;
    
    for (int i = 0; i < n; i++) {
        while (!s.empty() && prices[s.top()] > prices[i]) {
            answer[s.top()] = i - s.top();
            s.pop(); 
            
            // index 기준으로 value를 다룸
            // 현재 가격보다 작아졌으면 answer 기록
        }
        s.push(i);
    }
    
    while (!s.empty()) {
        // 끝까지 가격 유지된 case 한번에 처리
        answer[s.top()] = n - 1 - s.top();
        s.pop();
    }
    
    
    return answer;
}
