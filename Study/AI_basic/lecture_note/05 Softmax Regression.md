
> 이번 챕터에서는 3개 이상의 선택지로부터 1개를 선택하는 문제인 다중 클래스 분류(Multi-Class classification)를 풀기 위한 소프트맥스 회귀에 대해서 학습합니다.

# 05-01 원-핫 인코딩(One-Hot Encoding)

이번 챕터에서는 << 범주형 데이터를 처리할 때 레이블을 표현 >>하는 방법인 
원-핫 인코딩에 대해서 배워봅시다.

## 1. 원-핫 인코딩(One-hot encoding)이란?

원-핫 인코딩은 선택해야 하는 선택지의 개수만큼의 차원을 가지면서, 각 선택지의 인덱스에 해당하는 원소에는 1, 나머지 원소는 0의 값을 가지도록 하는 표현 방법입니다. 예를 들어 강아지, 고양이, 냉장고라는 3개의 선택지가 있다고 해보겠습니다.

원-핫 인코딩을 하기 위해서는 우선 각 선택지에 순차적으로 << 정수 인덱스 >> 를 부여합니다. 임의로 강아지는 0번 인덱스, 고양이는 1번 인덱스, 냉장고는 2번 인덱스를 부여하였다고 해봅시다. 이때 각 선택지에 대해서 원-핫 인코딩이 된 벡터는 다음과 같습니다.

강아지 = `[1, 0, 0]`
고양이 = `[0, 1, 0]`  
냉장고 = `[0, 0, 1]`

총 선택지는 3개였으므로 위 벡터들은 전부 3차원의 벡터가 되었습니다. 그리고 각 선택지의 벡터들을 보면 해당 선택지의 인덱스에만 1의 값을 가지고, 나머지 원소들은 0의 값을 가집니다. 예를 들어 고양이는 1번 인덱스였으므로 원-핫 인코딩으로 얻은 벡터에서 1번 인덱스만 1의 값을 가지는 것을 볼 수 있습니다.

이와 같이 원-핫 인코딩으로 표현된 벡터를 
**원-핫 벡터(one-hot vector)** 라고 합니다.

## 2. 원-핫 벡터의 무작위성

꼭 실제값을 원-핫 벡터로 표현해야만 다중 클래스 분류 문제를 풀 수 있는 것은 아니지만, 대부분의 다중 클래스 분류 문제가 << 각 클래스 간의 관계가 균등 >>하다는 점에서 원-핫 벡터는 이러한 점을 표현할 수 있는 적절한 표현 방법입니다.

다수의 클래스를 분류하는 문제에서는 이진 분류처럼 2개의 숫자 레이블이 아니라 << 클래스의 개수만큼 숫자 레이블이 필요 >> 합니다. 
이때 직관적으로 생각해볼 수 있는 레이블링 방법은 
분류해야 할 클래스 전체에 정수 인코딩을 하는 겁니다. 

예를 들어서 분류해야 할 레이블이 {red, green, blue}와 같이 3개라면 각각 0, 1, 2로 레이블을 합니다. 
또는 분류해야 할 클래스가 4개고 인덱스를 숫자 1부터 시작하고 싶다고 하면 {baby, child, adolescent, adult}라면 1, 2, 3, 4로 레이블을 해볼 수 있습니다. 

그런데 일반적인 다중 클래스 분류 문제에서 레이블링 방법으로는 위와 같은 정수 인코딩이 아니라 원-핫 인코딩을 사용하는 것이 보다 클래스의 성질을 잘 표현하였다고 할 수 있습니다. 그 이유를 알아봅시다.

Banana, Tomato, Apple라는 3개의 클래스가 존재하는 문제가 있다고 해봅시다. 레이블은 정수 인코딩을 사용하여 각각 1, 2, 3을 부여하였습니다. 

손실 함수로 선형 회귀 챕터에서 배운 평균 제곱 오차 MSE를 사용하면 정수 인코딩이 어떤 오해를 불러일으킬 수 있는지 확인할 수 있습니다. 아래의 식은 앞서 선형 회귀에서 배웠던 MSE를 다시 그대로 가져온 것입니다. $\hat{{y}}$는 예측값을 의미합니다.

$$\text{Loss function} = \frac{1}{n} \sum_{i}^{n} (y_i - \hat{y}_i)^2$$

직관적인 오차 크기 비교를 위해 평균을 구하는 수식은 제외하고 
제곱 오차로만 판단해봅시다.

실제값이 Tomato일때 예측값이 Banana이었다면 제곱 오차는 다음과 같습니다.  
$(2-1)^2=1$

실제값이 Apple일때 예측값이 Banana이었다면 제곱 오차는 다음과 같습니다.  
$(3-1)^2 = 4$

즉, Banana과 Tomato 사이의 오차보다 Banana과 Apple의 오차가 더 큽니다. 이는 기계에게 <<< Banana가 Apple보다는 Tomato에 더 가깝다는 정보 >>> 를 주는 것과 다름없습니다. 더 많은 클래스에 대해서 정수 인코딩을 수행했다고 해봅시다.

{Banana :1, Tomato :2, Apple :3, Strawberry :4, ... Watermelon :10}

이 정수 인코딩은 Banana가 Watermelon보다는 Tomato에 더 가깝다는 의미를 담고 있습니다. 이는 사용자가 부여하고자 했던 정보가 아닙니다. 

이러한 정수 인코딩의 순서 정보가 도움이 되는 분류 문제도 물론 있습니다. 바로 각 클래스가 순서의 의미를 갖고 있어서 회귀를 통해서 분류 문제를 풀 수 있는 경우입니다. 예를 들어 {baby, child, adolescent, adult}나 {1층, 2층, 3층, 4층}이나 {10대, 20대, 30대, 40대}와 같은 경우가 이에 해당됩니다. 

하지만 일반적인 분류 문제에서는 각 클래스는 << 순서의 의미를 갖고 있지 않으므로 각 클래스 간의 오차는 균등한 것이 옳습니다. >> 정수 인코딩과 달리 원-핫 인코딩은 분류 문제 모든 클래스 간의 관계를 균등하게 분배합니다.

아래는 세 개의 카테고리에 대해서 원-핫 인코딩을 통해서 레이블을 인코딩했을 때 각 클래스 간의 제곱 오차가 균등함을 보여줍니다.

$$((1,0,0)-(0,1,0))^2 = (1-0)^2 + (0-1)^2 + (0-0)^2 = 2$$
$$((1,0,0)-(0,0,1))^2 = (1-0)^2 + (0-0)^2 + (0-1)^2 = 2$$

다르게 표현하면 모든 클래스에 대해서 원-핫 인코딩을 통해 얻은 원-핫 벡터들은 모든 쌍에 대해서 유클리드 거리를 구해도 전부 유클리드 거리가 동일합니다. 원-핫 벡터는 이처럼 각 클래스의 표현 방법이 무작위성을 가진다는 점을 표현할 수 있습니다. 뒤에서 다시 언급되겠지만 이러한 원-핫 벡터의 관계의 무작위성은 때로는 단어의 유사성을 구할 수 없다는 단점으로 언급되기도 합니다.



# 05-02 소프트맥스 회귀(Softmax Regression) 이해

앞서 로지스틱 회귀를 통해 2개의 선택지 중에서 1개를 고르는 이진 분류(Binary Classification)를 풀어봤습니다. 이번 챕터에서는 소프트맥스 회귀를 통해 3개 이상의 선택지 중에서 1개를 고르는 다중 클래스 분류(Multi-Class Classification)를 실습해봅시다.

## 1. 다중 클래스 분류(Multi-class Classification)

이진 분류가 두 개의 답 중 하나를 고르는 문제였다면, 세 개 이상의 답 중 하나를 고르는 문제를 다중 클래스 분류(Multi-class Classification)라고 합니다. 
아래의 문제는 꽃받침 길이, 꽃받침 넓이, 꽃잎 길이, 꽃잎 넓이라는 4개의 특성(feature)로부터 setosa, versicolor, virginica라는 3개의 붓꽃 품종 중 어떤 품종인지를 예측하는 문제로 전형적인 다중 클래스 분류 문제입니다.

| 꽃받침 길이($x_1$) | 꽃받침 넓이($x_2$) | 꽃잎 길이($x_3$) | 꽃잎 넓이($x_4$) | 붓꽃 품종(y)   |
| ------------- | ------------- | ------------ | ------------ | ---------- |
| 5.1           | 3.5           | 1.4          | 0.2          | setosa     |
| 4.9           | 3.0           | 1.4          | 0.2          | setosa     |
| 5.8           | 2.6           | 4.0          | 1.2          | versicolor |
| 6.7           | 3.0           | 5.2          | 2.3          | virginica  |
| 5.6           | 2.8           | 4.9          | 2.0          | virginica  |

위 **붓꽃 품종 분류하기 문제**를 어떻게 풀지 고민하기 위해 앞서 배운 로지스틱 회귀의 이진 분류를 복습해보겠습니다.

이번 챕터의 설명에서 입력은 $X$, 가중치는 $W$, 편향은 $B$, 출력은 $\hat{Y}$로 
각 변수는 벡터 또는 행렬로 가정합니다.

- $\hat{Y}$은 예측값이라는 의미를 가지고 있으므로
	가설식에서  $H(X)$대신 사용되기도 합니다.

### 1. 로지스틱 회귀

이진 분류 : 로지스틱 회귀에서 시그모이드 함수는 예측값을 0과 1 사이의 값으로 만듭니다. 예를 들어 스팸 메일 분류기를 로지스틱 회귀로 구현하였을 때, 출력이 0.75이라면 이는 이메일이 스팸일 확률이 75%라는 의미가 됩니다. 반대로, 스팸 메일이 아닐 확률은 25%가 됩니다. 이 두 확률의 총 합은 1입니다.

