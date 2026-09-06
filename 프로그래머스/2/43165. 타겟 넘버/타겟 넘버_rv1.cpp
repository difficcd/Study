
#include <string>
#include <vector>
using namespace std;

int target;
int answer = 0;

// vector 복사 오버헤드를 줄이기 위해 const vector<int>& 참조 전달 적용
void dfs(const vector<int>& numbers, int idx, int sum) {
    // if (idx + 1 > numbers.size()) return; 필요없음
    // 원래 > return 해서 아래에 return 이없어서 불필요하게 한번더재귀:오버헤드 증가

    if (idx + 1 == numbers.size()) {
        // || 대신 if문 2개로 분리 (0이 들어와도 중복 카운트 정상 처리)
        if (sum + numbers[idx] == target) answer++;
        if (sum - numbers[idx] == target) answer++;
        return; // 탐색 완료 후 불필요한 재귀 호출 방지를 위해 return 필수
    }

    dfs(numbers, idx + 1, sum + numbers[idx]);
    dfs(numbers, idx + 1, sum - numbers[idx]);
}

int solution(vector<int> numbers, int tar) {
  // 코테 채점환경이 sol()함수를 여러번 호출할 수 있음
  // 잔여 ans :전역으로 못갈수이어서 초기화를 sol에서해주어야 함
  
    answer = 0;  
    target = tar;
    dfs(numbers, 0, 0);
  
    return answer;
}

