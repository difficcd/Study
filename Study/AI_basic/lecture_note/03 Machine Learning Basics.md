
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
# 여기부터 이어서 진행

앞서 단순 선형 회귀 모델은 다음과 같이 구현했었습니다.

```python
# 모델을 선언 및 초기화. 단순 선형 회귀이므로 input_dim=1, output_dim=1.
model = nn.Linear(1,1)
```

이를 클래스로 구현하면 다음과 같습니다.

```python
class LinearRegressionModel(nn.Module): # torch.nn.Module을 상속받는 파이썬 클래스
    def __init__(self): #
        super().__init__()
        self.linear = nn.Linear(1, 1) # 단순 선형 회귀이므로 input_dim=1, output_dim=1.

    def forward(self, x):
        return self.linear(x)
```

```python
model = LinearRegressionModel()
```

위와 같은 클래스를 사용한 모델 구현 형식은 대부분의 파이토치 구현체에서 사용하고 있는 방식으로 반드시 숙지할 필요가 있습니다.

클래스(class) 형태의 모델은 nn.Module 을 상속받습니다. 그리고 __init__()에서 모델의 구조와 동작을 정의하는 생성자를 정의합니다. 이는 파이썬에서 객체가 갖는 속성값을 초기화하는 역할로, 객체가 생성될 때 자동으로 호출됩니다. super() 함수를 부르면 여기서 만든 클래스는 nn.Module 클래스의 속성들을 가지고 초기화 됩니다. foward() 함수는 모델이 학습데이터를 입력받아서 forward 연산을 진행시키는 함수입니다. 이 forward() 함수는 model 객체를 데이터와 함께 호출하면 자동으로 실행이됩니다. 예를 들어 model이란 이름의 객체를 생성 후, model(입력 데이터)와 같은 형식으로 객체를 호출하면 자동으로 forward 연산이 수행됩니다.

-  식에 입력 로부터 예측된 를 얻는 것을 forward 연산이라고 합니다.

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
        self.linear = nn.Linear(3, 1) # 다중 선형 회귀이므로 input_dim=3, output_dim=1.

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

```ini
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

모델이 주어진 데이터에 대해 2000번 반복하여 학습합니다. 먼저, 모델은 입력 데이터인 x_train을 사용해 예측 값을 계산합니다. 이 예측 값과 실제 값인 y_train 간의 차이를 평균 제곱 오차 함수 F.mse_loss를 사용해 계산합니다. 이 차이를 비용이라고 부르며, 비용이 클수록 모델의 예측이 실제 값과 많이 다르다는 것을 의미합니다.

모델이 이 비용을 줄이도록 학습하기 위해, 먼저 옵티마이저의 기울기를 초기화합니다. 그런 다음, 비용 함수를 미분하여 각 파라미터에 대한 기울기를 계산합니다. 이 기울기를 사용해 옵티마이저는 모델의 파라미터를 업데이트하여 비용을 줄이는 방향으로 모델을 개선합니다.

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
        self.linear = nn.Linear(3, 1) # 다중 선형 회귀이므로 input_dim=3, output_dim=1.

    def forward(self, x):
        return self.linear(x)

model = MultivariateLinearRegressionModel()
```

MultivariateLinearRegressionModel이라는 클래스는 PyTorch의 nn.Module을 상속받아 정의됩니다. 이 클래스는 다중 입력 데이터를 받아 단일 출력을 예측하는 선형 회귀 모델을 구현합니다. 클래스 초기화 메서드에서 super()를 호출하여 부모 클래스인 nn.Module의 초기화를 수행한 뒤, nn.Linear 객체를 생성하여 모델의 선형층을 설정합니다. 이 계층은 입력 차원이 3, 출력 차원이 1로 설정되어 있으며, 이는 3개의 독립 변수를 사용하는 다중 선형 회귀 모델임을 의미합니다. 이제 학습률과 옵티마이저를 설정합니다.

```python
optimizer = torch.optim.SGD(model.parameters(), lr=1e-5) 
```

학습 파라미터를 업데이트하기 위해 확률적 경사 하강법(SGD) 옵티마이저를 설정합니다. 옵티마이저는 model.parameters()를 통해 모델의 모든 학습 가능한 파라미터를 가져와 이를 학습에 사용하며, 학습률 lr은 모델이 학습할 때 파라미터를 얼마나 빠르게 또는 느리게 업데이트할지를 결정합니다. 이제 2,000 에포크동안 for 루프를 사용하여 학습을 진행해보겠습니다.

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

학습은 모델이 주어진 입력 데이터 x_train을 사용하여 예측값을 계산하는 것으로 시작됩니다. 이 예측값을 모델의 출력이라고도 하며, model(x_train)을 호출하여 계산됩니다. 사실상, 이는 모델의 forward 메서드를 호출하는 것과 동일한 동작을 합니다.

예측값이 계산된 후, 이 값과 실제 목표값 y_train 간의 차이를 계산하는데, 이 차이를 손실 또는 비용이라고 부릅니다. 여기서는 파이토치의 F.mse_loss 함수를 사용하여 평균 제곱 오차를 계산합니다. 이 비용은 모델이 얼마나 잘못 예측했는지를 나타냅니다.

모델이 이 비용을 줄이도록 학습하기 위해, 먼저 옵티마이저의 기울기 값을 초기화합니다. 그런 다음, 비용 함수를 기준으로 각 파라미터에 대한 기울기를 계산하여 모델의 파라미터를 업데이트합니다. 이 과정은 비용 함수를 모델 파라미터에 대해 미분한 후, 옵티마이저가 이를 사용해 파라미터를 조정하는 방식으로 이루어집니다.

이 과정은 설정한 횟수만큼 반복되며, 모델은 점차적으로 더 정확한 예측을 하도록 학습됩니다. 학습이 100번 진행될 때마다 현재의 학습 단계와 비용을 출력하여 학습의 진행 상황을 모니터링합니다. 이 출력은 모델의 성능이 시간이 지남에 따라 어떻게 변화하고 있는지를 확인하는 데 중요한 역할을 합니다.



# 03-04 미니 배치, 데이터 로더 

# 03-05 벡터/행렬 연산 Review 


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



##