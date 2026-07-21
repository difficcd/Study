
> 이번 챕터에서는 인공지능에 입문할 때 가장 먼저 배우게 되는 머신 러닝 모델 중 하나인 선형 회귀(Linear Regression)을 딥 러닝 프레임워크 파이토치(PyTorch)로 구현합니다. 이를 통해 딥 러닝을 이해하기 위한 초석이 되는 개념들을 공부합니다.


# 03-01 선형 회귀, 자동 미분
이번 챕터에서는 선형 회귀 이론에 대해서 이해하고, 
파이토치(PyTorch)를 이용하여 선형 회귀 모델을 만들어보겠습니다.

- **1 데이터에 대한 이해(Data Definition)**  
    학습할 데이터에 대해서 알아봅니다.
    
- **2 가설(Hypothesis) 수립**  
    가설을 수립하는 방법에 대해서 알아봅니다.
    
- **3 손실 계산하기(Compute loss)**  
    학습 데이터를 이용해서 연속적으로 모델을 개선시키는데 
    이 때 손실(loss)를 이용합니다. 
    
- **4 경사 하강법(Gradient Descent)**  
    학습을 위한 핵심 알고리즘인 
    경사 하강법(Gradient Descent)에 대해서 이해합니다.
    
## 1. 데이터에 대한 이해(Data Definition)

이번 챕터에서 선형 회귀를 위해 사용할 예제는 
공부한 시간과 점수에 대한 상관관계입니다.

### 1) 훈련 데이터셋과 테스트 데이터셋

어떤 학생이 1시간 공부를 했더니 2점, 
다른 학생이 2시간 공부를 했더니 4점, 
또 다른 학생이 3시간을 공부했더니 6점을 맞았습니다. 
그렇다면, **내가 4시간을 공부한다면 몇 점을 맞을 수 있을까요?**

이 질문에 대답하기 위해서 1시간, 2시간, 3시간을 공부했을 때 각각 2점, 4점, 6점이 나왔다는 앞서 나온 정보를 이용해야 합니다. 

이때 예측을 위해 사용하는 데이터를 훈련 데이터셋(training dataset)이라고 합니다. 학습이 끝난 후, 이 모델이 얼마나 잘 작동하는지 판별하는 데이터셋을 테스트 데이터셋(test dataset)이라고 합니다.

### 2) 훈련 데이터셋의 구성

앞서 텐서에 대해서 배웠는데, 모델을 학습시키기 위한 데이터는 
<< 파이토치의 텐서의 형태(torch.tensor) >> 를 가지고 있어야 합니다. 
그리고 입력과 출력을 각기 다른 텐서에 저장할 필요가 있습니다. 
이때 보편적으로 입력은 x, 출력(label)은 y를 사용하여 표기합니다.

여기서 x_train은 공부한 시간, y_train은 그에 맵핑되는 점수를 의미합니다.

```python
x_train = torch.FloatTensor([[1], [2], [3]])
y_train = torch.FloatTensor([[2], [4], [6]])
```

