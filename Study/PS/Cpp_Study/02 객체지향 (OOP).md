# 객체의 생성

객체를 **배열·포인터로 다루기**, `new`/`delete` 로 **동적 생성·소멸**하기, 그리고 **this 포인터**까지.

> [!warning] 인자 있는 생성자만 선언하면 배열 생성 시 오류
> `Circle(int r)` 같은 생성자만 선언해 두면 기본 생성자(인자 X)가 자동 생성되지 않는다.
> 그래서 `Circle arr[]` 처럼 클래스 배열을 쓸 때 인자 없는 생성자가 없으면 **컴파일 오류**.

---

## 1. 객체 배열

객체 1차원 배열 + 포인터로 지시하기 / 객체 2차원 배열 사용하기.

```cpp
#include <iostream>
using namespace std;

//class
class Circle{
private:
  int radius;
public:
  Circle();
  Circle(int r);
  double getArea(){
    return 3*radius*radius;
  }
};

Circle::Circle(int r){
  radius = r;
}

Circle::Circle(){
  radius = 5;
}

Circle circlearr[2] = {Circle(10), Circle()};
Circle *p; // p의 변경은 main 에서만 가능 (전역=선언공간)

Circle circles[2][2] = {{Circle(1),Circle(2)}, {Circle(3),Circle(4)}};

//main
int main(){

  p = circlearr;

  cout << circlearr[0].getArea() << endl; // 3*10*10
  cout << circlearr[1].getArea() << endl; //3*5*5

  cout << p->getArea() << endl; //[0] 접근
  p++;
  cout << p->getArea() << endl; //[1] 접근

  //2d arr
  cout << circles[0][0].getArea(); // 3*1*1

}

```

## 2. 객체 포인터

객체 포인터의 선언과 접근 방법. (`p->getArea()` 와 `(*p).getArea()` 는 동일)

```cpp
#include <iostream>
using namespace std;

//class
class Circle{
private:
  int radius;
public:
  Circle();
  Circle(int r);
  double getArea(){
    return 3*radius*radius;
  }
};

Circle::Circle(int r){
  radius = r;
}

Circle::Circle(){
  radius = 5;
}

Circle circlearr[2] = {Circle(10), Circle()};
Circle donut; // 일반 객체
Circle *p; // p의 변경은 main 에서만 가능 (전역=선언공간)

double HEREPOINT = donut.getArea(); // 일반 객체 getArea()값

//main
int main(){

  p = circlearr; // 포인터명 = 배열이름
  // 기존에 배웠던 배열이름과 포인터의 관계 동일

  cout << circlearr[0].getArea() << endl; // 3*10*10
  cout << circlearr[1].getArea() << endl; //3*5*5

  cout << p->getArea() << endl; //[0] 접근
  p++;
  cout << p->getArea() << endl; //[1] 접근

  Circle *pt; // 객체 포인터 선언은 main에서도 가능
  pt = &donut;
  HEREPOINT = pt->getArea(); // 객포->함수

  cout << HEREPOINT << endl; // donut 매개X이므로 [1]과 동일.
  cout << (*pt).getArea() << endl; // 위와 동일한 호출법

}

```

---

## 3. 동적 할당 / 반환

- `new` / `delete` 로 동적 할당·반환. 정적 변수와 달리 **실행 중에 OS 에게 할당**받음
- **메모리 누수 주의**
    - `char* p = new char[1024];` 를 `p = &n;` 처럼 바꾸면 → 기존 1024 공간을 못 쓰게 됨 = 누수
    - `char* p` 를 여러 번 `new` 하면 → 할당할 때마다 공간이 생기고 delete 안 되어 누수

