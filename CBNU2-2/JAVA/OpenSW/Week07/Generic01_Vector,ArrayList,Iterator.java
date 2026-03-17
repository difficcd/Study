
import java.util.*;

class Point {
    private int x, y;
    public Point(int x, int y) {
        this.x= x; this.y= y;
    }
    public String toString() {
        return "(" + x + "," + y + ")";
    }
}


public class Main {
    public static void main(String[] args) {

        //# 컬렉션과 자동 박싱/언박싱 (1) - Vector<>
            // 정수값만 다루는 제네릭 벡터 생성
            Vector<Integer> v = new Vector<Integer>();
            v.add(5); // 5 삽입
            v.add(4); // 4 삽입
            v.add(-1); // -1 삽입
            v.add(2, 100); // 4와 -1 사이에 정수 100 삽입
            
            System.out.println("벡터내의 요소 객체 수: " + v.size());
            System.out.println("벡터의 현재용량: " + v.capacity());
        
            for(int i=0; i<v.size(); i++) { // 모든 요소 정수 출력하기
                int n = v.get(i);
                System.out.println(n);
            }
            int sum = 0;
            for(int i=0; i<v.size(); i++) { // 벡터속의 모든 정수 더하기
                int n = v.elementAt(i);
                sum += n;
            }
            System.out.println("벡터에 있는 정수합: " + sum);


        //# 컬렉션과 자동 박싱/언박싱 (2) - Vector<>
            Vector<Point> v_ = new Vector<Point>();
            // Point 객체를 요소로만 가지는 벡터 생성
            v_.add(new Point(2, 3)); // 3 개의Point 객체삽입
            v_.add(new Point(-5, 20));
            v_.add(new Point(30, -8));
            v_.remove(1); // 인덱스 1의 Point(-5, 20) 객체 삭제
    
            for(int i=0; i<v_.size(); i++) { // 벡터에 있는 Point 객체 모두검색 출력력
                Point p = v_.get(i); // 벡터에서 i번째 Point 객체 얻어내기
                System.out.println(p); // p.toString()을 이용하여 객체p 출력
            }

        
        //# ArrayList<E> 사용예제
            ArrayList<String> a = new ArrayList<String>();
            // 문자열만 삽입 가능한 ArrayList 컬렉션 생성
            // 위 코드 대신 var a = new ArrayList<String>(); 도 가능
    
            // 키보드로부터 4개의 이름 입력받아 ArrayList 삽입
            Scanner scanner = new Scanner(System.in);
            for (int i = 0; i < 4; i++) {
                System.out.print("이름을 입력하세요 >> ");
                String s = scanner.next(); // 키보드로부터 이름입력
                a.add(s); // ArrayList 컬렉션에 삽입
            }
            // ArrayList에 들어있는 모든 이름 출력
            for (int i = 0; i < a.size(); i++) {
                // ArrayList의 i번째 문자열 얻어오기
                String name = a.get(i);
                System.out.print(name + " ");
            }
            // 가장 긴 이름 출력
            int longestIndex = 0;
            for (int i = 1; i < a.size(); i++) {
                if (a.get(longestIndex).length() < a.get(i).length())
                    longestIndex = i;
            }
            System.out.println("\n가장 긴 이름은: " + a.get(longestIndex));
            scanner.close();


    
        //# 컬렉션 순차 검색 : iterator 활용하기
            Vector<Integer> _v = new Vector<Integer>();
            _v.add(5); // 5 삽입
            _v.add(4); // 4 삽입
            _v.add(-1); // -1 삽입
            _v.add(2, 100); // 4와 -1 사이에 정수 100 삽입
    
            // Iterator를 이용한 모든 정수 출력
            Iterator<Integer> it = _v.iterator();
            while(it.hasNext()) {
                int n = it.next();
                System.out.println(n);
            }
    
            // Iterator 이용하여 모든 정수 더하기
            int _sum = 0;
            it = _v.iterator(); // Iterator 객체 얻기
            while(it.hasNext()) {
                int n = it.next();
                _sum += n;
            }
            System.out.println("벡터에 있는 정수 합: " + _sum);
    }
}
