#### Pandas

# 1) 시리즈
import pandas as pd

sr = pd.Series([17000, 18000, 1000, 5000],
               index=["피자", "치킨", "콜라", "맥주"])
print('시리즈 출력 :')
print('-'*15)
print(sr)

print('시리즈의 값 : {}'.format(sr.values))
print('시리즈의 인덱스 : {}'.format(sr.index))

# (1) 각각 배열로 매핑해서 생성하기
values = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]
index = ['one', 'two', 'three']
columns = ['A', 'B', 'C']

df = pd.DataFrame(values, index=index, columns=columns)

print('데이터프레임 출력 :')
print('-'*18)
print(df, '\n')

    # .format(df.요소:index,values,columns.) 를 통해 걸러냄
print('데이터프레임의 인덱스 : {}'.format(df.index))
print('데이터프레임의 열이름: {}'.format(df.columns))
print('데이터프레임의 값 :')
print('-'*18)
print(df.values, '\n\n')


# (2) 리스트로 생성하기
data = [
    ['1000', 'Steve', 90.72],
    ['1001', 'James', 78.09],
    ['1002', 'Doyeon', 98.43],
    ['1003', 'Jane', 64.19],
    ['1004', 'Pilwoong', 81.30],
    ['1005', 'Tony', 99.14],
]

df = pd.DataFrame(data)
 # 데이터프레임 생성
print(df)
print('-'*25)

# 열이름 지정 가능
df = pd.DataFrame(data, columns=['학번', '이름', '점수'])
print(df)
print('-'*25)


# (3) 딕셔너리로 생성하기
data = {
    '학번' : ['1000', '1001', '1002', '1003', '1004', '1005'],
    '이름' : [ 'Steve', 'James', 'Doyeon', 'Jane', 'Pilwoong', 'Tony'],
    '점수': [90.72, 78.09, 98.43, 64.19, 81.30, 99.14]
    }

df = pd.DataFrame(data)
print(df)
print('-'*25)



## dataframe 조회 : head, tail, df[]

# 앞 부분을 3개만 보기
print('-'*25)
print(df.head(3))

# 뒷 부분을 3개만 보기
print('-'*25)
print(df.tail(3))

# '학번'에 해당되는 열을 보기
print('-'*25)
print(df['학번'])





#### Numpy
import numpy as np

# (0) ndarray 사용하기

# 1차원 배열
vec = np.array([1, 2, 3, 4, 5])
print(vec, '\n')

# 2차원 배열
mat = np.array([[10, 20, 30], [ 60, 70, 80]])
print(mat,'\n')

print('vec의 타입 :',type(vec))
print('vec의 축의 개수 :',vec.ndim) # 축의 개수 출력
print('vec의 크기(shape) :',vec.shape) # 크기 출력


print('\nmat의 타입 :',type(mat))
print('mat의 축의 개수 :',mat.ndim) # 축의 개수 출력
print('mat의 크기(shape) :',mat.shape) # 크기 출력



# (1) 여러가지 배열 생성 방법

# 모든 값이 0인 2x3 배열 생성.
zero_mat = np.zeros((2,3))
print(zero_mat, '\n')

# 모든 값이 1인 2x3 배열 생성.
one_mat = np.ones((2,3))
print(one_mat, '\n')

# 모든 값이 특정 상수인 배열 생성. 이 경우 7.
same_value_mat = np.full((2,2), 7)
print(same_value_mat, '\n')

# 대각선 값이 1이고 나머지 값이 0인 2차원 배열을 생성.
eye_mat = np.eye(3)
print(eye_mat, '\n')

# 임의의 값으로 채워진 배열 생성
random_mat = np.random.random((2,2))
print(random_mat)


# (2) np.arrange (배열 범위지정 생성, python range랑 비슷)

# 0부터 9까지
range_vec = np.arange(10)
print(range_vec, '\n')

# 1부터 9까지 +2씩 적용되는 범위
n = 2
range_n_step_vec = np.arange(1, 10, n)
print(range_n_step_vec, '\n')



# (3) np.reshape (배열 구조 바꾸기)
reshape_mat = np.array(np.arange(30)).reshape((5,6))
print(reshape_mat, '\n') # 0~29 요소가 5행 6열 행렬화됨



# (4) slice
mat = np.array([[1, 2, 3], [4, 5, 6]])
print(mat, '\n')

    # 첫번째 행 출력
slicing_mat = mat[0, :]
print(slicing_mat, '\n')

    # 두번째 열 출력
slicing_mat = mat[:, 1]
print(slicing_mat, '\n')



# (5) Numpy integer indexing

mat = np.array([[1, 2], [4, 5], [7, 8]])
print(mat, '\n')

    # 1행 0열의 원소
    # => 0부터 카운트하므로 두번째 행 첫번째 열의 원소.
print(mat[1, 0], '\n')

    # mat[[2행, 1행],[0열, 1열]]
    # 각 행과 열의 쌍을 매칭하면 2행 0열, 1행 1열의 두 개의 원소.
