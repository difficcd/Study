# -*- coding: utf-8 -*-

import torch
from sklearn.datasets import fetch_openml 
from sklearn.model_selection import train_test_split
import matplotlib.pyplot as plt
from torch.utils.data import TensorDataset, DataLoader
from torch import nn, optim

mnist = fetch_openml('mnist_784', version=1, cache=True)
# torchvision 대신 sklearn.datasets.fetch_openml() 이용

X = mnist.data / 255  
    # 이미지 데이터 픽셀값 범위 0~255 => 0~1로 정규화 (데이터 전처리)
    # 정규화해줘야 입력값이 적절한 범위
y = mnist.target

plt.imshow(X.values[0].reshape(28, 28), cmap='gray') 
plt.show() 
print("이미지 레이블 : {}".format(y[0])) 


X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=1/7, random_state=0
    ) # 01_Introduction.md 참조 
      # 직접 텐서로 반환하는 torchvision 가 아니므로, 가공 (from scratch)

X_train = torch.Tensor(X_train.values)
X_test = torch.Tensor(X_test.values)
y_train = torch.LongTensor(list(map(int, y_train)))
y_test = torch.LongTensor(list(map(int, y_test)))
    # list(map(int, y..)) => pytorch를 위해 tensor 꼴로(int..) 가공

ds_train = TensorDataset(X_train, y_train)
ds_test = TensorDataset(X_test, y_test)

loader_train = DataLoader(ds_train, batch_size=64, shuffle=True)
loader_test = DataLoader(ds_test, batch_size=64, shuffle=False)


model = nn.Sequential() 

model.add_module('fc1', nn.Linear(28*28*1, 100))    
model.add_module('relu1', nn.ReLU())

model.add_module('fc2', nn.Linear(100, 100))
model.add_module('relu2', nn.ReLU())

model.add_module('fc3', nn.Linear(100, 10))


loss_fn = nn.CrossEntropyLoss() 
optimizer = optim.Adam(model.parameters(), lr=0.01)
# 최적화 함수는 Adam 사용

# 모델 학습 함수  
def train(epoch):
    model.train() 
    for data, targets in loader_train: 

        optimizer.zero_grad() 
        outputs = model(data) 
        loss = loss_fn(outputs, targets) 
      
        loss.backward() 
        optimizer.step() 

    print('에포크 {}: 완료'.format(epoch)) 


# 모델 테스트 함수
def test(head):
    model.eval() 
    correct = 0
    with torch.no_grad(): 
        for data, targets in loader_test: 

            outputs = model(data) 
            _, predicted = torch.max(outputs.data, 1) 
         
            correct += predicted.eq(targets.data.view_as(predicted)).sum()
           
    data_num = len(loader_test.dataset) 
    print('{} 정확도: {}/{} ({:.0f}%)'.format(
        head, correct, data_num, 100.*correct/data_num
        ))


# 실행 예시 (필요시 추가)
for epoch in range(1, 4):
    train(epoch) 
    test(epoch) 