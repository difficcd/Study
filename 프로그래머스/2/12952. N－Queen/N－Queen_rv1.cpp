
// =============== 제출 코드 ================= //
// 131.92ms, memory: 4.57MB 수준

#include <string>
#include <vector>
#include <iostream>
using namespace std;

bool isOk(int x, int y, vector<int> board){
    for(int i=0; i<board.size(); i++){
        if(board[i] != -1){
            if(board[i] == y) return false;
            if(abs(x-i) == abs(y-board[i])) return false;
        }
    }
    return true;
}

void dfs(int &cnt, int c, int n, vector<int> &board){
    if (n == 0) { cnt++; return; }
    for(int i=0; i<board.size(); i++){
        if(board[i] == -1 && isOk(i,c,board)){
           board[i] = c;
           dfs(cnt, c+1, n-1, board);
           board[i] = -1;
        }
    }    
}

int solution(int n) {
    int answer = 0;

    vector<int> board(n, -1);
    dfs(answer, 0, n, board);
    
    return answer;
}





// ===============  개선된 코드 ================= //
// ~90.75ms  ~5.01MB 수준으로 효율성이 대폭 향상됨

#include <string>
#include <vector>
#include <iostream>
using namespace std;

// const & 로 복사오버헤드 피하기. (call by ref)
// const 없는 피드백 코드 > 내 코드  >>> 완전한 피드백 코드인 이유:
// isOk 부를때마다 vector 새로 할당해서 오버헤드가 커졌기 떄문임.
// 내 코드는 dfs에서 내 방식으로 검사하다 보니 매번 생성하진 않았음.

bool isOk(int x, int y, const vector<int> &board){
    for(int i=0; i<board.size(); i++){
        if(board[i] != -1){
            if(board[i] == y) return false;
            if(abs(x-i) == abs(y-board[i])) return false;
        }
    }
    return true;
}

void dfs(int &cnt, int row, int n, vector<int> &board){
  // n개의 queen 다 놓았으면 정답 카운트 : row==n return.
    if (row == n) { cnt++; return; }

  // 어떤 col에 놓을지 결정하는 루프
    for(int col = 0; col < n; col++){
        if(isOk(row, col, board)){
            board[row] = col;         
            dfs(cnt, row + 1, n, board); 
            board[row] = -1;           
        }
    }    
}

int solution(int n) {
    int answer = 0;

    vector<int> board(n, -1);
    dfs(answer, 0, n, board);
    
    return answer;
}
