
import java.util.Scanner;
import java.util.Random;

public class Practice17 { 
    public static void main(String[] args) {
   
            Scanner scanner= new Scanner(System.in);
            Random random = new Random();
            boolean loop = true;

            while(loop){
                int rand = random.nextInt(100);
                System.out.println("수를 결정하였습니다. 맞추어 보세요\n0-99");
                
                int input = -1; 
                int cnt = 0;

                int max=99; int min=0;
                
                while(true){
                    cnt ++;
                    System.out.print(cnt + ">>");
                    input = scanner.nextInt();
                    
                    if(input == rand) {
                        System.out.println("맞았습니다.");
                        System.out.println("다시 하시겠습니까(y/n)>>");
                        String retry = scanner.next();
                        if(retry.equals("n")) loop = false;

                        break;
                    }
                        
                    else if (input > rand) {
                        max = input;
                        System.out.println("더 낮게");
                        System.out.println(min + "-" + max);
                    }
                    else { 
                        min = input;
                        System.out.println("더 높게");
                        System.out.println(min + "-" +max);
                    }
                }
            }
            
            scanner.close();

    }
}
