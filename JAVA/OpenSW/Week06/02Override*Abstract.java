
// 메소드 오버라이딩
// 추상 클래스

//#오버라이딩(1)
class Shape { // 슈퍼 클래스
    public Shape next;
    public Shape() { next = null; }
    public void draw() {
        System.out.println("Shape");
    }
}
class Line extends Shape {
    public void draw() { // 메소드 오버라이딩
        System.out.println("Line");
    }
}
class Rect extends Shape {
    public void draw() { // 메소드 오버라이딩
        System.out.println("Rect");
    }
}
class Circle extends Shape {
    public void draw() { // 메소드 오버라이딩
        System.out.println("Circle");
    }
}



//#2 오버라이딩(2) : @Override
class Weapon {
    protected int fire() {
        return 1; // 무기는 기본 1명 살상
    }
}

class Cannon extends Weapon {
    @Override
    protected int fire() { // 오버라이딩
        return 10; // 대포는 한 번에 10명 살상
    }
}


// #3 추상 클래스
abstract class Calculator {
    public abstract int add(int a, int b); // 두 정수의 합 구하여 리턴
    public abstract int subtract(int a, int b); // 두 정수의 차 구하여 리턴
    public abstract double average(int[] a); // 배열 정수의 평균 리턴
}


public class Main extends Calculator{
    // #1,2
    static void paint(Shape p) {
        p.draw();
    }

    // #3 
    public int add(int a, int b) { // 추상메소드구현
        return a + b; }
    public int subtract(int a, int b) { // 추상메소드구현
        return a -b; }
    public double average(int[] a) { // 추상메소드구현
        double sum = 0;
        for(int i= 0; i<a.length; i++)
            sum += a[i];
        return sum/a.length; }

    
    public static void main(String[] args) {
        // #1 같은 메소드 이름/기능, 다른 구현
        Line line= new Line();
        paint(line);
        paint(new Shape());
        paint(new Line());
        paint(new Rect());
        paint(new Circle());

        // #2 @Override
        Weapon weapon;
        weapon = new Weapon();
        System.out.println("기본무기: " + weapon.fire()); // 1로 출력
        weapon = new Cannon();
        System.out.println("대포: " + weapon.fire()); // 10으로 잘 출력됨

        // #3 추상 메소드
        Main c = new Main();
            System.out.println(c.add(2,3)); // 5
            System.out.println(c.subtract(2,3)); // -1
            System.out.println(c.average(new int [] { 2,3,4 })); // 3.0
    }
}
