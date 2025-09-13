


//  13p 부터 이어서 실습내용 정리
//  20~25p 단위로 나눠서 정리


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
        
        String str = scanner.next();
        System.out.println(str);
        System.out.print($num);
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
        return n + m;
    }

    // 카멜(Camel) 표기법 : 소문자(default) 
    //          여러 단어로 구성되는 이름은 각 단어의 첫 글자만 대문자
    // 클래스 첫글자는 대문자, 변수와 메소드는 카멜 표기법(소문자 시작+다음 단어 대문자)
    // 상수 (static) : 모든 문자를 대문자로 표시


    // public static ~ : 메소드
    // main 메소드부터 실행 시작


    
}
