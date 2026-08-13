
> 기초 라이브러리 지식은 여기에, 
> 본격적인 PS 기초는 [[PS BASIC]] 에 정리.

# toupper, tolower
### 1. 개별 문자 변환 (`<cctype>`)

단일 문자를 다룰 때는 `std::toupper()`와 `std::tolower()`를 사용합니다.

```C++
#include <iostream>
#include <string>
#include <cctype> // toupper, tolower

using namespace std;

int main() {
    string str;
    cin >> str;

    for(char &c : str) {
        if (isupper(c)) cout << (char)tolower(c);
        else cout << (char)toupper(c);
    }
    return 0;
}
```

### 2. 문자열 전체 변환

C++의 `std::string` 전체를 변환할 때는 `std::transform` 알고리즘을 사용하는 것이 가장 깔끔합니다.

```C++
#include <iostream>
#include <string>
#include <algorithm> // std::transform용
#include <cctype>    // std::toupper/tolower용

int main() {
    std::string text = "Hello World";

    // 대문자로 변환
    std::transform(text.begin(), text.end(), text.begin(), ::toupper);
    std::cout << "Upper: " << text << std::endl;

    // 소문자로 변환
    std::transform(text.begin(), text.end(), text.begin(), ::tolower);
    std::cout << "Lower: " << text << std::endl;

    return 0;
}
```

#### 주요 포인트

- **헤더 파일:** `<cctype>`를 반드시 포함해야 합니다.
    
- **`std::transform`:** 컨테이너(문자열 등)의 시작(`begin`)부터 끝(`end`)까지 특정 동작(함수)을 적용할 때 사용하는 강력한 도구입니다.
    
- **주의사항:** `std::toupper`나 `std::tolower`를 단독으로 사용할 때, 일부 환경에서는 인자 타입을 `unsigned char`로 캐스팅해 주는 것이 안전합니다. (예: `::toupper(static_cast<unsigned char>(c))`)

### 3. 아스키 코드 활용법

- **숫자 0~9:** `48`('0') ~ `57`('9')
    
- **대문자 A~Z:** `65`('A') ~ `90`('Z')
    
- **소문자 a~z:** `97`('a') ~ `122`('z')

- **숫자 문자를 정수로:** `'0'`의 아스키 코드 값은 **48**입니다. 따라서 문자 `'5'`에서 `'0'`(또는 48)을 빼면 실제 정수 `5`가 됩니다.


![[Pasted image 20260709161714.png]]
#### 코드 예시

```C++
#include <iostream>

int main() {
    // 1. 문자 숫자 '5'를 정수 5로 변환
    char charDigit = '5';
    int num = charDigit - '0'; // 53 - 48 = 5
    std::cout << "Integer: " << num << std::endl;

    // 2. 소문자 'a'를 대문자 'A'로 변환
    char lower = 'a';
    char upper = lower - 32; // 'a'(97) - 32 = 'A'(65)
    std::cout << "Upper: " << upper << std::endl;

    // 3. 대문자 'A'를 소문자 'a'로 변환
    char upper2 = 'A';
    char lower2 = upper2 + 32; // 'A'(65) + 32 = 'a'(97)
    std::cout << "Lower: " << lower2 << std::endl;

    return 0;
}
```

#### 이 방법의 특징

- **장점:** 매우 빠르고 메모리 사용량이 없습니다. 함수 호출 오버헤드가 없어서 성능이 극도로 중요한 알고리즘 문제(PS) 풀이 등에서 아주 애용됩니다.
    
- **주의사항:** * **가독성:** `c - 48`이나 `c - '0'`이라고 적으면 의도는 명확하지만, `toupper()` 같은 표준 함수를 사용하는 것에 비해 '의도'를 파악하는 데 한 번 더 생각이 필요할 수 있습니다.
    
    - **환경:** 아스키(ASCII) 코드를 사용하는 시스템(대부분의 현대 시스템)에서는 완벽하게 동작하지만, 아주 드문 특수 환경(EBCDIC 등)에서는 동작하지 않을 수 있습니다.
        