> [!warning] new / delete 잡다한 규칙 (#중요)
> - `new`↔`delete`, `new[]`↔`delete[]` **짝 맞춰** 사용 (선언 형식대로)
> - NULL 포인터는 delete 해도 괜찮음 (아무 일도 안 함)
> - 동적 할당 안 받은 포인터라도 `p = NULL` 이면 `delete p` 가능
> - 동적 할당 안 했거나 이미 delete 한 걸 또 delete → **런타임 오류**
> - 배열로 할당한 포인터를 `[]` 없이 그냥 `delete` 하면 비정상 반환

```cpp
#include <iostream>
using namespace std;

//main
int main(){
  int *pt = new int(0); // 타입 포인터이름 = new 타입
  // (0)은 초기값으로, 0으로 초기화된 int 타입 뜻함
  // 배열은 초기화 불가

  int size;  int sum = 0;
  cin >> size;

  int *p = new int[size];
  // size 이후 선언되는 배열형식 포인터변수

  if(!p || !pt) return 0; // 메모리할당 여부 ck

  for(int i=0; i<size; i++){
    cin >> pt[i];
    cin >> p[i];
  }

  for(int i=0; i<size; i++){
    cout << pt[i] << " "; // 1 2 3 출력 (3 11 22 33 입력)
    sum += p[i]; // sum이니 6 출력
  }
  cout << "\n" << sum << endl;

  delete pt; // delete 포인터이름
  pt = NULL;
  // 포인터변수가 없어지는건 아니므로, NULL 지정이 안전

}

```

---

## 4. 객체 / 객체 배열 동적 생성 · 반환

- 객체·객체 배열을 포인터 + `new` 로 생성, `delete` 로 소멸
- **소멸자의 중요성** : delete 를 소멸자 기능으로 넣어주면 정리가 깔끔해짐
- 학습 포인트 : ① 생성자·소멸자가 동적 생성에 어떻게 관여하는지, ② 포인터로 객체·객체 배열에 어떻게 접근하는지
- 동적 배열 객체의 매개변수 생성자 : 교재 35p RV

```cpp
#include <iostream>
using namespace std;

//class
class Circle{
private:
  int radius;
public:
  Circle() { radius = 5; cout << "생성자 r = " << radius << endl; }
  Circle(int r) { radius = r; cout << "생성자 r = " << radius << endl; }
  ~Circle();

  void setRadius(int r)  {radius = r;}
  double getArea()  { return 3*radius*radius;}
};

Circle::~Circle(){
  cout << "소멸자 r = " << radius << endl;
}

Circle *PTHERE = new Circle(4); // 객체 동적 생성, 4는 생성자 매개변수
Circle *PArrHERE = new Circle[4]; // 객체 배열 동적 생성, 4는 배열크기
// 기본 생성자 무조건 필요 >> 동적 객체배열은 따로 매개변수지정 못해줌

// new Circle[4] 의 경우 객체마다 디폴트생성자 실행 (총 4회)

//main
int main(){

  Circle *QTHERE;
  QTHERE = new Circle; // 기본 생성자 : radius 5할당

  cout << QTHERE->getArea() << endl; // radius 5 * 5 *3
  cout << PTHERE->getArea() << endl; // radius 4 * 4 *3 >> 생성매개4

  delete QTHERE;
  delete PTHERE;

  // 동적 객체배열
  PArrHERE->setRadius(20);
  PArrHERE[1].setRadius(10);

  cout << PArrHERE->getArea() << endl; // 20 * 20 * 3
  cout << (PArrHERE+1)->getArea() << endl; // 10 * 10 * 3

  delete [] PArrHERE; // []로 선언되었으니, delete 도 []로.
  // 소멸자 객체마다실행 :: 총 4번 실행, 5 5 10 20 (배열 역순)

}

```

---

## 5. 멤버 함수의 this 포인터

- **this** = 그 멤버 함수를 소유한 **객체 자신을 가리키는 포인터**
- 클래스 멤버 함수 **안에서만** 사용. 컴파일러가 묵시적으로 삽입·선언한 변수
- **제약**
    - 멤버 함수가 아닌 함수에서는 사용 불가 (정의상 당연)
    - **static 멤버 함수에서도 불가** — 객체 생기기 전에 static 함수가 호출될 수 있으므로 (`클래스명::static함수` 로 객체 없이 호출 가능)

> [!note] this 는 컴파일러가 몰래 넘기는 매개변수
> 개발자가 `void setA(int x) { this->a = x; }` 를 작성하면,
> 컴파일러는 `void setA(Sample* this, int x) {...}` 로 변환한다.
> → `Sample ob; ob.setA(5);` 는 내부적으로 `setA(&ob, 5)` 로 처리 (객체 주소가 this 로 전달).

```cpp
#include <iostream>
using namespace std;

//class
class Circle{
private:
  int radius;
public:
  Circle() { radius = 5; }
  Circle(int r) { radius = r; }
  ~Circle();

  void setRadius(int radius)  { this->radius = radius; }
  //this 포인터 사용
  // 매개변수 이름과 바꿀 변수(멤버변수) 이름이 같을 때 사용가능!

  Circle* getAdd() {return this;}
  // 객체타입=클래스이름 + 리턴 포인터*
  // 객체 자기 자신의 주소를 리턴할 수 있어, 간편하다. &객체와 동일한 주소 반환

  double getArea()  { return 3*radius*radius;}
};

Circle::~Circle(){
  cout << "소멸자 r = " << radius << endl;
}

//main
int main(){
  Circle c1, c2;

  c1.setRadius(4); // 여기서 this 로 radius 값 갱신
  c2.setRadius(10);
  cout << c1.getArea() << "   주소 : " << &c1 << endl ; // 4 * 4 * 3
  cout << c2.getArea() << "   주소 : " << &c2 << endl ; // 10 * 10 * 3
  cout << c2.getAdd() << endl ; // c2 this 포인터 = &c2 동일하고, c1과 상이

  // &c1 은 c1객체의 this 포인터, &c2 는 c2객체의 this포인터와 같다
  // 두 객체의 this 포인터는 서로 다름을 알 수 있다
}

```

> 더 깊은 이해는 아래 **실습 코드 3번** 참조.

> [!example]- 실습 코드 (펼쳐보기)
> **1번**
>
> ```cpp
> #include <iostream>
> using namespace std;
>
> //class
> class Circle{
>   int radius;
> public:
>   Circle(int r=0) { radius = r; } // 입력 생성자 (소멸자는 default..)
>
>   void setRadius(int r)  { radius = r; }
>   double getArea()  { return 3.14*radius*radius;} // 원주율 3.14 가정
>
> };
>
> //main
> int main(){
>
>   Circle carr[3]; // 우선 디폴트 생성자로 생성
>   double C[3];
>   int count=0;
>
>   for (int i=0; i<3; i++)
>     {
>       cout << "원" << i+1 << "의 반지름 >> ";
>       cin >> C[i];
>       carr[i].setRadius(C[i]);
>     }
>
>   for (int i=0; i<3; i++)
>       if(carr[i].getArea() > 100 )
>         count++;
>
>   cout << "면적이 100보다 큰 원 " << count << "개입니다." << endl;
>
> }
>
> ```
>
> **2번**
>
> ```cpp
> #include <iostream>
> using namespace std;
>
>  class Color
> {
>  int red, green, blue;
>  public:
>    Color() { red = green = blue = 0;}
>    Color(int r, int g, int b)
>    { red = r; green = g; blue = b; }
>
>    void setColor(int r, int g, int b)
>    { red = r; green = g; blue = b; }
>    void show()
>    { cout<< red << ' '<< green << ' ' << blue << endl; }
> };
>
> //main
> int main(){
>
>   Color screenColor(255, 0, 0); // 빨간색의screenColor객체생성
>   Color *p; // Color 타입의포인터변수p 선언
>
>   p = &screenColor; // (1) p가screenColor의주소를가지도록코드작성
>   p->show();  // (2) p와show()를이용하여screenColor색출력
>   Color colors[3]; // (3) Color의일차원배열colors 선언. 원소는3개
>   p = colors; // (4) p가colors 배열을가리키도록코드작성
>
>   // (5) p와setColor()를이용하여colors[0], colors[1], colors[2]가
>   // 각각빨강, 초록, 파랑색을가지도록코드작성
>   p->setColor(255, 0, 0);
>   (p+1)->setColor(0, 255, 0);
>   (p+2)->setColor(0, 0, 255);
>   // (6) p와show()를이용하여colors 배열의모든객체의색출력. for 문이용
> for (int i=0; i<3; i++)
>     (p+i)->show();
> }
>
> ```
>
> **3번**
>
> ```cpp
> #include <iostream>
> using namespace std;
>
> class Person
> {
>     string name;
> public:
>     Person() { name = ""; }
>     Person(string name) { this->name = name; }
>     string getName() { return name; }
>     void setName(string name) { this->name = name; }
> };
>
> class Family {
>     string name;
>     Person* p; // Person 배열포인터
>     int size; // Person 배열의크기. 가족구성원수
> public:
>     Family(string name, int size); // size 개수만큼Person 배열동적생성
>     void setName(int index, string name);
>     void show(); // 모든가족구성원출력
>     ~Family();
> };
>
> Family::Family(string name, int size) {
>     this->name = name;
>     this->size = size;
>     p = new Person[size];
> }
>
> void Family::setName(int index, string name) {
>     this->p[index].setName(name);
> }
>
> void Family::show() {
>     cout << name << "가족은 다음과 같이 " << size << "명입니다." << endl;
>     for (int i = 0; i < size; i++)
>         cout << p[i].getName() << "  ";
> }
>
> Family::~Family() {
>     delete[] p;
> }
>
> //main
> int main() {
>     Family* simpson = new Family("Simpson", 3); // 3명으로 구성된 Simpson 가족
>     simpson->setName(0, "Mr. Simpson");
>     simpson->setName(1, "Mrs. Simpson");
>     simpson->setName(2, "Bart Simpson");
>     simpson->show();
>     delete simpson;
> }
>
> ```

---

# 함수와 참조, 복사 생성자

객체를 함수로 주고받는 방법과, **동적 메모리를 가진 객체를 복사할 때의 함정**(얕은 복사)을 다룬다. 이 챕터의 핵심은 **깊은 복사 생성자**.

## 1. 객체 전달과 참조

객체를 함수에 넘기는 4가지 방식 — 값 / 포인터 / 대입·리턴 / 참조.

### (1) call by value — 객체를 그대로 복사

- 매개변수 객체가 **복사·공간 할당**되지만 **생성자는 호출 X** (상태 그대로 전달하려고)
- 단, 함수 종료 시 **소멸자는 호출 O**
- 호출하는 쪽은 객체 이름만 넘김

```cpp
#include <iostream>
using namespace std;

class Circle {
private:
  int radius;

public:
  Circle(int r) {
    radius = r;
    cout << "생성 ";
  }
  ~Circle() { cout << "소멸 \n"; }
  int getRadius() { return radius; }
  void setRadius(int r) { radius = r; }
};

void increase(Circle c) {
  int r = c.getRadius();
  c.setRadius(r + 1);
}

int main() {
  Circle waffle(30);    //생성 소멸 (기본 객체의 동작)
  increase(waffle);     // 객체 매개변수, 소멸만 뜸 (생성자x)
  cout << waffle.getRadius() << " "; // 30
}
```

### (2) call by reference — 객체 포인터를 전달

- 객체 **주소만 전달** (매개변수가 객체 포인터) → 매개변수의 생성·소멸 **둘 다 없음**
- 함수 종료 시 포인터만 소멸 (포인터 소멸 ≠ 객체 소멸자)

```cpp
#include <iostream>
using namespace std;

class Circle { // () 실수 조심
private:
  int radius;

public:
  Circle(int r) {
    radius = r;
    cout << "생성 ";
  }
  ~Circle() { cout << "소멸 \n"; }
  int getRadius() { return radius; }
  void setRadius(int r) { radius = r; }
};

void increase(Circle *c) {
  int r = c->getRadius(); // -> 로 접근
  c->setRadius(r + 1);
}

int main() {
  Circle waffle(30);  //생성 소멸
  Circle *p;
  p = &waffle; // 객체 배열과 헷갈리지 X

  increase(p);   // 객체 매개변수가 객체포인터, 생성소멸 둘다 x
  cout << waffle.getRadius() << " "; // 31
}
```

### (3) 객체 대입 · 객체 리턴

- **객체 대입** : 모든 데이터가 **비트 단위 복사**. 일반 대입과 조건 동일 (타입 일치·독립)
- 객체 리턴 시에도 복사본이 만들어짐

```cpp
#include <iostream>
using namespace std;

class Circle {
private:
  int radius;

public:
  Circle() {radius = 0; cout << "생성";}
  Circle(int r) {radius = r; cout << "생성 ";}
  ~Circle() { cout << "소멸 \n"; }
  int getRadius() { return radius; }
  void setRadius(int r) { radius = r; }
};

Circle getCircle() {
  Circle tmp(50); // 생성, 소멸 1set
  return tmp; // 객체 tmp 리턴 (복사본!)
} // return 값이 Circle 인 것이고 멤버함수 아니니 주의

int main() {
  Circle c1(30), c2(10); // 생성 * 2

  c2 = c1;   //c2의 radius  30으로 갱신

  cout << c2.getRadius() << " \n"; // bit단위복사  30 출력
  cout << getCircle().getRadius() << " \n"; // tmp 복사, 50 출력

  Circle c; c = getCircle(); cout << c.getRadius() << " ";
  // tmp 객체가 c에 비트단위 복사되며 값 할당 (50)

}
```

### (4) 객체 참조 — 참조 매개변수 &

- 참조 매개변수(`&`)로 객체 전달. 일반 `&` 사용법과 같지만 **객체가 별명으로 넘어감**
- 참조는 생성·소멸이 없음 (생성/소멸 1회씩만)

```cpp
#include <iostream>
using namespace std;

class Circle {
private:
  int radius;

public:
  Circle() {radius = 0; cout << "생성 ";}
  Circle(int r) {radius = r; cout << "생성 ";}
  ~Circle() { cout << "소멸 \n"; }
  int getRadius() { return radius; }
  void setRadius(int r) { radius = r; }
};

void increaseCircle(Circle &c){ // 참조 매개변수 c
  int r = c.getRadius();
  c.setRadius(r+1);
}

int main() {
  Circle c;

  Circle &ref = c;  // c 객체 참조변수 ref 선언
  ref.setRadius(10);  // 참조로 값 변환
  cout << ref.getRadius() << " "; // 10 출력

  increaseCircle(c); // c를 참조 매개변수로 값갱신
  cout << c.getRadius() << endl;  // 11 출력

  // 생성, 소멸은 1회씩만. (참조 : 생성소멸 x)

}

```

---

## 2. 복사 생성자

### (1) 얕은 복사 vs 깊은 복사

객체 복사 시 멤버를 1:1 복사하는데, **동적 메모리를 가진 멤버**가 문제.

| 방식 | 동적 메모리 멤버 처리 |
|------|----------------------|
| **얕은 복사** | 주소만 복사 → 사본·원본이 **같은 메모리 공유** (위험) |
| **깊은 복사** | 사본이 **별도로 동적 할당** 후 내용만 복사 (안전) |

→ **완전한 복사 = 깊은 복사** (메모리 공유가 없어야 함)

### (2) 객체 간 초기화 vs 대입

- **초기화 = 복사 생성자** : 다른 객체와 같은 값 갖도록 멤버를 1:1 초기화
- **대입 = 대입 연산자** : 다른 객체의 값을 1:1 대입

### (3) 복사 생성자란

- 객체를 **복사 생성**할 때 호출되는 특별 생성자 (같은 클래스 객체로 초기화)
- 형태 : `클래스명(클래스명& 참조매개변수)` → ex) `Circle(Circle& c) {...}`
- 한 클래스에 **딱 하나**, 클래스 참조 매개변수를 갖는 독특한 생성자
- `객체(다른 객체)` 형식 → 공간 할당 후 복사 생성자 실행
- **복사 생성자 선언 없이** `c1(c2)` 로 생성하면? → 컴파일러가 **디폴트 복사 생성자** 자동 삽입 (`this->radius = c.radius;`, 매개변수 여러 개면 각각)

```cpp
#include <iostream>
using namespace std;

class Circle {
private:
  int radius;
public:
  Circle(int r) {radius = r; cout << "생성 ";}
  Circle(Circle& c) {this->radius = c.radius; cout << "복사 ";}
  // 복사 생성자 Circle& c의 참조 매개변수 꼴로 객체 초기화...

  ~Circle() { cout << "소멸 \n"; }
  int getRadius() { return radius; }
  void setRadius(int r) { radius = r; }
};

void increaseCircle(Circle &c){ // 참조 매개변수 c
  int r = c.getRadius();
  c.setRadius(r+1);
}

int main() {
  Circle c1(10), c2(c1);
  cout << c2.getRadius() << endl; // 생성 복사 10 소멸 소멸
}
```

