

# 이진 탐색 트리를 위한 노드 클래스
class BSTNode:
    def __init__(self, key, value=None):
        self.key = key           # 노드의 키
        self.value = value       # 키에 대응하는 값
        self.left = None         # 왼쪽 자식 노드
        self.right = None        # 오른쪽 자식 노드


# 이진 탐색 트리에서의 탐색 연산
def search_bst(n, key):
    if n is None:                 # 공백 트리이거나 탐색 실패
        return None
    elif key == n.key:            # 키를 찾은 경우
        return n
    elif key < n.key:             # 왼쪽 서브트리 탐색
        return search_bst(n.left, key)
    else:                         # 오른쪽 서브트리 탐색
        return search_bst(n.right, key)



# 이진 탐색 트리에서의 삽입 연산
def insert_bst(root, node):
    if root is None:              # 공백 위치에 노드 삽입
        return node
    if node.key == root.key:      # 동일 키는 삽입하지 않음
        return root

    if node.key < root.key:       # 왼쪽 서브트리에 삽입
        root.left = insert_bst(root.left, node)
    else:                         # 오른쪽 서브트리에 삽입
        root.right = insert_bst(root.right, node)

    return root                   # 루트 반환 (갱신된 트리)


# 이진 탐색 트리에서의 삭제 연산
def delete_bst(root, key):
    if root is None:              # 공백 트리
        return None

    # 삭제할 노드 탐색
    if key < root.key:
        root.left = delete_bst(root.left, key)
    elif key > root.key:
        root.right = delete_bst(root.right, key)

    else:
        # case 1, 2: 자식이 하나 이하인 경우
        if root.left is None:
            return root.right
        elif root.right is None:
            return root.left

        # case 3: 자식이 둘 다 있는 경우
        succ = root.right          
            # 후계자 = 오른쪽 서브트리의 최소 노드 탐색
        while succ.left is not None:
            succ = succ.left

        # 후계자의 데이터 복사
        root.key = succ.key
        root.value = succ.value

        # 후계자 노드 삭제
        root.right = delete_bst(root.right, succ.key)

    return root



# 트리 순회 (테스트용)
def inorder(n):
    if n is not None:
        inorder(n.left)
        print(n.key, end=' ')
        inorder(n.right)


# 실행 예시
if __name__ == "__main__":
    keys = [35, 24, 55, 16, 31, 53, 67, 9, 43, 87]
    root = None

    for k in keys:
        root = insert_bst(root, BSTNode(k))

    print("중위 순회 결과 (정렬된 출력):")
    inorder(root)
    print("\n")

    # 노드 삭제 예시
    print("노드 55 삭제 후:")
    root = delete_bst(root, 55)
    inorder(root)
    print()