실무적인 코드에서는 협업하는 사람들을 위해 가독성이 좋은 `<cctype>` 함수를 선호하고, 알고리즘 테스트나 효율이 극도로 중요한 구간에서는 질문하신 아스키 연산 방식을 자주 사용합니다.


# String
## String length

### 올바른 방법

C++의 `std::string` 클래스는 길이를 반환하는 함수로 `length()`와 `size()` 두 가지를 제공합니다. **두 함수는 완전히 동일하게 동작**하므로 원하시는 것을 사용하시면 됩니다.

```C++
#include <iostream>
#include <string>

int main() {
    std::string str = "Hello";

    // 둘 다 가능합니다
    std::cout << "길이 (length): " << str.length() << std::endl;
    std::cout << "길이 (size): " << str.size() << std::endl;

    return 0;
}
```

### 왜 `len()`은 안 되나요?

- C++ 표준 라이브러리(STL)의 컨테이너들은 일관성을 위해 길이를 반환하는 함수 이름을 `size()`로 통일했습니다.
    
- `std::string`은 컨테이너처럼 동작하도록 설계되었기 때문에 `size()`를 기본으로 사용하고, 기존 C 스타일 문자열의 `strlen()`과 혼동하지 않도록 `length()`라는 별칭(alias)도 함께 제공하는 것입니다.

### 참고: C 스타일 문자열의 경우

만약 `std::string` 객체가 아니라 `char` 배열(C 스타일 문자열)을 사용 중이라면 `strlen()` 함수를 사용해야 합니다.

```C++
char arr[] = "Hello";
// std::cout << arr.length(); // 이건 에러!
std::cout << strlen(arr);    // 이건 가능 (헤더 <cstring> 필요)
```

`std::string`을 사용하신다면 고민하지 말고 `.size()`나 `.length()`를 사용하시면 됩니다!


## 문자열 관련 함수

### 1. 기본 정보 및 길이 확인

가장 기본적으로 문자열의 상태를 확인하는 함수들입니다

|**함수**|**설명**|
|---|---|
|`str.length()` / `str.size()`|문자열의 길이를 반환합니다.|
|`str.empty()`|문자열이 비어있으면 `true`, 아니면 `false`를 반환합니다.|
|`str.capacity()`|현재 메모리 재할당 없이 저장 가능한 최대 문자 수를 반환합니다.|
|`str.clear()`|문자열의 모든 문자를 삭제합니다.|

### 2. 문자 접근 및 변경

특정 위치의 문자를 제어할 때 사용합니다.

|**함수**|**설명**|
|---|---|
|`str[i]` / `str.at(i)`|`i`번째 인덱스의 문자에 접근합니다. (`at`은 범위 체크를 수행함)|
|`str.front()`|첫 번째 문자를 반환합니다.|
|`str.back()`|마지막 문자를 반환합니다.|
|`str.push_back(c)`|문자열 끝에 문자 `c`를 추가합니다.|
|`str.pop_back()`|문자열의 마지막 문자를 제거합니다.|

### 3. 문자열 조작 (수정)

문자열을 합치거나, 자르거나, 수정하는 핵심 기능입니다.

| **함수**                     | **설명**                                |
| -------------------------- | ------------------------------------- |
| `str.append(s)`            | 문자열 `s`를 뒤에 붙입니다. (`+=` 연산자로 대체 가능)   |
| `str.substr(pos, len)`     | `pos` 위치에서 `len` 길이만큼의 부분 문자열을 반환합니다. |
| `str.replace(pos, len, s)` | `pos` 위치부터 `len`만큼을 `s`로 교체합니다.       |
| `str.insert(pos, s)`       | `pos` 위치에 `s`를 삽입합니다.                 |
| `str.erase(pos, len)`      | `pos` 위치부터 `len`만큼 삭제합니다.             |

### 4. 탐색 및 검색

특정 문자나 문자열이 어디에 있는지 찾을 때 유용합니다.

|**함수**|**설명**|
|---|---|
|`str.find(s)`|문자열 `s`가 처음 나타나는 위치(인덱스)를 반환합니다.|
|`str.rfind(s)`|문자열 `s`가 마지막으로 나타나는 위치를 뒤에서부터 찾습니다.|
|`str.find_first_of(s)`|`s`에 포함된 문자 중 하나라도 처음 나타나는 위치를 찾습니다.|

