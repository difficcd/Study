
import numpy as np # 넘파이 사용
import matplotlib.pyplot as plt # 맷플롯립 사용


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







_01_Basic_concept()