### (4) 얕은 복사 비정상 종료 사례

> [!danger] 동적 메모리 멤버 + 디폴트 복사 = 이중 delete 크래시
> 디폴트 복사 생성자는 `b.data = a.data;` 처럼 **주소만 복사**한다.
> → 두 객체가 같은 힙을 가리키고, 소멸자에서 **같은 메모리를 두 번 delete** → 비정상 종료.
> **동적 할당 멤버를 가진 객체는 반드시 깊은 복사 생성자를 직접 써야** 해결된다.

**① 간단한 예시**

```cpp
#include <iostream>
using namespace std;

class Sample {
public:
    int* data;
    Sample(int value) { data = new int(value); } //data 동적할당
    ~Sample() { delete data; } // 동적할당 해제
};

int main() {
    Sample a(10); // 힙에 할당을 해버림!
    Sample b(a); // 힙에 할당했는데 얘도 같은곳에 할당을하려고 시도를 함 >> 주소 충돌

    // *디폴트 복사 생성자는 b.data = a.data; 처럼 작동

    cout << *a.data << "  " << *b.data << endl;

    return 0;
    // 소멸자 : b.data랑 a.data랑 같은데이터 위치, 같은 메모리 두번 delete 된다.
}
```

**② 강의자료 예시 — Person (얕은 복사로 크래시)**

```cpp
#include <iostream>
#include <cstring>
using namespace std;

class Person {
    char* name;
    int id;
public:
    Person(int id, char* name);
    ~Person();
    void changeName(char* name);
    void show() { cout << id << ',' << name << endl; }
};

Person::Person(int id, char* name) {
    this->id = id;
    int len = strlen(name);
    this->name = new char[len + 1];
    strcpy(this->name, name);
}

Person::~Person() {
    if (name) // 만일 name에 동적 할당된 배열이 있으면
        delete[] name; // 동적 할당 메모리 소멸
}

void Person::changeName(char* name) { // 이름 변경
    if (strlen(name) > strlen(this->name))
        return;
    strcpy(this->name, name);
}

/*복사 생성자 정의 x > 디폴트 복사 생성자
this->id = father.id;
this->name = father.name;  // 얕은 복사! 포인터 주소만 복사됨
따라서 같은 주소 가리키게 되고, 이중 delete 문제가 발생함

=>id 는 괜찮지만, name 이 문제인 게 동적할당을 해버리면 걔는 힙을 먹는거임
근데 그 name 을 포함한 객체를 복사하려는데 동적할당 된 name같은 게 있다?
그러면 디폴트로는 해결이 안 됨. 같은 힙을 참조하려고 하니까 (this->name = father.name)
즉 !!! 동적할당이 필요한 멤버변수 가지는 객체를 복사하려면 참조 변수를 무조건 써야지 해결이 되는 것!!!
*/

int main() {
    Person father(1, "Kitae");
    Person daughter(father);
    //father 복사한 객체 생성 >> char*name 동적할당 얕은복사(메모리공유)
    //정의를 보면 복사 생성자가 없음.

    cout << "daughter 객체 생성 직후----" << endl;
    father.show();
    daughter.show();

    daughter.changeName("Grace");
    cout << "daughter 이름을 Grace로 변경한 후----" << endl;
    father.show();
    daughter.show();

    return 0; // daughter, father 객체 소멸자 : 두번 delete
    // father 객체 소멸할때 프로그램 비정상 종료
}

```

**③ 깊은 복사 생성자로 수정 (정상 동작)**

```cpp
#include <iostream>
#include <cstring>
using namespace std;

class Person {
    char* name;
    int id;
public:
    Person(int id, const char* name);         // 일반 생성자
    Person(const Person& person);             // 복사 생성자 (깊은 복사)
    ~Person();                                // 소멸자
    void changeName(const char* name);
    void show() { cout << id << ',' << name << endl; }
};

// 일반 생성자
Person::Person(int id, const char* name) {
    this->id = id;
    int len = strlen(name);
    this->name = new char[len + 1];
    strcpy(this->name, name);
}

// 복사 생성자 (깊은 복사)
Person::Person(const Person& person) {
    this->id = person.id;
    int len = strlen(person.name);
    this->name = new char[len + 1];
    strcpy(this->name, person.name);
}

// 소멸자
Person::~Person() {
    delete[] name;
}

// 이름 변경
void Person::changeName(const char* name) {
    if (strlen(name) > strlen(this->name))
        return;
    strcpy(this->name, name);
}

int main() {
    Person father(1, "Kitae");
    Person daughter(father); // 깊은 복사 → name은 독립된 메모리

    cout << "daughter 객체 생성 직후----" << endl;
    father.show();
    daughter.show();

    daughter.changeName("Grace");

    cout << "daughter 이름을 Grace로 변경한 후----" << endl;
    father.show();
    daughter.show();

    return 0;
}

```

> [!note] 보충
> - `char*` 동적 할당은 `<cstring>` 의 `strlen` 으로 길이를 재서 처리
> - **RVO (Return Value Optimization)** : C++17 이전엔 객체 리턴 시 복사본 생성(복사 생성자 호출)이었지만, C++17 이후엔 임시 객체 없이 바로 호출자에게 전달하도록 최적화됨
> - **대입 vs 초기화** : `c1 = c2` 는 대입, `Circle c2 = c1` (선언과 동시)은 초기화

> [!example]- 실습 코드 (펼쳐보기)
> **1번**
>
> ```cpp
> #include <iostream>
> #include <cstring>
> using namespace std;
>
>  class Circle {
>  private:
>  int radius;
> public:
>  Circle() { radius = 1; }
>  Circle(int radius) { this->radius = radius; }
>  double getArea() { return 3.14*radius*radius; }
>  void swap (Circle& c1, Circle& c2)
>     {
>         int temp;
>         temp = c1.radius;
>         c1.radius = c2.radius;
>         c2.radius = temp;
>     }
>  };
>
> int main() {
>     Circle c1(2), c2(3);
>     cout << c1.getArea() << " " ;
>     cout << c2.getArea() << endl ;
>
>     swap(c1, c2);
>     cout << c1.getArea() << " " ;
>     cout << c2.getArea() << endl ;
>
>     return 0;
> }
>
> ```
>
> **2번**
>
> ```cpp
> #include <iostream>
> #include <cstring>
> using namespace std;
>
>  class Circle {
>  int radius;
>  public:
>  Circle() { radius = 1; }
>  Circle(int radius) { this->radius = radius; }
>  void setRadius(int radius) { this->radius = radius; }
>  double getArea() { return 3.14*radius*radius; }
>
>  friend void readRadius(Circle &c);
> };
>
> void readRadius(Circle &c){
>     cout << "정수 값으로 반지름을 입력하세요>> ";
>     cin >> c.radius;
> }
>
> int main() {
>      Circle donut;
>      readRadius(donut);
>      cout << "donut의 면적 = " <<donut.getArea() << endl;
> }
>
> ```
>
> **3번**
>
> ```cpp
> #include <iostream>
> #include <cstring>
> using namespace std;
>
>  class Accumulator {
>  int value;
>  public:
>  Accumulator(int value) {this->value = value;}
> // 매개변수 value로 멤버 value를 초기화한다.
>  Accumulator& add(int n)
>  {this->value += n; return *this; }
> // value에 n을 더해 값을 누적한다.
>  int get() {return value; }
> // 누적된 값 value를 리턴한다.
>  };
>
> int main() {
>      Accumulator acc(10);
>      acc.add(5).add(6).add(7); // acc의 value 멤버가 28이 된다.
>      cout << acc.get() << endl; // 28 출력
> }
>
> ```
>
> **4번**
>
> ```cpp
> #include <iostream>
> #include <cstring>
> using namespace std;
>
> class Book {
>     char *title;
>     int price;
> public:
>     Book(const char* title, int price);         // ← const 추가
>     ~Book();
>     Book(const Book& book);
>     void set(const char* title, int price);     // ← const 추가
>     void show() { cout << title << ' ' << price << "원" << endl; }
> };
>
> Book::Book(const char* title, int price) {
>     this->price = price;
>     int len = strlen(title);
>     this->title = new char[len + 1];
>     strcpy(this->title, title);
> }
>
> Book::~Book() {
>     delete[] title;
> }
>
> Book::Book(const Book& book) {
>     this->price = book.price;
>     int len = strlen(book.title);
>     this->title = new char[len + 1];
>     strcpy(this->title, book.title);
> }
>
> void Book::set(const char* title, int price) {
>     delete[] this->title;
>     this->price = price;
>     int len = strlen(title);
>     this->title = new char[len + 1];
>     strcpy(this->title, title);
> }
>
> int main() {
>     Book cpp("명품C++", 10000);
>     Book java = cpp;
>     java.set("명품자바", 12000);
>     cpp.show();    // 명품C++ 10000원
>     java.show();   // 명품자바 12000원
> }
>
> ```
>
> - 자가 학습 (all 코드)
> **문자열과 깊은 복사 ***
>
> ```cpp
> #include <iostream>
> #include <cstring>
> using namespace std;
>
> class NameTag {
>     char* name;
> public:
>     NameTag(const char* str);
>     NameTag(const NameTag& other);  //깊은 복사 생성자
>     ~NameTag();
>     void show() {cout << name;}
>     void showadd() {cout << this;}
> };
>
> NameTag::NameTag(const char* str) {
>     int len = strlen(str);
>     this->name = new char[len+1];
>     strcpy(this->name, str); // 오류 발생 x (문자열 깊은복사 전형적)
>     // this->name = str; 이 오류가 나는 이유?
>     // >> 그냥 포인터라서.. 주소를 가리키는게 되어버림
>     // >> 같은 주소 가리키는 것 = 얕은 복사, 메모리 공유, 이중delete
> }
>
> NameTag::NameTag(const NameTag& other){
>     // this->name = other.name; 얕은 복사임
>     int len = strlen(other.name);
>     this->name = new char[len+1];
>     // strcpy 뿐만이 아니라 this->name할당까지 해줘야 제대로 동작
>     strcpy(this->name, other.name);
> }
>
> NameTag::~NameTag(){
>     delete[] name; // new char len+1 를 []로 했으니 당연히 []
> }
>
> int main() {
>
>     char str[100];
>     cin.getline(str, 100);
>
>     NameTag nm(str);
>     NameTag cpnm = nm;
>
>     cout << "원본: "; nm.show();
>     cout << "복사본: "; cpnm.show();
>
>     nm.showadd(); cout << "\n"; cpnm.showadd();
>
>     cout << "\n원본, 복사본은 서로다른 메모리 가짐\n";
> }
>
> ```
>
> **복사 생성자 간단 복습**
>
> ```cpp
> #include <iostream>
> #include <cstring>
> using namespace std;
>
> class Box {
>     int width, height;
> public:
>     Box(int w, int h) {width = w; height = h;};
>     Box(const Box& b)
>    {this->width = b.width; this->height = b.height;
>     cout << " [ 복사 생성자 호출됨 ] \n";}
>     void printSize()
>    {cout << "박스 크기: " << width << " x " << height << endl;}
> };
>
> void printBox(Box b);
>
> int main() {
>     cout << "박스의 너비, 높이 입력 >> ";
>     int w, h;
>     cin >> w >> h;
>
>     Box origin(w,h);
>     Box CP = origin; // 복사 생성자 호출
>
>     CP.printSize();
>
>     return 0;
> }
> ```

