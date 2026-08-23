
> C++ 기준, 자료구조/알고리즘에 필요한 알고리즘 지식 정리 




# 이진 탐색

## 기본
### 🔍 이진탐색이란?

- **핵심 조건:** 데이터가 **오름차순(또는 내림차순)으로 정렬**되어 있어야만 사용 가능
    
- **작동 방식:**  1부터 100 사이의 숫자를 맞출 때, 무식하게 1부터 다 세는 게 아니라 "중앙값(50)"을 먼저 물어보고, 업이면 절반을 버리고, 다운이면 또 절반을 버리면서 **범위를 매번 절반씩 깎아 나가는 방식**
    
- **시간 복잡도:** $O(\log N)$


### 💻 가장 기본형 코드 (배열에서 값 찾기)

C++에서는 `std::binary_search`를 써도 되지만, 
직접 구현하는 뼈대를 알아두면 나중에 응용하기 편해:

```C++
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool binarySearch(const vector<int>& arr, int target) {
    int left = 0;
    int right = arr.size() - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2; // 중간 인덱스 계산
        
        if (arr[mid] == target) {
            return true; // 찾았다!
        } 
        else if (arr[mid] < target) {
            left = mid + 1; // target이 더 크니까 왼쪽 반은 버림
        } 
        else {
            right = mid - 1; // target이 더 작으니까 오른쪽 반은 버림
        }
    }
    
    return false; // 끝내 못 찾음
}
```


### 💡 PS(코테)에서 진짜 중요한 것 (파라메트릭 서치)

여러 문제에서 쓰는 건 배열 안에서 특정 숫자를 찾는 게 아님.

- **"우리가 구하려는 정답(예: 상한액, 최소 시간 등)의 범위를 `left`와 `right`로 잡는다."**
    
- **"그 중간값(`mid`)을 정답이라고 가정했을 때, 조건에 맞는지(가능한지) 안 맞는지 판단한다."**
    
- 가능하면 범위를 더 좁히고, 불가능하면 다른 쪽 범위를 버리는 식으로 **최적의 정답을 찾아내는 것** 이걸 파라메트릭 서치(Parametric Search)라고 부름. 코테에 나오는 이진탐색의 90%는 다 이 형식






## mid? mid+-1?

**파라메트릭 서치(최적화 문제)에서는 상황에 따라 `left = mid + 1`을 쓰기도 하고, `left = mid`를 쓰기도 함.**

### 🔍 언제 `mid + 1` / `mid - 1`을 쓰고, 언제 `mid`를 쓸까

#### 1. 일반적인 '값 찾기' 이분탐색

- 배열 안에서 특정 원소 `target`의 위치를 찾을 때는 `left = mid + 1`, `right = mid - 1`을 쓰는 게 맞음. 왜냐하면 **이미 `mid` 자리에 있는 값은 `target`이 아니라고 확인**했기 때문에 다음 탐색 범위에서 확실하게 제외(`+1` 또는 `-1`)해 주는 것
    
#### 2. 파라메트릭 서치 (최적값 문제)

- 이번 문제처럼 "최솟값"이나 "최댓값"을 찾는 파라메트릭 서치에서는 코드를 어떻게 짜느냐에 따라 갈려.
    
- 만약 `mid` 시간이 **가능하다면**, 이 `mid`도 일단 정답의 후보가 될 수 있음. 그래서 범위를 줄일 때 `right = mid` (포함시킴)로 두고, **불가능한 경우에만** 확실하게 버리기 위해 `left = mid + 1`을 쓰는 방식을 자주 사용함
    
- 반대로 `left = mid + 1`, `right = mid - 1` 형태로 꽉 조이게 구현할 수도 있음. 단, 이 경우에는 반복문이 끝났을 때 정답이 어느 변수에 담기는지(보통 `left`나 저장해둔 변수) 신경 써야 할 디테일들이 조금 달라짐
    

### 💡 핵심 요약

- `mid`를 포함시키느냐 마느냐는 "현재 `mid`가 정답 후보로서 의미가 있는가?"에 따라 결정
    
