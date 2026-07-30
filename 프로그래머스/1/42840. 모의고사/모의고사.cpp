#include <string>
#include <vector>
#include <array>
#include <algorithm>

using namespace std;

vector<int> solution(vector<int> answers) {
    vector<int> answer;
    
    array<int, 3> count = {0,0,0};
    
    array<int, 5> s1 = {1,2,3,4,5};
    array<int, 8> s2 = {2,1,2,3,2,4,2,5};
    array<int, 10> s3 = {3,3,1,1,2,2,4,4,5,5};
    
    for(int i=0; i<answers.size(); i++){
        if(answers[i] == s1[i % 5]) count[0]++;
        if(answers[i] == s2[i % 8]) count[1]++;
        if(answers[i] == s3[i % 10]) count[2]++;
    }
    
    int maxidx = 0;
    for(int i=1; i<3; i++)
        if(count[maxidx] < count[i]) 
            maxidx = i;
        
    answer.push_back(maxidx+1);
    for(int i=0; i<3; i++)
        if(maxidx != i && count[maxidx] == count[i])
            answer.push_back(i+1);
    
    sort(answer.begin(), answer.end());
    
    
    return answer;
}