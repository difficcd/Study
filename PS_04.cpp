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


// 2206 벽 부수고 이동하기
#include <iostream>
#include <vector>
#include <tuple>
#include <queue>
#include <string>
using namespace std;

int N, M;

vector<tuple<int, int,int>> adjacent(tuple<int, int, int> v){

  int x = get<0>(v); 
  int y = get<1>(v);
  int z = get<2>(v); // 상태 보존 필요
  // >> broken = 1 된 이후로는 계속 1인 상태로 큐에 들어가줘야 함

  vector<tuple<int, int, int>> t;

  if( x+1 < N ) t.push_back({x+1,y,z});
  if( x-1 >= 0) t.push_back({x-1,y,z});
  if (y+1 < M) t.push_back({x,y+1,z});
  if (y-1 >= 0) t.push_back({x,y-1,z});

  return t;
}


int BFS_wall(vector<vector<int>> map, vector<vector<vector<int>>> &visited){

  queue<tuple<int, int, int>> q;
  q.push({0, 0, 0});
  visited[0][0][0] = 1; 

  while(!q.empty()){
    tuple<int, int, int> v = q.front();
    q.pop();

    int x = get<0>(v);
    int y = get<1>(v);
    int z = get<2>(v); 

    for(tuple<int, int, int> i : adjacent(v)){
      int nx = get<0>(i);
      int ny = get<1>(i);
      int nz = get<2>(i); // 인접정점의 원래 broken 여부

      if(!map[nx][ny] && !visited[nx][ny][z]){ // 길 존재 & 방문안함
        visited[nx][ny][z] = visited[x][y][z]+1; // map 방문
        // * 횟수 count 를 위해 level을 visited 값으로 둬야 함
        // * visited(인접정점의 x,y &  기존정점의 z) = 기존정점의 visited 값+1 (level)
        q.push({nx,ny,z}); 
      }

      if(map[nx][ny] && !nz) { // 벽 & 부순적 없음
        visited[nx][ny][1] = visited[x][y][z]+1;
        // * visited(인접정점의 x,y, 방문되었으니 z=1 상수로 지정) = ...
        q.push({nx,ny,1});
      }

    }

  }

  // result를 큐 전체에서 max로 갱신하지 말고
  // 도착점(N-1,M-1)의 visited[x][y][0/1] 중 최소를 선택해야 정확함

  int c0 = visited[N-1][M-1][0];
  int c1 = visited[N-1][M-1][1];

  if(c0 && c1) return min(c0,c1);
  else if(c0) return c0;
  else if(c1) return c1;
  else return -1;

}





int main(){
  cin >> N >> M;


  vector<vector<int>> map(N, vector<int>(M, 0));
  vector<vector<vector<int>>> visited(N, vector<vector<int>>(M, vector<int>(2, 0)));

  /*
  visited[x][y][0] : 벽 안 부수고 (x, y) 도착했을 때 방문 여부
  visited[x][y][1] : 벽 부수고 (x, y) 도착했을 때 방문 여부
  */

  for(int i=0; i<N; i++){
    string str; cin >> str;
    for (int j=0; j<M; j++) {
        int digit = str[j] - '0'; 
        map[i][j] = digit;
    }  
  }

  int result = BFS_wall(map, visited);

  if(visited[N-1][M-1][0] || visited[N-1][M-1][1]) cout << result << "\n";
  else cout << "-1";

}