- 일부 문제에서는 `mid` 시간으로 처리가 가능하다고 판별되어도, "더 최적의(더 짧은) 시간이 존재할 수 있으니까" 범위를 왼쪽으로 좁히되, 혹시 지금 `mid`가 최후의 최솟값일 수도 있으니 `right = mid`로 두고 좁히는 방식을 쓰기도 함.
    
- 반대로 "이미 검사했으니 얘는 확실히 빼고 가겠다" 싶으면 `mid + 1 / mid - 1`로 깔끔하게 쳐내도 됨.
    

편한 구조로 설계하되, 무한 루프(Infinite Loop)에 빠지지 않게 `mid` 계산 방식(`left + (right - left) / 2`)과 범위를 좁히는 조건만 잘 맞추면 됨

`left`와 `right`가 1 차이 날 때 `mid`가 `left`로 계산되면서 `left`가 제자리걸음을 쳐서 영원히 끝나지 않는 무한 루프(시간 초과)에 빠질 위험은 꼭 대비해야 함.


# DFS, BFS

## 기초

### DFS : iterator & flag 구현

**LOGIC ) DFS (스택 기반)**

- 방문여부 저장, 스택, 인접요소 접근 도구가 필요함
- 스택의 LIFO 구조를 통한 깊은 탐색이 가능한 이유 : 나중에 push된 neighbor가 먼저 pop → 한 branch를 깊게 탐색하며, 같은 부모의 다른 neighbor도 스택에 남아 있어 같은 부모의 다른 자식을 탐색가능하도록 해줌. 모든 branch 를 탐색하므로 깊은 탐색이 가능
- DFS 는 LIFO 구조의 stack 기반이므로 꺼낼 때 방문 처리가 필요하다. (스택에 노드를 넣는 과정에서 방문 처리하면 로직적 오류)

```C++
void dfs()
  스택 선언 및 스택에 시작노드 넣기
  
  while(스택이 빌 때까지) 
     <*>  스택의 top을 v로 두고(출력용) top 삭제
  	 <*>  v가 방문한 정점이면 continue, 방문하지 않으면 방문 flag 갱신 및 출력
     
     <*>  새로운 벡터 neighbors = adjlist[v] 
     <*>  위의 벡터를 통해 v의 인접정점 list 를 쉽게 접근가능
  	 <*>  작은 정점부터 방문하기 위해서는 해당 벡터 neighbors 를 정렬해줌
  		 * 스택이므로 역순으로 정렬해야 함 (top 을 이용하기 때문)
  
       <#> 중심 로직 :
         for (int neighbor : 모든 neighbors 요소에 대해)
         if (방문되지 않았으면)
         스택에 neighbor push
```

---

### BFS : iterator & flag 구현

**LOGIC ) BFS (큐 기반)**

- 인접 행렬, 방문여부 벡터, 스택, 인접요소 접근 벡터가 필요함
- 비교적 DFS 보다는 간단함. 그냥 인접한것 방문을 차례대로 하면서 push 해주면 됨
- DFS 는 FIFO 구조의 queue 기반이므로 꺼낼 때 방문 처리가 필요하다. (큐에 노드를 넣는 과정에서 방문 처리하지 않으면 로직적 오류)

```C++
void bfs()
  큐 선언 밎 큐에 시작노드 넣고, 방문 처리
  
  while(큐가 빌 때까지) 
     <*>  큐의 front를 v로 두고(출력용) pop()
     
     <*>  새로운 벡터 neighbors = adjlist[v] 
     <*>  위의 벡터를 통해 v의 인접정점 list 를 쉽게 접근가능
  	 <*>  작은 정점부터 방문하기 위해서는 해당 벡터 neighbors 를 정렬해줌
  		 * 큐이므로 정방향 정렬
         
       <#> 중심 로직 :
         for (int neighbor : 모든 neighbors 요소에 대해)
         if (방문되지 않았으면)
          neighbor 방문 처리하고, 큐에 push
```

#### 실제 구현 코드

```C++
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
        visited[v] = true; // stack top 방문 처리

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
    q.push(start); 
    visited[start] = true;   // 시작점 방문 처리
  
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

```




## 