
> C++ 기준, 자료구조/알고리즘에 필요한 lib 사용지식 정리



## unordered_map

### 문법 

`std::unordered_map`은 키(Key)와 값(Value)의 쌍을 저장하는 연관 컨테이너로, 내부적으로 **해시 테이블**을 사용하여 평균 $O(1)$이라는 매우 빠른 속도로 데이터를 관리합니다.

가장 자주 쓰는 핵심 사용법 5가지만 정리해 드립니다.

#### 1. 헤더 포함 및 선언

먼저 `<unordered_map>` 헤더를 포함해야 합니다.

```C++
#include <unordered_map>
#include <string>

// 기본 선언: <키 타입, 값 타입>
std::unordered_map<std::string, int> umap;
```

#### 2. 데이터 삽입 (`insert` 또는 `[]`)

```C++
// 1. [] 연산자 사용 (가장 간편함, 키가 없으면 생성, 있으면 덮어쓰기)
umap["apple"] = 10;

// 2. insert 사용 (키가 이미 존재하면 삽입 안 됨)
umap.insert({"banana", 20});
```

#### 3. 데이터 접근 및 확인

```C++
// 접근
int val = umap["apple"]; 

// 키 존재 여부 확인 (매우 중요!)
// find()는 키를 못 찾으면 map.end()를 반환합니다.
if (umap.find("apple") != umap.end()) {
    std::cout << "apple이 존재함" << std::endl;
}
```

#### 4. 삭제 (`erase`)

```C++
// 키를 기준으로 삭제
umap.erase("apple");
```

#### 5. 전체 순회

```C++
// pair를 이용한 순회
for (auto const& [key, value] : umap) {
    std::cout << key << ": " << value << std::endl;
}
```

#### PS에서 주의할 점 (꿀팁)

1. **`[]` 연산자의 위험성**: `umap["banana"]`라고 접근하는 순간, 만약 "banana"라는 키가 없다면 **무조건 0(또는 기본값)으로 초기화해서 새로운 노드를 생성**해버립니다. 단순히 키가 있는지 확인만 하려면 `find()`나 `count()`를 쓰세요.
    
2. **`count()` 함수**: 단순히 "키가 존재하는가?"만 알고 싶을 때는 `if (umap.count("apple"))`처럼 쓰면 코드가 매우 깔끔해집니다. (키가 있으면 1, 없으면 0 반환)
    
3. **순서**: `unordered_map`은 해시 기반이라 **데이터가 들어간 순서가 유지되지 않습니다.** 출력하면 뒤죽박죽으로 나옵니다. (순서가 중요하다면 `std::map`을 써야 하지만, PS에서는 보통 속도 때문에 `unordered_map`을 씁니다.)


### 예시

```Cpp
#include <iostream>
#include <vector>
#include <unordered_map>

int main() {
    std::vector<int> a = {2, 4, 1};
    std::vector<int> b = {2, 4};

    // 1. b에 있는 원소들을 해시 맵에 등록 (존재 여부 체크용)
    std::unordered_map<int, bool> b_map;
    for (int x : b) {
        b_map[x] = true;
    }

    // 2. a를 돌면서 b_map에 없는 녀석만 출력
    for (int x : a) {
        if (b_map.find(x) == b_map.end()) {
            std::cout << "b에 없는 값: " << x 
			          << std::endl;
        }
    }

    return 0;
}
```


**팁:** PS에서 "어떤 두 값을 더해서 뭔가가 된다"는 유형의 문제는 **해시 맵**을 쓰면 거의 바로 풀리는 경우가 많으니 꼭 기억해두세요!




# unordered_set

std::unordered_set은 말 그대로 "순서가 없는 집합"입니다.C++에서 가장 많이 쓰는 컨테이너 중 하나로, "중복을 허용하지 않고" 특정 값이 들어있는지 "빛의 속도로 찾고 싶을 때" 사용합니다. 

<unordered_set> 헤더를 포함해야 합니다.

핵심 특징 3가지

- 중복 불가: 같은 값을 여러 번 넣어(insert)도 하나만 남습니다.순서 없음: 원소들이 입력한 순서대로 저장되지 않고, 내부 해시 알고리즘에 의해 랜덤하게 배치됩니다.
- 압도적인 탐색/삽입 속도: 내부적으로 해시 테이블(Hash Table)을 사용하기 때문에, 원소를 찾거나 넣고 지울 때 평균 $O(1)$ (상수 시간)이라는 미친 속도를 자랑합니다.

- 기본 사용법 & 주요 함수

