

// 03~06 (19 ~ 36p)


import java.util.Scanner;

public class Main { 
    public static void main(String[] args) {

        // 03 *Java 의 data Type : 상수
            final double PI = 3.141592; 
            // 상수 선언 : final Type Name 
            // 상수는 변수와 달리 실행중 값 변경 X : 초기값 지정 필수
        
            // PI = 2.2; // 컴파일 오류 발생

        
        //03  Type 변환 (Casting)
            // 1 ) 자동 타입 변환
            long m = 25;        // 원래 int 인 25가 long 으로 변환
            double d1 = 3.14*10;  // 원래 int 인 10이 double(10.0)로 변환

            // 2 ) 강제 타입 변환
            int n1 = 300;
            // byte b = n; int -> byte 자동변환 불가 (큰타입->작은타입 불가)

            double d2 = 1.9;
            int n2 = (int)d2; // 강제 타입 변환, 값 손실 가능성 존재

        
        // 04 System.in 과 Scanner Class
            // System.in : 키보드로부터 직접 읽는 자바 표준 입력스트림
            // 키 값을 바이트 데이터로 리턴하므로 응용프로그램이 문자정보로 변환해야함
            // 키보드에 입력된 키를 문자,정수,실수,문자열 등으로 변환해주는 게 Scanner(util)
            Scanner scanner= new Scanner(System.in); // Scanner 객체 생성
        
            // >>  System.in 에게 키를 읽게 하고 원하는 타입으로 반환해 리턴해줌
            // >>  Scanner : 입력되는 키 값을 공백으로 구분되는 아이템 단위로 읽음
            String name = scanner.next();
            int age = scanner.nextInt();
            double weight = scanner.nextDouble();
            boolean single = scanner.nextBoolean();
        
            
            scanner.close();
            // Scanner 객체 사용 종료는 객체이름.close(); 로 함
            // Scanner 닫힐 때 System.in 도 함께 닫힘, 더 이상 사용 불가
        
            
        // 05 식과 연산자
            // 연산 : 주어진 식 계산해 결과 얻어내는 과정 
            // 증감, 산술, 시프트, 비교, 비트, 논리, 조건, 대입
            // 연산자 우선순위 : postfix, prefix, 양수부호 음수부호, 형변환 */% ... 25p

            // 1 ) 산술 연산자 => + - * / %
            int s1 = 69/10; // 몫 6
            int s2 = 69%10; // 나머지 9 
                // => 나누기, 나머지로 10의자리와 1의자리 분리한 것
            int r = s2 % 2;  // 홀짝 판단

            // 2 ) 증감 연산자 => ++, -- 두 가지
            int a = 10;
            a--; System.out.println(a); // 9
            System.out.println(--a); // 8
            System.out.println(a--); // 8
            System.out.println(a); // 7
            // a-- : a를 1감소하고 감소 전의 값 반환

            // 3 ) 대입 연산자 => +=, -=, &=, ^=, |=, <<= 이런거 다 포함

            // 4 ) 비교 연산자 / 논리 연산자
                // 비교 : <, > , !=, == ..
                // !a, a||b, a&&b, a^b(XOR)      => 비트연산자와 구분

            // 5 ) 조건 연산자 : 세 개의 피연산자로 구성된 삼항(ternary) 연산자
            int x = 5;
            int y = 3;
            int s0 = (x>y)?1:-1; // x>y true 이므로 s0 = 1 대입됨

            
        // 06 조건문
            // 비교/논리 연산이 혼합된 식으로 구성, 조건식 결과 = boolean
            // if, if-else, 다중 if-else문은 생략

            switch (x) { // x위치는 정수, 문자, 문자열 리터럴만 가능함!
                case 1:
                    // 실행 문장
                    break;
                case 2: 
                    // case 2는 그냥 넘어감
                    // break 존재 : 끝냄. 존재X : 다음 case 검사
                default:
                    x=10;
                    break;
            }


        }
}
