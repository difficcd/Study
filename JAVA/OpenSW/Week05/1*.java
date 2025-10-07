

// 01~03 (3 ~ 26p)

class Circle{     
    int radius;
    double area;
    
    public Circle(){      
        this(0);            
    }
    
    public Circle(int r){ 
        if (r <= 0) {
            this.radius = 1;  
            return;           
        }
        this.radius = r;
        this.radius = r;
        this.area = 3.14 * r * r;
    }

    
    public double getArea(){
        this.area = 3.14 * 
            this.radius *
            this.radius;
        return this.area;
    }
    
}



public class Main { 
    public static void main(String[] args) {
        Circle donut = new Circle(); 
        donut.radius = 5;
        System.out.println(donut.getArea());


        Circle pizza = new Circle(4); 
        System.out.println(pizza.getArea());
        System.out.println();

        
        Circle temp;  temp = pizza;
        pizza = donut; donut = temp; 
        System.out.println(donut.getArea());
        System.out.println(pizza.getArea());
    }
}