---

# friend와 연산자 중복

캡슐화를 **의도적으로 뚫는 friend**, 그리고 연산자에 새 의미를 부여하는 **연산자 오버로딩**.

## 1. C++ 에서의 friend

**friend = 접근 지정자의 예외.** 클래스 멤버는 아니지만, `private` 멤버까지 접근하도록 특별히 권한을 부여.

- 클래스 멤버로 두긴 부적합한데 그 클래스의 모든 멤버에 접근해야 할 때 사용
- **friend 초대 3가지 유형** (교재 4p)
    1. 전역 함수
    2. 다른 클래스의 멤버 함수
    3. 다른 클래스 전체

**① 전역 함수를 friend 로**

```cpp
#include <iostream>
using namespace std;

class Rect;

bool equals(Rect r, Rect s);
// friend 전에 존재해야하니 맨 위에 한 번 선언해준다.

class Rect {
    int width, height;
public:
    Rect(int width, int height) {
        this->width = width;
        this->height = height;
    }
    friend bool equals(Rect r, Rect s); // bool equals 를 friend 로 선언
};

bool equals(Rect r, Rect s) {
    if (r.width == s.width && r.height == s.height) return true;
    else return false;
}

int main() {
    Rect a(3, 4), b(4, 5);
    if (equals(a, b)) cout << "equal \n"; // 자기 함수처럼 쓸 수 있다
    else cout << "not equal ";
}

```

**② 다른 클래스의 멤버 함수를 friend 로**

```cpp
#include <iostream>
using namespace std;

class Rect;
//전방 선언 (아래에서 선언되기전인 Rect 사용하기떄문)

class RectManager{
    public:
    bool equals(Rect r, Rect s);
    // 다른 클래스 멤버 함수
};

class Rect{
    int width, height;
    public:
        Rect(int width, int height) {
            this->width = width;
            this->height = height;
        }
    friend bool RectManager:: equals(Rect r,Rect s);
    // 다른 클래스 멤버 함수를 friend로
};

bool RectManager::equals(Rect r, Rect s){
    if (r.width == s.width && r.height == s.height) return true;
    else return false;
}

int main() {
  Rect a(3,4), b(4,5);
  RectManager man; // 다른 클래스 객체로 사용

  if(man.equals(a,b)) cout << "equal \n";
    else cout << "not equal ";
}
```

**③ 다른 클래스 전체를 friend 로 (제한 없음)**

```cpp
#include <iostream>
using namespace std;

class Rect;  //전방 선언

class RectManager{
    public:
    bool equals(Rect r, Rect s);
    void copy(Rect& dest, Rect& src);
};

class Rect{
    int width, height;
    public:
        Rect(int width, int height) {
            this->width = width;
            this->height = height;
        }
    friend RectManager;
    // 클래스명만 사용해서 friend선언해줌
    // 하나가 아니라 클래스 전체를 friend화(open)
};

bool RectManager::equals(Rect r, Rect s){
    if (r.width == s.width && r.height == s.height) return true;
    else return false;
}

void RectManager::copy(Rect& dest, Rect& src){
    dest.width = src.width;
    dest.height = src.height;
}

int main() {
  Rect a(3,4), b(4,5);
  RectManager man; // 다른 클래스 객체로 사용

  if(man.equals(a,b)) cout << "equal \n";
    else cout << "not equal ";
}
```

## 2. 연산자 중복 (연산자 오버로딩)

### (1) 개념

- 일상의 `+` : 빨강 + 파랑 = 보라, 2 + 3 = 5 … → 같은 기호, 다른 의미(**다형성**)
- C++ 도 연산자에 **새로운 의미를 정의** 가능 → 가독성 ↑
- 예) `+` 연산자 : 정수 더하기 / 문자열 합치기 / 객체 합 연산(색 섞기 등) / 배열 합치기

### (2) 특징 (제약)

- **C++ 에 원래 있는 연산자만** 중복 가능 (`3 %% 5` 같은 건 컴파일 오류)
- 제외 연산자 : `.`  `.*`  `::`(범위지정)  `?:`(삼항) → 이 넷 빼고 가능 (`+ - * / % ^ | ~ >> new` 등)
- 피연산자 **개수 변경 불가**, 연산 **우선순위 변경 불가**
- 반드시 **클래스와 관계**를 가지며, **클래스 안에서만** 중복 가능 (OOP 언어라서)

### (3) 연산자 함수

연산자를 **함수 형태**로 구현. 형식 : `TYPE operator연산자(매개변수);`

- **방법 1** — 클래스 **멤버 함수**로 구현 (operator 함수를 클래스 내부에 선언)
- **방법 2** — 외부 함수로 구현하고 클래스에 **friend** 로 선언

#### 이항 연산자 중복 (매개변수 O)

**+ 연산자 (멤버 함수)**

```cpp
#include <iostream>
using namespace std;

class Power { // 에너지를 표현하는 파워 클래스
private:
  int kick;  // 발로 차는 힘
  int punch; // 주먹으로 치는 힘
public:
  Power(int kick = 0, int punch = 0) { //생성자
    this->kick = kick;
    this->punch = punch;
  }

  Power operator+(Power op2) { //연산자 중복함수 멤버함수 선언
    // 실행시 C = a+b ->  C = a. + (b); 로 컴파일러가 변환
    Power tmp;                           // 임시 객체 생성
    tmp.kick = this->kick + op2.kick;    // kick 더하기
    tmp.punch = this->punch + op2.punch; // punch 더하기
    return tmp;                          // 리턴 타입은 Power operator+
  }                                      // 덩어리적인 덧셈 구현

  void show() { cout << "kick = " << kick << " " << "punch = " << punch << endl; }
};

int main() {
  Power a(3, 5), b(4, 6), c;
  c = a + b; // 파워 객체 + 연산
  a.show();
  b.show();
  c.show();
}

```

**== 연산자**

```cpp
#include <iostream>
using namespace std;

class Power { // 에너지를 표현하는 파워 클래스
private:
  int kick;  // 발로 차는 힘
  int punch; // 주먹으로 치는 힘
public:
  Power(int kick = 0, int punch = 0) { //생성자
    this->kick = kick;
    this->punch = punch;
  }

bool operator==(Power op2) {
    if(kick==op2.kick && punch==op2.punch) return true;
    else return false;
}
  void show() { cout << "kick = " << kick << " " << "punch = " << punch << endl; }

};

int main() {
  Power a(3, 5), b(3, 5);

  a.show();
  b.show();

 if (a == b) cout << "두 파워가 같다." << endl;

}

```

**+= 연산자**

```cpp
#include <iostream>
using namespace std;

class Power { // 에너지를 표현하는 파워 클래스
private:
  int kick;  // 발로 차는 힘
  int punch; // 주먹으로 치는 힘
public:
  Power(int kick = 0, int punch = 0) { //생성자
    this->kick = kick;
    this->punch = punch;
  }

    Power operator+=(Power op2)  {
     kick = kick + op2.kick; // kick +=
     punch = punch + op2.punch; // punch +=
     return *this; // 합한 결과 리턴 (this 쓰면 확실해짐
      }
  void show() { cout << "kick = " << kick << " " << "punch = " << punch << endl; }

};

int main() {
  Power a(3, 5), b(4, 6);

  a.show(); b.show();

  a += b; // a값 갱신 (a+b)
  a.show(); b.show();

}

```

**상수 + 객체 (friend 외부 함수)**

```cpp
#include <iostream>
using namespace std;

class Power { // 에너지를 표현하는 파워 클래스
public:
  int kick;  // 발로 차는 힘
  int punch; // 주먹으로 치는 힘
  Power(int kick = 0, int punch = 0) { //생성자
    this->kick = kick;
    this->punch = punch;
  }
  friend Power operator+(int op1, Power op2); // 프렌드 선언
  void show() { cout << "kick = " << kick << " " << "punch = " << punch << endl; }
};

Power operator+(int op1, Power op2){ // 외부에서 연산자중복함수 선언
     Power tmp;
     tmp.kick = op1 + op2.kick;
     tmp.punch = op1 + op2.punch;
     return tmp;
     }

int main() {
  Power a(3, 5), b;
  a.show(); b.show();
  b = 2 + a;
  a = 1 + a;
  a.show(); b.show(); // b는 2 + a 로 초기화, a는 1증가
}

```

**객체 + 객체 (friend 외부 함수)**

```cpp
#include <iostream>
using namespace std;

class Power { // 에너지를 표현하는 파워 클래스
private:
  int kick;  // 발로 차는 힘
  int punch; // 주먹으로 치는 힘

public:
  Power(int kick = 0, int punch = 0) { //생성자
    this->kick = kick;
    this->punch = punch;
  }
  friend Power operator+(Power op1, Power op2); // 프렌드 선언
  void show() { cout << "kick = " << kick << " " << "punch = " << punch << endl; }
};

Power operator+(Power op1, Power op2){ // 외부에서 연산자중복함수 선언
     Power tmp; // 임시객체생성
    tmp.kick= op1.kick + op2.kick; // kick 더하기
    tmp.punch= op1.punch + op2.punch; // punch 더하기
    return tmp; // 임시객체리턴
     }

int main() {
  Power a(3, 5), b(4,6), c;
  c = a + b;

  a.show(); b.show(); c.show();
}

```

