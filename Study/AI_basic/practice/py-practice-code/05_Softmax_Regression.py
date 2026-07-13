import torch
import torch.nn.functional as F
import torch.nn as nn
import torch.optim as optim

# mnist 분류기 구현
import torchvision.datasets as dsets
import torchvision.transforms as transforms
from torch.utils.data import DataLoader
import matplotlib.pyplot as plt
import random


def _01_softmax_regression_implement():

    # 공통 data
    torch.manual_seed(1)
    x_train = [[1, 2, 1, 1],
           [2, 1, 3, 2],
           [3, 1, 3, 4],
           [4, 1, 5, 5],
           [1, 7, 5, 5],
           [1, 2, 5, 6],
           [1, 6, 6, 6],
           [1, 7, 7, 7]] # shape : (8,4)
    y_train = [2, 2, 2, 1, 1, 1, 0, 0] # shape : (8,)
    x_train = torch.FloatTensor(x_train)
    y_train = torch.LongTensor(y_train)
    
    print(x_train.shape, '\n')
    print(y_train.shape, '\n')


    def cost_function_implement():
        z = torch.FloatTensor([1, 2, 3])
        hypothesis = F.softmax(z, dim=0) 
        print('\n', hypothesis, '\n') 
        hypothesis.sum() # softmax = 0~1 범위 수로 변환

        # =====  softmax regression "Cost function" 구현하기 ===== #
        z = torch.rand(3, 5, requires_grad=True) # (3,5) 랜덤 tensor
        hypothesis = F.softmax(z, dim=1) 
        y = torch.randint(5, (3,)).long() # 임의의 label

        print(z, '\n')
        print(hypothesis, '\n')
        print(y, '\n')

        # 원 핫 인코딩
        y_one_hot = torch.zeros_like(hypothesis)  # (3,5) 초기화(0)
        print(y.unsqueeze(1), '\n')               # (3,)에서 (3,1)로 unsqueeze 
        y_one_hot.scatter_(1, y.unsqueeze(1), 1) 
            # unsqueeze : 02 ) _t04_7_unsqueeze() 참조

        # 손실 함수
        torch.log(F.softmax(z, dim=1))  # low level
        F.log_softmax(z, dim=1)         # high level (pytorch 제공) 

        """
        cost = (y_one_hot * -torch.log(hypothesis)).sum(dim=1).mean() # low level
        cost = (y_one_hot * - F.log_softmax(z, dim=1)).sum(dim=1).mean() 
                                                        # log_softmax() 사용
        cost = F.nll_loss(F.log_softmax(z, dim=1), y)   # high level
        cost = F.cross_entropy(z,y)         # cross entropy : cost func + softmax func
        """

        cost = nn.CrossEntropyLoss()(z, y)  # class 이용한 구현 방식 (생성+사용)
        print(cost, '\n') 

        """
        criterion = nn.CrossEntropyLoss()   # 객체 생성
        loss = criterion(z, y)              # 객체 사용
        print(loss, '\n')
        """
        
        # 손실값의 평균 대신 "합계"가 필요할 떄는?
        # 손실함수 객체를 한 번만 생성. 호출할 때는 무조건 criterion으로만 호출
        criterion = nn.CrossEntropyLoss(reduction='sum')

        # 같은 객체로 여러 번 계산 가능
        loss1 = criterion(z, y)           # 첫 번째 계산
        loss2 = criterion(z, y)           # 두 번째 계산

        # 새로운 데이터가 있다면
        z2 = torch.rand(3, 5, requires_grad=True)
        y2 = torch.randint(5, (3,)).long()
        loss3 = criterion(z2, y2)  # 새 데이터로 계산

        print("loss1: ", loss1, "\nloss2: ", loss2, "\nloss3: ", loss3, '\n')
    # cost_function_implement()

    def regression_implement_low_level():
        # 원-핫 인코딩 
        y_one_hot = torch.zeros(8, 3)
        y_one_hot.scatter_(1, y_train.unsqueeze(1), 1)
        print(y_one_hot.shape, '\n') # label (8,3) 

        # 모델 초기화
        W = torch.zeros((4, 3), requires_grad=True)
        b = torch.zeros((1, 3), requires_grad=True)

        # optimizer 설정
        optimizer = optim.SGD([W, b], lr=0.1)

        nb_epochs = 1000
        for epoch in range(nb_epochs + 1):

            # 가설
            hypothesis = F.softmax(x_train.matmul(W) + b, dim=1) 

            # 비용 함수 (직접 구현)
            cost = (y_one_hot * -torch.log(hypothesis)).sum(dim=1).mean()

            # cost로 H(x) 개선
            optimizer.zero_grad()
            cost.backward()
            optimizer.step()

            # 100번마다 로그 출력
            if epoch % 100 == 0:
                print('Epoch {:4d}/{} Cost: {:.6f}'.format(
                    epoch, nb_epochs, cost.item()
                ))
    # regression_implement_low_level()

    def regression_implement_high_level():
        # 모델 초기화
        W = torch.zeros((4, 3), requires_grad=True)
        b = torch.zeros((1, 3), requires_grad=True)
        
        optimizer = optim.SGD([W, b], lr=0.1) # optimizer 설정

        nb_epochs = 1000
        for epoch in range(nb_epochs + 1):

            # Cost 계산 (torch의 cross_entropy() 사용)
            z = x_train.matmul(W) + b  # hypothesis
            cost = F.cross_entropy(z, y_train) 

            # cost로 H(x) 개선
            optimizer.zero_grad()
            cost.backward()
            optimizer.step()

            # 100번마다 로그 출력
            if epoch % 100 == 0:
                print('Epoch {:4d}/{} Cost: {:.6f}'.format(
                    epoch, nb_epochs, cost.item()
                ))
    # regression_implement_high_level()

    def regression_implement_nnModule():

        # 모델을 선언 및 초기화
        # 4개 특성 가지고 3개 클래스로 분류: input_dim=4, output_dim=3.
        model = nn.Linear(4, 3)
        optimizer = optim.SGD(model.parameters(), lr=0.1)

        nb_epochs = 1000
        for epoch in range(nb_epochs + 1):

            prediction = model(x_train) #  H(x) 계산
            cost = F.cross_entropy(prediction, y_train) # cost 계산

            # cost로 H(x) 개선
            optimizer.zero_grad()
            cost.backward()
            optimizer.step()

            # 100번마다 로그 출력
            if epoch % 100 == 0:
                print('Epoch {:4d}/{} Cost: {:.6f}'.format(
                    epoch, nb_epochs, cost.item()
                ))
    # regression_implement_nnModule()

    def regression_implement_Class():
        class SoftmaxClassifierModel(nn.Module):
            def __init__(self):
                super().__init__()
                self.linear = nn.Linear(4, 3) # Output이 3!

            def forward(self, x):
                return self.linear(x)

        model = SoftmaxClassifierModel()
        optimizer = optim.SGD(model.parameters(), lr=0.1)

        nb_epochs = 1000
        for epoch in range(nb_epochs + 1):

            prediction = model(x_train) # H(x) 계산
            cost = F.cross_entropy(prediction, y_train)

            # cost로 H(x) 개선
            optimizer.zero_grad()
            cost.backward()
            optimizer.step()

            # 100번마다 로그 출력
            if epoch % 100 == 0:
                print('Epoch {:4d}/{} Cost: {:.6f}'.format(
                    epoch, nb_epochs, cost.item()
                ))
    regression_implement_Class()
    
