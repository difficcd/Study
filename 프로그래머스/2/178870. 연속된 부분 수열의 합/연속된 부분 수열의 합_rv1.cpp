#include <vector>

using namespace std;

vector<int> solution(vector<int> sequence, int k) {
    vector<int> answer = {0, (int)sequence.size()};
    int sum = 0;
    
    for (int l = 0, r = 0; r < sequence.size(); r++) {
        sum += sequence[r]; // 현재 r을 sum에 추가 (r=0부터이므로 시작요소 포함
        
        while (sum > k) {
            sum -= sequence[l++];
        } 
      // sum이 k 초과하면, l: k 범위 이내로 될 때까지 shrink
      // l < r 유지 가능 : sum 크기로 인해 모순 없이 shrink함
        
        if (sum == k) {
            if (r - l < answer[1] - answer[0]) {
                answer = {l, r};
            } // vector<int> size 고정이면 {} 할당 가능
        }     //  answer[1] - answer[0] 로 min_len 대체 가능
    }

  // cycle 적인 설계 (while? for? 1개?2개?)를 미리 효율적으로 해둬야 함
  // 이 문제는 사실 l++ 보다는 r++ 이 경향적으로 작음 : r기준 l이동(while)
  // 내 로직은 l 기준으로 편협하게 끌고가려다가 복잡해졌고, 부정확해짐(통과는 했지만)
  // 총정리 : 로직적인 부분(sum의 변화, r,l의 이동 간단화) 고민 많이 하고 숙련시키기
    
    return answer;
}