#### 단항 연산자 중복 (매개변수 X)

- 전위 연산자 : `!op` `~op` `++op` `--op` / 후위 연산자 : `op++` `op--`
- **전위 ++ 와 후위 ++ 구분법?**
    - `operator++` 라는 같은 이름을 쓰되, 컴파일러가 알아서 구분
    - `++a` → `++(a)`, `a++` → `++(a, 0)` 으로 변환해 구분 (인자 `0` 은 의미 없는 구분용)

**전위/후위 ++ (멤버 함수)**

```cpp
#include <iostream>
using namespace std;

class Power { // 에너지를 표현하는 파워 클래스
public:
  int kick;  // 발로 차는 힘
  int punch; // 주먹으로 치는 힘
  Power(int kick = 0, int punch = 0) { //생성자
    this->kick = kick;
    this->punch = punch;
  }

    Power operator++() { // 매개변수X :: 전위 연산자
     kick++; punch++;
     return *this; // 변경된객체자신(객체a) 리턴
    }

    Power operator++(int x) { // 매개변수O :: 후위 연산자
    // friend 로 선언하려면 매개변수로 객체 받아야함! (접근방법이 없음)
     Power tmp = *this; // 증가 이전 객체상태
     kick++; punch++;
     return tmp; // 변경된객체자신(객체a) 리턴
    }

  void show() { cout << "kick = " << kick << " " << "punch = " << punch << endl; }

};

int main() {
  Power a(3, 5), b(4, 6);

  a.show(); b.show();
  ++a; a.show(); // 전위 test

  cout << "b++ 값 : " << b++.kick << endl; // 후위 = 증가 전, 4
    // 편의상 public 으로 변수선언, 바로 접근
  cout << "b++ 값 : " << b.kick << endl; // 증가 후, 5

}

```

**전위/후위 ++ (friend + 레퍼런스)**

```cpp
#include <iostream>
using namespace std;

class Power { // 에너지를 표현하는 파워 클래스
private:
  int kick;  // 발로 차는 힘
  int punch; // 주먹으로 치는 힘

public:
  Power(int kick = 0, int punch = 0) { //생성자
    this->kick = kick;
    this->punch = punch;
  }

 friend Power& operator++(Power& op); // 전위 ++
 friend Power operator++(Power& op, int x);  // 후위 ++

  void show() { cout << "kick = " << kick << " " << "punch = " << punch << endl; }
};

Power& operator++(Power& op) { // 전위++
    // 레퍼런스 리턴값으로 객체 원본값 변경
    op.kick++; op.punch++;
    return op;
    // 연산결과리턴 (++a 에서는 필요없으나 b = ++a 에서 필요)
    // 함수정의 자체를 리턴값있게 (void아님) 했기에 리턴 꼭필요
    // return 0 같이 이해
}

 Power operator++(Power& op, int x) { // 후위++
     // 레퍼런스 리턴값으로 객체 원본값 변경
    Power tmp= op; // 변경하기전의op 상태저장
    op.kick++; op.punch++;
     return tmp; // 변경이전의op 리턴
}

int main() {
  Power a(3,5), b;
 b = ++a; a.show(); b.show();  // a 증가 후의 값 4 6  4 6
 b = a++; a.show(); b.show();
// a 는 증가된 값 (2번 증가 완료)  b는 증가전 a값이니 4 6 그대로 나옴
}

```

> [!example]- 실습 코드 (펼쳐보기)
> **A1번**
>
> ```python
> #include <iostream>
> #include <cstring>
> using namespace std;
>
> class Book {
> 	string title;
> 	int price;
> 	int pages;
> public:
> 	Book(string title = "", int price = 0, int pages = 0) {
> 		this->title = title; this->price = price; this->pages = pages;
> 	}
> 	void show() {
> 		cout << title << ' ' << price << "원" << pages << " 페이지" << endl;
> 	}
> 	string getTitle() {
> 		return title;
> 	}
>
> 	Book operator+=(int num) {
> 		price = price + num;
> 		return *this;
> 	}
>
> 	Book operator-=(int num) {
> 		price = price - num;
> 		return *this;
> 	}
> };
>
> int main() {
>
> 	Book a("청춘", 20000, 300), b("미래", 30000, 500);
> 	a += 500;    // 책 a의가격500원증가
> 	b -= 500;
> 	// 책 b의 가격500원감소
> 	a.show();
> 	b.show();
> }
> ```
>
> **A2번**
>
> ```python
> #include <iostream>
> #include <cstring>
> using namespace std;
>
> class Book {
> 	string title;
> 	int price;
> 	int pages;
> public:
> 	Book(string title = "", int price = 0, int pages = 0) {
> 		this->title = title; this->price = price; this->pages = pages;
> 	}
> 	void show() {
> 		cout << title << ' ' << price << "원" << pages << " 페이지" << endl;
> 	}
> 	string getTitle() {
> 		return title;
> 	}
>
> 	friend Book& operator+=(Book& op, int num);
> 	friend Book& operator-=(Book& op, int num);
>
> };
>
> Book& operator+=(Book& op, int num) {
> 	op.price += num;
> 	return op; // 객체 리턴
> }
>
> Book& operator-=(Book& op, int num) {
> 	op.price -= num;
> 	return op; // 객체 리턴
> }
>
> int main() {
>
> 	Book a("청춘", 20000, 300), b("미래", 30000, 500);
> 	a += 500;    // 책 a의가격500원증가
> 	b -= 500;
> 	// 책 b의 가격500원감소
> 	a.show();
> 	b.show();
> }
> ```
>
> **B3번**
>
> ```python
> #include <iostream>
> #include <cstring>
> using namespace std;
>
> class Book {
> 	string title;
> 	int price;
> 	int pages;
> public:
> 	Book(string title = "", int price = 0, int pages = 0) {
> 		this->title = title; this->price = price; this->pages = pages;
> 	}
> 	void show() {
> 		cout << title << ' ' << price << "원" << pages << " 페이지" << endl;
> 	}
> 	string getTitle() {
> 		return title;
> 	}
>
> 	bool operator==(int num) {
> 		if (this->price == num) return true;
> 		else false;
> 	}
>
> 	bool operator==(string title) {
> 		if (this->title == title) return true;
> 		else false;
> 	}
>
> 	bool operator==(Book op) {
> 		if (title == op.title && price == op.price && pages == op.pages)
> 			return true;
> 		else return false;
> 	}
>
> };
>
> int main() {
>
> 	Book a("명품 C++", 30000, 500), b("고품 C++", 30000, 500);
> 	// price 비교
> 	if (a == 30000) cout << "정가 30000원" << endl;
> 	// 책 title 비교
> 	if (a == "명품 C++") cout << "명품 C++ 입니다." << endl;
> 	// title, price, pages 모두 비교
> 	if (a == b) cout << "두 책이 같은 책입니다." << endl;
> }
> ```
>
> **B4번**
>
> ```python
> #include <iostream>
> #include <cstring>
> using namespace std;
>
> class Book {
> 	string title;
> 	int price;
> 	int pages;
> public:
> 	Book(string title = "", int price = 0, int pages = 0) {
> 		this->title = title; this->price = price; this->pages = pages;
> 	}
> 	void show() {
> 		cout << title << ' ' << price << "원" << pages << " 페이지" << endl;
> 	}
> 	string getTitle() {
> 		return title;
> 	}
>
> 	friend bool operator==(Book& op, int num);
> 	friend bool operator==(Book& op, string title);
> 	friend bool operator==(Book& op1, Book&op2);
> };
>
> bool operator==(Book& op, int num) {
> 	if (op.price == num) return true;
> 	else false;
> }
>
> bool operator==(Book& op, string title) {
> 	if (op.title == title) return true;
> 	else false;
> }
>
> bool operator==(Book& op1, Book&op2) {
> 	if (op1.title == op2.title && op1.price == op2.price && op1.pages == op2.pages)
> 		return true;
> 	else return false;
> }
>
> int main() {
>
> 	Book a("명품 C++", 30000, 500), b("고품 C++", 30000, 500);
> 	// price 비교
> 	if (a == 30000) cout << "정가 30000원" << endl;
> 	// 책 title 비교
> 	if (a == "명품 C++") cout << "명품 C++ 입니다." << endl;
> 	// title, price, pages 모두 비교
> 	if (a == b) cout << "두 책이 같은 책입니다." << endl;
> }
> ```
>
> **5번**
>
> ```python
> #include <iostream>
> #include <cstring>
> using namespace std;
>
> class Book {
> 	string title;
> 	int price;
> 	int pages;
> public:
> 	Book(string title = "", int price = 0, int pages = 0) {
> 		this->title = title; this->price = price; this->pages = pages;
> 	}
> 	void show() {
> 		cout << title << ' ' << price << "원" << pages << " 페이지" << endl;
> 	}
> 	string getTitle() {
> 		return title;
> 	}
>
> 	bool operator!() {
> 		// 가격이 0 이상이면 0 리턴, 0이면 1
> 		return !(this->price);
> 	}
> };
>
> int main() {
>
> 	Book book("벼룩시장", 0, 50); // 가격은 0
> 	if (!book) cout << "공짜다" << endl;
> }
> ```

---

# 상속

**기존 클래스를 그대로 물려받아 확장하는 것.** 코드 재사용·분류·간결화의 핵심 도구.

## 1. 상속 개념

- 자식 클래스 = 부모 클래스를 **그대로 가져와** 생성
- **기본 클래스**(부모·슈퍼클래스) & **파생 클래스**(자식·서브클래스) — 파생으로 갈수록 개념이 구체화(가지 뻗기)
- **다중 상속** : 부모 클래스가 여러 개일 수 있음 (재활용성)
- **목적** : 클래스 관리·분류 용이, 간결화, 재사용 — 기존 클래스에 구현된 기능을 그대로 사용
    - EX) `(a,b,c,A) (a,b,c,B) (a,b,c,C)` 처럼 공통 요소(a,b,c)를 부모에 두고 A/B/C 만 각자 선언 → 공통 요소 관리가 쉬움

## 2. 파생 클래스 정의 · 객체 생성

기본형 : `class 파생클래스명 : 접근변경자 기본클래스이름 { };`