![](https://static.wikidocs.net/images/page/59427/%EB%A1%9C%EC%A7%80%EC%8A%A4%ED%8B%B1%ED%9A%8C%EA%B7%80.PNG)

**가설 :** $H(X) = sigmoid(WX+B)$

### 2. 소프트맥스 회귀

소프트맥스 회귀는 확률의 << 총 합이 1이 되는 이 아이디어를 다중 클래스 분류 문제에 적용 >> 합니다. 소프트맥스 회귀는 각 클래스. 
즉, 각 선택지마다 소수 확률을 할당합니다. 이때 총 확률의 합은 1이 되어야 합니다. 이렇게 되면 각 선택지가 정답일 확률로 표현됩니다.

![](https://static.wikidocs.net/images/page/59427/%EC%86%8C%ED%94%84%ED%8A%B8%EB%A7%A5%EC%8A%A4%ED%9A%8C%EA%B7%80.PNG)

결국 소프트맥스 회귀는 선택지의 개수만큼의 차원을 가지는 벡터를 만들고, 해당 벡터가 벡터의 모든 원소의 합이 1이 되도록 원소들의 값을 변환시키는 어떤 함수를 지나게 만들어야 합니다. 위의 그림은 붓꽃 품종 분류하기 문제 등과 같이 선택지의 << 개수가 3개일때, 3차원 벡터가 어떤 함수 ?를 지나 원소의 총 합이 1 >>이 되도록 원소들의 값이 변환되는 모습을 보여줍니다. 뒤에서 배우겠지만, 이 함수를 소프트맥스(softmax) 함수라고 합니다.

**가설 :** $H(X) = softmax(WX+B)$

## 2. 소프트맥스 함수(Softmax function)

소프트맥스 함수는 분류해야하는 정답지(클래스)의 총 개수를 k라고 할 때, 
k차원의 벡터를 입력받아 각 클래스에 대한 확률을 추정합니다. 우선 수식에 대해 설명하고, 그 후에는 그림으로 이해해보겠습니다.

(로지스틱 함수 = 0~1사이로좁혀주는데 이진분류 기준 = 시그모이드
소프트맥스 함수 = 0~1사이로좁혀주는데 다부류 기준
활성화 함수가 소프트, 로지스틱의 상위 개념 :
그 외 ReLU(RV => 시그모이드 대체 ; 기울기소멸문제 대응) , 
Tanh 등도 활성화 함수의 종류.. 부록 용어정리 참조.)

### 1) 소프트맥스 함수의 이해

k차원의 벡터에서 i번째 원소를 $z_i$, i번째 클래스가 정답일 확률을 $p_i$로 나타낸다고 하였을 때 소프트맥스 함수는 $p_i$를 다음과 같이 정의합니다.

$$p_i = \frac{e^{z_i}}{\sum_{j=1}^{k} e^{z_j}} \quad \text{for } i = 1, 2, \ldots, k
$$
위에서 풀어야 하는 문제에 소프트맥스 함수를 차근차근 적용해 봅시다. 
위에서 풀어야하는 문제의 경우 k=3이므로 3차원 벡터 $z = \begin{bmatrix} z_1 & z_2 & z_3 \end{bmatrix}$의 입력을 받으면 소프트맥스 함수는 아래와 같은 출력을 리턴합니다.

$$softmax(z) = \left[ \frac{e^{z_1}}{\sum_{j=1}^{3} e^{z_j}} \;\; \frac{e^{z_2}}{\sum_{j=1}^{3} e^{z_j}} \;\; \frac{e^{z_3}}{\sum_{j=1}^{3} e^{z_j}} \right] = [p_1, p_2, p_3] = \hat{y}
= \text{예측값} $$

 각각은 1번 클래스가 정답일 확률, 2번 클래스가 정답일 확률, 3번 클래스가 정답일 확률을 나타내며 각각 0과 1사이의 값으로 총 합은 1이 됩니다. 여기서 분류하고자하는 3개의 클래스는 virginica, setosa, versicolor이므로 이는 결국 주어진 입력이 virginica일 확률, setosa일 확률, versicolor일 확률을 나타내는 값을 의미합니다. 여기서는 i가 1일 때는 virginica일 확률을 나타내고, 2일 때는 setosa일 확률, 3일때는 versicolor일 확률이라고 지정하였다고 합시다. 이 지정 순서는 문제를 풀고자 하는 사람의 무작위 선택입니다. 이에따라 식을 문제에 맞게 다시 쓰면 아래와 같습니다.

$$softmax(z) = \left[ \frac{e^{z_1}}{\sum_{j=1}^{3} e^{z_j}} \;\; \frac{e^{z_2}}{\sum_{j=1}^{3} e^{z_j}} \;\; \frac{e^{z_3}}{\sum_{j=1}^{3} e^{z_j}} \right] = [p_1, p_2, p_3] =  [P_{virginica}, \; p_{setosa}, \; p_{versico\lor}]$$


다소 복잡해보이지만 어려운 개념이 아닙니다. 분류하고자 하는 클래스가 k개일 때, k차원의 벡터를 입력받아서 모든 벡터 원소의 값을 0과 1사이의 값으로 값을 변경하여 다시 k차원의 벡터를 리턴한다는 내용을 식으로 기재하였을 뿐입니다. 

방금 배운 개념을 그림을 통해 다시 설명하면서 더 깊이 들어가보겠습니다.


### 2) 그림을 통한 이해

![](https://static.wikidocs.net/images/page/35476/softmax1_final_final.PNG)

위의 그림에 점차 살을 붙여봅시다. 여기서는 샘플 데이터를 1개씩 입력으로 받아 처리한다고 가정해봅시다. 즉, 배치 크기가 1입니다.

위의 그림에는 두 가지 질문이 있습니다. 

첫번째 질문은 소프트맥스 함수의 입력에 대한 질문입니다. 
하나의 샘플 데이터는 4개의 독립 변수 $x$를 가지는데 이는 모델이 4차원 벡터를 입력으로 받음을 의미합니다. 그런데 소프트맥스의 함수의 입력으로 사용되는 벡터는 벡터의 차원이 분류하고자 하는 클래스의 개수가 되어야 하므로 어떤 가중치 연산을 통해 << 3차원 벡터로 변환되어야 합니다. >> 
위의 그림에서는 소프트맥스 함수의 입력으로 사용되는 3차원 벡터를 $z$로 표현하였습니다.

![](https://static.wikidocs.net/images/page/35476/softmaxbetween1and2.PNG)

샘플 데이터 벡터를 소프트맥스 함수의 입력 벡터로 차원을 축소하는 방법은 간단합니다. 소프트맥스 함수의 << 입력 벡터 $z$의 차원수만큼 결과값의 나오도록 가중치 곱을 진행 >> 합니다. 위의 그림에서 화살표는 총 (4 × 3 = 12) 12개이며 전부 다른 가중치를 가지고, 학습 과정에서 점차적으로 오차를 최소화하는 가중치로 값이 변경됩니다.
=> 최종 확률 값은 당연히 3개가 나와 한다. 즉, 소프트맥스에 들어가는 입력벡터인 z라는 애도 반드시 3차원 벡터여야 한다는 것.
이렇게 봤을 떄 4차원의 입력데이터 x를 z로 대응시키려면? 모든 입력의 영향을 받도록 모든 입력 * 가중치의 sum으로 하게 하되, z의 차원 자체는 3차원으로 하도록 하는 것. (화살표 개수가 이래서 입력개수 * 소프트맥스 함수 입력개수(차원)인 것.)



두번째 질문은 오차 계산 방법에 대한 질문입니다. 
소프트맥스 함수의 출력은 분류하고자하는 클래스의 개수만큼 차원을 가지는 벡터로 각 원소는 0과 1사이의 값을 가집니다. 이 각각은 특정 클래스가 정답일 확률을 나타냅니다. 여기서는 첫번째 원소인 $p_1$은 virginica가 정답일 확률, 두번째 원소인 $p_2$는 setosa가 정답일 확률, 세번째 원소인 $p_3$은 versicolor가 정답일 확률로 고려하고자 합니다. 

그렇다면 이 예측값과 비교를 할 수 있는 << 실제값의 표현 방법 >> 이 있어야 합니다. 소프트맥스 회귀에서는 실제값을 원-핫 벡터로 표현합니다.

![](https://static.wikidocs.net/images/page/35476/softmax2_final.PNG)

위의 그림은 소프트맥스 함수의 출력 벡터의 첫번째 원소 $p_1$가 virginica가 정답일 확률, 두번째 원소 $p_2$가 setosa가 정답일 확률, 세번째 원소 $p_3$가 versicolor가 정답일 확률을 의미한다고 하였을 때, 각 실제값의 정수 인코딩은 1, 2, 3이 되고 이에 원-핫 인코딩을 수행하여 실제값을 원-핫 벡터로 수치화한 것을 보여줍니다.

![](https://static.wikidocs.net/images/page/35476/softmax4.PNG)

예를 들어 현재 풀고 있는 샘플 데이터의 실제값이 setosa라면
setosa의 원-핫 벡터는 `[0 1 0]`입니다. 
이 경우, 예측값과 실제값의 오차가 0이 되는 경우는 소프트맥스 함수의 결과가 `[0 1 0]`이 되는 경우입니다. << 이 두 벡터의 오차를 계산하기 위해서 소프트맥스 회귀는 비용 함수로 크로스 엔트로피 함수를 사용 >> 하는데, 이는 뒤에서 비용 함수를 설명하는 부분에서 다시 언급하겠습니다.

![](https://static.wikidocs.net/images/page/35476/softmax5.PNG)

이제 앞서 배운 선형 회귀나 로지스틱 회귀와 마찬가지로 
오차로부터 가중치를 업데이트 합니다.

![](https://static.wikidocs.net/images/page/35476/softmax6_final.PNG)

더 정확히는 선형 회귀나 로지스틱 회귀와 마찬가지로 편향 또한 업데이트의 대상이 되는 매개 변수입니다. 소프트맥스 회귀를 벡터와 행렬 연산으로 이해해봅시다.

입력을 특성(feature)의 수만큼의 차원을 가진 입력 벡터 $x$라고 하고, 
가중치 행렬을 $W$ , 편향을 $b$ 라고 하였을 때, 소프트맥스 회귀에서 예측값을 구하는 과정을 벡터와 행렬 연산으로 표현하면 아래와 같습니다.

![](https://static.wikidocs.net/images/page/59427/%EA%B0%80%EC%84%A4.PNG)

여기서 $f$는 특성의 수이며 $c$는 클래스의 개수에 해당됩니다.
(W는 클래스의 수가 상위, 베이스 : 예측해야하는 값의 수니까.로 c로 들어가고,
각 클래스별로 f개의 특성이 있으니 c x f. X라는 입력은 각 블럭이 특성 그 자체이니 f x 1. (특성 내부의 속성이 없음.) B는 클래스-종류별로 붙는 편향이니 c x 1.
예측값은 이 결과로 - 상위 베이스인 클래스 수만큼의 c x 1 벡터꼴로 나옴)


## 3. 붓꽃 품종 분류하기 행렬 연산으로 이해하기

위의 붓꽃 품종 분류 문제의 가설식을 행렬 연산으로 표현해보겠습니다. 
우선 위의 예제의 데이터는 전체 샘플의 개수가 5개, 특성이 4개이므로 
5 × 4 행렬 $X$ 로 정의합니다.

![[Pasted image 20260710132940.png|178]]

편의를 위해 각 행렬의 원소 위치를 반영한 변수로 표현하겠습니다.

![[Pasted image 20260710132957.png|179]]

이번 문제는 선택지가 총 3개인 문제이므로 (예측값구조:열 = 클래스 종류 3개)
가설의 예측값으로 얻는 행렬 $\hat{Y}$ 의 열의 개수는 3개여야 합니다. 
그리고 각 행은 행렬 $X$ 의 각 행의 예측값이므로 (행 = 각 예측값. 특성으로부터 추츨한 각 클래스에 해당할 확률. 아래 예시 참조)
행의 크기는 동일해야 합니다. 
결과적으로 행렬 $\hat{Y}$의 크기는 5 × 3입니다.

```
              setosa   versicolor  virginica    ← 열(3개) = 클래스
샘플1(5.1..)  [ 0.7      0.2         0.1  ]      ← 이 행의 합 = 1
샘플2(4.9..)  [ 0.6      0.3         0.1  ]
샘플3(5.8..)  [ 0.1      0.8         0.1  ]
샘플4(6.7..)  [ 0.1      0.2         0.7  ]
샘플5(5.6..)  [ 0.2      0.1         0.7  ]
                ↑ 행(5개) = 샘플

```

![[Pasted image 20260710133006.png|250]]

크기 5 × 3의 행렬  $\hat{Y}$는 
크기 5 × 4 입력 행렬 $S$, 가중치 행렬 $W$ 곱으로  ($H(x) = softmax(WX+B)) = \hat{Y}$)
얻어지는 행렬이므로 가중치 행렬 $W$의 크기는 추정을 통해 4 × 3의 크기를 가진 행렬임을 알 수 있습니다.

![[Pasted image 20260710133017.png|302]]

편향 행렬 는 예측값 행렬 와 크기가 동일해야 하므로 5 × 3의 크기를 가집니다.

![[Pasted image 20260710133026.png|125]]

결과적으로 가설식은 다음과 같습니다.

![[Pasted image 20260710133043.png]]




## 4. 비용 함수(Cost function)

소프트맥스 회귀에서는 비용 함수로 크로스 엔트로피 함수를 사용합니다. 
여기서는 소프트맥스 회귀에서의 크로스 엔트로피 함수뿐만 아니라, 
다양한 표기 방법에 대해서 이해해보겠습니다.

### 1) 크로스 엔트로피 함수

아래에서 $y$는 실제값(정답)을 나타내며, $k$는 클래스의 개수로 정의합니다. 
$y_j$는 실제값 원-핫 벡터의 $j$번째 인덱스를 의미하며, $p_j$는 샘플 데이터가 $j$번째 클래스일 확률을 나타냅니다. 표기에 따라서 $\hat{y}_j$로 표현하기도 합니다.

$$cost(W) = -\sum_{j=1}^{k} y_j \log(p_j)$$

이 함수가 왜 비용 함수로 적합한지 알아보겠습니다. 
$c$가 실제값 원-핫 벡터에서 1을 가진 원소의 인덱스라고 한다면, $p_c = 1$은 $\hat{y}$가 $y$를 정확하게 예측한 경우가 됩니다. 이를 식에 대입해보면 $-1 \log(1) = 0$이 되기 때문에, 결과적으로 $\hat{y}$가 $y$를 정확하게 예측한 경우의 크로스 엔트로피 함수의 값은 0이 됩니다. 즉, $-\sum_{j=1}^{k} y_j \log(p_j)$ 이 값을 최소화하는 방향으로 학습해야 합니다.

이제 이를 $n$개의 전체 데이터에 대한 평균을 구한다고 하면 
최종 비용 함수는 다음과 같습니다.

$$cost(W) = -\frac{1}{n} \sum_{i=1}^{n} \sum_{j=1}^{k} y_j^{(i)} \log(p_j^{(i)})$$

### 2) 이진 분류에서의 크로스 엔트로피 함수

로지스틱 회귀에서 배운 크로스 엔트로피 함수식과 달라 보이지만, 
본질적으로는 동일한 함수식입니다. 
로지스틱 회귀의 크로스 엔트로피 함수식으로부터 
소프트맥스 회귀의 크로스 엔트로피 함수식을 도출해봅시다.

$$cost(W) = -(y \log H(X) + (1 - y) \log(1 - H(X)))$$

위의 식은 앞서 로지스틱 회귀에서 배웠던 크로스 엔트로피의 함수식을 보여줍니다.
=> [[04 Logistic Regression#^cost]] 참조, 복습 필수.

위의 식에서 $y$를 $y_1$, $1 - y$를 $y_2$로 치환하고 
$H(X)$를 $p_1$, $1 - H(X)$를 $p_2$로 치환해봅시다. 결과적으로 아래의 식을 얻을 수 있습니다.

$$-(y_1 \log(p_1) + y_2 \log(p_2))$$

이 식은 아래와 같이 표현할 수 있습니다.

$$-\left(\sum_{i=1}^{2} y_i \log p_i\right)$$

소프트맥스 회귀에서는 k의 값이 고정된 값이 아니므로 2를 k로 변경합니다.

$$-\left(\sum_{i=1}^{k} y_i \log p_i\right)$$

위의 식은 결과적으로 소프트맥스 회귀의 식과 동일합니다. 
역으로 소프트맥스 회귀에서 로지스틱 회귀의 크로스 엔트로피 함수식을 얻는 것은 $k$를 2로 하고, $y_1$과 $y_2$를 각각 $y$와 $1 - y$로 치환하고, $p_1$과 $p_2$를 각각 $H(X)$와 $1 - H(X)$로 치환하면 됩니다.

정리하면 소프트맥스 함수의 최종 비용 함수에서 $k$가 2라고 가정하면 결국 로지스틱 회귀의 비용 함수와 같습니다.

$$cost(W) = -\frac{1}{n} \sum_{i=1}^{n} \sum_{j=1}^{k} y_j^{(i)} \log(p_j^{(i)}) = -\frac{1}{n} \sum_{i=1}^{n} \left[ y^{(i)} \log(p^{(i)}) + (1 - y^{(i)}) \log(1 - p^{(i)}) \right]$$


# 05-03 소프트맥스 회귀 다양한 방법으로 구현

이번 챕터에서는 소프트맥스 회귀의 비용 함수를 직접 구현하여 크로스 엔트로피 함수에 대해서 이해하고, 파이토치로 소프트맥스 회귀를 로우 레벨, 하이 레벨, nn.Module, 클래스를 사용한 방법 등 가능한 한 다양한 방법으로 구현해보겠습니다.

## 1. 소프트맥스 회귀의 비용 함수 구현

앞으로의 모든 실습은 아래의 코드가 이미 진행되었다고 가정합니다.

```python
import torch
import torch.nn.functional as F
```

```python
torch.manual_seed(1)
```

### 1) 파이토치로 소프트맥스의 비용 함수 구현하기 (로우-레벨)

소프트맥스 회귀를 구현함에 있어 우선 
소프트맥스 함수의 비용 함수를 로우-레벨로 구현해봅시다.  
3개의 원소를 가진 벡터 텐서를 정의하고, 
이 텐서를 통해 소프트맥스 함수를 이해해보겠습니다.

```python
z = torch.FloatTensor([1, 2, 3])
```

이 텐서를 소프트맥스 함수의 입력으로 사용하고, 그 결과를 확인해보겠습니다.

```python
hypothesis = F.softmax(z, dim=0)
print(hypothesis)
```

```python
tensor([0.0900, 0.2447, 0.6652])
```

3개의 원소의 값이 0과 1사이의 값을 가지는 벡터로 변환된 것을 볼 수 있습니다. 이 원소들의 값의 합이 1인지 확인해보겠습니다.

```python
hypothesis.sum()
```

```python
tensor(1.)
```

총 원소의 값의 합은 1입니다. 

이번에는 비용 함수를 직접 구현해보겠습니다. 
우선 임의의 3 × 5 행렬의 크기를 가진 텐서를 만듭니다.

```python
z = torch.rand(3, 5, requires_grad=True) # (3,5) 0~1사이 무작위 tensor
```

이제 이 텐서에 대해서 소프트맥스 함수를 적용합니다. 
단, << 각 샘플에 대해서 >> 소프트맥스 함수를 적용하여야 하므로 
두번째 차원에 대해서 소프트맥스 함수를 적용한다는 의미에서 dim=1을 써줍니다.

< 데이터 형식 RV > 
=> 적용해야 하는 기준은, 각 "열"에 대해! (각 샘플의 특성에 대해서 적용해야 함.)
```
              setosa   versicolor  virginica    ← 열(3개) = 클래스
샘플1(5.1..)  [ 0.7      0.2         0.1  ]      ← 이 행의 합 = 1
샘플2(4.9..)  [ 0.6      0.3         0.1  ]
샘플3(5.8..)  [ 0.1      0.8         0.1  ]
샘플4(6.7..)  [ 0.1      0.2         0.7  ]
샘플5(5.6..)  [ 0.2      0.1         0.7  ]
                ↑ 행(5개) = 샘플
```

```python
hypothesis = F.softmax(z, dim=1)
print(hypothesis)
```

```python
tensor([[0.2645, 0.1639, 0.1855, 0.2585, 0.1277],
        [0.2430, 0.1624, 0.2322, 0.1930, 0.1694],
        [0.2226, 0.1986, 0.2326, 0.1594, 0.1868]], grad_fn=<SoftmaxBackward>)
```

이제 각 행의 원소들의 합은 1이 되는 텐서로 변환되었습니다. 
소프트맥스 함수의 출력값은 결국 예측값입니다. 
즉, 위 텐서는 3개의 샘플에 대해서 5개의 클래스 중 어떤 클래스가 정답인지를 예측한 결과입니다.

이제 각 샘플에 대해서 임의의 레이블을 만듭니다.

```python
y = torch.randint(5, (3,)).long()
print(y)
```

```python
tensor([0, 2, 1])
```

이제 각 레이블에 대해서 원-핫 인코딩을 수행합니다.

```python
# 모든 원소가 0의 값을 가진 3 × 5 텐서 생성
y_one_hot = torch.zeros_like(hypothesis) 
y_one_hot.scatter_(1, y.unsqueeze(1), 1)
```

```python
tensor([[1., 0., 0., 0., 0.],
        [0., 0., 1., 0., 0.],
        [0., 1., 0., 0., 0.]])
```

위의 연산에서 어떻게 원-핫 인코딩이 수행되었는지 보겠습니다. 우선, torch.zeros_like(hypothesis)를 통해 모든 원소가 0의 값을 가진 3 × 5 텐서를 만듭니다. 
그리고 이 텐서는 y_one_hot에 저장이 된 상태입니다.

두번째 줄을 해석해봅시다. y.unsqueeze(1)를 하면 (3,)의 크기를 가졌던 y 텐서는 (3 × 1) 텐서가 됩니다. 즉, 다시 말해서 y.unsqueeze(1)의 결과는 아래와 같습니다.

```python
print(y.unsqueeze(1))
```

```python
tensor([[0],
        [2],
        [1]])
```

그리고 scatter의 첫번째 인자로 dim=1에 대해서 수행하라고 알려주고, 
세번째 인자에 숫자 1을 넣어주므로서 두번째 인자인 y_unsqeeze(1)이 알려주는 위치에 숫자 1을 넣도록 합니다. 

#### scatter 함수
`scatter` 함수는 원본 텐서(`input`)의 값을 바꾸는 것이 아니라, **특정 인덱스에 따라 값을 채운 새로운 텐서**를 반환합니다. (사실 `scatter_`처럼 뒤에 언더바(`_`)가 붙으면 원본을 직접 수정합니다 = 덮어쓰기 연산)
**문법:** `input.scatter_(dim, index, src)`

- **`dim`**: 값을 채울 방향 (0은 행 방향, 1은 열 방향).
- **`index`**: 값을 넣을 위치 정보 (텐서 형태여야 함).
- **`src`**: 삽입할 값 (숫자 하나이거나 텐서).

=> 쉽게 말하면 특정 index 에 src값을 채워주는 함수.



앞서 텐서 조작하기 2챕터에서 연산 뒤에 \_를 붙이면 In-place Operation (덮어쓰기 연산)임을 배운 바 있습니다. 이에 따라서 y_one_hot의 최종 결과는 결국 아래와 같습니다.

```python
print(y_one_hot)
```

```python
tensor([[1., 0., 0., 0., 0.],
        [0., 0., 1., 0., 0.],
        [0., 1., 0., 0., 0.]])
```

이제 비용 함수 연산을 위한 재료들을 전부 손질했습니다. 
소프트맥스 회귀의 비용 함수는 다음과 같았습니다.
$$cost(W) = -\frac{1}{n} \sum_{i=1}^{n} \sum_{j=1}^{k} y_j^{(i)} \log(p_j^{(i)})$$
마이너스 부호를 뒤로 빼면 다음 식과도 동일합니다.
$$cost(W) = \frac{1}{n} \sum_{i=1}^{n} \sum_{j=1}^{k} y_j^{(i)} \times (-\log(p_j^{(i)}))$$

이를 코드로 구현하면 아래와 같습니다. 
$\sum_{j=1}^{k}$는 sum(dim=1)으로 구현하고, $\frac{1}{n}\sum_{i=1}^{n}$는 mean()으로 구현합니다.

```python
cost = (y_one_hot * -torch.log(hypothesis)).sum(dim=1).mean()
print(cost)
```

```python
tensor(1.4689, grad_fn=<MeanBackward1>)
```

### 2) 파이토치로 소프트맥스의 비용 함수 구현하기 (하이-레벨)

이제 소프트맥스의 비용 함수를 좀 더 하이-레벨로 구현하는 방법에 대해서 알아봅시다.


#### 2-1) F.softmax() + torch.log() = F.log_softmax()

앞서 소프트맥스 함수의 결과에 로그를 씌울 때는 
다음과 같이 소프트맥스 함수의 출력값을 로그 함수의 입력으로 사용했습니다.

```python
# Low level
torch.log(F.softmax(z, dim=1))
```

```python
tensor([[-1.3301, -1.8084, -1.6846, -1.3530, -2.0584],
        [-1.4147, -1.8174, -1.4602, -1.6450, -1.7758],
        [-1.5025, -1.6165, -1.4586, -1.8360, -1.6776]], grad_fn=<LogBackward>)
```

그런데 파이토치에서는 두 개의 함수를 결합한 F.log_softmax()라는 도구를 제공합니다.

```python
# High level
F.log_softmax(z, dim=1)
```

```python
tensor([[-1.3301, -1.8084, -1.6846, -1.3530, -2.0584],
        [-1.4147, -1.8174, -1.4602, -1.6450, -1.7758],
        [-1.5025, -1.6165, -1.4586, -1.8360, -1.6776]], grad_fn=<LogSoftmaxBackward>)
```

두 출력 결과가 동일한 것을 볼 수 있습니다. 이제 비용 함수를 보겠습니다.

#### 2-2) F.log_softmax() + F.nll_loss() = F.cross_entropy()

앞서 로우-레벨로 구현한 비용 함수는 다음과 같았습니다.

```python
# Low level
# 첫번째 수식
(y_one_hot * -torch.log(F.softmax(z, dim=1))).sum(dim=1).mean()
```

```python
tensor(1.4689, grad_fn=<MeanBackward1>)
```

그런데 위의 수식에서 torch.log(F.softmax(z, dim=1))를 
방금 배운 F.log_softmax()로 대체할 수 있습니다.

```python
# 두번째 수식
(y_one_hot * - F.log_softmax(z, dim=1)).sum(dim=1).mean()
```

```python
tensor(1.4689, grad_fn=<MeanBackward0>)
```

이를 더 간단하게 하면 다음과 같습니다. 
F.nll_loss()를 사용할 때는 << 원-핫 벡터를 넣을 필요 없이 >>
바로 실제값을 인자로 사용합니다. => 수식 자체를 포함한 함수인 것.

```python
# High level
F.nll_loss(F.log_softmax(z, dim=1), y)
```

```python
tensor(1.4689, grad_fn=<NllLossBackward>)
```

여기서 nll이란 Negative Log Likelihood의 약자입니다. 
위에서 nll_loss는 F.log_softmax()를 수행한 후에 남은 수식들을 수행합니다. 
이를 더 간단하게 하면 다음과 같이 사용할 수 있습니다. 
F.cross_entropy()는 F.log_softmax()와 F.nll_loss()를 포함하고 있습니다.

```python
# 네번째 수식
F.cross_entropy(z, y)
```

```python
tensor(1.4689, grad_fn=<NllLossBackward>)
```

- **여기서 잠깐! F.cross_entropy는 비용 함수에 <<소프트맥스 함수까지 포함>>하고 있음을 기억하고 있어야 구현 시 혼동하지 않습니다.**

이제 소프트맥스 회귀를 로우-레벨과 F.cross_entropy를 사용해서 구현해보겠습니다.

#### 2-3) nn.CrossEntropyLoss(): 클래스를 이용한 구현 방식

```python
nn.CrossEntropyLoss()(z, y)
```

```python
tensor(1.4689, grad_fn=<NllLossBackward0>)
```

**F.cross_entropy()는 함수입니다.**

- 호출할 때마다 F.cross_entropy(입력, 정답) 형태로 사용
- 매번 << 호출시 필요한 설정을 인자로 전달해야 함 >>

**nn.CrossEntropyLoss()는 클래스입니다.**

- 클래스는 설정값들을 저장할 수 있는 틀(template)
- 클래스로부터 실제 사용할 수 있는 객체(instance)를 생성해야 함
- 객체를 만들 때 << 설정값들을 미리 정해두고, 나중에 계속 재사용 가능 >>

위에서 nn.CrossEntropyLoss()(z, y)는 클래스로 객체를 생성함과 동시에 
바로 호출한 것입니다. 실제로는 아래와 같이 사용하는 경우가 보편적입니다.

```python
# 1단계: 클래스로 객체 생성
criterion = nn.CrossEntropyLoss()

# 2단계: 생성된 객체 사용
loss = criterion(z, y)
print(loss)
```

```python
tensor(1.4689, grad_fn=<NllLossBackward0>)
```

클래스를 사용하는 주요 이유는 
<< 설정값을 객체에 저장해두고 나중에 사용할 수 있기 >> = 재사용성 때문입니다.

<< 함수는 매번 호출할 때마다 모든 설정을 인자로 전달해야 하지만, 클래스는 객체를 만들 때 한 번만 설정하면 됩니다. >> 
이는 코드를 더 깔끔하게 만들어주고, 같은 설정을 반복해서 쓸 때 실수를 줄여줍니다.

```python
# 손실함수 객체를 한 번만 생성. 이제 호출할때는 무조건 criterion으로만 호출함.
criterion = nn.CrossEntropyLoss()

# 같은 객체로 여러 번 계산 가능
loss1 = criterion(z, y)           # 첫 번째 계산
loss2 = criterion(z, y)           # 두 번째 계산

# 새로운 데이터가 있다면
z2 = torch.rand(3, 5, requires_grad=True)
y2 = torch.randint(5, (3,)).long()
loss3 = criterion(z2, y2)         # 새 데이터로 계산
```

특별한 설정이 필요한 경우를 살펴보겠습니다. 
예를 들어 손실값의 평균 대신 합계가 필요한 경우입니다.

```python
# 평균 대신 합계를 구하는 설정
# 손실함수 객체를 한 번만 생성. 이제 호출할때는 무조건 criterion으로만 호출함.
criterion = nn.CrossEntropyLoss(reduction='sum')

# 같은 객체로 여러 번 계산 가능
loss1 = criterion(z, y)           # 첫 번째 계산
loss2 = criterion(z, y)           # 두 번째 계산

# 새로운 데이터가 있다면
z2 = torch.rand(3, 5, requires_grad=True)
y2 = torch.randint(5, (3,)).long()
loss3 = criterion(z2, y2)         # 새 데이터로 계산
```

만약 함수로 같은 작업을 한다면 매번 reduction='sum'을 써줘야 합니다.

```python
# 함수 방식에서는 매번 설정을 반복해야 함
loss_sum1 = F.cross_entropy(z, y, reduction='sum')
loss_sum2 = F.cross_entropy(z2, y2, reduction='sum')  # 설정 반복
```

- **여기서 잠깐! nn.CrossEntropyLoss()도 F.cross_entropy()와 마찬가지로 비용 함수에 소프트맥스 함수까지 포함하고 있음을 기억하고 있어야 구현 시 혼동하지 않습니다.**

## 2. 소프트맥스 회귀 구현하기 

### 1) 데이터셋 준비

앞으로의 모든 실습은 아래의 과정이 이미 진행되었다고 가정합니다.

필요한 도구들을 임포트합니다.

```python
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
```

```python
torch.manual_seed(1)
```

훈련 데이터와 레이블을 텐서로 선언합니다.

```python
x_train = [[1, 2, 1, 1],
           [2, 1, 3, 2],
           [3, 1, 3, 4],
           [4, 1, 5, 5],
           [1, 7, 5, 5],
           [1, 2, 5, 6],
           [1, 6, 6, 6],
           [1, 7, 7, 7]]
y_train = [2, 2, 2, 1, 1, 1, 0, 0]
x_train = torch.FloatTensor(x_train)
y_train = torch.LongTensor(y_train)
```

x_train의 각 샘플은 4개의 특성을 가지고 있으며, 총 8개의 샘플이 존재합니다. y_train은 각 샘플에 대한 레이블인데, 
여기서는 0, 1, 2의 값을 가지는 것으로 보아 총 3개의 클래스가 존재합니다.

### 2) 소프트맥스 회귀 구현하기(로우-레벨)

이제 x_train의 크기와 y_train의 크기를 확인합니다.

```python
print(x_train.shape)
print(y_train.shape)
```

```python
torch.Size([8, 4])
torch.Size([8])
```

x_train의 크기는 8 × 4이며, y_train의 크기는 8 × 1입니다. 
그런데 최종 사용할 레이블은 y_train에서 원-핫 인코딩을 한 결과이어야 합니다. 
클래스의 개수는 3개이므로 y_train에 원-핫 인코딩한 결과는 8 × 3의 개수를 가져야 합니다.

```python
y_one_hot = torch.zeros(8, 3)
y_one_hot.scatter_(1, y_train.unsqueeze(1), 1)
print(y_one_hot.shape)
```

```python
torch.Size([8, 3])
```

y_train에서 원-핫 인코딩을 한 결과인 y_one_hot의 크기는 8 × 3입니다. 
즉, W 행렬의 크기는 4 × 3이어야 합니다.  
W와 b를 선언하고, 옵티마이저로는 경사 하강법을 사용합니다. 
그리고 학습률은 0.1로 설정합니다.

```python
# 모델 초기화
W = torch.zeros((4, 3), requires_grad=True)
b = torch.zeros((1, 3), requires_grad=True)
# optimizer 설정
optimizer = optim.SGD([W, b], lr=0.1)
```

nb_epochs를 1000으로 설정하여 학습을 1000번 반복할 것을 지정합니다.

```python
nb_epochs = 1000
for epoch in range(nb_epochs + 1):

    # 가설
    hypothesis = F.softmax(x_train.matmul(W) + b, dim=1) 

    # 비용 함수
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
```

for 루프 내부에서, 입력 데이터 x_train과 가중치 행렬 W, 그리고 편향 벡터 b를 사용하여 가설(hypothesis)을 계산합니다. 
이때 소프트맥스 함수(F.softmax)를 사용해 각 클래스에 대한 예측 확률을 구합니다.

그 다음으로 비용 함수(cost)를 계산하는데, 이 비용 함수는 교차 엔트로피 손실 함수와 유사합니다. 원-핫 인코딩된 실제 레이블 y_one_hot과 가설의 로그값을 곱한 뒤, 각 데이터 포인트별 손실을 계산하고 이를 평균내어 최종 비용을 구합니다.

계산된 비용 함수를 최소화하기 위해, 먼저 옵티마이저의 기울기 정보를 초기화합니다. 그런 다음, 비용 함수에 대해 역전파(backward)를 수행하여 가중치와 편향에 대한 기울기를 계산합니다. 마지막으로 옵티마이저의 step()을 호출하여 가중치와 편향을 업데이트합니다.

매 100번째 에포크마다 현재 에포크 번호와 비용 함수 값을 출력하여 학습 진행 상황을 모니터링합니다.

### 3) 소프트맥스 회귀 구현하기(하이-레벨)

이제는 F.cross_entropy()를 사용하여 비용 함수를 구현해보겠습니다. 주의할 점은 F.cross_entropy()는 그 자체로 소프트맥스 함수를 포함하고 있으므로 가설에서는 소프트맥스 함수를 사용할 필요가 없습니다. 위와 동일한 x_train과 y_train을 사용합니다.

```python
# 모델 초기화
W = torch.zeros((4, 3), requires_grad=True)
b = torch.zeros((1, 3), requires_grad=True)
# optimizer 설정
optimizer = optim.SGD([W, b], lr=0.1)

nb_epochs = 1000
for epoch in range(nb_epochs + 1):

    # Cost 계산
    z = x_train.matmul(W) + b
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
```

F.cross_entropy()를 사용하는 것 외에는 
위의 코드와 동일하므로 설명은 생략하겠습니다.

### 4) 소프트맥스 회귀 nn.Module로 구현하기

이번에는 nn.Module로 소프트맥스 회귀를 구현해봅시다. 
선형 회귀에서 구현에 사용했던 nn.Linear()를 사용합니다. 
output_dim이 1이었던 선형 회귀때와 달리 
output_dim은 이제 클래스의 개수여야 합니다.

```python
# 모델을 선언 및 초기화. 4개의 특성을 가지고 3개의 클래스로 분류. input_dim=4, output_dim=3.
model = nn.Linear(4, 3)
```

아래에서 << F.cross_entropy()를 사용할 것이므로 
따로 소프트맥스 함수를 가설에 정의하지 않습니다. >> (포함됨)

```python
# optimizer 설정
optimizer = optim.SGD(model.parameters(), lr=0.1)

nb_epochs = 1000
for epoch in range(nb_epochs + 1):

    # H(x) 계산
    prediction = model(x_train)

    # cost 계산
    cost = F.cross_entropy(prediction, y_train)

    # cost로 H(x) 개선
    optimizer.zero_grad()
    cost.backward()
    optimizer.step()

    # 20번마다 로그 출력
    if epoch % 100 == 0:
        print('Epoch {:4d}/{} Cost: {:.6f}'.format(
            epoch, nb_epochs, cost.item()
        ))
```

### 5) 소프트맥스 회귀 클래스로 구현하기

이제 소프트맥스 회귀를 nn.Module을 상속받은 클래스로 구현해봅시다. 
먼저, SoftmaxClassifierModel 클래스를 정의합니다. 
이 클래스는 PyTorch의 nn.Module을 상속받아 정의된 신경망 모델입니다. 

클래스의 **init** 메서드에서 nn.Linear를 사용해 입력 차원이 4이고 출력 차원이 3인 선형 계층(Linear layer or nn.Linear)을 정의합니다. 
여기서 출력 차원이 3인 이유는 모델이 3개의 클래스를 예측하기 때문입니다. forward 메서드는 모델의 순전파(forward) 과정을 정의합니다.

```python
class SoftmaxClassifierModel(nn.Module):
    def __init__(self):
        super().__init__()
        self.linear = nn.Linear(4, 3) # Output이 3!

    def forward(self, x):
        return self.linear(x)
```

모델 인스턴스를 생성하고 SGD 옵티마이저를 설정합니다. 
옵티마이저는 모델의 파라미터를 입력받아 학습률 0.1로 경사 하강법을 수행합니다.

```python
model = SoftmaxClassifierModel()

# optimizer 설정
optimizer = optim.SGD(model.parameters(), lr=0.1)
```

다음으로, nb_epochs를 1000으로 설정하여 학습을 1000번 반복할 것을 지정합니다. for 루프 내에서 에포크를 순차적으로 실행합니다.

```python
nb_epochs = 1000
for epoch in range(nb_epochs + 1):

    # H(x) 계산
    prediction = model(x_train)

    # cost 계산
    cost = F.cross_entropy(prediction, y_train)

    # cost로 H(x) 개선
    optimizer.zero_grad()
    cost.backward()
    optimizer.step()

    # 20번마다 로그 출력
    if epoch % 100 == 0:
        print('Epoch {:4d}/{} Cost: {:.6f}'.format(
            epoch, nb_epochs, cost.item()
        ))
```

각 에포크에서 모델의 예측값(prediction)을 계산하기 위해, 입력 데이터 x_train을 모델에 입력하여 예측값을 얻습니다. 이 예측값은 아직 소프트맥스 함수를 거치지 않은 상태입니다.

그 다음, F.cross_entropy() 함수를 사용하여 비용 함수 cost를 계산합니다. 이 함수는 소프트맥스 << 활성화 함수와 교차 엔트로피 손실 함수를 결합한 형태 >> 로, 모델의 예측값과 실제 레이블 y_train을 비교하여 비용을 계산합니다.

비용 함수 cost를 최소화하기 위해 옵티마이저의 기울기 정보를 초기화한 후, 역전파를 수행하여 기울기를 계산합니다. 그런 다음, 옵티마이저의 step()을 호출하여 모델의 파라미터를 업데이트합니다.

마지막으로, 매 100번째 에포크마다 현재 에포크 번호와 비용 함수 값을 출력하여 학습 진행 상황을 모니터링합니다. 이를 통해 모델이 점차적으로 학습되고 있는지 확인할 수 있습니다.

# 05-04 소프트맥스 회귀로 MNIST 데이터 분류

이번 챕터에서는 MNIST 데이터에 대해서 이해하고, 
파이토치(PyTorch)로 소프트맥스 회귀를 구현하여 
MNIST 데이터를 분류하는 실습을 진행해봅시다.

MNIST 데이터는 아래의 링크에 공개되어져 있습니다.  
링크 : http://yann.lecun.com/exdb/mnist

## 1. MNIST 데이터 이해하기

![](https://static.wikidocs.net/images/page/60324/mnist.png)

MNIST는 숫자 0부터 9까지의 이미지로 구성된 손글씨 데이터셋입니다. 
이 데이터는 과거에 우체국에서 << 편지의 우편 번호를 인식하기 위해서 >> 만들어진 훈련 데이터입니다. 총 << 60,000개의 훈련 데이터와 레이블, 총 10,000개의 테스트 데이터와 레이블로 구성되어져 있습니다 >>. 
레이블은 0부터 9까지 총 10개입니다. 
이 예제는 머신 러닝을 처음 배울 때 접하게 되는 가장 기본적인 예제이기도 합니다.

MNIST 문제는 손글씨로 적힌 숫자 이미지가 들어오면, 
그 이미지가 무슨 숫자인지 맞추는 문제입니다. (이미지 => 숫자)
예를 들어 숫자 5의 이미지가 입력으로 들어오면 이게 숫자 5다! 라는 것을 맞춰야 합니다. 이 문제는 사람에게는 굉장히 간단하지만 기계에게는 그렇지가 않습니다.

우선 MNIST 문제를 더 자세히 보겠습니다. 
각각의 이미지는 아래와 같이 28 픽셀 × 28 픽셀의 이미지입니다.

![209](https://static.wikidocs.net/images/page/60324/mnist_SVbcYYG.png)

이 문제를 풀기 위해 여기서는 28 픽셀 × 28 픽셀 = 784 픽셀이므로, 
각 << 이미지를 총 784의 원소를 가진 벡터로 만들어줄겁니다. >> 이렇게 되면 총 784개의 특성을 가진 샘플이 되는데, 이는 앞서 우리가 풀었던 그 어떤 문제들보다 
<< 특성이 굉장히 많은 >> 샘플입니다. (1샘플당 784 feature)

![239](https://static.wikidocs.net/images/page/60324/%EB%8B%A4%EC%9A%B4%EB%A1%9C%EB%93%9C.png)

784차원의 벡터로 만드는 코드를 미리보기로 보면 아래와 같습니다.

```python
for X, Y in data_loader:
  # 입력 이미지를 [batch_size × 784]의 크기로 reshape
  # 레이블은 원-핫 인코딩
  X = X.view(-1, 28*28)
```

위의 코드에서 X는 for문에서 호출될 때는 (배치 크기 × 1 × 28 × 28)의 크기를 가지지만, << view를 통해서 (배치 크기 × 784)의 크기로 변환됩니다. >> 

view 함수 사용법 : 
```python
import torch

# 3x2 텐서 생성 (총 6개 요소)
x = torch.tensor([[1, 2], [3, 4], [5, 6]])
print(x.shape) # torch.Size([3, 2])

# 2x3 텐서로 변경
y = x.view(2, 3)
print(y)

x = torch.rand(4, 4) # 총 16개 요소
y = x.view(2, -1)    # 2 x (알아서 계산) -> 2 x 8이 됨
z = x.view(-1, 4)    # -1 x 4 -> 4 x 4가 됨

# ==== view 함수는 메모리 연속을 강제함. reshape()는 알아서 바꿔줌.

# 차원을 바꾼 후 view를 쓰고 싶을 때
x = torch.randn(2, 3)
y = x.transpose(0, 1) # 3x2가 됨

# y.view(6) <- 에러 발생 가능성 높음!
# 안전하게 해결:
y = y.contiguous().view(6)
```


## 2. 토치비전(torchvision) 소개하기

본격적인 실습에 들어가기에 앞서 
토치비전(torchvision)이라는 도구를 설명하겠습니다. 
torchvision은 << 유명한 데이터셋들, 이미 구현되어져 있는 유명한 모델들, 일반적인 이미지 전처리 도구 >> 들을 포함하고 있는 패키지입니다. 

아래의 링크는 torchvision에 어떤 데이터셋들(datasets)과 모델들(models) 그리고 어떤 전처리 방법들(transforms)을 제공하고 있는지 보여줍니다.

링크 : [torchvision — Torchvision 0.28 documentation](https://docs.pytorch.org/vision/stable/index.html)

- **자연어 처리를 위해서는 토치텍스트(torchtext)라는 패키지가 있습니다.**


## 3. 분류기 구현을 위한 사전 설정

우선 필요한 도구들을 임포트합니다.

```python
import torch
import torchvision.datasets as dsets
import torchvision.transforms as transforms
from torch.utils.data import DataLoader
import torch.nn as nn
import matplotlib.pyplot as plt
import random
```

현재 환경에서 GPU 연산이 가능하다면 GPU 연산을 하고, 그렇지 않다면 CPU 연산을 하도록 합니다.

```python
USE_CUDA = torch.cuda.is_available() # GPU를 사용가능하면 True, 아니라면 False를 리턴
device = torch.device("cuda" if USE_CUDA else "cpu") # GPU 사용 가능하면 사용하고 아니면 CPU 사용
print("다음 기기로 학습합니다:", device)
```

구글의 Colab에서 '런타임 > 런타임 유형 변경 > 하드웨어 가속기 > GPU'를 선택하면 USE_CUDA의 값이 True가 되면서 '다음 기기로 학습합니다: cuda'라는 출력이 나옵니다. 즉, GPU로 연산하겠다는 의미입니다. 
(colab 에서 돌리면 => "다음 기기로 학습합니다: cuda" 가 나오는 것을 볼 수 있음.)

반면에 '하드웨어 가속기 > None'을 선택하면 USE_CUDA의 값이 False가 되면서 
'다음 기기로 학습합니다: cpu'라는 출력이 나옵니다. 즉, CPU로 연산하겠다는 의미입니다.

위의 방법은 앞으로 자주 쓰이게되므로 기억해둡시다.
(torch.cuda.is_available() :  NVIDIA GPU CUDA 있는지없는지 확인하는거.)
(NVIDIA의 CUDA 드라이버/sw스택이 GPU의 유무를 확인하는 API를 제공하는 것)

```
[추가 내용: torch.cuda]
- **`torch.cuda.device_count()`**: 현재 사용 가능한 GPU가 몇 개인지 숫자로 알려줍니다.
    
- **`torch.cuda.get_device_name(0)`**: 0번 GPU의 정확한 이름(예: 'NVIDIA GeForce RTX 3060')을 출력합니다.
```


랜덤 시드를 고정합니다.

```python
# for reproducibility
random.seed(777)
torch.manual_seed(777)
if device == 'cuda':
    torch.cuda.manual_seed_all(777)
```


하이퍼파라미터를 변수로 둡니다.

```python
# hyperparameters
training_epochs = 15
batch_size = 100
```

#### cuda.manual_seed_all, manual_seed, random.seed 뭔 차이?
##### 1. `random.seed(777)`

- **대상:** 파이썬 기본 `random` 모듈.
- **범위:** 파이썬 표준 라이브러리인 `random`을 사용하여 생성하는 난수들.
- **용도:** 데이터 로딩 시 순서를 섞거나, 일반적인 파이썬 리스트에서 무작위 샘플링을 할 때 영향을 줍니다.
- **주의:** 파이토치의 텐서 연산(가중치 초기화 등)에는 영향을 **주지 않습니다.**

##### 2. `torch.manual_seed(1)`

- **대상:** << CPU에서 생성되는 파이토치 난수. >> 
- **범위:** `torch.rand()`, `torch.randn()` 등을 호출할 때 생성되는 텐서의 값.
- **용도:** 딥러닝 모델의 <<< 가중치 초기값을 똑같이 맞추거나 >>>, 학습 데이터 배치를 똑같이 섞을 때 필수적입니다.

##### 3. `torch.cuda.manual_seed_all(777)`

- **대상:** 모든 GPU 장치.
- **범위:** 시스템에 있는 << **모든 NVIDIA GPU**에서 생성되는 파이토치 난수. >>
- **용도:** GPU를 여러 개 사용하거나 하나만 사용하더라도, GPU 위에서 돌아가는 파이토치 연산의 난수 값을 고정합니다.
- **특징:** `torch.manual_seed()`만 쓰면 CPU 연산만 고정될 수 있기 때문에, GPU를 쓴다면 이 함수도 같이 써주는 것이 안전합니다.


## 4. MNIST 분류기 구현하기

torchvision.datasets.dsets.MNIST를 사용하여 MNIST 데이터셋을 불러올 수 있습니다.

```python
# MNIST dataset
mnist_train = dsets.MNIST(root='MNIST_data/',
                          train=True,
                          transform=transforms.ToTensor(),
                          download=True)

mnist_test = dsets.MNIST(root='MNIST_data/',
                         train=False,
                         transform=transforms.ToTensor(),
                         download=True)
```

첫번째 인자 root는 MNIST 데이터를 다운로드 받을 경로입니다. 
두번째 인자 train: (훈련이냐 테스트냐 구분)
   인자로 True를 주면, MNIST의 훈련 데이터를 리턴받으며 
   False를 주면 테스트 데이터를 리턴받습니다
세번째 인자 transform은 현재 << 데이터를 파이토치 텐서로 변환 >> 해줍니다. 
네번째 인자 download는 해당 경로에 << MNIST 데이터가 없다면 다운로드 받겠다는 의미입니다. >>

이렇게 데이터를 다운로드했다면 앞서 미니 배치와 데이터로드 챕터에서 학습했던 데이터로더(DataLoader)를 사용합니다.
복습 : [[03 Machine Learning Basics#^dataloader]]

```python
# dataset loader
data_loader = DataLoader(dataset=mnist_train,
                         batch_size=batch_size, # 배치 크기는 100
                         shuffle=True,
                         drop_last=True)
```

이때 DataLoader에는 4개의 인자가 있습니다. 
첫번째 인자인 dataset은 로드할 대상을 의미하며, 
두번째 인자인 batch_size는 배치 크기, 
shuffle은 매 에포크마다 미니 배치를 셔플할 것인지의 여부, 
<< drop_last는 마지막 배치를 버릴 것인지를 의미합니다. >>
=>
- **drop_last를 하는 이유를 이해하기 위해서 1,000개의 데이터가 있다고 했을 때, 배치 크기가 128이라고 해봅시다. 1,000을 128로 나누면 총 7개가 나오고 나머지로 104개가 남습니다. 이때 104개를 마지막 배치로 한다고 하였을 때 128개를 충족하지 못하였으므로 104개를 그냥 버릴 수도 있습니다. 이때 마지막 배치를 버리려면 drop_last=True를 해주면 됩니다. <<< 이는 다른 미니 배치보다 개수가 적은 마지막 배치를 경사 하강법에 사용하여 마지막 배치가 "상대적으로 과대 평가"되는 현상을 막아줍니다. >>> ** 

---

이제 모델을 설계합니다. input_dim은 784이고, output_dim은 10입니다.

```python
# MNIST data image of shape 28 * 28 = 784
linear = nn.Linear(784, 10, bias=True).to(device)
```

to() 함수는 << 연산을 어디서 수행할지 >> 를 정합니다. 
to() 함수는 모델의 매개변수를 지정한 장치의 메모리로 보냅니다. 
CPU를 사용할 경우에는 필요가 없지만, GPU를 사용하려면 to('cuda')를 해 줄 필요가 있습니다. << 아무것도 지정하지 않은 경우에는 CPU 연산이라고 보면 됩니다. >>

bias는 편향 << b를 사용할 것인지를 나타냅니다. 기본값은 True이므로 굳이 할 필요는 없지만 명시적으로 True를 해주었습니다. >>



이제 비용 함수와 옵티마이저를 정의합니다.

```python
# 비용 함수와 옵티마이저 정의
criterion = nn.CrossEntropyLoss().to(device) # 내부적으로 소프트맥스 함수를 포함하고 있음.
optimizer = torch.optim.SGD(linear.parameters(), lr=0.1)
```

- **앞서 소프트맥스 회귀를 배울 때는 torch.nn.functional.cross_entropy()를 사용하였으나 여기서는 torch.nn.CrossEntropyLoss()을 사용하고 있습니다. 둘 다 파이토치에서 제공하는 크로스 엔트로피 함수로 둘 다 소프트맥스 함수를 포함하고 있습니다. (결과는 동일하지만 재사용성을 위해 class방식 사용)**

```python
for epoch in range(training_epochs): # 앞서 training_epochs의 값은 15로 지정함.
    avg_cost = 0
    total_batch = len(data_loader)

    for X, Y in data_loader:
        # 배치 크기가 100이므로 아래의 연산에서 X는 (100, 784)의 텐서가 된다.
        X = X.view(-1, 28 * 28).to(device)
        # 레이블은 원-핫 인코딩이 된 상태가 아니라 0 ~ 9의 정수.
        Y = Y.to(device)

        optimizer.zero_grad()
        hypothesis = linear(X)
        cost = criterion(hypothesis, Y)
        cost.backward()
        optimizer.step()

        avg_cost += cost / total_batch

    print('Epoch:', '%04d' % (epoch + 1), 'cost =', '{:.9f}'.format(avg_cost))

print('Learning finished')
```

먼저, training_epochs의 값은 15로 설정되어 있으며, 모델은 총 15번의 에포크 동안 학습됩니다. 
avg_cost는 에포크 동안의 << 평균 비용 = 각 배치에서의 cost/총묶음수를 누적합시킨 것이 avg_cost. >> 을 저장하는 변수이며, 
total_batch는 에포크당 수행할 배치(batch) 수를 계산합니다. 
data_loader는 미니 배치 학습을 위해 데이터를 반복적으로 제공하는 역할을 합니다.

루프 내부에서는 각 배치마다 입력 데이터 X와 레이블 Y를 받아옵니다. 
이때, X는 이미지 데이터로서 (100, 784) 크기의 텐서로 변환되는데, 
이는 << 배치 크기 100에 28x28 픽셀의 이미지가 일렬로 펼쳐진 상태(784) >>
를 나타냅니다.  이 데이터와 레이블 Y는 모델 학습을 위해 지정된 장치(device)로 전송됩니다.

다음으로, 옵티마이저의 기울기 정보를 초기화하고, 
모델의 가설(hypothesis)을 계산합니다. (지금까지 하던 대로)
linear(X)는 모델의 순전파(forward) 과정을 수행하여 예측 값을 계산합니다.

그 후, 손실 함수(criterion)를 사용하여 예측 값과 실제 레이블 Y 간의 비용(cost)을 계산합니다. 이 비용은 모델의 성능을 나타내며, 비용이 작을수록 모델의 예측이 실제 값에 가까워집니다.

이후, cost.backward()를 호출하여 역전파(backpropagation)를 수행하고, 기울기를 계산합니다. 그리고 옵티마이저의 step()을 호출하여 모델의 파라미터(가중치와 편향)를 업데이트합니다.

각 배치의 비용을 avg_cost에 누적하여 에포크당 평균 비용을 계산하고, 에포크가 끝날 때마다 현재 에포크 번호와 평균 비용을 출력합니다. 모든 에포크가 종료되면 "Learning finished" 메시지를 출력하여 학습이 완료되었음을 알립니다.

```python
Epoch: 0001 cost = 0.535468459
Epoch: 0002 cost = 0.359274209
Epoch: 0003 cost = 0.331187516
Epoch: 0004 cost = 0.316578060
Epoch: 0005 cost = 0.307158142
Epoch: 0006 cost = 0.300180763
Epoch: 0007 cost = 0.295130193
Epoch: 0008 cost = 0.290851474
Epoch: 0009 cost = 0.287417054
Epoch: 0010 cost = 0.284379572
Epoch: 0011 cost = 0.281825274
Epoch: 0012 cost = 0.279800713
Epoch: 0013 cost = 0.277808994
Epoch: 0014 cost = 0.276154339
Epoch: 0015 cost = 0.274440885
Learning finished
```

학습된 모델을 테스트 데이터로 평가하고, 테스트 데이터에서 임의의 이미지를 선택하여 모델이 해당 이미지를 어떻게 예측하는지 시각적으로 확인해보겠습니다.

```python
# 테스트 데이터를 사용하여 모델을 테스트한다.
with torch.no_grad(): # torch.no_grad()를 하면 gradient 계산을 수행하지 않는다.
    X_test = mnist_test.test_data.view(-1, 28 * 28).float().to(device)
    Y_test = mnist_test.test_labels.to(device)

    prediction = linear(X_test)
    correct_prediction = torch.argmax(prediction, 1) == Y_test
    accuracy = correct_prediction.float().mean()
    print('Accuracy:', accuracy.item())

    # MNIST 테스트 데이터에서 무작위로 하나를 뽑아서 예측을 해본다
    r = random.randint(0, len(mnist_test) - 1)
    X_single_data = mnist_test.test_data[r:r + 1].view(-1, 28 * 28).float().to(device)
    Y_single_data = mnist_test.test_labels[r:r + 1].to(device)

    print('Label: ', Y_single_data.item())
    single_prediction = linear(X_single_data)
    print('Prediction: ', torch.argmax(single_prediction, 1).item())

    plt.imshow(mnist_test.test_data[r:r + 1].view(28, 28), cmap='Greys', interpolation='nearest')
    plt.show()
```

```python
Accuracy: 0.8883000016212463
Label:  5
Prediction:  5
```

![](https://static.wikidocs.net/images/page/60324/pred.PNG)

- 평가 모드 활성화: with torch.no_grad(): 블록 내에서는 기울기 계산을 하지 않도록 설정합니다. 이는 모델을 테스트할 때 필요 없는 기울기 계산을 방지하여 메모리와 연산 효율을 높입니다.
    
- 테스트 데이터 준비: mnist_test.test_data는 테스트 데이터셋의 이미지 데이터를 포함하며, view(-1, 28 * 28)를 통해 28x28 크기의 이미지를 일렬로 펼쳐 (1, 784) 형태로 변환합니다.(test 샘플중 하나만 뽑아서 테스트해보는 것이니 1x..임) .float() 메서드를 통해 데이터를 실수형으로 변환하고, .<< to(device)로 모델과 동일한 장치(CPU나 GPU)에 배치합니다. >>  mnist_test.test_labels는 해당 데이터셋의 레이블(정답)을 포함합니다. 
    
- 모델 예측 및 정확도 계산: linear(X_test)를 통해 테스트 데이터에 대한 모델의 예측을 수행합니다.([[#여러 가지 회귀 복습/구분]]  참조)  
- torch.argmax(prediction, 1)은 각 이미지에 대해 예측된 클래스 레이블을 반환하며, 이 값이 실제 레이블 Y_test와 동일한지 여부를 correct_prediction 변수에 저장합니다. 이후, correct_prediction.float().mean()를 통해 전체 테스트 데이터셋에 대한 정확도를 계산하고 출력합니다.
    
- 임의의 테스트 샘플 예측: random.randint(0, len(mnist_test) - 1)을 사용하여 테스트 데이터셋에서 임의의 샘플을 선택합니다. 선택된 이미지를 모델에 입력하여 예측하고, 실제 레이블(Y_single_data)과 모델의 예측 결과(single_prediction)를 출력합니다.
    
- 이미지 시각화: plt.imshow()를 사용하여 선택된 이미지를 시각적으로 보여줍니다. cmap='Greys'는 이미지를 회색조로 표시하고, interpolation='nearest'는 이미지를 확대할 때 보간을 최소화하여 원본의 형태를 유지합니다.



# 용어 정리
## ReLU 함수 복습, 사용되는 단계
### ReLU란?

**ReLU(Rectified Linear Unit)**는 신경망의 활성화 함수 중 하나로, 아주 단순합니다:

$f(x) = \max(0, x)$

- 입력이 0보다 크면 → 그대로 통과
- 입력이 0 이하면 → 0으로 출력

질문하신 내용을 하나씩 짚어볼게요. 몇 가지 오해가 섞여 있습니다.

### 1. 누구를 위한 함수인가? 가중치 전달용인가?

절반만 맞습니다. 활성화 함수는 
<< **가중치를 전달하는 게 아니라, 뉴런의 출력값을 변환**하는 역할 >>
입니다. 흐름은 이렇습니다:

$\text{출력} = f(w_1x_1 + w_2x_2 + \cdots + b)$

즉, 입력 × 가중치의 합을 구한 뒤, 
<< 그 결과에 **비선형성을 부여**하는 게 활성화 함수입니다. >> 
활성화 함수가 없으면 층을 아무리 쌓아도 결국 하나의 선형 함수와 같아져서, 
깊은 신경망의 의미가 사라집니다. 그래서 "비선형성을 위한" 함수라고 이해하는 게 정확합니다.

### 2. 가중치는 0~1 사이?

**아닙니다.** 두 가지가 혼동된 것 같아요:
- **가중치**: 범위 제한 없음. 음수, 1보다 큰 값 모두 가능. 학습으로 계속 변함
- **활성화 함수의 출력**: 함수에 따라 다름
    - Sigmoid: 0~1
    - Tanh: -1~1
    - **ReLU: 0~∞ (상한 없음!)**

"0~1"은 아마 Sigmoid 함수의 출력 범위를 떠올리신 것 같습니다.

### 3. "계단함수 → 기울기 소멸 → ReLU" 인가?

순서가 살짝 다릅니다. 실제 역사는:

1. **계단 함수(Step function)** — 초기 퍼셉트론에서 사용. 문제: 미분값이 거의 모든 곳에서 0이라 **역전파(경사하강법) 학습 자체가 불가능**
2. **Sigmoid / Tanh** — 계단 함수를 "부드럽게" 만들어 미분 가능하게 함. 그런데 층이 깊어지면 미분값(최대 0.25)이 계속 곱해지며 **기울기 소실(vanishing gradient)** 발생
3. **ReLU** — Sigmoid의 기울기 소실을 해결하려고 등장. 양수 구간에서 기울기가 항상 1이라 깊은 층에서도 기울기가 잘 전달됨 + 계산도 훨씬 빠름

즉, **ReLU가 직접 대체한 건 계단 함수가 아니라 Sigmoid/Tanh**이고, 기울기 소실 문제는 Sigmoid 시대의 문제입니다.

### 정리

|항목|오해|실제|
|---|---|---|
|역할|가중치 전달|출력에 비선형성 부여|
|0~1 범위|가중치가 0~1|Sigmoid 출력이 0~1, ReLU는 0~∞|
|등장 배경|계단함수의 기울기 소실 해결|Sigmoid의 기울기 소실 해결|

참고로 ReLU도 단점이 있습니다 — 음수 입력이 계속되면 뉴런이 죽어버리는 **Dying ReLU** 문제요. 이를 보완한 Leaky ReLU, GELU 등이 요즘 많이 쓰입니다. 더 궁금하시면 이어서 설명해드릴게요.

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

## argmax 함수 복습

소프트맥스 회귀의 마지막 출력층(`prediction`)은 보통 10개의 클래스(0~9)에 대한 **확률 분포**를 담고 있습니다.

예를 들어, 숫자 '2'를 입력했을 때 모델의 출력(`prediction`)이 아래와 같다고 해보죠.

```python
# [0, 1, 2, 3, 4, 5, 6, 7, 8, 9] (각 숫자에 대한 확률)
tensor([[0.01, 0.02, 0.85, 0.01, 0.05, 
		 0.02, 0.01, 0.01, 0.01, 0.01]])
```

여기서 "모델이 예측한 최종 정답"은 무엇인가요? 
바로 0.85라는 가장 큰 확률값을 가진 '2'입니다.

이때 `torch.argmax(prediction, 1)`을 하면, 10개의 확률값 중에서 **가장 큰 값을 가진 인덱스(위치)인 `2`를 반환**해 줍니다. 즉, "모델이 2번 숫자라고 예측했구나!"를 알기 위해 쓰는 것입니다.

### `torch.argmax(prediction, 1)`의 의미
=> 1은 dim=1 인 것.

- **`prediction`**: (배치 크기, 10) 형태의 텐서입니다.
    
- **`1`**: 1번 dim = 차원(가로 방향)을 보라는 뜻입니다. (각 데이터별로 10개의 확률 중 제일 큰 걸 찾으라는 뜻)
    
- **반환값**: 가장 큰 확률을 가진 **위치(인덱스)**.

## 여러 가지 회귀 복습/구분

> linear(X_test) => 선형회귀로 보이는데.. 헷갈리네
   선형회귀 :  매우단순하게 일차함수 W b 조정
   로지스틱회귀 : 선그어서 이진분류 하도록 로지스틱함수 (시그모이드)추가  
   다부류 분류 - 소프트맥스 회귀 : 복잡한 선을 그어야함 : 오차함수도 교차엔트로피가 되고, 소프트맥스함수로 다부류의 회귀값을 총합1로바꿈. 근데 여기서 선형회귀로 W, b 조절하는건 똑같음 => 이라 linear 로 결과값이 나오는거?


### 1. 선형 결합($WX+b$)은 "공통 엔진"입니다

질문하신 대로 선형 회귀, 로지스틱 회귀(이진 분류), 소프트맥스 회귀(다중 분류) 모두 **내부 엔진인 $WX+b$는 똑같습니다.**

- **선형 회귀:** $WX+b$의 결과값(연속된 숫자)을 그대로 사용.
    
- **이진 분류:** $WX+b$를 통과한 값을 시그모이드(Sigmoid)에 넣어 $0 \sim 1$ 사이의 확률로 변환.
    
- **다중 분류:** $WX+b$를 통과한 값을 소프트맥스(Softmax)에 넣어 여러 클래스 확률의 **총합이 1**이 되도록 변환.
    

그래서 코드에서 `nn.Linear(784, 10)`을 사용하는 이유는, **입력(784)을 받아서 10개의 클래스에 대한 '점수(Logits)'를 만들기 위한 10개의 선형 함수($WX+b$)를 동시에 돌리고 있기 때문**입니다.

### 2. 왜 `linear`라고 부르나요?

네, 정확합니다. `nn.Linear`는 이름 그대로 **선형 연산만 수행**하기 때문입니다.

- `linear(X)`의 결과는 아직 활성화 함수(Softmax)를 거치지 않은 원시 점수(Raw Score)입니다.
    
- 이 점수들을 **소프트맥스**라는 껍데기를 씌워 확률로 바꾸느냐, 아니면 그냥 쓰느냐는 그 뒤의 선택입니다.
    

### 3. 구조적 차이 (왜 이름이 같은가?)

이 부분이 혼동스러우실 텐데, 파이토치 설계상 모델의 가장 바닥에 있는 '행렬 연산기'를 `nn.Linear`라는 클래스로 만들어 두었습니다.

|**모델**|**사용 엔진**|**뒤에 붙는 '껍데기'(활성화 함수)**|
|---|---|---|
|**선형 회귀**|`nn.Linear`|없음|
|**로지스틱 회귀**|`nn.Linear`|`torch.sigmoid`|
|**소프트맥스 회귀**|`nn.Linear`|`torch.softmax` (혹은 `CrossEntropyLoss` 내부)|

### 결론

"다중 분류인데 왜 `linear`를 쓰지?"라고 생각하시는 게 매우 정상입니다. 하지만 딥러닝에서 **분류는 '선형 연산($WX+b$)으로 점수를 뽑고 + 활성화 함수(Softmax)로 판단'하는 과정**이 한 세트라고 보시면 됩니다.

즉, `linear(X)`는 "우리 모델이 아직 활성화 함수(Softmax)를 통과하기 전의 날것의 상태"라고 보시면 정확합니다!



## torchvision dsets.MNIST() vs sklearn.datasets.fetch_openml()


### 1. `torchvision.datasets.MNIST` (PyTorch 전용)

파이토치 생태계의 표준입니다.

- **형태:** **`torch.Tensor`** 형태로 바로 가져옵니다.
    
- **변환(Transform):** `transform=transforms.ToTensor()`를 쓰면 픽셀값을 바로 `0~1` 사이의 실수(float)로 정규화해주고, 텐서 차원도 학습하기 좋은 형태로 알아서 바꿔줍니다.
    
- **장점:** `DataLoader`와 찰떡궁합입니다. `.to(device)`로 GPU에 올리거나 배치(batch) 단위로 데이터를 쪼개는 작업이 한 번에 해결됩니다.
    
- **추천 상황:** 딥러닝 모델 학습, 신경망 구현, GPU 연산 시.
    

### 2. `sklearn.datasets.fetch_openml` (데이터 과학/머신러닝 전용)

Scikit-learn에서 사용하는 데이터 로더입니다.

- **형태:** **`numpy` 배열** 혹은 `pandas` 데이터프레임 형태로 가져옵니다.
    
- **변환:** 직접 넘파이 배열을 파이토치 텐서로 바꿔주어야 합니다 (`torch.from_numpy()` 등).
    
- **장점:** 머신러닝 알고리즘(SVM, Random Forest, KNN 등)을 테스트할 때 매우 편리합니다. 굳이 텐서로 바꿀 필요 없이 바로 알고리즘에 넣으면 되기 때문입니다.
    
- **추천 상황:** 전통적인 머신러닝 알고리즘 비교, 데이터 전처리 분석, 모델 없이 데이터만 살펴볼 때.
    

### 비교 요약표

|**비교 항목**|**torchvision.MNIST**|**sklearn.fetch_openml**|
|---|---|---|
|**주요 출력**|**`torch.Tensor`**|**`numpy.ndarray`**|
|**딥러닝 연동**|즉시 가능 (최적화됨)|추가 변환 필요|
|**데이터 전처리**|`transforms` 제공|직접 구현 필요|
|**주 목적**|딥러닝 모델 학습|머신러닝 알고리즘 학습|

### 💡 왜 `torchvision`을 써야 하나요?

질문자님이 지금 하고 계신 `Softmax Regression` 실습 코드에서 만약 `sklearn`으로 데이터를 가져온다면, 아래와 같은 번거로운 작업이 추가됩니다:

1. 데이터를 `numpy`에서 `torch.tensor`로 변환.
    
2. `float` 타입으로 캐스팅.
    
3. 이미지 차원 조절 (reshape).
    
4. 직접 배치 크기만큼 데이터를 슬라이싱해서 `DataLoader`처럼 만드는 루프 구현.
    

하지만 `torchvision`을 쓰면 이 모든 게 **`DataLoader(mnist_train, batch_size=100, shuffle=True)`** 한 줄로 끝납니다.

**한 줄 결론:** 딥러닝을 하신다면 `torchvision`이 압도적으로 편하고 빠릅니다. 다른 거 고민하지 마시고 지금 쓰시는 방식대로 유지하세요!