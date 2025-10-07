

// 04~09 (26 ~ 45p)

class Shared
 { public static final double PI = 3.14; }

class Circle{ 
    int radius;
    double area;

    public Circle(int r){ 
        if (r <= 0) {
            this.radius = 1;  
            return;           
        }
        this.radius = r;
        this.radius = r;
        this.area = Shared.PI * r * r;
    }

    public double getArea(){ 
        return this.area;
    }

     void increase(int m) { m = m + 1; }
     void increase(Circle m) { m.radius++; }
}


public class Main { 
    
     public static int abs(int a) {
         if(a < 0) a *= -1;
         return a;
     }   
    
     public static void main(String[] args) {
        Circle [] c;       
        c = new Circle[5];  

        for(int i=0; i<c.length; i++)   
            c[i] = new Circle(i);     

        for(int i=0; i<c.length; i++)
            System.out.println( (int)c[i].getArea() );

            int n = 10;
            c[0].increase(n); 
            System.out.println("\n" + n);
        
            Circle m = new Circle(10);
            m.increase(m);
            System.out.println(m.radius);
        
        Circle a, b;
        a = new Circle(1);
        b = new Circle(2);
        b = a;
        System.gc(); 

        System.out.println(abs(-5)); 

    }

    public class FinalFieldClass {
        final int ROWS = 10;   
        void f() {
            int[] intArray = new int [ROWS]; 
            // ROWS = 30;        
        }
    }
}
