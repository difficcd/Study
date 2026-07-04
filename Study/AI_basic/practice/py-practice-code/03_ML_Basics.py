
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim


def _01_Linear_Regression():

    # 선형 회귀 실습 (SGD 함수 사용)
    def practice():
        # 현재 실습하고 있는 파이썬 코드를 재실행해도
        #  다음에도 같은 결과가 나오도록 랜덤 시드(random seed) 주기
        torch.manual_seed(1)

        # 훈련 데이터 선언
        x_train = torch.FloatTensor([[1], [2], [3]]) #(3,1)
        y_train = torch.FloatTensor([[2], [4], [6]]) #(3,1)

        print('\n')
        print('[x_train]\n', x_train)
        print(x_train.shape, '\n')

        print('[y_train]\n',y_train)
        print(y_train.shape, '\n\n')


        # (1) 가중치, 편향 setting
            # 가중치 W를 0으로 초기화 (zeros)
            # requires_grad : 학습 통해 값이 변경되는 변수 명시
        W = torch.zeros(1, requires_grad=True) 
        print('Weight: ', W)  # 가중치 W
        b = torch.zeros(1, requires_grad=True) 
        print('Bias: ', b, '\n\n')  # 편향 b


        # (4) 경사 하강법 설정 (학습률 0.01)
        optimizer = optim.SGD([W, b], lr=0.01)


        # shape 확인하기
        hypothesis = x_train * W + b 
        print('[hypothesis]\n', hypothesis, '\n') 
        print('[Err(Sub)]\n', hypothesis - y_train, '\n')
        cost = torch.mean((hypothesis - y_train) ** 2) 
        print('cost: ', cost, '\n')
        print('[optimizer]\n', optimizer, '\n\n')



        nb_epochs = 2000 # 원하는만큼 경사 하강법을 반복
        for epoch in range(nb_epochs + 1):

            # (2) 가설 세우기
            hypothesis = x_train * W + b  # H(x) = Wx + b
            
            # (3) cost function
            # 앞서 배운 torch.mean으로 평균 도출
            cost = torch.mean((hypothesis - y_train) ** 2) 

            optimizer.zero_grad()   # gradient를 0으로 초기화
            cost.backward()         # cost fun 미분 gradient 계산
            optimizer.step()        # W와 b를 업데이트 (W=W-lr x dL/dW)

            # 100번마다 로그 출력
            if epoch % 100 == 0:
                print('Epoch {:4d}/{} W: {:.3f}, b: {:.3f} Cost: {:.6f}'
                    .format(
                    epoch, nb_epochs, W.item(), b.item(), cost.item()
                ))
    
    practice()


    # optimizer.zero_grad()가 필요한 이유
    def zero_grad():
        print('\n')
        w = torch.tensor(2.0, requires_grad=True)

        nb_epochs = 20
        for epoch in range(nb_epochs + 1):
            z = 2*w
            z.backward()
            
            print('수식을 w로 미분한 값 : {}'.format(w.grad))
            # backward 역방향 미분 한 값을 계속 누적함 
            # 누적값X, 업데이트한 값 기준으로 연산 필요: zero_grad사용

    zero_grad()


    # torch.manual_seed()를 하는 이유
    def manual_seed():
        print('\n')

        torch.manual_seed(3)
        print('랜덤 시드가 3일 때')
        for i in range(1,3):
            print(torch.rand(1))

        torch.manual_seed(5)
        print('랜덤 시드가 5일 때')
        for i in range(1,3):
            print(torch.rand(1))

        torch.manual_seed(3)
        print('랜덤 시드가 3일 때(RE)')
        for i in range(1,3):
            print(torch.rand(1))

    manual_seed()


    def Autograd():
        w = torch.tensor(2.0, requires_grad=True)
            # requires_grad : w.grad에 w에대한 미분값 저장.

        y = w**2
        z = 2*y + 5

        z.backward() # z 라는 수식에 대한 w의 기울기 계산
        print('\n 수식을 w로 미분한 값 : {}'.format(w.grad))

        # y.backward : y'=2w, w=2.0 => 4.0
        # z.backward : z'=2y'=> 2*4 => 8.0
    
    Autograd()



