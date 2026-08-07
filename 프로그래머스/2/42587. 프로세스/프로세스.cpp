
#include <vector>
#include <queue>
using namespace std;

int solution(vector<int> priorities, int location) {
    int answer = 0;
    int count[9] = {};  // count[0]~[8] = 1~9 
    
    queue<pair<int, int>> q;
    vector<int> ans;

    int max = 0;   
    for(int i=0; i<priorities.size(); i++) {
        q.push({i, priorities[i]});
        // q : index, priorities
        
        if(max < priorities[i]) max = priorities[i];
        for(int c=0; c<9; c++) 
            if(c+1 == priorities[i]) count[c]++;
    }
    
   while(!q.empty()){
       pair<int,int> proc = q.front(); 
       
       if(max <= 0) break;
       if(count[max-1] <= 0 && max > 0) {
         
           for(int j=max-1; j>=0; j--){
               if(count[j] > 0) {
                   max = j+1;
                   break;
               }
           }
       }
       
       if(max == proc.second) {
               q.pop();
               ans.push_back(proc.first);
               count[proc.second-1]--;
       }
       else {
           q.pop();
           q.push(proc);
       }
   }
    
    for(int i=0; i<ans.size(); i++) {
        if(ans[i] == location) {
            answer = i+1;
            break;
        }
    }
    
    
    return answer;
}