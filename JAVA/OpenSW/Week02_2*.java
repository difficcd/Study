

// 03~06 (19 ~ 36p)


import java.util.Scanner;

public class Main { 
    public static void main(String[] args) {

            final double PI = 3.141592; 

            long m = 25;      
            double d1 = 3.14*10;  
            int n1 = 300;

            double d2 = 1.9;
            int n2 = (int)d2; 

   
            Scanner scanner= new Scanner(System.in); 

            String name = scanner.next();
            int age = scanner.nextInt();
            double weight = scanner.nextDouble();
            boolean single = scanner.nextBoolean();
        
            
            scanner.close();

            int s1 = 69/10; 
            int s2 = 69%10; 

            int r = s2 % 2;  

            int a = 10;
            a--; System.out.println(a);
            System.out.println(--a); 
            System.out.println(a--); 
            System.out.println(a); 

            int x = 5;
            int y = 3;
            int s0 = (x>y)?1:-1; 


            switch (x) { 
                case 1:
                    break;
                case 2: 
                default:
                    x=10;
                    break;
            }


        }
}
