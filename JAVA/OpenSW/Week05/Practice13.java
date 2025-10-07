import java.util.Scanner;

public class Practice13 {

    public static class WordGameApp {
        Player[] players;

        public WordGameApp() { }
        private void createPlayers(int size){
            players = new Player[size];
        }

        public Boolean checkSuccess(String lastWord, String newWord){
            int lastIndex = lastWord.length() - 1;
            char lastChar = lastWord.charAt(lastIndex);
            char firstChar = newWord.charAt(0);
            return lastChar == firstChar; // 이어지면 true, 아니면 false
        }

        public void run(){
            Scanner sc = new Scanner(System.in); // Scanner 한 번만 생성

            System.out.println("끝말잇기 게임을 시작합니다...");
            System.out.print("게임에 참가하는 인원은 몇명입니까>>");
            int player_size = sc.nextInt();

            createPlayers(player_size);

            String temp;
            for(int i = 0; i < player_size; i++){
                System.out.print("참가자의 이름을 입력하세요>>");
                temp = sc.next();
                players[i] = new Player(temp, " ");
            }

            System.out.println("시작하는 단어는 아버지입니다");
            String lastWord = "아버지";

            boolean check = true;
            while(check){
                for(int i = 0; i < player_size; i++) {
                    String newWord = players[i].getWordFromUser(sc);
                    check = checkSuccess(lastWord, newWord);

                    if(!check){
                        System.out.println(players[i].username + "이 졌습니다.");
                        break;
                    }
                    players[i].word = newWord;
                    lastWord = newWord; // lastWord 갱신
                }
                if(!check) break;
            }
            sc.close();
        }

        public static void main(String[] args) {
            WordGameApp app = new WordGameApp();
            app.run();
        }
    }

    public static class Player {
        String username;
        String word;

        public Player(String name, String word){
            this.username = name;
            this.word = word;
        }

        // Scanner를 외부에서 받아서 사용
        public String getWordFromUser(Scanner sc) {
            System.out.print(this.username + ">>");
            this.word = sc.next();
            return this.word;
        }
    }
}
