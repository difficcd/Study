
import numpy as np              # 넘파이 사용
import matplotlib.pyplot as plt # 맷플롯립 사용

import torch            
import torch.nn as nn           
import torch.optim as optim
import numpy as np

import time                     # FLOPs 측정 실습



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
    
        

# _01_Basic_concept()
# _02_Perceptron()
# _03_Perceptron_XOR()
_04_BackPropagation()