### 5. 기타 유용한 변환 (숫자 ↔ 문자열)

`<string>` 헤더에 포함된 유틸리티 함수들입니다.

- **숫자 → 문자열:** `std::to_string(value)`
- **문자열 → 숫자:** `std::stoi()`, `std::stoll()`, `std::stof()`, `std::stod()` 등

### 예시 코드

```C++

#include <iostream>
#include <string>

int main() {
    std::string str = "Hello, C++!";

    // 1. 길이 확인
    std::cout << "길이: " << str.length() << std::endl;

    // 2. 부분 문자열 (substr)
    std::string sub = str.substr(7, 3); // "C++"
    std::cout << "부분: " << sub << std::endl;

    // 3. 찾기 (find)
    size_t pos = str.find("C++");
    if (pos != std::string::npos) {
        std::cout << "'C++' 위치: " << pos << std::endl;
    }

    // 4. 숫자 변환
    int num = 123;
    std::string sNum = std::to_string(num);
    
    return 0;
}
```

이 함수들만 잘 활용해도 문자열 처리는 대부분 가능합니다!



# Vector

`<vector>` 헤더를 포함해야 사용할 수 있습니다.
### 1. 주요 멤버 함수

중간 원소를 it erase 로 지우면 => 자리가 당겨짐.
맨 마지막 원소가 지워졌으면 v.end반환.

중요실수: it순회하던중에 erase 쓰면 큰일남.
seg fault! (it++ 로 순회하면 큰일나는 것. 정확히는..
it++은 규칙적 증가를 하니까 이상한 애를 가리킴.
지울 것이면 알아서 it++ 을 하지 말고 안 지우는 경우에만 it++ 해야함! 밀리는 특성 떄문에.)

| **분류**    | **함수**               | **설명**                           |
| --------- | -------------------- | -------------------------------- |
| **추가/제거** | `push_back(val)`     | 뒤에 값 `val`을 추가합니다.               |
|           | `pop_back()`         | 마지막 요소를 제거합니다.                   |
|           | `insert(it, val)`    | 반복자(`it`)가 가리키는 위치에 `val` 삽입.    |
|           | `erase(it)`          | 반복자(`it`)가 가리키는 요소 제거.           |
|           | `clear()`            | 모든 요소 삭제.                        |
| **접근**    | `operator[]`         | `v[i]` 형태로 접근 (범위 검사 X, 빠름).     |
|           | `at(i)`              | `v.at(i)` 형태로 접근 (범위 검사 O, 안전함). |
|           | `front()` / `back()` | 첫 번째 / 마지막 요소 반환.                |
| **정보**    | `size()`             | 현재 들어있는 요소 개수.                   |
|           | `capacity()`         | 메모리 할당 용량 (실제 수용 가능 개수).         |
|           | `empty()`            | 비어있으면 `true` 반환.                 |
|           |                      |                                  |

벡터 값 실제로 변경하기 (참조로 원본값 직접 변경)

```cpp
// 1. 값 수정하기 (참조 & 사용)
for (int& i : v) {
    i--; // v 안의 실제 값이 1씩 감소함!
}

// 2. auto 활용 (권장)
for (auto& i : v) {
    i--;
}
```

### 2. 예시 코드

가장 자주 쓰이는 방식들입니다.

```C++
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v;

    // 1. 추가
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);

    // 2. 접근
    std::cout << "첫 번째 요소: " << v[0] << std::endl;
    std::cout << "마지막 요소: " << v.back() << std::endl;

    // 3. 순회 (범위 기반 for문)
    for (int n : v) {
        std::cout << n << " ";
    }
    std::cout << std::endl;

    // 4. 삭제
    v.pop_back(); // 30 제거

    // 5. 전체 삭제
    v.clear();
    std::cout << "크기: " << v.size() << std::endl;

    return 0;
}
```

### 3. 알아두면 좋은 팁

- **초기화:** `std::vector<int> v(5, 0);` 하면 크기 5짜리 벡터가 0으로 채워져서 생성됩니다.
    
