public class Practice08 {
    public static class ArrayParameterEx{
        static void replaceSpace(char a[]) { // 배열a의공백문자를‘,’ 로변경
            for (int i= 0; i< a.length; i++)
                if (a[i] == ' ')  // 공백문자를‘,’로변경
            a[i] = ',';
        }
        static void printCharArray(char a[]) { // 배열a의문자들을화면에출력
            for (int i= 0; i< a.length; i++)
                System.out.print(a[i]); // 배열원소문자출력
            System.out.println(); // 배열원소모두출력후줄바꿈
        }
        public static void main (String args[]) {
            char c[] = {'T', 'h', 'i', 's', ' ', 'i', 's', ' ', 'a', ' ', 'p', 'e', 'n', 'c', 'i', 'l', '.'};
            printCharArray(c); // 원래배열원소출력
            replaceSpace(c); // 공백문자바꾸기
            printCharArray(c); // 수정된배열원소출력
        }
    }

}
