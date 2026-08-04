#include <string>
#include <vector>
#include <algorithm>
using namespace std;

vector<int> solution(vector<int> array, 
                     vector<vector<int>> commands) {
    vector<int> answer;
    
    for(int l=0; l<commands.size(); l++){
        vector<int> temp;
        
        int i = commands[l][0];
        int j = commands[l][1];
        int k = commands[l][2];
        
        for (int idx=i-1; idx<j; idx++) 
            temp.push_back(array[idx]);
        
        sort(temp.begin(), temp.end());
        answer.push_back(temp[k-1]);
        
    }
    
    return answer;
}