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
