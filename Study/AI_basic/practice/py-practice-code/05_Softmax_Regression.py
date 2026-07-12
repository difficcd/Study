import torch
import torch.nn.functional as F
import torch.nn as nn
import torch.optim as optim

torch.manual_seed(1)

def _01_softmax_regression_implement():

    # 공통 data
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
    


_01_softmax_regression_implement()