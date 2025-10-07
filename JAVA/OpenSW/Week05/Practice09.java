
public class Practice09 {
    public static class GarbageEx
    {
        public static void main(String[]
                                        args
        ) {
            String a = new String("Good");
            String b = new String("Bad");
            String c = new String("Normal");
            String d, e;
            a = null; // 할당되었던 값이 제거되었으므로 가비지 발생 O
            d = c;
            c = null; // d가 참조중이므로 가비지 발생 X
        }
    }
}