![](https://static.wikidocs.net/images/page/53560/tensor1.PNG)

이제 모델의 가설을 세워보겠습니다.

## 2. 가설(Hypothesis) 수립

머신 러닝에서 식을 세울때 이 식을 가설(Hypothesis)라고 합니다. 
보통 머신 러닝에서 가설은 임의로 추측해서 세워보는 식일수도 있고, 
경험적으로 알고 있는 식일 수도 있습니다. 
그리고 맞는 가설이 아니라고 판단되면 계속 수정해나가게 되는 식이기도 합니다.

선형 회귀의 가설은 이미 널리 알려져있으므로 고민할 필요가 없습니다. 
선형 회귀란 학습 데이터와 가장 잘 맞는 하나의 직선을 찾는 일입니다. 
이때 선형 회귀의 가설(직선의 방정식)은 아래와 같은 형식을 가집니다.

$y = Wx + b$

가설의 를 따서  대신 다음과 같이 식을 표현하기도 합니다.

$H(x) = Wx + b$

이때 $x$와 곱해지는 $W$를 가중치(Weight)라고 하며, $b$를 편향(bias)이라고 합니다.

- $W$와 $b$는 중학교 수학 과정인 직선의 방정식에서 기울기와 y절편에 해당됩니다.
	(인공지능 수학/이산수학/선형대수 참조..)

## 3. 비용 함수(Cost function)에 대한 이해

앞으로 딥 러닝을 학습하면서 인터넷에서 이런 용어들을 본다면, 전부 같은 용어로 생각하면 되겠습니다.

**비용 함수(cost function)** 
= **손실 함수(loss function)** 
= **오차 함수(error function)** 
= **목적 함수(objective function)**  

특히 비용 함수와 손실 함수란 용어는 기억해두는 것이 좋습니다.

비용 함수에 대해서 이해하기 위해서 여기서만 잠깐 새로운 예제를 사용해보겠습니다.  
어떤 4개의 훈련 데이터가 있고, 이를 2차원 그래프에 4개의 점으로 표현한 상태라고 하겠습니다. (인공지능 이론에서 배운 cost func 실무활용)

![](https://static.wikidocs.net/images/page/53560/%EA%B7%B8%EB%A6%BC1.PNG)

지금 목표는 4개의 점을 가장 잘 표현하는 직선을 그리는 일입니다. 임의로 3개의 직선을 그려보겠습니다.

![](https://static.wikidocs.net/images/page/53560/%EA%B7%B8%EB%A6%BC2.PNG)

위의 그림은 서로 다른 와 의 값에 따라서 천차만별로 그려진 3개의 직선의 모습을 보여줍니다. 이 3개의 직선 중에서 4개의 점을 가장 잘 반영한 직선은 어떤 직선인가요? 검은색 직선이라고 말하는 사람도 있을 것이고, 잘 모르겠다고 말하는 사람도 있을 것입니다. 검은색 직선이라고 말하는 사람은 검은색 직선이 가장 4개의 점에 가깝게 지나가는 느낌을 받고 있기 때문입니다.

하지만 수학에서 느낌이라는 표현을 사용하는 것은 아무런 의미도 없습니다. 어떤 직선이 << 가장 적절한 직선 >>(~으로부터 예측이 나옴) 인지를 수학적인 근거를 대서 표현할 수 있어야 합니다. 그래서 오차(error)라는 개념을 도입하겠습니다.

![](https://static.wikidocs.net/images/page/53560/%EA%B7%B8%EB%A6%BC3.PNG)

위 그림은 임의로 그려진 주황색 선에 대해서 각 실제값(4개의 점)과 
직선의 예측값(동일한 값에서의 직선의 값)에 대한 값의 차이를 빨간색 화살표 ↕로 표현한 것입니다. 각 실제값과 각 예측값과의 차이고, 이를 각 실제값에서의 오차라고 말할 수 있습니다. 이 직선의 예측값들과 실제값들과의
<< 총 오차(total error) >> 는 어떻게 구할까요? 직관적으로 생각하기에 모든 오차를 다 더하면 될 것 같습니다. 각 오차를 전부 더해봅시다.

위 주황색 직선의 식은 $y= 13x+1$ 이며, 각 오차는 다음과 같습니다.

|hours()|2|3|4|5|
|---|---|---|---|---|
|실제값|25|50|42|61|
|예측값|27|40|53|66|
|오차|-2|10|-9|-5|

각 오차를 계산해봤습니다. 그런데 수식적으로 단순히 '오차 = 실제값 - 예측값'으로 정의하면 오차값이 <<음수>>가 나오는 경우가 생깁니다. 예를 들어 위의 표에서만 봐도 오차가 음수인 경우가 3번이나 됩니다.

이 경우, 오차를 모두 더하면 덧셈 과정에서 오차값이 +가 되었다가 -되었다가 하므로 제대로 된 << 오차의 $크기$ >>를 측정할 수 없습니다. 그래서 오차를 그냥 전부 더하는 것이 아니라, 각 오차들을 << 제곱해준 뒤에 전부 더하겠습니다. >>

이를 수식으로 표현하면 아래와 같습니다. 
단, 여기서 $n$은 갖고 있는 데이터의 개수를 의미합니다.

$$\sum_{i=1}^{n} \left[ y^{(i)} - H(x^{(i)}) \right]^2 = (-2)^2 + 10^2 + (-9)^2 + (-5)^2 = 210$$

이때 데이터의 개수인 $n$으로 나누면, 오차의 제곱합에 대한 평균을 구할 수 있는데 이를 평균 제곱 오차(Mean Squared Error, MSE)라고 합니다. 수식은 아래와 같습니다.

$$\frac{1}{n} \sum_{i=1}^{n} \left[ y^{(i)} - H(x^{(i)}) \right]^2 = 210/4 = 52.5$$

이를 실제로 계산하면 52.5가 됩니다. 이는  $y= 13x+1$ 의 예측값과 실제값의 
평균 제곱 오차(MSE) 의 값이 52.5임을 의미합니다.

평균 제곱 오차는 이번 회귀 문제에서 적절한 $W$와 $b$를 찾기위해서 최적화된 식입니다. 그 이유는 평균 제곱 오차의 값을 최소값으로 만드는 $W$와 $b$를 찾아내는 것이 가장 훈련 데이터를 잘 반영한 직선을 찾아내는 일이기 때문입니다.

평균 제곱 오차를 $W$와 $b$에 의한 비용 함수(Cost function)로 재정의해보면 다음과 같습니다.  

$$\text{cost}(W, b) = \frac{1}{n} \sum_{i=1}^{n} \left[ y^{(i)} - H(x^{(i)}) \right]^2$$

다시 정리하겠습니다. $cost(W,b)$를 최소가 되게 만드는 $W$와 $b$를 구하면 
훈련 데이터를 << 가장 잘 나타내는 직선 >>을 구할 수 있습니다.



## 4. 옵티마이저 - 경사 하강법(Gradient Descent)

이제 앞서 정의한 비용 함수(Cost Function)의 값을 최소로 하는 $W$와 $b$를 찾는 방법에 대해서 배울 차례입니다. 
이때 사용되는 것이 **옵티마이저(Optimizer)** 알고리즘입니다. **최적화 알고리즘**이라고도 부릅니다. 그리고 이 
<<< 옵티마이저 알고리즘을 통해 적절한 $W$와 $b$를 찾아내는 과정을 머신 러닝에서 학습(training)이라고 부릅니다. >>>

여기서는 가장 기본적인 옵티마이저 알고리즘인 
경사 하강법(Gradient Descent)에 대해서 배웁니다.

이번 설명에서 편향 $b$는 고려하지 않겠습니다. 
즉, $b$가 0이라고 가정한 $y=Wx$와 같은 식을 기준으로 설명합니다.

![](https://static.wikidocs.net/images/page/53560/%EA%B7%B8%EB%A6%BC4.PNG)

가중치 $W$가 직선의 방정식에서는 기울기였음을 기억합시다. 
이제 $W$를 기울기라고 명명하고 설명합니다.

위의 그림에서 주황색선은 기울기 $W$가 20일 때, 초록색선은 기울기 가 1일 때를 보여줍니다. 
다시 말하면 각각 $y=20x$, $y=x$ 에 해당되는 직선입니다. ↕는 각 점에서의 실제값과 두 직선의 예측값과의 오차를 보여줍니다. 이는 앞서 예측에 사용했던  $y=13x+1$ 직선보다 확연히 큰 오차값들입니다. 

즉, 기울기가 지나치게 크면 실제값과 예측값의 오차가 커지고, 
기울기가 지나치게 작아도 실제값과 예측값의 오차가 커집니다. 
사실  $b$또한 마찬가지인데 $b$가 지나치게 크거나 작으면 오차가 커집니다.

설명의 편의를 위해 편향 $b$가 없이 단순히 가중치 $W$만을 사용한 $H(x)=Wx$라는 가설을 가지고, 경사 하강법을 설명하겠습니다. 비용 함수의 값 $cost(W)$는 cost라고 줄여서 표현해보겠습니다. 
이에 따라 $W$와 cost의 관계를 그래프로 표현하면 다음과 같습니다.

![](https://static.wikidocs.net/images/page/21670/%EA%B8%B0%EC%9A%B8%EA%B8%B0%EC%99%80%EC%BD%94%EC%8A%A4%ED%8A%B8.PNG)

기울기 $W$가 무한대로 커지면 커질 수록 cost의 값 또한 무한대로 커지고, 반대로 기울기가 무한대로 작아져도 cost의 값은 무한대로 커집니다. 
위의 그래프에서 cost가 가장 작을 때는 맨 아래의 볼록한 부분입니다. 
기계가 해야할 일은 cost가 가장 최소값을 가지게 하는 $W$를 찾는 일이므로, 맨 아래의 볼록한 부분의 $W$의 값을 찾아야 합니다.

![](https://static.wikidocs.net/images/page/21670/%EA%B2%BD%EC%82%AC%ED%95%98%EA%B0%95%EB%B2%95.PNG)

기계는 임의의 초기값 $W$값을 정한 뒤에, 맨 아래의 볼록한 부분을 향해 점차 $W$의 값을 수정해나갑니다. 위의 그림은 값이 점차 수정되는 과정을 보여줍니다. 
그리고 이를 가능하게 하는 것이 경사 하강법(Gradient Descent)입니다. 이를 이해하기 위해서는 고등학교 수학 과정인 미분을 이해해야 합니다. 경사 하강법은 미분을 배우게 되면 가장 처음 배우게 되는 개념인 한 점에서의 순간 변화율 또는 접선에서의 기울기의 개념을 사용합니다.

![](https://static.wikidocs.net/images/page/21670/%EC%A0%91%EC%84%A0%EC%9D%98%EA%B8%B0%EC%9A%B8%EA%B8%B01.PNG)

위의 그림에서 초록색 선은 $W$가 임의의 값을 가지게 되는 네 가지의 경우에 대해서, 그래프 상으로 접선의 기울기를 보여줍니다. 주목할 것은 맨 아래의 볼록한 부분으로 갈수록 접선의 기울기가 점차 작아진다는 점입니다. 그리고 맨 아래의 볼록한 부분에서는 결국 접선의 기울기가 0이 됩니다. 그래프 상으로는 초록색 화살표가 수평이 되는 지점입니다.

즉, cost가 최소화가 되는 지점은 접선의 기울기가 0이 되는 지점이며, 또한 미분값이 0이 되는 지점입니다. 경사 하강법의 아이디어는 비용 함수(Cost function)를 미분하여 현재 $W$에서의 접선의 기울기를 구하고, 접선의 기울기가 낮은 방향으로 $W$의 값을 변경하는 작업을 반복하는 것에 있습니다. (가중치가 조정 대상임!)

이 반복 작업에는 현재 $W$에 접선의 기울기(gradient)를 구해 특정 숫자 $α$를 곱한 값을 빼서 새로운 $W$로 사용하는 식이 사용됩니다.

$$gradient = \frac{\partial cost(W)}{\partial W}$$

기울기가 음수일 때와 양수일 때 어떻게 $W$값이 조정되는지 보겠습니다.



- **기울기가 음수일 때(Negative gradient) : $W$의 값이 증가**

$$W := W - \alpha \times (-gradient) = W + \alpha \times gradient$$

	기울기가 음수면 $W$의 값이 증가하는데 
	이는 결과적으로 접선의 기울기가 0인 방향으로 $W$의 값이 조정됩니다.  
	만약, 접선의 기울기가 양수라면 위의 수식은 아래와 같이 표현할 수 있습니다.


- **기울기가 양수일 때(Pogitive gradient): $W$의 값이 감소**

$$W := W - \alpha \times (+gradient)$$

	기울기가 양수면 $W$의 값이 감소하게 되는데 이는 결과적으로 기울기가 0인 방향으로 $W$의 값이 조정되게 합니다. 즉, 아래의 수식은 접선의 기울기가 음수거나, 양수일 때 모두 접선의 기울기가 0인 방향으로 의 값을 조정합니다.

$$W := W - \alpha \frac{\partial}{\partial W} cost(W)$$

그렇다면 여기서 학습률(learning rate)이라고 말하는 $\alpha$는 어떤 의미를 가질까요? 학습률 $\alpha$는 $W$의 값을 변경할 때, << 얼마나 크게 변경할지 >> (보폭) 를 결정합니다. 또는 $W$를 그래프의 한 점으로보고 접선의 기울기가 0일 때까지 경사를 따라 내려간다는 관점에서는 << 얼마나 큰 폭으로 이동할지 >> 를 결정합니다. 

직관적으로 생각하기에 학습률 $\alpha$의 값을 무작정 크게 하면 접선의 기울기가 최소값이 되는 $W$를 빠르게 찾을 수 있을 것같지만 그렇지 않습니다. (발산)

![](https://static.wikidocs.net/images/page/21670/%EA%B8%B0%EC%9A%B8%EA%B8%B0%EB%B0%9C%EC%82%B0.PNG)

위의 그림은 학습률 $\alpha$가 지나치게 높은 값을 가질 때, 접선의 기울기가 0이 되는 $W$를 찾아가는 것이 아니라 $W$의 값이 발산하는 상황을 보여줍니다. 반대로 학습률 $\alpha$가 지나치게 낮은 값을 가지면 학습 속도가 느려지므로 적당한 $\alpha$의 값을 찾아내는 것도 중요합니다.

지금까지는 $b$는 배제시키고 최적의 $W$를 찾아내는 것에만 초점을 맞추어 경사 하강법의 원리에 대해서 배웠는데, 실제 경사 하강법은 $W$와 $b$에 대해서 동시에 경사 하강법을 수행하면서 최적의  $W$와 $b$의 값을 찾아갑니다.

- **가설, 비용함수, 옵티마이저는 머신러닝 분야에서 사용되는 포괄적 개념입니다. 풀고자하는 각 문제에 따라 가설, 비용 함수, 옵티마이저는 전부 다를 수 있으며 << 선형 회귀에 가장 적합한 비용 함수는 평균 제곱 오차, 옵티마이저는 경사 하강법입니다. >>  => 이진분류, 다부류 문제 등 존재.

이제 가설, 비용 함수, 옵티마이저에 대해서 학습하였으니 
파이토치로 구현해보겠습니다.

## 5. 파이토치로 선형 회귀 구현하기

우선 실습을 위해 파이토치의 도구들을 임포트하는 기본 셋팅을 진행합니다.

### 1) 기본 셋팅

```python
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
```

```python
# 현재 실습하고 있는 파이썬 코드를 재실행해도 다음에도 같은 결과가 나오도록 랜덤 시드(random seed)를 줍니다.
torch.manual_seed(1)
```

실습을 위한 기본적인 셋팅이 끝났습니다. 이제 훈련 데이터인 x_train과 y_train을 선언합니다.

### 2) 변수 선언

```python
x_train = torch.FloatTensor([[1], [2], [3]])
y_train = torch.FloatTensor([[2], [4], [6]])
```

x_train과 x_train의 크기(shape)를 출력해보겠습니다.

```python
print(x_train)
print(x_train.shape)
```

```python
tensor([[1.],
        [2.],
        [3.]])
torch.Size([3, 1])
```

x_train의 값이 출력되고, x_train의 크기가 (3 × 1)임을 알 수 있습니다.  
y_train과 y_train의 크기(shape)를 출력해보겠습니다.

```python
print(y_train)
print(y_train.shape)
```

```python
tensor([[2.],
        [4.],
        [6.]])
torch.Size([3, 1])
```

y_train의 값이 출력되고, y_train의 크기가 (3 × 1)임을 알 수 있습니다.

### 3) 가중치와 편향의 초기화

선형 회귀란 학습 데이터와 가장 잘 맞는 하나의 직선을 찾는 일입니다.  
그리고 가장 잘 맞는 직선을 정의하는 것은 바로 $W$와 $b$입니다.  
선형 회귀의 목표는 가장 잘 맞는 직선을 정의하는 $W$와 $b$ 값을 찾는 것입니다.

우선 가중치 W를 0으로 초기화하고, 이 값을 출력해보겠습니다.

```python
# 가중치 W를 0으로 초기화하고 학습을 통해 값이 변경되는 변수임을 명시함.
W = torch.zeros(1, requires_grad=True) 
# 가중치 W를 출력
print(W) 
```

```python
tensor([0.], requires_grad=True)
```

가중치 W가 0으로 초기화되어있으므로 0이 출력된 것을 확인할 수 있습니다. 
위에서 << requires_grad=True >>가 인자로 주어진 것을 확인할 수 있습니다.
이는 이 변수는 << 학습을 통해 계속 값이 변경되는 변수 >>임을 의미합니다.

마찬가지로 편향 도 0으로 초기화하고, 
학습을 통해 값이 변경되는 변수임을 명시합니다.

```python
b = torch.zeros(1, requires_grad=True)
print(b)
```

```python
tensor([0.], requires_grad=True)
```

현재 가중치 $W$와 $b$ 둘 다 0이므로 현 직선의 방정식은 다음과 같습니다.  

$y=0\times x+0$

지금 상태에선 $x$에 어떤 값이 들어가도 가설은 0을 예측하게 됩니다. 
즉, 아직 적절한 $W$와 $b$의 값이 아닙니다.

### 4) 가설 세우기

파이토치 코드 상으로 직선의 방정식에 해당되는 가설을 선언합니다.

$H(x)=Wx+b$

```python
hypothesis = x_train * W + b
print(hypothesis)
```

### 5) 비용 함수 선언하기

파이토치 코드 상으로 선형 회귀의 비용 함수에 해당되는 
평균 제곱 오차를 선언합니다.

$$cost(W,b) = \frac{1}{n} \sum_{i=1}^{n} \left[ y^{(i)} - H(x^{(i)}) \right]^2 $$

```python
# 앞서 배운 torch.mean으로 평균을 구한다.
cost = torch.mean((hypothesis - y_train) ** 2) 
print(cost)
```

```python
tensor(18.6667, grad_fn=<MeanBackward1>)
```

### 6) 경사 하강법 구현하기

이제 경사 하강법을 구현합니다. 아래의 'SGD'는 경사 하강법의 일종입니다. 
lr은 학습률(learning rate)를 의미합니다.  (보폭)
학습 대상인 < W와 b가 SGD의 입력 >>이 됩니다.

```python
optimizer = optim.SGD([W, b], lr=0.01)
```

=> "앞으로 이 파라미터들(`[W, b]`)을, 이 학습률(0.01)로, 
	SGD 방식으로 업데이트하겠다"는 **설정 선언** (실제 계산 이전 단계.)

optimizer.zero_grad()를 실행하므로서 
미분을 통해 얻은 기울기를 0으로 초기화합니다. 
기울기를 초기화해야만 새로운 가중치 편향에 대해서 
새로운 기울기를 구할 수 있습니다. 

그 다음 cost.backward() 함수를 호출하면 
가중치 W와 편향 b에 대한 기울기가 계산됩니다.  (자동미분!)
(prediction이 얼마나 틀렸는지(loss)를 W와 b로 미분하는 과정.)
(손실함수를 줄이는 방향을 찾는 거니까 당연히 loss 에 대한 기울기 도출.)
(**loss → prediction → W** 경로를 거꾸로 미분(chain rule)하는 게 backward, 이게 역전파로 이어짐.)

그 다음 경사 하강법 최적화 함수 opimizer의 .step() 함수를 호출하여 
인수로 들어갔던 W와 b에서 리턴되는 변수들의 기울기에 학습률(learining rate) 0.01을 곱하여 빼줌으로서 업데이트합니다. (실제 경사하강.)
=> $W = W - lr \times  \frac{dL}{dW}$ (=backward 결과. $L$=Loss, b도 동일하게 업데이트.)

```python
# gradient를 0으로 초기화
optimizer.zero_grad() 
# 비용 함수를 미분하여 gradient 계산
cost.backward() 
# W와 b를 업데이트
optimizer.step() 
```

- **requires_grad=True와 backward()에 대한 정리는 자동 미분(Autograd) 챕터에 별도 정리하였습니다.**

### 7) 전체 코드

```python
# 데이터
x_train = torch.FloatTensor([[1], [2], [3]])
y_train = torch.FloatTensor([[2], [4], [6]])
# 모델 초기화
W = torch.zeros(1, requires_grad=True)
b = torch.zeros(1, requires_grad=True)
# optimizer 설정
optimizer = optim.SGD([W, b], lr=0.01)

nb_epochs = 1999 # 원하는만큼 경사 하강법을 반복
for epoch in range(nb_epochs + 1):

    # H(x) 계산
    hypothesis = x_train * W + b

    # cost 계산
    cost = torch.mean((hypothesis - y_train) ** 2)

    # cost로 H(x) 개선
    optimizer.zero_grad()
    cost.backward()
    optimizer.step()

    # 100번마다 로그 출력
    if epoch % 100 == 0:
        print('Epoch {:4d}/{} W: {:.3f}, b: {:.3f} Cost: {:.6f}'.format(
            epoch, nb_epochs, W.item(), b.item(), cost.item()
        ))
```

결과적으로 훈련 과정에서 $W$와 $b$는 훈련 데이터와 잘 맞는 직선을 표현하기 위한 적절한 값으로 변화해갑니다.

```python
Epoch    0/2000 W: 0.187, b: 0.080 Cost: 18.666666
Epoch  100/2000 W: 1.746, b: 0.578 Cost: 0.048171
Epoch  200/2000 W: 1.800, b: 0.454 Cost: 0.029767
Epoch  300/2000 W: 1.843, b: 0.357 Cost: 0.018394
Epoch  400/2000 W: 1.876, b: 0.281 Cost: 0.011366
Epoch  500/2000 W: 1.903, b: 0.221 Cost: 0.007024
Epoch  600/2000 W: 1.924, b: 0.174 Cost: 0.004340
Epoch  700/2000 W: 1.940, b: 0.136 Cost: 0.002682
Epoch  800/2000 W: 1.953, b: 0.107 Cost: 0.001657
Epoch  900/2000 W: 1.963, b: 0.084 Cost: 0.001024
Epoch 1000/2000 W: 1.971, b: 0.066 Cost: 0.000633
Epoch 1100/2000 W: 1.977, b: 0.052 Cost: 0.000391
Epoch 1200/2000 W: 1.982, b: 0.041 Cost: 0.000242
Epoch 1300/2000 W: 1.986, b: 0.032 Cost: 0.000149
Epoch 1400/2000 W: 1.989, b: 0.025 Cost: 0.000092
Epoch 1500/2000 W: 1.991, b: 0.020 Cost: 0.000057
Epoch 1600/2000 W: 1.993, b: 0.016 Cost: 0.000035
Epoch 1700/2000 W: 1.995, b: 0.012 Cost: 0.000022
Epoch 1800/2000 W: 1.996, b: 0.010 Cost: 0.000013
Epoch 1900/2000 W: 1.997, b: 0.008 Cost: 0.000008
Epoch 2000/2000 W: 1.997, b: 0.006 Cost: 0.000005
```

**에포크(Epoch)** 는 전체 훈련 데이터가 학습에 한 번 사용된 주기를 말합니다.  
이번 실습의 경우 2,000번을 수행했습니다.

최종 훈련 결과를 보면 최적의 기울기 $W$는 2에 가깝고, 
$b$는 0에 가까운 것을 볼 수 있습니다.  

현재 훈련 데이터가 x_train은 `[[1], [2], [3]]`이고 y_train은 `[[2], [4], [6]]`인 것을 감안하면  
실제 정답은 $W$가 2이고, $b$가 0인 $H(x)=2x$이므로 거의 정답을 찾은 셈입니다.

## 6. optimizer.zero_grad()가 필요한 이유

파이토치는 미분을 통해 얻은 기울기를 
이전에 계산된 기울기 값에 누적시키는 특징이 있습니다. 예를 들어봅시다.

```python
import torch
w = torch.tensor(2.0, requires_grad=True)

nb_epochs = 20
for epoch in range(nb_epochs + 1):

  z = 2*w

  z.backward()
  print('수식을 w로 미분한 값 : {}'.format(w.grad))
```

```python
수식을 w로 미분한 값 : 2.0
수식을 w로 미분한 값 : 4.0
수식을 w로 미분한 값 : 6.0
수식을 w로 미분한 값 : 8.0
수식을 w로 미분한 값 : 10.0
수식을 w로 미분한 값 : 12.0
수식을 w로 미분한 값 : 14.0
수식을 w로 미분한 값 : 16.0
수식을 w로 미분한 값 : 18.0
수식을 w로 미분한 값 : 20.0
수식을 w로 미분한 값 : 22.0
수식을 w로 미분한 값 : 24.0
수식을 w로 미분한 값 : 26.0
수식을 w로 미분한 값 : 28.0
수식을 w로 미분한 값 : 30.0
수식을 w로 미분한 값 : 32.0
수식을 w로 미분한 값 : 34.0
수식을 w로 미분한 값 : 36.0
수식을 w로 미분한 값 : 38.0
수식을 w로 미분한 값 : 40.0
수식을 w로 미분한 값 : 42.0
```

계속해서 미분값인 2가 누적되는 것을 볼 수 있습니다. 그렇기 때문에 optimizer.zero_grad()를 통해 미분값을 계속 0으로 초기화시켜줘야 합니다.


## 7. torch.manual_seed()를 하는 이유

torch.manual_seed()를 사용한 프로그램의 결과는 다른 컴퓨터에서 실행시켜도 동일한 결과를 얻을 수 있습니다. 
그 이유는 torch.manual_seed()는 난수 발생 순서와 값을 동일하게 보장해준다는 특징때문입니다. 

우선 랜덤 시드가 3일 때 두 번 난수를 발생시켜보고, 
다른 랜덤 시드를 사용한 후에 다시 랜덤 시드를 3을 사용한다면 
난수 발생값이 동일하게 나오는지 보겠습니다.

```python
torch.manual_seed(3)
print('랜덤 시드가 3일 때')
for i in range(1,3):
  print(torch.rand(1))
```

```python
랜덤 시드가 3일 때
tensor([0.0043])
tensor([0.1056])
```

랜덤 시드가 3일때 두 개의 난수를 발생시켰더니 0.0043과 0.1056이 나옵니다. 이제 랜덤 시드값을 바꿔봅시다.

```python
torch.manual_seed(5)
print('랜덤 시드가 5일 때')
for i in range(1,3):
  print(torch.rand(1))
```

```python
랜덤 시드가 5일 때
tensor([0.8303])
tensor([0.1261])
```

0.8303과 0.1261이 나옵니다. 이제 다시 랜덤 시드값을 3으로 돌려보겠습니다. 이렇게 하면 프로그램을 다시 처음부터 실행한 것처럼 난수 발생 순서가 초기화됩니다.

```python
torch.manual_seed(3)
print('랜덤 시드가 다시 3일 때')
for i in range(1,3):
  print(torch.rand(1))
```

```python
랜덤 시드가 다시 3일 때
tensor([0.0043])
tensor([0.1056])
```

다시 동일하게 0.0043과 0.1056이 나옵니다.

텐서에는 requires_grad라는 속성이 있습니다. 
이것을 True로 설정하면 자동 미분 기능이 적용됩니다. 

선형 회귀부터 신경망과 같은 복잡한 구조에서 파라미터들이 모두 이 기능이 적용됩니다. requires_grad = True가 적용된 텐서에 연산을 하면, 계산 그래프가 생성되며 backward 함수를 호출하면 그래프로부터 자동으로 미분이 계산됩니다. 
파이토치의 학습 과정을 보다 더 잘 이해하기 위해서 자동 미분에 대해서 이해해봅시다.



## 8. 자동 미분(Autograd) 실습하기

자동 미분에 대해서 실습을 통해 이해해봅시다. 
임의로 $2w^2+5$라는 식을 세워보고, $w$에 대해 미분해보겠습니다.

```python
import torch
```

값이 2인 임의의 스칼라 텐서 w를 선언합니다. 
이때 required_grad를 True로 설정합니다. 이는 이 텐서에 대한 기울기를 저장하겠다는 의미입니다. 뒤에서 보겠지만, 
이렇게 하면 << w.grad에 w에 대한 미분값이 저장 >> 됩니다.

```python
w = torch.tensor(2.0, requires_grad=True)
```

이제 수식을 정의합니다.

```python
y = w**2
z = 2*y + 5
```

이제 해당 수식을 w에 대해서 미분해야합니다. 
.backward()를 호출하면 해당 수식의 w에 대한 기울기를 계산합니다.

```python
z.backward()
```

이제 w.grad를 출력하면 w가 속한 수식을 w로 미분한 값이 저장된 것을 확인할 수 있습니다.

```python
print('수식을 w로 미분한 값 : {}'.format(w.grad))
```

```python
수식을 w로 미분한 값 : 8.0
```




# 03-02 다중 선형회귀

앞서 배운 $x$가 1개인 선형 회귀를 
단순 선형 회귀(Simple Linear Regression)이라고 합니다.  
이번 챕터에서는 다수의 $x$로부터 $y$를 예측하는 다중 선형 회귀(Multivariable Linear Regression)에 대해서 이해합니다.

## 1. 데이터에 대한 이해(Data Definition)

다음과 같은 훈련 데이터가 있습니다. 
앞서 배운 단순 선형 회귀와 다른 점은 독립 변수 $x$의 개수가 이제 1개가 아닌 3개라는 점입니다. 3개의 퀴즈 점수로부터 최종 점수를 예측하는 모델을 만들어보겠습니다.

| Quiz 1 (x1) | Quiz 2 (x2) | Quiz 3 (x3) | Final (y) |
| ----------- | ----------- | ----------- | --------- |
| 73          | 80          | 75          | 152       |
| 93          | 88          | 93          | 185       |
| 89          | 91          | 80          | 180       |
| 96          | 98          | 100         | 196       |
| 73          | 66          | 70          | 142       |

독립 변수 $x$의 개수가 3개므로 이를 수식으로 표현하면 아래와 같습니다.

$H(x) = w_1x_1 + w_2x_2 + w_3x_3 + b$


## 2. 파이토치로 구현하기

우선 필요한 도구들을 임포트하고 랜덤 시드를 고정합니다.

```python
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
```

```python
torch.manual_seed(1)
```

이제 훈련 데이터를 선언해보겠습니다.

  
위의 식을 보면 이번에는 단순 선형 회귀와 다르게 $x$의 개수가 3개입니다. 
그러니까 $x$를 3개 선언합니다.

```python
# 훈련 데이터
x1_train = torch.FloatTensor([[73], [93], [89], [96], [73]])
x2_train = torch.FloatTensor([[80], [88], [91], [98], [66]])
x3_train = torch.FloatTensor([[75], [93], [90], [100], [70]])
y_train = torch.FloatTensor([[152], [185], [180], [196], [142]])
```

이제 가중치 $w$와 편향 $b$를 선언합니다. 가중치 $w$도 3개 선언해주어야 합니다.

```python
# 가중치 w와 편향 b 초기화
w1 = torch.zeros(1, requires_grad=True)
w2 = torch.zeros(1, requires_grad=True)
w3 = torch.zeros(1, requires_grad=True)
b = torch.zeros(1, requires_grad=True)
```

이제 가설, 비용 함수, 옵티마이저를 선언한 후에 
경사 하강법을 1,000회 반복합니다.

```python
# optimizer 설정
optimizer = optim.SGD([w1, w2, w3, b], lr=1e-5)

nb_epochs = 1000
for epoch in range(nb_epochs + 1):

    # H(x) 계산
    hypothesis = x1_train * w1 + x2_train * w2 + x3_train * w3 + b

    # cost 계산
    cost = torch.mean((hypothesis - y_train) ** 2)

    # cost로 H(x) 개선
    optimizer.zero_grad()
    cost.backward()
    optimizer.step()

    # 100번마다 로그 출력
    if epoch % 100 == 0:
        print('Epoch {:4d}/{} w1: {:.3f} w2: {:.3f} w3: {:.3f} b: {:.3f} Cost: {:.6f}'.format(
            epoch, nb_epochs, w1.item(), w2.item(), w3.item(), b.item(), cost.item()
        ))
```

위의 경우 가설을 선언하는 부분인 
hypothesis =$x_1\_train * w1 + x_2\_train * w2 + x_3\_train * w3 + b$ 에서도
x_train의 개수만큼 w와 곱해주도록 작성해준 것을 확인할 수 있습니다.

## 3. 벡터와 행렬 연산으로 바꾸기

위의 코드를 개선할 수 있는 부분이 있습니다. 이번에는 $x$의 개수가 3개였으니까 x1_train, x2_train, x3_train와 w1, w2, w3를 일일히 선언해주었습니다. 
그런데 $x$의 개수가 1,000개라고 가정해봅시다. 위와 같은 방식을 고수할 경우 x_train1 ~ x_train1000을 전부 선언하고, w1 ~ w1000을 전부 선언해야 합니다. 

다시 말해 $x$와 $w$변수 선언만 총 합 2,000개를 해야합니다.
또한 가설을 선언하는 부분에서도 마찬가지로 x_train과 w의 곱셈이 이루어지는 항을 1,000개를 작성해야 합니다. 이는 굉장히 비효율적입니다.

=> 이를 해결하기 위해 행렬 곱셈 연산(또는 벡터의 내적)을 사용합니다.

- **행렬의 곱셈 과정에서 이루어지는 
  벡터 연산을 벡터의 내적(Dot Product)이라고 합니다.**

![](https://static.wikidocs.net/images/page/54841/%ED%96%89%EB%A0%AC%EA%B3%B1.PNG)

위의 그림은 행렬 곱셈 연산 과정에서 벡터의 내적으로 
1 × 7 + 2 × 9 + 3 × 11 = 58이 되는 과정을 보여줍니다.

이 행렬 연산이 어떻게 현재 배우고 있는 가설과 상관이 있다는 걸까요?  
바로 가설을 벡터와 행렬 연산으로 표현할 수 있기 때문입니다.

### 1) 벡터 연산으로 이해하기

위 식은 아래와 같이 두 벡터의 내적으로 표현할 수 있습니다.

![](https://static.wikidocs.net/images/page/54841/%EB%82%B4%EC%A0%81.PNG)

두 벡터를 각각 $X$와 $W$로 표현한다면, 가설은 다음과 같습니다.
$H(X) = XW$
$x$의 개수가 3개였음에도 이제는 $X$와 $W$라는 두 개의 변수로 표현된 것을 볼 수 있습니다.

### 2) 행렬 연산으로 이해하기

훈련 데이터를 살펴보고, 벡터와 행렬 연산을 통해 가설 $H(X)$를 표현해보겠습니다.

|Quiz 1 (x1)|Quiz 2 (x2)|Quiz 3 (x3)|Final (y)|
|---|---|---|---|
|73|80|75|152|
|93|88|93|185|
|89|91|80|180|
|96|98|100|196|
|73|66|70|142|

전체 훈련 데이터의 개수를 셀 수 있는 1개의 단위를 
<< 샘플(sample) >>이라고 합니다. 현재 샘플의 수는 총 5개입니다.  
각 샘플에서 $y$를 결정하게 하는 각각의 독립 변수 $x$를 특성(feature)이라고 합니다. 
현재 특성은 3개입니다.



이는 독립 변수 들의 수가 (샘플의 수 × 특성의 수) = 15개임을 의미합니다. 
독립 변수 $x$들을 (샘플의 수 × 특성의 수)의 크기를 가지는 
하나의 행렬로 표현해봅시다. 그리고 이 행렬을 $X$라고 하겠습니다.

![[Pasted image 20260704164454.png|159]]

그리고 여기에 가중치 을 원소로 하는 벡터를 라 하고 이를 곱해보겠습니다.

![[Pasted image 20260704164529.png|464]]

위의 식은 결과적으로 다음과 같습니다.

$H(X)=XW$

이 가설에 각 샘플에 더해지는 편향 $b$를 추가해봅시다. 샘플 수만큼의 차원을 가지는 편향 벡터 $B$를 만들어 더합니다.

![[Pasted image 20260704164651.png|481]]

위의 식은 결과적으로 다음과 같습니다.

$H(X)=XW+B$

결과적으로 전체 훈련 데이터의 가설 연산을 3개의 변수만으로 표현하였습니다.  
이와 같이 벡터와 행렬 연산은 식을 간단하게 해줄 뿐만 아니라 다수의 샘플의 병렬 연산이므로 속도의 이점을 가집니다.

이를 참고로 파이토치로 구현해봅시다.

## 4. 행렬 연산을 고려하여 파이토치로 구현하기

이번에는 행렬 연산을 고려하여 파이토치로 재구현해보겠습니다.  
이번에는 훈련 데이터 또한 행렬로 선언해야 합니다.

```python
x_train  =  torch.FloatTensor([[73,  80,  75], 
                               [93,  88,  93], 
                               [89,  91,  80], 
                               [96,  98,  100],   
                               [73,  66,  70]])  
y_train  =  torch.FloatTensor([[152],  [185],  [180],  [196],  [142]])
```

이전에 x_train을 3개나 구현했던 것과 다르게 이번에는 
x_train 하나에 모든 샘플을 전부 선언하였습니다. 
다시 말해 (5 x 3) 행렬 을 선언한 것입니다.

x_train과 y_train의 크기(shape)를 출력해보겠습니다.

```python
print(x_train.shape)
print(y_train.shape)
```

```python
torch.Size([5, 3])
torch.Size([5, 1])
```

각각 (5 × 3) 행렬과 (5 × 1) 행렬(또는 벡터)의 크기를 가집니다.  
이제 가중치 $W$와 편향 $b$를 선언합니다.

```python
# 가중치와 편향 선언
W = torch.zeros((3, 1), requires_grad=True)
b = torch.zeros(1, requires_grad=True)
```

여기서 주목할 점은 가중치 $w$의 크기가 (3 × 1) 벡터라는 점입니다. 
행렬의 곱셈이 성립되려면 << 곱셈의 좌측에 있는 행렬의 열의 크기와 
우측에 있는 행렬의 행의 크기가 일치해야 합니다. >>

현재 X_train의 행렬의 크기는 (5 × 3)이며,  
벡터의 크기는 (3 × 1)이므로 두 행렬과 벡터는 행렬곱이 가능합니다. 
행렬곱으로 가설을 선언하면 아래와 같습니다.

```python
hypothesis = x_train.matmul(W) + b
```

가설을 행렬곱으로 간단히 정의하였습니다. 이는 앞서 x_train과 w의 곱셈이 이루어지는 각 항을 전부 기재하여 가설을 선언했던 것과 대비됩니다. 

이 경우, 사용자가 << 독립 변수 $x$의 수를 늘리거나 줄이더라도 >> 
위의 가설 선언 코드를 수정할 필요가 없습니다. 이제 해야할 일은 비용 함수와 옵티마이저를 정의하고, 정해진 에포크만큼 훈련을 진행하는 일입니다. 이를 반영한 전체 코드는 다음과 같습니다.

```python
x_train  =  torch.FloatTensor([[73,  80,  75], 
                               [93,  88,  93], 
                               [89,  91,  80], 
                               [96,  98,  100],   
                               [73,  66,  70]])  
y_train  =  torch.FloatTensor([[152],  [185],  [180],  [196],  [142]])

# 모델 초기화
W = torch.zeros((3, 1), requires_grad=True)
b = torch.zeros(1, requires_grad=True)
# optimizer 설정
optimizer = optim.SGD([W, b], lr=1e-5)

nb_epochs = 20
for epoch in range(nb_epochs + 1):

    # H(x) 계산
    # 편향 b는 브로드 캐스팅되어 각 샘플에 더해집니다.
    hypothesis = x_train.matmul(W) + b

    # cost 계산
    cost = torch.mean((hypothesis - y_train) ** 2)

    # cost로 H(x) 개선
    optimizer.zero_grad()
    cost.backward()
    optimizer.step()

    print('Epoch {:4d}/{} hypothesis: {} Cost: {:.6f}'.format(
        epoch, nb_epochs, hypothesis.squeeze().detach(), cost.item()
    ))
```

학습이 끝난 모델에 임의의 입력 값을 넣어 예측을 해봅시다.

```python
# 임의의 입력 값에 대한 예측
with torch.no_grad():
    new_input = torch.FloatTensor([[75, 85, 72]])  # 임의의 입력
    prediction = new_input.matmul(W) + b
    print('Predicted value for input {}: {}'.format(new_input.squeeze().tolist(), prediction.item()))
```

```python
Predicted value for input [75.0, 85.0, 72.0]: 156.8051300048828
```

`with torch.no_grad():` 
이 블록 안에서 수행되는 모든 연산에 대해 역전파(즉, 기울기 계산)를 비활성화합니다. 예측을 할 때는 가중치를 업데이트할 필요가 없기 때문에, 메모리와 계산 자원을 절약하기 위해 `torch.no_grad()`를 사용하는 것이 좋습니다.

`new_input = torch.FloatTensor([[75, 85, 72]])`  
여기서는 예측하고자 하는 새로운 입력 값을 정의합니다. `new_input`은 `FloatTensor` 형식의 2차원 텐서로, 이 경우 `[[75, 85, 72]]`라는 값을 가지는 텐서(1,3)를 생성합니다. 
이 값들은  <<  모델이 학습한 기존 데이터와 동일한 차원 >> 을 가지며, 
각 숫자는 특정 특징(feature)을 나타냅니다. 


`prediction = new_input.matmul(W) + b`  
이 줄에서 모델이 예측을 수행합니다. 새로운 입력 `new_input`과 학습된 가중치 `W`를 행렬 곱셈(`matmul`)으로 계산하고, 그 결과에 편향 `b`를 더합니다. 
이 수식은 모델이 새로운 입력에 대해 예측한 값을 계산하는 과정입니다. 이때 `W`와 `b`는 학습 과정을 통해 얻어진 최적의 값들입니다.
(초기 가설에서 W,b가 다중 선형회귀로 조정된 형태의 예측 함수: prediction fun.)

`print('Predicted value for input {}: {}'.format(new_input.squeeze().tolist(), prediction.item()))`  
마지막으로, 예측된 값을 출력합니다. 
`new_input.squeeze().tolist()`는 입력 값을 리스트 형태로 변환하여 보기 쉽게 만듭니다.
`prediction.item()`은 텐서로 반환된 예측 값을 파이썬의 숫자 자료형으로 변환해 출력합니다. 출력문에서는 입력된 값과 그에 대한 모델의 예측 값을 함께 보여줍니다.

# 03-03 nn.Module과 클래스로 구현

이전 챕터까지는 선형 회귀를 좀 더 직접적으로 이해하기 위해 가설, 비용 함수를 직접 정의해서 선형 회귀 모델을 구현했습니다. 이번에는 파이토치에서 이미 구현되어져 제공되고 있는 함수들을 불러오는 것으로 더 쉽게 선형 회귀 모델을 구현해보겠습니다.

예를 들어 파이토치에서는 선형 회귀 모델이 nn.Linear()라는 함수로, 
또 평균 제곱오차가 nn.functional.mse_loss()라는 함수로 구현되어져 있습니다. 
아래는 이번 실습에서 사용할 두 함수의 사용 예제를 간단히 보여줍니다.

```python
import torch.nn as nn
model = nn.Linear(input_dim, output_dim)
```

```python
import torch.nn.functional as F
cost = F.mse_loss(prediction, y_train)
```

## 1. 단순 선형 회귀 구현하기

우선 필요한 도구들을 임포트합니다.

```python
import torch
import torch.nn as nn
import torch.nn.functional as F
```

```python
torch.manual_seed(1)
```

이제 데이터를 선언합니다. 
아래 데이터는 를 가정된 상태에서 만들어진 데이터로 우리는 이미 정답이 W=2, b=0임을 알고 있는 사태입니다. 
모델이 이 두 W와 b의 값을 제대로 찾아내도록 하는 것이 목표입니다.

```python
# 데이터
x_train = torch.FloatTensor([[1], [2], [3]])
y_train = torch.FloatTensor([[2], [4], [6]])
```

데이터를 정의하였으니 이제 선형 회귀 모델을 구현할 차례입니다.  
nn.Linear()는 입력의 차원, 출력의 차원을 인수로 받습니다.

```python
# 모델을 선언 및 초기화. 단순 선형 회귀이므로 input_dim=1, output_dim=1.
model = nn.Linear(1,1)
```

위 torch.nn.Linear 인자로 1, 1을 사용하였습니다. 

하나의 입력 에 대해서 하나의 출력 $x$을 가지므로, 
입력 차원과 출력 차원 모두 1을 인수로 사용하였습니다. 

model에는 가중치 W와 편향 b가 저장되어져 있습니다. 이 값은 model.parameters()라는 함수를 사용하여 불러올 수 있는데, 
한 번 출력해보겠습니다.

```python
print(list(model.parameters()))
```

```python
[Parameter containing:
tensor([[0.5153]], requires_grad=True), Parameter containing:
tensor([-0.4414], requires_grad=True)]
```

2개의 값이 출력되는데 첫번째 값이 W고, 두번째 값이 b에 해당됩니다. 두 값 모두 현재는 랜덤 초기화가 되어져 있습니다. 그리고 두 값 모두 학습의 대상이므로 requires_grad=True가 되어져 있는 것을 볼 수 있습니다.

이제 옵티마이저를 정의합니다. 
model.parameters()를 사용하여 W와 b를 전달합니다.  
학습률(learning rate)은 0.01로 정합니다.

```python
# optimizer 설정. 경사 하강법 SGD를 사용하고 learning rate를 의미하는 lr은 0.01
optimizer = torch.optim.SGD(model.parameters(), lr=0.01) 
```

```python
# 전체 훈련 데이터에 대해 경사 하강법을 2,000회 반복
nb_epochs = 2000
for epoch in range(nb_epochs+1):

    # H(x) 계산
    prediction = model(x_train)

    # cost 계산
    cost = F.mse_loss(prediction, y_train)
        # <== 파이토치에서 제공하는 평균 제곱 오차 함수 (MSE)

    # cost로 H(x) 개선하는 부분
    # gradient를 0으로 초기화
    optimizer.zero_grad()
    # 비용 함수를 미분하여 gradient 계산
    cost.backward() # backward 연산
    # W와 b를 업데이트
    optimizer.step()

    if epoch % 100 == 0:
    # 100번마다 로그 출력
      print('Epoch {:4d}/{} Cost: {:.6f}'.format(
          epoch, nb_epochs, cost.item()
      ))
```

```python
Epoch    0/2000 Cost: 13.103540
... 중략 ...
Epoch 2000/2000 Cost: 0.000000
```

학습이 완료되었습니다. Cost의 값이 매우 작습니다. W와 b의 값도 최적화가 되었는지 확인해봅시다.  
에 임의의 값 4를 넣어 모델이 예측하는 의 값을 확인해보겠습니다.

```python
# 임의의 입력 4를 선언
new_var =  torch.FloatTensor([[4.0]]) 
# 입력한 값 4에 대해서 예측값 y를 리턴받아서 pred_y에 저장
pred_y = model(new_var) # forward 연산
# y = 2x 이므로 입력이 4라면 y가 8에 가까운 값이 나와야 제대로 학습이 된 것
print("훈련 후 입력이 4일 때의 예측값 :", pred_y) 
```

```python
훈련 후 입력이 4일 때의 예측값 : tensor([[7.9989]], 
grad_fn=<AddmmBackward>)
```

사실 이 문제의 정답은 $y=2x$가 정답이므로 y값이 8에 가까우면 W와 b의 값이 어느정도 최적화가 된 것으로 볼 수 있습니다. 실제로 예측된 y값은 7.9989로 8에 매우 가깝습니다.

이제 학습 후의 W와 b의 값을 출력해보겠습니다.

```python
print(list(model.parameters()))
```

```python
[Parameter containing:
tensor([[1.9994]], requires_grad=True), Parameter containing:
tensor([0.0014], requires_grad=True)]
```

W의 값이 2에 가깝고, b의 값이 0에 가까운 것을 볼 수 있습니다.

- $H(x)$ 식에 입력 $x$로부터 예측된 $y$를 얻는 것을 forward 연산이라고 합니다.
- 학습 전, prediction = model(x_train)은 x_train으로부터 예측값을 리턴하므로 forward 연산입니다.
- 학습 후, pred_y = model(new_var)는 임의의 값 new_var로부터 예측값을 리턴하므로 forward 연산입니다.
- 학습 과정에서 비용 함수를 미분하여 기울기를 구하는 것을 backward 연산이라고 합니다.
- cost.backward()는 비용 함수로부터 기울기를 구하라는 의미이며 backward 연산입니다.

## 2. 다중 선형 회귀 구현하기

이제 nn.Linear()와 nn.functional.mse_loss()로 다중 선형 회귀를 구현해봅시다. 
사실 코드 자체는 달라지는 건 거의 없는데, 
<< nn.Linear()의 인자값과 학습률(learning rate)만 조절 >> 해주었습니다.

```python
import torch
import torch.nn as nn
import torch.nn.functional as F
```

```python
torch.manual_seed(1)
```

이제 데이터를 선언해줍니다. 
여기서는 3개의 $x$로부터 하나의 $y$를 예측하는 문제입니다.  
즉, 가설 수식은 $H(x) = w_1x_1 + w_2x_2 + w_3x_3 + b$입니다.

```python
# 데이터
x_train = torch.FloatTensor([[73, 80, 75],
                             [93, 88, 93],
                             [89, 91, 90],
                             [96, 98, 100],
                             [73, 66, 70]])
y_train = torch.FloatTensor([[152], [185], [180], [196], [142]])
```

데이터를 정의하였으니 이제 선형 회귀 모델을 구현할 차례입니다. 
nn.Linear()는 입력의 차원, 출력의 차원을 인수로 받습니다.

```python
# 모델을 선언 및 초기화. 다중 선형 회귀이므로 input_dim=3, output_dim=1.
model = nn.Linear(3,1)
```

위 torch.nn.Linear 인자로 3, 1을 사용하였습니다. 
3개의 입력 x에 대해서 하나의 출력 y을 가지므로, 입력 차원은 3, 출력 차원은 1을 인수로 사용하였습니다. 
model에는 3개의 가중치 w와 편향 b가 저장되어져 있습니다. 이 값은 model.parameters()라는 함수를 사용하여 불러올 수 있는데, 한 번 출력해보겠습니다.

```python
print(list(model.parameters()))
```

```python
[Parameter containing:
tensor([[ 0.2975, -0.2548, -0.1119]], requires_grad=True), Parameter containing:
tensor([0.2710], requires_grad=True)]
```

첫번째 출력되는 것이 3개의 w고, 두번째 출력되는 것이 b에 해당됩니다. 두 값 모두 현재는 랜덤 초기화가 되어져 있습니다. 그리고 두 출력 결과 모두 학습의 대상이므로 requires_grad=True가 되어져 있는 것을 볼 수 있습니다.

이제 옵티마이저를 정의합니다. model.parameters()를 사용하여 3개의 w와 
b를 전달합니다. 학습률(learning rate)은 0.00001로 정합니다. 
파이썬 코드로는 1e-5로도 표기합니다. 0.01로 하지 않는 이유는 
<< 기울기가 발산하기 때문 >> 입니다. 궁금하다면 해보시기 바랍니다.

=> 차원이 늘수록 loss 지형이 더 가파르고 복잡해짐.
같은 학습률이어도, gradient가 크면 한 번에 이동하는 거리가 훨씬 커서 발산함.
(gradient가 큰 이유: 다중회귀에서는 변수,가중치가 여러 항이므로 변수값이 크면 오차 자체가 더 커지게 되는 구조임.)


![](https://static.wikidocs.net/images/page/21670/%EA%B8%B0%EC%9A%B8%EA%B8%B0%EB%B0%9C%EC%82%B0.PNG)

위의 그림은 앞서 배웠던 내용으로, 학습률(learning rate)이 모델의 필요한 크기보다 높을 때, 기울기가 발산하는 현상을 보여줍니다.

```python
optimizer = torch.optim.SGD(model.parameters(), lr=1e-5) 
```

이하 코드는 단순 선형 회귀를 구현했을 때와 동일합니다.

```python
nb_epochs = 2000
for epoch in range(nb_epochs+1):

    # H(x) 계산
    prediction = model(x_train)
    # model(x_train)은 model.forward(x_train)와 동일함.

    # cost 계산
    cost = F.mse_loss(prediction, y_train) # <== 파이토치에서 제공하는 평균 제곱 오차 함수

    # cost로 H(x) 개선하는 부분
    # gradient를 0으로 초기화
    optimizer.zero_grad()
    # 비용 함수를 미분하여 gradient 계산
    cost.backward()
    # W와 b를 업데이트
    optimizer.step()

    if epoch % 100 == 0:
    # 100번마다 로그 출력
      print('Epoch {:4d}/{} Cost: {:.6f}'.format(
          epoch, nb_epochs, cost.item()
      ))
```

```python
Epoch    0/2000 Cost: 31667.597656
... 중략 ...
Epoch 2000/2000 Cost: 0.199777
```

학습이 완료되었습니다. 
Cost의 값이 매우 작습니다. 3개의 w와 b의 값도 최적화가 되었는지 확인해봅시다.  
$x$에 임의의 입력 [73, 80, 75]를 넣어 모델이 예측하는 $y$의 값을 확인해보겠습니다.

```python
# 임의의 입력 [73, 80, 75]를 선언
new_var =  torch.FloatTensor([[73, 80, 75]]) 
# 입력한 값 [73, 80, 75]에 대해서 예측값 y를 리턴받아서 pred_y에 저장
pred_y = model(new_var) 
print("훈련 후 입력이 73, 80, 75일 때의 예측값 :", pred_y) 
```

```python
훈련 후 입력이 73, 80, 75일 때의 예측값 : tensor([[151.2305]], grad_fn=<AddmmBackward>)
```

사실 3개의 값 73, 80, 75는 훈련 데이터로 사용되었던 값입니다. 
당시 y의 값은 152였는데, 현재 예측값이 151이 나온 것으로 보아 어느정도는 3개의 w와 b의 값이 최적화 된것으로 보입니다. 
이제 학습 후의 3개의 w와 b의 값을 출력해보겠습니다.

```python
print(list(model.parameters()))
```

```python
[Parameter containing:
tensor([[0.9778, 0.4539, 0.5768]], requires_grad=True), Parameter containing:
tensor([0.2802], requires_grad=True)]
```

파이토치의 대부분의 구현체들은 
<< 대부분 모델을 생성할 때 클래스(Class)를 사용 >>하고 있습니다. 
=> w1, w2 같은 텐서 선언 시에도 객체로 나옴. 클래스로 구현한 것

앞서 배운 선형 회귀를 클래스로 구현해보겠습니다. 앞서 구현한 코드와 다른 점은 오직 클래스로 모델을 구현했다는 점입니다.

## 3. 모델을 클래스로 구현하기

앞서 단순 선형 회귀 모델은 다음과 같이 구현했었습니다.

```python
# 모델을 선언 및 초기화. 단순 선형 회귀이므로 input_dim=1, output_dim=1.
model = nn.Linear(1,1)
```

이를 클래스로 구현하면 다음과 같습니다. ^88b651

```python
class LinearRegressionModel(nn.Module): 
# torch.nn.Module을 상속받는 파이썬 클래스
    def __init__(self): #
        super().__init__()
        self.linear = nn.Linear(1, 1) # 단순 선형 회귀이므로 input_dim=1, output_dim=1.

    def forward(self, x):
        return self.linear(x)
```

```python
model = LinearRegressionModel()
```

위와 같은 클래스를 사용한 모델 구현 형식은 
<< 대부분의 파이토치 구현체에서 사용하고 있는 방식>>으로 
반드시 숙지할 필요가 있습니다.

클래스(class) 형태의 모델은 nn.Module 을 상속받습니다. 
그리고 __init__()에서 모델의 구조와 동작을 정의하는 생성자를 정의합니다. 
이는 파이썬에서 객체가 갖는 속성값을 초기화하는 역할로, 객체가 생성될 때 자동으로 호출됩니다. 

super() 함수를 부르면 여기서 만든 클래스는 nn.Module 클래스의 속성들을 가지고 초기화 됩니다. 
```
`super`를 사용하는 주요 목적

1. 부모의 생성자 호출 (`super()`): 
   자식 클래스에서 객체를 생성할 때, 부모 클래스의 초기화 로직을 
   그대로 사용하거나 확장해야 할 때 필수적으로 사용합니다.
    
2. 부모의 메서드 호출 (`super.메서드명()`): 
   부모 클래스에 정의된 메서드를 오버라이딩(재정의)했을 때, 
   부모의 원래기능 유지하면서 자식만의 기능을 추가하고 싶을 때 사용합니다.
```


foward() 함수는 모델이 학습데이터를 입력받아서 forward 연산을 진행시키는 함수입니다. 
이 forward() 함수는 << model 객체를 데이터와 함께 호출하면 자동으로 실행이됩니다. >> 예를 들어 << model이란 이름의 객체를 생성 후, model(입력 데이터)와 같은 형식으로 객체를 호출하면 자동으로 forward 연산이 수행됩니다. >>

-  $H(x)$ 식에 입력 $x$ 로부터 예측된 $y$ 를 얻는 것을 forward 연산이라고 합니다.

앞서 다중 선형 회귀 모델은 다음과 같이 구현했었습니다.

```python
# 모델을 선언 및 초기화. 다중 선형 회귀이므로 input_dim=3, output_dim=1.
model = nn.Linear(3,1)
```

이를 클래스로 구현하면 다음과 같습니다.

```python
class MultivariateLinearRegressionModel(nn.Module):
    def __init__(self):
        super().__init__()
        self.linear = nn.Linear(3, 1) 
        # 다중 선형 회귀이므로 input_dim=3, output_dim=1.

    def forward(self, x):
        return self.linear(x)
```

```python
model = MultivariateLinearRegressionModel()
```

## 4. 단순 선형 회귀 클래스로 구현하기

이제 모델을 클래스로 구현한 코드를 보겠습니다. 달라진 점은 모델을 클래스로 구현했다는 점 뿐입니다. 다른 코드는 전부 동일합니다.

```python
import torch
import torch.nn as nn
import torch.nn.functional as F
```

```python
torch.manual_seed(1)
```

```python
# 데이터
x_train = torch.FloatTensor([[1], [2], [3]])
y_train = torch.FloatTensor([[2], [4], [6]])
```

```python
class LinearRegressionModel(nn.Module):
    def __init__(self):
        super().__init__()
        self.linear = nn.Linear(1, 1)

    def forward(self, x):
        return self.linear(x)
```

```python
model = LinearRegressionModel()
```

```python
# optimizer 설정. 경사 하강법 SGD를 사용하고 learning rate를 의미하는 lr은 0.01
optimizer = torch.optim.SGD(model.parameters(), lr=0.01) 
```

```python
# 전체 훈련 데이터에 대해 경사 하강법을 2,000회 반복
nb_epochs = 2000
for epoch in range(nb_epochs+1):

    # H(x) 계산
    prediction = model(x_train)

    # cost 계산
    cost = F.mse_loss(prediction, y_train) # <== 파이토치에서 제공하는 평균 제곱 오차 함수

    # cost로 H(x) 개선하는 부분
    # gradient를 0으로 초기화
    optimizer.zero_grad()
    # 비용 함수를 미분하여 gradient 계산
    cost.backward() # backward 연산
    # W와 b를 업데이트
    optimizer.step()

    if epoch % 100 == 0:
    # 100번마다 로그 출력
      print('Epoch {:4d}/{} Cost: {:.6f}'.format(
          epoch, nb_epochs, cost.item()
      ))
```

모델이 주어진 데이터에 대해 2000번 반복하여 학습합니다. 
먼저, 모델은 입력 데이터인 x_train을 사용해 예측 값을 계산합니다. 
이 예측 값과 실제 값인 y_train 간의 차이를 평균 제곱 오차 함수 F.mse_loss를 사용해 계산합니다. 이 차이를 비용이라고 부르며, 비용이 클수록 모델의 예측이 실제 값과 많이 다르다는 것을 의미합니다.

모델이 이 비용을 줄이도록 학습하기 위해, 
먼저 옵티마이저의 기울기를 초기화합니다. 
그런 다음, 비용 함수를 미분하여 각 파라미터에 대한 기울기를 계산합니다. 
이 기울기를 사용해 옵티마이저는 모델의 파라미터를 업데이트하여 비용을 줄이는 방향으로 모델을 개선합니다.

이 과정은 설정한 횟수만큼 반복되며, 모델은 점차 더 정확한 예측을 하도록 학습됩니다. 또한, 학습이 100번 진행될 때마다 현재 에포크 번호와 비용을 출력하여 학습 과정이 어떻게 진행되고 있는지를 확인할 수 있습니다. 이 로그는 학습 중에 모델의 성능이 어떻게 변화하는지를 보여주는 중요한 지표가 됩니다.

## 5. 다중 선형 회귀 클래스로 구현하기

이제 모델을 클래스로 구현한 코드를 보겠습니다. 달라진 점은 모델을 클래스로 구현했다는 점 뿐입니다. 다른 코드는 전부 동일합니다.

```python
import torch
import torch.nn as nn
import torch.nn.functional as F
```

```python
torch.manual_seed(1)
```

```python
# 데이터
x_train = torch.FloatTensor([[73, 80, 75],
                             [93, 88, 93],
                             [89, 91, 90],
                             [96, 98, 100],
                             [73, 66, 70]])
y_train = torch.FloatTensor([[152], [185], [180], [196], [142]])
```

이제 모델 클래스를 만들어봅시다.

```python
class MultivariateLinearRegressionModel(nn.Module):
    def __init__(self):
        super().__init__()
        self.linear = nn.Linear(3, 1) 
        # 다중 선형 회귀이므로 input_dim=3, output_dim=1.

    def forward(self, x):
        return self.linear(x)

model = MultivariateLinearRegressionModel()
```

MultivariateLinearRegressionModel이라는 클래스는 
PyTorch의 nn.Module을 상속받아 정의됩니다. 
이 클래스는 다중 입력 데이터를 받아 단일 출력을 예측하는 선형 회귀 모델을 구현합니다. (lib 내부 구현 구조 .. super 사용을 통해 init함수 받아옴)

클래스 초기화 메서드에서 super()를 호출하여 부모 클래스인 nn.Module의 초기화를 수행한 뒤, nn.Linear 객체를 생성하여 모델의 선형층을 설정합니다. 

이 계층은 입력 차원이 3, 출력 차원이 1로 설정되어 있으며, 이는 3개의 독립 변수를 사용하는 다중 선형 회귀 모델임을 의미합니다. 이제 학습률과 옵티마이저를 설정합니다.

```python
optimizer = torch.optim.SGD(model.parameters(), lr=1e-5) 
```

학습 파라미터를 업데이트하기 위해 확률적 경사 하강법(SGD) 옵티마이저를 설정합니다. 옵티마이저는 model.parameters()를 통해 모델의 모든 학습 가능한 파라미터를 가져와 이를 학습에 사용하며, 
학습률 lr은 모델이 학습할 때 파라미터를 얼마나 빠르게 또는 느리게 업데이트할지를 결정합니다. 이제 2,000 에포크동안 for 루프를 사용하여 학습을 진행해보겠습니다.

```python
nb_epochs = 2000
for epoch in range(nb_epochs+1):

    # H(x) 계산
    prediction = model(x_train)
    # model(x_train)은 model.forward(x_train)와 동일함.

    # cost 계산
    cost = F.mse_loss(prediction, y_train) # <== 파이토치에서 제공하는 평균 제곱 오차 함수

    # cost로 H(x) 개선하는 부분
    # gradient를 0으로 초기화
    optimizer.zero_grad()
    # 비용 함수를 미분하여 gradient 계산
    cost.backward()
    # W와 b를 업데이트
    optimizer.step()

    if epoch % 100 == 0:
    # 100번마다 로그 출력
      print('Epoch {:4d}/{} Cost: {:.6f}'.format(
          epoch, nb_epochs, cost.item()
      ))
```

학습은 모델이 주어진 입력 데이터 x_train을 사용하여 예측값을 계산하는 것으로 시작됩니다. 이 예측값을 모델의 출력($y$)이라고도 하며, model(x_train)을 호출하여 계산됩니다.

사실상, 이는 모델의 forward 메서드를 호출하는 것과 동일한 동작을 합니다.

예측값이 계산된 후, 이 값과 실제 목표값 y_train 간의 차이를 계산하는데, 이 차이를 손실 또는 비용이라고 부릅니다. 여기서는 파이토치의 F.mse_loss 함수를 사용하여 평균 제곱 오차를 계산합니다. 이 비용은 모델이 얼마나 잘못 예측했는지를 나타냅니다.

모델이 이 비용을 줄이도록 학습하기 위해, 먼저 옵티마이저의 기울기 값을 초기화합니다. 그런 다음, 비용 함수를 기준으로 각 파라미터에 대한 기울기를 계산하여 모델의 파라미터를 업데이트합니다. 이 과정은 비용 함수를 모델 파라미터에 대해 미분한 후, 옵티마이저가 이를 사용해 파라미터를 조정하는 방식으로 이루어집니다.

이 과정은 설정한 횟수만큼 반복되며, 모델은 점차적으로 더 정확한 예측을 하도록 학습됩니다. 학습이 100번 진행될 때마다 현재의 학습 단계와 비용을 출력하여 학습의 진행 상황을 모니터링합니다. 이 출력은 모델의 성능이 시간이 지남에 따라 어떻게 변화하고 있는지를 확인하는 데 중요한 역할을 합니다.



# 03-04 미니 배치, 데이터 로더                                   (Mini Batch and DataLoader)

이번 챕터에서 배우는 내용은 선형 회귀에 한정되는 내용은 아닙니다. 
이번 챕터에서는 데이터를 로드하는 방법과 미니 배치 경사 하강법(Minibatch Gradient Descent)에 대해서 학습합니다.

## 1. 미니 배치와 배치 크기(Mini Batch and Batch Size)

앞서 배운 다중 선형 회귀에서 사용했던 데이터를 상기해봅시다.

```python
x_train = torch.FloatTensor([[73, 80, 75],
                             [93, 88, 93],
                             [89, 91, 90],
                             [96, 98, 100],
                             [73, 66, 70]])
y_train = torch.FloatTensor([[152], [185], [180], [196], [142]])
```

위 데이터의 샘플의 개수는 5개입니다. 
전체 데이터를 하나의 행렬로 선언하여 전체 데이터에 대해서 경사 하강법을 수행하여 학습할 수 있습니다. 
그런데 위 데이터는 현업에서 다루게 되는 방대한 양의 데이터에 비하면 굉장히 적은 양입니다. 만약, 데이터가 수십만개 이상이라면 전체 데이터에 대해서 경사 하강법을 수행하는 것은 매우 느릴 뿐만 아니라 많은 계산량이 필요합니다. 
정말 어쩌면 메모리의 한계로 계산이 불가능한 경우도 있을 수 있습니다.

그렇기 때문에 전체 데이터를 더 작은 단위로 나누어서 
해당 단위로 학습하는 개념이 나오게 되었습니다.  
이 단위를 미니 배치(Mini Batch)라고 합니다.

![](https://static.wikidocs.net/images/page/55580/%EB%AF%B8%EB%8B%88%EB%B0%B0%EC%B9%98.PNG)

위의 그림은 전체 데이터를 미니 배치 단위로 나누는 것을 보여줍니다. 
미니 배치 학습을 하게되면 << 미니 배치만큼만 가져가서 미니 배치에 대한 대한 비용(cost)를 계산하고, 경사 하강법을 수행합니다.  >> 

그리고 다음 미니 배치를 가져가서 경사 하강법을 수행하고 마지막 미니 배치까지 이를 반복합니다. 이렇게 << 전체 데이터에 대한 학습이 1회 끝나면 1 에포크(Epoch)가 끝나게 됩니다. >> 

- **에포크(Epoch)는 전체 훈련 데이터가 학습에 한 번 사용된 주기를 말한다고 언급한 바 있습니다.**

미니 배치 학습에서는 << 미니 배치의 개수만큼 >> 경사 하강법을 수행해야 전체 데이터가 한 번 전부 사용되어 1 에포크(Epoch)가 됩니다. 

미니 배치의 개수는 결국 미니 배치의 크기를 몇으로 하느냐에 따라서 달라지는데 미니 배치의 크기를 배치 크기(batch size)라고 합니다.

- **전체 데이터에 대해서 한 번에 경사 하강법을 수행하는 방법을 '배치 경사 하강법'이라고 부릅니다. 반면, 미니 배치 단위로 경사 하강법을 수행하는 방법을 '미니 배치 경사 하강법'이라고 부릅니다.**
    
- **배치 경사 하강법** 은 경사 하강법을 할 때, 전체 데이터를 사용하므로 가중치 값이 최적값에 수렴하는 과정이 매우 안정적이지만, 계산량이 너무 많이 듭니다. 

- **미니 배치 경사 하강법** 은 경사 하강법을 할 때, 전체 데이터의 일부만을 보고 수행하므로 최적값으로 수렴하는 과정에서 값이 조금 헤매기도 하지만 훈련 속도가 빠릅니다.
    
- 배치 크기는 보통 2의 제곱수를 사용합니다. ex) 2, 4, 8, 16, 32, 64... 그 이유는 << CPU와 GPU의 메모리가 2의 배수이므로 배치크기가 2의 제곱수일 경우에 데이터 송수신의 효율을 높일 수 있다고 합니다. >>
    

## 2. 이터레이션(Iteration)

미니 배치와 배치 크기의 정의에 대해서 이해하였다면 
이터레이션(iteration)을 정의할 수 있습니다.

![](https://static.wikidocs.net/images/page/36033/batchandepochiteration.PNG)

위의 그림은 에포크와 배치 크기와 이터레이션의 관계를 보여줍니다. 
위의 그림의 예제를 통해 설명해보겠습니다.

이터레이션은 한 번의 에포크 내에서 이루어지는 매개변수인 가중치 $W$와 $b$의 업데이트 횟수입니다. 전체 데이터가 2,000일 때 배치 크기를 200으로 한다면 이터레이션의 수는 총 10개입니다. 이는 한 번의 에포크 당 매개변수 업데이트가 10번 이루어짐을 의미합니다. (총 데이터를 한 번 순회할 수 있는 반복 횟수? 주기? 라고 생각하면 될 듯. 얼만큼의 묶음이 나오는가?)

이제 미니 배치 학습을 할 수 있도록 도와주는 파이토치의 도구들을 알아봅시다.

## 3. 데이터 로드하기(Data Load)

파이토치에서는 데이터를 좀 더 쉽게 다룰 수 있도록 유용한 도구로서 데이터셋(Dataset)과 데이터로더(DataLoader)를 제공합니다. ^dataloader

이를 사용하면 **미니 배치 학습**, 데이터 셔플(shuffle), 병렬 처리까지 간단히 수행할 수 있습니다. 
기본적인 사용 방법은 Dataset을 정의하고, 이를 DataLoader에 전달하는 것입니다.

Dataset을 커스텀하여 만들 수도 있지만 여기서는 << 텐서를 입력받아 Dataset의 형태로 변환해주는 TensorDataset을 사용해보겠습니다 >>.

실습을 위해 기본적으로 필요한 파이토치의 도구들을 임포트합니다.

```python
import torch
import torch.nn as nn
import torch.nn.functional as F
```

TensorDataset과 DataLoader를 임포트합니다.

```python
from torch.utils.data import TensorDataset # 텐서데이터셋
from torch.utils.data import DataLoader # 데이터로더
```

TensorDataset은 기본적으로 텐서를 입력으로 받습니다. 
텐서 형태로 데이터를 정의합니다.

```python
x_train  =  torch.FloatTensor([[73,  80,  75], 
                               [93,  88,  93], 
                               [89,  91,  90], 
                               [96,  98,  100],   
                               [73,  66,  70]])  
y_train  =  torch.FloatTensor([[152],  [185],  [180],  [196],  [142]])
```

이제 이를 TensorDataset의 입력으로 사용하고 dataset으로 저장합니다.

```python
dataset = TensorDataset(x_train, y_train)
# 파이토치의 데이터셋
```

파이토치의 데이터셋을 만들었다면 데이터로더를 사용 가능합니다. 
데이터로더는 기본적으로 2개의 인자를 입력받습니다. 

하나는 데이터셋, 미니 배치의 크기입니다. 이때 미니 배치의 크기는 통상적으로 
2의 배수를 사용합니다. (ex) 64, 128, 256...) 
그리고 추가적으로 많이 사용되는 인자로 shuffle이 있습니다. shuffle=True를 선택하면 Epoch마다 데이터셋을 섞어서 데이터가 학습되는 순서를 바꿉니다.

사람도 같은 문제지를 계속 풀면 어느 순간 문제의 순서에 익숙해질 수 있습니다. 예를 들어 어떤 문제지의 12번 문제를 풀면서, '13번 문제가 뭔지는 기억은 안 나지만 어제 풀었던 기억으로 정답은 5번이었던 것 같은데' 하면서 문제 자체보단 순서에 익숙해질 수 있다는 것입니다. 

그럴 때 문제지를 풀 때마다 문제 순서를 랜덤으로 바꾸면 도움이 될 겁니다. 
마찬가지로 모델이 데이터셋의 순서에 익숙해지는 것을 방지하여 학습할 때는 이 옵션을 True를 주는 것을 권장합니다.

```python
dataloader = DataLoader(dataset, batch_size=2, shuffle=True)
```

이제 모델과 옵티마이저를 설계합니다.

```python
model = nn.Linear(3,1)
optimizer = torch.optim.SGD(model.parameters(), lr=1e-5) 
```

이제 훈련을 진행합니다. 아래 코드에서는 batch_idx와 samples를 주석 처리했는데 어떤 식으로 훈련되고 있는지 궁금하다면 주석 처리를 해제하고 훈련시켜보시기 바랍니다.

```python
nb_epochs = 20
for epoch in range(nb_epochs + 1):
  for batch_idx, samples in enumerate(dataloader):
    # print(batch_idx)
    # print(samples)
    x_train, y_train = samples
    # H(x) 계산
    prediction = model(x_train)

    # cost 계산
    cost = F.mse_loss(prediction, y_train)

    # cost로 H(x) 계산
    optimizer.zero_grad()
    cost.backward()
    optimizer.step()

    print('Epoch {:4d}/{} Batch {}/{} Cost: {:.6f}'.format(
        epoch, nb_epochs, batch_idx+1, len(dataloader),
        cost.item()
        ))
```

```python
Epoch    0/20 Batch 1/3 Cost: 26085.919922
Epoch    0/20 Batch 2/3 Cost: 3660.022949
Epoch    0/20 Batch 3/3 Cost: 2922.390869
... 중략 ...
Epoch   20/20 Batch 1/3 Cost: 6.315856
Epoch   20/20 Batch 2/3 Cost: 13.519956
Epoch   20/20 Batch 3/3 Cost: 4.262849
```

Cost의 값이 점차 작아집니다. (사실 아직 에포크를 더 늘려서 훈련하면 Cost의 값이 더 작아질 여지가 있습니다. 에포크를 늘려서도 훈련해보세요.) 
이제 모델의 입력으로 임의의 값을 넣어 예측값을 확인합니다.

```python
# 임의의 입력 [73, 80, 75]를 선언
new_var =  torch.FloatTensor([[73, 80, 75]]) 
# 입력한 값 [73, 80, 75]에 대해서 예측값 y를 리턴받아서 pred_y에 저장
pred_y = model(new_var) 
print("훈련 후 입력이 73, 80, 75일 때의 예측값 :", pred_y) 
```

```lua
훈련 후 입력이 73, 80, 75일 때의 예측값 : tensor([[154.3850]], grad_fn=<AddmmBackward>)
```

앞 내용을 잠깐 복습해봅시다. 파이토치에서는 데이터셋을 좀 더 쉽게 다룰 수 있도록 유용한 도구로서 torch.utils.data.Dataset과 torch.utils.data.DataLoader를 제공합니다. 이를 사용하면 미니 배치 학습, 데이터 셔플(shuffle), 병렬 처리까지 간단히 수행할 수 있습니다. 
기본적인 사용 방법은 Dataset을 정의하고, 이를 DataLoader에 전달하는 것입니다.

## 4. 커스텀 데이터셋(Custom Dataset)

그런데 torch.utils.data.Dataset을 상속받아 직접 << 커스텀 데이터셋(Custom Dataset)을 만드는 경우도 있습니다. >>
torch.utils.data.Dataset은 파이토치에서 데이터셋을 제공하는 추상 클래스입니다. Dataset을 상속받아 다음 메소드들을 오버라이드 하여 커스텀 데이터셋을 만들어보겠습니다.

커스텀 데이터셋을 만들 때, 일단 가장 기본적인 뼈대는 아래와 같습니다. 여기서 필요한 기본적인 define은 3개입니다.

```python
class CustomDataset(torch.utils.data.Dataset): 
  def __init__(self):

  def __len__(self):

  def __getitem__(self, idx): 
```

이를 좀 더 자세히 봅시다.

```python
class CustomDataset(torch.utils.data.Dataset): 
  def __init__(self):
  데이터셋의 전처리를 해주는 부분

  def __len__(self):
  데이터셋의 길이 = 즉, 총 샘플의 수를 적어주는 부분

  def __getitem__(self, idx): 
  데이터셋에서 특정 1개의 샘플을 가져오는 함수
```

- len(dataset)을 했을 때 데이터셋의 크기를 리턴할 **len**
- dataset[i]을 했을 때 i번째 샘플을 가져오도록 하는 인덱싱을 위한 **get_item**

=> 왜 필요할까??
클래스로 직접 데이터셋을 만드는 이유는 파이토치의 강력한 도구인 `DataLoader`와 매끄럽게 연결하기 위해서입니다. 왜 이런 구조를 사용하는지, 내부적으로 어떤 일이 일어나는지 핵심만 짚어드릴게요.

### 1. 왜 굳이 이런 '형식'을 갖춰야 할까?

파이토치는 데이터가 아무리 크고 복잡해도 "데이터를 어떻게 가져오고(idx), 전체 길이가 얼마인지(len)"만 통일된 방식으로 알려주면, 
<< 알아서 데이터를 섞거나(Shuffle) 배치(Batch) 단위로 잘라주는 기능(`DataLoader`)을 제공합니다. >>
=> 즉, 내가 원하는 형태의 데이터셋을 만지면서도 파이토치 lib 의 기능들을 이용가능하게 하려면 이런 형태로 선언해줘야 한다는 것이다.

- **`__len__`**: "데이터가 총 몇 개야?" (학습이 몇 번 반복될지 결정)
    
- **`__getitem__`**: "idx번째 데이터 하나만 줘!" (실제 학습에 사용할 데이터 로딩)
    

이렇게 클래스로 만들어두면, 나중에 데이터가 100만 개로 늘어나서 메모리에 다 올릴 수 없을 때, **`__getitem__` 내부만 수정하여** 디스크에서 데이터를 하나씩 불러오도록(Lazy Loading) 손쉽게 변경할 수 있습니다.

### 2. 코드의 흐름 (이게 왜 편한가요?)

작성하신 코드를 `DataLoader`에 넣으면 이런 마법이 일어납니다.

```Python
dataset = CustomDataset()
dataloader = DataLoader(dataset, batch_size=2, shuffle=True)

for x_batch, y_batch in dataloader:
    # 1. 셔플(Shuffle)이 자동으로 됨
    # 2. 배치(Batch) 크기인 2개씩 묶어서 가져옴
    # 3. 모델에 바로 넣을 수 있는 텐서 형태임
    print(x_batch, y_batch)
```

### 3. 커스텀 데이터셋 구조의 이점

- **메모리 효율:** 데이터를 한꺼번에 메모리에 올리지 않고, 필요할 때마다 `__getitem__`을 통해 꺼내 쓸 수 있습니다.
    
- **복잡한 전처리:** `__getitem__` 안에서 이미지 변환, 정규화, 텍스트 토큰화 등 학습에 필요한 **복잡한 가공을 데이터가 호출될 때마다 즉석에서(On-the-fly)** 처리할 수 있습니다.
    
- **확장성:** 엑셀, 이미지, CSV, 사운드 등 어떤 형태의 데이터든 이 구조만 지키면 똑같은 방식으로 모델을 학습시킬 수 있습니다.
    

### 요약하자면

작성하신 코드는 "내 데이터를 파이토치가 이해할 수 있는 '규격화된 박스'에 담는 과정"입니다. 이 박스에 담아두기만 하면, 파이토치가 제공하는 학습 최적화 도구들을 100% 활용할 수 있기 때문에 굳이 번거롭게 클래스를 상속받아 사용하는 것입니다.


## 5. 커스텀 데이터셋(Custom Dataset)으로                        선형 회귀 구현하기

```python
import torch
import torch.nn.functional as F
from torch.utils.data import Dataset
from torch.utils.data import DataLoader
```

```python
# Dataset 상속
class CustomDataset(Dataset): 
  def __init__(self):
    self.x_data = [[73, 80, 75],
                   [93, 88, 93],
                   [89, 91, 90],
                   [96, 98, 100],
                   [73, 66, 70]]
    self.y_data = [[152], [185], [180], [196], [142]]

  # 총 데이터의 개수를 리턴
  def __len__(self): 
    return len(self.x_data)

  # 인덱스를 입력받아 그에 맵핑되는 입출력 데이터를 파이토치의 Tensor 형태로 리턴
  def __getitem__(self, idx): 
    x = torch.FloatTensor(self.x_data[idx])
    y = torch.FloatTensor(self.y_data[idx])
    return x, y
```

위 코드는 PyTorch의 Dataset 클래스를 상속받아 사용자 정의 데이터셋 클래스를 만드는 방법을 보여줍니다. 
CustomDataset이라는 클래스는 두 가지 데이터를 포함합니다. 
x_data는 입력 데이터이고, y_data는 해당 입력에 대응하는 출력 데이터입니다.

클래스가 초기화될 때, 이 두 데이터를 내부 변수로 저장합니다. 
데이터셋의 길이를 반환하는 메서드가 정의되어 있으며, 이 메서드는 데이터셋에 포함된 데이터의 개수를 반환합니다. 

이 클래스에서 가장 중요한 메서드는 << 인덱스를 입력으로 받아 해당 인덱스에 맵핑된 데이터를 반환하는 것입니다. >>

이 메서드는 x_data와 y_data의 특정 인덱스에 해당하는 데이터를 torch.FloatTensor 형식으로 변환하여 반환합니다.

```python
dataset = CustomDataset()
dataloader = DataLoader(dataset, batch_size=2, shuffle=True)
```

```python
model = torch.nn.Linear(3,1)
optimizer = torch.optim.SGD(model.parameters(), lr=1e-5) 
```

위 코드는 CustomDataset 클래스를 인스턴스화하여 데이터셋 객체를 만듭니다. 
이 데이터셋을 기반으로 PyTorch의 DataLoader 객체를 생성하며, 
여기서 배치 크기를 설정하고 데이터를 무작위로 섞을지 여부를 결정합니다. 

배치 크기는 한 번에 몇 개의 데이터를 모델에 입력할지 결정하며, 무작위 섞기는 모델이 학습하는 동안 데이터 순서에 의한 편향을 방지하기 위해 사용됩니다.

그 다음, 입력 차원이 3이고 출력 차원이 1인 선형 회귀 모델을 정의합니다. 
이 모델은 torch.nn.Linear 클래스를 사용하여 구현됩니다. 

모델이 데이터에서 학습할 수 있도록 옵티마이저를 설정하는데, 여기서는 확률적 경사 하강법(SGD) 옵티마이저를 사용합니다. 옵티마이저는 모델의 파라미터를 업데이트할 때 학습률을 사용하며, 학습률은 모델이 얼마나 빠르게 또는 느리게 학습할지를 결정하는 중요한 하이퍼파라미터입니다.

```python
nb_epochs = 20
for epoch in range(nb_epochs + 1):
  for batch_idx, samples in enumerate(dataloader):
    # print(batch_idx)
    # print(samples)
    x_train, y_train = samples
    # H(x) 계산
    prediction = model(x_train)

    # cost 계산
    cost = F.mse_loss(prediction, y_train)

    # cost로 H(x) 계산
    optimizer.zero_grad()
    cost.backward()
    optimizer.step()

    print('Epoch {:4d}/{} Batch {}/{} Cost: {:.6f}'.format(
        epoch, nb_epochs, batch_idx+1, len(dataloader),
        cost.item()
        ))
```

전체 학습 반복 횟수는 nb_epochs로 설정되어 있습니다. 외부 반복문에서는 각 학습 주기, 즉 에포크를 제어하며, 내부 반복문에서는 데이터 로더에서 미니배치 단위로 데이터를 가져와 모델을 학습시킵니다.

데이터 로더에서 가져온 << 각 배치는 samples라는 변수에 저장 >> 되고, 
이 배치에서 입력 데이터 x_train과 타겟 데이터 y_train을 분리합니다. 

모델은 입력 데이터에 대해 예측 값을 계산하고, 그 예측 값과 실제 타겟 값 사이의 손실을 평균 제곱 오차 함수 F.mse_loss를 사용해 계산합니다.

계산된 손실 값을 기반으로 모델의 가중치를 업데이트하기 위해 기울기를 초기화한 후, 손실에 대한 역전파를 수행하고, 옵티마이저를 사용해 모델의 파라미터를 업데이트합니다.

각 에포크와 배치가 끝날 때마다 현재 에포크와 배치 번호, 그리고 해당 배치에서의 손실 값이 출력됩니다. 이를 통해 학습이 잘 진행되고 있는지를 실시간으로 모니터링할 수 있습니다.

```python
Epoch    0/20 Batch 1/3 Cost: 29410.156250
Epoch    0/20 Batch 2/3 Cost: 7150.685059
Epoch    0/20 Batch 3/3 Cost: 3482.803467
... 중략 ...
Epoch   20/20 Batch 1/3 Cost: 0.350531
Epoch   20/20 Batch 2/3 Cost: 0.653316
Epoch   20/20 Batch 3/3 Cost: 0.010318
```

```python
# 임의의 입력 [73, 80, 75]를 선언
new_var =  torch.FloatTensor([[73, 80, 75]]) 
# 입력한 값 [73, 80, 75]에 대해서 예측값 y를 리턴받아서 pred_y에 저장
pred_y = model(new_var) 
print("훈련 후 입력이 73, 80, 75일 때의 예측값 :", pred_y) 
```

```python
훈련 후 입력이 73, 80, 75일 때의 예측값 : tensor([[151.2319]], grad_fn=<AddmmBackward>)
```




# 03-05 벡터/행렬 연산 Review 

앞서 독립 변수 $x$가 2개 이상인 다중 선형 회귀를 배웠습니다. 
이후에 배우게 될 실습인 소프트맥스 회귀에서는 << 종속 변수 $y$의 종류도 3개 이상이 되면서 더욱 복잡해집니다. >> 
그리고 이러한 식들이 겹겹이 누적되면 인공 신경망의 개념이 됩니다.
(소프트맥스=다부류 , y가 3개이상 / 로지스틱=이진분류, y가2개)
(인공 신경망 - 레이어 하나 = 선형변환+활성화함수 => 이걸 겹겹이 쌓은게 MLP=다층퍼셉트론.  소프트맥스회귀=다부류분류=선형변환한번+소프트맥스 
인공신경망 = 선형변환+활성화함수 자체를 여러번반복한후 마지막에 소프트맥스적용)

우리는 각 변수들의 연산을 벡터와 행렬 연산으로 이해할 수 있어야 합니다. 
다시 말해 사용자가 데이터와 변수의 개수로부터 행렬의 크기, 더 나아가 텐서의 크기를 산정할 수 있어야 합니다. 

앞서 **텐서 조작하기 실습**을 통해서 벡터와 행렬에 대해서 언급한 바 있습니다. 여기서 기본적인 벡터와 행렬 연산에 대해서 다시 복습해보겠습니다.

## 1. 벡터와 행렬과 텐서

벡터는 크기와 방향을 가진 양입니다. 
숫자가 나열된 형상이며 파이썬에서는 1차원 배열 또는 리스트로 표현합니다. 

반면, 행렬은 행과 열을 가지는 2차원 형상을 가진 구조입니다. 
파이썬에서는 2차원 배열로 표현합니다. 가로줄을 행(row)라고 하며, 세로줄을 열(column)이라고 합니다. 3차원부터는 주로 텐서라고 부릅니다. 

텐서는 파이썬에서는 3차원 이상의 배열로 표현합니다.

## 2. 텐서(Tensor)

인공 신경망은 복잡한 모델 내의 연산을 주로 행렬 연산을 통해 해결합니다. 
그런데 여기서 말하는 행렬 연산이란 단순히 2차원 배열을 통한 행렬 연산만을 의미하는 것이 아닙니다. 

머신 러닝의 입, 출력이 복잡해지면 << **3차원 텐서**에 대한 이해>> 가 필수로 요구됩니다. 
예를 들어 인공 신경망 모델 중 하나인 RNN에서는 3차원 텐서에 대한 개념 이해 없이는 이해하기가 쉽지 않습니다.

Numpy를 사용하여 텐서를 설명해보겠습니다.

```python
import numpy as np
```

### 1) 0차원 텐서(스칼라)

스칼라는 하나의 실수값으로 이루어진 데이터를 말합니다. 이를 0차원 텐서라고 합니다. 차원을 영어로 Dimension이라고 하므로 0D 텐서라고도 합니다.

```python
d = np.array(5)
print('텐서의 차원 :',d.ndim)
print('텐서의 크기(shape) :',d.shape)
```

```python
텐서의 차원 : 0
텐서의 크기(shape) : ()
```

Numpy의 ndim을 출력했을 때 나오는 값에 주목합시다. 
ndim을 출력했을 때 나오는 값을 우리는 축(axis)의 개수 또는 
텐서의 차원이라고 부릅니다. 반드시 이 두 용어를 기억해둡시다.

### 2) 1차원 텐서(벡터)

숫자를 배열한 것을 벡터라고합니다. 벡터는 1차원 텐서입니다. 
주의할 점은 벡터에서도 차원이라는 용어를 쓰는데, 
벡터의 차원과 텐서의 차원은 다른 개념이라는 점입니다. 
아래의 예제는 4차원 벡터이지만, 1차원 텐서입니다. 1D 텐서라고도 합니다.

```python
d = np.array([1, 2, 3, 4])
print('텐서의 차원 :',d.ndim)
print('텐서의 크기(shape) :',d.shape)
```

```python
텐서의 차원 : 1
텐서의 크기(shape) : (4,)
```

벡터의 차원과 텐서의 차원의 정의로 인해 혼동할 수 있는데 
벡터에서의 차원은 하나의 축에 놓인 원소의 개수를 의미하는 것이고, 
텐서에서의 차원은 축의 개수를 의미합니다.

### 3) 2차원 텐서(행렬)

행과 열이 존재하는 벡터의 배열. 
즉, 행렬(matrix)을 2차원 텐서라고 합니다. 2D 텐서라고도 합니다.

```python
# 3행 4열의 행렬
d = np.array([[1, 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12]])
print('텐서의 차원 :',d.ndim)
print('텐서의 크기(shape) :',d.shape)
```

```python
텐서의 차원 : 2
텐서의 크기(shape) : (3, 4)
```

텐서의 크기(shape)에 대해서도 정리합시다. 
텐서의 크기란, 각 축을 따라서 얼마나 많은 차원이 있는지를 나타낸 값입니다. 
텐서의 크기를 바로 머릿속으로 떠올릴 수 있으면 모델 설계 시에 유용합니다. 

처음에는 어려울 수도 있는데, 순차적으로 확장해나가며 생각하는 것도 방법입니다. 위의 경우 3개의 커다란 데이터가 있는데 그 각각의 커다란 데이터는 작은 데이터 4개로 이루어졌다고 생각할 수 있습니다.

### 4) 3차원 텐서(다차원 배열)

행렬 또는 2차원 텐서를 단위로 한 번 더 배열하면 3차원 텐서라고 부릅니다. 
3D 텐서라고도 합니다. 
사실 위에서 언급한 0차원 ~ 2차원 텐서는 각각 스칼라, 벡터, 행렬이라고 해도 무방하므로 3차원 이상의 텐서부터 본격적으로 텐서라고 부릅니다. 

데이터 사이언스 분야 한정으로 
주로 3차원 이상의 배열을 텐서라고 부른다고 이해해도 좋습니다. 
3D 텐서는 적어도 여기서는 3차원 배열로 이해하면 되겠습니다. 

이 3차원 텐서의 구조를 이해하지 않으면, 복잡한 인공 신경망의 입, 출력값을 이해하는 것이 쉽지 않습니다. 개념 자체는 어렵지 않지만 반드시 알아야하는 개념입니다.

```python
d = np.array([
            [[1, 2, 3, 4, 5], [6, 7, 8, 9, 10], [10, 11, 12, 13, 14]],
            [[15, 16, 17, 18, 19], [19, 20, 21, 22, 23], [23, 24, 25, 26, 27]]
            ])
print('텐서의 차원 :',d.ndim)
print('텐서의 크기(shape) :',d.shape)
```

```python
텐서의 차원 : 3
텐서의 크기(shape) : (2, 3, 5)
```

자연어 처리에서 특히 자주 보게 되는 것이 이 3D 텐서입니다. 
3D 텐서는 시퀀스 데이터(sequence data)를 표현할 때 자주 사용되기 때문입니다. 
여기서 시퀀스 데이터는 주로 < 단어의 시퀀스 > 를 의미하며, 
시퀀스는 주로 문장이나 문서, 뉴스 기사 등의 텍스트가 될 수 있습니다. 이 경우 3D 텐서는 (samples, timesteps, word_dim)이 됩니다. 
또는 일괄로 처리하기 위해 데이터를 묶는 단위인 배치의 개념에 대해서 뒤에서 배울텐데 (batch_size, timesteps, word_dim)이라고도 볼 수 있습니다.

samples 또는 batch_size는 샘플의 개수, timesteps는 시퀀스의 길이, 
word_dim은 단어를 표현하는 벡터의 차원을 의미합니다. 

더 상세한 설명은 RNN 챕터에서 배우게 되겠지만 자연어 처리에서 왜 3D 텐서의 개념이 사용되는지 간단한 예를 들어봅시다.
다음과 같은 3개의 훈련 데이터가 있다고 해봅시다.

- 문서1 : I like NLP
- 문서2 : I like DL
- 문서3 : DL is AI

이를 인공 신경망의 모델의 입력으로 사용하기 위해서는 
각 단어를 벡터화해야 합니다. 

단어를 벡터화하는 방법으로는 원-핫 인코딩이나 워드 임베딩이라는 방법이 대표적입니다. 워드 임베딩은 아직 배우지 않았으므로 원-핫 인코딩으로 각 단어를 벡터화 해보겠습니다.

|단어|One-hot vector|
|---|---|
|I|[1 0 0 0 0 0]|
|like|[0 1 0 0 0 0]|
|NLP|[0 0 1 0 0 0]|
|DL|[0 0 0 1 0 0]|
|is|[0 0 0 0 1 0]|
|AI|[0 0 0 0 0 1]|

훈련 데이터의 단어들을 모두 원-핫 벡터로 바꿔서 인공 신경망의 입력으로 한 꺼번에 사용한다고 하면 다음과 같습니다. 
이렇게 훈련 << 데이터를 다수 묶어 입력으로 사용하는 것을 딥 러닝에서는 배치(Batch)라고 합니다. >>

```python
[[[1, 0, 0, 0, 0, 0], [0, 1, 0, 0, 0, 0], [0, 0, 1, 0, 0, 0]],  
[[1, 0, 0, 0, 0, 0], [0, 1, 0, 0, 0, 0], [0, 0, 0, 1, 0, 0]],  
[[0, 0, 0, 1, 0, 0], [0, 0, 0, 0, 1, 0], [0, 0, 0, 0, 0, 1]]]  
```

이는 (3, 3, 6)의 크기를 가지는 3D 텐서입니다.

### 5) 그 이상의 텐서

3차원 텐서를 배열로 합치면 4차원 텐서가 됩니다. 
4차원 텐서를 배열로 합치면 5차원 텐서가 됩니다. 
이렇게 텐서는 다차원 배열로서 계속해서 확장될 수 있습니다.

![](https://static.wikidocs.net/images/page/37001/tensor.png)

위의 그림은 각 텐서를 도형으로 시각화한 모습을 보여줍니다.

### 6) PyTorch에서의 텐서

2챕터의 '텐서 조작하기' 실습을 참고하세요.

## 3. 벡터와 행렬의 연산

벡터와 행렬의 기본적인 연산에 대해서 알아보겠습니다.

```python
import numpy as np
```

### 1) 벡터와 행렬의 덧셈과 뺄셈

같은 크기의 두 개의 벡터나 행렬은 덧셈과 뺄셈을 할 수 있습니다. 
이 경우 같은 위치의 원소끼리 연산하면 됩니다. 

이러한 연산을 요소별(element-wise) 연산이라고 합니다.

다음과 같이 A와 B라는 두 개의 벡터가 있다고 해봅시다.

![[Pasted image 20260707190110.png|167]]

이때 두 벡터 A와 B의 덧셈과 뺄셈은 아래와 같습니다.
![[Pasted image 20260707190123.png|216]]

Numpy를 이용하여 이를 구현할 수 있습니다.

```python
A = np.array([8, 4, 5])
B = np.array([1, 2, 3])
print('두 벡터의 합 :',A+B)
print('두 벡터의 차 :',A-B)
```

```python
두 행렬의 합 : [9 6 8]
두 행렬의 차 : [7 2 2]
```

행렬도 마찬가지입니다. A와 B라는 두 개의 행렬이 있다고 하였을 때, 
두 행렬 A와 B의 덧셈과 뺄셈은 아래와 같습니다.

![[Pasted image 20260707190150.png|519]]
![[Pasted image 20260707190204.png|524]]

Numpy를 이용하여 이를 구현할 수 있습니다.

```python
A = np.array([[10, 20, 30, 40], [50, 60, 70, 80]])
B = np.array([[5, 6, 7, 8],[1, 2, 3, 4]])
print('두 행렬의 합 :')
print(A + B)
print('두 행렬의 차 :')
print(A - B)
```

```python
두 행렬의 합 :
[[15 26 37 48]
 [51 62 73 84]]
두 행렬의 차 :
[[ 5 14 23 32]
 [49 58 67 76]]
```

### 2) 벡터의 내적과 행렬의 곱셈

벡터의 점곱(dot product) 또는 내적(inner product)에 대해 알아봅시다. 
벡터의 내적은 연산을 점(dot)으로 표현하여 와 같이 표현하기도 합니다.

내적이 성립하기 위해서는 두 벡터의 <차원이 같아야> 하며, 
두 벡터 중 앞의 벡터가 행벡터(가로 방향 벡터)이고 뒤의 벡터가 열벡터(세로 방향 벡터)여야 합니다. 아래는 두 벡터의 차원이 같고 곱셈의 대상이 각각 행벡터이고 열벡터일 때 내적이 이루어지는 모습을 보여줍니다. 벡터의 내적의 결과는 스칼라가 된다는 특징이 있습니다.

![[Pasted image 20260707190311.png|480]]

Numpy를 이용하여 이를 구현할 수 있습니다.

```python
A = np.array([1, 2, 3])
B = np.array([4, 5, 6])
print('두 벡터의 내적 :',np.dot(A, B))
```

```python
두 벡터의 내적 : 32
```

행렬의 곱셈을 이해하기 위해서는 벡터의 내적을 이해해야 합니다. 
행렬의 곱셈은 왼쪽 행렬의 행벡터(가로 방향 벡터)와 오른쪽 행렬의 열벡터(세로 방향 벡터)의 내적(대응하는 원소들의 곱의 합)이 결과 행렬의 원소가 되는 것으로 이루어집니다. 
다음과 같이 A와 B라는 두 개의 행렬이 있다고 하였을 때, 두 행렬 A와 B의 행렬의 곱셈은 아래와 같습니다.

![[Pasted image 20260707190406.png|556]]

Numpy를 이용하여 이를 구현할 수 있습니다.

```python
A = np.array([[1, 3],[2, 4]])
B = np.array([[5, 7],[6, 8]])
print('두 행렬의 행렬곱 :')
print(np.matmul(A, B))
```

```python
두 행렬의 행렬곱 :
[[23 31]
 [34 46]]
```

행렬의 곱셈은 딥 러닝을 이해하기 위해 필수적인 개념이므로 반드시 숙지해야 합니다. 행렬 곱셈에서의 주요한 두 가지 조건 또한 반드시 기억해둡시다.

- 두 행렬의 곱 A × B이 성립되기 위해서는 행렬 A의 열의 개수와 행렬 B의 행의 개수는 같아야 한다.
- 두 행렬의 곱 A × B의 결과로 나온 행렬 AB의 크기는 A의 행의 개수와 B의 열의 개수를 가진다.

벡터와 행렬의 곱 또는 행렬과 벡터의 곱 또한 행렬의 곱셈과 동일한 원리로 이루어집니다. (그냥 한쪽이 1xn인 행렬이랑 같음)

## 4. 다중 선형 회귀 행렬 연산으로 이해하기

독립 변수가 2개 이상일 때, 1개의 종속 변수를 예측하는 문제를 
행렬의 연산으로 표현한다면 어떻게 될까요? 

다중 선형 회귀나 다중 로지스틱 회귀가 이러한 연산의 예인데, 
여기서는 다중 선형 회귀를 통해 예를 들어보겠습니다. 

다음은 독립 변수 가 n개인 다중 선형 회귀 수식입니다.

$$y=w_1x_1+w_2x_2+w_3x_3+...+w_nx_n+b$$

이는 입력 벡터 $[x_1,...,x_n]$ 와 가중치 벡터 $[w_1,...,w_n]$ 의 내적으로 표현할 수 있습니다.

![[Pasted image 20260707191931.png|465]]

또는 가중치 벡터 $[w_1,...,w_n]$ 와 입력 벡터 $[x_1,...,x_n]$ 의 내적으로 표현할 수도 있습니다.

![[Pasted image 20260707192056.png|479]]


샘플의 개수가 많을 경우에는 행렬의 곱셈으로 표현이 가능합니다. 

> 다음은 집의 크기, 방의 수, 층의 수, 집이 얼마나 오래되었는지와 
   집의 가격이 기록된 부동산 데이터라고 가정합시다. 해당 데이터를 학습하여 새로운 집의 정보가 들어왔을 때, 집의 가격을 예측하는 모델을 구현한다고 해봅시다.

![[Pasted image 20260707223434.png|533]]

위 데이터에 대해서 입력 행렬 $X$와 가중치 벡터 $W$의 곱으로 표현하면 다음과 같습니다.

![[Pasted image 20260707192609.png|441]]

여기에 편향 벡터 $B$를 더 해주면 위 데이터에 대한 전체 가설 수식 $H(X)$를 표현할 수 있습니다.

![[Pasted image 20260707192620.png|345]]

$H(X)=XW+B$

위의 수식에서 입력 행렬 $X$는 5행 4열의 크기를 가집니다. 
출력 벡터를 $Y$라고 하였을 때 $Y$는 5행 1열의 크기를 가집니다. 

여기서 곱셈이 성립하기 위해서 가중치 벡터 %W%의 크기는 4행 1열을 가져야함을 추론할 수 있습니다. 만약 가중치 벡터를 앞에 두고 입력 행렬을 뒤에 두고 행렬 연산을 한다면 이는 아래와 같습니다.

![[Pasted image 20260707192846.png|458]]

수학적 관례로 아래와 같이 수식으로 표현할 때는 
주로 가중치 $W$가 입력 $X$의 앞에 오는 편입니다.

$H(X)=WX+B$

<< 인공 신경망도 본질적으로 위와 같은 행렬 연산입니다. >>


## 5. 샘플(Sample)과 특성(Feature)

훈련 데이터의 입력 행렬을 $X$라고 하였을 때 
샘플(Sample)과 특성(Feature)의 정의는 다음과 같습니다.

![](https://static.wikidocs.net/images/page/35821/n_x_m.PNG)

머신 러닝에서는 데이터를 셀 수 있는 단위로 구분할 때, 각각을 샘플이라고 부르며
종속 변수 $y$를 예측하기 위한 각각의 << 독립 변수 $x$를 특성이라고 부릅니다. >>

## 6. 가중치와 편향 행렬의 크기 결정

앞서 언급하였던 행렬 곱셈의 두 가지 주요한 조건을 기억해둡시다.

- 두 행렬의 곱 J × K이 성립되기 위해서는 행렬 J의 열의 개수와 행렬 K의 행의 개수는 같아야 한다.
- 두 행렬의 곱 J × K의 결과로 나온 행렬 JK의 크기는 J의 행의 개수와 K의 열의 개수를 가진다.

이로부터 입력과 출력의 행렬의 크기로부터 가중치 행렬 W와 편향 행렬 B의 크기를 찾아낼 수 있습니다. 독립 변수 행렬을 X, 종속 변수 행렬을 Y라고 하였을 때, 이때 행렬 X를 입력 행렬(Input Matrix), Y를 출력 행렬(Output Matrix)이라고 합시다.

![](https://static.wikidocs.net/images/page/37001/matrix1.PNG)

이제 입력 행렬의 크기와 출력 행렬의 크기로부터 
W행렬과 B행렬의 크기를 추론해봅시다.

![](https://static.wikidocs.net/images/page/37001/matrix2.PNG)

행렬의 덧셈에 해당되는 B행렬은 Y행렬의 크기에 영향을 주지 않습니다. 그러므로 B행렬의 크기는 Y행렬의 크기와 같습니다.

![](https://static.wikidocs.net/images/page/37001/matrix3.PNG)

행렬의 곱셈이 성립되려면 행렬의 곱셈에서 앞에 있는 행렬의 열의 크기와 뒤에 있는 행렬의 행의 크기는 같아야 합니다. 그러므로 입력 행렬 X로부터 W행렬의 행의 크기가 결정됩니다.

![](https://static.wikidocs.net/images/page/37001/matrix4.PNG)

두 행렬의 곱의 결과로서 나온 행렬의 열의 크기는 행렬의 곱셈에서 뒤에 있는 행렬의 열의 크기와 동일합니다. 그러므로 출력 행렬 Y로부터 W행렬의 열의 크기가 결정됩니다. 

입력 행렬과 출력 행렬의 크기로부터 가중치 행렬과 편향 행렬의 크기를 추정할 수 있다면, 딥 러닝 모델을 구현하였을 때 해당 모델에 << 존재하는 총 매개변수의 개수를 계산하기 쉽습니다. >>어떤 딥 러닝 모델의 총 매개변수의 개수는 해당 모델에 존재하는 가중치 행렬과 편향 행렬의 모든 원소의 수이기 때문입니다.




# 용어 정리

## Optimizer : optim.SGD : SGD, GD

SGD 가 경사하강법을 위한 함수지?
그럼 사진의 연산을 수행하는 함수를 torch.optim  유틸lib에서 제공하는거고

 $a←a−\eta\frac{\partial E}{\partial a}​$
 $b←b−\eta\frac{\partial E}{\partial b}​$

코드로 대응하면:

```python
# 사진 수식 → 코드 대응
# a → W
# b → b
# η (eta, 학습률) → lr=0.01
# ∂E/∂a → W.grad   (backward()가 계산해서 저장해둔 것)
# ∂E/∂b → b.grad

optimizer.step()
# 내부적으로:
# W = W - 0.01 * W.grad    ← a ← a - η * ∂E/∂a
# b = b - 0.01 * b.grad    ← b ← b - η * ∂E/∂b
```

### 역할 분담 정리

```
loss.backward()   →  ∂E/∂a, ∂E/∂b 계산  (사진의 gradient 계산 부분)
optimizer.step()  →  a, b 업데이트       (사진의 ← 화살표 부분)
```

사진의∇E=(∂a/∂E​,∂b/∂E​)가 바로 `[W.grad, b.grad]`예요.
gradient 벡터를 구성하는 두 편미분값이 각 파라미터의 `.grad`에 저장되는 거고, `optim.SGD`는 그걸 꺼내서 η만큼 빼는 그 연산을 수행합니다.

즉 님 이해 완전히 맞아요 — **`torch.optim`이 사진의 업데이트 수식을 구현해둔 유틸 라이브러리**입니다.



### **Stochastic Gradient Descent** — 확률적 경사하강법

> Stochastic = 무작위적인, 확률적인. 
#### 먼저 일반 경사하강법(GD)과 차이부터

사진의 수식에서 오차함수 E가 :
$$E = \frac{1}{n} \sum_{i=1}^{n} \left[ y_i - f(x_i) \right]^2$$

**n = 전체 데이터 수**예요. 
일반 GD는 이걸 **전체 데이터를 다 써서** gradient를 구합니다. 

```
전체 데이터 100만 개
   ↓ 전부 계산
gradient 한 번 구함
   ↓
파라미터 한 번 업데이트
```

문제: 데이터가 많으면 **한 번 업데이트하는 데 너무 오래 걸려요.**

### SGD의 핵심 아이디어

**"전체 안 봐도 돼. 랜덤으로 하나(또는 일부)만 뽑아서 gradient 구하자."**

```
전체 데이터 100만 개
   ↓ 랜덤으로 1개(또는 mini-batch) 뽑음
gradient 한 번 구함
   ↓
파라미터 한 번 업데이트
   ↓
또 랜덤으로 뽑고 반복...
```

**Stochastic = 확률적** — 랜덤 샘플링한다는 뜻이에요.

### 세 가지 변형

```
GD          : 전체 데이터로 gradient → 정확하지만 느림
SGD         : 데이터 1개로 gradient → 빠르지만 noisy
Mini-batch  : 중간(보통 32~256개)  → 둘의 절충안
```

실전에서 `optim.SGD`는 << 사실상 **mini-batch SGD** >> 예요. 데이터를 배치 단위로 잘라서 루프를 돌거든요 (앞서 본 그 배치(B)가 여기서 쓰이는 거예요).

### 왜 noisy한 게 오히려 장점이 되기도 하나

정확한 gradient(GD)만 따라가면 **local minimum(지역 최솟값. 국소 최적해)에 갇힐 수** 있어요. SGD는 노이즈 때문에 **튀어나와서 더 좋은 minimum을 찾기도** 해요.

```
loss 지형:
     ↘  local   global
      \_min  ___min___
            ↑
         GD가 여기 갇힐 수 있음
         SGD는 노이즈로 탈출 가능
```

### 그럼 Adam/AdamW는 뭐가 다른가

SGD의 단점: **학습률(lr)을 하나로 고정**해서 모든 파라미터에 똑같이 적용해요.

Adam은: **파라미터마다 lr을 자동으로 조정**해요. 자주 업데이트되는 파라미터는 lr을 줄이고, 드물게 업데이트되는 건 lr을 키우는 식으로요. 
<< 그래서 대부분의 딥러닝, 특히 LLM에서 SGD 대신 AdamW를 씁니다. >>
=> 실습용이어서 학습률 개념 등도 같이 알려주게 SGD 사용했나봄
직관적으로 더 와닿는 느낌
## Automatic differentiation 이론과 구현 이해

![[Pasted image 20260704170957.png|404]]

**이 그림이 바로 PyTorch autograd/backward의 내부 동작 원리**예요.

그림의 $f(x_1, x_2) = x_1x_2 + \sin x_1$을 PyTorch로 쓰면:

```python
x1 = torch.tensor(2.0, requires_grad=True)
x2 = torch.tensor(3.0, requires_grad=True)

w1 = torch.sin(x1)      # w1 = sin(x1)
w2 = x1 * x2            # w2 = x1*x2
f = w1 + w2             # f = w1 + w2

f.backward()             # 이 한 줄이 그림 오른쪽 전체를 수행

print(x1.grad)           # ∂f/∂x1 = cos(x1) + x2
print(x2.grad)           # ∂f/∂x2 = x1
```

### backward 내부에서 일어나는 일

**순전파(forward) — 그림 왼쪽 위로 가는 화살표:**

```
x1, x2 입력
   ↓
w1 = sin(x1)     w2 = x1*x2      계산하면서
   ↓                  ↓          동시에 "계산 그래프"를
f = w1 + w2                      자동으로 기록해둠
```

PyTorch는 연산할 때마다 **어떤 연산을 했는지 그래프로 기록**해요. 
이게 그림의 트리 구조(sin, *, + 노드)입니다.

**역전파(backward) — 그림 오른쪽 아래로 가는 화살표:**

`f.backward()` 호출하면 그림 오른쪽처럼 
**연쇄법칙(chain rule)을 그래프 위에서 거꾸로** 적용해요:

```
∂f/∂f = 1  (출발점)

   ↓ chain rule
   
∂f/∂w1 = 1,  ∂f/∂w2 = 1   (+ 노드)

   ↓ chain rule
   
∂f/∂x1 = ∂f/∂w1 * ∂w1/∂x1 + ∂f/∂w2 * ∂w2/∂x1
        = 1 * cos(x1)      + 1 * x2
        = cos(x1) + x2              ← x1.grad에 저장
        
∂f/∂x2 = ∂f/∂w2 * ∂w2/∂x2
        = 1 * x1 = x1               ← x2.grad에 저장
```

### 핵심 — "기본 연산의 도함수를 미리 알고 있다"

그림 설명에서 **"각 기본 연산의 도함수를 사용하여 연쇄법칙으로 계산"**이라고 한 게 바로 이거예요.

PyTorch는 sin, cos, +, *, exp 같은 기본 연산(primitive)의 **도함수를 미리 다 구현해뒀어요.** 그래서:

```
sin 노드 → 도함수가 cos인 거 알고 있음
*   노드 → 도함수가 상대방 값인 거 알고 있음
+   노드 → 도함수가 1인 거 알고 있음
```

아무리 복잡한 신경망도 결국 이 기본 연산들의 조합이라, **전체를 자동으로 미분할 수 있는 거예요.** Transformer의 수천만 개 파라미터도, backward() 한 줄로 전부 gradient가 계산되는 이유가 바로 이 메커니즘입니다.


### 컴퓨터가 어떻게 "연쇄법칙이 필요하다"를 아는가

**순전파 때 그래프를 자동으로 기록**하는 게 핵심이에요.

![[Pasted image 20260704170957.png|437]]

```python
x1 = torch.tensor(2.0, requires_grad=True)
x2 = torch.tensor(3.0, requires_grad=True)

w1 = torch.sin(x1)    # 이 시점에 엣지 기록: x1 → w1, 연산=sin
w2 = x1 * x2          # 이 시점에 엣지 기록: x1 → w2, 연산=mul
                      #                    x2 → w2, 연산=mul
f  = w1 + w2          # 이 시점에 엣지 기록: w1 → f, 연산=add
                      #                    w2 → f, 연산=add
```

연산할 때마다 PyTorch가 **"누가 누구를 만들었고, 어떤 연산이었나"**를
노드에 저장해요. 그러면 이런 그래프가 메모리에 쌓여요:

```
f.grad_fn    = AddBackward     (f는 덧셈으로 만들어짐)
w1.grad_fn   = SinBackward     (w1은 sin으로 만들어짐)
w2.grad_fn   = MulBackward     (w2는 곱셈으로 만들어짐)
x1.grad_fn   = None            (x1은 leaf, 시작점)
x2.grad_fn   = None            (x2는 leaf, 시작점)
```

### backward()가 하는 그래프 탐색

`f.backward()` 호출하면 **BFS/DFS로 그래프를 거꾸로 탐색**합니다.

```
1단계: f 노드에서 시작
   grad = 1.0 (출발점)
   grad_fn = AddBackward
   → "add의 도함수는 1이니까, 양쪽에 그대로 전달"
   → w1에 1.0 전달, w2에 1.0 전달

2단계: w1 노드
   grad = 1.0 (위에서 받음)
   grad_fn = SinBackward
   → "sin의 도함수는 cos이니까"
   → x1에 1.0 * cos(x1) 전달

3단계: w2 노드
   grad = 1.0 (위에서 받음)
   grad_fn = MulBackward
   → "mul의 도함수: x1에 대해선 x2, x2에 대해선 x1"
   → x1에 1.0 * x2 전달
   → x2에 1.0 * x1 전달

4단계: x1 노드 (leaf)
   w1에서 받은 것: cos(x1)
   w2에서 받은 것: x2
   → 둘을 더함: x1.grad = cos(x1) + x2  ← 완성!

   x2 노드 (leaf)
   w2에서 받은 것: x1
   → x2.grad = x1  ← 완성!
```

### 핵심: 각 노드가 "자기 도함수"만 알면 된다

컴퓨터는 **전체 수식을 보지 않아요.** 각 연산 노드가:

```
AddBackward  : "내 도함수는 1, 그냥 그대로 전달"
MulBackward  : "내 도함수는 상대방 값, 그걸 곱해서 전달"
SinBackward  : "내 도함수는 cos, 그걸 곱해서 전달"
```

이것만 알고 있어요. 그리고 **위에서 내려온 gradient × 자기 도함수**를 계산해서 아래로 넘기는 것만 반복합니다.

사람이 "f=w에 관한 식, w=x에 관한 식이니 연쇄법칙!"이라고 **전체를 보고 판단**하는 것을, 컴퓨터는 **"위에서 받은 값 × 내 도함수 → 아래로 전달"이라는 단순한 규칙을 그래프 위에서 반복**해서 같은 결과를 냅니다.

### 정리

```
사람: 수식 전체를 보고 → "연쇄법칙 필요" 판단 → 한 번에 계산

컴퓨터:
순전파: 연산할 때마다 그래프 엣지 기록
역전파: 그래프를 거꾸로 BFS/DFS 탐색
        각 노드에서 (위에서 받은 grad × 자기 도함수) → 아래로 전달
        leaf 노드(x)에 도달하면 .grad에 저장
```

**연쇄법칙이 "경로를 따라 곱해서 더한다"는 것**, 그리고 그래프 탐색이 **"경로를 따라 노드를 거치면서 곱한다"는 것** — 이 둘이 구조적으로 완전히 같아서, 그래프 탐색으로 연쇄법칙을 구현할 수 있는 거예요.



### 1. 그래프는 어디에 기록되는가 — RAM에 Python 객체로

**단순히 RAM에 Python 객체로 저장돼요.** 추상화를 거치긴 하지만, 
그 추상화의 결과물이 결국 RAM에 올라가는 거예요.

구체적으로는 각 텐서가 **`grad_fn`이라는 Python 속성**을 갖고, 
거기에 C++ 객체의 포인터가 달립니다:

```python
w1 = torch.sin(x1)

# 이 시점에 w1이라는 Python 객체 안에:
w1.data          # 실제 값 (VRAM 또는 RAM에 있는 텐서)
w1.grad_fn       # SinBackward0 객체 (C++ 객체, Python에서 참조)
w1.grad_fn.next_functions  
			# [(x1의 AccumulateGrad, 0)]  ← 부모 노드 포인터
```

**연결 리스트처럼 노드들이 포인터로 이어진** 구조예요:

```
f.grad_fn(AddBackward)
   └── w1.grad_fn(SinBackward)
         └── x1의 AccumulateGrad
   └── w2.grad_fn(MulBackward)
         ├── x1의 AccumulateGrad
         └── x2의 AccumulateGrad
```

이게 **동적 계산 그래프(Dynamic Computation Graph)**예요. 
코드가 실행되는 순간 그때그때 노드가 생성되고 포인터로 이어지는 방식이라, 
매 순전파마다 새로 만들어집니다.

### 2. 옵티마이저 지정과 관계 있나 — 없어요

그래프 기록은 **옵티마이저와 무관**해요. 조건은 딱 하나:

```python
x = torch.tensor(2.0, requires_grad=True)   # ← 이것만 있으면
```

<< `requires_grad=True`인 텐서가 연산에 끼는 순간, **자동으로 그래프를 기록** >>합니다. 옵티마이저는 나중에 `.grad`를 꺼내 쓰는 역할만 해요.

```python
# requires_grad=True → 그래프 기록 O
x = torch.tensor(2.0, requires_grad=True)
y = x * 3   # grad_fn 생성됨

# requires_grad=False (기본값) → 그래프 기록 X
x = torch.tensor(2.0)
y = x * 3   # grad_fn = None
```

`no_grad()`는 이걸 강제로 끄는 스위치:

```python
with torch.no_grad():
    y = x * 3   # requires_grad=True여도 그래프 기록 안 함
```

### 3. AST/파싱 트리랑 비슷한가

**구조적으로 비슷하지만, 만들어지는 시점이 달라요.** 이게 핵심 차이입니다.

|          | AST                  | PyTorch 계산 그래프 |
| -------- | -------------------- | -------------- |
| 만들어지는 시점 | **실행 전** (컴파일/파싱 단계) | **실행 중** (런타임) |
| 만드는 주체   | 컴파일러/파서              | 연산자 오버로딩       |
| 내용       | 코드 구조                | 실제 연산 + 중간값    |
| 고정 여부    | 코드 바뀌면 재생성           | 매 순전파마다 새로 생성  |
|          |                      |                |

AST는 `x * 3`이라는 **코드 구조**를 표현하고, 
PyTorch 그래프는 `x=2.0 일 때 x * 3을 실행한 결과와 경로`를 표현해요.

PyTorch가 **"Define by Run"** 방식이라고 불리는 이유가 이거예요 — 코드를 미리 분석하는 게 아니라, **실행하면서 그때그때 그래프를 동적으로 만듭니다.** (반대가 TensorFlow 1.x의 "Define and Run" — 실행 전에 그래프를 먼저 선언)

### 4. Python 인터프리터가 어떻게 관리하는가

이게 가장 흥미로운 부분이에요. Python은 **연산자 오버로딩**으로 이걸 구현해요.

Python에서 `a * b`는 내부적으로 `a.__mul__(b)`를 호출해요. 
<< PyTorch 텐서는 이 `__mul__`을 **오버로딩해서** >> 
계산 + 그래프 기록을 같이 합니다:

```python
# 우리가 치는 코드
w2 = x1 * x2

# Python 인터프리터가 실행하는 것
w2 = x1.__mul__(x2)
# 이 안에서 (C++로 구현됨):
# 1. 실제 곱셈 계산 → w2.data에 저장
# 2. MulBackward 객체 생성
# 3. w2.grad_fn = MulBackward
# 4. MulBackward 안에 x1.data, x2.data 캐싱 (역전파 때 필요)
# 5. MulBackward.next_functions = [x1.grad_fn, x2.grad_fn]
```

Python 인터프리터 자체가 특별한 걸 하는 게 아니라, 
**텐서 객체의 연산자 오버로딩이 "계산 + 그래프 기록"을 동시에 수행**하는 거예요. 인터프리터 입장에선 그냥 메서드 호출인데, 그 메서드가 부작용으로 그래프를 만들어요.

### 전체 흐름 한 번에

```
Python 코드: w2 = x1 * x2
   ↓
Python 인터프리터: x1.__mul__(x2) 호출
   ↓
PyTorch C++ 엔진(ATen):
   ① 실제 곱셈 → RAM/VRAM에 결과 저장
   ② MulBackward C++ 객체 생성 → RAM에 저장
   ③ x1.data, x2.data를 MulBackward 안에 캐싱
   ④ w2.grad_fn = MulBackward (Python 객체 속성으로)
   ↓
backward() 호출 시:
   grad_fn 포인터 따라가며 BFS/DFS
   각 노드의 캐싱된 값으로 도함수 계산
   leaf 텐서의 .grad에 누적
```

---

정리:

- **기록 위치**: RAM, C++ 객체로 (Python에서 포인터로 참조)
- **기록 조건**: `requires_grad=True`인 텐서가 연산에 끼면 자동. 옵티마이저 무관.
- **AST와 차이**: 실행 전 분석이 아니라 실행 중 동적 생성 (Define by Run)
- **Python의 역할**: 인터프리터가 특별한 게 아니라, **연산자 오버로딩**이 계산+기록을 동시에 수행


## no_grad

실습 코드의 `new_input`은 `requires_grad=False`(기본값)이지만, 
**W와 b는 `requires_grad=True`예요.** 
<< 연산에 `requires_grad=True`인 텐서가 **하나라도** 끼면 >> 그래프가 기록돼요.

```python
W = torch.zeros((3, 1), requires_grad=True)    # True
b = torch.zeros(1, requires_grad=True)         # True

new_input = torch.FloatTensor([[75, 85, 72]])  # False

prediction = new_input.matmul(W) + b
# new_input은 False지만
# W, b가 True → prediction.grad_fn 생성됨 → 그래프 기록됨!
```

그래서 `no_grad()`로 끄는 거예요. 
**"어느 하나라도 requires_grad=True면 전파된다"**가 규칙입니다.

### FloatTensor 자체의 오버헤드가 있나

**네, 있어요.** `requires_grad`와 무관하게 텐서를 다루는 것만으로 발생하는 오버헤드들이 있어요.

```
텐서 연산 시 항상 발생하는 것들:
① requires_grad 체크 ("이거 기록해야 하나?")
② dtype/device 체크 (float인지, cpu인지 cuda인지)
③ shape 검증 (matmul 가능한지)
④ 메모리 할당 (결과 텐서를 위한 새 메모리)
⑤ Python ↔ C++ 경계 오버헤드 (함수 호출마다 발생)
```

특히 **⑤ Python ↔ C++ 경계**가 생각보다 커요. Python에서 `matmul()`을 부르는 순간, Python 인터프리터가 C++ 엔진으로 넘어갔다가 다시 돌아오는 비용이 발생해요. 연산이 많을수록 이게 쌓입니다.

### no_grad가 아끼는 것들

```python
with torch.no_grad():
    prediction = new_input.matmul(W) + b
```

끄는 것들:

```
① grad_fn 객체 생성 안 함      → 메모리 절약
② 중간값 캐싱 안 함            → 메모리 절약
③ requires_grad 전파 안 함     → 연산 단순화
④ 결과 텐서가 leaf처럼 동작    → 후속 처리 단순
```

안 꺼지는 것들:

```
dtype/shape 체크, 메모리 할당, Python↔C++ 경계 — 이건 항상 발생
```

### 실제 얼마나 차이나나

추론(inference) 상황에서 `no_grad` 유무 차이:

```
그래프 기록 있음: 연산 + grad_fn 생성 + 중간값 저장
그래프 기록 없음: 연산만

메모리: 보통 30~50% 절약
속도:   10~30% 향상 (모델 크기에 따라 다름)
```

대형 LLM 추론에서는 이 차이가 매우 크기 때문에, **추론 때는 항상 no_grad를 쓰는 게 관례**예요.

### 정리

```
no_grad가 필요한 이유:
   new_input은 False지만 W,b가 True
   → 연산에 True가 하나라도 끼면 그래프 기록됨
   → 추론 때는 불필요하니까 no_grad로 차단

FloatTensor 자체 오버헤드:
   그래프 기록과 무관하게
   dtype/shape 체크, 메모리 할당, Python↔C++ 경계가 항상 발생
   no_grad는 이것까지 없애진 않고
   grad_fn 생성 + 중간값 캐싱만 차단
```

"텐서를 다루는 것만으로 오버헤드가 있다"는 직관 정확해요. 
그래서 고성능 추론 엔진(TensorRT, vLLM 등)은 
PyTorch 텐서 오버헤드 자체도 줄이려고 C++ 레벨로 직접 내려가기도 합니다.




## python 문법 : with

Python의 **컨텍스트 매니저(context manager)** 문법이에요.

### 기본 개념

```python
with 어떤것() as 변수:
    # 이 블록 안에서만 특정 상태 유지
    코드...
# 블록 벗어나면 자동으로 원래 상태로 복원
```

**"이 블록 안에서만 이 조건을 적용해라, 나가면 자동으로 해제해라"**
가 핵심이에요.

### no_grad 예시로 보면

```python
with torch.no_grad():
    prediction = new_input.matmul(W) + b
    # 이 안에서만 그래프 기록 OFF

# 여기서 자동으로 그래프 기록 다시 ON
```

`with` 없이 쓰면:


```python
torch.no_grad().__enter__()   # 수동으로 켜고
prediction = new_input.matmul(W) + b
torch.no_grad().__exit__()    # 수동으로 꺼야 함
```

`with`가 이 `__enter__`/`__exit__` 호출을 **자동으로** 해주는 거예요.

### 가장 흔한 사용 패턴들

```python
# 1. 파일 열기 (가장 대표적)
with open("data.csv", "r") as f:
    data = f.read()
# f.close()가 자동으로 호출됨
# with 없으면 파일 닫는 걸 깜빡할 수 있음

# 2. PyTorch no_grad
with torch.no_grad():
    output = model(x)

# 3. GPU 관련
with torch.cuda.amp.autocast():   # 자동 mixed precision
    output = model(x)
```

### 왜 쓰나 — "자원 관리 안전장치"

`with` 없이 쓰면:
```python
f = open("data.csv")
# 중간에 에러 발생하면?
f.close()   # ← 이게 실행 안 됨! 파일이 안 닫힘
```

`with` 쓰면:
```python
with open("data.csv") as f:
    # 에러 나도
    pass
# f.close() 자동 실행 보장
```

**블록을 어떻게 나가든(정상 종료든, 에러든) 항상 `__exit__`가 실행됩니다.** 
<< 자원 누수를 막는 안전장치 >> 예요.

### 내부 구조 — 어떻게 동작하나

`with`와 함께 쓸 수 있으려면 해당 객체가 두 메서드를 구현해야 해요:

```python
class 컨텍스트매니저:
    def __enter__(self):
        # with 블록 진입 시 실행
        # "상태 ON"
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        # with 블록 탈출 시 실행 (에러 나도 실행됨)
        # "상태 OFF"
```

`torch.no_grad()`도 내부적으로 이렇게 구현돼 있어요:

```python
class no_grad:
    def __enter__(self):
        self.prev = torch.is_grad_enabled()
        torch.set_grad_enabled(False)   # 그래프 기록 OFF

    def __exit__(self, *args):
        torch.set_grad_enabled(self.prev)  # 원래 상태로 복원
```

---

정리: `with` = **"블록 진입/탈출 시 자동으로 특정 동작을 실행해주는 문법".** 파일 닫기, 그래프 기록 토글, GPU 설정 변경 등 **"쓰고 나서 반드시 원복해야 하는 것"** 에 쓰는 패턴이에요. OS 수업에서 본 RAII(Resource Acquisition Is Initialization) 패턴이랑 철학이 같습니다.




## 라그랑주 승수법 vs 선형 회귀

**선형회귀의 최적화:**

$min_{W,b } \space\frac{1}{n}\sum(y_i - (Wx_i + b))^2$

**아무 제약 없이** loss를 최소화하는 W, b를 찾는 거예요. 제약조건이 없으니 그냥 gradient = 0으로 풀거나, SGD로 반복해서 찾아요.


**라그랑주 승수법:**

$min f(x) \quad \text{subject to} \quad g(x) = 0$

**제약조건이 있을 때** 최적값을 찾는 방법이에요. 
제약이 있으니 일반 미분으로 못 풀고, 라그랑지안을 구성해서 풀어요:  

$L(x,λ)=f(x)+λg(x)$


### 차이 한 줄

```
선형회귀  : 제약 없는 최적화  (unconstrained)
라그랑주  : 제약 있는 최적화  (constrained)
```

선형회귀는 **라그랑주가 필요 없어요.** 제약조건이 없으니까요.


### 그럼 어디서 연결되나

ML에서 라그랑주가 **직접 쓰이는 곳**은 따로 있어요:

**① SVM (Support Vector Machine)**

```
min  (1/2)||W||²          ← 마진 최대화
subject to  y_i(Wx_i + b) ≥ 1   ← 제약: 모든 점이 마진 밖에 있어야 함
```

이 제약 있는 최적화를 라그랑주 승수법으로 풀어요. 
SVM이 라그랑주의 가장 대표적인 ML 응용이에요.


**② 정규화(Regularization)와 연결**

선형회귀에 **L2 정규화(Ridge)** 를 추가하면:

$\min_{W} \frac{1}{n}\sum(y_i - Wx_i)^2 + \lambda||W||^2$

이게 사실 **"W의 크기를 제한한다"는 
제약조건을 라그랑주로 처리한 것과 수학적으로 동치**예요:

```
제약 있는 버전:
min  loss
subject to  ||W||² ≤ C

↕ (라그랑주 쌍대성으로 변환)

제약 없는 버전:
min  loss + λ||W||²
```

그래서 Ridge 회귀의 λ가 라그랑주 승수 λ와 같은 역할이에요.
**"제약을 penalty 항으로 흡수"**한 거죠.

### 정리

```
선형회귀 자체  : 라그랑주 불필요 (제약 없음)

연결고리:
정규화 선형회귀(Ridge) ↔ 라그랑주로 제약을 penalty로 변환한 것
SVM               ↔ 라그랑주를 직접 사용하는 대표 ML 모델
```

라그랑주는 "제약이 있는 최적화" 전반에 쓰이는 수학 도구고, ML에서는 **SVM, 정규화, 일부 강화학습**에서 나와요. 선형회귀에서 라그랑주를 공부하고 싶으시다면 **Ridge 회귀 → 제약 최적화 형태로 변환**하는 과정을 보시면 연결이 가장 자연스러워요.

## SVM 개념 복습

**Support Vector Machine** — 분류(classification)를 위한 고전 ML 알고리즘이에요.

### 핵심 아이디어

두 클래스를 나누는 경계선(결정 경계)을 그을 때, 
**"경계선과 가장 가까운 점들 사이의 거리(마진)를 최대화"** 하는 
경계선을 찾는 거예요.

```
          ○ ○                
        ○   ○      ← 클래스 A
    ----○-----------  ← 결정 경계
          ●   ●
        ●       ●   ← 클래스 B
          ● ●
```

경계선에 가장 가까운 점들을 **서포트 벡터(Support Vector)**라고 부르고, 이름이 여기서 왔어요.

### 왜 마진 최대화인가

```
경계선 후보 A (마진 좁음):
   ○|●  ← 새 데이터가 조금만 애매해도 오분류

경계선 후보 B (마진 넓음):
   ○  |  ●  ← 새 데이터에 더 여유 있게 대응
```

마진이 넓을수록 **일반화 성능이 좋아요.** 
훈련 데이터에 과적합될 여지가 줄어들거든요.

### 선형 SVM의 수식

```
결정 경계: Wx + b = 0

클래스 A: Wx + b ≥ +1
클래스 B: Wx + b ≤ -1

마진 = 2/||W||
```

마진을 최대화 = ||W|| 를 최소화. 그래서:

$\min \frac{1}{2}||W||^2 \quad \text{subject to} \quad y_i(Wx_i + b) \geq 1$

이게 아까 말한 라그랑주가 쓰이는 그 수식이에요.

### 딥러닝 시대에도 쓰이나

솔직히 **이미지/텍스트/LLM 분야에선 거의 안 써요.** 
딥러닝이 압도적으로 잘하거든요.

하지만 여전히 쓰이는 곳이 있어요:

```
데이터가 적을 때     : 딥러닝은 데이터 많이 필요, SVM은 적어도 잘 됨
표 형태 데이터       : 여전히 경쟁력 있음
해석 가능성 필요할 때 : 의료/금융 등
```

### 위키독스/ML 공부 맥락에서 위치

```
전통 ML (지금 공부 중)
├─ 선형회귀    : 연속값 예측
├─ 로지스틱회귀: 분류 (확률 출력)
├─ SVM        : 분류 (마진 최대화)
├─ 결정트리    : 분류/회귀
└─ 랜덤포레스트: 결정트리 앙상블

         ↓ (데이터 많고 복잡하면)

딥러닝
├─ MLP
├─ CNN
└─ Transformer (지금 CS336에서 보는 것)
```

SVM은 **딥러닝 이전 시대의 대표 분류기**예요. 
지금은 딥러닝에 자리를 많이 내줬지만, ML의 역사와 최적화 이론을 이해하는 데 중요한 모델이라 여전히 교과서에 나옵니다. 라그랑주 승수법이 실제로 적용된 가장 깔끔한 ML 예시이기도 하고요.



## 다중 선형회귀에서 발산하면 왜 NaN가 나올까?

다중 회귀에서 lr을 0.01로 잡고 학습을 시켜보니

![[Pasted image 20260706105234.png|358]]

이렇게 나옴. 
발산하면 얼마나 커지는건지 NaN 가 나오나 싶었는데
학습률을 쪼끔 올렸다고 수의 범위를 초과할정도로 발산했음

그리고 신기하게 거의 정확하게 2배씩 늘어남
=> 이유는 아래와 같음

> 매 Epoch마다 Cost가 2배 이상 폭발적으로 커지는 것은 **오버슈팅(Overshooting)에 의한 '양의 피드백(Positive Feedback)'** 현상입니다.
	- 한 번 잘못된 방향으로 크게 튀면, **그곳에서의 기울기는 더 가팔라집니다.**
	- 더 가팔라진 기울기에 똑같은 학습률(0.01)을 곱하니, **다음 이동 거리는 이전보다 훨씬 더 커집니다.**
	- 이게 반복되니 Cost가 단순히 늘어나는 게 아니라 지수함수적으로(2배, 10배, 100배...) 폭증하는 것입니다.

$W$가 너무 큰 학습률 때문에 부동소수점 오버플로 난거임.
데이터가 적으니까 더 민감하게 반응. 개별 데이터에 대한 영향이 크기 때문에..
데이터가 많은데 lr이 크다? 그래도 이렇게 막 튀지는 않음
데이터가 많고 lr이 작다? 이거는 좀 느리겠지만 정밀하게 조정하고
데이터가 적고 lr이 크면 매우 큰 발산이 되는것.

```
**평탄하지 않은 언덕:** 데이터가 적을 때 오차 공간을 그려보면, 아주 좁고 가파른 계곡 모양이 되기 쉽습니다. 이때 학습률 0.01은 너무나 큰 보폭이라서, 살짝만 발을 내디뎌도 계곡 바닥을 지나쳐 반대편 경사면 꼭대기로 튕겨 올라가 버리는 것입니다.
```


=> 다중 선형회귀에서 학습률(Learning Rate, $lr$)을 0.01로 설정했을 때 `NaN`(Not a Number)이 발생하는 현상은 **"발산(Divergence)"** 때문입니다.

간단히 말해, **학습률이 너무 커서 가중치($W$)가 최적점을 찾지 못하고, 매 단계마다 널뛰기를 하다가 결국 컴퓨터가 감당할 수 없을 만큼 큰 값으로 치솟아 버린 것**입니다.

이 과정을 단계별로 설명해 드리겠습니다.

### 1. 오버슈팅(Overshooting)의 연쇄 작용

1. **과도한 보폭:** 학습률이 높으면 경사하강법이 가야 할 보폭이 너무 커집니다. 최적의 지점을 지나쳐 반대편으로 넘어가 버리는데(Overshooting), 이때 오차(Cost)가 이전보다 오히려 더 커집니다.
    
2. **기울기의 급증:** 다음 단계에서 기울기(Gradient)를 다시 계산할 때, 더 멀어진 지점에서의 기울기는 이전보다 훨씬 더 가파릅니다.
    
3. **악순환의 반복:** 더 큰 기울기에 학습률을 곱하니 보폭은 이전보다 더 커집니다. 이제는 최적점 근처가 아니라 아예 산 정상 저 멀리 밖으로 튕겨 나갑니다.
    
4. **폭발(Explosion):** 가중치와 오차 값이 기하급수적으로 커지다가, 컴퓨터가 표현할 수 있는 실수형 데이터의 최대치를 넘어서는 순간(`inf`)이 오고, 그다음 연산에서 `NaN`이 찍히게 됩니다.
    

### 2. 왜 0.01이 큰 값인가요?

학습률의 적정 범위는 데이터의 스케일(Scale)에 절대적으로 의존합니다.

- **입력값의 크기:** 만약 입력 데이터($X$)의 단위가 1,000, 10,000 단위라면, $W \cdot X$의 결과값은 매우 커지고, 이를 바탕으로 계산되는 오차의 기울기도 매우 커집니다.
    
- **스케일링 부족:** 데이터가 표준화(Standardization, 평균 0, 분산 1로 변환)되어 있지 않다면, 0.01이라는 학습률은 다중 선형회귀에서 굉장히 공격적이고 큰 수치가 됩니다.
    

### 3. 해결을 위한 체크리스트

만약 `NaN`이 계속 나온다면 다음 순서로 조정해 보세요.

1. **데이터 스케일링 (가장 중요):** 입력 데이터를 `StandardScaler` 등을 사용해 **표준화**하십시오. 데이터의 범위를 맞추면 학습률 0.01이 적절한 값이 될 확률이 높습니다.
    
2. **학습률 낮추기:** 0.01에서 0.001, 0.0001로 단계적으로 줄여보며 `NaN`이 발생하는지 확인하세요.
    
3. **초기값 확인:** 가중치 초기값이 너무 크지 않은지 확인하세요. (보통 0에 가까운 작은 무작위 값으로 초기화합니다.)
    
4. **데이터에 NaN/Inf 확인:** 드물지만, 원본 데이터 자체에 이미 결측치(`NaN`)나 무한대(`Inf`) 값이 섞여 있을 수 있습니다. `print(df.isnull().sum())`으로 확인해 보세요.
    

결론적으로, 지금 겪고 계신 현상은 "학습률이 데이터가 가진 기울기의 변화 속도를 따라가지 못해 시스템이 폭발한 것"입니다. 데이터를 먼저 정규화하시면 대부분 해결됩니다.


=> epoch 출력을 정상적인 학습률로 다중선형회귀 과정 출력해보면?

![[Pasted image 20260706105650.png|296]]

이렇게 아주 빠르게 cost가 줄어드는 걸 볼 수 있음.
얘도 신기하게 3배씩 줄어듦 

이유 => 결론부터 말씀드리면, **특정 조건 하에서 비용 함수(Cost Function)가 지수적으로 감소하는 것은 매우 자연스러운 현상**입니다. 
왜 그런지 하나씩 짚어드릴게요.

### 1. 왜 "일정한 비율"로 줄어드나요? (수학적 배경)

선형 회귀에서 사용하는 MSE(Mean Squared Error) 비용 함수는 **이차 함수(Quadratic Form)** 모양의 그릇 형태를 띱니다.

경사하강법을 아주 쉽게 표현하면 다음과 같습니다.

> **"현재 위치의 기울기(Gradient)에 비례해서 다음 위치로 이동한다."**

기울기(Gradient)라는 것은 "현재 위치에서 최적점까지 얼마나 먼가?"를 나타내는 지표이기도 합니다. 즉, **"멀리 있을수록 기울기는 가파르고, 가까이 올수록 기울기는 완만해집니다."**

- 최적점에 가까워질수록 기울기가 줄어드니, 이동하는 보폭도 자연스럽게 줄어듭니다.
    
- 이 과정이 반복되면서, 오차 값(Cost)은 이전 값의 일정 비율만큼만 남기는 형태(예: 매번 1/3씩 남음)가 되는데, 이게 바로 지수적 감소(Exponential Decay)입니다.
    
- 이 비율은 학습률($lr$)과 데이터의 특성(피처 간의 상관관계 등)이 복합적으로 얽혀서 결정되는 고유한 값입니다.
    

### 2. "2배 3배"는 어디서 오는가?

이 숫자는 데이터의 곡률(Curvature)과 학습률(lr)의 합작품입니다.

- **데이터의 분포:** 데이터들이 선형 모델이 표현할 수 있는 최적의 직선과 얼마나 긴밀하게 붙어있는지, 즉 데이터가 얼마나 "깔끔하게 정리되어 있는지"에 따라 기울기가 변하는 속도가 다릅니다.
    
- **학습률:** 학습률이 최적의 보폭이라면 비용 함수를 가장 효율적으로 깎아내리는데, 이때 발생하는 일정한 감소 비율이 3배(또는 1/3)라는 숫자로 나타난 것입니다.
    

### 3. "부의 피드백(Negative Feedback)"이 맞나요?

네, 완벽한 표현입니다!

- **아까의 상황(발산):** 오차가 커지면 기울기도 커지고, 그게 다시 오차를 더 키우는 양의 피드백(Positive Feedback)의 굴레였습니다.
    
- **지금의 상황(수렴):** 오차가 줄어들면 기울기도 작아지고, 그게 보폭을 줄여서 아주 정밀하게 최적점을 찾아가는 **부의 피드백(Negative Feedback)** 루프가 형성된 것입니다. 시스템이 스스로를 안정시키는 아주 건강한 상태죠.
    

### 요약하자면:

지금 보고 계신 그 일정하게 줄어드는 Cost는, **모델이 현재 데이터의 규칙을 아주 효율적으로 파악하며 최적점을 향해 '브레이크를 밟으며' 예쁘게 착륙하고 있는 모습**입니다.

이 숫자가 2배인지 3배인지보다 더 중요한 것은, "어떤 속도로 줄어드는가"입니다.

- 너무 빨리 줄어들면(예: 10배씩), 학습률이 너무 커서 곧 튕겨 나갈 위험이 있다는 신호일 수 있습니다.
    
- 이렇게 차분하게 줄어드는 모습이 학습이 아주 안정적으로 잘 되고 있다는 최고의 증거입니다.


## 이해 겸 재미를 위한 가중치 조정 실습

기존에 lr=1e-5 였던 다중회귀를 lr=1e-6으로 시키면

![[Pasted image 20260706110339.png|397]]
훨씬 천천~히줄어듦
그래봤자 100번에 걸쳐서 4점대까지 줄어들긴 함.
생각보다 엄청 빨리 조정됨. 당연히 수학적으로 줄여가는거니까 그럴만도 하긴 하지..

Epoch 2000/2000 Cost: 0.223902
이게 1e-6 결과

Epoch 2000/2000 Cost: 0.199770
이게 1e-5 결과


엥?
정밀하게 하니까 오히려 전자가 더 나은 cost 를 보여주지 않을까 싶었는데
최적화된게 1e-5여서인지 오히려 후자가 개선된 cost네..
=> 생각해보니 보폭이 너무 작으면 내려가다가 멈추는게 당연한거같다.
epoch 반복 횟수도 중요하고 여러 요소가 모두 중요한듯

그렇다면 2e-5(0.0002=>0.00002)는 어떨까 싶어서 돌려봄.


Epoch 2000/2000 Cost: 0.188540
오... 신기하게 개선됨
왠지 그럴거같긴했음

3을 했을 때도 개선됐음.
Epoch 2000/2000 Cost: 0.183278 (3e-5)

정수로는 4가 최적인 거 같음
Epoch 2000/2000 Cost: 0.180164

5부터가 발산이고 ..

Epoch 5000/5000 Cost: 0.170421 (epoch 2000=>5000)
확실히 epoch 수도 매우 중요한듯
lr만큼은 아니어도 반복 횟수 자체도 많은 영향을 미침

어디까지 줄어드나 궁금해서 100000번을 돌려봤더니
Epoch 100000/100000 Cost: 0.156234
이정도까지 줄어들긴 하는데 비용 대비 효과가 미미하다
어쩔 수 없긴 하지만..

> 결론: epoch 10만 + SGD lr = 4e-5 에서 cost 0.156234 

### 참고로 현대에는 학습률을 어떻게 조정할까?
**Learning Rate Scheduler:** 처음에는 `4e-5`로 크게 움직이다가, 나중에는 `1e-6`처럼 아주 작게 움직이도록 학습률을 시간에 따라 줄여주는 기법입니다. (실제 현업에서 가장 많이 씁니다.)

이런게 있다고 함

 하긴 lr이라는 하이퍼피라미터 하나를 조정하는거로 GPU연산 효율 높일수있으면 무조건 달려들어서 최적화하긴하겠지
 
지금, 현대에서는 lr에대한 최적화공식이 거의 기정사실화된수준으로 공공연하게나와있는게 있겠지??

=> 있음!!
- **Adaptive Learning Rate (적응형 학습률):** 이제는 사람이 학습률을 하나로 고정하지 않습니다.
    - **Adam (Adaptive Moment Estimation):** 현재 가장 많이 쓰이는 알고리즘입니다. **"기울기가 크면 보폭을 줄이고, 기울기가 작아지면 보폭을 다시 조절하는"** 방식으로, 알아서 최적의 보폭을 찾아갑니다. 아까 겪으셨던 '발산' 문제나 '수렴 속도' 문제를 알고리즘 수준에서 해결해 줍니다.
    => 아 위에있던 Lerning Rate scheduler 가 뭐 이상한 다른 기법이 아니라
    모멘텀 쓰는 Adam 최적화함수였음... 
        
- **Learning Rate Scheduler:** * **Cosine Annealing:** 처음에는 학습률을 크게 시작해서 빠르게 내려가다가, 최적점 근처에 오면 학습률을 코사인 그래프처럼 완만하게 줄여서 아주 정밀하게 최적점을 찍게 만듭니다.
    
- **Warm-up:** 처음부터 큰 학습률을 쓰면 발산하기 쉬우니, 아주 작은 값으로 시작해서 0.01 같은 원래 학습률까지 서서히 올려주는 기법입니다.

그래서 코드에 Adam 을 도입해서 돌려봤음.
![[Pasted image 20260706112803.png|344]]
결과가 이렇게 나와서 첨엔 놀랬음..
그래서 10만번 돌려보니까?
![[Pasted image 20260706114447.png|281]]
Adam 이 확실히 더 깊은 곳까지 뚫었음.
SGD는 0.15정도였는데 얘는 0.10까지도 감.
알고 보니까, 얘는 내부에 여러가지 반영점이 많아서 
함수 내부적으로 더 큰 lr을 줘야(SGD에 비해) 더 잘 학습하는 특성이 있음.

=> 아래 내용 참조
### 1. 왜 SGD는 예민하고 Adam은 둔감해도 될까?

- **SGD (단순무식):** 모든 파라미터에 동일한 학습률을 곱합니다. 그래서 가장 경사가 급한 파라미터 하나가 전체 학습을 망치지 않도록, 전체적으로 학습률을 아주 << 보수적으로(작게) 잡아야 합니다. >> 안 그러면 << 한 놈 때문에  >> 모델 전체가 튕겨 나갑니다.
    ![[Pasted image 20260706120247.png]]
- **Adam (맞춤형):** 파라미터마다 제각각인 기울기를 따로따로 관리합니다. 많이 움직이는 놈은 학습률을 줄이고, 정체되어 있는 놈은 학습률을 키워줍니다. 그래서 **"어느 정도 큰 학습률을 줘도, 지가 알아서 파라미터별로 보폭을 줄여서 최적점을 찾아가기 때문에"** 발산할 확률이 훨씬 낮습니다.
- 강의자료의 "가중치별 다른 학습율" 이라는 게 이런 장점 때문이었음.
	하기야 더 정밀하게 찾아보려면 가중치별로 다르게 정밀관리하는게 좋긴 하지
    

### 2. Adam의 '인프라'가 다른 점 (비유하자면)

- **SGD:** "앞만 보고 달려!" (현재 기울기 방향으로만 무작정 이동)
    
- **Adam:** "이 길은 평소에 많이 가본 길(관성)인데, 최근에 보니까 경사가 너무 급하네? 그럼 보폭을 좀 줄이자!" (Momentum + Adaptive Learning Rate)
    

이 '인프라' 차이 덕분에 Adam은 **초반에는 빠른 속도로 훅 들어가고, 후반에는 보폭을 자동으로 줄여서 최적점에 아주 섬세하게 착륙**합니다. 아까 SGD가 0.15에서 깔짝대던 구간을 Adam은 자연스럽게 '감속'하면서 통과해버린 것이죠.

### 3. Adam 사용할 때의 마음가짐

Adam을 쓰실 때는 **`lr=1e-3` (0.001)** 정도로 시작해 보세요. 만약 학습이 잘 안 되면 `3e-4` 정도로 내리고, 그래도 안 되면 `3e-3`으로 올려보는 식으로 log 스케일(10배 단위)로 조정하는 게 국룰입니다. (0.00004처럼 좁은 범위에서 끙끙대지 않아도 됩니다.)



그래서 lr=1e-3 을 주고 돌려보면?
![[Pasted image 20260706114639.png|293]]

![[Pasted image 20260706114734.png|211]]

흠... 아직 미묘한 부분이 있지만 꽤 잘 줄어든다.
하지만 좀 아쉽긴 함

1e-2 를 주면
Epoch 2000/2000 Cost: 0.180833
요래 나오고,

4e-2를 주면
Epoch 2000/2000 Cost: 0.149090
요래 나오고 

어? 얘는 0.1까지 버틴다
9e-2 줘도 Epoch 2000/2000 Cost: 0.129114
2e-1 줘도 Epoch 2000/2000 Cost: 0.129114

심지어 학습률을 1 이상으로 줘도 발산 안함
하긴 자기가 학습률 조정하는거니까 그럴만도 하나
정확한 이유는 :


정확합니다! 지금 경험하신 그 현상이 **Adam이 왜 현대 딥러닝의 '표준 옵티마이저'가 되었는지**를 보여주는 핵심 증거입니다.

사용자님께서 "지가 알아서 lr 조정해서 그런 건가?"라고 하셨는데, 정확히 맞습니다. Adam의 **'적응형 학습률(Adaptive Learning Rate)'** 기능이 초능력을 발휘하고 있는 것이죠.

### 1. 왜 `2e-1` (0.2) 심지어 `1.0`을 줘도 발산하지 않을까?

SGD는 `lr=4e-5`만 넘어가도 우주 밖으로 튕겨 나갔는데, Adam은 왜 이렇게 튼튼할까요?

- **개별 파라미터별 자동 감속:** Adam은 각 파라미터(Weight)마다 '최근에 얼마나 많이 움직였는지(이동 평균)'를 추적합니다.
    
    - 만약 학습률을 `0.2`처럼 아주 크게 주면, 처음엔 엄청나게 큰 보폭으로 움직이려고 할 겁니다.
        
    - 하지만 Adam 내부에서는 "어? 이거 너무 많이 움직이는데?"라고 판단하고, 그 파라미터의 학습률을 **알아서 매우 작게 깎아버립니다.**
        
- **수학적 보호막:** Adam은 기울기(Gradient)의 제곱값을 이용해 학습률을 나누어줍니다 ($lr / \sqrt{v_t}$). 그래서 기울기가 튈 때마다 분모가 커져서 학습률이 자동으로 억제됩니다. 사실상 '자동으로 조절되는 브레이크'를 내장하고 있는 셈이죠.
    

### 2. 비용(Cost)이 정체되는 이유 (0.129114 부근)

`9e-2`나 `2e-1`에서 동일하게 `0.129114`가 나오는 건, 모델이 이미 **'더 이상 내려갈 곳이 없는 최저점(Global Minimum)'** 근처에 도달했기 때문입니다.

- **Adam의 한계:** 학습률을 너무 크게 주면(예: 0.2), 아무리 Adam이 브레이크를 걸어준다고 해도, 최적점 근처에서 아주 미세하게 덜덜 떨리며 진동하게 됩니다.
    
- **SGD와의 차이:** SGD는 이 상태에서 발산(폭발)해버리지만, Adam은 **'덜덜 떨면서도 최적점을 벗어나지 않고 그 자리에 머무는'** 안정성을 보여줍니다. 그래서 Cost가 더 줄어들지는 않아도 `inf`나 `nan`이 나오지 않는 것이죠.
    

### 3. 현대 딥러닝의 팁: 왜 그럼 다들 `1e-3`을 쓰나?

`2e-1`을 줘도 발산 안 하고 잘 학습되는 걸 보니 "와, 그냥 앞으로 다 크게 줘야지!"라고 생각하실 수 있습니다. 하지만 여기엔 함정이 있습니다.

- **최적점 정밀도:** 학습률이 너무 크면, 아무리 자동 조절이 되어도 최적점의 '정중앙'에 안착하기 어렵습니다. 계속 근처를 크게 크게 돌게 되죠.
    

그래서 고수들은 "발산하지 않는 가장 큰 학습률"을 찾는 게 아니라, "가장 정교하게 학습되는 학습률(`1e-3` ~ `3e-4` 수준)"을 먼저 사용하고, 그 뒤에 학습률 스케줄러(Learning Rate Scheduler)를 붙여서 "처음엔 크게, 끝날 땐 작게" 조절하는 전략을 씁니다.





##