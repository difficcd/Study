
import numpy as np # 넘파이 사용
import matplotlib.pyplot as plt # 맷플롯립 사용

import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim


def sigmoid(x): # 시그모이드 함수 정의
    return 1/(1+np.exp(-x))

def sigmoid_W_Test():
    x = np.arange(-5.0, 5.0, 0.1) 
    y1 = sigmoid(0.5*x) # y = 0.5x  
    y2 = sigmoid(x)     # y = x
    y3 = sigmoid(2*x)   # y = 2x

    plt.plot(x, y1, 'r', linestyle='--') 
    plt.plot(x, y2, 'g')
    plt.plot(x, y3, 'b', linestyle='--') 
    plt.plot([0,0],[1.0,0.0], ':') # 가운데 점선 추가

    plt.title('Sigmoid Function')
    plt.show()

def sigmoid_b_Test():
    x = np.arange(-5.0, 5.0, 0.1)
    y1 = sigmoid(x+0.5)  # x + 0.5
    y2 = sigmoid(x+1)    # x + 1
    y3 = sigmoid(x+1.5)  # x + 1.5

    plt.plot(x, y1, 'r', linestyle='--') 
    plt.plot(x, y2, 'g') 
    plt.plot(x, y3, 'b', linestyle='--') 
    plt.plot([0,0],[1.0,0.0], ':') 

    plt.title('Sigmoid Function')
    plt.show()



def _01_logistic_regression_basic():
    # material(AI)\practice\04_03_(2)logistic_regression.py 
    # 데이터 csv 파일 이용한 로지스틱 회귀 이진분류 예제 reference
    # 단, 여기서는 pytorch 대신 scikit-learn 사용

    torch.manual_seed(1)

    # 간단한 텐서 예제 
    x_data = [[1, 2], [2, 3], [3, 1], [4, 3], [5, 3], [6, 2]] # (6,2)
    y_data = [[0], [0], [0], [1], [1], [1]] # (6,1)
    x_train = torch.FloatTensor(x_data)
    y_train = torch.FloatTensor(y_data)

    W = torch.zeros((2, 1), requires_grad=True) # 크기는 2 x 1
    b = torch.zeros(1, requires_grad=True)

    hypothesis = 1 / (1 + torch.exp(-(x_train.matmul(W) + b))) # 직접 구현
    # hypothesis = torch.sigmoid(x_train.matmul(W) + b) # pytorch 제공
    print(hypothesis) # 예측값인 H(x) 출력



    print(-(y_train[0] * torch.log(hypothesis[0]) + 
          (1 - y_train[0]) * torch.log(1 - hypothesis[0]))) # 한 원소에 대한 cost
    
    # cost(losses) 직접 구현
    losses = -(y_train * torch.log(hypothesis) + 
           (1 - y_train) * torch.log(1 - hypothesis))
    print(losses)
    cost = losses.mean()
    print(cost)

    # F.binary_cross_entropy(hypothesis, y_train) # pytorch 제공


    # optimizer 설정
    optimizer = optim.SGD([W, b], lr=1)

    nb_epochs = 1000
    for epoch in range(nb_epochs + 1):

        # Cost 계산
        hypothesis = torch.sigmoid(x_train.matmul(W) + b)
        cost = -(y_train * torch.log(hypothesis) + 
                (1 - y_train) * torch.log(1 - hypothesis)).mean()

        # cost로 H(x) 개선
        optimizer.zero_grad()
        cost.backward()
        optimizer.step()

        # 100번마다 로그 출력
        if epoch % 100 == 0:
            print('Epoch {:4d}/{} Cost: {:.6f}'.format(
                epoch, nb_epochs, cost.item()
            ))

    
    # 훈련 데이터로 예측 테스트
    hypothesis = torch.sigmoid(x_train.matmul(W) + b)
    print(hypothesis)

    # 기준을 0.5로 하여 이진 분류 결과로 변환
    prediction = hypothesis >= torch.FloatTensor([0.5])
    print(prediction)

    print(W)
    print(b)

