
// extends 상속받기

class Person {
    private int weight;
    int age;
    protected int height;
    public String name;
    public void setWeight(int weight) {
        this.weight= weight;
    }
    public int getWeight() {
        return weight;
    }
}

class Student extends Person {
    public void set() {
        age = 30; // 디폴트 멤버 접근 가능
        name = "홍길동"; // public 멤버 접근 가능
        height = 175; // protected 멤버 접근 가능
        // weight = 99; // 오류. private 접근 불가
        setWeight(99); 
        // private 멤버 weight은 setWeight()으로 간접 접근
    }
}

class Point {
    private int x, y; 
    public Point() {
        this.x= this.y= 0;
    }
    public Point(int x, int y) {
        this.x= x; this.y= y;
    }
    public void showPoint() { 
        System.out.println("(" + x + "," + y + ")");
    }
}

class ColorPoint extends Point {
    private String color; 
    public ColorPoint(int x, int y, String color) {
        super(x, y); // 부모 클래스 생성자 Point(x, y) 호출
        this.color= color;
    }
    public void showColorPoint() { 
        System.out.print(color);
        showPoint(); // 부모 클래스 showPoint() 호출
    }
}


class Per_son { }
class Stu_dent extends Per_son { }
class Researcher extends Per_son { }
class Professor extends Researcher { }

public class Main{
    static void print(Per_son p) {
        if(p instanceof Per_son)
        System.out.print("Person ");
        if(p instanceof Stu_dent)
        System.out.print("Student ");
        if(p instanceof Researcher)
        System.out.print("Researcher ");
        if(p instanceof Professor)
        System.out.print("Professor ");
        System.out.println();
    }
    
    public static void main(String[] args) {
        Student s = new Student();
        s.set();


        ColorPoint cp = new ColorPoint(5, 6, "blue");
        cp.showColorPoint();

        System.out.print("Student() ->\t"); print(new Stu_dent());
        System.out.print("Researcher() ->\t"); print(new Researcher());
        System.out.print("Professor() ->\t"); print(new Professor());
    }
}

/*
new Student() ->	
    Person Student  >> Person의 자식: Person, Student 
new Researcher() ->
    Person Researcher  >> Person의 자식: Person, Researcher
new Professor() ->
    Person Researcher Professor >> Researche 자식 : 3개 모두 해당
*/

