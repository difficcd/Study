
> 이번 챕터에서는 이진 분류를 수행하는 로지스틱 회귀에 대해서 알아보겠습니다.

# 04-01 로지스틱 회귀(Logistic Regression)

일상 속 풀고자하는 많은 문제 중에서는 두 개의 선택지 중에서 정답을 고르는 문제가 많습니다.
예를 들어 시험을 봤는데 이 시험 점수가 합격인지 불합격인지가 궁금할 수도 있고, 어떤 메일을 받았을 때 이게 정상 메일인지 스팸 메일인지를 분류하는 문제도 그렇습니다. 

이렇게 둘 중 하나를 결정하는 문제를 **이진 분류(Binary Classification)** 라고 합니다. 그리고 이진 분류를 풀기 위한 대표적인 알고리즘으로 로지스틱 회귀(Logistic Regression)가 있습니다.

- **로지스틱 회귀는 알고리즘의 이름은 회귀이지만 실제로는 분류(Classification) 작업에 사용할 수 있습니다. => sigmoid 함수(logistic 함수)**

## 1. 이진 분류(Binary Classification)

학생들이 시험 성적에 따라서 합격, 불합격이 기재된 데이터가 있다고 가정해봅시다. 시험 성적이 $x$라면, 합불 결과는 $y$입니다. 이 시험의 커트라인은 공개되지 않았는데 이 데이터로부터 특정 점수를 얻었을 때의 합격, 불합격 여부를 판정하는 모델을 만들고자 합시다.

|score()|result()|
|---|---|
|45|불합격|
|50|불합격|
|55|불합격|
|60|합격|
|65|합격|
|70|합격|

위의 데이터에서 합격을 1, 불합격을 0이라고 하였을 때 그래프를 그려보면 아래와 같습니다.

