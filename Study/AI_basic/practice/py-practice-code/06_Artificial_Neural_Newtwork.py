
import numpy as np              # 넘파이 사용
import matplotlib.pyplot as plt # 맷플롯립 사용

import torch            
import torch.nn as nn           
import torch.optim as optim
import numpy as np

import time                     # FLOPs 측정 실습
from sklearn.datasets import load_digits

from sklearn.datasets import fetch_openml # 전처리 필요 MNIST
from torch.utils.data import TensorDataset, DataLoader
from sklearn.model_selection import train_test_split




def _01_Basic_concept():

    #### 시그모이드 함수 
    def sigmoid(x):
        return 1/(1+np.exp(-x))
    x = np.arange(-5.0, 5.0, 0.1) 
    y = sigmoid(x)

    plt.plot(x, y)
    plt.plot([0,0],[1.0,0.0], ':') 
    plt.title('Sigmoid Function')
    plt.show()


    # 하이퍼볼릭 탄젠트 함수 (쌍곡탄젠트 tanh)
    y = np.tanh(x)

    plt.plot(x, y)
    plt.plot([0,0],[1.0,-1.0], ':')
    plt.axhline(y=0, color='orange', linestyle='--')
    plt.title('Tanh Function')
    plt.show()


    # ReLU 렐루 함수
    def relu(x):
        return np.maximum(0, x)
    y = relu(x)

    plt.plot(x, y)
    plt.plot([0,0],[5.0,0.0], ':')
    plt.title('Relu Function')
    plt.show()


    # Reaky ReLU 리키 렐루 함수
    a = 0.1
    def leaky_relu(x):
        return np.maximum(a*x, x)
    y = leaky_relu(x)

    plt.plot(x, y)
    plt.plot([0,0],[5.0,0.0], ':')
    plt.title('Leaky ReLU Function')
    plt.show()


    # Softmax 함수
    y = np.exp(x) / np.sum(np.exp(x))

    plt.plot(x, y)
    plt.title('Softmax Function')
    plt.show()

def _02_Perceptron():
    print("\n")

    def AND_gate(x1, x2):
        w1, w2, b = 0.5, 0.5, -0.7
        result = x1*w1 + x2*w2 + b
        if result <= 0:
            return 0
        else:
            return 1
    print( "AND gate: ",
        AND_gate(0, 0), AND_gate(0, 1), 
        AND_gate(1, 0), AND_gate(1, 1)
    )

    def NAND_gate(x1, x2):
        w1, w2, b = -0.5, -0.5, 0.7
        result = x1*w1 + x2*w2 + b
        if result <= 0:
            return 0
        else:
            return 1
    print ( "NAND gate: ",
        NAND_gate(0, 0), NAND_gate(0, 1), 
        NAND_gate(1, 0), NAND_gate(1, 1)
    )
    
    def OR_gate(x1, x2):
        w1, w2, b = 0.6, 0.6, -0.5
        result = x1*w1 + x2*w2 + b
        if result <= 0:
            return 0
        else:
            return 1
    print ( "OR gate: ",
        OR_gate(0, 0), OR_gate(0, 1), 
        OR_gate(1, 0), OR_gate(1, 1)
    )


    # === 숙제 : XOR gate 구현하기 === #
    def XOR_gate(x1, x2):
        s1 = NAND_gate(x1,x2)
        s2 = OR_gate(x1,x2)
        y = AND_gate(s1,s2)
        return y

    print ( "XOR gate: ",
        XOR_gate(0, 0), XOR_gate(0, 1), 
        XOR_gate(1, 0), XOR_gate(1, 1)
    )

