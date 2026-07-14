#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

string solution(vector<string> participant, 
                vector<string> completion) {
    string answer = "";
    
    unordered_map<string, bool> com_map;
    unordered_map<string, int> count;
        
    for (string s : completion) 
        com_map[s] = true;
        
    for (string s : completion) count[s]--;
    
    
        
    for (string s : participant) {
        if(count.find(s) != count.end()) 
            count[s] += 1; // 존재하면 +=1
        else count[s] = 1;
    }
    
    // 동명이인이 여러명 있어도 그 수만큼 체크해야 함
    // 이 이름에 대해 몇 명이 있는가를 알아야 함
    
    for (string s : participant) {
        if(com_map.find(s) == com_map.end()){
            answer = s;
            count[s]--;
        }
    }
    if(answer == ""){
        for (auto const& [key, value] : count) {
            if(value != 0) answer = key;
        }   
    }
    
    
    return answer;
}