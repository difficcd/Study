from sklearn.tree import DecisionTreeRegressor
import pandas as pd
import matplotlib.pyplot as plt

# 1. 데이터 불러오기 (보스턴 주택 데이터)
# 정규표현식(sep='\s+')을 사용해 공백으로 구분된 데이터를 읽어옵니다.
df = pd.read_csv('housing.data', header=None, sep='\s+')
df.columns = ['CRIM', 'ZN', 'INDUS', 'CHAS', 'NOX', 'RM', 'AGE', 'DIS', 'RAD', 
              'TAX', 'PTRATIO', 'B', 'LSTAT', 'MEDV']

print(df.head())

# 2. 특징(X)과 타겟(y) 설정
# LSTAT(하위 계층 비율)을 입력으로, MEDV(주택 가격)를 출력으로 설정합니다.
X = df[['LSTAT']].values
y = df['MEDV'].values

# 3. 결정 트리 회귀 모델 생성 및 학습
# max_depth=3으로 설정하여 나무가 너무 깊게 자라지 않도록(과적합 방지) 제한합니다.
tree = DecisionTreeRegressor(max_depth=3)
tree.fit(X, y)

# 4. 시각화를 위한 데이터 정렬
# 그래프를 매끄럽게 그리기 위해 X값을 기준으로 인덱스를 정렬합니다.
sort_idx = X.flatten().argsort()

# 5. 결과 시각화
plt.scatter(X[sort_idx], y[sort_idx], c='lightblue') # 실제 데이터 산점도
plt.plot(X[sort_idx], tree.predict(X[sort_idx]), color='red', linewidth=2) # 모델의 예측선
plt.xlabel('LSTAT(% Lower Status of the Population)')
plt.ylabel('MEDV(Price in $1000)')
plt.show()