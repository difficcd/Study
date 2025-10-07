

// 01~03 (3 ~ 21p)
// 이미 아는 내용은 skip

import java.util.Scanner;

public class Main { 
    public static void main(String[] args) {

        // 01 반복문 & 02 continue/break
        
            // for : 반복 횟수 명확, while/do while : 조건에 따라 반복
            // do while 은 반복조건 나중에 따질때 적합

            // for (초기문; 조건식; 반복 후 작업) 작업문
            //        조건식 비어있으면 항상 true

            for(int i=0; i<10; i++, System.out.println(i)){
                // out : 0~9 vs 1~10
                if(i == 5) continue; // 5이면 다음 반복으로 skip
                System.out.print("No continue  "); // 6(5)만 skip
            } System.out.println();
        

            int i = 0;
            while(i<10){
                if(i == 8) {i++; continue;} 
                    // 8 만 출력 안 됨
                    // i++ 하지 않으면 영원히 반복
                System.out.print(i); 
                i++; 
            } System.out.println();

        
            int j=0;
            do{
                if(j == 8) {j++; continue;}
                System.out.print(j);
                j++;
            }while(j<10); // 세미콜론 필요
            System.out.println();


            // break 는 for, while 자체를 빠져나가는 것 (설명skip)

            
    // 03 배열

        // index 와 index 에 대응하는 data로 이루어진 연속적 데이터구조
        // 배열 이름 == 배열에 대한 레퍼런스 변수(배열에 대한 주소값 갖는변수)

        int arr1 [];        // 배열 선언 : 배열 레퍼런스 변수 선언  !!크기지정시 오류
        arr1 = new int[5];  // 배열 생성 : 배열의 저장 공간 할당 !!사용은 무조건 생성후
        for(int k=0; k<5; k++){
            arr1[k] = k+1;
            System.out.print(arr1[k]);
        } System.out.println();

        int arr2 [] = {1,2,3,4,5};
        int arr3 [] = arr2; // 레퍼런스 치환 : 같은 배열 참조
        for(int k=0; k<arr3.length; k++) // ! arr.length 사용 가능 (배열크기)
            System.out.print(arr3[k]);
        // ** 자바는 레퍼런스 변수/배열 공간이 분리되어 있기에
        //    다수의 레퍼런스 변수가 하나의 배열 공간 가르키는 배열 공유가 쉬움
        
        //  for-each 문 : arr 안의 원소에 대해 접근하는 문법 (C++)
        for (int k : arr2)
            System.out.print(k);
    }
}
