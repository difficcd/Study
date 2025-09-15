

// 01~03 자바의 데이터 타입-리터럴 (1~18p)


import java.util.Scanner;
// Scanf 기능은 Scanner Class를 import 하여 사용


// 한 public class 안에 여러 개의 메서드가 존재
// 메서드는 클래스 안에서만 선언 가능

public class Main { 

    // main은 반드시 public, static, void 타입으로 선언
    // String[] args 로 실행 인자를 전달받음
    public static void main(String[] args) {
        Scanner scanner= new Scanner(System.in);

        int $num = sum(1,2);
        // 메소드 내에 선언된 지역변수 : 실행 끝 = 자동 소멸
        // 특수문자는 $,_만 가능함. 한글 가능. 키워드, 숫자시작변수 불가


        // 리터럴 (literal)
        // : 프로그램에서 직접 표현한 값. 정수, 실수, 문자, 논리, 문자열 모두 존재

        // 1) 정수 리터럴 
            int n1 = 0x15; 
            int n2 = 015;
            // int n2 = 0b0101;
            // 015 : 8진수, 0b : 2진수
            System.out.println(n1 + ", " + n2); 
            // 21 : 1*5 + 16*1,  13 : 1*5 + 8*1

            long g1 = 24L, g2 = 24l;
            System.out.println(g1 + ", " + g2); // long int

        // 2) 실수 리터럴 : double 로 컴파일
            double d = 0.1234;
            double e = 1234E-4; // 1234x10^-4 이므로 d==e
            System.out.println(d + ", " + e);

            float f = 0.1234f;
            double w = .1234D; // .1234D, .1234 동일
            System.out.println(f + ", " + w);

        
        // 3) 문자 리터럴 : 단일 인용부호 혹은 \\u 사용
            char a = 'A';
            char c = '\u0041'; // 'A' 의 <<유니코드>> 값
            System.out.println(a + " == " + c);
            // 특수문자 리터럴 : 16p

        // 4) 논리 리터럴 : true,false 로 Boolean 에 치환/조건문 사용
            boolean bool1 = true; // 정수 1 사용 불가!
            boolean bool2 = 10 > 0; // true
        
            // C와 달리 while(true) 의 true 를 1로 쓸 수 없음. 
            // null 타입*** 은 객체 레퍼런스에 대입할 때만 가능
            String str_example = null; // 이건 가능

        // 5) var 키워드 : 변수 타입 생략 
            // Java 10부터 도입된 키워드, 지역 변수 선언에만 가능
            // 컴파일러가 알아서 추론해 준다
            var price = 300;
            var pi = 3.14;
            // 초기값 지정해야 오류 안 남! (생성 시점에 필수적으로 추론)
        
        String str = scanner.next();
        System.out.println(str);
        System.out.println($num);
        // 표준 출력 스트림 = System.out

        scanner.close();

        boolean flag = true;    System.out.println(flag);
        byte byte_example = 10;    System.out.println(byte_example);  // -128~127


    // reference type : 배열/클래스/인터페이스에 대한 ref 존재
    // !문자열은 자바의 기본 타입에 속하지 않음 >> Java lib 제공 String Class 이용
    
        String toolName = "JDK ";
        toolName += 1.8; // 숫자를 String 에 영입시킬 수 있음
        System.out.println(toolName + " is patched");
        
    }

    public static int sum(int n, int m) { 
        // Java 도 함수를 밑에 선언해도 된다.
        return n + m;
    }

    // 카멜(Camel) 표기법 : 소문자(default) 
    //          여러 단어로 구성되는 이름은 각 단어의 첫 글자만 대문자
    // 클래스 첫글자는 대문자, 변수와 메소드는 카멜 표기법(소문자 시작+다음 단어 대문자)
    // 상수 (static) : 모든 문자를 대문자로 표시


    // public static ~ : 메소드
    // main 메소드부터 실행 시작


    
}