- 상속받은 건 재정의 X. 단, 처리 과정이 다른 함수는 **재정의(오버라이딩)** → 그 클래스 객체엔 오버라이딩된 함수 적용
- **업 캐스팅** : 파생 객체를 **부모 객체처럼** 다루기
- **다운 캐스팅** : 업캐스팅한 걸 다시 파생 객체로 되돌리기 → **명시적 형 변환 필요** (다중 상속이면 어디로 돌아갈지 모르니)

### 상속에서의 접근 지정자

| 멤버 | 파생 클래스에서 접근 |
|------|---------------------|
| `private` | **불가** — 선언 클래스 내에서만 (파생도 못 봄) |
| `protected` | **가능** — 선언 클래스 + 파생 클래스까지 (자식만 허락) |
| `public` | 가능 — 기본적으로 모두 접근 |

> 자세한 건 교재 18p 복습.

**① 상속 기본 & 부모/자식 객체 접근 범위**

```cpp
#include <iostream>
 #include <string>
 using namespace std;

// 2차원평면한점 클래스Point
class Point
{
int x, y; //한점(x,y) 좌표값
public:
 Point() {};
 void set(int x, int y) { this->x = x; this->y = y; }
 void showPoint() { cout<< "(" << x << "," << y << ")" << endl;}
 };

class ColorPoint: public Point {
// ColorPoint. Point를상속받음
string color; // 점의색표현
public:
 void setColor(string color)  { this->color = color; }
 void showColorPoint(){
 cout<< color << ":";
 showPoint(); // Point의showPoint() 호출
  };
};

int main() {
ColorPoint cp;

// cp.x = 10; (부모 클래스의 private 멤버라서, 접근이 불가하다)

cp.set(3,4);
cp.setColor("Red");
cp.showColorPoint();
}
```

**② 객체 포인터 : 업 캐스팅 & 다운 캐스팅**

```cpp
#include <iostream>
 #include <string>
 using namespace std;

class Point
{
int x, y;
public:
 Point() {};
 void set(int x, int y) { this->x = x; this->y = y; }
 void showPoint() { cout<< "(" << x << "," << y << ")" << endl;}
 };

class ColorPoint: public Point {
string color;
public:
 void setColor(string color)  { this->color = color; }
 void showColorPoint(){
 cout<< color << ":";
 showPoint(); // Point의 showPoint() 호출
  };
};

int main() {
ColorPoint cp;

// 부모 객체 포인터를 pBase, 자식 객체 포인터를 pDer

ColorPoint *pDer = &cp;
Point* pBase = pDer; //업캐스팅 (cp를 부모의객체처럼!)

pBase->set(3,4); // 부모객.포처럼 동작 (pDer->도 당연히된다)
pBase->showPoint();

  // 단, pBase->showColorPoint 는 컴파일 오류
  // 부모의 객체 포인터가 접근할 수 없기에 다운 캐스팅 필요

pDer = (ColorPoint*)pBase; // 다운 캐스팅 (되돌리는 느낌)
pDer->setColor("Red"); //자기 객체니 자기 거에만 접근가능
pDer->showColorPoint();
  // pBase->showColorPoint 는 컴파일 오류 ***
  // point* 같은 것은 정적이라, 해당 클래스 내에서만 찾아야 함
  // 즉 부모를 형변환한 값을 pDer에 대입하여 사용

}
```

**③ protected 상속의 접근 허용 범위**

```cpp
#include <iostream>
 #include <string>
 using namespace std;

// 2차원평면한점 클래스Point
class Point
{
int x, y; //한점(x,y) 좌표값
public:
 Point() {};
 void set(int x, int y) { this->x = x; this->y = y; }
 void showPoint() { cout<< "(" << x << "," << y << ")" << endl;}
 };

class ColorPoint: protected Point {
// ColorPoint. Point를상속받음
string color; // 점의색표현
public:
 void setColor(string color)  { this->color = color; }
 void showColorPoint(){
 // x = 10; 이런 건 당연히 안 된다 (부모 private는 어떻게 가져오든 X)
 cout<< color << ":";
 showPoint(); // Point의 showPoint() 호출 (protect라, 허용된다.)
  };
};

int main() {
ColorPoint cp;

// cp.set(3,4); 객체 포인터도 접근 불가! = 자식 내에서가 아니라서.

cp.setColor("Red");
cp.showColorPoint();
}
```

## 3. 파생 클래스의 생성자 · 소멸자

- **생성 순서** : 부모 → 파생 (부모 생성자가 먼저 실행)
- **소멸 순서** : 파생 → 부모 (생성의 역순)
- 파생 생성자는 부모의 **디폴트 생성자를 묵시적으로 호출**
    - 부모가 `F(int x)` 면 자식도 인자를 넘겨줘야 함 (`자식 → 부모` 호출)
    - 자식이 `F'(int x)`, 부모가 `F()` 면 OK (인자 있는 생성자가 디폴트 생성자 호출)
    - `F'(int x) : F(x+3) { }` 처럼 부모 생성자를 **명시적으로** 호출도 가능

> [!tip] 부모의 디폴트 생성자는 거의 필수
> 위 구조 때문에, 상속을 쓸 거면 부모 클래스에 **인자 없는 디폴트 생성자**를 웬만하면 만들어 두는 게 안전.

**생성자 명시적 지정**

```cpp
#include <iostream>
#include <cstring>
using namespace std;

class Circle {
private:
	int radius;
public:
	Circle() { radius = 0; }
	Circle(int r) { radius = r; }
	double getArea() {
		return 3 * radius * radius;
	}
	void show() { cout << radius; }
};

class NamedCircle: public Circle {
private:
	string title;
public:
	NamedCircle(int r, string title) : Circle(r) { this->title = title; } // 생성자 명시적 지정

};

int main() {
	NamedCircle waffle(3, "waffle"); // 반지름이 3이고 이름이 waffle인 원
	waffle.show();
}

```

- **(4) 접근 지정자와 접근 변경자**
    - **어떤 접근 변경자가 붙어도 기본 클래스 private는 절대 접근 불가능
    - private 상속 → 모두 private멤버로 간주한다
    - protected 상속 → 모두 protected 멤버로 간주한다
    - public 상속 * → protected 는 protected, public 은 public 으로 그대로 상속받는다
- **(5) 다중상속**

> [!example]- 실습 코드 (펼쳐보기)
> **1번**
>
> ```cpp
> #include <iostream>
> #include <cstring>
> using namespace std;
>
> class Circle {
> private:
>     int radius;
>     string title;
> public:
>     Circle() { radius = 0; }
>     Circle(int r, string t) { radius = r; title = t; }
>
>     double getArea() { return 3 * radius * radius; }
>     int getRadius() { return radius; }
>     string getTitle() { return title; }
> };
>
> class NamedCircle : public Circle {
> public:
>     NamedCircle(int r, string t) : Circle(r, t) {}
>
>     void show() {
>         cout << "반지름이 " << getRadius() << "이고 이름이 " << getTitle() << "인 원" << endl;
>     }
> };
>
> int main() {
>     NamedCircle waffle(3, "waffle"); // 반지름이 3이고 이름이 waffle인 원
>     waffle.show();
> }
>
> ```
>
> **2번**
>
> ```cpp
> #include <iostream>
> #include <cstring>
> using namespace std;
>
> class Circle {
> public:
>     int radius; string title;
>     Circle() { radius = 0; }
>     Circle(int r, string t) { radius = r; title = t; }
> };
>
> class NamedCircle : public Circle {
> public:
>     NamedCircle() {}
>     NamedCircle(int r, string t) : Circle(r,t) {}
> };
>
> int main() {
>     NamedCircle pizza[5];
>
>     cout << "5개의 정수 반지름과 원의 이름을 입력하세요."<< endl;
>     for (int i = 0; i < 5; i++) {
>         cout << i+1 << " >> ";  cin >> pizza[i].radius >> pizza[i].title;
>     }
>
>     int max = pizza[0].radius;  NamedCircle maxcircle;
>
>     for (int i = 1; i < 5; i++) {
>         if (max < pizza[i].radius) {
>             maxcircle = pizza[i];
>             max = pizza[i].radius;
>         }
>     }
>
>     cout << "가장 면적이 큰 피자는 " << maxcircle.title << "입니다.";
>
> }
>
> ```
>
> **3번**
>
> ```cpp
> #include <iostream>
> using namespace std;
>
> class BaseArray {
> private:
>     int capacity;
>     int* mem;
> protected:
>     BaseArray(int capacity = 100) {
>         this->capacity = capacity;
>         mem = new int[capacity];
>     }
>     ~BaseArray() { delete[] mem; }
>     void put(int index, int val) { mem[index] = val; }
>     int get(int index) { return mem[index]; }
>     int getCapacity() { return capacity; }
> };
>
> class MyQueue : public BaseArray {
> private:
>     int front, rear, count;
> public:
>     MyQueue(int capa = 100) : BaseArray(capa), front(0), rear(0), count(0) {}
>
>     void enqueue(int n) { put(rear, n); rear++; count++;}
>     int dequeue() { int val = get(front); front++; count--; return val;}
>
>     int length() { return count; }
>     int capacity() { return getCapacity(); }
> };
>
> int main() {
> 	MyQueue mQ(100);
> 	int n;
> 	cout << "큐에 삽입할 5개의 정수를 입력하라>> ";
> 	for (int i = 0; i < 5; i++) {
> 		cin >> n;
> 		mQ.enqueue(n); // 큐에삽입
> 	}
> 	cout << "큐의용량: " << mQ.capacity() << ", 큐의크기: " << mQ.length() << endl;
> 	cout << "큐의원소를순서대로제거하여출력한다>> ";
> 	while (mQ.length() != 0) {
> 		cout << mQ.dequeue() << ' '; // 큐에서제거하여출력
> 	}
> 	cout << endl << "큐의현재크기: " << mQ.length() << endl;
>
> }
>
> ```

---

# 가상 함수와 추상 클래스

**다형성의 핵심.** 부모 포인터로 자식의 재정의 함수를 호출(동적 바인딩)하고, 구현을 강제하는 추상 클래스까지 다룬다.

## 1. 클래스 형변환 규칙 (복습)

**업 / 다운 캐스팅 복습**

