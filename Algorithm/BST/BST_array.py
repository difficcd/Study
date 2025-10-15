
# 배열 기반 이진 탐색 트리 (BST)

MAX_SIZE = 100
NIL = None
tree = [NIL] * MAX_SIZE


# (1) 탐색 연산
def search_bst(key, idx=1):
    if idx >= MAX_SIZE or tree[idx] == NIL:
        return NIL
    if tree[idx] == key:
        return idx
    elif key < tree[idx]:
        return search_bst(key, idx * 2)
    else:
        return search_bst(key, idx * 2 + 1)


# (2) 삽입 연산
def insert_bst(key, idx=1):
    if idx >= MAX_SIZE:
        return
    if tree[idx] == NIL:
        tree[idx] = key
        return

    if key < tree[idx]:
        insert_bst(key, idx * 2)
    elif key > tree[idx]:
        insert_bst(key, idx * 2 + 1)
    else:
        # 동일 키 허용하지 않음
        return


# (3) 삭제 연산
def find_min_index(idx):
    while idx * 2 < MAX_SIZE and tree[idx * 2] != NIL:
        idx = idx * 2
    return idx


def delete_bst(key, idx=1):
    target = search_bst(key, idx)
    if target is None:
        print(f"Key {key} not found.")
        return

    left = target * 2
    right = target * 2 + 1

    # Case 1: 단말 노드
    if (left >= MAX_SIZE or tree[left] == NIL) and (right >= MAX_SIZE or tree[right] == NIL):
        tree[target] = NIL

    # Case 2: 왼쪽 자식만 있는 경우
    elif (tree[left] != NIL and (right >= MAX_SIZE or tree[right] == NIL)):
        tree[target] = tree[left]
        tree[left] = NIL

    # Case 2: 오른쪽 자식만 있는 경우
    elif ((left >= MAX_SIZE or tree[left] == NIL) and tree[right] != NIL):
        tree[target] = tree[right]
        tree[right] = NIL

    # Case 3: 양쪽 자식 모두 있는 경우
    else:
        succ = find_min_index(right)
        if succ is not None:
            tree[target] = tree[succ]
            delete_bst(tree[succ], right)


# (4) 출력 함수 (트리 상태 확인)
def print_tree():
    print([tree[i] for i in range(1, 32)])  # 깊이 5까지만 표시


# 실행 예시
if __name__ == "__main__":
    insert_bst(50)
    insert_bst(30)
    insert_bst(70)
    insert_bst(20)
    insert_bst(40)
    insert_bst(60)
    insert_bst(80)

    print("초기 트리:")
    print_tree()

    print("\n탐색: 40 →", search_bst(40))
    print("탐색: 90 →", search_bst(90))

    print("\n삭제: 20")
    delete_bst(20)
    print_tree()

    print("\n삭제: 30")
    delete_bst(30)
    print_tree()

    print("\n삭제: 50 (루트)")
    delete_bst(50)
    print_tree()
