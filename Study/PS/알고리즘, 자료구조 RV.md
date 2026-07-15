


## deque : 양방향 큐 

=> 아마 안 배운 애인거같음.
훈련해보기.

## Hash 문제 기본

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