![](https://static.wikidocs.net/images/page/22881/%EB%A1%9C%EC%A7%80%EC%8A%A4%ED%8B%B1%ED%9A%8C%EA%B7%80.PNG)

이러한 점들을 표현하는 그래프는 알파벳의 S자 형태로 표현됩니다. 이러한 $x$와 $y$의 관계를 표현하기 위해서는 $Wx+b$ 와 같은 직선 함수가 아니라 S자 형태로 표현할 수 있는 함수가 필요합니다. << 이런 문제에 직선을 사용할 경우 분류 작업이 잘 동작하지 않습니다. >>

그래서 이번 로지스틱 회귀의 가설은 선형 회귀 때의 $H(x)=Wx+b$가 아니라, 
위와 같이 S자 모양의 그래프를 만들 수 있는 어떤 특정 함수 $f$ 를 추가적으로 사용하여 $H(x)=f(Wx+b)$ 의 가설을 사용할 겁니다. 그리고 위와 같이 S자 모양의 그래프를 그릴 수 있는 어떤 함수 가 이미 널리 알려져있습니다. 
바로 시그모이드 함수입니다.

## 2. 시그모이드 함수(Sigmoid function)

위와 같이 S자 형태로 그래프를 그려주는 
시그모이드 함수의 방정식은 아래와 같습니다.

$$H(x) = \text{sigmoid}(Wx + b) = \frac{1}{1 + e^{-(Wx+b)}} = \sigma(Wx + b)$$

![[Pasted image 20260708124241.png|394]]

선형 회귀에서는 최적의 $W$와 $b$를 찾는 것이 목표였습니다. 여기서도 마찬가지입니다. 선형 회귀에서는 $W$가 직선의 기울기, $b$가 y절편을 의미했습니다. 
그렇다면 여기에서는 << $W$와 $b$가 함수의 그래프에 어떤 영향을 주는지 직접 그래프를 그려서 알아보겠습니다. >>

- **파이썬에서는 그래프를 그릴 수 있는 도구로서 Matplotlib을 사용할 수 있습니다.**

우선 Matplotlib과 Numpy를 임포트합니다.

```python
%matplotlib inline
import numpy as np # 넘파이 사용
import matplotlib.pyplot as plt # 맷플롯립 사용
```

Numpy를 사용하여 시그모이드 함수를 정의합니다.

```python
def sigmoid(x): # 시그모이드 함수 정의
    return 1/(1+np.exp(-x))
```

### 1) W가 1이고 b가 0인 그래프

가장 먼저 $W$가 1이고, $b$가 0인 그래프를 그려봅시다.

```python
x = np.arange(-5.0, 5.0, 0.1)
y = sigmoid(x)

plt.plot(x, y, 'g')
plt.plot([0,0],[1.0,0.0], ':') # 가운데 점선 추가
plt.title('Sigmoid Function')
plt.show()
```

![](https://static.wikidocs.net/images/page/22881/%EC%8B%9C%EA%B7%B8%EB%AA%A8%EC%9D%B4%EB%93%9C%EA%B7%B8%EB%9E%98%ED%94%84.png)

위의 그래프를 통해 시그모이드 함수는 출력값을 0과 1사이의 값으로 조정하여 반환함을 알 수 있습니다. $b$가 0일 때 0.5의 값을 가집니다. $b$가 매우 커지면 1에 수렴합니다. 반면, 가 매우 작아지면 0에 수렴합니다.




### 2) W값의 변화에 따른 경사도의 변화

이제 의 값을 변화시키고 이에 따른 그래프를 확인해보겠습니다.

```python
x = np.arange(-5.0, 5.0, 0.1)
y1 = sigmoid(0.5*x)
y2 = sigmoid(x)
y3 = sigmoid(2*x)

plt.plot(x, y1, 'r', linestyle='--') # W의 값이 0.5일때
plt.plot(x, y2, 'g') # W의 값이 1일때
plt.plot(x, y3, 'b', linestyle='--') # W의 값이 2일때
plt.plot([0,0],[1.0,0.0], ':') # 가운데 점선 추가
plt.title('Sigmoid Function')
plt.show()
```

![](https://static.wikidocs.net/images/page/22881/%EC%8B%9C%EA%B7%B8%EB%AA%A8%EC%9D%B4%EB%93%9C%ED%95%A8%EC%88%98%EC%9D%98%EA%B8%B0%EC%9A%B8%EA%B8%B0%EC%9D%98%EB%B3%80%ED%99%94.png)

위의 그래프는 의 값이 0.5일때 빨간색선, 의 값이 1일때는 초록색선, 의 값이 2일때 파란색선이 나오도록 하였습니다. 자세히 보면 의 값에 따라 그래프의 경사도가 변하는 것을 볼 수 있습니다. 앞서 선형 회귀에서 가중치 는 직선의 기울기를 의미했지만, 여기서는 그래프의 경사도를 결정합니다. $W$ 의 값이 커지면 경사가 커지고 $W$ 의 값이 작아지면 경사가 작아집니다.

### 3) b값의 변화에 따른 좌, 우 이동

이제 의 값에 따라서 그래프가 어떻게 변하는지 확인해보겠습니다.

```python
x = np.arange(-5.0, 5.0, 0.1)
y1 = sigmoid(x+0.5)
y2 = sigmoid(x+1)
y3 = sigmoid(x+1.5)

plt.plot(x, y1, 'r', linestyle='--') # x + 0.5
plt.plot(x, y2, 'g') # x + 1
plt.plot(x, y3, 'b', linestyle='--') # x + 1.5
plt.plot([0,0],[1.0,0.0], ':') # 가운데 점선 추가
plt.title('Sigmoid Function')
plt.show()
```

![](https://static.wikidocs.net/images/page/22881/b%EC%9D%98%EC%9D%B4%EB%8F%99.png)

위의 그래프는 의 값에 따라서 그래프가 << 좌, 우 >>로 이동하는 것을 보여줍니다.

### 4) 시그모이드 함수를 이용한 분류

시그모이드 함수는 입력값이 한없이 커지면 1에 수렴하고, 입력값이 한없이 작아지면 0에 수렴합니다. **시그모이드 함수의 출력값은 0과 1 사이의 값을 가지는데** 이 특성을 이용하여 분류 작업에 사용할 수 있습니다. 

예를 들어 임계값을 0.5라고 정해보겠습니다. 출력값이 0.5 이상이면 1(True), 0.5이하면 0(False)으로 판단하도록 할 수 있습니다. 이를 << 확률이라고 생각 >> 하면 해당 레이블에 속할 확률이 50%가 넘으면 해당 레이블로 판단하고, 해당 레이블에 속할 확률이 50%보다 낮으면 아니라고 판단하는 것으로 볼 수 있습니다.

![[Pasted image 20260708131726.png|493]]

### 시그모이드 함수는 어떤 역할을 하는가 (심화 이해)

> 로지스틱 함수 (시그모이드함수) 적용하는 로지스틱 회귀에서는, 시그모이드 적용함으로써 0~1사이로 출력을 좁혀 특정기준으로 분류할수있음 뿐만이 아니라 판단의 예측 기준을 좀더 누그러뜨림(중간 부분을 모 아니면 도가 되도록 좀 더 일그러지게함)으로써 정확도/정밀도에도 기여하는건가? 아니면 전자만 하는건가?

좋은 질문이야. 결론부터 말하면 — **너의 직관이 맞아. 단순히 [0,1]로 눌러 담는 것(bounding) 이상의 역할을 해.** 근데 정확히 "왜" 그런지는 설명이 좀 더 필요해.

#### 우선 그림에서 뭐가 문제였는지부터

왼쪽 선형회귀 그림을 보면, 오른쪽 위쪽에 몰려있는 y=1 데이터들 때문에 직선의 기울기가 그쪽으로 끌려가 있어. 이게 선형회귀를 분류기로 쓸 때 생기는 대표적인 문제야:

**선형회귀(MSE, 최소제곱법)는 "이미 확실하게 맞춘 점"도 계속 손실(loss)에 기여해.** 예를 들어 x가 아주 커서 이미 확실히 클래스 1인 점이 있어도, 예측값이 1을 넘어서든 말든 $(y_{pred} - y_{true})^2$ 이 0이 아니면 계속 페널티를 먹어. 그 결과 이 "이미 확실한" 점들이 직선 전체의 기울기와 절편을 계속 흔들어버려서, 정작 중요한 **경계 근처(애매한 영역)의 정확도가 <<왜곡>> => 정밀하지 못함.**돼.

#### 시그모이드가 하는 두 가지 역할

**역할 1 (너가 말한 "전자"): [0,1]로 압축** — 확률로 해석 가능하게 만들고 임계값(threshold)으로 분류 가능하게 함. 이건 맞아.

**역할 2 (너가 말한 "일그러짐"): 양 끝에서 포화(saturate)됨** — 이게 핵심이야. 시그모이드의 미분은:

$\sigma'(z) = \sigma(z)(1-\sigma(z))$

$z$가 아주 크거나 아주 작으면 $σ′(z)→0$. 즉 **"이미 확실하게 맞춘 점"은 그래디언트가 거의 0이 돼서 더 이상 파라미터 업데이트에 영향을 못 줘.** 반대로 $z≈0z$ (경계 근처, 애매한 영역)일 때 $σ′(z)$가 최대(0.25)가 돼서, **모델의 학습 에너지가 자연스럽게 "헷갈리는 경계 영역"에 집중**되게 돼.

이게 바로 너가 말한 "중간 부분을 모 아니면 도가 되도록 일그러지게 함"의 정확한 메커니즘이야 — 양 끝은 flat(포화)해지고 중간만 steep해지는 S자 모양 자체가, 학습 과정에서 **<<극단값에 안 흔들리고 경계 근처 데이터에 민감>>하게 반응하도록 만드는 구조적 장치**인 거지.

#### 근데 한 가지 짚을 게 있어: 손실 함수도 같이 바뀐다

여기서 중요한 포인트 하나 — 로지스틱 회귀는 시그모이드만 씌우는 게 아니라 **손실 함수 자체도 MSE에서 cross-entropy(log-loss)로 바뀌어**:

$$L = -[y\log\hat{y} + (1-y)\log(1-\hat{y})]$$

이건 우연이 아니라 $y \sim \text{Bernoulli}(\hat{y})$로 보고 **가능도(likelihood)를 최대화(MLE)** 하다 보면 자연스럽게 유도되는 손실함수야.
시그모이드 + cross-entropy를 짝지으면 미분이 놀랍도록 깔끔해져:

$\frac{\partial L}{\partial z} = \hat{y} - y$

포화되는 시그모이드 곡선의 모양 + 그에 맞는 손실함수(cross-entropy)가 **함께** 작동해서 네가 말한 정확도/견고성(robustness) 향상 효과가 나오는 거야. 시그모이드 모양 하나만으로 되는 게 아니라, 그 모양에 맞는 손실함수를 같이 써야 시너지가 나는 구조.

#### 정리

|           | 단순 [0,1] 압축만? | 실제로는?                       |
| --------- | ------------- | --------------------------- |
| 극단값 처리    | 여전히 큰 오차로 취급됨 | 그래디언트 0에 수렴 → 무시됨           |
| 경계 영역 민감도 | 그대로           | 그래디언트 최대 → 학습 집중            |
| 손실함수      | MSE 그대로 써도 무방 | Cross-entropy와 세트로 써야 진짜 효과 |

그러니까 너의 이해는 정확해 — "그냥 [0,1]로 누른다"가 다가 아니라, 
**양 끝의 포화 구조 자체가 극단값의 영향력을 줄이고 경계 근처에 학습을 집중시키는 정칙화(regularization)에 가까운 효과**를 낸다고 이해하면 돼. 
다만 이 효과는 시그모이드 모양 단독이 아니라 cross-entropy 손실함수와 짝을 이룰 때 완전해진다는 것도 같이 기억해두면 좋을 것 같아.



## 3. 비용 함수(Cost function)

이제 로지스틱 회귀의 가설이 $H(x)=sigmoid(Wx+b)$ 인 것은 알았습니다. 
이제 최적의 $W$와 $b$를 찾을 수 있는 비용 함수(cost function)를 정의해야 합니다. 
그런데 혹시 앞서 선형 회귀에서 배운 비용 함수인 평균 제곱 오차(Mean Square Error, MSE)를 로지스틱 회귀의 비용 함수로 그냥 사용하면 안 될까요?

다음은 선형 회귀에서 사용했던 평균 제곱 오차의 수식입니다.

$$\text{cost}(W, b) = \frac{1}{n} \sum_{i=1}^{n} \left[ y^{(i)} - H(x^{(i)}) \right]^2$$

위의 비용 함수 수식에서 가설은 이제 $H(x)=Wx+b$가 아니라 $H(x) = sigmoid(Wx+b)$ 입니다. 그리고 이 비용 함수를 미분하면 선형 회귀때와 달리 다음의 그림과 유사한 심한 비볼록(non-convex) 형태의 그래프가 나옵니다.

![](https://static.wikidocs.net/images/page/22881/%EB%A1%9C%EC%BB%AC%EB%AF%B8%EB%8B%88%EB%A9%88.PNG)

위와 같은 그래프에 경사 하강법을 사용할 경우의 문제점은 경사 하강법이 
오차가 최소값이 되는 구간에 도착했다고 판단한 그 구간이
<< 실제 오차가 완전히 최소값이 되는 구간이 아닐 수 있다 >> 는 점입니다. 

사람이 등산 후에 산을 내려올 때도, 가파른 경사를 내려오다가 넓은 평지가 나오면 순간적으로 다 내려왔다고 착각할 수 있습니다. 하지만 실제로는 그곳이 다 내려온 것이 아니라 잠깐 평지가 나왔을 뿐이라면 길을 더 찾아서 더 내려가야 할 겁니다. 

모델도 마찬가지로 << 실제 오차가 최소가 되는 구간을 찾을 수 있도록 >>
"올바른 오차함수를 통해!" 도와주어야 합니다. 만약, 실제 최소가 되는 구간을 잘못 판단하면 최적의 가중치 $W$가 아닌 다른 값을 택해 모델의 성능이 더 오르지 않습니다.

이를 전체 함수에 걸쳐 최소값인 글로벌 미니멈(Global Minimum): 전역최소이 아닌 특정 구역에서의 최소값인 로컬 미니멈(Local Minimum): 국소최소 에 도달했다고 합니다. 이는 cost가 최소가 되는 가중치 를 찾는다는 비용 함수의 목적에 맞지 않습니다.

시그모이드 함수의 특징은 함수의 출력값이 0과 1사이의 값이라는 점입니다. 
즉, << 실제값이 1일 때 예측값이 0에 가까워지면 오차가 커져야 하며, 
실제값이 0일 때, 예측값이 1에 가까워지면 오차가 커져야 합니다.  >>
그리고 이를 충족하는 함수가 바로 로그 함수입니다. 
다음은 $y=0.5$ 에 대칭하는 두 개의 로그 함수 그래프입니다.

![](https://static.wikidocs.net/images/page/57805/%EA%B7%B8%EB%9E%98%ED%94%84.PNG)

실제값이 1일 때의 그래프를 주황색 선으로 표현하였으며, 실제값이 0일 때의 그래프를 초록색 선으로 표현하였습니다. 

실제값이 1이라고 해봅시다. 
이 경우, 예측값인  $H(x)$ 의 값이 1이면 오차가 0이므로 당연히 cost는 0이 됩니다. 
반면, $H(x)$ 가 0으로 수렴하면 cost는 무한대로 발산합니다. 실제값이 0인 경우는 그 반대로 이해하면 됩니다(초록, 1-H(x)의 경우.). 
이 두 개의 로그 함수를 식으로 표현하면 다음과 같습니다.

$$ \text{if } y = 1 \rightarrow \text{cost}(H(x), y) = -\log(H(x))$$
$$\text{if } y = 0 \rightarrow \text{cost}(H(x), y) = -\log(1 - H(x))$$

$y$의 실제값이 1일 때 $- \log H(x)$ 그래프를 사용하고 
$y$의 실제값이 0일 때 $- \log (1-H(x))$ 그래프를 사용해야 합니다.  
이는 다음과 같이 하나의 식으로 통합할 수 있습니다.

$$ \text{cost}(H(x), y) = -[y \log H(x) + (1 - y) \log(1 - H(x))] $$

왜 위 식이 두 개의 식을 통합한 식이라고 볼 수 있을까요? 실제값 $y$ 가 1이라고 하면 덧셈 기호를 기준으로 우측의 항이 없어집니다. 반대로 실제값 가 0이라고 하면 덧셈 기호를 기준으로 좌측의 항이 없어집니다. 선형 회귀에서는 모든 오차의 평균을 구해 평균 제곱 오차를 사용했었습니다. 마찬가지로 여기에서도 모든 오차의 평균을 구합니다.

cost = 음의 로그 가능도 (negative log likelihood)

![[Pasted image 20260708142245.png|446]]

$$\text{cost}(W) = -\frac{1}{n} \sum_{i=1}^{n} [y^{(i)} \log H(x^{(i)}) + (1 - y^{(i)}) \log(1 - H(x^{(i)}))]$$
정리하면, 위 비용 함수는 실제값 $y$와 예측값 $H(x)$의 차이가 커지면 cost가 커지고, 
실제값 $y$와 예측값 $H(x)$ 의 차이가 작아지면 cost는 작아집니다. 
이제 위 비용 함수에 대해서 경사 하강법을 수행하면서 
최적의 가중치 $W$를 찾아갑니다.

$$W := W - \alpha \frac{\partial}{\partial W} \text{cost}(W)$$



## 4. 파이토치로 로지스틱 회귀 구현하기

이제 파이토치로 로지스틱 회귀 중에서도 
다수의 $x$로 부터 $y$를 예측하는 다중 로지스틱 회귀를 구현해봅시다.

우선 필요한 도구들을 임포트합니다.

```python
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
```

```python
torch.manual_seed(1)
```

x_train과 y_train을 텐서로 선언합니다.

```python
x_data = [[1, 2], [2, 3], [3, 1], [4, 3], [5, 3], [6, 2]]
y_data = [[0], [0], [0], [1], [1], [1]]
x_train = torch.FloatTensor(x_data)
y_train = torch.FloatTensor(y_data)
```

앞서 훈련 데이터를 행렬로 선언하고, 
행렬 연산으로 가설을 세우는 방법을 배웠습니다.  
여기서도 마찬가지로 행렬 연산을 사용하여 가설식을 세울겁니다. 
x_train과 y_train의 크기를 확인해봅시다.

```python
print(x_train.shape)
print(y_train.shape)
```

```python
torch.Size([6, 2])
torch.Size([6, 1])
```

현재 x_train은 6 × 2의 크기(shape)를 가지는 행렬이며, y_train은 6 × 1의 크기를 가지는 벡터입니다. 

x_train을 $X$ 라고 하고, 이와 곱해지는 가중치 벡터를 $W$ 라고 하였을 때, $XW$ 가 성립되기 위해서는  $W$ 벡터의 크기는 2 × 1이어야 합니다. 이제 W와 b를 선언합니다.

```python
W = torch.zeros((2, 1), requires_grad=True) # 크기는 2 x 1
b = torch.zeros(1, requires_grad=True)
```

이제 가설식을 세워보겠습니다. 
파이토치에서는 $e^x$ 를 구현하기 위해서 torch.exp(x)를 사용합니다.  
이에 따라 행렬 연산을 사용한 가설식은 다음과 같습니다.

```python
hypothesis = 1 / (1 + torch.exp(-(x_train.matmul(W) + b)))
```

앞서 W와 b는 torch.zeros를 통해 전부 0으로 초기화 된 상태입니다. 
이 상태에서 예측값을 출력해봅시다.

```python
print(hypothesis) # 예측값인 H(x) 출력
```

```python
tensor([[0.5000],
        [0.5000],
        [0.5000],
        [0.5000],
        [0.5000],
        [0.5000]], grad_fn=<MulBackward>)
```

실제값 y_train과 크기가 동일한 6 × 1의 크기를 가지는 예측값 벡터가 나오는데 
모든 값이 0.5입니다.

사실 가설식을 좀 더 간단하게도 구현할 수 있습니다. 이미 파이토치에서는 시그모이드 함수를 이미 구현하여 제공하고 있기 때문입니다. 다음은 torch.sigmoid를 사용하여 좀 더 간단히 구현한 가설식입니다.

```python
hypothesis = torch.sigmoid(x_train.matmul(W) + b)
```

앞서 구현한 식과 본질적으로 동일한 식입니다. 마찬가지로 W와 b가 0으로 초기화 된 상태에서 예측값을 출력해봅시다.

```python
print(hypothesis)
```

```python
tensor([[0.5000],
        [0.5000],
        [0.5000],
        [0.5000],
        [0.5000],
        [0.5000]], grad_fn=<SigmoidBackward>)
```

앞선 결과와 동일하게 y_train과 크기가 동일한 6 × 1의 크기를 가지는 예측값 벡터가 나오는데 모든 값이 0.5입니다.

이제 아래의 비용 함수값. 즉, 현재 예측값과 실제값 사이의 cost를 구해보겠습니다.

우선, 현재 예측값과 실제값을 출력해보겠습니다.

```python
print(hypothesis)
print(y_train)
```

```python
tensor([[0.5000],
        [0.5000],
        [0.5000],
        [0.5000],
        [0.5000],
        [0.5000]], grad_fn=<SigmoidBackward>)
tensor([[0.],
        [0.],
        [0.],
        [1.],
        [1.],
        [1.]])
```

현재 총 6개의 원소가 존재하지만 하나의 샘플. 즉, 하나의 원소에 대해서만 오차를 구하는 식을 작성해보겠습니다.

```python
-(y_train[0] * torch.log(hypothesis[0]) + 
  (1 - y_train[0]) * torch.log(1 - hypothesis[0]))
```

```python
tensor([0.6931], grad_fn=<NegBackward>)
```

이제 모든 원소에 대해서 오차를 구해보겠습니다.

```python
losses = -(y_train * torch.log(hypothesis) + 
           (1 - y_train) * torch.log(1 - hypothesis))
print(losses)
```

```python
tensor([[0.6931],
        [0.6931],
        [0.6931],
        [0.6931],
        [0.6931],
        [0.6931]], grad_fn=<NegBackward>)
```

그리고 이 전체 오차에 대한 평균을 구합니다.

```python
cost = losses.mean()
print(cost)
```

```python
tensor(0.6931, grad_fn=<MeanBackward1>)
```

결과적으로 얻은 cost는 0.6931입니다.

지금까지 비용 함수의 값을 직접 구현하였는데, 사실 파이토치에서는 로지스틱 회귀의 비용 함수를 이미 구현해서 제공하고 있습니다.  
사용 방법은 torch.nn.functional as F와 같이 임포트 한 후에 F.binary_cross_entropy(예측값, 실제값)과 같이 사용하면 됩니다.

```python
F.binary_cross_entropy(hypothesis, y_train)
```

```python
tensor(0.6931, grad_fn=<BinaryCrossEntropyBackward>)
```

동일하게 cost가 0.6931이 출력되는 것을 볼 수 있습니다. 
모델의 훈련 과정까지 추가한 전체 코드는 아래와 같습니다.

```python
x_data = [[1, 2], [2, 3], [3, 1], [4, 3], [5, 3], [6, 2]]
y_data = [[0], [0], [0], [1], [1], [1]]
x_train = torch.FloatTensor(x_data)
y_train = torch.FloatTensor(y_data)
```

```python
# 모델 초기화
W = torch.zeros((2, 1), requires_grad=True)
b = torch.zeros(1, requires_grad=True)
# optimizer 설정
optimizer = optim.SGD([W, b], lr=1)

nb_epochs = 1000
for epoch in range(nb_epochs + 1):

    # Cost 계산
    hypothesis = torch.sigmoid(x_train.matmul(W) + b)
    cost = -(y_train * torch.log(hypothesis) + 
             (1 - y_train) * torch.log(1 - hypothesis)).mean()

    # cost로 H(x) 개선
    optimizer.zero_grad()
    cost.backward()
    optimizer.step()

    # 100번마다 로그 출력
    if epoch % 100 == 0:
        print('Epoch {:4d}/{} Cost: {:.6f}'.format(
            epoch, nb_epochs, cost.item()
        ))
```

```python
Epoch    0/1000 Cost: 0.693147
... 중략 ...
Epoch 1000/1000 Cost: 0.019852
```

학습이 끝났습니다. 
이제 훈련했던 훈련 데이터를 그대로 입력으로 사용했을 때, 제대로 예측하는지 확인해보겠습니다.  
현재 W와 b는 훈련 후의 값을 가지고 있습니다. 현재 W와 b를 가지고 예측값을 출력해보겠습니다.

```python
hypothesis = torch.sigmoid(x_train.matmul(W) + b)
print(hypothesis)
```

```python
tensor([[2.7648e-04],
        [3.1608e-02],
        [3.8977e-02],
        [9.5622e-01],
        [9.9823e-01],
        [9.9969e-01]], grad_fn=<SigmoidBackward>)
```

현재 위 값들은 0과 1 사이의 값을 가지고 있습니다. 이제 0.5를 넘으면 True, 넘지 않으면 False로 값을 정하여 출력해보겠습니다.

```python
prediction = hypothesis >= torch.FloatTensor([0.5])
print(prediction)
```

```python
tensor([[False],
        [False],
        [False],
        [ True],
        [ True],
        [ True]])
```

실제값은 `[[0], [0], [0], [1], [1], [1]]`이므로, 이는 결과적으로 False, False, False, True, True, True와 동일합니다. 즉, 기존의 실제값과 동일하게 예측한 것을 볼 수 있습니다. 훈련이 된 후의 W와 b의 값을 출력해보겠습니다.

```python
print(W)
print(b)
```

```python
tensor([[3.2530],
        [1.5179]], requires_grad=True)
tensor([-14.4819], requires_grad=True)
```


# 04-02 nn.Module, 클래스 구현 

잠깐만 복습을 해보면 선형 회귀 모델의 가설식은 $H(x) = Wx+b$ 이었습니다. 
그리고 이 가설식을 구현하기 위해서 파이토치의 nn.Linear()를 사용했습니다. 
그리고 로지스틱 회귀의 가설식은 $H(x) = sigmoid(Wx+b)$ 입니다. 

파이토치에서는 nn.Sigmoid()를 통해서 시그모이드 함수를 구현하므로 결과적으로 <<nn.Linear()의 결과를 nn.Sigmoid()를 거치게 하면>> 로지스틱 회귀의 가설식이 됩니다.

파이토치를 통해 이를 구현해봅시다.

## 1. 파이토치의 nn.Linear와 nn.Sigmoid로                           로지스틱 회귀 구현하기

우선 구현을 위해 필요한 파이토치의 도구들을 임포트합니다.

```python
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
```

```python
torch.manual_seed(1)
```

훈련 데이터를 텐서로 선언합니다.

```python
x_data = [[1, 2], [2, 3], [3, 1], [4, 3], [5, 3], [6, 2]]
y_data = [[0], [0], [0], [1], [1], [1]]
x_train = torch.FloatTensor(x_data)
y_train = torch.FloatTensor(y_data)
```

nn.Sequential()은 << nn.Module 층을 차례로 쌓을 수 있도록!! 합니다. >>>
뒤에서 이를 이용해서 인공 신경망을 구현하게 되므로 기억하고 있으면 좋습니다. 

조금 쉽게 말해서 nn.Sequential()은 와 같은 수식과 시그모이드 함수 등과 같은 
여러 함수들을 <<연결>>해주는 역할을 합니다. 
이를 이용해서 로지스틱 회귀를 구현해봅시다.

```python
model = nn.Sequential(
   nn.Linear(2, 1), # input_dim = 2, output_dim = 1
   nn.Sigmoid() # 출력은 시그모이드 함수를 거친다
)
```

현재 W와 b는 랜덤 초기화가 된 상태입니다. 
훈련 데이터를 넣어 예측값을 확인해봅시다.

```python
model(x_train)
```

```python
tensor([[0.4020],
        [0.4147],
        [0.6556],
        [0.5948],
        [0.6788],
        [0.8061]], grad_fn=<SigmoidBackward>)
```

6 × 1 크기의 예측값 텐서가 출력됩니다. 
그러나 현재 W와 b는 임의의 값을 가지므로 현재의 예측은 의미가 없습니다. 
이제 경사 하강법을 사용하여 훈련해보겠습니다. 
총 100번의 에포크를 수행합니다. 
각 에포크마다 정확도를 계산하여 정확도도 출력해보겠습니다

```python
# optimizer 설정
optimizer = optim.SGD(model.parameters(), lr=1)

nb_epochs = 1000
for epoch in range(nb_epochs + 1):

    # H(x) 계산
    hypothesis = model(x_train)

    # cost 계산
    cost = F.binary_cross_entropy(hypothesis, y_train)

    # cost로 H(x) 개선
    optimizer.zero_grad()
    cost.backward()
    optimizer.step()

    # 20번마다 로그 출력
    if epoch % 10 == 0:
        prediction = hypothesis >= torch.FloatTensor([0.5]) 
	        # 예측값이 0.5를 넘으면 True로 간주
        correct_prediction = prediction.float() == y_train 
	        # 실제값과 일치하는 경우만 True로 간주
        accuracy = correct_prediction.sum().item() 
        / len(correct_prediction) # 정확도를 계산
        print('Epoch {:4d}/{} Cost: {:.6f} Accuracy {:2.2f}%'.format( # 각 에포크마다 정확도를 출력
            epoch, nb_epochs, cost.item(), accuracy * 100,
        ))
```

이 코드는 이진 분류 모델을 학습시키는 과정을 보여줍니다. 
먼저, SGD 옵티마이저를 설정하고, 1000번 동안 모델을 학습시킵니다. 

각 반복에서 모델이 x_train을 사용해 예측을 수행하고, 이 예측값과 실제값인 y_train을 비교해 손실 값을 계산합니다.  (교차 엔트로피 손실함수)

그런 다음, 손실을 줄이기 위해 모델의 파라미터를 업데이트합니다. 

학습 과정에서 매 10번째 반복마다 예측의 정확도를 계산하고 출력합니다. 이 정확도는 모델이 얼마나 잘 학습되고 있는지를 평가하는 지표로 사용됩니다.

```python
Epoch    0/1000 Cost: 0.539713 Accuracy 83.33%
... 중략 ...
Epoch 1000/1000 Cost: 0.019843 Accuracy 100.00%
```

중간부터 정확도는 100%가 나오기 시작합니다. 기존의 훈련 데이터를 입력하여 예측값을 확인해보겠습니다.

```python
model(x_train)
```

```python
tensor([[0.0240],
        [0.1476],
        [0.2739],
        [0.7967],
        [0.9491],
        [0.9836]], grad_fn=<SigmoidBackward>)
```

0.5를 넘으면 True, 그보다 낮으면 False로 간주합니다. 실제값은 `[[0], [0], [0], [1], [1], [1]]` 입니다. 이는 False, False, False, True, True, True에 해당되므로 전부 실제값과 일치하도록 예측한 것을 확인할 수 있습니다.

훈련 후의 W와 b의 값을 출력해보겠습니다.

```python
print(list(model.parameters()))
```

```python
[Parameter containing:
tensor([[3.2534, 1.5181]], requires_grad=True), Parameter containing:
tensor([-14.4839], requires_grad=True)]
```

출력값이 앞 챕터에서 nn.Module을 사용하지 않고 
로지스틱 회귀를 구현한 실습에서 얻었던 W와 b와 거의 일치합니다.

앞 챕터 결과:

```python
tensor([[3.2530],
        [1.5179]], requires_grad=True)
tensor([-14.4819], requires_grad=True)
```



## 2. 인공 신경망으로 표현되는 로지스틱 회귀

사실 로지스틱 회귀는 인공 신경망으로 간주할 수 있습니다.

![](https://static.wikidocs.net/images/page/58686/logistic_regression.PNG)

위의 인공 신경망 그림에서 각 화살표는 입력과 곱해지는 가중치 또는 편향입니다. 각 입력에 대해서 검은색 화살표는 가중치, 회색 화살표는 편향이 곱해집니다. 
각 입력 $x_{i}$는 각 입력의 가중치 $w_i$와 곱해지고, 편향 $b$는 상수 1과 곱해지는 것으로 
표현되었습니다. 그리고 출력하기 전에 시그모이드 함수를 지나게 됩니다.

결과적으로 위의 인공 신경망은 다음과 같은 다중 로지스틱 회귀를 표현하고 있습니다.  

$H(x) = sigmoid(x_1w_1+x_2w_2+b)$

- **뒤에서 인공 신경망을 배우면서 언급하겠지만, 시그모이드 함수는 인공 신경망의 은닉층에서는 거의 사용되지 않습니다.**

로지스틱 회귀와 소프트맥스 회귀 : https://hyeonnii.tistory.com/239

<< 파이토치의 대부분의 구현체 >> 들은 대부분 모델을 생성할 때 클래스(Class)를 사용하고 있습니다. 앞서 배운 로지스틱 회귀를 클래스로 구현해보겠습니다. 

앞서 구현한 코드와 다른 점은 오직 클래스로 모델을 구현했다는 점입니다.

## 3. 모델을 클래스로 구현하기

앞서 로지스틱 회귀 모델은 다음과 같이 구현했었습니다.

```python
model = nn.Sequential(
   nn.Linear(2, 1), # input_dim = 2, output_dim = 1
   nn.Sigmoid() # 출력은 시그모이드 함수를 거친다
)
```

이를 클래스로 구현하면 다음과 같습니다.

```python
class BinaryClassifier(nn.Module):
    def __init__(self):
        super().__init__()
        self.linear = nn.Linear(2, 1)
        self.sigmoid = nn.Sigmoid()

    def forward(self, x):
        return self.sigmoid(self.linear(x))
```

위와 같은 클래스를 사용한 모델 구현 형식은 <<< 대부분의 파이토치 구현체에서 사용하고 있는 방식으로 반드시 숙지할 필요가 있습니다. >>>

클래스(class) 형태의 모델은 nn.Module 을 상속받습니다. 
그리고 __init__()에서 모델의 구조와 동적을 정의하는 생성자를 정의합니다. 

이는 파이썬에서 객체가 갖는 속성값을 초기화하는 역할로, 객체가 생성될 때 자동으로 호출됩니다. super() 함수를 부르면 여기서 만든 클래스는 nn.Module 클래스의 속성들을 가지고 초기화 됩니다. 

foward() 함수는 모델이 학습데이터를 입력받아서 forward 연산을 진행시키는 함수입니다. 이 forward() 함수는 model 객체를 데이터와 함께 호출하면 자동으로 실행이 됩니다. 
예를 들어 model이란 이름의 객체를 생성 후, model(입력 데이터)와 같은 형식으로 객체를 호출하면 자동으로 forward 연산이 수행됩니다.

-  식에 입력 로부터 예측된 를 얻는 것을 forward 연산이라고 합니다.

(03-모델을 클래스로 구현하기 파트에서는 파이토치 선형회귀 구현체 숙지 복습하며 읽으면 좋음.)

## 4. 로지스틱 회귀 클래스로 구현하기

이제 모델을 클래스로 구현한 코드를 보겠습니다. 
달라진 점은 모델을 클래스로 구현했다는 점 뿐입니다. 다른 코드는 전부 동일합니다.

```python
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
```

```python
torch.manual_seed(1)
```

```python
x_data = [[1, 2], [2, 3], [3, 1], [4, 3], [5, 3], [6, 2]]
y_data = [[0], [0], [0], [1], [1], [1]]
x_train = torch.FloatTensor(x_data)
y_train = torch.FloatTensor(y_data)
```

x_data는 입력 데이터로, 각 항목은 두 개의 숫자로 구성된 리스트입니다. 
y_data는 각 입력 데이터에 해당하는 출력 값(레이블)으로, 0과 1로 이루어진 리스트입니다. 이 데이터를 torch.FloatTensor로 변환하여, x_train과 y_train이라는 PyTorch 텐서로 만듭니다. 이 텐서들은 이후 모델 학습에 사용됩니다.

```python
class BinaryClassifier(nn.Module):
    def __init__(self):
        super().__init__()
        self.linear = nn.Linear(2, 1)
        self.sigmoid = nn.Sigmoid()

    def forward(self, x):
        return self.sigmoid(self.linear(x))

model = BinaryClassifier()
```

BinaryClassifier라는 클래스는 << 신경망을 정의 >> 하는 부분입니다. 

이 클래스는 두 개의 숫자를 입력으로 받아 하나의 출력을 내는 모델을 만듭니다. 
먼저, **init** 함수에서 nn.Linear를 사용하여 입력 데이터를 선형 변환하고, 
그 결과에 Sigmoid 함수를 적용하여 0과 1 사이의 값을 출력합니다. 

이 값은 << 입력 데이터가 특정 클래스에 속할 확률 >> 을 나타냅니다. 
이 모델은 이진 분류 문제에서 사용됩니다.

```python
# optimizer 설정
optimizer = optim.SGD(model.parameters(), lr=1)

nb_epochs = 1000
for epoch in range(nb_epochs + 1):

    # H(x) 계산
    hypothesis = model(x_train)

    # cost 계산
    cost = F.binary_cross_entropy(hypothesis, y_train)

    # cost로 H(x) 개선
    optimizer.zero_grad()
    cost.backward()
    optimizer.step()

    # 20번마다 로그 출력
    if epoch % 10 == 0:
        prediction = hypothesis >= torch.FloatTensor([0.5]) # 예측값이 0.5를 넘으면 True로 간주
        correct_prediction = prediction.float() == y_train # 실제값과 일치하는 경우만 True로 간주
        accuracy = correct_prediction.sum().item() / len(correct_prediction) # 정확도를 계산
        print('Epoch {:4d}/{} Cost: {:.6f} Accuracy {:2.2f}%'.format( # 각 에포크마다 정확도를 출력
            epoch, nb_epochs, cost.item(), accuracy * 100,
        ))
```

옵티마이저로 SGD(Stochastic Gradient Descent)를 설정하고 학습률을 1로 지정합니다. 이후, 1000번의 에포크 동안 모델을 학습시키기 위해 반복문을 실행합니다.

각 에포크에서, 먼저 모델을 통해 예측값인 hypothesis를 계산합니다. 이 예측값과 실제 레이블인 y_train 간의 손실을 binary_cross_entropy 함수로 계산하여 cost 값을 얻습니다.

다음으로, 옵티마이저의 기울기를 초기화하고, cost.backward()를 통해 역전파를 수행하여 모델의 파라미터에 대한 기울기를 계산합니다. 그런 다음, optimizer.step()을 호출하여 파라미터를 업데이트합니다.

매 10번째 에포크마다, 예측값이 0.5 이상인 경우를 True로 간주하여 이진 분류를 수행하고, 이 예측이 실제 레이블과 얼마나 일치하는지 비교하여 정확도를 계산합니다. 계산된 정확도와 비용(cost)을 출력하여 학습 진행 상황을 모니터링합니다. 이 과정을 통해 모델이 점차적으로 학습되어, 정확한 예측을 할 수 있도록 합니다.



## 가능도 개념 복습
### 확률 vs 가능도: 같은 식, 다른 변수 취급

$P(x \mid \theta)$  라는 함수가 있다고 하자. 
여기서 x는 데이터, θ는 파라미터(모델).

- **확률(probability)로 볼 때**: θ를 고정하고, x를 변수로 본다.  
    → "이 모델이 주어졌을 때, 이 데이터가 나올 확률은?"  
    → x에 대해 적분(또는 합)하면 1이 된다.
- **가능도(likelihood)로 볼 때**: xx x(관측된 데이터)를 고정하고, θ\theta θ를 변수로 본다.  
    → "이 데이터가 관측됐다는 걸 이미 알고 있을 때, 어떤 θ가 이 데이터를 그럴듯하게 만드는가?"  
    → 여기서는 θ에 대해 적분해도 1이 안 됨. 확률분포가 아니라 그냥 함수.

기호로는 똑같이 쓰기도 해서 헷갈리는데, 보통 이렇게 표기 구분함:  

$L(θ∣x)=P(x∣θ)$

**숫자는 같지만 "무엇의 함수로 보느냐"가 다르다** — 이게 전부야.

### "모델이 학습 데이터를 생성할 가능성"이 정확히 뭘 뜻하는가

너가 말한 정의를 정확히 풀면:

- 학습 데이터 $x_1, x_2, ..., x_n$ (예: 코퍼스의 다음 토큰들)이 이미 주어져 있고 고정됨
- 모델 파라미터 θ (weights)를 바꿔가면서
- "이 θ를 가진 모델이 실제로 관측된 이 데이터들을 생성할 확률이 얼마나 되는가"를 계산

LLM 맥락으로 구체화하면:

$L(\theta) = \prod_{i} P_\theta(x_i \mid x_{<i})$

즉 코퍼스의 각 토큰 $x_i$를, 그 앞 토큰들 $x_{<i}$​가 주어졌을 때 << 모델이 예측하는 확률의 곱. 이게 전체 시퀀스에 대한 가능도야. >>

**학습(training)이 하는 일 
= 가능도를 최대화하는 θ를 찾는 것 (MLE, Maximum Likelihood Estimation).** 
실제로는 곱이 아니라 로그를 씌워서 합으로 바꾸고(underflow 방지 + 미분 편의), 
부호를 뒤집어서 최소화 문제로 만든 게 바로 너가 아는 **cross-entropy loss**야.

$-\log L(\theta) = -\sum_i \log P_\theta(x_i \mid x_{<i})$


즉 가능도는 "관측을 설명하는 파라미터를 거꾸로 찾는" 관점이고, 
확률은 "파라미터가 주어졌을 때 관측이 나올 확률을 순방향으로 계산하는" 관점이야.

### 요약

|         | 확률 (Probability)     | 가능도 (Likelihood)           |
| ------- | -------------------- | -------------------------- |
| 고정      | θ                    | x (데이터)                    |
| 변수      | x                    | θ                          |
| 적분/합이 1 | O (x에 대해)            | X (θ에 대해서는 1 아님)           |
| 질문      | 이 모델에서 이 데이터가 나올 확률? | 이 데이터를 가장 잘 설명하는 모델은?      |
| LLM 학습  | —                    | 이걸 최대화 = cross-entropy 최소화 |

```
그럼 가능도는 Lang model에서 self attention, 딥러닝 신경망으로 예측한 다음 토큰이 실제 학습 데이터와 일치할 확률이라고 이해하면되는건가?

A B C 가 정답일떄 A B D, A B C를 각각 1회 4회 출력했으면 0.8인건가?
```


좋은 직관이야! 방향은 맞는데, 딱 하나 짚어야 할 지점이 있어
— **"여러 번 뽑아서 빈도를 세는 것"이 아니라, 모델이 한 번의 forward pass로 확률분포 자체를 직접 출력**한다는 점이야.

### 너의 예시를 정확히 고쳐보면

Self-attention + FFN을 거친 후, 모델의 마지막 층은 **softmax**를 통과해서 어휘 전체(vocab)에 대한 확률분포를 한 번에 뱉어.

A,BA, B A,B 다음에 올 토큰을 예측한다고 하면:

```
P(A B 다음 토큰 = C) = 0.8
P(A B 다음 토큰 = D) = 0.15
P(A B 다음 토큰 = 기타) = 0.05
```

이게 **한 번의 forward pass에서 나오는 결과**야. 5번 생성해서 4번은 C, 1번은 D가 나왔다는 식으로 **경험적으로 빈도를 세서 추정하는 게 아니라**, 모델이 그 자리에서 정확히 소수점 확률값을 계산해서 뱉어.

> P() => A, B 라는 입력이 들어오면:
	1. **여러 층의 self-attention + FFN**을 거치면서, 입력이 가중치들과 행렬곱을 반복하며 변환됨 → 마지막에 **hidden state** (벡터 하나)가 나옴
	2. 이 hidden state에 **LM head weight**를 곱해서 → vocab 크기만큼의 **logits** (아직 확률 아님, 그냥 실수값들. 예: C=4.2, D=2.1, 기타=0.3 이런 식)
	3. 이 logits에 **softmax**를 적용 → 그제서야 0~1 사이의 확률로 변환됨 (P(C)=0.8, P(D)=0.15...)

→ 정답이 C일 때, **그 위치에서의 likelihood 기여분 = 모델이 C에 할당한 확률값 그 자체** (예: 0.8).

### 왜 이 차이가 중요한가

너의 "1회 vs 4회 → 0.8" 비유는 사실 꽤 정확한 직관이야. **빈도주의(frequentist) 확률 해석**으로 보면, 모델을 무한히 많이 샘플링했을 때 C가 나오는 비율이 결국 softmax가 준 확률값에 수렴하거든. 즉:

$$\lim_{n\to\infty} \frac{\text{C가 나온 횟수}}{n} = P_\theta(C \mid A, B)$$

그래서 "그렇게 이해해도 되냐"는 질문에는 **개념적으로는 맞다**고 답할 수 있어. 다만 실제로 학습(training) 중에는:
- 모델을 여러 번 실행해서 빈도를 세지 않고
- **딱 한 번의 forward pass로 나온 softmax 출력값을 그대로 확률로 사용**

### 전체 시퀀스로 확장

A,B,C 전체가 정답 시퀀스일 때, 시퀀스 전체의 likelihood는 각 위치에서의 조건부 확률을 곱한 거야 (teacher forcing — 이전 토큰은 항상 **모델이 생성한 게 아니라 실제 정답**을 넣어줌):

$$L = P(A) \times P(B \mid A) \times P(C \mid A, B)$$

이 중 마지막 항이 너가 예시로 든 0.8이고, << 학습은 이 곱(정확히는 log 취한 합)을 최대화하는 방향으로 파라미터를 업데이트하는 과정 >> 이야.

### 요약

|너의 이해|실제|
|---|---|
|N번 생성 → C 나온 비율 계산|softmax가 한 번에 확률분포 전체를 출력|
|경험적(empirical) 빈도|모델이 직접 계산한 정확한 확률값|
|극한에서는...|...두 값이 같아짐 (직관은 맞음)|