- **메모리:** `push_back`을 할 때마다 매번 메모리를 재할당하면 느려집니다. 데이터 개수를 미리 안다면 `v.reserve(100);`을 통해 미리 공간을 확보해두는 것이 성능상 매우 좋습니다.
    
- **반복자(Iterator):** `v.begin()`은 시작 위치, `v.end()`는 끝 다음 위치를 반환합니다. `std::sort(v.begin(), v.end());`처럼 알고리즘과 결합할 때 필수적입니다.


### 1. 벡터의 고정 크기 배열 (`vector<int> v[N]`) — 질문하신 방법

가장 직관적인 방법입니다. **"세로 크기는 고정되어 있고, 가로 크기는 마음대로 늘어나는"** 구조입니다. 주로 알고리즘 문제(PS)에서 그래프의 인접 리스트를 구현할 때 정말 많이 씁니다.

```cpp
#include <iostream>
#include <vector>

int main() {
    // 크기가 5인 vector 배열 생성 (v[0]부터 v[4]까지 존재)
    std::vector<int> v[5];

    // 각각의 벡터에 데이터 추가
    v[0].push_back(10);
    v[0].push_back(20);
    
    v[1].push_back(30);

    // 출력할 때도 2차원 배열처럼 접근
    std::cout << v[0][1] << std::endl; // v[0]의 1번째 원소인 20 출력

    return 0;
}
```

- **단점:** 한 번 배열 크기를 `[5]`로 정하면 실행 중에 `[6]`, `[7]`로 세로 크기를 늘릴 수 없습니다.
    

### 2. 2차원 벡터 (`vector<vector<int>> v`) — 가장 추천!

세로 크기와 가로 크기 모두 실행 중에 마음대로 늘리고 줄일 수 있는 **진정한 동적 2차원 구조**입니다. C++에서 여러 개의 벡터를 다룰 때 가장 표준적이고 권장되는 방식입니다.


```cpp
#include <iostream>
#include <vector>

int main() {
    // 1. 완전히 비어있는 2차원 벡터 만들기
    vector<vector<int>> v1;

    // 2. 크기를 지정해서 만들기 (5행 0열)
    vector<vector<int>> v2(5);

    // 3. 크기와 초기값 지정해서 만들기 
    // (5행 3열을 모두 0으로 초기화)
    vector<vector<int>> v3(
	    5, vector<int>(3, 0)
    );

    // 사용법은 동일합니다.
    v3[0].push_back(99); 
    cout << v3[0][3] << endl; 
    // 기존 3개 뒤에 push_back 되었으므로 4번째 원소 출력
    
    return 0;
}
```

### 3. 고정 크기 배열의 현대적 버전 (`array<vector<int>, N>`)

1번 방식(`vector<int> v[5]`)은 C 스타일의 배열이라 가끔 포인터로 오해받거나 안전성 문제가 생길 수 있습니다. 세로 크기가 고정된 벡터 여러 개를 만들고 싶다면 C++ 표준 스타일인 `std::array`를 섞어 쓰는 것이 더 안전합니다.

```C++
#include <iostream>
#include <vector>
#include <array> // 필수

int main() {
    // 세로 크기 5로 고정된 벡터 배열
    std::array<std::vector<int>, 5> v;

    v[0].push_back(10);
    return 0;
}
```

### 💡 한 줄 요약

- **알고리즘 문제 풀 때 세로 크기가 정해져 있다면:** `vector<int> v[100];` (짜기 제일 편함)
    
- **세로 크기도 유연하게 늘려야 하거나 실무 코드를 짤 때:** `vector<vector<int>> v;` (가장 안전하고 강력함)


# iterator (vector)
### 기본 사용법 (기본 `for`문)

`begin()`으로 시작 위치를 얻고, `end()`를 만나기 전까지 반복자를 증가(`++it`)시키며 접근합니다. 반복자가 가리키는 실제 값을 얻을 때는 포인터처럼 역참조 연산자(`*`)를 사용합니다.

```C++
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {10, 20, 30, 40};

    // 데이터타입::iterator 변수명
    std::vector<int>::iterator it;

    for (it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << " "; // *it로 값을 가져옴
    }
    // 출력 결과: 10 20 30 40

    return 0;
}
```