```cpp
#include <iostream>
#include <string>
using namespace std;

class Point
{
	int x, y;
public:
	Point() {};
	void set(int x, int y) { this->x = x; this->y = y; }
	void showPoint() { cout << "(" << x << "," << y << ")" << endl; }
};

class ColorPoint : public Point {
	string color;
public:
	ColorPoint() {};
	void setColor(string color) { this->color = color; }
	void showColorPoint() {
		cout << color << ":";
		showPoint(); // Point의 showPoint() 호출
	};
};

int main() {
	ColorPoint* pt = new ColorPoint;
	pt->set(1, 2);
	pt->setColor("red");
	pt->showColorPoint();  // red:(1,2)

	// 업캐스팅: ColorPoint* → Point*
	Point* pPoint = pt;
	pPoint->showPoint();   // (1,2)
	// pPoint->setColor("blue"); // 불가 (Point에는 없음)

	// 다운캐스팅: Point* → ColorPoint*
	Point* base = new ColorPoint;          // 업캐스팅
	ColorPoint* derived = (ColorPoint*)base; // 다운캐스팅
	derived->set(3, 4);
	derived->setColor("blue");
	derived->showColorPoint(); // blue:(3,4)
}

```

**장점 — 하나의 인터페이스로 여러 타입 처리**

```cpp
void f(Shape& s) {
 s.Draw(); // 인터페이스는 동일하나 다양하게 전달
 }

 int main() {
 Rectangle r1;
 f(r1);

 Circle e1;
 f(e1);
 }
```

## 2. 가상 함수

- **가상 함수** = 기본 클래스 포인터로 호출해도 **파생 클래스에 재정의된 함수가 호출**되도록 하는 멤버 함수
    - 형식 : `virtual 리턴타입 함수이름() const`
    - print 만 하는 함수 등은 const 함수로 지정하면 좋음
- **동적 바인딩** = 호출될 함수를 **실행 시간에** 결정 (가상 함수는 바인딩을 실행 시점까지 미룸)
- **오버라이딩** = 같은 형태의 함수를 재정의 / **오버로딩** = 이름만 같고 인자가 다른 함수

**기본 예시**

```cpp
#include <iostream>
#include <string>
using namespace std;

// 부모 클래스
class Point {
	int x, y;
public:
	Point() {
		cout << "Point 생성자 호출" << endl;
	}

	// 가상 소멸자
	virtual ~Point() {
		cout << "Point 소멸자 호출" << endl;
	}

	void set(int x, int y) {
		this->x = x;
		this->y = y;
	}

	// 가상 함수: 동적 바인딩
	virtual void show() {
		cout << "(" << x << "," << y << ")" << endl;
	}
};

// 자식 클래스
class ColorPoint : public Point {
	string color;
public:
	ColorPoint() {
		cout << "ColorPoint 생성자 호출" << endl;
	}

	~ColorPoint() override {
		cout << "ColorPoint 소멸자 호출" << endl;
	}

	void setColor(string color) {
		this->color = color;
	}

	// 오버라이딩
	void show() override {
		cout << color << ":";
		Point::show();  // 부모의 show()도 함께 호출
	}
};

// main 함수
int main() {
	// 부모 포인터로 자식 객체를 생성 (다형성)
	Point* p = new ColorPoint;

	// 부모 함수 호출
	p->set(10, 20);

	// 다운캐스팅해서 자식 함수 호출 (setColor)
	ColorPoint* cp = (ColorPoint*)p;
	cp->setColor("red");

	// 동적 바인딩: 자식 클래스의 show()가 호출됨
	p->show();  // 출력: red:(10,20)

	// 가상 소멸자를 통해 올바르게 소멸자 호출됨
	delete p;

	return 0;
}

```

```cpp
#include <iostream>
using namespace std;

class Animal {
public:
    virtual void speak() {cout << "Some animal sound\n";}
};

class Dog : public Animal {
public:
    void speak() override { cout << "Bark!\n"; }
};

class Cat : public Animal {
public:
    void speak() override {cout << "Meow!\n";}
};

void makeSound(Animal* a) {  a->speak(); }
// 자식 객체 따라 다르게 동작 (동적 바인딩) => 다형성!

int main() {
    Dog d;
    Cat c;
    makeSound(&d); // Bark!
    makeSound(&c); // Meow!
}

```

## 3. 추상 클래스와 인터페이스 상속

- 기본 클래스의 가상 함수 = 파생 클래스가 **재정의할 함수를 알려주는** 역할
- 굳이 구현할 필요가 없으면 → **순수 가상 함수**로 관리

### 순수 가상 함수 & 추상 클래스

- **순수 가상 함수** = 코드 없이 **선언만** 있는 가상 멤버 함수
    - 형식 : `virtual 리턴타입 함수이름() const = 0;`
    - 파생 클래스에서 **재정의(구현) 필수** → 구현이 파생에 종속되는 함수
- **추상 클래스** = 순수 가상 함수를 **하나 이상** 가진 클래스
    - 완전하지 않아 **객체 생성 불가** (단, 포인터·레퍼런스 변수는 정의 가능)
    - 목적 : 인스턴스 생성이 아니라 상속에서 **기본 클래스 역할**
    - 추상 클래스를 상속만 하면 자동으로 추상 클래스, 함수를 오버라이딩(구현)하면 더 이상 추상 아님

> [!note] 상속의 3가지 결
> - **구현 상속** : 기본 클래스가 함수의 구현을 제공 (껍데기 아닌 완전한 기능)
> - **디폴트 구현 + 인터페이스 상속** : 가상 함수 → 파생이 재정의할 수도, 안 할 수도
> - **인터페이스 상속** : 순수 가상 함수 → 파생이 **필수로 재정의** (껍데기만 제공)

### 어떤 멤버 함수를 쓸지 — 가이드라인

- 특별할 것 없으면 → **일반 멤버 함수**
- 다형성이 필요하면 → **가상 함수**
- 다형성을 위해 함수 원형만 필요하면 → **순수 가상 함수**

> [!example]- 실습 코드 (펼쳐보기)
> **1번**
>
> ```cpp
> #include <iostream>
> #include <string>
> using namespace std;
>
> class Shape {
> 	int r;
> 	string n;
> public:
> 	Shape() {};
> 	virtual string getName() const=0;
> 	virtual double getArea() const=0;
> };
>
> class Oval : public Shape {
> 	int r1, r2; string name;
> public:
> 	Oval(string n, int r1, int r2) {
> 		this->name = n;
> 		this->r1 = r1;
> 		this->r2 = r2;
> 	}
> 	string getName() const {
> 		return name;
> 	}
> 	double getArea() const {
> 		return r1 * r2 * 3.14;
> 	}
> };
>
> class Rect : public Shape {
> 	int L1, L2; string name;
> public:
> 	Rect(string n, int L1, int L2) {
> 		this->name = n;
> 		this->L1 = L1;
> 		this->L2 = L2;
> 	}
> 	string getName() const {
> 		return name;
> 	}
> 	double getArea() const {
> 		return L1 * L2;
> 	}
> };
>
> class Triangular : public Shape {
> 	int L1, L2; string name;
> public:
> 	Triangular(string n, int L1, int L2) {
> 		this->name = n;
> 		this->L1 = L1;
> 		this->L2 = L2;
> 	}
> 	string getName() const {
> 		return name;
> 	}
> 	double getArea() const {
> 		return L1 * L2 * 1/2;
> 	}
> };
>
> int main() {
> 	Shape* p[3];
> 	p[0] = new Oval("빈대떡", 10, 20);
> 	p[1] = new Rect("찰떡", 30, 40);
> 	p[2] = new Triangular("토스트", 30, 40);
>
> 	for (int i = 0; i < 3; i++)
> 		cout << p[i]->getName() << " 넓이는 " << p[i]->getArea() << endl;
> 	for (int i = 0; i < 3; i++) delete p[i];
>
> }
> ```
>
> **2번**
>
> ```cpp
> #include <iostream>
> #include <string>
> using namespace std;
>
> class Calculator {
> public:
> 	virtual int add(int a, int b) = 0; // 두정수의합리턴
> 	virtual int subtract(int a, int b) = 0; // 두정수의차리턴
> 	virtual double average(int a[], int size) = 0; // 배열a의평균리턴. size는배열의크기
> };
>
> class GoodCalc : public Calculator {
> 	int a, b;
> public:
> 	GoodCalc() {};
>
> 	virtual int add(int a, int b) { return a + b; }
> 	virtual int subtract(int a, int b) { return a - b; }
> 	virtual double average(int a[], int size) {
> 		double sum = 0;
> 		for (int i = 0; i < size; i++)
> 			sum += a[i];
> 		return  (sum / size);
> 	}
> };
>
> int main() {
> 	int a[] = { 1,2,3,4,5 };
> 	Calculator* p = new GoodCalc();
> 	cout << p->add(2, 3) << endl;
> 	cout << p->subtract(2, 3) << endl;
> 	cout << p->average(a, 5) << endl;
> 	delete p;
> }
> ```
>
> - 3번
> ---

---

# 템플릿과 STL

타입을 일반화하는 **템플릿(제네릭)**, 그리고 그 위에 세워진 표준 자료구조·알고리즘 모음 **STL**.

## 1. 클래스 템플릿

- **처리 알고리즘은 동일, 데이터 형만 다양**할 때 → 타입을 일반화해 하나로
    - 로직은 그대로 두고, 데이터 형식마다 함수 이름을 다르게 안 해도 됨
    - 범용 데이터 형식 → 여러 타입에 대해 함수/클래스 정의를 자동 생성
