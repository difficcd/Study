

import java.util.Scanner;

public class Practice14 {
    public static void main(String[] args) {
        Scanner scanner= new Scanner(System.in);

        System.out.println("가위바위보 게임입니다. 가위, 바위, 보 중에서 입력하세요 ");
        System.out.print("철수 >> ");
        String player1 = scanner.next();

        System.out.print("영희 >> ");
        String player2 = scanner.next();

        String winner ;

        int p1=0, p2=0;

        // 가위 0 바위 1 보 2
        // 기본 대소비교, 0 > 2만 예외

        if(player1.equals("가위")) p1 = 0;
        else if(player1.equals("바위")) p1 = 1;
        else if(player1.equals("보")) p1 = 2;

        if(player2.equals("가위")) p2 = 0;
        else if(player2.equals("바위")) p2 = 1;
        else if(player2.equals("보")) p2 = 2;


        if(p1 == 0 && p2 == 2)  winner = "철수가 이겼습니다.";
        else if(p1 == 2 && p2 == 0)  winner = "영희가 이겼습니다.";
        else if(p1==p2) winner = "비겼습니다.";
        else if(p1>p2) winner = "철수가 이겼습니다.";
        else winner = "영희가 이겼습니다.";

        System.out.print(winner);

        scanner.close();
    }
}
