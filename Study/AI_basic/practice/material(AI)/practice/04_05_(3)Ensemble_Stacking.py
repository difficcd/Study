from sklearn import datasets
from sklearn.ensemble import RandomForestClassifier
from sklearn.linear_model import LogisticRegression
from sklearn.naive_bayes import GaussianNB
from sklearn.neighbors import KNeighborsClassifier
from sklearn.model_selection import cross_val_predict  # [버그 수정] 스태킹 누수 방지용
import numpy as np

# 1. 데이터 로드 및 특징 선택
iris = datasets.load_iris()
# 이미지 코드상 특징의 일부(1:3번 인덱스 열)만 사용하고 있습니다.
X, y = iris.data[:, 1:3], iris.target

# 2. 정확도 계산 함수 정의
def CalculateAccuracy(y_test, pred_label):
    # 실제값과 예측값이 같은 개수(nnz) 계산
    nnz = np.shape(y_test)[0] - np.count_nonzero(pred_label - y_test)
    # 백분율로 정확도 산출
    acc = 100 * nnz / float(np.shape(y_test)[0])
    return acc

# 3. 개별 모델 생성 (기반 모델들)
clf1 = KNeighborsClassifier(n_neighbors=2)
clf2 = RandomForestClassifier(n_estimators=2, random_state=1)
clf3 = GaussianNB()
# 메타 모델 (최종 결정 모델)
lr = LogisticRegression()

# 4. 각 개별 모델 학습
clf1.fit(X, y)
clf2.fit(X, y)
clf3.fit(X, y)

# 5. 각 모델의 예측 및 정확도 출력
f1 = clf1.predict(X)
acc1 = CalculateAccuracy(y, f1)
print("accuracy from KNN: " + str(acc1))

f2 = clf2.predict(X)
acc2 = CalculateAccuracy(y, f2)
print("accuracy from Random Forest: " + str(acc2))

f3 = clf3.predict(X)
acc3 = CalculateAccuracy(y, f3)
print("accuracy from Naive Bayes: " + str(acc3))

# 6. 스태킹(Stacking): 개별 모델의 예측결과를 합쳐서 새로운 특징 데이터 생성
#   [버그 수정] 데이터 누수(leakage) 제거:
#     기존 코드는 base 모델을 전체 X로 학습한 뒤(29~31), 같은 X에 대한 in-sample
#     예측(f1,f2,f3)을 그대로 메타 모델의 학습 특징으로 사용 -> 메타 모델이 정답을
#     이미 본 예측으로 학습하는 꼴이라 정확도가 낙관적으로 부풀려짐.
#     cross_val_predict로 out-of-fold 예측(각 샘플을 학습에 쓰지 않은 모델의 예측)을
#     만들어 메타 특징으로 쓰면 누수 없이 학습할 수 있음.
oof1 = cross_val_predict(clf1, X, y, cv=5)
oof2 = cross_val_predict(clf2, X, y, cv=5)
oof3 = cross_val_predict(clf3, X, y, cv=5)
f = np.transpose([oof1, oof2, oof3]) # 각 행 = 샘플당 3개 모델의 OOF 예측값

# 7. 메타 모델 학습 및 최종 예측
lr.fit(f, y)
final = lr.predict(f)

acc4 = CalculateAccuracy(y, final)
print("accuracy from Stacking: " + str(acc4))