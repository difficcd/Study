
public class Practice10 {
    class Sample {
        public int a;
        private int b;
        int c; // 디폴트 접근 지정
    }
    public class AccessEx{
        public static void main(String[] args) {
            Sample aClass= new Sample();
            aClass.a= 10;
            aClass.b= 10;  // private 요소 접근, 오류
            aClass.c= 10;  // default 는 같은 패키지 내에서 접근가능
        }
}
