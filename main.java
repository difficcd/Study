
// Write once, run anywhere. JAVA

/*   Java 언어의 특징 정리
1. 기계어로 직접 컴파일/링크하는 C, C++ 컴파일러와 달리
자바 컴파일러는 바이트코드인 클래스 파일(.class) 을 생성하고 
JVM 이 바이트 코드를 인터프리팅함 => Java는 하이브리드 언어
- 플랫폼에 종속된 C,C++ 와 다르게 JVM 만 있으면 알아서 동작
  즉, 프로그램 실행을 운영체제가 아니라 JVM 이 함 (독립적)
- 종속된 언어는 윈도우/리눅스 혹은 macOS 호환이 불가능..

2. Java 는 클래스 기반 OOP 언어, 함수가 항상 클래스 안에 존재
여러 기능을 모듈화하려면 여러 java 파일로 쪼개서 관리한다
=> 한 .java 파일에는 하나의 public class 만 선언 가능 
   *public 이 아닌 일반 class 는 여러 개 선언 가능하긴 하다
=> 또한, 파일 이름과 같아야 함 (ex. class Main : Main.java)


3. public class 안의 함수들 앞에 public static 을 붙여야 함
c의 main함수처럼 프로그램 시작은 항상 public static void 
main(String[] args) 에서 한다.  static 은 객체 없이 바로 
사용할 수 있게 하기 위해 필요하다.
=> OOP 언어지만 프로그램 시작 시에 미리 객체를 생성할 수는 
없기 때문에, static 으로 바로 실행 가능하도록 구현한다. 
main 은 약속된 이름이다.
=>  main(String[] args) 의 매개변수는 문자열 배열로,
실행 시 외부에서 전달하는 값을 받을 수 있음 (명령행 인자)
운영체제는 문자열을 전달하기 때문에 String 으로 지정힌 것
*/



import java.util.Scanner;
import java.util.ArrayList;
// Scanf 기능은 Scanner Class를 import 하여 사용

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        // 입력 객체 (Scanner 객체) 생성

        ArrayList<String> list = new ArrayList<String>();
        // 버전이 높으면 new ArrayList<>(); 로도 쓸 수 있음
      
        list.add("apple");
        System.out.println(list.get(0));
        // ArrayList 는 C++ 의 벡터 클래스와 비슷하다.
      
        String s = list.get(0); // 값이므로 직접 변수에 넣을 수도 있음

        // next : 공백 전까지 문자열 입력 
        // nextInt/Double.. 이런 식으로 특정 자료형 입력
      
        String input = sc.nextLine(); // 문자열 한 줄 입력
        int num = sc.nextInt(); // 정수 입력
      
        System.out.println(input + num);
      
        sc.close(); 
        // 자원 정리 (I/O resource 는 GC 외 범위)

      
        // 변수 선언 형식은 C 와 거의 동일
        double pi = 3.14;
        boolean isStudent = true; 
        String name = "홍길동";
        int age = 20;

        System.out.print(name + ", ");
        System.out.println("나이는 " + age + "살입니다.");
          // 줄바꿈 포함은 print에 ln을 붙여주면 된다

        int result = add(1, 2); 
        System.out.println("덧셈 결과: " + result);

          // ==== Person.java ====
          // 객체 생성 (new 키워드 사용)
      
          Person p1 = new Person("홍길동", 20);
          Person p2 = new Person("김철수", 25);
      
          // 메서드 호출
          p1.introduce();
          p2.introduce();
    }

    // main 밖에 선언
    public static int add(int a, int b) {
        return a + b;
    }
}