- **장점** : 코드 재사용 / **단점** : 포팅 취약, 디버깅 어려움, 오류 메시지 빈약
- **제네릭 프로그래밍** = 제네릭 함수/클래스 활용 (C#, Java 등도 지원)

**형식**

```cpp
template < class A, class B, int MAX >
 class TwoArray {
 // 중간 생략
 A arr1[ MAX ];
 B arr2[ MAX ];
 };
 TwoArray< char, double, 20 > arr;

/*
class TwoArray_char_double_20{
 // 중간 생략
 char arr1[ 20 ];
 double arr2[ 20 ];
 };
 */
```

**Stack 템플릿 클래스 예시**

```cpp
#include <iostream>
using namespace std;

const int DEFAULT_SIZE = 100;

template <class T = int>
class Stack {
protected:
	int m_size; int m_top; T* m_buffer;
public:
	Stack(int size = DEFAULT_SIZE);
	~Stack(); void Push(T value); T Pop();
};

//생성자
template <class T>
Stack<T>::Stack(int size) {
	m_size = size;	m_top = -1;
	m_buffer = new T[m_size];
}

//소멸자
template <class T>
Stack<T>::~Stack() { delete[] m_buffer; }

//push 함수 구현
template <class T>
void Stack<T>::Push(T value) {
	if (m_top >= m_size - 1) {
		cerr << "Stack Overflow!" << endl;
		return;
	}
	m_buffer[++m_top] = value;
}

//pop 함수 구현
template <class T>
T Stack<T>::Pop() {
	if (m_top < 0) {
		cerr << "Stack Underflow!" << endl;
		return T();  // 기본 생성된 값 반환
	}
	return m_buffer[m_top--];
}

int main() {
	Stack<int> iStack;
	Stack<double> dStack;

	iStack.Push(3);
	int n = iStack.Pop();

	dStack.Push(3.5);
	double d = dStack.Pop();

	cout << "int pop: " << n << ", double pop: " << d << endl;

	return 0;
}

```

> [!note] 템플릿 사용 시 주의
> - 실제로 **사용되기 전까지는 코드가 생성되지 않음** (인스턴스화하는 코드를 만나면 그때 컴파일러가 클래스 코드 생성)
> - 함수 템플릿 정의 / 클래스 템플릿 멤버 함수 정의는 **헤더 파일**에 놓여야 함

## 2. STL (Standard Template Library)

C++ 표준 라이브러리 중 하나. 성능 우수·안정성 검증된 제네릭 클래스·함수 모음.

- **구성 3요소**
    - **컨테이너** = 데이터를 담는 자료구조 템플릿 클래스 (list, queue, stack, map, set, vector)
    - **iterator(반복자)** = 컨테이너 원소를 가리키는 포인터. 원소를 순회·접근할 때 사용
    - **알고리즘** = 복사·검색·정렬 등을 구현한 템플릿 함수 (컨테이너의 멤버 함수가 아님)

```cpp
vector<int>::iterator it;
 it = v.begin();
 // 이런 식으로 생성

 /*  begin() 과 end() 함수
• STL 컨테이너가공통으로제공하는함수
• begin()  함수 : 첫 번째 원소를 가리키는iterator를 리턴
• end() 함수 : 마지막 원소를가리키는iterator를 리턴 */
```

- **헤더(이름공간)**
    - 컨테이너 사용 → `#include <vector>`, `<list>` …
    - 알고리즘 함수 사용 → 항상 `#include <algorithm>`
- **map** : key 기준으로 빠른 검색·정렬. `find` `size` `erase` 등 제공해 편리
    - 내부적으로 pair 를 사용 → map 헤더 포함 시 pair 사용 가능
    - `dic.at("love")` 와 `dic["love"]` 는 기능은 같지만, 없는 key 일 때 `at` 은 **오류**, `[]` 는 **빈 값 생성**
- **STL 컨테이너의 장점** : 종류가 달라도 **인터페이스가 동일** → 코드 재활용성 우수 (최소 수정으로 컨테이너 교체 가능)

**list — iterator 로 탐색**

```cpp
#include <iostream>
#include <list>
using namespace std;

int main() {
  // int 타입 담을 링크드리스트 생성
  list<int> intList;

  for (int i = 0; i < 10; ++i)
    intList.push_back(i + 1); // 1 ~ 10 링크드리스트 넣음

  intList.remove(5); // 5를찾아서제거한다.
  // 내용 출력.

  // iterator : 반복자 (컨테이너 원소 포인터)
  list<int>::iterator it;
  for (it = intList.begin(); it != intList.end(); ++it)
    cout << *it << "\n";
  return 0;
}
```

**map & std::pair — pair 는 서로 다른 값 2개를 묶는 자료구조**

> map 순회는 iterator 반복문보다 `const auto& pair` 범위 기반 for 가 간단하다.

```cpp
#include <iostream>
#include <map>
#include <string>
using namespace std;

int main() {

  // #map 예시
  map<string, string> dic;

  dic.insert(make_pair("love", "사랑"));

  string kor = dic["love"];
  string kor2 = dic.at("love");

  cout << kor << ", " << kor2 << endl;

  // #pair 예시
  pair<int, string> p(1, "Hello");
  cout << p.first << " " << p.second;

// ========================

  // @map 예시 구체화
  map<string, int> studentScores;

  // 학생3명 이름 점수 map에추가
  studentScores.insert(make_pair("Alice", 85));
  studentScores.insert(make_pair("Bob", 92));
  studentScores.insert(make_pair("Charlie", 78));

  map<string, int>::iterator it;

  // "Bob" 찾아서 삭제
  it = studentScores.find("Bob");
  if (it != studentScores.end())
    studentScores.erase(it);

  // 학생목록출력(iterator 사용)
  cout << "\n=== 학생점수목록===\n";
  for (it = studentScores.begin(); it != studentScores.end(); ++it)
    cout << it->first << ": " << it->second << "\n";

  // const auto 사용
  for (const auto &pair : studentScores) {
    cout << pair.first << ": " << pair.second << "\n";
  }

  return 0;
}

```

**vector & sort**

- `sort` 함수는 `<algorithm>` 헤더에 정의
- `sort(시작 포인터, 끝 포인터)` 형태로 사용
- 단, **map 요소는 정렬 불가** → 배열 포인터, vector 의 `begin`/`end` 반복자만 쓰자

```cpp
#include <algorithm>
#include <vector>
#include <iostream>
using namespace std;
void main() {
	// 벡터
	vector<char> vec;
	vec.push_back('e');
	vec.push_back('b');
	vec.push_back('a');
	vec.push_back('d');
	vec.push_back('c');

	// sort() 함수로 벡터 정렬
	sort(vec.begin(), vec.end());
	cout << "\nvector 정렬후\n";
	vector<char>::iterator it;
	for (it = vec.begin(); it != vec.end(); ++it)
		cout << *it;

	// 배열 정렬
	char arr[5] = { 'd','c','b','a','e' };
	// sort() 함수 정렬
	sort(arr, arr + 5);
	cout << "\n배열 정렬 후\n";
		for (char* p = arr; p != arr + 5; ++p)
			cout << *p;
}
```

> [!example]- 실습 코드 (펼쳐보기)
> **1번**
>
> ```cpp
> #include <iostream>
> using namespace std;
>
> template <typename T>
> T* remove(T src[], int sizeSrc, T minus[], int sizeMinus, int& retSize) {
>
>     // x 몇개 y 몇개 (제거해야하는 개수)
>
>     T* temp = new T[sizeSrc];
>     int count = 0;
>
>     for (int i = 0; i < sizeSrc; i++) {
>         bool found = false;
>         for (int j = 0; j < sizeMinus; j++) {
>             if (src[i] == minus[j]) {
>                 found = true;
>                 break;
>             }
>         }
>         if (!found)
>             temp[count++] = src[i];
>     }
>
>     retSize = count;
>
>     if (retSize == 0) {
>         delete[] temp;
>         return nullptr;
>     }
>
>     T* result = new T[retSize];
>     for (int i = 0; i < retSize; i++)
>         result[i] = temp[i];
>
>     delete[] temp;
>     return result;
> }
>
> int main() {
>     // remove() 함수를 int로 구체화하는 경우
>     cout << "정수 배열 {1,2,3,4}에서 정수 배열 {-3,5,10,1,2,3}을 뺍니다" << endl;
>     int x[] = { 1, 2, 3, 4 };
>     int y[] = { -3, 5, 10, 1, 2, 3 };
>     int retSize;
>
>     int* p = remove(x, 4, y, 6, retSize);
>     if (retSize == 0) {
>         cout << "모두 제거되어 리턴하는 배열이 없습니다." << endl;
>     }
>     else {
>         for (int i = 0; i < retSize; i++)
>             cout << p[i] << ' ';
>         cout << endl;
>         delete[] p;
>     }
>
>     // remove() 함수를 double로 구체화하는 경우
>     cout << "실수 배열 {1.1, 2.2, 3.3, 4.4}에서 실수 배열 {3.3, 5.5}를 뺍니다"
>         << endl;
>     double dx[] = { 1.1, 2.2, 3.3, 4.4 };
>     double dy[] = { 3.3, 5.5 };
>     double* dp = remove(dx, 4, dy, 2, retSize);
>
>     if (retSize == 0) {
>         cout << "모두 제거되어 리턴하는 배열이 없습니다." << endl;
>     }
>     else {
>         for (int i = 0; i < retSize; i++)
>             cout << dp[i] << ' ';
>         cout << endl;
>         delete[] dp;
>     }
>
>     return 0;
> }
>
> ```
>
> **2번**
>
> ```cpp
> #include <iostream>
> #include <map>
> #include <string>
>
> using namespace std;
>
> int main() {
> 	map<string, string> dic; // 맵 컨테이너 생성. 키는 영어단어, 값은 한글단어
>
> 	// 단어 3개를 map에 저장
> 	dic.insert(make_pair("love", "사랑"));
> 	dic.insert(make_pair("apple", "사과"));
> 	dic["cherry"] = "체리";
>
> 	cout << "저장된 단어 개수: " << dic.size() << endl;
>
> 	string eng;
> 	while (true) {
> 		cout << "찾고 싶은 단어 >> ";
> 		cin >> eng;
>
> 		if (eng == "exit")
> 			break;
>
> 		// map에서 키가 존재하는지 확인
> 		if (dic.find(eng) == dic.end())
> 			cout << "없음" << endl;
> 		else
> 			cout << dic[eng] << endl;  // 해당 키의 값을 출력
> 	}
>
> 	/*
> 	map<type, type> 이름 으로 맵 생성
> 	insert(make_pair(1 2 )) 이런 식으로 짝지을 수 있다.
> 	dic.end(), dic.start() 는 반복자이다. => dic.find(eng) == dic.end() 이런 식으로 써야 함
> 	dic.size() 는 해당 딕셔너리의 목록개수를 출력해준다 (pair 개수) => return size 형식
> 	map 은 파이썬 딕셔너리와 비슷한 느낌
> 	*/
>
> 	cout << "종료합니다..." << endl;
> }
>
> ```
>
> - *추가 : 13은  pdf 보고 세부사항 복습!!!!
> ---
> ---
>
