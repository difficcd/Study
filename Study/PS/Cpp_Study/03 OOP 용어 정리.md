# 용어 정리 (Python)

OOP 핵심 용어를 Python 예시로 먼저 정리. (개념 참고: [boycoding 블로그](https://boycoding.tistory.com/135))

- **클래스**(설계도) : 사용자 정의 자료형과 유사. **기능 + 변수**로 객체의 공통 특성을 서술
    - 구조체 = 변수 모음 / 클래스 = 변수 + 기타 요소(함수 등) 추가
- **객체**(실사용 변수) : 클래스 형으로 만들어진 값을 가지는 실체 = 그 클래스의 **인스턴스**
    - 자료 + 처리 명령을 묶은 **메서드**로 구성 (함수보다 높은 수준의 모듈화)
    - 하나의 클래스로 여러 객체 생성 가능
- **메서드** : 객체가 수행하는 동작을 기술
- **생성자** : 객체 초기화에 쓰는 메서드로 값을 배정 (C++ 에는 소멸자도 존재)

```python
class Car:                 # Car 클래스(특성 블록) 생성
    def drive(self):       # def name : 메서드 (객체가 뭘 하는지)
        print("the car is driving")

car1 = Car()               # 인스턴스 생성 : 객체이름 = 클래스이름()
car2 = Car()

car1.drive()               # 메서드 호출 : 객체이름.메서드이름()  → the car is driving
car2.drive()               # → the car is driving
```

> [!note] 자료형 자동 일치
> - **C++** : 제네릭 프로그래밍(STL 라이브러리)으로 자료형 자동 일치
> - **Python** : 언어 자체적으로 자료형 자동 일치
> - → 형 변환 같은 번거로운 요소를 줄일 수 있음

---

# 용어 정리 (C++)

- **클래스** = 객체를 정의하는 틀 (변수 + 프로시저(함수)) — 특성: **캡슐화 · 상속 · 다형성**
- **객체** = 클래스 형으로 만들어진 실사용 변수
- **멤버 변수** = 객체 내부의 변수 (내부 정보 저장)
- **멤버 함수** = 객체 내부의 프로시저 (객체가 할 수 있는 행위)
- **생성자** = 객체 초기화에 쓰는 멤버 함수
- C++ 에서는 `<iostream>` 이 기본 입출력 라이브러리 (`std::cout << "내용\n";` — 자료형 자동 조정)

> [!tip] 핵심 키워드
> 클래스 · 객체 · 멤버 변수 · 멤버 함수 · 생성자 / 캡슐화 · 상속 · 다형성 · 제네릭 프로그래밍

아래 코드 하나에 OOP 4대 개념이 모두 들어있음 — **캡슐화(private) · 상속 · 다형성(virtual/override) · 제네릭(template)**:

```cpp
#include <iostream>
#include <vector>
using namespace std;

template <typename T>              // 제네릭(템플릿): T 를 다양한 타입으로
class Animal {
private:
    T age;                         // 멤버 변수 (캡슐화: private)
public:
    Animal(T a) : age(a) {}        // 생성자
    virtual void sound() {         // 다형성: 가상 함수
        cout << "Animal sound" << endl;
    }
    T getAge() { return age; }
};

class Dog : public Animal<int> {   // 상속 (부모 Animal, 자식 Dog)
public:
    Dog(int a) : Animal(a) {}
    void sound() override {        // 다형성: 오버라이딩
        cout << "Woof!" << endl;
    }
};

int main() {
    Dog myDog(3);                  // 객체 생성
    cout << "Dog's age: " << myDog.getAge() << endl;   // 3
    myDog.sound();                                     // Woof!
    return 0;
}
```

- 제네릭 프로그래밍은 [[00 C++ 기초 문법#함수 템플릿]] 참조

```cpp
template <class T>
T max(T a, T b) { return (a > b ? a : b); }
// main 에서 max(int, int), max(double, double) 모두 사용 가능
```
