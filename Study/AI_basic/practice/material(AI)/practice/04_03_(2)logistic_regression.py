import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

dataset = pd.read_csv('User_Data.csv')

x = dataset.iloc[:, [2, 3]].values  # 입력
y = dataset.iloc[:, 4].values      # 출력

from sklearn.model_selection import train_test_split
xtrain, xtest, ytrain, ytest = train_test_split(x, y, test_size = 0.25, random_state = 0)

from sklearn.preprocessing import StandardScaler
sc_x = StandardScaler()
xtrain = sc_x.fit_transform(xtrain)
xtest = sc_x.transform(xtest)
print (xtrain[0:10, :])

from sklearn.linear_model import LogisticRegression
classifier = LogisticRegression(random_state = 0)
classifier.fit(xtrain, ytrain)
y_pred = classifier.predict(xtest)

from sklearn.metrics import confusion_matrix
cm = confusion_matrix(ytest, y_pred)
print ("혼동행렬 : \n", cm)

from sklearn.metrics import accuracy_score
print ("정확도 : ", accuracy_score(ytest, y_pred))


# 이어서, 시각화 코드
from matplotlib.colors import ListedColormap

# 테스트 세트 데이터를 시각화용 변수로 설정
X_set, y_set = xtest, ytest

# 그래프의 격자(Meshgrid) 생성: 데이터의 최소~최대 범위를 0.01 간격으로 촘촘하게 채움
X1, X2 = np.meshgrid(np.arange(start = X_set[:, 0].min() - 1, 
                               stop = X_set[:, 0].max() + 1, step = 0.01),
                     np.arange(start = X_set[:, 1].min() - 1, 
                               stop = X_set[:, 1].max() + 1, step = 0.01))

# 격자의 모든 점에 대해 모델의 예측값 계산 후 배경색 칠하기
plt.contourf(X1, X2, classifier.predict(
    np.array([X1.ravel(), X2.ravel()]).T).reshape(
    X1.shape), alpha = 0.75, cmap = ListedColormap(('red', 'green')))

# 그래프의 축 범위 설정
plt.xlim(X1.min(), X1.max())
plt.ylim(X2.min(), X2.max())

# 실제 데이터 포인트들을 그래프 위에 산점도로 표시
colors = ListedColormap(('red', 'green'))
for i, j in enumerate(np.unique(y_set)):
    plt.scatter(X_set[y_set == j, 0], X_set[y_set == j, 1],
                color = colors(i), label = j)

# 그래프 제목 및 라벨 설정
plt.title('Classifier (Test set)')
plt.xlabel('Age')
plt.ylabel('Estimated Salary')
plt.legend()
plt.show()