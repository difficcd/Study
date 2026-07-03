# -*- coding: utf-8 -*-

import torch
from sklearn.datasets import fetch_openml
from sklearn.model_selection import train_test_split
import matplotlib.pyplot as plt
from torch.utils.data import TensorDataset, DataLoader
from torch import nn, optim

mnist = fetch_openml('mnist_784', version=1, cache=True)
# 7만개의 28x28 숫자 이미지 데이터set 가져오기

X = mnist.data / 255
y = mnist.target
# X: /255 를 통해 0~255의 픽셀값을 0~1로 정규화 (학습 효율 높여줌)
# Y: 각 이미지 데이터의 정답 (숫자 사진이 1이면, 1이 들어있음)


plt.imshow(X.values[0].reshape(28, 28), cmap='gray') 
# 학습 데이터로 사용될 원본 데이터 확인
plt.show() 
# 이미지 화면에 띄우기
print("이미지 레이블 : {}".format(y[0])) # 정답 텍스트로 출력



X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=1/7, random_state=0)
# 테스트 데이터를 1/7, 학습 데이터를 6/7 로 설정하기
# X: 데이터, y: 라벨(정답), train:학습, test:테스트 데이터
# X_train shape: [60000, 784] 7만개 * 6/7, 28*28
# X_test shape: [10000, 784] 7만개 * 1/7, 28*28
# Y_train shape: [60000]  정답 리스트이므로, 개수 동일
# Y_test shape: [10000]

X_train = torch.Tensor(X_train.values)
X_test = torch.Tensor(X_test.values)
y_train = torch.LongTensor(list(map(int, y_train)))
y_test = torch.LongTensor(list(map(int, y_test)))
# 각 데이터 집합을 텐서로 변경하기 (pytorch tensor)


ds_train = TensorDataset(X_train, y_train)
ds_test = TensorDataset(X_test, y_test)
# 이미지, 정답 한 쌍으로 묶기


loader_train = DataLoader(ds_train, batch_size=64, shuffle=True)
loader_test = DataLoader(ds_test, batch_size=64, shuffle=False)
# 각 데이터 64개씩 묶은 loader 변수 생성 (batch)
# 학습 데이터는 더 좋은 학습을 위해 무작위성 높여서(셔플) 준비


model = nn.Sequential() # 비어있는 신경망

model.add_module('fc1', nn.Linear(28*28*1, 100))    # 모델 구성
 # 입력 28*28*1 => 100개 노드인 1층 만들기 
 # shape: [100, 784]
model.add_module('relu1', nn.ReLU())
 # 활성화 함수를 ReLU 로 하여 음수값 0처리 & 비선형 변환

model.add_module('fc2', nn.Linear(100, 100))
 # 100 => 100개 노드인 2층 만들기
 # shape: [100, 100]
model.add_module('relu2', nn.ReLU())
 # 활성화 함수 ReLU 통과시키기

model.add_module('fc3', nn.Linear(100, 10))
 # 100 => 10개 노드로 합쳐 최종적으로 0~9 나타내도록 함
 # shape: [10, 100]


loss_fn = nn.CrossEntropyLoss() 
# 손실 함수를 교차 엔트로피로 세팅 
optimizer = optim.Adam(model.parameters(), lr=0.01)
# 오차 역전파 알고리즘의 가중치 조절을 위한 가중치 업데이트 도구
# lr = 0.01 은 학습률!


# 모델 학습 함수 
def train(epoch):
    model.train() # 학습 모드로 변환
    for data, targets in loader_train: 
    # 가공한 학습 데이터의 데이터(data)와 정답(targets) 순회:
    # data shape: [64, 784] 64개 묶음이며, 28*28
    # target shape: [64] 64개 이미지의 정답

        optimizer.zero_grad() # 그래디언트 초기화
        outputs = model(data) # 모델의 예즉값을 outputs에 저장
        # outputs shape: [64, 10] 모델 마지막 층 통과한 출력 (0~9 범위이므로 10)

        loss = loss_fn(outputs, targets) 
        # 출력과 정답 간의 오차를 손실함수(교차 엔트로피) 통해 구하기

        loss.backward() # 그래프를 통해 미분 계산 (오차 역전파)
        optimizer.step() # 역전파 값을 통해 가중치 업데이트

    print('에포크 {}: 완료'.format(epoch)) # 에포크 = 학습 횟수 단위



# 모델 테스트 함수
def test(head):
    model.eval() # 테스트 모드로 변환
    correct = 0
    with torch.no_grad(): 
        # no_grad = 가중치 변경 X (테스트이므로, 저원 절약)
        for data, targets in loader_test: 
            # 가공한 테스트 데이터의 데이터(data)와 정답(targets) 순회:

            outputs = model(data) # 모델의 예측값
            _, predicted = torch.max(outputs.data, 1) 
            # 예측값 중 가장 가능성이 큰 값 선택 
            # predicted shape: [64] 64개 데이터셋에 대해 정답가능성 큰 인덱스

            correct += predicted.eq(targets.data.view_as(predicted)).sum()
            # 예측값이 맞은 경우를 correct 변수에 누적시킴
    
    data_num = len(loader_test.dataset) # 테스트 데이터 개수
    print('{} 정확도: {}/{} ({:.0f}%)'.format(head, correct, data_num, 100.*correct/data_num))
    # 정확도는 전체 데이터가 분모, 맞은 개수가 분자

# 실행 예시 (필요시 추가)
for epoch in range(1, 4): # 3회의 에포크 반복
    train(epoch) # 학습
    test(epoch) # 테스트