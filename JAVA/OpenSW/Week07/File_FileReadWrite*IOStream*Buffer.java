
import java.io.*;
import java.util.*;

public class Main {
    public static void main(String[] args) {
        //# 파일 입출력 : FileReader
            FileReader fin = null;
            try {
                fin = new FileReader("c:\\windows\\system.ini");
                int c;
                while ((c = fin.read()) != -1) {
                    System.out.print((char)c);
                }
                fin.close();
            }
            catch (IOException e) {
                System.out.println("입출력오류");
            }

        //# 파일 입출력 : InputStreamReader *FileInputStream
            InputStreamReader in = null;
            FileInputStream finS = null;
            try {
                finS = new FileInputStream("c:\\hangul.txt");
                in = new InputStreamReader(finS, "MS949");
                int c;
                System.out.println("인코딩 문자 집합은 " + in.getEncoding());
                while ((c = in.read()) != -1) {
                    System.out.print((char)c);
                }
                in.close();
                finS.close();
            } catch (IOException e) {
                System.out.println("입출력 오류");
            }

        //# 문자 스트림 : FileWriter 로 텍스트 파일 쓰기
            Scanner scanner= new Scanner(System.in);
            FileWriter fout= null;
            int c;
            try {
                fout= new FileWriter("c:\\test.txt");
                while(true) {
                    String line = scanner.nextLine();
                    if(line.length() == 0)
                        break;
                    fout.write(line, 0, line.length());
                    fout.write("\r\n", 0, 2);
                }
                fout.close();
            } catch (IOException e) {
                System.out.println("입출력 오류");
            }
            scanner.close();
            

        
        //# 문자 스트림 : FileOutputStream 로 바이너리 파일 쓰기
            byte b[] = {7,51,3,4,-1,24};
            try {
                FileOutputStream fout_=
                        new FileOutputStream("c:\\Temp\\test.out");
                for(int i=0; i<b.length; i++)
                    fout_.write(b[i]);
                fout_.close();
                } catch(IOException e) {
                    System.out.println( "c:\\Temp\\test.out" +
                            "에 저장할 수 없습니다. " +
                            "경로명을 확인해 주세요");
                    return;
                }
            System.out.println("c:\\Temp\\test.out을 저장하였습니다.");
        
        //# 문자 스트림 : FileOutputStream 로 바이너리 파일 읽기
            byte b_[] = new byte [6]; // 비어있는byte 배열
            try {
                FileInputStream fin_ = new FileInputStream("c:\\Temp\\test.out");
                int n=0, c_=0;
                while((c = fin.read())!= -1) {
                    b_[n] = (byte)c_;
                    n++; }
                System.out.println("c:\\Temp\\test.out에서 읽은 배열을 출력합니다.");
                for(int i=0; i<b_.length; i++) System.out.print(b[i] + " ");
                System.out.println();
                fin_.close();
            } catch(IOException e) {
                System.out.println( "c:\\Temp\\test.out에서 읽지 못했습니다. " +
                        "경로명을 체크해보세요");
            }
        
        //# 버퍼 스트림 생성/활용 
            FileReader _fin = null;
            int _c;
            try {
                _fin = new FileReader("c:\\Temp\\test.out");
                BufferedOutputStream out = new // 버퍼 스트림
                        BufferedOutputStream(System.out, 5);
                while ((_c = _fin.read()) != -1) {
                    out.write(_c);
                }
                new Scanner(System.in).nextLine();
                out.flush();
                _fin.close();
                out.close();
            } catch (IOException e) {  e.printStackTrace();  }
            
        //# 파일 타입 알아내고 디렉터리 이름 변경
            File f1 = new File("c:\\windows\\system.ini");
            System.out.println(f1.getPath() + ", " + f1.getParent() 
                               + ", " + f1.getName());
    
            String res = "";
            if (f1.isFile()) res = "파일";
            else if (f1.isDirectory()) res = "디렉토리";
    
            System.out.println(f1.getPath() + "은 " + res + "입니다.");
    
            File f2 = new File("c:\\Temp\\java_sample");
            if (!f2.exists()) {
                f2.mkdir(); // 존재하지 않으면 디렉토리 생성
            }
    
            listDirectory(new File("c:\\Temp")); // 아래 함수 정의 확인
            f2.renameTo(new File("c:\\Temp\\javasample")); 
            listDirectory(new File("c:\\Temp"));
    
    }

    public static void listDirectory(File dir) {
        System.out.println("-----" + dir.getPath() + "의 서브리스트입니다.-----");
        File[] subFiles = dir.listFiles();
        for (int i = 0; i < subFiles.length; i++) {
            File f = subFiles[i];
            long t = f.lastModified();
            System.out.print(f.getName());
            System.out.print("\t파일 크기: " + f.length());
            System.out.printf("\t수정한 시간: %tb %td %ta %tT\n", t, t, t, t);
        }
    }
}