def _03_Perceptron_XOR():

    def Single_Layer_Perceptron():

        device = 'cuda' if torch.cuda.is_available() else 'cpu'
        torch.manual_seed(777)
        if device == 'cuda':
            torch.cuda.manual_seed_all(777)

        X = torch.FloatTensor([[0, 0], [0, 1], [1, 0], [1, 1]]).to(device)
        Y = torch.FloatTensor([[0], [1], [1], [0]]).to(device)

        linear = nn.Linear(2, 1, bias=True)
        sigmoid = nn.Sigmoid()
        model = nn.Sequential(linear, sigmoid).to(device) # Single Layer

        # 비용 함수와 옵티마이저 정의 ()
        criterion = torch.nn.BCELoss().to(device)
        optimizer = torch.optim.SGD(model.parameters(), lr=1)
    
        for step in range(10001): # TRAIN
            optimizer.zero_grad()
            hypothesis = model(X)

            cost = criterion(hypothesis, Y) # 비용 함수
            cost.backward()
            optimizer.step()

            if step % 100 == 0: # 100번째 에포크마다 비용 출력
                print(step, cost.item())

        with torch.no_grad(): # TEST
            hypothesis = model(X)
            predicted = (hypothesis > 0.5).float()
            accuracy = (predicted == Y).float().mean()
            
            print('모델의 출력값(Hypothesis): ', hypothesis.detach().cpu().numpy())
            print('모델의 예측값(Predicted): ', predicted.detach().cpu().numpy())
            print('실제값(Y): ', Y.cpu().numpy())
            print('정확도(Accuracy): ', accuracy.item())
    # Single_Layer_Perceptron()


    def Multi_Layer_Perceptron():
        device = 'cuda' if torch.cuda.is_available() else 'cpu'

        # for reproducibility
        torch.manual_seed(777)
        if device == 'cuda':
            torch.cuda.manual_seed_all(777)

        X = torch.FloatTensor([[0, 0], [0, 1], [1, 0], [1, 1]]).to(device)
        Y = torch.FloatTensor([[0], [1], [1], [0]]).to(device)

        model = nn.Sequential(
          nn.Linear(2, 10, bias=True), # input_layer = 2, hidden_layer1 = 10
          nn.Sigmoid(),
          nn.Linear(10, 10, bias=True), # hidden_layer1 = 10, hidden_layer2 = 10
          nn.Sigmoid(),
          nn.Linear(10, 10, bias=True), # hidden_layer2 = 10, hidden_layer3 = 10
          nn.Sigmoid(),
          nn.Linear(10, 1, bias=True), # hidden_layer3 = 10, output_layer = 1
          nn.Sigmoid()
          ).to(device)
        
        criterion = torch.nn.BCELoss().to(device)
        optimizer = torch.optim.SGD(model.parameters(), lr=1)  
                    # modified learning rate from 0.1 to 1
                
        for epoch in range(10001):
            optimizer.zero_grad()
            # forward 연산
            hypothesis = model(X)

            # 비용 함수
            cost = criterion(hypothesis, Y)
            cost.backward()
            optimizer.step()

            # 100의 배수에 해당되는 에포크마다 비용을 출력
            if epoch % 100 == 0:
                print(epoch, cost.item())

        with torch.no_grad():
            hypothesis = model(X)
            predicted = (hypothesis > 0.5).float()
            accuracy = (predicted == Y).float().mean()
            print('모델의 출력값(Hypothesis): ', hypothesis.detach().cpu().numpy())
            print('모델의 예측값(Predicted): ', predicted.detach().cpu().numpy())
            print('실제값(Y): ', Y.cpu().numpy())
            print('정확도(Accuracy): ', accuracy.item())
    # Multi_Layer_Perceptron()

    
    # 추가탐구 ) playground.tensorflow 참조
    #           cpu 로 돌리기엔 규모가 크니 colab 등 gpu로 실행하기.
    #           (1000(data) / 256(batch)) x 20000 (epochs) : 성능 측정도 실습
    def _Two_Spirals_practice():
        def make_spirals(n_points=500, noise=0.3, scale=10.0):
            """scale 파라미터로 데이터 스케일 조정"""
            n = np.sqrt(np.random.rand(n_points, 1)) * 780 * (2 * np.pi) / 360
            d1x = (-np.cos(n) * n + np.random.rand(n_points, 1) * noise) * scale
            d1y = ( np.sin(n) * n + np.random.rand(n_points, 1) * noise) * scale
            X0 = np.hstack((d1x, d1y));  y0 = np.zeros((n_points, 1)) # 1번쨰 소용돌이
            X1 = np.hstack((-d1x, -d1y)); y1 = np.ones((n_points, 1)) # 2번째 소용돌이
            X = np.vstack((X0, X1)); y = np.vstack((y0, y1))
            return torch.FloatTensor(X), torch.FloatTensor(y)


        class DeepNet(nn.Module):
            """8층 깊은 네트워크 — lr 폭발 조건"""
            def __init__(self):
                super().__init__()
                self.net = nn.Sequential(
                    nn.Linear(2, 64),  nn.ReLU(),
                    nn.Linear(64, 64), nn.ReLU(),
                    nn.Linear(64, 64), nn.ReLU(),
                    nn.Linear(64, 32), nn.ReLU(),
                    nn.Linear(32, 32), nn.ReLU(),
                    nn.Linear(32, 16), nn.ReLU(),
                    nn.Linear(16, 8),  nn.ReLU(),
                    nn.Linear(8, 1),   nn.Sigmoid(),
                )
            def forward(self, x):
                return self.net(x)


        # === FLOPs 직접 세기 ===
        # Linear 층 하나의 순전파 FLOPs = 2 × 입력차원 × 출력차원 (곱셈 + 덧셈)
        # 역전파 ≈ 순전파의 2배  →  학습 1스텝(샘플 1개) ≈ 순전파 × 3
        def count_forward_flops(model):
            flops = 0
            for m in model.modules():
                if isinstance(m, nn.Linear):
                    flops += 2 * m.in_features * m.out_features
            return flops  # 샘플 1개 순전파 기준


        def train(lr, epochs=20000, batch_size=256, scale=10.0, seed=42):
            torch.manual_seed(seed)
            np.random.seed(seed)

            X, y = make_spirals(n_points=500, noise=0.3, scale=scale)
            X = (X - X.mean(dim=0)) / X.std(dim=0) # 데이터 정규화
            device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
            X, y = X.to(device), y.to(device)

            model = DeepNet().to(device)
            criterion = nn.BCELoss()
            optimizer = optim.SGD(model.parameters(), lr=lr)

            dataset = torch.utils.data.TensorDataset(X, y)
            loader  = torch.utils.data.DataLoader(dataset, batch_size=batch_size, shuffle=True)

            print(f"\n{'='*45}")
            print(f"  lr={lr}  |  층=8  |  scale={scale}  |  batch={batch_size}")
            print(f"{'='*45}")

            start = time.time()

            for epoch in range(epochs + 1):
                model.train()
                for xb, yb in loader:
                    optimizer.zero_grad()
                    out = model(xb)
                    loss = criterion(out, yb)
                    if torch.isnan(loss):
                        print(f"Epoch {epoch:5d} | Loss: NaN ← 폭발!")
                        return
                    loss.backward()
                    optimizer.step()

                if epoch % 1000 == 0:
                    model.eval()
                    with torch.no_grad():
                        out = model(X)
                        total_loss = criterion(out, y).item()
                        acc = ((out > 0.5).float() == y).float().mean().item() * 100
                    print(f"Epoch {epoch:5d} | Loss: {total_loss:.4f} | Acc: {acc:.1f}%")
            
            elapsed = time.time() - start

            # ---- 오버헤드 측정 ----
            n_samples     = X.shape[0]                   # 1000
            iterations    = len(loader) * (epochs + 1)   # 총 업데이트 횟수
            total_samples = n_samples * (epochs + 1)     # 처리한 샘플 수 합산

            fwd_flops   = count_forward_flops(model)     # 샘플 1개 순전파
            total_flops = fwd_flops * 3 * total_samples  # 순전파 + 역전파(×2)

            samples_per_sec = total_samples / elapsed    # throughput
            time_per_step   = elapsed / iterations       # time / step

            print(f"─── 오버헤드 측정 ───")
            print(f"경과 시간        : {elapsed:.1f} s")
            print(f"iterations       : {iterations:,}")
            print(f"time / step      : {time_per_step * 1000:.3f} ms")
            print(f"throughput       : {samples_per_sec:,.0f} samples/sec")
            print(f"순전파 FLOPs/샘플: {fwd_flops:,}")
            print(f"총 학습 FLOPs    : {total_flops / 1e12:.2f} TFLOPs")
            print(f"실측 연산 성능   : {total_flops / elapsed / 1e9:.2f} GFLOPS")
            if torch.cuda.is_available():
                print(f"GPU 메모리       : {torch.cuda.memory_allocated() / 1024**2:.1f} MB")

        print("\n[실험 1] 데이터 스케일 10배 + 8층 + Mini-batch")
        print("─ lr=1.0")
        train(lr=1.0,  scale=10.0)
        print("\n─ lr=0.01")
        train(lr=0.01, scale=10.0)

        print("\n\n[실험 2] 스케일 더 키우기 (scale=50)")
        print("─ lr=1.0")
        train(lr=1.0,  scale=50.0)
        print("\n─ lr=0.01")
        train(lr=0.01, scale=50.0)
    _Two_Spirals_practice() 

