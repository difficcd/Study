#include <string>
#include <vector>
using namespace std;

int target;
int answer=0;

void dfs(vector<int> numbers, int idx, int sum) {
    
    if(idx+1 > numbers.size()) return;
    if(idx+1 == numbers.size()) {
        if(sum + numbers[idx] == target
           || sum - numbers[idx] == target) { 
            answer++;
        }
    }
    dfs(numbers, idx+1, sum + numbers[idx]);
    dfs(numbers, idx+1, sum - numbers[idx]);
}


int solution(vector<int> numbers, int tar) {
    
    target = tar;
    dfs(numbers, 0, 0);
    
    return answer;
}