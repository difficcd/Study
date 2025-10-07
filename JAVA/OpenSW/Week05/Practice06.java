public class Practice06 {

    static class Circle { // static으로 변경
        int radius;
        public Circle(int radius) {
            this.radius = radius;
        }
        public double getArea() {
            return 3.14 * radius * radius;
        }
    }

    public static class CircleArray {
        public static void main(String[] args) {
            Circle[] c = new Circle[5]; // 5개의 Circle 레퍼런스 배열 생성
            for(int i = 0; i < c.length; i++)
                c[i] = new Circle(i); // 각 Circle 객체 생성
            for(int i = 0; i < c.length; i++)
                System.out.print((int)(c[i].getArea()) + " ");
        }
    }
}
