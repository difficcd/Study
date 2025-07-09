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

