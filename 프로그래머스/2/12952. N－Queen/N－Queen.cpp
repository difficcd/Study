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

void dfs(int &cnt, int row, int n, vector<int> &board){
    if (row == n) { cnt++; return; }
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