def _04_BackPropagation():
        
        def BackPropagation():

            device = 'cuda' if torch.cuda.is_available() else 'cpu'
            torch.manual_seed(777)
            if device == 'cuda':
                torch.cuda.manual_seed_all(777)

            X = torch.FloatTensor([[0.1, 0.2]]).to(device) # (1,2)
            Y = torch.FloatTensor([[0.4, 0.6]]).to(device) # (1,2)

            class TwoLayerNet(nn.Module):
                def __init__(self):
                    super().__init__()
                    self.linear1 = nn.Linear(2, 2, bias=False)
                    self.linear2 = nn.Linear(2, 2, bias=False)
                    self.sigmoid = nn.Sigmoid()

                def forward(self, x):
                    z1 = self.linear1(x)          # 은닉층 선형 변환
                    h1 = self.sigmoid(z1)         # 은닉층 활성화
                    z2 = self.linear2(h1)         # 출력층 선형 변환
                    o  = self.sigmoid(z2)         # 출력층 활성화
                    return z1, h1, z2, o          # 중간값 전부 반환

            model = TwoLayerNet().to(device)

            # 비용 함수와 옵티마이저 정의 (회귀 문제 = MSE)
            criterion = torch.nn.MSELoss()
            optimizer = torch.optim.SGD(model.parameters(), lr=0.5)

            # 가중치 초기화 (신경망 조건과 동일하게 맞추기)
            # 0(1) 1(1sig) 2(2) 3(2sig)
            # 방향은 입력 데이터와의 행렬곱 쉬움: .1거는 .3, .25 ..
            with torch.no_grad(): 
                model.linear1.weight.copy_(torch.tensor([[0.3, 0.25],
                                                         [0.4, 0.35]]))
                model.linear2.weight.copy_(torch.tensor([[0.45, 0.4],
                                                         [0.7, 0.6]]))

            for step in range(3): # TRAIN
                optimizer.zero_grad()
                z1, h1, z2, o = model(X)

                cost = criterion(o, Y) 
                cost.backward()
                optimizer.step()

                print(f"\n=== step {step} ===")
                print(f"z1, z2 (은닉층 입력) : {z1.detach()}")
                print(f"h1, h2 (은닉층 출력) : {h1.detach()}")
                print(f"z3, z4 (출력층 입력) : {z2.detach()}")
                print(f"o1, o2 (예측값)      : {o.detach()}")
                print(f"\ncost: {cost.item():.8f}\n")
                print(f"W1~W4 :  \n{model.linear1.weight.data}")
                print(f"W5~W8 :  \n{model.linear2.weight.data}\n")
        BackPropagation()
    
