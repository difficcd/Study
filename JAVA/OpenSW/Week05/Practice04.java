public class Practice04 {
    public static class Book {
        String title;
        String author;
        public Book(String t) { // 생성자1
            title = t; author = "작자미상";
        }
        public Book(String t, String a) { // 생성자2
            title = t; author = a;
        }
        public static void main(String [] args) {
            Book littlePrince= new Book("어린왕자", "생텍쥐페리"); // 생성자2호출
            Book loveStory= new Book("춘향전"); // 생성자1 호출
            System.out.println(littlePrince.title+ " " + littlePrince.author);
            System.out.println(loveStory.title+ " " + loveStory.author);
        }
    }
}
