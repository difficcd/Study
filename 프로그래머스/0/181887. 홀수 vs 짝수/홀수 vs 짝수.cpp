#include <string>
#include <vector>

using namespace std;

int max(int a, int b){
    if(a>b) return a;
    else return b;
}
    
int solution(vector<int> num_list) {
    int answer = 0;
    int osum=0, esum=0;
    
    for(int i=0; i<num_list.size(); i++){
        if(i % 2 == 0)
            esum += num_list[i];
        else osum += num_list[i];
        
    }
    
    answer = max(osum, esum);
    
    return answer;
}