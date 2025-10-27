

import java.util.*;


class GStack<T> { // 제네릭 스택 선언
    int tos;
    Object [] stck;
    public GStack() {
        tos= 0;
        stck= new Object [10];
    }
    public void push(T item) {
        if(tos== 10)
            return;
        stck[tos] = item;
        tos++;
    }
    public T pop() {
        if(tos== 0)
            return null;
        tos--;
        return (T)stck[tos];
    }
}

public class Main {
    public static void main(String[] args) {
        //# HashMap<K,V>
            // 영어 단어와 한글 단어의 쌍을 저장하는 HashMap 컬렉션 생성
            HashMap<String, String> dic = new HashMap<String, String>();

            // 3개의 (key, value) 쌍을 dic에 저장
            dic.put("baby", "아기");
            dic.put("love", "사랑");
            dic.put("apple", "사과");
    
            // 영어 단어를 입력받아 한글 단어 검색, "exit" 입력 시 종료
            Scanner scanner = new Scanner(System.in);
            while (true) {
                System.out.print("찾고 싶은 단어는? ");
                String eng = scanner.next();
    
                // "exit" 입력 시 프로그램 종료
                if (eng.equals("exit")) {
                    System.out.println("종료합니다...");
                    break;
                }
    
                // 해시맵에서 '키' eng의 '값' kor 검색
                String kor = dic.get(eng);
                if (kor == null)
                    System.out.println(eng + "는 없는 단어입니다.");
                else
                    System.out.println(kor);
            }
            scanner.close();

        //# HashMap (2)
        // 사용자 이름과 점수를 기록하는 HashMap 컬렉션 생성
            HashMap<String, Integer> javaScore = new HashMap<String, Integer>();
            javaScore.put("김성동", 97);
            javaScore.put("황기태", 88);
            javaScore.put("김남윤", 98);
    
            System.out.println("HashMap의 요소 개수: " + javaScore.size());
    
            // 모든 사람의 점수 출력
            // Set & KeySet() : JavaScore 에 들어있는 모든 (Key,value) 쌍 출력
            Set<String> keys = javaScore.keySet();
    
            // key 문자열을 순서대로 접근할 수 있는 Iterator 리턴
            Iterator<String> it = keys.iterator();
            while (it.hasNext()) {
                String name = it.next();
                int score = javaScore.get(name);
                System.out.println(name + " : " + score);
            }

        //# Collections Class 활용
        // 문자열 정렬, 반대로 정렬, 이진 검색 가능 (아래 printList도 참조)
            LinkedList<String> myList= new LinkedList<String>();
            myList.add("트랜스포머"); myList.add("스타워즈"); myList.add("매트릭스");
            myList.add(0,"터미네이터"); myList.add(2,"아바타"); // index 삽입
    
            Collections.sort(myList);       // 요소 정렬
            printList(myList);              // 정렬된 요소 출력

            Collections.reverse(myList);    // 요소의 순서를 반대로
            printList(myList);              // 요소출력
    
            int index = Collections.binarySearch(myList, "아바타") + 1;
            System.out.println("아바타는 " + index + "번째 요소입니다.");


        //# String, Integer 스택 제네릭 클래스로 선언하기 (GStack 구조참조)
            GStack<String> stringStack= new GStack<String>();
            stringStack.push("seoul");
            stringStack.push("busan");
            stringStack.push("LA");
    
            for(int n=0; n<3; n++)
                System.out.println(stringStack.pop());
    
            GStack<Integer> intStack= new GStack<Integer>();
            intStack.push(1);
            intStack.push(3);
            intStack.push(5);
    
            for(int n=0; n<3; n++)
                System.out.println(intStack.pop());
            
    }


    static void printList(LinkedList<String> l) {
        Iterator<String> iterator = l.iterator();
        while (iterator.hasNext()) {
            String e = iterator.next();
            String separator;
            if (iterator.hasNext())
                separator = " -> ";
            else
                separator = "\n";
            System.out.print(e + separator);
        }
    }
}
