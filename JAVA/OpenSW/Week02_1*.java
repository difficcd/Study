


import java.util.Scanner;

public class Main { 
    public static void main(String[] args) {
        Scanner scanner= new Scanner(System.in);

        int $num = sum(1,2);
        
            int n1 = 0x15; 
            int n2 = 015;
            
            System.out.println(n1 + ", " + n2); 

            long g1 = 24L, g2 = 24l;
            System.out.println(g1 + ", " + g2);

        
            double d = 0.1234;
            double e = 1234E-4;
            System.out.println(d + ", " + e);

            float f = 0.1234f;
            double w = .1234D; 
            System.out.println(f + ", " + w);

        
        
            char a = 'A';
            char c = '\u0041'; 
            System.out.println(a + " == " + c);

       
            boolean bool1 = true; 
            boolean bool2 = 10 > 0; 
        
            String str_example = null; 

            var price = 300;
            var pi = 3.14;

        
        String str = scanner.next();
        System.out.println(str);
        System.out.println($num);

        scanner.close();

        boolean flag = true;    System.out.println(flag);
        byte byte_example = 10;    System.out.println(byte_example);  

    
        String toolName = "JDK ";
        toolName += 1.8; 
        System.out.println(toolName + " is patched");
        
    }

    public static int sum(int n, int m) { 
        return n + m;
    }
    
}