def _05_load_digits_MLP():
    digits = load_digits() # 1,979개의 이미지 데이터 로드

    print(digits.images[0]) # (8,8)
    print('label: ', digits.target[0]) # label
    print('전체 샘플의 수 : {}'.format(len(digits.images)), '\n')

    images_and_labels = list(zip(digits.images, digits.target))
    for index, (image, label) in enumerate(images_and_labels[:5]): 
        # 5개의 샘플만 출력
        plt.subplot(2, 5, index + 1)
        plt.axis('off')
        plt.imshow(image, cmap=plt.cm.gray_r, interpolation='nearest')
        plt.title('sample: %i' % label)
    plt.show()

    for i in range(5):
        print(i,'번 인덱스 샘플의 레이블 : ',digits.target[i])
    print('\n', digits.data[0], '\n') # (8,8)이 아니라 (64,)로 바꾸어 샘플단위로 학습

    X = digits.data # 이미지. 즉, 특성 행렬
    Y = digits.target # 각 이미지에 대한 레이블

    # 모델 정의: 순차적인 레이어 구조
    model = nn.Sequential(
        nn.Linear(64, 32), # 입력층: 64, 첫 번째 은닉층: 32
        nn.ReLU(),         # 활성화 함수: ReLU
        nn.Linear(32, 16), # 첫 번째 은닉층: 32, 두 번째 은닉층: 16
        nn.ReLU(),         # 활성화 함수: ReLU
        nn.Linear(16, 10)  # 두 번째 은닉층: 16, 출력층: 10 (클래스의 개수)
    )

    # 입력 데이터 X와 레이블 Y를 텐서로 변환
    X = torch.tensor(X, dtype=torch.float32)
    Y = torch.tensor(Y, dtype=torch.int64)
    loss_fn = nn.CrossEntropyLoss() # 이 비용 함수는 소프트맥스 함수를 포함하고 있음.
    optimizer = optim.Adam(model.parameters())
    losses = []

    # 총 100번의 에포크 동안 모델 학습
    for epoch in range(100):
        optimizer.zero_grad()      # 옵티마이저의 기울기 초기화
        y_pred = model(X)          # 순전파 연산으로 예측값 계산
        loss = loss_fn(y_pred, Y)  # 손실 함수로 비용 계산
        loss.backward()            # 역전파 연산으로 기울기 계산
        optimizer.step()           # 옵티마이저를 통해 파라미터 업데이트

        # 10번째 에포크마다 현재 에포크와 손실 값 출력
        if epoch % 10 == 0:
            print('Epoch {:4d}/{} Cost: {:.6f}'.format(
                    epoch, 100, loss.item()
                ))

        # 손실 값을 리스트에 추가하여 추적
        losses.append(loss.item())

    plt.plot(losses)
    plt.show()
            
