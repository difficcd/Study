
public class Practice06 {
    public static void main(String[] args) {
        // 비교연산
        System.out.println('a' > 'b');
        System.out.println(3 >= 2);
        System.out.println(-1 < 0);
        System.out.println(3.45 <= 2);
        System.out.println(3 == 2);
        System.out.println(3 != 2);
        System.out.println(!(3 != 2));
        
        // 비교연산 & 논리연산 복합
        System.out.println((3 > 2) && (3 > 4));
        System.out.println((3 != 2) || (-1 > 0));
        System.out.println((3 != 2) ^ (-1 > 0));
    }
}


// 결과는 차례대로 F T T F F T F F T T 
// F=false, T=true

