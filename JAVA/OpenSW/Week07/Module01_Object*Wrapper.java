

class Point {
    private int x, y;
    
    public Point(int x, int y) {
        this.x= x;
        this.y= y;
    }
    public String toString() {
        return "Point(" + x + "," + y + ")";
    }
    public boolean equals(Object obj) {
        Point p = (Point)obj;
        if(x == p.x && y == p.y) return true;
        else return false;
    }
}

public class Main{
    public static void print(Object obj) {
      // Object class 의 주요 메소드 사용 예시
      
        System.out.println(obj.getClass().getName()); // 클래스 이름
        System.out.println(obj.hashCode()); // 해시코드값
        System.out.println(obj.toString()); // 객체를 문자열로 만들어 출력
        System.out.println(obj); // 객체 출력
    }
    
    public static void main(String [] args) {
        // Objec class - #1 사용 예제
        Point p1 = new Point(2,3);
        print(p1);

        // Objec class - #2 Object class toString 메소드
        Point p2 = new Point(2,3);
        System.out.println(p2.toString());
        System.out.println(p2); // p는 p.toString()으로 자동 변환
        System.out.println(p2 + " 입니다.");
        // p.toString() + "입니다"로 자동변환


        // Objec class - #3 객체 비교와 equals() 메소드
        Point a = new Point(2,3);
        Point b = new Point(2,3);
        Point c = new Point(3,4);
        if(a == b) // false
            System.out.println("a==b");
        if(a.equals(b)) // true
            System.out.println("a is equal to b");
        if(a.equals(c)) // false
            System.out.println("a is equal to c");


        // Wrapper class - #1 메소드 활용
        System.out.println(Character.toLowerCase('A')); // 'A'를 소문자로 변환
        char c1='4', c2='F';
        if(Character.isDigit(c1)) // 문자 c1이 숫자이면 true
            System.out.println(c1 + "는숫자");
        if(Character.isAlphabetic(c2)) // 문자 c2가 영문자이면 true
            System.out.println(c2 + "는영문자");

        System.out.println(Integer.parseInt("-123")); // "-123" 을 10진수로 변환
        System.out.println(Integer.toHexString(28)); // 28을 2진수 문자열로 변환
        System.out.println(Integer.toBinaryString(28)); // 28을 16진수 문자열로 변환
        System.out.println(Integer.bitCount(28)); // 28에대한 2진수 1의개수

        Double d = Double.valueOf(3.14);
        System.out.println(d.toString()); // Double을 문자열 "3.14"로 변환
        System.out.println(Double.parseDouble("3.14")); // 문자열을 실수 3.14로 변환
        boolean e = (4>3); // b는 true
        System.out.println(Boolean.toString(e)); // true를 문자열 "true"로 변환
        System.out.println(Boolean.parseBoolean("false")); // 문자열을 false로 변환

        /* 실행 결과는 아래와 같다.
        a
        4는숫자
        F는영문자
        -123
        1c
        11100
        3
        3.14
        3.14
        true
        false
        */


        // Wrapper class - #2 박싱, 언박싱
        int n = 10;
        Integer intObject = n; // auto boxing (기본타입을 Wrapper객체로변환)
        System.out.println("intObject = " + intObject);

        int m = intObject + 10; // auto unboxing (Wrapper객체 기본타입값 빼냄)
        System.out.println("m = " + m);

        /* 실행 결과는 아래와 같다.
        intObject = 10
        m = 20
        */
    }
}
