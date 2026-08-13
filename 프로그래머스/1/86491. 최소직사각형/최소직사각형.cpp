#include <string>
#include <vector>
#include <algorithm>
using namespace std;

int solution(vector<vector<int>> sizes) {
    int answer = 0;
    vector<int> w;
    vector<int> h;

    for(int i=0; i<sizes.size(); i++){
        if(sizes[i][0] < sizes[i][1])
            swap(sizes[i][0], sizes[i][1]);
    }
    
    for(int i=0; i<sizes.size(); i++){
        w.push_back(sizes[i][0]);
        h.push_back(sizes[i][1]);
    }
    
    sort(w.begin(), w.end());
    sort(h.begin(), h.end());
  
    answer = w[w.size()-1] * h[h.size()-1];
        
    return answer;
}