def _06_MNIST_MLP():
    mnist = fetch_openml('mnist_784', version=1, 
                         cache=True, as_frame=False)
    mnist.data[0]
    mnist.target[0]
    mnist.target = mnist.target.astype(np.int8) # 타입변환 

    X = mnist.data / 255
    y = mnist.target
    # 0-255 범위에서 0-1 범위로 정규화 (학습 효율)

    print("X[0]:\n", X[0], "\ny[0]", y[0])

    plt.imshow(X[0].reshape(28, 28), cmap='gray')
    print("\n이 이미지 데이터의 레이블은 {:.0f}이다\b".format(y[0]))

    X_train, X_test, y_train, y_test = train_test_split(
        X, y, test_size=1/7, random_state=0
    )

    # 텐서로 변환
    X_train = torch.Tensor(X_train)
    X_test = torch.Tensor(X_test)
    y_train = torch.LongTensor(y_train)
    y_test = torch.LongTensor(y_test)
        # CrossEntropyLoss = label 을 int64 롱텐서로 요구함
        # pytorch 내부에서 레이블을 인덱스로 사용하기 때문.

    # TensorDataset 객체 생성
    ds_train = TensorDataset(X_train, y_train)
    ds_test = TensorDataset(X_test, y_test)

    # DataLoader 객체 생성
    loader_train = DataLoader(ds_train, batch_size=64, shuffle=True)
    loader_test = DataLoader(ds_test, batch_size=64, shuffle=False)

    model = nn.Sequential()
    model.add_module('fc1', nn.Linear(28*28*1, 100))
    model.add_module('relu1', nn.ReLU())
    model.add_module('fc2', nn.Linear(100, 100))
    model.add_module('relu2', nn.ReLU())
    model.add_module('fc3', nn.Linear(100, 10))

    print('\nmodel:\n', model, '\n')

    # 오차함수 선택
    loss_fn = nn.CrossEntropyLoss()

    # 가중치를 학습하기 위한 최적화 기법 선택
    optimizer = optim.Adam(model.parameters(), lr=0.01)

    epochs = 3
    for epoch in range(epochs): # TRAIN LOOP
        for data, targets in loader_train:
            optimizer.zero_grad()            # 옵티마이저의 기울기 초기화
            y_pred = model(data)             # 순전파 연산으로 예측값 계산
            loss = loss_fn(y_pred, targets)  # 손실 함수로 비용 계산
            loss.backward()                  # 역전파 연산으로 기울기 계산
            optimizer.step()                 # 옵티마이저를 통해 파라미터 업데이트

        print('Epoch {:4d}/{} Cost: {:.6f}'.format(
               epoch + 1, 3,        loss.item()))
        

    # ==== TRAIN => TEST ==== #

    model.eval()  # 신경망을 추론 모드로 전환 : TEST (관습으로 넣음)
    correct = 0

    # 데이터로더에서 미니배치를 하나씩 꺼내 추론을 수행
    with torch.no_grad():  # 추론 과정에는 미분이 필요없음
        for data, targets in loader_test:

            outputs = model(data)  # 데이터를 입력하고 출력을 계산

            # 추론 계산
            _, predicted = torch.max(outputs.data, 1)  
                # 확률이 가장 높은 레이블이 무엇인지 계산
            correct += predicted.eq(targets.data.view_as(predicted)).sum()  
                # 정답과 일치한 경우 정답 카운트를 증가

    # 정확도 출력
    data_num = len(loader_test.dataset)  # 데이터 총 건수
    print('\n테스트 데이터에서 예측 정확도: {}/{} ({:.0f}%)\n'.format(
            correct, data_num, 100. * correct / data_num ))
    

    # ===== 임의 데이터에 대한 TEST ===== #
    
    index = 2018
    model.eval()  # 신경망을 추론 모드로 전환
    data = X_test[index]
    output = model(data)                      # 데이터를 입력하고 출력을 계산
    _, predicted = torch.max(output.data, 0)  # 확률이 가장 높은 레이블이 무엇인지 계산

    print("예측 결과 : {}".format(predicted))

    X_test_show = (X_test[index]).numpy()
    plt.imshow(X_test_show.reshape(28, 28), cmap='gray')
    print("이 이미지 데이터의 정답 레이블은 {:.0f}입니다\n".format(y_test[index]))

