#include <string>
#include <iostream>
#include <vector>
using namespace std;

bool solution(string s)
{
    bool answer = true;
    vector<int> stack;
    // (((())) => (만큼 넣고 )만큼 빼면 ( 1개 남음.
    
    int num = 0;
    for(int i=0; i<s.size(); i++){
        if(s[i] == '(') stack.push_back(0);
        
        if(s[i] == ')'){
            if(!stack.empty()) stack.pop_back(); 
            else if(stack.empty()) return false;
        }
    }
    
    if(!stack.empty()) answer = false;

    return answer;
}