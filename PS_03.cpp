// 16953번 그리디 풀이
#include <iostream> 
using namespace std;

int main(){
  int A, B, cnt=1;
  cin >> A; cin >> B;
  

  while(true){

    if (B<A) {
      cout << "-1";
      break;
    };
    
    
    if (B == A) {
      cout << cnt ;
      break;
    }
    else if(B % 10 == 1) B /= 10, cnt++;
    else if (B % 2 == 0) B /= 2, cnt++;
    else {
      cout << "-1";
      break;
    }
  }

}


// 1991번 트리 순회
#include <iostream>
using namespace std;

struct Node {
  char name;
  char info[2];
  Node *left;
  Node *right;

  Node(char value)
      : name(value), info{'.', '.'}, left(nullptr), right(nullptr){};
};

int N;


void preorder(Node* root){
  if(root){
    cout << root->name;
    preorder(root->left);
    preorder(root->right);
  }
}

void inorder(Node* root){
  if(root){
    inorder(root->left);
    cout << root->name;
    inorder(root->right);
  }
}

void postorder(Node* root){
  if(root){
    postorder(root->left);
    postorder(root->right);
    cout << root->name;
  }

}


int main() {
  cin >> N;
  char n1, n2, n3;

  Node *node[N];

  for (int i = 0; i < N; i++) {
    cin >> n1 >> n2 >> n3;
    node[i] = new Node(n1);
    node[i]->info[0] = n2; // left
    node[i]->info[1] = n3; // right
  }

  Node *root = node[0];

  for (int i = 0; i < N; i++) {

    if (node[i]->info[0] != '.') {
      for (int j = 0; j < N; j++) {
        if (node[j]->name == node[i]->info[0])
          node[i]->left = node[j];
      }
    }

    if (node[i]->info[1] != '.') {
      for (int j = 0; j < N; j++) {
        if (node[j]->name == node[i]->info[1])
          node[i]->right = node[j];
      }
    }
    
  }

  preorder(root); cout << "\n";
  inorder(root); cout << "\n";
  postorder(root);
  
}

// 의외로 로직이 간단해서 놀랐다.
// 이것보다 더 간단하게 하기위해서는 map 을 사용한다
#include <iostream>
#include <map>
using namespace std;

struct Node {
    char name;
    Node* left;
    Node* right;
    Node(char value) : name(value), left(nullptr), right(nullptr) {}
};

void preorder(Node* root) {
    if(root){
        cout << root->name;
        preorder(root->left);
        preorder(root->right);
    }
}

void inorder(Node* root) {
    if(root){
        inorder(root->left);
        cout << root->name;
        inorder(root->right);
    }
}

void postorder(Node* root) {
    if(root){
        postorder(root->left);
        postorder(root->right);
        cout << root->name;
    }
}

int main() {
    int N; cin >> N;
    char n1, n2, n3;

    map<char, Node*> nodes;

    // 1. 노드 생성
    for(int i=0; i<N; i++){
        cin >> n1 >> n2 >> n3;
        if(nodes.find(n1) == nodes.end()) nodes[n1] = new Node(n1);
        if(n2 != '.' && nodes.find(n2) == nodes.end()) nodes[n2] = new Node(n2);
        if(n3 != '.' && nodes.find(n3) == nodes.end()) nodes[n3] = new Node(n3);

        nodes[n1]->left = (n2 != '.') ? nodes[n2] : nullptr;
        nodes[n1]->right = (n3 != '.') ? nodes[n3] : nullptr;
    }

    Node* root = nodes.begin()->second; // 입력 순서의 첫 노드가 루트

    preorder(root); cout << "\n";
    inorder(root); cout << "\n";
    postorder(root); cout << "\n";

    return 0;
}


// 요세푸스 간단히 복습

#include <iostream>
#include <vector>
using namespace std;

int main(){
  int N, K; 
  cin >> N;
  cin >> K;

  vector<int> v, res;

  for(int i=1; i<=N; i++)
    v.push_back(i);


  int idx = K-1;

  cout << "<";
  
  for(int i=0; i<N; i++){

    if (i == N-1) cout << v[idx];
    else cout << v[idx] << ", ";

    res.push_back(v[idx]);
    v.erase(v.begin()+idx);
    if(!v.empty()) idx = (idx + K - 1) % v.size();
  }
  
  cout << ">";


}


// 1874번 : 스택 수열
#include <iostream>
using namespace std;

int temp;
int flag=0, cnt=0;

void push(int *(&stack), int &value, int &top, char* (&result)){
  stack[top] = value;
  top++;
  result[cnt] = '+'; cnt++;
}

void pop(int *(&stack), int &top, char* (&result)){
  stack[top-1] = 0;
  top--;
  result[cnt] = '-'; cnt++;
}


// 0 1 2 3 4
// 1 2 3

// top 은 다음 삽입할 위치 idx