def _02_multiple_linear_regression():

    def implement_basic():
        torch.manual_seed(1)
        print('\n')

        # 훈련 데이터
        x1_train = torch.FloatTensor([[73], [93], [89], [96], [73]]) # (5x1)
        x2_train = torch.FloatTensor([[80], [88], [91], [98], [66]]) # (5x1)
        x3_train = torch.FloatTensor([[75], [93], [90], [100], [70]]) # (5x1)
        y_train = torch.FloatTensor([[152], [185], [180], [196], [142]]) 

        # 가중치 w와 편향 b 초기화
        w1 = torch.zeros(1, requires_grad=True)
        w2 = torch.zeros(1, requires_grad=True)
        w3 = torch.zeros(1, requires_grad=True)
        b = torch.zeros(1, requires_grad=True)

            # optimizer 설정
        optimizer = optim.SGD([w1, w2, w3, b], lr=1e-5)

        nb_epochs = 1000
        
        for epoch in range(nb_epochs + 1):

            # 가설 H(x) 선언 
            hypothesis = x1_train * w1 + x2_train * w2 + x3_train * w3 + b
            cost = torch.mean((hypothesis - y_train) ** 2) # MSE 

            # cost로 H(x) 개선
            optimizer.zero_grad()
            cost.backward()
            optimizer.step()

            # 100번마다 로그 출력
            if epoch % 100 == 0:
                print('Epoch {:4d}/{} w1: {:.3f} w2: {:.3f} w3: {:.3f} ' \
                    'b: {:.3f} Cost: {:.6f}'.format(
                    epoch, nb_epochs, 
                    w1.item(), w2.item(), w3.item(), b.item(), cost.item()
                ))
    implement_basic()
    
    def implement_matrix():
        print('\n')
        x_train  =  torch.FloatTensor([[73,  80,  75], 
                                       [93,  88,  93], 
                                       [89,  91,  80], 
                                       [96,  98,  100],   
                                       [73,  66,  70]])  # shape : (5,3)
        y_train  =  torch.FloatTensor([[152],  [185],  [180],  [196],  [142]])
                    # shape : (5,1)

        """ # 훈련 데이터 (basic 방법으로는 (5x1)x3 선언. 현재는 (5,3) 통합)
        x1_train = torch.FloatTensor([[73], [93], [89], [96], [73]]) # (5x1)
        x2_train = torch.FloatTensor([[80], [88], [91], [98], [66]]) # (5x1)
        x3_train = torch.FloatTensor([[75], [93], [90], [100], [70]]) # (5x1)
        y_train = torch.FloatTensor([[152], [185], [180], [196], [142]]) 
        """

        # 가중치와 편향 선언
        W = torch.zeros((3, 1), requires_grad=True)
        b = torch.zeros(1, requires_grad=True)

        optimizer = optim.SGD([W, b], lr=1e-5)


        nb_epochs = 20
        for epoch in range(nb_epochs + 1):

            # H(x) 계산
            # 편향 b는 브로드 캐스팅되어 각 샘플에 더해짐.
            hypothesis = x_train.matmul(W) + b   # H(X) = XW + B
            cost = torch.mean((hypothesis - y_train) ** 2)  # MSE

            optimizer.zero_grad()
            cost.backward()
            optimizer.step()

            print('Epoch {:4d}/{} hypothesis: {} Cost: {:.6f}'.format(
                epoch, nb_epochs, hypothesis.squeeze().detach(), cost.item()
            ))


        # 임의의 입력 값에 대한 예측 
        # no_grad : 자동미분 그래프 기록 off (역전파 기울기 계산 등을 비활성화)
        with torch.no_grad():
            new_input = torch.FloatTensor([[75, 85, 72]])  # 임의의 입력
            prediction = new_input.matmul(W) + b
            print('Predicted value for input {}: {}'
                .format(new_input.squeeze().tolist(), prediction.item()))
    implement_matrix()



