#include <string>
#include <vector>
#include <algorithm>
using namespace std;

string solution(vector<int> numbers) {
    string answer = "";
    vector<string> temp;

    for(int i : numbers) temp.push_back(to_string(i));

    sort(temp.begin(), temp.end(), [](const string& a, 
                                      const string& b){
       return a+b > b+a;
    });
    
    for(string s : temp ) answer += s;

    if(answer[0] == '0') answer = "0";
    
    return answer;
}