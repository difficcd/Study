

// 01~03 (3 ~ 21p)

import java.util.Scanner;

public class Main { 
    public static void main(String[] args) {
      
            for(int i=0; i<10; i++, System.out.println(i)){
                if(i == 5) continue;
                System.out.print("No continue  ");
            } System.out.println();
        

            int i = 0;
            while(i<10){
                if(i == 8) {i++; continue;} 
                System.out.print(i); 
                i++; 
            } System.out.println();

        
            int j=0;
            do{
                if(j == 8) {j++; continue;}
                System.out.print(j);
                j++;
            }while(j<10);
            System.out.println();

  
        int arr1 [];        
        arr1 = new int[5]; 
        for(int k=0; k<5; k++){
            arr1[k] = k+1;
            System.out.print(arr1[k]);
        } System.out.println();

        int arr2 [] = {1,2,3,4,5};
        int arr3 [] = arr2; 
        for(int k=0; k<arr3.length; k++) 
            System.out.print(arr3[k]);
        
        for (int k : arr2)
            System.out.print(k);
    }
}