> **중요:** `v.end()`는 마지막 요소가 아니라, **마지막 요소의 바로 다음 칸(빈 공간)**을 가리킵니다. 따라서 `*v.end()`를 출력하려고 하면 에러가 발생합니다.

### 2. `auto` 키워드로 스마트하게 쓰기 (권장)

`std::vector<int>::iterator`라고 길게 쓰는 것은 귀찮고 오타가 나기 쉽습니다. C++11부터는 `auto`를 사용하면 컴파일러가 알아서 데이터 타입을 추론해 주므로 코드가 훨씬 깔끔해집니다.


```C++
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {10, 20, 30, 40};

    // auto를 사용하면 매우 간결해집니다.
    for (auto it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << " ";
    }

    return 0;
}
```

### 3. 값을 수정할 수 없는 읽기 전용 반복자 (`cbegin`, `cend`)

만약 반복자를 돌면서 값만 읽고, 실수로라도 값을 수정하고 싶지 않다면 `const_iterator`를 의미하는 `cbegin()`과 `cend()`를 사용하는 것이 안전합니다.

```C++
for (auto it = v.cbegin(); it != v.cend(); ++it) {
    // *it = 50; // 에러 발생! (읽기 전용이기 때문)
    std::cout << *it << " ";
}
```

### 4. 실전 활용: `std::sort`나 알고리즘 함수와 함께 쓰기

반복자는 보통 C++ 표준 알고리즘 함수(`std::sort`, `std::find` 등)의 인자로 줄 때 가장 빛을 발합니다.

```C++
#include <iostream>
#include <vector>
#include <algorithm> // std::sort용

int main() {
    std::vector<int> v = {40, 10, 30, 20};

    // 처음부터 끝까지 정렬하라는 의미로 반복자를 넘겨줍니다.
    std::sort(v.begin(), v.end());

    return 0;
}
```

요약하자면, **`v.begin()`으로 시작해서 `!= v.end()` 일 때까지 `++it`로 전진하며, 값을 쓸 때는 `*it`를 사용한다!** 이것만 기억하시면 됩니다.


# to_string, stoi, etc.

\<string> 헤더

| 변환 방향                 | 사용하는 방법                 | 예시                           |
| --------------------- | ----------------------- | ---------------------------- |
| **숫자 → 숫자**           | `static_cast<바꿀타입>(변수)` | `static_cast<float>(an_int)` |
| **숫자 → 문자열**          | `std::to_string(숫자)`    | `to_string(10)` → `"10"`     |
| **문자열 → 정수(`int`)**   | `std::stoi(문자열)`        | `stoi("5")` → `5`            |
| **문자열 → 실수(`float`)** | `std::stof(문자열)`        | `stof("3.1")` → `3.1f`       |

# switch-case RV
## case문

```cpp
#include <iostream>

int main() {
    int number = 2;

    // switch(조건을 검사할 변수)
    switch (number) {
        case 1:
            std::string("1번을 선택하셨습니다.");
            break; // 중요! 실행을 마치고 switch 문을 빠져나갑니다.
        
        case 2:
            std::cout << "2번을 선택하셨습니다." << std::endl;
            break;

        case 3:
            std::cout << "3번을 선택하셨습니다." << std::endl;
            break;

        default: // 위 case 중 맞는 게 하나도 없을 때 실행 (if문의 else 역할)
            std::cout << "1, 2, 3 외의 다른 숫자입니다." << std::endl;
            break;
    }

    return 0;
}
```
# Array

```c++
#include <iostream>
#include <array> // <array> 헤더 필요

using namespace std;

int main() {
    // 선언: std::array<타입, 크기> 이름;
    array<int, 5> arr = {1, 2, 3, 4, 5};

    // 주요 기능
    cout << "크기: " << arr.size() << "\n";           // 1. 크기 구하기 (size())
    cout << "첫 번째: " << arr.front() << "\n";        // 2. 맨 앞 원소
    cout << "마지막: " << arr.back() << "\n";          // 3. 맨 뒤 원소

    // 안전한 접근 (.at())
    // arr.at(10); // 범위를 벗어나면 out_of_range 예외를 던져 안전함!

    // 전체 채우기
    arr.fill(0); // 모든 원소를 0으로 변경

    return 0;
}
```

