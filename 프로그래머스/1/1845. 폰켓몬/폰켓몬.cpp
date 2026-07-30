#include <vector>
#include <unordered_map>
using namespace std;

int solution(vector<int> nums)
{
    int answer = 0;
    
    // size / 2 가 최대임 (shortcut eval 가능)
    // 그 전까지는 해시에 하나하나 넣어서 개수 세기
    
    unordered_map<int, bool> poke_map;
    
    for (int i : nums) {
        poke_map[i] = true;
    }
    
    if(poke_map.size()  >= nums.size() / 2)
        answer = nums.size() / 2 ;
    else answer = poke_map.size();
    
    
    return answer;
}