

public class Practice07 {
    public static void main(String[] args) {
        int a = 3, b = 5;
        System.out.println("두 수의 차는 " + ((a > b) ? (a - b) : (b - a)));
    }
}

// 2가 출력됨. : a>b false -> b-a 출력
