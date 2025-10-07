

// 04~07 (22p ~ 33p)

public class Main { 
    public static void main(String[] args) {

        
            int arr1[][] = new int[5][5];
            for(int i=0; i<5; i++)
                for(int j=0; j<5; j++)
                    arr1[i][j] = (i+1) * (i+1);

            int arr2[][] = {{0,1},{2,3}}; 
       

            System.out.println(arr2.length);
            System.out.println(arr2[0].length + " " + arr2[1].length);

            int i[][];
            i = new int[4][];
            i[0] = new int[1]; 
            i[1] = new int[2]; 
            i[2] = new int[3];
            i[3] = new int[4];

            System.out.println("\n" + i.length);
            System.out.println(i[0].length); 
            System.out.println(i[1].length); 
            System.out.println(i[2].length); 
            System.out.println(i[3].length); 


            int[] return_arr = returntest();
            for(int k=0; k<return_arr.length; k++)
                System.out.print(return_arr[k] + " ");

                
            try {
                int result = 10 / 0;
                System.out.println("\n\n결과: " + result);
            } catch (Exception e) { 
                System.out.println("\n\n예외 발생: " + e.getMessage());
            } finally {
                System.out.println("finally 블록 실행");
            }

            
    }

     public static int[] returntest(){
         int temp[] = new int[4];
         for(int i=0; i<temp.length; i++)
             temp[i] = i+1;
         System.out.println();
         return temp;

     }
    
}