indexing_mat = mat[[2, 1],[0, 1]] # mat[위치1, 위치2..] => 새로운 배열
print(indexing_mat, '\n')

import numpy as np

# (6) 연산
x = np.array([1,2,3])
y = np.array([4,5,6])

# result = np.add(x, y)와 동일
result = x + y
print(result)

# result = np.subtract(x, y)와 동일
result = x - y
print(result)

# result = np.multiply(result, x)와 동일
result = result * x
print(result)

# result = np.divide(result, x)와 동일
result = result / x
print(result)

# result * x 방식은 벡터 * 벡터.
# 벡터*행렬 혹은 행렬곱은 dot() 사용해야 함
mat1 = np.array([[1,2],[3,4]])
mat2 = np.array([[5,6],[7,8]])
mat3 = np.dot(mat1, mat2)

print('\n', mat1, '\n\n', mat2, '\n\n', mat3)


#### Matplotlib
import matplotlib.pyplot as plt

plt.title('test')
plt.plot([1,2,3,4],[2,4,8,6]) #line 1
plt.plot([1.5,2.5,3.5,4.5],[3,5,8,10]) # line 2

# 축 label
plt.xlabel('hours')
plt.ylabel('score')

plt.legend(['A student', 'B student']) # 범례 삽입 (1, 2순)
plt.show()



#### data Splitting
from sklearn.model_selection import train_test_split

# (1) zip 함수의 기본
X, y = zip(['a', 1], ['b', 2], ['c', 3])
print('X 데이터 :',X)
print('y 데이터 :',y)
print('\n')

    # 리스트의 리스트 또는 행렬 또는 뒤에서 배울 개념인 2D 텐서.
sequences = [['a', 1], ['b', 2], ['c', 3]]
X, y = zip(*sequences)
print('X 데이터 :',X)
print('y 데이터 :',y)



# (2) dataframe 이용하여 분리하기
values = [['당신에게 드리는 마지막 혜택!', 1],
['내일 뵐 수 있을지 확인 부탁드...', 0],
['도연씨. 잘 지내시죠? 오랜만입...', 0],
['(광고) AI로 주가를 예측할 수 있다!', 1]]
columns = ['메일 본문', '스팸 메일 유무']

df = pd.DataFrame(values, columns=columns)

# column명으로 재추출도 가능
X = df['메일 본문']
y = df['스팸 메일 유무']

print('X 데이터 :',X.to_list())
print('y 데이터 :',y.to_list())


# (3) Numpy 이용하여 분리하기
np_array = np.arange(0,16).reshape((4,4))
print('\n\n전체 데이터 :')
print(np_array)

X = np_array[:, :3] # 마지막 열 제외
y = np_array[:,3]   # 마지막 3열만 y데이터 저장
  # : = 다 담아라, :N = N열/행부터는 빼고 담아라, N = N열/행만 담아라

print('\nX 데이터 :')
print(X)
print('y 데이터 :',y,'\n\n')

df


# (4) scikit-learn 이용하여 분리하기

# 임의로 X와 y 데이터를 생성
X, y = np.arange(10).reshape((5, 2)), range(5)

print('[(4)scikit-learn] X 전체 데이터 :')
print(X)
print('y 전체 데이터 :')
print(list(y))

# 7:3의 비율로 훈련 데이터와 테스트 데이터 분리 (사이킷 런)
X_train, X_test, y_train, y_test = train_test_split(X, y,
                        test_size=0.3, random_state=1234)

# random_state 가 달라지면 split 되는 양상도 달라짐

print('\n\nX 훈련 데이터 :')
print(X_train)
print('X 테스트 데이터 :')
print(X_test)

print('\ny 훈련 데이터 :')
print(y_train)
print('y 테스트 데이터 :')
print(y_test)



# (5) 수동으로 분리하기
X, y = np.arange(0,24).reshape((12,2)), range(12)
    # 실습을 위해 임의로 X와 y가 이미 분리 된 데이터를 생성

print('\n\nX [(5)수동] 전체 데이터 :')
print(X)
print('y 전체 데이터 :')
print(list(y))


# (5)-1 : 훈련데이터/학습데이터 개수 지정

num_of_train = int(len(X) * 0.8)
    # 데이터의 전체 길이의 80%에 해당하는 길이값을 구한다.
num_of_test = int(len(X) - num_of_train)
    # 전체 길이에서 80%에 해당하는 길이를 뺀다.

print('\n훈련 데이터의 크기 :',num_of_train)
print('테스트 데이터의 크기 :',num_of_test)

X_test = X[num_of_train:] # 전체 데이터 중에서 20%만큼 뒤의 데이터 저장
y_test = y[num_of_train:] # 전체 데이터 중에서 20%만큼 뒤의 데이터 저장
X_train = X[:num_of_train] # 전체 데이터 중에서 80%만큼 앞의 데이터 저장
y_train = y[:num_of_train] # 전체 데이터 중에서 80%만큼 앞의 데이터 저장

print('\nX 테스트 데이터 :')
print(X_test)
print('y 테스트 데이터 :')
print(list(y_test))