# max

```c++
#include <iostream>
#include <algorithm> // std::max 헤더

using namespace std;

int main() {
    int a = 10;
    int b = 20;

    int result = max(a, b); // 더 큰 값인 20 반환
    cout << result << endl;

    return 0;
}
```

# sort

C++에서 `std::sort`는 `<algorithm>` 헤더에 있으며, **평균 $O(N \log N)$** 속도를 보장하는 가장 강력한 정렬 함수입니다.

기본 사용법부터 커스텀 정렬까지 패턴별로 정리해 드릴게요.

### 1. 기본 정렬 (오름차순 / 내림차순)

`sort(시작위치, 끝위치)` 형태로 사용하며, 기본값은 **오름차순**입니다.

(특정위치를 원하면 begin + i 처럼 포인터처럼써주면됨. iterator니까..)

(또, 기본 벡터정렬은 기본 숫자, 문자열 모두 잘 정렬해줌.)

!!! 내림차순 정렬 방법을 기억해주자.
(참고로 내림차할때쓰는 greater 인자는 비교 도구임. 함수객체로, 앞의 원소가 뒤보다 크면 자리를 바꾸라는 기준 알려주는 비교도구. 비교 기준이 정반대로 뒤집하는거임. 기본은 작은 게 먼저.)

```C++
#include <iostream>
#include <vector>
#include <algorithm> // sort 사용을 위해 필수!

using namespace std;

int main() {
    // 1. vector 오름차순 (작은 수 -> 큰 수)
    vector<int> v = {5, 2, 8, 1, 4};
    sort(v.begin(), v.end()); // {1, 2, 4, 5, 8}

    // 2. vector 내림차순 (큰 수 -> 작은 수)
    sort(v.begin(), v.end(), greater<int>()); // {8, 5, 4, 2, 1}

    // 3. C 스타일 기본 배열 정렬
    int arr[5] = {5, 2, 8, 1, 4};
    sort(arr, arr + 5); // 시작 주소, 끝 주소(포인터)

    return 0;
}
```

### 2. 커스텀 정렬 (조건이 복잡할 때)

비교 함수(Comparator)나 **람다(Lambda) 함수**를 세 번째 인자로 넘겨주면 원하는대로 정렬할 수 있습니다.

#### ① 좌표(`pair<int, int>`) 정렬

기본적으로 `pair`를 `sort` 돌리면 **first 기준 오름차순 ➡️ 같다면 second 기준 오름차순**으로 정렬됩니다. 만약 이 순서를 바꾸고 싶다면 람다 함수를 쓰면 됩니다.


```C++
vector<pair<int, int>> v = {{1, 4}, {3, 2}, {1, 2}};

// y좌표(second) 기준 오름차순, y가 같으면 x좌표(first) 기준 오름차순
sort(v.begin(), v.end(), [](const pair<int,int>& a, const pair<int,int>& b) {
    if (a.second == b.second) return a.first < b.first;
    return a.second < b.second;
});
// 결과: {1, 2} -> {3, 2} -> {1, 4}
```

a, b 인자로 잡았을 때 
a > b (앞의것이 뒤보다 크면 자리 바꾸라 == 내림차)
b > a (반대, 오름차.)

정렬 함수(cmp)에서 `true`를 리턴한다는 건 "야, `a`가 `b`보다 우선순위가 높으니까 `a`를 더 앞으로 보내줘!"라는 뜻!!!

#### ② 문자열 길이순 정렬 (길이가 같으면 사전순)


```C++
vector<string> words = {"apple", "cat", "banana", "dog"};

sort(words.begin(), words.end(), [](const string& a, const string& b) {
    if (a.length() != b.length()) {
        return a.length() < b.length(); 
    }
    return a < b; 
});
// 결과: "cat" -> "dog" -> "apple" -> "banana"
```

### 3. 구조체(struct) 정렬

구조체 멤버 변수가 여러 개일 때도 비교 함수만 깔끔하게 작성해 주면 됩니다.

