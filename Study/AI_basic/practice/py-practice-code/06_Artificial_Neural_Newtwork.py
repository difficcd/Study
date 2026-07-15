
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



# _01_Basic_concept()
_02_Perceptron()
