#include <string>
#include <vector>
#include <stack>
using namespace std;

int solution(int n, vector<vector<int>> computers) {
    int answer = 0;
    
    vector<int> visited(computers.size(), 0);
    stack<int> s;   // index 로 순회
    
    for(int k=0; k<computers.size(); k++){
        
        s.push(k);
        
        if(visited[k] != 1){
            answer++;
            while(!s.empty()){
                int idx = s.top();
                vector<int> node = computers[idx];
                s.pop(); 

                if(visited[idx] == 1) continue;
                visited[idx] = 1;

                for(int i=0; i<computers[0].size(); i++){
                    if(i != idx && computers[idx][i] == 1
                       && visited[i] != 1){
                        s.push(i);
                    }
                }
            }
        }
        
    }
    
    return answer;
}