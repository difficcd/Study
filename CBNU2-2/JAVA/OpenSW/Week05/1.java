

// 01~03 (3 ~ 26p)

class Circle{     // default 접근지정 class
    int radius;
    double area;
    
    // 매개변수에 따라 생성자 오버로딩
    // 생성자는 리턴 타입 지정 불가 (아무 값도 리턴하지 않음)
    
    /* 기본 생성자 (default constructor) : 아무 일도 안 하는 생성자
                >> public Circle() {}
                >> 생성자 선언이 없으면 컴파일러에 의해 자동 삽입 */
    
    /* 생성자는 객체 생성 시 필수적으로 호출되어야 함 
        >> 필드 초기화, 메모리 확보, 파일 열기, 네트워크 연결 등 
           객체가 활동하기 전 필요한 초기 준비를 하는 데 이용 */
    
    public Circle(){       // 생성자-1 (매개변수X)
        this(0);            
        /* this() : 다른 생성자 호출
            1 반드시 생성자 코드에서만 호출 가능
            2 반드시 같은 클래스 내 다른 생성자만 호출
            3 반드시 생성자의 첫 번째 문장이어야 함 */
    }
    
    public Circle(int r){  // 생성자-2 (매개변수O)
        if (r <= 0) {
            this.radius = 1;  
            return;           
            // return; - 아무 값도 리턴하지 않지만
            // 조건에 따라 아래 코드의 실행이 결정됨
        }
        this.radius = r;
        this.radius = r;
        this.area = 3.14 * r * r;
        /* this : 객체 자신에 대한 레퍼런스
                  컴파일러에 의해 자동 관리 

           1 객체의 멤버 변수, 메소드 변수 이름이 같은 경우
           2 다른 메소드 호출 시 객체 자신의 레퍼런스 전달할 때
           3 메소드가 객체 자신의 레퍼런스를 반환할 때 */
    }

    
    public double getArea(){  // 메소드
        this.area = 3.14 * 
            this.radius *
            this.radius;
        return this.area;
    }
}



public class Main { // public class는 단 1개
    public static void main(String[] args) {
        
        // 생성자는 new를 통해 객체 생성 시 객체당 1회 호출
        // donut, pizza : 레퍼런스 변수
        
        Circle donut = new Circle(); // 객체 생성 (생성자-1)
        donut.radius = 5;
        System.out.println(donut.getArea());


        Circle pizza = new Circle(4); // 객체 생성 (생성자-2)
        System.out.println(pizza.getArea());
        System.out.println();

        
        Circle temp;  temp = pizza;
        pizza = donut; donut = temp; 
        System.out.println(donut.getArea());
        System.out.println(pizza.getArea());
        // 객체 치환 (얕은 복사 - 여러 레퍼런스가 같은 객체 참조함)
    }
}
