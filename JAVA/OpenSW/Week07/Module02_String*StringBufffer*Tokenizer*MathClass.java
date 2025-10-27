

import java.util.StringTokenizer ;

public class Main {
    public static void main(String[] args) {

        // #1 String Class 활용
            String a = new String(" C#");
                    String b = new String(",C++ ");
            System.out.println(a + " 길이: " + a.length()); // 문자열 길이(문자개수)
            System.out.println(a.contains("#")); // 문자열 포함관계
    
            // 문자열 연결
            a = a.concat(b);  System.out.println(a);
    
            // 문자열 앞뒤 공백제거
            a = a.trim();   System.out.println(a);
    
            // 문자열 대치
            a = a.replace("C#","Java");  System.out.println(a);
    
            // 문자열 분리
            String s[] = a.split(","); 
            for (int i=0; i<s.length; i++)
                System.out.println("분리 문자열 => " + i+ ": " + s[i]);
    
            // 인덱스 5~끝 리턴
            a = a.substring(5);  System.out.println(a);
    
            // 인덱스 2 문자 리턴
            char c = a.charAt(2);  System.out.println(c);


        // #2 StringBuffer Class 활용
            StringBuffer sb = new StringBuffer("This");

            // 문자열덧붙이기
            sb.append(" is pencil");  System.out.println(sb);

            // "my" 문자열 삽입
            sb.insert(7, " my");  System.out.println(sb);

            // "my"를 "your"로변경
            sb.replace(8, 10, "your");  System.out.println(sb);

            // "your " 삭제
            sb.delete(8, 13);  System.out.println(sb);

            // 스트링버퍼 내 문자열 길이 수정
            sb.setLength(4);    System.out.println(sb);

        
        // #3 StringTokenizer Class 활용
            // import java.util.StringTokenizer ; 필요
            StringTokenizer st = new StringTokenizer("홍길동/장화/홍련/콩쥐/팥쥐", "/");
            // '/'를 구분문자로 지정
            while (st.hasMoreTokens())
                System.out.println(st.nextToken());

        
        // #4 Math Class 활용
            System.out.println(Math.PI); // 원주율 상수 출력
            System.out.println(Math.ceil('a')); // ceil(올림)
            System.out.println(Math.floor('a')); // floor(내림)
            System.out.println(Math.sqrt(9)); // 제곱근
            System.out.println(Math.exp(2)); // e의2승
            System.out.println(Math.round(3.14)); // 반올림

            // [1, 45] 사이의 정수형 난수 5개
            System.out.print("이번주 행운의 번호는 ");
            for(int i=0; i<5; i++)
                System.out.print((int)(Math.random()*45 + 1) + " ");
    }
}