```cpp
#include <iostream>
#include <unordered_set>

int main() {
    std::unordered_set<int> s;

    s.insert(10);
    s.insert(20);
    s.insert(30);
    s.insert(20); // 중복 데이터는 무시

    // 2. 원소 찾기 (find) - 가장 중요한 기능!
    // 못 찾으면 s.end()를 반환합니다.
    if (s.find(20) != s.end()) {
        std::cout << "20이 존재합니다!" << std::endl;
    }

    // 3. 존재 여부 확인 (count) - C++20 전까지 많이쓰임
    // 중복이 안 되므로 있으면 1, 없으면 0 반환
    if (s.count(30)) {
        std::cout << "30이 존재합니다!" << std::endl;
    }

    // 4. 원소 삭제 (erase)
    s.erase(10); // 10 제거

    // 5. 크기 확인
    std::cout << "크기: " << s.size() << std::endl;         // 출력: 2 (20, 30만 남음)

    // 6. 전체 순회 (범위 기반 for문)
    // 순서는 저장한 순서와 다르게 출력될 수 있습니다.
    for (const auto& num : s) {
        std::cout << num << " ";
    }

    return 0;
}
```


std::set vs std::unordered_set 차이점 (중요!)알고리즘 문제를 풀 때 두 개를 언제 써야 할지 헷갈리는 경우가 많습니다.

| **비교 항목**     | **std::set**                          | **std::unordered_set**                          |
| ------------- | ------------------------------------- | ----------------------------------------------- |
| **내부 구조**     | 레드-블랙 트리 (이진 탐색 트리)                   | **해시 테이블 (Hash Table)**                         |
| **정렬 여부**     | **자동 오름차순 정렬됨**                       | **정렬 안 됨 (순서 무작위)**                             |
| **탐색 시간 복잡도** | $O(\log N)$                           | **평균 $O(1)$** (최악 $O(N)$)                       |
| **추천 상황**     | **정렬된 상태**로 유지하거나<br><br>범위 검색이 필요할 때 | 단순 **중복 제거**나<br><br>**존재 여부(있나 없나) 빠르게 조회**할 때 |

# set, map

set, map 도 unordered 랑 동일한 사용법이지만 내부 구현이 다름. 상황에 따라 골라서 사용해주어야 함.
## 1. `set` vs `unordered_set` (값 하나만 저장)

| 구분         | `std::set`                                                    | `std::unordered_set`                                    |
| ---------- | ------------------------------------------------------------- | ------------------------------------------------------- |
| **내부 구조**  | **레드-블랙 트리** (이진 탐색 트리)                                       | **해시 테이블 (Hash Table)**                                 |
| **정렬 여부**  | **오름차순으로 자동 정렬됨**                                             | **정렬 안 됨** (순서 무작위)                                     |
| **시간 복잡도** | 탐색/삽입/삭제 모두 **$O(\log N)$**                                   | 탐색/삽입/삭제 **평균 $O(1)$** (최악 $O(N)$)                      |
| **헤더 파일**  | `<set>`                                                       | `<unordered_set>`                                       |
| **주요 용도**  | 데이터가 **정렬된 상태**여야 하거나,<br>`lower_bound()` 같은 범위 탐색이 필요할 때<br> | 단순 **중복 제거**나<br><br>**존재 여부("아까 나온 숫자인가?") 빠르게 조회**할 때 |

---

## 2. `map` vs `unordered_map` (Key - Value 짝지어 저장)

| 구분         | `std::map`                                       | `std::unordered_map`                          |
| ---------- | ------------------------------------------------ | --------------------------------------------- |
| **내부 구조**  | **레드-블랙 트리**                                     | **해시 테이블 (Hash Table)**                       |
| **정렬 여부**  | **Key를 기준으로 자동 정렬됨**                             | **Key 정렬 안 됨**                                |
| **시간 복잡도** | Key 탐색/삽입/삭제 모두 **$O(\log N)$**                  | Key 탐색/삽입/삭제 **평균 $O(1)$** (최악 $O(N)$)        |
| **헤더 파일**  | `<map>`                                          | `<unordered_map>`                             |
| **주요 용도**  | Key 순서대로 출력을 해야 하거나, key의 최대 최솟값을 자주 찾아야 할 때<br> | key로 value를 O(1) 속도로 조회하거나,<br><br>빈도수 카운팅할 때 |


### 1. 기본 선택은 무조건 `unordered_` 붙은 녀석!

