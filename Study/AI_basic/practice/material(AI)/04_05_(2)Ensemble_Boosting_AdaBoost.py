from sklearn import datasets
from sklearn.model_selection import train_test_split
from sklearn.ensemble import AdaBoostClassifier
from sklearn import metrics

# 1. 데이터 로드
iris = datasets.load_iris()
X = iris.data    # 입력 특징
y = iris.target  # 출력 타겟

# 2. 데이터 분리 (테스트 데이터 30%)
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3)

# 3. AdaBoost 모델 생성 및 학습
# n_estimators=50: 50개의 약한 학습기(Weak Learner)를 순차적으로 결합
# learning_rate=1: 학습률 설정
abc = AdaBoostClassifier(n_estimators=50, learning_rate=1) # 모델 생성
model = abc.fit(X_train, y_train)

# 4. 예측 및 정확도 측정
y_pred = model.predict(X_test) # 추론(예측)
print("Accuracy:", metrics.accuracy_score(y_test, y_pred))