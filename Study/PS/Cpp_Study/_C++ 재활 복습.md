
# 대소문자 변환
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

|**함수**|**설명**|
|---|---|
|`str.append(s)`|문자열 `s`를 뒤에 붙입니다. (`+=` 연산자로 대체 가능)|
|`str.substr(pos, len)`|`pos` 위치에서 `len` 길이만큼의 부분 문자열을 반환합니다.|
|`str.replace(pos, len, s)`|`pos` 위치부터 `len`만큼을 `s`로 교체합니다.|
|`str.insert(pos, s)`|`pos` 위치에 `s`를 삽입합니다.|
|`str.erase(pos, len)`|`pos` 위치부터 `len`만큼 삭제합니다.|

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

# 
