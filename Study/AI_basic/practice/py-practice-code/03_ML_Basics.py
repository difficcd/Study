
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim

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



