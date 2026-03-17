// 트리 구현 연습!
// C++ 으로 BST LVR순회 구현하기
// https://www.acmicpc.net/problem/24479 풀기위해 ㄱㄱ

#include <iostream>
using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;

    Node(int value) : data(value), left(nullptr), right(nullptr) {}
    // C 에서 typedef Node 해서 Node A.. 이런 식으로 생성하던 걸
    // C++ 에서는 Node(..) 로 생성함 (생성자 문법) >> 자세한 건 #구현 참조
};

void insert(Node*& root, int value) { 
    if (!root) {
        root = new Node(value);
        return;
    }
    if (value < root->data) // BST
        insert(root->left, value);
    else
        insert(root->right, value);
}

void inorder(Node* root) { // 중위 순회
    if (!root) return;
    inorder(root->left);
    cout << root->data << " ";
    inorder(root->right);
}

int main() {
    Node* root = nullptr;
    insert(root, 5);
    insert(root, 3);
    insert(root, 7);
    insert(root, 4);

    inorder(root); // 3 4 5 7
}


// 1629 번 곱셈 : 분할정복 복습과 오버플로 처리
#include <iostream>
using namespace std;

int C;

long long power(long long A, long long B){
  
  if(B == 1) return A % C;
  else {
    long long temp = power(A, B/2);
    temp = (temp * temp) % C;
    
    if (B % 2 == 0) return temp;
    else return (temp * (A % C)) % C;
  }
}


int main(){
  long long A, B;

  cin >> A >> B >> C;
  cout << power(A,B);
  
  
}


// 10434번 행복한 소수
// 소수상근수 가볍 복습

import java.util.Scanner;
import java.util.HashSet;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        
        int P= sc.nextInt();
        
        for(int i=1; i<=P; i++){
            int number= sc.nextInt();
            System.out.print(number);
            System.out.print(" ");
            
            int N= sc.nextInt();
            System.out.print(N);
            System.out.print(" ");
            
            if(N != 1) happy_num(N);
            else System.out.println("NO");
        }

        sc.close();

    }

    public static void happy_num(int N){
        HashSet<Integer> list = new HashSet<Integer>();

        String str = String.valueOf(N);
        list.add(N);

        while(true) {
            int sum = 0;
            for(int j=0; j<str.length(); j++){
                int num = str.charAt(j) - '0';
                sum += num * num;
            }
            str = String.valueOf(sum);

            if (sum == 1 && is_prime(N)) { // 먼저 1 체크
                System.out.println("YES");
                break;
            }
            if (list.contains(sum)) { // 방문했으면 무한루프 방지
                System.out.println("NO");
                break;
            }
            list.add(sum);
        }
    }

    
    public static boolean is_prime(int N){
        boolean prime =true;

        if (N != 1){
            for (int i=2; i*i <=N; i++){
                int tmp = N;
                if(tmp % i == 0)
                    prime = false;
            }
        }
        else prime = false;

        return prime;
    }
    

}


// DFS 와 BFS
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>

using namespace std;
vector<int> out;

void dfs(int start, vector<vector<int>>& adjList, vector<bool>& visited) {
    stack<int> s;
    s.push(start);

    while (!s.empty()) {
        int v = s.top();
        s.pop();
        
        if (visited[v]) continue;  
        visited[v] = true;

        out.push_back(v);

        vector<int> neighbors = adjList[v];
        
        sort(neighbors.rbegin(), neighbors.rend()); 
        // stack : 역순 정렬
        
        for (int neighbor : neighbors) {
            if (!visited[neighbor]) {
                s.push(neighbor);
            }
        }
    }

    for (int i=0; i<out.size(); i++){
        if(i==out.size()-1) cout << out[i];
        else cout << out[i] << " ";
    }
    out.clear();
}


void bfs(int start, vector<vector<int>>& adjList, vector<bool>& visited) {
    queue<int> q;
    visited[start] = true;   
    // 시작점 방문 처리
    q.push(start);

    while (!q.empty()) {
        int v = q.front();
        q.pop();

        out.push_back(v);

        vector<int> neighbors = adjList[v];
        
        sort(neighbors.begin(), neighbors.end()); 
        
        for (int neighbor : neighbors) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;  
                q.push(neighbor);
            }
        }
    }

    for (int i=0; i<out.size(); i++){
        if(i==out.size()-1) cout << out[i];
        else cout << out[i] << " ";
    }
    out.clear();
    
}


int main() {
    int V, E, start;
    cin >> V >> E >> start;

    vector<vector<int>> adjList(V + 1);


    for (int i = 0; i < E; i++) {
        int v1, v2;
        cin >> v1 >> v2;
        adjList[v1].push_back(v2);
        adjList[v2].push_back(v1);
    }

    vector<bool> visited(V + 1, false);

    dfs(start, adjList, visited);
    cout << endl;

    fill(visited.begin(), visited.end(), false);

    bfs(start, adjList, visited);
    cout << endl;

    return 0;
}


// 1012 번 유기농 배추 


#include <iostream>
#include <vector>
#include <stack>
using namespace std;

int M, N;
// 지렁이 : 상하좌우 네 방향 이동 가능
// 요소들의 연결 여부이므로, DFS 활용

vector<pair<int,int>> adjacent(int x, int y){
    vector<pair<int,int>> v;
    if(x-1 >= 0) v.push_back({x-1, y});
    if(x+1 < N)  v.push_back({x+1, y});
    if(y-1 >= 0) v.push_back({x, y-1});
    if(y+1 < M)  v.push_back({x, y+1});
    return v;
}

int worm_count(vector<vector<int>> &node){

  vector<vector<int>> visited(N, vector<int>(M, 0));
  stack<pair<int,int>> s; 
  int worms=0;

    
    for(int i=0; i<N; i++){
      for(int j=0; j<M; j++){
        if((node[i][j] == 1) && !visited[i][j]) {
          // 배추O 방문X 위치찾았으면
          
          s.push({i,j}); // 시작 노드 i,j
          worms++;
          
          while( !s.empty() ) { 

            pair<int,int> v = s.top(); 
            vector<pair<int,int>> neighbors = adjacent(v.first, v.second);
            
            s.pop();
            
            if(visited[v.first][v.second]) continue;
            visited[v.first][v.second] = 1; 

            for(pair<int,int> neighbor : neighbors){
              int nx = neighbor.first;
              int ny = neighbor.second;
              if(!visited[nx][ny] && node[nx][ny] == 1)
                  s.push({nx, ny});
            }
            
          }
        }
      }
    }
  return worms;
  
}

int main(){
  int T, K; 

  int X, Y; 

  cin >> T; 
  
  for(int i=0; i<T; i++){ 
    
      cin >> M; cin >> N; // 땅 크기
      cin >> K; // 좌표 수
  
      vector<vector<int>> node(N, vector<int>(M, 0));
    
      for(int j=0; j<K; j++){
        cin >> X; cin >> Y;
        node[Y][X] = 1;
      }

      cout << worm_count(node) << "\n";
    
    
  }
  
}
