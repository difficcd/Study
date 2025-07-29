// 백준 제출 내용 & 피드백
// https://www.acmicpc.net/status?user_id=owele&result_id=4 여기서 순서대로 함 (JAVA)



// 0709 


// 2739번 : C++ 가볍게 복습용으로 풀이
// 10926번 :  걍 간단한 출력문제 
import java.util.Scanner;

public class Main{

  public static void main(String[] args){
    Scanner sc = new Scanner(System.in);
    String str = sc.next();

    
    System.out.print(str + "??!");
  }
}

// 2884번 : 시간 계산 문제 
import java.util.Scanner;
public class Main {
  public static void main(String[] args) {
    Scanner sc = new Scanner(System.in);
    int H = sc.nextInt();
    int M = sc.nextInt();
    
    M -= 45; // 45분 가감부터 하고
    if (M < 0) { // 음수 분이면
      M += 60; // H 에서 빌려오기
      H--;
      if (H < 0)
        H = 23; // 음수 시면 23시로 만들기 (초과)
    }
    sc.close();
    System.out.println(H + " " + M);
  }
}


/* 원래 내가 생각한 로직 메모
  
  24 시간, 60 분 => 분으로 환산하는 방법
  예를 들어, 10시 10분 => 10*60 => 610분 - 45분 한 다음 다시 변환 (나누기와 나머지 사용)
  0시 30분 이런식으로 들어온건 간단하게 24를 더해서 처리해주면 됨

  단, 2884번은 조건이 달라서 틀림으로 처리되었다.
 */


// 1152번 단어 수 세기 (문자열연습)

import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        String s = sc.nextLine();
        StringBuilder str = new StringBuilder(s);
      // StringBuilder : Java 문자열 쉽게 수정하게 해 줌 
      // 문자열 수정 수단이 없어 새로 문자열을 만들어야 해서 비효율 
      // setCharAt을 제공
      
        int num = 0;

        if (s.charAt(0) == ' ') {
            if (s.charAt(s.length() - 1) == ' ') {
                str.setCharAt(s.length() - 1, 'a')
                  // String변수.length() => 문자열 길이
                  // setCharAt(대체할 위치, 대체할 문자)
                num--; 
            } else
                str.setCharAt(0, 'a');
        } else if (s.charAt(s.length() - 1) == ' ')
            str.setCharAt(s.length() - 1, 'a');
        else
            num++;

        for (int i = 0; i < s.length(); i++) {
            if (s.charAt(i) == ' ')
                num++;
        }

        System.out.println(num);

        sc.close();

    }
}

// 로직이 너무 중구난방하고 더러움 : 아래는 개선코드

import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        String s = sc.nextLine().trim(); // 앞뒤 공백 제거

        if (s.isEmpty()) {
            System.out.println(0); // 비었으면 0 출력
        } else {
            String[] words = s.split("\\s+"); // 이스케이프+\s :공백
            //words: 나눠진 단어들을 담는 문자열 배열(String[])
            
            // \\s+ 공백(스페이스, 탭, 줄바꿈 등)을 1개 이상
            System.out.println(words.length);
        }

        sc.close();
    }
}

// 1978번 : 소수 찾기 간단한 연습

import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        int N = sc.nextInt();
        int[] num = new int[N]; // int num[N] java식 선언 (C++유사)
        int[] cnt = new int[N];
        int count = 0;

        // 반복 입력 예제
        for (int i = 0; i < N; i++) {
            num[i] = sc.nextInt();
            cnt[i] = 0; // cnt 초기화
        }

        for (int j = 0; j < N; j++) {
            if (num[j] == 1)
                continue; // 1은 소수가 아님..
            else {
                for (int k = 1; k <= num[j]; k++)
                    if ((num[j] % k) == 0) // 나누어떨어지면
                        cnt[j]++;
            } // 해당 num 수의 cnt 증가
        }

        for (int i = 0; i < N; i++)
            if (cnt[i] == 2)
                count++;

        System.out.println(count);

        sc.close();

    }
}


// 1676번 : 팩토리얼 0의개수 (수학,로직)
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        int N = sc.nextInt();
        int cntF=0;
        int cntT=0;
        
        
        // 10 => 10 (5 2) => 2
        // 20 => 20 (15 12) 10 (5 2) => 4
        // 25 => (25 22) 2*10 (15 12) 10 (5 2)
        // 적립식으로 count => 5의 배수는 n개, 2의 배수는 m개

        // 10 9 8 7 6 => 8 나누면 2가 3개인거임 
        
        for(int i=N; i>0; i--) {
        	int j = i;
        	while(j % 5 == 0) {
        		cntF++;
        		j = j/5; 
        	}
        	
        	int k = i;
        	while(k % 2 == 0) {
        		cntT++;
        		k = k/2; 
        	}
        }

        int result;
        
        if(cntF>cntT) result = cntT;
        else if(cntT>cntF) result = cntF;
        else result = cntT;
        // java 의 Math.min() 쓰면 빠르다 : 기본 제공해줌!
        
        System.out.println(result);

        sc.close();  

    }
}


// 9421번, 소수상근수
// Java 에서 해시테이블 & 소수 로직 복습
// i*i 로 하는게 sqrt 로 하는 것보다 정확함. sqrt 는 다소 오차범위가 존재하고 불안정함.

import java.util.Scanner;
import java.util.HashSet;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        int N= sc.nextInt();
        
        spone_num(N);

        sc.close();

    }

    public static void spone_num(int N){
        
        for(int i=1; i <= N; i++) {
            HashSet<Integer> list = new HashSet<Integer>();

            String str = String.valueOf(i);
            list.add(i); // 스스로가 다시 나오는 경우도 loop

            
            while(true) {
                int sum = 0;
                
                for(int j=0; j<str.length(); j++){
                    int num = str.charAt(j) - '0';
                    sum += num * num; // 끝나면 한 개 출력하는거임

                }
                str = String.valueOf(sum); 
                
                if (sum == 1 && is_prime(i)) {
                    System.out.println(i);
                    break;
                }
                
                if (list.contains(sum))
                    break;
                
                else list.add(sum);    
            }
        
            
        }
        
    }
    public static boolean is_prime(int N){
        boolean prime =true;
        
        if (N != 1){
            for (int i=2; i*i <=N; i++){
                int tmp = N;
                if(tmp % i == 0)
                    prime = false;
            }
        }
        else prime = false;
        
        return prime;
    }
}
