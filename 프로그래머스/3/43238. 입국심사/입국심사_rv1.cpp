#include <vector>

using namespace std;

long solution(int n, vector<int> times) {
    int tSize=times.size();
    long answer = 0, s=0, e=1e9*n, mid, doneN; 
    // 아예 최대를 e=1e9*n로 정함 (long long 범위) 
    // 단 long linux,mac에선 64bit, window에선 32bit 일 수 있음.
    // 환경에 따라 조심히 써야 함. (채점환경 고려)

    while(s<e) {
        mid=(s+e)>>1;  // 비트 연산자로 최적화 (속도)
        doneN=0;       
      
        for(int i=0; i<tSize; i++) doneN += mid/times[i];
      // 처리가능인원 게산 (동일 로직)
      
        if(doneN>=n) e=mid; // 이어가야만 함
        else s=mid+1;       // 언제 끝나든 이상하지 않음
    }

    return answer=s;
}
