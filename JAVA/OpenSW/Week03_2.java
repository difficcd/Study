

// 04~07 (22p ~ p)
// 이미 아는 내용은 skip

public class Main { 
    public static void main(String[] args) {

        // 04 다차원 배열
        
            int arr1[][] = new int[5][5];
            for(int i=0; i<5; i++)
                for(int j=0; j<5; j++)
                    arr1[i][j] = (i+1) * (i+1);

            int arr2[][] = {{0,1},{2,3}}; 
            // [] 2개 : {} 2개이고, []안에 개수 쓰면 X
            // N==M arr : 정방형, 아닌 배열은 비정방형. 데이터배치&length 참고 24p!

            System.out.println(arr2.length);
            System.out.println(arr2[0].length + " " + arr2[1].length);

            int i[][];
            i = new int[4][];
            i[0] = new int[1]; // 0행에 원소 1개 생성 : [0][0]
            i[1] = new int[2]; // 1행에 원소 2개 생성 : [1][0], [1][1]
            i[2] = new int[3];
            i[3] = new int[4];

            System.out.println("\n" + i.length);
            System.out.println(i[0].length); // 1
            System.out.println(i[1].length); // 2
            System.out.println(i[2].length); // 3
            System.out.println(i[3].length); // 4

        
        // 05 메소드에서의 배열 리턴
    
            // 메소드는 레퍼런스만 리턴 = 배열 이름으로 리턴
            int[] return_arr = returntest();
            for(int k=0; k<return_arr.length; k++)
                System.out.print(return_arr[k] + " ");


        // 06 main() 메소드

            // (1) main 메소드의 특징
                // main() 은 자바 응용프로그램 실행시작 메소드
                // public static void main(String[] args)의 의미
                /*public : 다른클래스 호출가능, JVM 에서 호출되어야 함
                static : 자신 포함 클래스 객체 생성 전에 JVM 에 호출됨
                void : main 은 아무 값도 리턴하지 않음 (끝낼때는 return;)
                문자열 배열 : 명령행 입력된 인자들을 문자열배열로 만들어 main 전달 */

            // (2) main 메소드의 매개변수
                // main 메소드 실행 전에 인자들은 각각 문자열로 만들어지고
                // 문자열(String) 배열에 저장됨
                // 문자열 배열에 대한 레퍼런스가 main매개변수 args 에 전달되어
                // 이런 방식으로 main() 메소드를 며령행 인자를 전달받음


        // 07 자바의 예외 처리

            // 예외란 : 컴파일 오류(문법에 맞지 않게 작성된 코드, 컴파일러가 사전ck)
            //          예외 오류 (오작동/결과 악영향 가능성 있는 런타임 오류)
            // 1. 0 나누기 오류  2. 배열 인덱스 범위 오류
            // 3. 존재하지 않는 파일 읽으려는 경우  4. 입력 타입 불일치 오류
                
            try {
                int result = 10 / 0; // 오류 발생 (ArithmeticException)
                System.out.println("\n\n결과: " + result);
                // 예외 발생 가능성 있는 실행문
            } catch (Exception e) { 
                System.out.println("\n\n예외 발생: " + e.getMessage());
                // 예외 처리문
            } finally {
                System.out.println("finally 블록 실행");
                // 예외 발생 여부와 무관히 무조건 실행. 생략 가능
            }

            // 예외 발생 시 동작, 자바의 예외 클래스 : 32-33p
            
    }

     public static int[] returntest(){
         int temp[] = new int[4];
         for(int i=0; i<temp.length; i++)
             temp[i] = i+1;
         System.out.println();
         return temp;
         // returntest 메소드의 temp 레퍼런스가 new int[4] 배열 가리킴
         // temp 레퍼런스를 받는 레퍼런스와 값 치환해주면 handle 교환됨
     }
    
}
