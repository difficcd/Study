from sklearn import datasets
import numpy as np

# 1. 데이터 분할 함수: 특정 조건(value)에 따라 데이터를 왼쪽/오른쪽으로 나눔
def test_split(index, value, dataset):
    left, right = list(), list()
    for row in dataset:
        if row[index] < value:
            left.append(row)
        else:
            right.append(row)
    return left, right

# 2. 지니 지수(Gini Index) 계산: 데이터의 불순도를 측정 (낮을수록 좋음)
def gini_index(groups, classes):
    n_instances = float(sum([len(group) for group in groups]))
    gini = 0.0
    for group in groups:
        size = float(len(group))
        if size == 0:
            continue
        score = 0.0
        for class_val in classes:
            p = [row[-1] for row in group].count(class_val) / size
            score += p * p
        gini += (1.0 - score) * (size / n_instances)
    return gini

# 3. 최적의 분할 지점 찾기: 지니 지수가 가장 낮은 조건(index, value)을 탐색
def get_split(dataset):
    class_values = list(set(row[-1] for row in dataset))
    b_index, b_value, b_score, b_groups = 999, 999, 999, None
    for index in range(len(dataset[0])-1):
        for row in dataset:
            groups = test_split(index, row[index], dataset)
            gini = gini_index(groups, class_values)
            if gini < b_score:
                b_index, b_value, b_score, b_groups = index, row[index], gini, groups
    return {'index':b_index, 'value':b_value, 'groups':b_groups}

# 4. 터미널 노드(잎사귀) 생성: 가장 빈도수가 높은 클래스를 결과값으로 반환
def to_terminal(group):
    outcomes = [row[-1] for row in group]
    return max(set(outcomes), key=outcomes.count)

# 5. 재귀적으로 트리 가지치기 (핵심 로직)
def split(node, max_depth, min_size, depth):
    left, right = node['groups']
    del(node['groups'])
    # 데이터가 한쪽으로 쏠렸을 때
    if not left or not right:
        node['left'] = node['right'] = to_terminal(left + right)
        return
    # 최대 깊이에 도달했을 때
    if depth >= max_depth:
        node['left'], node['right'] = to_terminal(left), to_terminal(right)
        return
    # 왼쪽 가지 생성
    if len(left) <= min_size:
        node['left'] = to_terminal(left)
    else:
        node['left'] = get_split(left)
        split(node['left'], max_depth, min_size, depth+1)
    # 오른쪽 가지 생성
    if len(right) <= min_size:
        node['right'] = to_terminal(right)
    else:
        node['right'] = get_split(right)
        split(node['right'], max_depth, min_size, depth+1)

# 6. 트리 빌드 시작 함수
def build_tree(train, max_depth, min_size):
    root = get_split(train)
    split(root, max_depth, min_size, 1)
    return root

# 7. 트리 구조 출력 함수
def print_tree(node, depth=0):
    if isinstance(node, dict):
        print('%s[X%d < %.3f]' % ((depth*' ', (node['index']+1), node['value'])))
        print_tree(node['left'], depth+1)
        print_tree(node['right'], depth+1)
    else:
        print('%s[%s]' % ((depth*' ', node)))

# --- 실행 부분 ---
iris = datasets.load_iris()
dataset = np.c_[iris.data, iris.target]

tree = build_tree(dataset, 3, 1)
print_tree(tree)