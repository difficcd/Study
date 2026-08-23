#include <string>
#include <vector>

using namespace std;

vector<int> solution(vector<int> seq, int k) {
    vector<int> answer(2);
    
    int l = 0;
    int r = l;  
    int sum = seq[l];
    int min_len = seq.size() + 1;
    
    while(l<seq.size() && r<seq.size() && l <= r) {
        
        if(sum == k && min_len > r-l+1) {
            min_len = r-l+1;
            answer[0] = l;
            answer[1] = r;
            
            r++;
            sum += seq[r];
            continue;
        }
        else if (sum == k) {
            r++;
            sum += seq[r];
            continue;
        }
        if(sum > k) {
            sum -= seq[l];
            l++;
        }
        if(sum < k) {
            r++;
            sum += seq[r];
        }
    }
    
    
    return answer;
}