```C++
struct Student {
    string name;
    int score;
    int age;
};

vector<Student> students = {{"Kim", 90, 20}, {"Lee", 90, 18}, {"Park", 100, 19}};

// 점수 높은 순(내림차순) ➡️ 점수 같으면 나이 어린 순(오름차순)
sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
    if (a.score != b.score) return a.score > b.score;
    return a.age < b.age;
});
```

### 🚨 커스텀 정렬 작성 시 주의사항 (`Strict Weak Ordering`)

비교 연산 조건을 만들 때 **두 값이 같으면 무조건 `false`를 반환**해야 합니다. (등호 `=` 절대 금지!)

- ⭕ `return a.score < b.score;`
    
- ❌ `return a.score <= b.score;` ➡️ **런타임 에러(Segmentation fault) 발생 원인!**






# swap


```C++
#include <iostream>
#include <algorithm> // swap이 들어있는 헤더

using namespace std;

int main() {
    int a = 10;
    int b = 20;

    cout << "전: a = " << a << ", b = " << b << "\n";

    // a와 b의 값을 서로 맞바꿈!
    swap(a, b);

    cout << "후: a = " << a << ", b = " << b << "\n";
    // 출력 결과: a = 20, b = 10
}
```





# malloc, calloc (c style)

C++에서는 메모리 동적 할당에 주로 `new`를 쓰지만, C 스타일의 `malloc`과 `calloc`은 `<cstdlib>` (또는 `<stdlib.h>`) 헤더를 포함한 뒤 다음과 같이 사용할 수 있습니다.

### 0. realloc 

```C++

#include <cstdlib>

// 1. 최초 5개 할당
int* arr = (int*) malloc(5 * sizeof(int));

// 2. 10개 크기로 재할당 (늘리기)
int* temp = (int*) realloc(arr, 10 * sizeof(int));

if (temp != NULL) {
    arr = temp; // 재할당 성공 시 주소 업데이트
} else {
    // 메모리 부족 등으로 재할당 실패 처리
}

// 3. 해제
free(arr);
```

### 💡 `realloc` 작동 방식

1. **제자리 확장:** 기존 메모리 뒤쪽에 연속된 공간이 남아있으면, **그 자리 그대로** 크기만 늘려줍니다.
    
2. **이동 확장:** 뒤쪽 공간이 부족하면 **다른 빈 공간에 새로 메모리를 크게 잡고, 기존 데이터를 복사**한 뒤 이전 메모리는 알아서 해제해 줍니다.
3. 

### 1. `malloc` (Memory Allocation)

- **특징:** 지정한 바이트 크기만큼 메모리를 할당합니다. **초기화되지 않아서 쓰레기값이 들어있습니다.**
    
- **문법:** `(타입*) malloc(개수 * sizeof(타입))`
    
```C++

#include <cstdlib> // 또는 <stdlib.h>

// int 5개짜리 동적 배열 할당
int* arr = (int*) malloc(5 * sizeof(int));

// 사용 후 반드시 free로 해제
free(arr);
```

### 2. `calloc` (Clear Allocation)

- **특징:** 메모리를 할당함과 동시에 **모든 비트를 0으로 초기화**합니다.
    
- **문법:** `(타입*) calloc(개수, sizeof(타입))` _(콤마로 개수와 크기를 전달)_
    

```C++
#include <cstdlib>

// int 5개짜리 동적 배열 할당 + 0으로 초기화
int* arr = (int*) calloc(5, sizeof(int));

// 사용 후 반드시 free로 해제
free(arr);
```

### 💡 주의 및 요약

1. C++에서는 반환되는 `void*` 타입을 목적 타입에 맞게 반드시 명시적 형변환 `(int*)`을 해주어야 합니다.
    
2. `malloc`과 `calloc`으로 할당한 메모리는 반드시 `free()`로 해제해야 메모리 누수가 나지 않습니다.
    
3. **C++ 실무/PS 팁:** C++에서는 단순 기본형 데이터가 아니면 생성자/소멸자를 호출해 주는 `new` / `delete`를 쓰거나, 가급적 `std::vector`를 쓰는 것이 메모리 관리 면에서 훨씬 안전하고 편리합니다.





# array : new, fixed

### 1. `new`를 이용한 배열 동적 할당