def _02_logistic_regression_Pytorch_Class():

    def logistic_regression_pytorch():
        torch.manual_seed(1)

        x_data = [[1, 2], [2, 3], [3, 1], [4, 3], [5, 3], [6, 2]]
        y_data = [[0], [0], [0], [1], [1], [1]]

        x_train = torch.FloatTensor(x_data)
        y_train = torch.FloatTensor(y_data)

        model = nn.Sequential(
            nn.Linear(2, 1), # input_dim = 2, output_dim = 1
            nn.Sigmoid() # 출력은 시그모이드 함수를 거친다
        )

        model(x_train) # 예측값 H(x) 확인

        # optimizer 설정
        optimizer = optim.SGD(model.parameters(), lr=1)

        nb_epochs = 1000 # 학습 epochs 수행
        for epoch in range(nb_epochs + 1):

            hypothesis = model(x_train) # H(x) 계산
            cost = F.binary_cross_entropy(hypothesis, y_train) # cost 계산

            # cost로 H(x) 개선
            optimizer.zero_grad()
            cost.backward()
            optimizer.step()

            # 20번마다 로그 출력
            if epoch % 10 == 0:
                prediction = hypothesis >= torch.FloatTensor([0.5]) 
                # 예측값이 0.5를 넘으면 True로 간주
                correct_prediction = prediction.float() == y_train 
                # 실제값과 일치하는 경우만 True로 간주

                accuracy = correct_prediction.sum().item() / len(correct_prediction)
                # 정확도를 계산
                print('Epoch {:4d}/{} Cost: {:.6f} Accuracy {:2.2f}%'.format( 
                    # 각 에포크마다 정확도를 출력
                    epoch, nb_epochs, cost.item(), accuracy * 100,
                ))
        
        torch.set_printoptions(sci_mode=False, precision=4) # 보기쉽게 소수로 출력
        print(model(x_train)) # 예측 테스트

        print(list(model.parameters())) # 조절된 W,b값
    # logistic_regression_pytorch()

    def logistic_regression_class():

        torch.manual_seed(1)
        x_data = [[1, 2], [2, 3], [3, 1], [4, 3], [5, 3], [6, 2]]
        y_data = [[0], [0], [0], [1], [1], [1]]
        x_train = torch.FloatTensor(x_data)
        y_train = torch.FloatTensor(y_data)

        # Sequential 
        class BinaryClassifier(nn.Module):
            def __init__(self):
                super().__init__()
                self.linear = nn.Linear(2, 1)
                self.sigmoid = nn.Sigmoid()

            def forward(self, x):
                return self.sigmoid(self.linear(x))
        
        model = BinaryClassifier()
        optimizer = optim.SGD(model.parameters(), lr=1)

        nb_epochs = 1000
        for epoch in range(nb_epochs + 1):
            
            hypothesis = model(x_train)  # H(x) 계산
            cost = F.binary_cross_entropy(hypothesis, y_train)

            # cost로 H(x) 개선
            optimizer.zero_grad()
            cost.backward()
            optimizer.step()

            # 20번마다 로그 출력
            if epoch % 10 == 0:
                prediction = hypothesis >= torch.FloatTensor([0.5])
                # 예측값이 0.5를 넘으면 True로 간주
                correct_prediction = prediction.float() == y_train 
                # 실제값과 일치하는 경우만 True로 간주
                accuracy = correct_prediction.sum().item() / len(correct_prediction) 
                # 정확도를 계산
                print('Epoch {:4d}/{} Cost: {:.6f} Accuracy {:2.2f}%'.format( 
                    # 각 에포크마다 정확도를 출력
                    epoch, nb_epochs, cost.item(), accuracy * 100,
                ))
    logistic_regression_class()



# sigmoid_W_Test()
# sigmoid_b_Test()

# _01_logistic_regression_basic()
_02_logistic_regression_Pytorch_Class()


    




