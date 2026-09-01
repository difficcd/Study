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