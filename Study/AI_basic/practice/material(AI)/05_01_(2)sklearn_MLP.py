import pandas as pd
from sklearn.datasets import load_wine
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.neural_network import MLPClassifier
from sklearn.metrics import classification_report, confusion_matrix

# 1. 와인 데이터 로드 및 확인
wine = load_wine()
data = pd.DataFrame(data=wine['data'], columns=wine['feature_names'])
print(data.head())

# 2. 데이터 분리
X = wine.data
y = wine.target
X_train, X_test, y_train, y_test = train_test_split(X, y)

# 3. 데이터 표준화 (Feature Scaling)
# 신경망 모델은 데이터의 스케일에 민감하므로 필수적인 단계입니다.
scaler = StandardScaler()
scaler.fit(X_train)

# 학습 데이터와 테스트 데이터를 동일한 기준으로 변환
X_train = scaler.transform(X_train)
X_test = scaler.transform(X_test)

# 4. MLP 분류기 모델 생성 및 학습
# hidden_layer_sizes=(13, 13, 13): 13개의 노드를 가진 은닉층을 3층으로 쌓음
# max_iter=500: 최대 반복 학습 횟수
mlp = MLPClassifier(hidden_layer_sizes=(13, 13, 13), max_iter=500)
mlp.fit(X_train, y_train)

# 5. 예측 및 결과 평가
predictions = mlp.predict(X_test)

# 오차 행렬(Confusion Matrix) 출력
print("\n--- Confusion Matrix ---")
print(confusion_matrix(y_test, predictions))

# 상세 분류 보고서(Precision, Recall, F1-score) 출력
print("\n--- Classification Report ---")
print(classification_report(y_test, predictions))