* 대부분의 코테 문제는 **"이게 있냐 없냐"**, "이 Key의 Value가 뭐냐"만 빠르게 묻습니다.
* $O(1)$ 속도를 내는 `unordered_set` / `unordered_map`을 **기본값**으로 먼저 고려하세요.

### 2. 이럴 때만 `unordered_`를 빼고 `set` / `map` 쓰기!

1. **정렬이 필요할 때:** 원소나 Key가 들어오는 족족 **자동으로 정렬**되어 유지되어야 할 때
2. **범위 탐색이 필요할 때:** "x보다 큰 값 중 가장 작은 값"을 찾는 `lower_bound()`, `upper_bound()`를 써야 할 때 (`unordered_` 시리즈는 불가능)
3. **최댓값/최솟값을 계속 빼야 할 때:** `*s.begin()` (최솟값), `*s.rbegin()` (최댓값) 접근이 필요할 때

---

> **한 줄 요약**
> * **`unordered_` (해시):** 정렬 따위 상관없고 **속도($O(1)$)가 최고**일 때
> * **`접두사 없음` (트리):** 속도는 약간 느려도($O(\log N)$) **정렬 상태 유지**가 필수일 때
> 



# priority_queue

**"우선순위가 가장 높은 원소가 먼저 나오는"** 자료구조. 
내부적으로는 힙(Heap) 트리 구조로 구현되어 있어서, 값을 넣고 뺄 때마다 자동으로 정렬됨.

### 1. 기본 선언 및 헤더

```C++
#include <queue>
using namespace std;
```


```C++
// 1. 기본: 최대 힙 (숫자가 클수록 우선순위가 높음 = 큰 값이 튀어나옴)
priority_queue<int> pq;

// 2. 최소 힙 (숫자가 작을수록 우선순위가 높음 = 작은 값이 튀어나옴)
priority_queue<int, vector<int>, greater<int>> min_pq;
```

### 2. 핵심 메서드 (기본 조작)

큐(`queue`)나 스택(`stack`)이랑 인터페이스가 거의 동일

- **`push(val)`**: 원소 추가 (로그 시간 $O(\log N)$)
- **`pop()`**: 우선순위가 가장 높은 원소 삭제 (반환은 안 해줌!)
- **`top()`**: 우선순위가 가장 높은 원소 **확인 (조회)**
- **`empty()`**: 큐가 비었으면 `true`, 아니면 `false`
- **`size()`**: 원소의 개수 반환

### 3. 코드 예시


```C++
#include <iostream>
#include <queue>

using namespace std;

int main() {
    // 최대 힙 생성
    priority_queue<int> pq;

    // 데이터 삽입
    pq.push(10);
    pq.push(30);
    pq.push(20);
    pq.push(5);

    // top()으로 가장 큰 값 확인하고 pop()으로 제거하기
    while (!pq.empty()) {
        cout << pq.top() << " "; // 출력: 30 20 10 5
        pq.pop();
    }

    return 0;
}
```

### 4. 커스텀 정렬 (구조체나 클래스를 넣을 때)

앞서 배운 커스텀 정렬처럼, 입맛대로 우선순위를 정하고 싶을 때는 
구조체와 연산자 오버로딩(`operator<`)을 쓰거나, 커스텀 비교 함수(Comparator)를 지정


```C++
// 예시: pair를 쓸 때 오름차순으로 정렬하고 싶다면?
// priority_queue<T, Container, Compare> 구조를 가짐
priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> custom_pq;
```

주로 **가장 큰 값을 빠르게 뽑아내거나(다익스트라, 그리디 등)** 할 때 자주 사용함.

# stack

- **헤더:** `#include <stack>`

- **주요 함수:**
    
    - `push(val)`: 맨 위에 데이터 추가
    - `pop()`: 맨 위 데이터 삭제 (반환값 없음)
    - `top()`: 맨 위 데이터 반환 (조회)
    - `empty()`: 비어있으면 `true`, 아니면 `false`
    - `size()`: 데이터 개수 반환


```Cpp
#include <iostream>
#include <stack>

using namespace std;

int main() {
    stack<int> s;

    // 데이터 삽입
    s.push(10);
    s.push(20);
    s.push(30); // 스택 상태: [10, 20, 30(Top)]

    cout << "Top element: " << s.top() << "\n"; // 30

    // 데이터 순차적으로 꺼내기
    while (!s.empty()) {
        cout << s.top() << " "; // 30 -> 20 -> 10 순서로 출력
        s.pop();
    }
    cout << "\n";

    return 0;
}
```



# queue

- **헤더:** `#include <queue>`
    