void execute(int *(&arr), int *(&stack), int &top, int n, char *(&result)){
    for(int i=1; i<n; i++){
      
      if(arr[i] == stack[top-1])
        pop(stack, top, result);
        
      else if(arr[i] > stack[top-1]){
        for(int j=temp+1; j<=arr[i]; j++){
          
          push(stack, j, top, result); 
          
          if(j == arr[i]) temp = arr[i];
          if(arr[i] == stack[top-1]) pop(stack, top, result);
        }
      }
      else {
        cout << "NO"; 
        flag=1;
        break;
      }
      
    }
}


void print_res(char* (&result)){
  for(int i=0; i<cnt; i++)
    cout << result[i] << "\n";
}



int main(){
  int n, top=0;

  cin >> n;
  
  int* arr = new int[n];
  int* stack = new int[n];
  char* result = new char[n*2];

  
  for(int i=0; i<n; i++)   cin >> arr[i];
  for(int j=1; j<=arr[0]; j++) push(stack, j, top, result);     
  
  // top : idx
  // stack[0] = 1, stack[1] = 2, ...

  top = arr[0];
  temp = arr[0];
  
  pop(stack, top, result);
  execute(arr, stack, top, n, result);
  if(!flag) print_res(result);
  

  delete[] arr;
  delete[] stack;
  delete[] result;
  
}


// 1074 Z
#include <iostream>
using namespace std;


/*
4개 단위 => 2 2가 단위
N = 2 (4x4) 인 경우 2^N-1 = 2, 2 기준
N = 3 (8x8) 인 경우 2^N-1 = 4, 4 기준 => 2,2 기준

1 ) N = 2 (section 4)
A 0~1 x 0~1  => (N=1, r,c)
B 0~1 x '2'~3 는  4 + (0,1,2,3) => (N=1, r, c-2)
C '2'~3 x 0~1 는  4*2 + (0,1,2,3) => (N=1, r-2, c)
D '2'~3 x '2'~3 은 4*3 + .. => (N=1, r-2, c-2)

2 ) N = 3 
0~3 x 0~3   (1)
0~3 x 4~7   16 + (0,1,..,15) => (N=2, r-4, c)
4~7 x 0~3   16*2 +  => (N=2, r, c-4)
4~7 x 4~7   16*3 + 


c-2*(N-1)

3 ) N = 4
0~7 x 0~7 2)실행... 이런식으로 짜면됨
0~7 x 8~15 : 


초기 범위 1, 3, 7, 15 : 2^n/2-1
4, 4*4, 4*4*4*4 이런식으로 증가
N 2 3 4 

*/


unsigned long long z(int N, int r, int c){

  if (N == 1) { // 2 x 2
    return r * 2 + c;
  }

  unsigned long long num = 1ULL << ((N - 1) * 2); // (2^(N-1))^2 = 4^(N-1)
  
  if( 0 <= r && r < (1<<(N-1)) ) { 
    if( 0 <= c && c < (1<<(N-1)) ) { // 0~3 x 0~3
      return z(N-1, r, c);
    }
    else {  // 0~3 x 4~7
      return (num + z(N-1, r, c-(1 << (N-1))));
    }
  }
  else { 
    if( 0 <= c && c < (1<<(N-1)) ) { // 4~7 x 0~3
      return (num*2 + z(N-1, r-(1 << (N-1)), c));
    }
    else { 
      return (num*3 + z(N-1, r-(1 << (N-1)), c-(1 << (N-1))));
    }
    
  }
  return 0;
}



int main(){
  int N, r, c;
   cin >> N >> r >> c;
   cout << z(N, r, c);
}


// DFS BFS 복습
#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>

using namespace std;
vector<int> out;

void dfs(int start, vector<vector<int>> &adjList, vector<bool> &visited) {

  stack<int> s;
  s.push(start);

  while(!s.empty()){
    int v = s.top();
    s.pop();

    if(!visited[v]) visited[v] = true;
    else continue;

    out.push_back(v);

    vector<int> neighbors = adjList[v];

    sort(neighbors.rbegin(), neighbors.rend());

    for(int neighbor : neighbors){
      if(!visited[neighbor])
        s.push(neighbor);
    }
    
  }

  for (int i = 0; i < out.size(); i++) {
    if (i == out.size() - 1)
      cout << out[i];
    else
      cout << out[i] << " ";
  }
  out.clear();
}

void bfs(int start, vector<vector<int>> &adjList, vector<bool> &visited) {
  queue<int> q;
  q.push(start);
  visited[start] = true;
  

  while(!q.empty()){
    int v = q.front();
    q.pop();
    out.push_back(v);

    vector<int> neighbors = adjList[v];

    sort(neighbors.begin(), neighbors.end());

    for(int neighbor : neighbors){
      if(!visited[neighbor]){
        q.push(neighbor);
        visited[neighbor]=true;
      }
    }
  }
  
  for (int i = 0; i < out.size(); i++) {
    if (i == out.size() - 1)
      cout << out[i];
    else
      cout << out[i] << " ";
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

