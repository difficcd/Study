// 트리 구현 연습!
// C++ 으로 BST LVR순회 구현하기
// https://www.acmicpc.net/problem/24479 풀기위해 ㄱㄱ

#include <iostream>
using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;

    Node(int value) : data(value), left(nullptr), right(nullptr) {}
    // C 에서 typedef Node 해서 Node A.. 이런 식으로 생성하던 걸
    // C++ 에서는 Node(..) 로 생성함 (생성자 문법) >> 자세한 건 #구현 참조
};

void insert(Node*& root, int value) { 
    if (!root) {
        root = new Node(value);
        return;
    }
    if (value < root->data) // BST
        insert(root->left, value);
    else
        insert(root->right, value);
}

void inorder(Node* root) { // 중위 순회
    if (!root) return;
    inorder(root->left);
    cout << root->data << " ";
    inorder(root->right);
}

int main() {
    Node* root = nullptr;
    insert(root, 5);
    insert(root, 3);
    insert(root, 7);
    insert(root, 4);

    inorder(root); // 3 4 5 7
}


// 1629 번 곱셈 : 분할정복 복습과 오버플로 처리
#include <iostream>
using namespace std;

int C;

long long power(long long A, long long B){
  
  if(B == 1) return A % C;
  else {
    long long temp = power(A, B/2);
    temp = (temp * temp) % C;
    
    if (B % 2 == 0) return temp;
    else return (temp * (A % C)) % C;
  }
}


int main(){
  long long A, B;

  cin >> A >> B >> C;
  cout << power(A,B);
  
  
}


// 10434번 행복한 소수
// 소수상근수 가볍 복습

import java.util.Scanner;
import java.util.HashSet;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        
        int P= sc.nextInt();
        
        for(int i=1; i<=P; i++){
            int number= sc.nextInt();
            System.out.print(number);
            System.out.print(" ");
            
            int N= sc.nextInt();
            System.out.print(N);
            System.out.print(" ");
            
            if(N != 1) happy_num(N);
            else System.out.println("NO");
        }

        sc.close();

    }

    public static void happy_num(int N){
        HashSet<Integer> list = new HashSet<Integer>();

        String str = String.valueOf(N);
        list.add(N);

        while(true) {
            int sum = 0;
            for(int j=0; j<str.length(); j++){
                int num = str.charAt(j) - '0';
                sum += num * num;
            }
            str = String.valueOf(sum);

            if (sum == 1 && is_prime(N)) { // 먼저 1 체크
                System.out.println("YES");
                break;
            }
            if (list.contains(sum)) { // 방문했으면 무한루프 방지
                System.out.println("NO");
                break;
            }
            list.add(sum);
        }
    }

    
    public static boolean is_prime(int N){
        boolean prime =true;

        if (N != 1){
            for (int i=2; i*i <=N; i++){
                int tmp = N;
                if(tmp % i == 0)
                    prime = false;
            }
        }
        else prime = false;

        return prime;
    }
    

}

