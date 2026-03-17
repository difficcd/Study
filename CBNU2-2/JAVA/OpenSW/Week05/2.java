

// 04~09 (26 ~ 45p)

class Shared
 { public static final double PI = 3.14; }
  // 모든 클래스 공유 가능 상수 PI 

     
class Circle{ 
    int radius;
    double area;

    public Circle(int r){ 
        if (r <= 0) {
            this.radius = 1;  
            return;           
        }
        this.radius = r;
        this.radius = r;
        this.area = Shared.PI * r * r;
    }

    public double getArea(){ 
        return this.area;
    }

     // 메소드 오버로딩 사례
     void increase(int m) { m = m + 1; }
     void increase(Circle m) { m.radius++; }
}


public class Main { 
    
     public static int abs(int a) {
         if(a < 0) a *= -1;
         return a;
     }   
    
     public static void main(String[] args) {
        Circle [] c;        // 클래스 배열에 대한 레퍼런스 변수 c선언: 크기지정-오류
        c = new Circle[5];  // Circle 객체에 대한 레퍼런스 5개 생성


        for(int i=0; i<c.length; i++)    // c.length: 배열 길이
            c[i] = new Circle(i);        // i번째 객체 생성

        for(int i=0; i<c.length; i++)
            System.out.println( (int)c[i].getArea() );
            // 배열 c의 i번째 객체 접근 : c[i] 레퍼런스 사용


        // # 인자 전달 - 기본 타입의 값 전달
            int n = 10;
            c[0].increase(n); 
            System.out.println("\n" + n);
        
            // 값 복사이므로 변화 없음 (n = 10)

        
        // # 인자 전달 - 객체가 전달되는 경우
            Circle m = new Circle(10);
            m.increase(m);
            System.out.println(m.radius);
        
            // 객체(클래스,배열) : 레퍼런스값 복사 전달
            // 즉 주소 복사 (n = 11)

        
        // 가비지 컬렉션
        Circle a, b;
        a = new Circle(1);
        b = new Circle(2);
        b = a; // b가 가리키던 객체는 가비지가 됨
        System.gc(); // 가비지 컬렉션 작동 요청


        System.out.println(abs(-5)); // static 메소드 사용법
         
        // static으로 선언된 필드/메소드는 하나만 생성되어 클래스 객체들 사이에서 공유됨
        
    }
    
    // Final 필드 : final 필드를 선언하면 필드는 상수가 됨
    //               ### 44p의 오버라이딩 관련 개념 복습
    public class FinalFieldClass {
        final int ROWS = 10;   // 상수 정의, 이때 초기값(10)을 반드시 설정
        void f() {
            int[] intArray = new int [ROWS]; // 상수 활용
            // ROWS = 30;        컴파일 오류 발생, 변경 불가능
        }
    }
}
