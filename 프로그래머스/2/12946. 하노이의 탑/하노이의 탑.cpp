#include <string>
#include <vector>
#include <iostream>
using namespace std;

vector<int> v[3];
vector<vector<int>> answer;

void hanoi(int num, int from, int via, int to){
    if(num==0) return;
    
    hanoi(num-1, from, to, via);
    
    answer.push_back({from+1, to+1});
    v[to].push_back(v[from].back());
    v[from].pop_back();

    hanoi(num-1, via, from, to);
}

vector<vector<int>> solution(int n) {
    
    for(int i=n; i>=1; i--) v[0].push_back(i);
    hanoi(n, 0, 1, 2);
    
    return answer;
}