def _03_pytorch_linear_regression():
    def _1t_linear_regression():
        print('\n')
        torch.manual_seed(1)
        
        x_train = torch.FloatTensor([[1], [2], [3]])
        y_train = torch.FloatTensor([[2], [4], [6]])

        # 모델 선언/초기화) 단순 선형 회귀이므로 input_dim=1, output_dim=1.
        model = nn.Linear(1,1)
        print(list(model.parameters())) # 모델이 가진 W, b 출력 (랜덤 초기화됨)

        # optimizer 설정. 경사 하강법 SGD를 사용하고 learning rate를 의미하는 lr은 0.01
        optimizer = torch.optim.SGD(model.parameters(), lr=0.01) 

        # 전체 훈련 데이터에 대해 경사 하강법을 2,000회 반복
        nb_epochs = 2000
        for epoch in range(nb_epochs+1):

            prediction = model(x_train) # H(x) 계산 (nn.Linear 자체적으로 식 구성)
            cost = F.mse_loss(prediction, y_train) # <== 파이토치에서 제공하는 MSE
            
            # cost로 H(x) 개선하는 부분
            optimizer.zero_grad()   # gradient를 0으로 초기화
            cost.backward()         # 비용 함수를 미분하여 gradient 계산
            optimizer.step()        # W와 b를 업데이트

            if epoch % 100 == 0:
                # 100번마다 로그 출력
                print('Epoch {:4d}/{} Cost: {:.6f}'.format(
                    epoch, nb_epochs, cost.item()
                ))

        #### 예측 테스트
        # 임의의 입력 4를 선언
        new_var =  torch.FloatTensor([[4.0]]) 

        # 입력한 값 4에 대해서 예측값 y를 리턴받아서 pred_y에 저장
        pred_y = model(new_var) # forward 연산

        # y = 2x 이므로 입력이 4라면 y가 8에 가까운 값이 나와야 제대로 학습이 된 것
        print("훈련 후 입력이 4일 때의 예측값 :", pred_y) 

        print(list(model.parameters()))
    _1t_linear_regression()

    def _2t_multiple_linear_regression():
        print('\n')
        torch.manual_seed(1)
        x_train = torch.FloatTensor([[73, 80, 75],
                                    [93, 88, 93],
                                    [89, 91, 90],
                                    [96, 98, 100],
                                    [73, 66, 70]]) 
        y_train = torch.FloatTensor([[152], [185], [180], [196], [142]])

        # 모델을 선언 및 초기화. 다중 선형 회귀이므로 input_dim=3, output_dim=1.
        # dim : col rank : (5x3) (3x1)
        model = nn.Linear(3,1)
        print(list(model.parameters())) # 3,1 dim 이므로 3개의 W, 1개의 b
        optimizer = torch.optim.SGD(model.parameters(), lr=1e-5)  
            # 다중회귀이므로 더 적은 학습률 ( lr=0.00001 )
        
        nb_epochs = 2000
        for epoch in range(nb_epochs+1): # 동일한 코드
            prediction = model(x_train)
            cost = F.mse_loss(prediction, y_train) 
            optimizer.zero_grad()
            cost.backward()
            optimizer.step()

            if epoch % 100 == 0:
                print('Epoch {:4d}/{} Cost: {:.6f}'.format(
                    epoch, nb_epochs, cost.item()
                ))

        #### 예측 
        new_var =  torch.FloatTensor([[73, 80, 75]]) 
        # 입력한 값 [73, 80, 75]에 대해서 예측값 y를 리턴받아서 pred_y에 저장
        pred_y = model(new_var) 
        print("훈련 후 입력이 73, 80, 75일 때의 예측값 :", pred_y) 

        print(list(model.parameters())) # 학습 후 W, b 



    _2t_multiple_linear_regression()





def _pytorch_Autograd_understand():
    x1 = torch.tensor(2.0, requires_grad=True)
    x2 = torch.tensor(3.0, requires_grad=True)

    w1 = torch.sin(x1)      # w1 = sin(x1)
    w2 = x1 * x2            # w2 = x1*x2
    f = w1 + w2             # f = w1 + w2

    f.backward()             # 이 한 줄이 자동 미분 전체를 수행

    print(x1.grad)           # ∂f/∂x1 = cos(x1) + x2
    print(x2.grad)           # ∂f/∂x2 = x1



#_01_Linear_Regression()
#_02_multiple_linear_regression()
_03_pytorch_linear_regression()

#_pytorch_Autograd_understand()


