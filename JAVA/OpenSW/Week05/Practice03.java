public class Practice03 {
    public static class Circle {
        int radius;
        String name;
        public Circle() { // 매개변수 없는 생성자
            radius = 1; name = ""; // 필드 초기화. radius 의초기값은1
        }
        public Circle(int r, String n) { // 매개변수 가진 생성자
            radius = r; name = n; // 매개변수로 필드 초기화
        }
        public double getArea() {
            return 3.14*radius*radius;
        }
        public static void main(String[] args) {
            Circle pizza = new Circle(10, "자바피자");
            // Circle 객체생성, 반지름10
            double area = pizza.getArea();
            System.out.println(pizza.name + "의 면적은 " + area);

            Circle donut = new Circle();
            // Circle 객체생성, 반지름1
            donut.name = "도넛피자"; // 이름변경
            area = donut.getArea();
            System.out.println(donut.name + "의 면적은 " + area);
        }
    }

}
