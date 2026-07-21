import pandas as pd
from sklearn import datasets
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier
from sklearn import metrics

# 데이터 로드 및 확인
iris = datasets.load_iris()
print('Class names :', iris.target_names)
print('target : [0:setosa, 1:versicolor, 2:virginica]')
print('No. of Data :', len(iris.data))
print('Feature names :', iris.feature_names)

# DataFrame 생성
data = pd.DataFrame({
    'sepal length': iris.data[:, 0],
    'sepal width': iris.data[:, 1],
    'petal length': iris.data[:, 2],
    'petal width': iris.data[:, 3],
    'species': iris.target
})

print(data.head()) # 일부 데이터 출력

# 데이터 분리 (입력 X, 출력 y)
x = data[['sepal length', 'sepal width', 'petal length', 'petal width']] # 입력
y = data['species'] # 출력

# 학습 데이터와 테스트 데이터 분리 (테스트 데이터 30%)
x_train, x_test, y_train, y_test = train_test_split(x, y, test_size=0.3)
print("No. of training data: ", len(x_train))
print("No. of test data: ", len(y_test))

# Random Forest 모델 생성 및 학습
forest = RandomForestClassifier(n_estimators=100) # 모델 생성
forest.fit(x_train, y_train)

# 예측 및 정확도 확인
y_pred = forest.predict(x_test) # 추론(예측)
print('Accuracy :', metrics.accuracy_score(y_test, y_pred))