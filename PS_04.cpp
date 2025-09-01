// 7576 토마토
#include <iostream>
#include <queue>
using namespace std;

int M, N; //  2 ≤ M,N ≤ 1,000
int blocked=0;
// 1익음 0안익음 -1빈공간
// 끝까지 방문하고 방문몬한 곳이 있으면 -1


vector<pair<int, int>> adjacent(int n, int m){ // 0 to ...
  vector<pair<int, int>> adj;
  if(n+1 < N)  adj.push_back({n+1,m});
  if(n-1 >= 0)  adj.push_back({n-1,m});
  if(m+1 < M)  adj.push_back({n,m+1});
  if(m-1 >= 0)  adj.push_back({n,m-1});

  return adj;
}


int BFS_tomato(vector<vector<int>>& box) {
    queue<pair<int,int>> q;

    // 시작점(익은 토마토) 전부 큐에 넣기
    for (int i=0; i<N; i++) {
        for (int j=0; j<M; j++) {
            if (box[i][j] == 1) q.push({i,j});
        }
    }

    int days = 0;
    while (!q.empty()) {

      pair<int,int> p = q.front();
      int x = p.first;
      int y = p.second;
      q.pop();
      
        for (pair<int, int> adj : adjacent(x,y)) {
          int nx = adj.first, ny = adj.second;
            if (box[nx][ny] == 0) {
                box[nx][ny] = box[x][y] + 1; // 날짜 저장
                q.push({nx,ny});
                days = max(days, box[nx][ny]);
            }
        }
    }

    // 결과 확인
    for (int i=0; i<N; i++) {
        for (int j=0; j<M; j++) 
            if (box[i][j] == 0) return -1; // 못 익은 토마토 있음
    }

    return days - 1; // 처음 1에서 시작했으니 -1
}


int main(){
  cin >> M >> N;
  int zero=0;

  vector<vector<int>> box(N, vector<int>(M)); 
  // 크기를 미리 지정해야 box[i][j] 접근 가능
  vector<pair<int,int>> start;
  
  for(int i=0; i<N; i++){
    for(int j=0; j<M; j++){
      int temp; cin >> temp; 
      box[i][j] = temp;
      
      if(temp == 1) start.push_back({i,j});
      if(temp == 0) zero++; 
    }
  }


  int result = BFS_tomato(box);
  
  if(blocked) cout << "-1";
  else if(zero) cout << result;
  else cout << "0";

  // 상하좌우가 인접 정점 list 임
  // 1이 들어온 위치를 기억해서 모두 인자로 보내야함

  
}


// 토마토 초기 코드
#include <iostream>
#include <queue>
using namespace std;

int M, N; //  2 ≤ M,N ≤ 1,000
int blocked=0;
// 1익음 0안익음 -1빈공간
// 끝까지 방문하고 방문몬한 곳이 있으면 -1


vector<pair<int, int>> adjacent(int n, int m){ // 0 to ...
  vector<pair<int, int>> adj;
  if(n+1 < N)  adj.push_back({n+1,m});
  if(n-1 >= 0)  adj.push_back({n-1,m});
  if(m+1 < M)  adj.push_back({n,m+1});
  if(m-1 >= 0)  adj.push_back({n,m-1});

  return adj;
}


int All_empty(vector<queue<pair<int,int>>> qlist){
  int count=0;
  for(int i=0; i<qlist.size(); i++){
    if(qlist[i].empty()) count++;
  }
  
  if(count == qlist.size()) return 1;
  else return 0;
}


void printbox(vector<vector<int>> box){
  for(int i=0; i<N; i++){
    for(int j=0; j<M; j++)
      cout << box[i][j] << " ";
    cout << "\n";
  }
}




int BFS_tomato(vector<vector<int>> &box, vector<pair<int,int>> start){

  int count=0;
  int size = start.size(); // 병렬 처리해야 하는 시작 정점 수

  vector<vector<int>> box_temp(N, vector<int>(M));  // 바로 바꾸지 않기 위한 임시공간
  vector<queue<pair<int,int>>> qlist(size); // 시작 정점 수만큼 queue 생성

  box_temp = box; 
  
  for(int i=0; i<size; i++) // 각 start vertex 를 큐에 넣어줌
    qlist[i].push(start[i]);

  // 모든 큐가 빌 때까지 작업 수행 : All_empty
  // 한 반복당 시작정점의모든 1에 대해 수행해야 함

  int flag=1;
  while(!All_empty(qlist) && flag){

    flag=0;
    
    pair<int,int> v[size];
    
    for(int i=0; i<size; i++){
      int qs = qlist[i].size();

      for(int s=0; s<qs; s++){
        v[i] = qlist[i].front();
        qlist[i].pop();
    
        for(pair<int, int> adj : adjacent(v[i].first,v[i].second)){
          if(!box[adj.first][adj.second]){
            box_temp[adj.first][adj.second] = 1;
            flag=1;
            qlist[i].push(adj);
          }
        }
      }

      for(int i=0; i<N; i++){
        for(int j=0; j<M; j++)
          box[i][j] = box_temp[i][j];
      }
    }
    
    if (flag) count++; 
  }

  for(int i=0; i<N; i++){
    for(int j=0; j<M; j++){
      if(box[i][j] == 0) blocked = 1;
    }
  }

  
  return count;
}


int main(){
  cin >> M >> N;
  int zero=0;

  vector<vector<int>> box(N, vector<int>(M)); 
  // 크기를 미리 지정해야 box[i][j] 접근 가능
  vector<pair<int,int>> start;
  
  for(int i=0; i<N; i++){
    for(int j=0; j<M; j++){
      int temp; cin >> temp; 
      box[i][j] = temp;
      
      if(temp == 1) start.push_back({i,j});
      if(temp == 0) zero++; 
    }
  }


  int result = BFS_tomato(box, start);
  
  if(blocked) cout << "-1";
  else if(zero) cout << result;
  else cout << "0";

  // 상하좌우가 인접 정점 list 임
  // 1이 들어온 위치를 기억해서 모두 인자로 보내야함

  
}