- **주요 함수:**
    
    - `push(val)`: 뒤(Tail)에 데이터 추가
    - `pop()`: 앞(Head) 데이터 삭제 (반환값 없음)
    - `front()`: 가장 앞에 있는 데이터 반환
    - `back()`: 가장 뒤에 있는 데이터 반환
    - `empty()`: 비어있으면 `true`, 아니면 `false`
    - `size()`: 데이터 개수 반환
    
### 코드 예제

```C++
#include <iostream>
#include <queue>

using namespace std;

int main() {
    queue<int> q;

    // 데이터 삽입
    q.push(10);
    q.push(20);
    q.push(30); // 큐 상태: [10(Front) ... 30(Back)]

    cout << "Front element: " << q.front() << "\n"; // 10
    cout << "Back element: " << q.back() << "\n";   // 30

    // 데이터 순차적으로 꺼내기
    while (!q.empty()) {
        cout << q.front() << " "; // 10 -> 20 -> 30 순서로 출력
        q.pop();
    }
    cout << "\n";

    return 0;
}
```


# deque

### (Double-Ended Queue - 양방향 큐)

앞과 뒤 양쪽 모두에서 데이터의 삽입과 삭제가 가능한 유연한 자료구조입니다. 
(`vector`처럼 인덱스로 조회도 가능)



- **헤더:** `#include <deque>`
    
- **주요 함수:**
    
    - `push_front(val)`: 맨 앞에 데이터 추가
    - `push_back(val)`: 맨 뒤에 데이터 추가
    - `pop_front()`: 맨 앞 데이터 삭제
    - `pop_back()`: 맨 뒤 데이터 삭제
    - `front()` / `back()`: 앞/뒤 데이터 조회
    - `dq[i]`: `vector`처럼 인덱스로 접근 가능 (`O(1)`)
    - `empty()` / `size()`: 비어있는지 확인 / 개수 반환
    

### 코드 예제

```C++
#include <iostream>
#include <deque>

using namespace std;

int main() {
    deque<int> dq;

    // 양방향 삽입
    dq.push_back(20);  // 뒤에 넣기: [20]
    dq.push_front(10); // 앞에 넣기: [10, 20]
    dq.push_back(30);  // 뒤에 넣기: [10, 20, 30]

    // 인덱스로 접근 가능
    cout << "Index 1 value: " << dq[1] << "\n"; // 20

    // 양방향 삭제 및 조회
    cout << "Front: " << dq.front() << ", Back: " << dq.back() << "\n"; // Front: 10, Back: 30

    dq.pop_front(); // 앞 제거 ([20, 30남음])
    dq.pop_back();  // 뒤 제거 ([20만 남음])

    cout << "Remaining size: " << dq.size() << "\n"; // 1

    return 0;
}
```




# max_element

범위 안에서 가장 큰 값이 있는 "위치"(반복자, Iterator)를 찾아주는 함수. 
(단, 실행 횟수를 고려해야 함. 데이터를 한 번 훑기 때문에 O(N))

```C++
#include <algorithm>

vector<int> v = {1, 3, 9, 2, 5};

// v에서 가장 큰 값 찾기 (반환값이 포인터/반복자 형태임)
auto max_it = max_element(v.begin(), v.end());

// 최댓값의 "값"을 보려면 *를 붙임
int max_val = *max_it; // 9
```



# priority_queue

일반 `queue`는 먼저 넣은 게 먼저 나오는 구조(`FIFO`)지만, 
**우선순위 큐**는 넣은 값들 중에서 무조건 가장 큰(우선순위가 높은) 놈이 맨 앞으로 옴.

```C++
#include <queue>

// 기본 선언 (기본적으로 숫자가 클수록 우선순위가 높음 -> 내림차순)
priority_queue<int> pq;

pq.push(3);
pq.push(10);
pq.push(5);

// 현재 들어있는 것 중 가장 큰 값이 튀어나옴
pq.top(); // 10 (pop하지 않고 보기만 함)

pq.pop(); // 가장 큰 10을 삭제
```

- **두 번째 코드의 논리:**
    
    1. `q`에는 **`(위치, 중요도)`** 쌍을 넣고, `pq`에는 `중요도`만 몽땅 집어넣어.
        
    2. `pq.top()`을 하면 지금 남아있는 것 중 **가장 센 중요도**가 뭔지 0초 만에 알 수 있어.
        
    3. 큐의 맨 앞(`q.front().second`)과 `pq.top()`을 비교해서, 지금 꺼낸 녀석이 대장(최댓값)이 맞는지 곧바로 판정하는 거야.
        




# 