#include <string>
#include <vector>

using namespace std;

vector<int> solution(vector<int> progresses, vector<int> speeds) {
    vector<int> answer;
    
    // 각 반복마다 progresses += speeed;
    // progresses 역순으로 넣고 progresses == 100 되면 vec.pop
    // if 문에 걸려서 pop되는 애들 개수를 세면 됨
    
    vector<int> prog;
    vector<int> spd;
    
    for(int i = progresses.size() - 1; i >= 0; i--) {
        prog.push_back(progresses[i]);
        spd.push_back(speeds[i]); 
    }
    
    while(!prog.empty()){
        
        for(int i=0; i<prog.size(); i++)
            prog[i] += spd[i];
        
        if(prog.back() >= 100) {
            int cnt = 0;
            while(!prog.empty() && prog.back() >= 100) {
                prog.pop_back();
                spd.pop_back();
                cnt++;
            }
            answer.push_back(cnt);
        }
        
    }

    
    
    return answer;
}