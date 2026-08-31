#include <string>
#include <vector>
#include <algorithm>
using namespace std;

int max_cnt = 0;
bool visited[8]; 

void dfs(int k, int cnt, vector<vector<int>>& dungeons) {
    max_cnt = max(max_cnt, cnt);
    for(int i=0; i<dungeons.size(); i++){
        if(!visited[i] && dungeons[i][0] <= k){
            visited[i] = true;
            dfs(k-dungeons[i][1], cnt+1, dungeons);
            visited[i] = false;
        }
    }
}

int solution(int k, vector<vector<int>> dungeons) {
    int answer = -1;
    dfs(k, 0, dungeons);
    answer = max_cnt;
    
    return answer;
}