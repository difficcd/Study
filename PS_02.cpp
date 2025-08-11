// 트리 구현 연습!
// C++ 으로 BST LVR순회 구현하기

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