동적 할당은 프로그램 실행 중(Runtime)에 크기를 결정할 때 사용합니다. 메모리의 **힙(Heap) 영역**에 할당되므로 사용이 끝난 뒤 **반드시 `delete[]`로 해제**해 주어야 메모리 누수(Memory Leak)가 발생하지 않습니다.

```C++
#include <iostream>

int main() {
    int size = 5; // 변수를 통해 크기 지정 가능

    // 1. 메모리 할당
    int* arr = new int[size]; 

    // 2. 초기화와 동시에 할당하고 싶은 경우 (C++11 이상)
    // int* arr = new int[size]{ 1, 2, 3, 4, 5 }; // 1, 2, 3, 4, 5로 초기화
    // int* arr = new int[size]{};               // 모두 0으로 초기화

    // 값 대입 및 사용
    for (int i = 0; i < size; ++i) {
        arr[i] = (i + 1) * 10;
    }

    // 출력
    for (int i = 0; i < size; ++i) {
        std::cout << arr[i] << " "; // 10 20 30 40 50
    }

    // 3. 메모리 해제 (배열은 반드시 delete[] 사용!)
    delete[] arr;

    return 0;
}
```

### 2. 크기가 고정된 배열 선언법

크기가 고정된 배열이라면 굳이 `new`를 쓸 필요 없이 **정적 배열**을 선언하는 것이 훨씬 간단하고 안전합니다. 정적 배열은 **스택(Stack) 영역**에 할당되며 함수가 끝날 때 자동으로 메모리가 해제됩니다.

#### 방법 A: C 스타일 일반 배열

```C++
int arr[5];               // 쓰레기값 들어있음
int arr2[5] = {1, 2, 3};  // {1, 2, 3, 0, 0} 로 초기화
int arr3[5] = {};         // 모든 요소 0으로 초기화
```

_주의:_ `int arr[n];` 처럼 크기 자리에 변수를 넣는 것은 표준 C++에서 허용되지 않습니다 (크기는 컴파일 시점에 결정되는 상수여야 함).

#### 방법 B: 현대 C++ 표준 배열 (`std::array`)

고정 크기 배열을 쓸 때는 `<array>` 헤더의 `std::array`를 사용하는 것이 권장됩니다. 일반 배열처럼 가볍고 안전한 기능(크기 조회 등)을 제공합니다.

```C++
#include <array>

std::array<int, 5> arr = {1, 2, 3, 4, 5};
std::cout << arr.size(); // 5
```

### 요약 Guide

|**구분**|**선언 방식**|**특징**|
|---|---|---|
|**고정 크기 (추천)**|`std::array<int, 5> arr;`|크기 고정, 자동 메모리 관리, C++ 표준 권장|
|**고정 크기 (기본)**|`int arr[5];`|가장 기본적인 형태|
|**동적 크기 (`new`)**|`int* arr = new int[n];`|실행 중 크기 변경 지정 가능, **`delete[]` 필수**|
|**동적 크기 (실무 추천)**|`std::vector<int> arr(n);`|크기 가변, **자동 메모리 해제**, 가장 편리함|

> **Tip:** C++ 실무나 현대적인 코드에서는 메모리를 직접 관리하는 `new`/`delete[]` 대신 **`std::vector`**를 훨씬 많이 사용합니다.




# pair

(헤더: `#include <utility>` 또는 `#include <vector>`, `#include <queue>` 등에 포함됨)

### 1. 선언 및 초기화

```C++
#include <utility>

pair<int, int> p1;                  // 기본 선언 (쓰레기값)
pair<int, int> p2 = {10, 20};       // 값 초기화
pair<int, int> p3 = make_pair(10, 20); // 함수로 초기화
```

### 2. 멤버 변수 접근

- **`first`**: 첫 번째 값
- **`second`**: 두 번째 값

```C++
cout << p2.first;  // 출력: 10
cout << p2.second; // 출력: 20
```

### 3. 큐(`queue`)나 벡터(`vector`)와 조합할 때


```C++
queue<pair<int, int>> q;

// 데이터 넣기
q.push({priority, location}); // 또는 make_pair(priority, location)

// 데이터 조회 및 사용
int current_priority = q.front().first;
int current_location = q.front().second;

q.pop();
```

# do {} while () ;
#