def _07_prevent_Overfitting():

    def complexity():
        # 데이터 생성 (적은 수 + 노이즈)
        np.random.seed(42)
        X_train = np.linspace(-3, 3, 20)          # 훈련 데이터 20개만
        y_train = np.sin(X_train) + np.random.normal(0, 0.3, 20) 
                                                # 노이즈 추가

        X_test = np.linspace(-3, 3, 100)          # 테스트 200개
        y_test = np.sin(X_test)                   # 노이즈 없는 정답

        # 텐서 변환
        X_train_t = torch.FloatTensor(X_train).unsqueeze(1)
        y_train_t = torch.FloatTensor(y_train).unsqueeze(1)
        X_test_t  = torch.FloatTensor(X_test).unsqueeze(1)
        y_test_t  = torch.FloatTensor(y_test).unsqueeze(1)

        # 복잡한 모델 (과적합 유도)
        class Architecture1(nn.Module):
            def __init__(self, input_size, hidden_size, num_classes):
                super(Architecture1, self).__init__()
                self.fc1 = nn.Linear(input_size, hidden_size)
                self.relu = nn.ReLU()
                self.fc2 = nn.Linear(hidden_size, hidden_size)
                self.fc3 = nn.Linear(hidden_size, hidden_size)
                self.fc4 = nn.Linear(hidden_size, num_classes)

            def forward(self, x):
                out = self.fc1(x)
                out = self.relu(out)
                out = self.fc2(out)
                out = self.relu(out)
                out = self.fc3(out)
                out = self.relu(out)
                out = self.fc4(out) 
                return out # 층을 4개로 깊게 쌓아보기
        
        class Architecture2(nn.Module):
            def __init__(self, input_size, hidden_size, num_classes):
                super(Architecture2, self).__init__()
                self.fc1 = nn.Linear(input_size, hidden_size)
                self.relu = nn.ReLU()
                self.fc2 = nn.Linear(hidden_size, num_classes)

            def forward(self, x):
                out = self.fc1(x)
                out = self.relu(out)
                out = self.fc2(out)
                return out

        # 모델 생성
        model = [
            Architecture1(input_size=1, hidden_size=512, num_classes=1),
            Architecture2(input_size=1, hidden_size=128, num_classes=1)
        ]

        for i in range(2): 
            optimizer = torch.optim.Adam(model[i].parameters(), lr=0.01)
            criterion = nn.MSELoss()

            print('\n\n model ', i, '  parameter:\n')
            for param in model[i].parameters(): print(param.shape)
            print('\n')

            train_losses = []
            test_losses  = []

            for epoch in range(3000):
                model[i].train()
                optimizer.zero_grad()
                pred = model[i](X_train_t)
                loss = criterion(pred, y_train_t)
                loss.backward()
                optimizer.step()

                model[i].eval()
                with torch.no_grad():
                    test_loss = criterion(model[i](X_test_t), y_test_t)

                train_losses.append(loss.item())
                test_losses.append(test_loss.item())

                if epoch % 500 == 0:
                    print(f"Epoch {epoch} | Train: {loss.item():.4f} | Test: {test_loss.item():.4f}")

            # 그래프
            plt.figure(figsize=(12, 4))

            plt.subplot(1, 2, 1)
            plt.plot(train_losses, label='Train Loss')
            plt.plot(test_losses,  label='Test Loss')

            # y축 범위 가공
            cutoff = 40  # 초반 40 epoch 제거
            max_y = max(max(train_losses[cutoff:]), max(test_losses[cutoff:]))
            plt.ylim(0, max_y * 1.5)  # 상단 여유 1.5배

            plt.legend()
            plt.title('Loss graph')

            plt.subplot(1, 2, 2)
            model[i].eval()
            with torch.no_grad():
                pred_line = model[i](X_test_t).numpy()
            plt.scatter(X_train, y_train, label='train_data')
            plt.plot(X_test, y_test,     label='label')
            plt.plot(X_test, pred_line,  label='model-prediction')
            plt.legend()
            plt.title('prediction result')
            plt.show()
    # complexity()

    def regularization():
         # 데이터 생성 (적은 수 + 노이즈)
        np.random.seed(42)
        X_train = np.linspace(-3, 3, 20)          # 훈련 데이터 20개만
        y_train = np.sin(X_train) + np.random.normal(0, 0.3, 20) 
                                                # 노이즈 추가

        X_test = np.linspace(-3, 3, 100)          # 테스트 200개
        y_test = np.sin(X_test)                   # 노이즈 없는 정답

        # 텐서 변환
        X_train_t = torch.FloatTensor(X_train).unsqueeze(1)
        y_train_t = torch.FloatTensor(y_train).unsqueeze(1)
        X_test_t  = torch.FloatTensor(X_test).unsqueeze(1)
        y_test_t  = torch.FloatTensor(y_test).unsqueeze(1)

        # 복잡한 모델 (과적합 유도)
        class Architecture1(nn.Module):
            def __init__(self, input_size, hidden_size, num_classes):
                super(Architecture1, self).__init__()
                self.fc1 = nn.Linear(input_size, hidden_size)
                self.relu = nn.ReLU()
                self.fc2 = nn.Linear(hidden_size, hidden_size)
                self.fc3 = nn.Linear(hidden_size, hidden_size)
                self.fc4 = nn.Linear(hidden_size, num_classes)

            def forward(self, x):
                out = self.fc1(x)
                out = self.relu(out)
                out = self.fc2(out)
                out = self.relu(out)
                out = self.fc3(out)
                out = self.relu(out)
                out = self.fc4(out) 
                return out # 층을 4개로 깊게 쌓아보기

        # 모델 생성
        model = [
            Architecture1(input_size=1, hidden_size=512, num_classes=1),
            Architecture1(input_size=1, hidden_size=512, num_classes=1)
        ]

        for i in range(2): 

            if i == 0:
                optimizer = torch.optim.Adam(model[i].parameters(), lr=0.01)
            else:
                time.sleep(30)
                optimizer = torch.optim.Adam(model[i].parameters(), lr=0.01,
                                             weight_decay=1e-4, foreach=True)


            criterion = nn.MSELoss()

            print('\n\n model ', i, '  parameter:\n')
            for param in model[i].parameters(): print(param.shape)
            print('\n')

            train_losses = []
            test_losses  = []

            for epoch in range(3000):
                model[i].train()
                optimizer.zero_grad()
                pred = model[i](X_train_t)
                loss = criterion(pred, y_train_t)
                loss.backward()
                optimizer.step()

                model[i].eval()
                with torch.no_grad():
                    test_loss = criterion(model[i](X_test_t), y_test_t)

                train_losses.append(loss.item())
                test_losses.append(test_loss.item())

                if epoch % 500 == 0:
                    print(f"Epoch {epoch} | Train: {loss.item():.4f} | Test: {test_loss.item():.4f}")

            # 그래프
            plt.figure(figsize=(12, 4))

            plt.subplot(1, 2, 1)
            plt.plot(train_losses, label='Train Loss')
            plt.plot(test_losses,  label='Test Loss')

            plt.legend()
            plt.title('Loss graph')

             # y축 범위 가공
            cutoff = 30  # 초반 40 epoch 제거
            max_y = max(max(train_losses[cutoff:]), max(test_losses[cutoff:]))
            plt.ylim(0, max_y * 1.5)  # 상단 여유 1.5배

            plt.subplot(1, 2, 2)
            model[i].eval()
            with torch.no_grad():
                pred_line = model[i](X_test_t).numpy()
            plt.scatter(X_train, y_train, label='train_data')
            plt.plot(X_test, y_test,     label='label')
            plt.plot(X_test, pred_line,  label='model-prediction')
            plt.legend()
            plt.title('prediction result')
            plt.show()
    # regularization()

    def dropout():
         # 데이터 생성 (적은 수 + 노이즈)
        np.random.seed(42)
        X_train = np.linspace(-3, 3, 20)          # 훈련 데이터 20개만
        y_train = np.sin(X_train) + np.random.normal(0, 0.3, 20) 
                                                # 노이즈 추가

        X_test = np.linspace(-3, 3, 100)          # 테스트 200개
        y_test = np.sin(X_test)                   # 노이즈 없는 정답

        # 텐서 변환
        X_train_t = torch.FloatTensor(X_train).unsqueeze(1)
        y_train_t = torch.FloatTensor(y_train).unsqueeze(1)
        X_test_t  = torch.FloatTensor(X_test).unsqueeze(1)
        y_test_t  = torch.FloatTensor(y_test).unsqueeze(1)

        # 복잡한 모델 (과적합 유도)
        class Architecture1(nn.Module):
            def __init__(self, input_size, hidden_size, num_classes):
                super(Architecture1, self).__init__()
                self.fc1 = nn.Linear(input_size, hidden_size)
                self.relu = nn.ReLU()
                self.fc2 = nn.Linear(hidden_size, hidden_size)
                self.fc3 = nn.Linear(hidden_size, hidden_size)
                self.fc4 = nn.Linear(hidden_size, num_classes)

            def forward(self, x):
                out = self.fc1(x)
                out = self.relu(out)
                out = self.fc2(out)
                out = self.relu(out)
                out = self.fc3(out)
                out = self.relu(out)
                out = self.fc4(out) 
                return out 
            
        class Architecture2(nn.Module):
            def __init__(self, input_size, hidden_size, num_classes):
                super(Architecture2, self).__init__()
                self.fc1 = nn.Linear(input_size, hidden_size)
                self.relu = nn.ReLU()
                self.dropout = nn.Dropout(p=0.3) 
                self.fc2 = nn.Linear(hidden_size, hidden_size)
                self.fc3 = nn.Linear(hidden_size, hidden_size)
                self.fc4 = nn.Linear(hidden_size, num_classes)

            def forward(self, x):
                out = self.fc1(x)
                out = self.relu(out)
                out = self.dropout(out)
                out = self.fc2(out)
                out = self.relu(out)
                out = self.dropout(out)
                out = self.fc3(out)
                out = self.relu(out)
                out = self.dropout(out)
                out = self.fc4(out) 
                return out 

        # 모델 생성
        model = [
            Architecture1(input_size=1, hidden_size=512, num_classes=1),
            Architecture2(input_size=1, hidden_size=512, num_classes=1)
        ]

        for i in range(2): 

            if i == 0:
                optimizer = torch.optim.Adam(model[i].parameters(), lr=0.01)
            else:
                time.sleep(30)
                optimizer = torch.optim.Adam(model[i].parameters(), lr=0.01,
                                             weight_decay=1e-4, foreach=True)


            criterion = nn.MSELoss()

            print('\n\n model ', i, '  parameter:\n')
            for param in model[i].parameters(): print(param.shape)
            print('\n')

            train_losses = []
            test_losses  = []

            for epoch in range(3000):
                model[i].train()
                optimizer.zero_grad()
                pred = model[i](X_train_t)
                loss = criterion(pred, y_train_t)
                loss.backward()
                optimizer.step()

                model[i].eval()
                with torch.no_grad():
                    test_loss = criterion(model[i](X_test_t), y_test_t)

                train_losses.append(loss.item())
                test_losses.append(test_loss.item())

                if epoch % 500 == 0:
                    print(f"Epoch {epoch} | Train: {loss.item():.4f} | Test: {test_loss.item():.4f}")

            # 그래프
            plt.figure(figsize=(12, 4))

            plt.subplot(1, 2, 1)
            plt.plot(train_losses, label='Train Loss')
            plt.plot(test_losses,  label='Test Loss')

            plt.legend()
            plt.title('Loss graph')

             # y축 범위 가공
            cutoff = 30  # 초반 40 epoch 제거
            max_y = max(max(train_losses[cutoff:]), max(test_losses[cutoff:]))
            plt.ylim(0, max_y * 1.5)  # 상단 여유 1.5배

            plt.subplot(1, 2, 2)
            model[i].eval()
            with torch.no_grad():
                pred_line = model[i](X_test_t).numpy()
            plt.scatter(X_train, y_train, label='train_data')
            plt.plot(X_test, y_test,     label='label')
            plt.plot(X_test, pred_line,  label='model-prediction')
            plt.legend()
            plt.title('prediction result')
            plt.show()
    dropout()


# _01_Basic_concept()
# _02_Perceptron()
# _03_Perceptron_XOR()
# _04_BackPropagation()
# _05_load_digits_MLP()
# _06_MNIST_MLP()
_07_prevent_Overfitting()