def _02_softmax_regression_mnist():
    # material(AI)\practice\PyTorch_MLP_Prog.py 참조

    USE_CUDA = torch.cuda.is_available() 
             # GPU를 사용가능하면 True, 아니라면 False를 리턴
    device = torch.device("cuda" if USE_CUDA else "cpu") 
             # GPU 사용 가능하면 사용하고 아니면 CPU 사용
    print("GPU: ", USE_CUDA)
    print("다음 기기로 학습합니다: ", device, '\n')

    # for reproducibility
    random.seed(777)
    torch.manual_seed(777)
    if device == 'cuda':
        torch.cuda.manual_seed_all(777)

    # hyperparameters
    training_epochs = 15
    batch_size = 100

    # MNIST dataset
    mnist_train = dsets.MNIST(root='MNIST_data/',
                            train=True,
                            transform=transforms.ToTensor(),
                            download=True)

    mnist_test = dsets.MNIST(root='MNIST_data/',
                            train=False,
                            transform=transforms.ToTensor(),
                            download=True)
    
    # dataset loader
    data_loader = DataLoader(dataset=mnist_train,
                            batch_size=batch_size, # 배치 크기는 100
                            shuffle=True,
                            drop_last=True)
    
    # 모델 설계 : MNIST data image of shape 28 * 28 = 784
    linear = nn.Linear(784, 10, bias=True).to(device)


    # 비용 함수와 옵티마이저 정의
    criterion = nn.CrossEntropyLoss().to(device) 
                # 내부적으로 소프트맥스 함수를 포함
    optimizer = torch.optim.SGD(linear.parameters(), lr=0.1)



    for epoch in range(training_epochs): 
        avg_cost = 0
        total_batch = len(data_loader) # 100개로 나눈 묶음 개수
        
        for X, Y in data_loader: 
            # 배치 크기가 100이므로 아래의 연산에서 X는 (100, 784)의 텐서가 된다.
            # row = sample 수 = 100,    col = feature 수 = 28*28 = 784

            X = X.view(-1, 28 * 28).to(device)
            Y = Y.to(device) # 레이블: 원-핫 인코딩되지 않은 0 ~ 9의 정수.

            optimizer.zero_grad()
            hypothesis = linear(X)
            cost = criterion(hypothesis, Y)
            cost.backward()
            optimizer.step()

            avg_cost += cost / total_batch

        print('Epoch:', '%04d' % (epoch + 1), 
              'cost =', '{:.9f}'.format(avg_cost))
    print('Learning finished')

    # ==== 모델 테스트 (mnist_test data 사용) ==== #
    with torch.no_grad(): # torch.no_grad(): gradient 계산 방지 (RV)
        X_test = mnist_test.test_data.view(-1, 28 * 28).float().to(device)
        Y_test = mnist_test.test_labels.to(device)

        prediction = linear(X_test) # 전체 데이터 한 번에 연산 (큰 경우 나누기)

        correct_prediction = torch.argmax(prediction, 1) == Y_test
        accuracy = correct_prediction.float().mean()
        print('Accuracy:', accuracy.item())

        # MNIST 테스트 데이터에서 << 무작위로 하나 >> 뽑아서 예측 수행
        r = random.randint(0, len(mnist_test) - 1)
        X_single_data = mnist_test.test_data[r:r + 1].view(-1, 28 * 28).float().to(device)
        Y_single_data = mnist_test.test_labels[r:r + 1].to(device)

        print('Label: ', Y_single_data.item())
        single_prediction = linear(X_single_data)
        print('Prediction: ', torch.argmax(single_prediction, 1).item())

        plt.imshow(mnist_test.test_data[r:r + 1].view(28, 28),
                    cmap='Greys', interpolation='nearest')
        plt.show()



# _01_softmax_regression_implement()
_02_softmax_regression_mnist()