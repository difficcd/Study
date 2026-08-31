
> 다대일 RNN과 CNN을 이용하여 텍스트 분류


# 13-01 RNN을 이용한 텍스트 분류(Text classification using PyTorch)

이번 챕터에서는 파이토치(PyTorch)로 인공 신경망을 이용한 텍스트 분류를 실습합니다. 
실습에 앞서 딥 러닝을 이용해서 텍스트 분류가 수행될 때, 
어떤 작업과 구성으로 진행되는지 간단히 미리 정리해보겠습니다.

## 1. 훈련 데이터에 대한 이해

앞으로 배우게 될 텍스트 분류 작업은 지도 학습(Supervised Learning)에 속합니다. 지도 학습의 훈련 데이터는 레이블이라는 이름의 미리 정답이 적혀있는 데이터로 구성되어 있습니다. 쉽게 비유하면, 기계는 정답이 적혀져 있는 문제지를 열심히 공부하고, 향후에 정답이 없는 문제에 대해서도 정답을 예측해서 대답하게 되는 메커니즘입니다.

예를 들어 스팸 메일 분류기의 훈련 데이터같은 경우에는 메일의 내용과 해당 메일이 정상 메일인지, 스팸 메일인지 적혀있는 레이블로 구성되어져 있습니다. 아래와 같은 형식의 메일 샘플이 약 20,000개 있다고 가정해봅시다.

| 텍스트(메일의 내용)          | 레이블(스팸 여부) |
| -------------------- | ---------- |
| 당신에게 드리는 마지막 혜택! ... | 스팸 메일      |
| 내일 뵐 수 있을지 확인 부탁...  | 정상 메일      |
| 쉿! 혼자 보세요...         | 스팸 메일      |
| 언제까지 답장 가능할...       | 정상 메일      |
| ...                  | ...        |
| (광고) 멋있어질 수 있는...    | 스팸 메일      |

20,000개의 메일 샘플을 가진 이 데이터는 메일의 내용을 담고 있는 텍스트 데이터와 이 데이터가 스팸 메일인지 아닌지가 적혀있는 레이블. 두 가지 열로 이루어져있습니다. 

기계는 이 20,000개의 메일 샘플 데이터를 학습하게 되는데, 
만약 데이터가 깔끔하고 모델 또한 잘 설계되어져 있다면 
학습이 다 된 이 모델은 훈련 데이터에서는 없었던 어떤 메일 텍스트가 주어졌을 때 레이블을 예측하게 됩니다.

![](https://static.wikidocs.net/images/page/24873/%EB%B6%84%EB%A5%98.png)



## 2. 훈련 데이터와 테스트 데이터

위에서는 20,000개의 메일 샘플을 전부 훈련에 사용한다고 했지만 사실 갖고있는 전체 데이터를 전부 훈련에 사용하는 것 보다는 테스트용으로 일부는 남겨놓는 것으로 바람직합니다. 

예를 들어서 20,000개의 샘플 중에서 18,000개의 샘플은 훈련용으로 사용하고, 2,000개의 샘플은 테스트용으로 보류한 채 훈련을 시킬 때는 사용하지 않을 수 있습니다. 

그리고 나서 18,000개의 샘플로 모델이 훈련이 다 되면, 이제 보류해두었던 2,000개의 테스트용 샘플에서 레이블은 보여주지않고 모델에게 맞춰보라고 요구한 뒤, 정확도를 확인해볼 수 있습니다. 2,000개의 샘플에도 레이블이 있으므로 모델이 실제로 정답을 얼마나 맞추는지 정답률을 계산하게 됩니다.

뒤에 나오게 될 예제에서는 갖고 있는 데이터에서 분류하고자 하는 텍스트 데이터의 열을 X, 
레이블 데이터의 열을 y라고 명명합니다. 

그리고 이를 훈련 데이터(X_train, y_train)와 테스트 데이터(X_test, y_test)로 분리합니다. 모델은 X_train과 y_train을 학습하고, X_test에 대해서 레이블을 예측하게 됩니다. 

그리고 모델이 예측한 레이블과 y_test를 비교해서 정답률을 계산하게 됩니다.



## 3. 단어에 대한 인덱스 부여

앞서 워드 임베딩 챕터에서 단어를 밀집 벡터(dense vector)로 바꾸는 워드 임베딩에 대해서 배운 바 있습니다. 8챕터와 9챕터에서 설명하였지만, 파이토치(PyTorch)의 nn.Embedding()은 << 단어 각각에 대해 정수가 맵핑된 입력에 대해서 임베딩 작업을 수행 >> 할 수 있게 해줍니다.

단어 각각에 숫자 맵핑, 인덱스를 부여하는 방법으로는 정수 인코딩 챕터에서와 같이 단어를 << 빈도수 순대로 정렬하고 순차적으로 인덱스를 부여 >> 하는 방법이 있습니다. << 로이터 뉴스 분류하기와 IMDB 리뷰 감성 분류하기 챕터에서도 이 방법을 사용하였으며, 해당 챕터에서 사용할 데이터들은 이미 이 작업이 끝난 상태 >> 입니다.

등장 빈도순대로 단어를 정렬하여 인덱스를 부여하였을 때의 장점은 등장 빈도수가 적은 단어의 제거입니다. 

예를 들어서 25,000개의 단어가 있다고 가정하고, 해당 단어를 등장 빈도수 순가 높은 순서로 1부터 25,000까지 인덱스를 부여했다고 해보겠습니다. 
이렇게 되면 등장 빈도수 순대로 등수가 부여된 것과 다름없기 때문에 전처리 작업에서 1,000을 넘는 인덱스를 가진 단어들을 제거시켜버리면 등장 빈도수 상위 1,000개의 단어만 남길 수 있습니다.


## 4. ==RNN으로 분류하기

```python
# 실제 RNN 은닉층을 추가하는 코드.
nn.RNN(input_size, hidden_size, batch_first=True)
```

텍스트 분류 관점에서 앞서 배운 RNN 코드의 timesteps와 input_dim, 그리고 hidden_size를 해석해보면 다음과 같습니다. (위의 코드에서는 바닐라 RNN을 사용했지만, RNN의 변형인 LSTM이나 GRU도 아래의 사항은 동일합니다.)

hidden_size = 출력의 크기(output_dim).  
timesteps = 시점의 수 = 각 문서에서의 단어 수.  
input_size = 입력의 크기 = 각 단어의 벡터 표현의 차원 수.


## 5. RNN의 다-대-일(Many-to-One) 문제

텍스트 분류는 RNN의 다-대-일(Many-to-One) 문제에 속합니다. 
즉, 텍스트 분류는 모든 시점(time step)에 대해서 입력을 받지만 
<< 최종 시점의 RNN 셀만이 은닉 상태를 출력하고, 이것이 출력층으로 가서 활성화 함수를 통해 정답을 고르는 문제 >> 가 됩니다.

이 때 두 개의 선택지 중에서 정답를 고르는 이진 분류(Binary Classification) 문제라고 하며, 
세 개 이상의 선택지 중에서 정답을 고르는 다중 클래스 분류(Multi-Class Classification) 문제라고 합니다. 이 두 문제에서는 각각 문제에 맞는 다른 활성화 함수와 손실 함수를 사용할 것입니다.

이진 분류의 문제의 경우 출력층의 활성화 함수로 시그모이드 함수(0~1사이 확률 뱉음)를, 다중 클래스 문제라면 출력층의 활성화 함수로 소프트맥스 함수(RV: 다부류에 대응하는 확률 뱉음)를 사용합니다. 

또한, 다중 클래스 분류 문제의 경우에는 클래스가 N개라면 출력층에 해당되는 밀집층(dense layer)의 크기는 N으로 합니다. 즉, 출력층의 뉴런의 수는 N개입니다. (하지만 소프트맥스 함수로 이진 분류를 할 수도 있습니다. 출력층에 뉴런을 2개로 배치하면 됩니다.)



# 13-02 LSTM을 이용한 네이버 영화 리뷰 분류

이번에 사용할 데이터는 네이버 영화 리뷰 데이터입니다. 
총 200,000개 리뷰로 구성된 데이터로 영화 리뷰에 대한 텍스트와 해당 리뷰가 긍정인 경우 1, 부정인 경우 0을 표시한 레이블로 구성되어져 있습니다. 
해당 데이터를 다운로드 받아 감성 분류를 수행하는 모델을 만들어보겠습니다.

## 1. 네이버 영화 리뷰 데이터에 대한 이해와 전처리

데이터 다운로드 링크 : https://github.com/e9t/nsmc/

```python
import pickle
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import re
import urllib.request
from konlpy.tag import Mecab
from tqdm import tqdm
from sklearn.model_selection import train_test_split
from collections import Counter
```

### 1) 데이터 로드하기

위 링크로부터 훈련 데이터에 해당하는 ratings_train.txt와 테스트 데이터에 해당하는 ratings_test.txt를 다운로드합니다.

```python
urllib.request.urlretrieve("https://raw.githubusercontent.com/e9t/nsmc/master/ratings_train.txt", filename="ratings_train.txt")
urllib.request.urlretrieve("https://raw.githubusercontent.com/e9t/nsmc/master/ratings_test.txt", filename="ratings_test.txt")
```

pandas를 이용하여 훈련 데이터는 train_data에 테스트 데이터는 test_data에 저장합니다.

```python
train_data = pd.read_table('ratings_train.txt')
test_data = pd.read_table('ratings_test.txt')
```

train_data에 존재하는 영화 리뷰의 개수를 확인해봅시다.

```python
print('훈련용 리뷰 개수 :',len(train_data)) # 훈련용 리뷰 개수 출력
```

```python
훈련용 리뷰 개수 : 150000
```

train_data는 총 150,000개의 리뷰가 존재합니다. 상위 5개의 샘플을 출력해봅시다.

```python
train_data[:5] # 상위 5개 출력
```

![](https://static.wikidocs.net/images/page/44249/navermovie1.PNG)

해당 데이터는 id, document, label 총 3개의 열로 구성되어져 있습니다. id는 감성 분류를 수행하는데 도움이 되지 않으므로 앞으로 무시합니다. 

결국 이 모델은 리뷰 내용을 담고있는 document와 해당 리뷰가 긍정(1), 부정(0)인지를 나타내는 label 두 개의 열을 학습하는 모델이 되어야 합니다.

또한 단지 상위 5개의 샘플만 출력해보았지만 한국어 데이터와 영어 데이터의 차이를 확인할 수 있습니다. 예를 들어, 인덱스 2번 샘플은 띄어쓰기를 하지 않아도 글을 쉽게 이해할 수 있는 한국어의 특성으로 인해 띄어쓰기가 되어있지 않습니다. test_data의 리뷰 개수와 상위 5개의 샘플을 확인해봅시다.

```python
print('테스트용 리뷰 개수 :',len(test_data)) # 테스트용 리뷰 개수 출력
```

```python
테스트용 리뷰 개수 : 50000
```

test_data는 총 50,000개의 영화 리뷰가 존재합니다. 상위 5개의 샘플을 출력해봅시다.

```python
test_data[:5]
```

![](https://static.wikidocs.net/images/page/44249/navermovie2.PNG)

test_data도 train_data와 동일한 형식으로 id, document, label 3개의 열로 구성되어져 있습니다.

### 2) 데이터 정제하기

train_data의 데이터 중복 유무를 확인합니다.

```python
# document 열과 label 열의 중복을 제외한 값의 개수
train_data['document'].nunique(), train_data['label'].nunique()

#nunique() : 고유한 값들이 몇 개인지 반환하는 함수 (return : number)
```

```python
(146182, 2)
```

총 150,000개의 샘플이 존재하는데 document열에서 중복을 제거한 샘플의 개수가 146,182개라는 것은 약 4,000개의 중복 샘플이 존재한다는 의미입니다. 

label 열은 0 또는 1의 두 가지 값만을 가지므로 2가 출력됩니다. 중복 샘플을 제거합니다.

```python
# document 열의 중복 제거
train_data.drop_duplicates(subset=['document'], inplace=True)

# drop_duplicates : dataframe, series 받아서 중복제거해주는 함수 (pandas)
```
- **`unique()`**: 특정 열에 어떤 값들이 들어있는지 순수한 값의 목록(리스트)만 뽑아서 확인하고 싶을 때
    
- **`drop_duplicates()`**: 중복된 데이터 행을 제거하고 **데이터프레임의 형태(테이블)를 그대로 유지**하면서 분석/전처리를 이어가고 싶을 때


중복 샘플을 제거하였습니다. 중복이 제거되었는지 전체 샘플 수를 확인합니다.

```python
print('총 샘플의 수 :',len(train_data))
```

```python
총 샘플의 수 : 146183
```

중복 샘플이 제거되었습니다. train_data에서 해당 리뷰의 긍, 부정 유무가 기재되어있는 레이블(label) 값의 분포를 보겠습니다.


**`df['col'].value_counts()`**: 해당 열에 있는 값 종류별로 개수(빈도수)를 세어 내림차순으로 정렬합니다.

```python
train_data['label'].value_counts().plot(kind = 'bar')
```

![](https://static.wikidocs.net/images/page/44249/label_distribution.PNG)

앞서 확인하였듯이 약 146,000개의 영화 리뷰 샘플이 존재하는데 그래프 상으로 긍정과 부정 둘 다 약 72,000개의 샘플이 존재하여 레이블의 분포가 균일한 것처럼 보입니다. 정확하게 몇 개인지 확인해봅시다.

```python
print(train_data.groupby('label').size().reset_index(name = 'count'))
```

```python
   label  count
0      0  73342
1      1  72841
```

레이블이 0인 리뷰가 근소하게 많습니다. 리뷰 중에 Null 값을 가진 샘플이 있는지 확인합니다.

```python
print(train_data.isnull().values.any())
# 미리 null 여부 확인해서 숏컷하기 편하게 해주는 함수
```

```python
True
```

True가 나왔다면 데이터 중에 Null 값을 가진 샘플이 존재한다는 의미입니다. 어떤 열에 존재하는지 확인해봅시다.

```python
print(train_data.isnull().sum())
# Null 값을 가진 샘플이 없어도 오류를 뱉지 않으니
# isnull().values.any()로 굳이 2차 확인하지 않아도 됨.
```

```python
id          0
document    1
label       0
dtype: int64
```

리뷰가 적혀있는 document 열에서 Null 값을 가진 샘플이 총 1개가 존재한다고 합니다. 
그렇다면 document 열에서 Null 값이 존재한다는 것을 조건으로 Null 값을 가진 샘플이 어느 인덱스의 위치에 존재하는지 한 번 출력해봅시다.

```python
train_data.loc[train_data.document.isnull()]
```

![](https://static.wikidocs.net/images/page/44249/navermoive4new.PNG)

출력 결과는 위와 같습니다. Null 값을 가진 샘플을 제거하겠습니다.

```python
train_data = train_data.dropna(how = 'any') # Null 값이 존재하는 행 제거
print(train_data.isnull().values.any()) # Null 값이 존재하는지 확인
```

```python
False
```

Null 값을 가진 샘플이 제거되었습니다. 
다시 샘플의 개수를 출력하여 1개의 샘플이 제거되었는지 확인해봅시다.

```python
print(len(train_data))
```

```python
146182
```

데이터의 전처리를 수행해보겠습니다. 


위의 train_data와 test_data에서 온점(.)이나 ?와 같은 각종 특수문자가 사용된 것을 확인했습니다. train_data로부터 한글만 남기고 제거하기 위해서 정규 표현식을 사용해보겠습니다.

우선 영어를 예시로 정규 표현식을 설명해보겠습니다. 영어의 알파벳들을 나타내는 정규 표현식은 \[a-zA-Z]입니다. 이 정규 표현식은 영어의 소문자와 대문자들을 모두 포함하고 있는 정규 표현식으로 이를 응용하면 영어에 속하지 않는 구두점이나 특수문자를 제거할 수 있습니다. 예를 들어 알파벳과 공백을 제외하고 모두 제거하는 전처리를 수행하는 예제는 다음과 같습니다.

```python
#알파벳과 공백을 제외하고 모두 제거
eng_text = 'do!!! you expect... people~ to~ read~ the FAQ, etc. and actually accept hard~! atheism?@@'
print(re.sub(r'[^a-zA-Z ]', '', eng_text))
```

```python
'do you expect people to read the FAQ etc and actually accept hard atheism'
```

위와 같은 원리를 한국어 데이터에 적용하고 싶다면, 우선 한글을 범위 지정할 수 있는 정규 표현식을 찾아내면 되겠습니다. 

우선 자음과 모음에 대한 범위를 지정해보겠습니다. 일반적으로 자음의 범위는 ㄱ ~ ㅎ, 모음의 범위는 ㅏ ~ ㅣ와 같이 지정할 수 있습니다. 

해당 범위 내에 어떤 자음과 모음이 속하는지 알고 싶다면 아래의 링크를 참고하시기 바랍니다.

링크 : https://www.unicode.org/charts/PDF/U3130.pdf  
ㄱ ~ ㅎ: 3131 ~ 314E  
ㅏ ~ ㅣ: 314F ~ 3163

완성형 한글의 범위는 가 ~ 힣과 같이 사용합니다. 해당 범위 내에 포함된 음절들은 아래의 링크에서 확인할 수 있습니다.

- 링크 : https://www.unicode.org/charts/PDF/UAC00.pdf

위 범위 지정을 모두 반영하여 train_data에 한글과 공백을 제외하고 모두 제거하는 정규 표현식을 수행해봅시다.

```python
# 한글과 공백을 제외하고 모두 제거 (regex true 로 주면 regex 기준으로 텍스트매칭)
train_data['document'] = train_data['document'].str.replace("[^ㄱ-ㅎㅏ-ㅣ가-힣 ]","", regex=True)
train_data[:5]
```

![](https://static.wikidocs.net/images/page/44249/navermovie5.PNG)

상위 5개의 샘플을 다시 출력해보았는데, 정규 표현식을 수행하자 기존의 << 공백. 즉, 띄어쓰기는 유지되면서 온점과 같은 구두점 등은 제거>>되었습니다. 

사실 네이버 영화 리뷰는 한글이 아니더라도 영어, 숫자, 특수문자로도 리뷰를 업로드할 수 있습니다. 다시 말해 기존에 한글이 없는 리뷰였다면 더 이상 아무런 값도 없는 빈(empty) 값이 되었을 것입니다. 

train_data에 공백(whitespace)만 있거나 빈 값을 가진 행이 있다면 
Null 값으로 변경하도록 하고, Null 값이 존재하는지 확인해보겠습니다.

```python
train_data['document'] = train_data['document'].str.replace('^ +', "", regex=True) # white space 데이터를 empty value로 변경
train_data['document'].replace('', np.nan, inplace=True)
print(train_data.isnull().sum())
```

```python
id            0
document    789
label         0
dtype: int64
```

Null 값이 789개나 새로 생겼습니다. Null 값이 있는 행을 5개만 출력해볼까요?

```python
train_data.loc[train_data.document.isnull()][:5]
```

### `loc` (Location의 약자)

`loc`은 ==데이터프레임==에서 "라벨(이름)을 기준으로 원하는 행(Row)과 열(Column)을 선택(슬라이싱)"할 때 사용하는 속성입니다.

- 기본 형태: `df.loc[행_조건_또는_라벨, 열_조건_또는_라벨]`
    

![](https://static.wikidocs.net/images/page/44249/top_null_data.PNG)

Null 샘플들은 레이블이 긍정일 수도 있고, 부정일 수도 있습니다. 
아무런 의미도 없는 데이터므로 제거해줍니다.

```python
train_data = train_data.dropna(how = 'any')
print(len(train_data))
```

```python
145393
```

샘플 개수가 또 다시 줄어서 145,393개가 남았습니다. 테스트 데이터에 앞서 진행한 전처리 과정을 동일하게 진행합니다.

```python
test_data.drop_duplicates(subset = ['document'], inplace=True) # document 열에서 중복인 내용이 있다면 중복 제거
test_data['document'] = test_data['document'].str.replace("[^ㄱ-ㅎㅏ-ㅣ가-힣 ]","", regex=True) # 정규 표현식 수행
test_data['document'] = test_data['document'].str.replace('^ +', "", regex=True) # 공백은 empty 값으로 변경
test_data['document'].replace('', np.nan, inplace=True) # 공백은 Null 값으로 변경
test_data = test_data.dropna(how='any') # Null 값 제거
print('전처리 후 테스트용 샘플의 개수 :',len(test_data))
```

```python
전처리 후 테스트용 샘플의 개수 : 48852
```

dropna 함수 설명

- **`how='any'` (기본값):** 지정한 축(행 또는 열)에 결측치가 **단 1개라도 있으면** 해당 행/열을 제거합니다.
    
- **`how='all'`:** 지정한 축의 모든 값이 결측치일 때만 해당 행/열을 제거합니다. (일부만 NaN인 행은 살아남음)

### `dropna()`의 주요 인자 (Parameters)

| **인자**        | **기본값** | **역할 및 설명**                                                                                                         |
| ------------- | ------- | ------------------------------------------------------------------------------------------------------------------- |
| **`axis`**    | `0`     | **`0` 또는 `'index'`**: 결측치가 있는 행(Row)을 삭제<br><br>`1` 또는 `'columns'`: 결측치가 있는 **열(Column)**을 삭제                       |
| **`subset`**  | `None`  | **특정 컬럼(또는 행)만 지정**하여 결측치 검사<br><br>예: `df.dropna(subset=['age', 'email'])` $\rightarrow$ age나 email에 NaN이 있을 때만 삭제 |
| **`thresh`**  | `None`  | **최소한 남겨야 할 정상(Non-NaN) 데이터의 개수** 지정<br><br>예: `thresh=3` $\rightarrow$ NaN이 아닌 정상 값이 최소 3개 이상이어야 살아남음              |
| **`inplace`** | `False` | `True`로 설정하면 새로운 데이터프레임을 반환하지 않고 <br>**원본을 직접 수정**                                                                  |


### 3) 토큰화

토큰화를 진행해봅시다. 토큰화 과정에서 불용어를 제거하겠습니다. 
불용어는 정의하기 나름인데, 한국어의 조사, 접속사 등의 보편적인 불용어를 사용할 수도 있겠지만 결국 풀고자 하는 문제의 데이터를 지속 검토하면서 계속해서 추가하는 경우 또한 많습니다.

실제 현업인 상황이라면 일반적으로 아래의 불용어보다 더 많은 불용어를 사용할 수 있습니다.

```python
stopwords = ['도', '는', '다', '의', '가', '이', '은', '한', '에', '하', '고', '을', '를', '인', '듯', '과', '와', '네', '들', '듯', '지', '임', '게']
```

여기서는 위 정도로만 불용어를 정의하고, 
토큰화를 위한 형태소 분석기는 KoNLPy의 Mecab을 사용합니다. Mecab을 복습해봅시다.

```python
mecab = Mecab()
mecab.morphs('와 이런 것도 영화라고 차라리 뮤직비디오를 만드는 게 나을 뻔')
```


```python
['와', '이런', '것', '도', '영화', '라고', '차라리', '뮤직', '비디오', '를', '만드', '는', '게', '나을', '뻔'
```

한국어을 토큰화할 때는 영어처럼 띄어쓰기 기준으로 토큰화를 하는 것이 아니라, 
주로 형태소 분석기를 사용한다고 언급한 바 있습니다. 
train_data에 형태소 분석기를 사용하여 토큰화를 하면서 불용어를 제거하여 X_train에 저장합니다.

```python
X_train = []
for sentence in tqdm(train_data['document']):
    tokenized_sentence = mecab.morphs(sentence) # 토큰화
    stopwords_removed_sentence = [word for word in tokenized_sentence if not word in stopwords] # 불용어 제거
    X_train.append(stopwords_removed_sentence)
```


### `tqdm` 함수 완전 정리

`tqdm`은 아랍어로 "진행"을 뜻하는 taqaddum(taqaddum)과 스페인어로 "너무 좋아해"라는 te quiero mucho에서 유래한 이름으로, **반복문(loop)의 진행 상황을 시각적인 게이지(Progress Bar)로 보여주는 파이썬 라이브러리**입니다.

텍스트 전처리나 대용량 데이터프레임 처리처럼 시간이 오래 걸리는 작업에서 필수적으로 쓰입니다. 

- `tqdm()`으로 리스트나 iterator를 감싸주기만 하면 됩니다.

+ Pandas의 `.apply()` 작업에 tqdm 진행바를 출력하고 싶을 때는 `tqdm.pandas()`를 먼저 선언해 준 뒤, `apply()` 대신 `progress_apply()`를 사용합니다.

---

상위 3개의 샘플만 출력하여 결과를 확인해봅시다.

```python
print(X_train[:3])
```

```python
[['아', '더', '빙', '진짜', '짜증', '나', '네요', '목소리'], ['흠', '포스터', '보고', '초딩', '영화', '줄', '오버', '연기', '조차', '가볍', '않', '구나'], ['너무', '재', '밓었다그래서보는것을추천한다']]
```

토큰화가 진행된 것을 볼 수 있습니다. 테스트 데이터에 대해서도 동일하게 토큰화를 해줍니다.

```python
X_test = []
for sentence in tqdm(test_data['document']):
    tokenized_sentence = mecab.morphs(sentence) # 토큰화
    stopwords_removed_sentence = [word for word in tokenized_sentence if not word in stopwords] # 불용어 제거
    X_test.append(stopwords_removed_sentence)
```

지금까지 훈련 데이터와 테스트 데이터에 대해서 텍스트 전처리를 진행해보았습니다. 
이제 학습 데이터와 검증 데이터, 그리고 테스트 데이터를 준비해보겠습니다.

### 4) 학습 데이터, 검증 데이터, 테스트 데이터

이미 학습 데이터와 테스트 데이터는 준비되었지만 학습하는 동안의 성능 평가를 진행할 검증 데이터가 추가로 필요합니다. 데이터프레임의 레이블 열을 별도로 분리하여 y_train과 y_test로 저장해줍니다. 이제 학습 데이터는 X_train, y_train에 저장되고, 테스트 데이터는 X_test, y_test에 저장이 될 것입니다.

<< 학습 데이터 중에서 20%를 분할하여 추가로 검증 데이터를 만들어줍니다. >>  
머신 러닝 문제를 풀 때, 데이터의 분리는 주로 사이킷런에서 제공하는 train_test_split을 사용해 진행합니다. test_size에 비율을 넣어주면 기존 데이터에 대해서 해당 비율만큼 일부 데이터를 분할하여 반환합니다.

랜덤으로 분할하는 과정에서 레이블 불균형이 발생하지 않도록, 
레이블의 ==균형 비율을 유지하면서 분할하고 싶다면 분할 시 기존 데이터의 y데이터를 stratify의 값으로 사용== 하면 됩니다.

```python
y_train = np.array(train_data['label'])
y_test = np.array(test_data['label'])

X_train, X_valid, y_train, y_valid = train_test_split(X_train, y_train, test_size=0.2, random_state=0, stratify=y_train)
```

실제로 비율이 잘 유지되면서 분할되었는지 확인해봅시다.

```python
print('--------학습 데이터의 비율-----------')
print(f'부정 리뷰 = {round(np.sum(y_train==0)/len(y_train) * 100,3)}%')
print(f'긍정 리뷰 = {round(np.count_nonzero(y_train)/len(y_train) * 100,3)}%')
print('--------검증 데이터의 비율-----------')
print(f'부정 리뷰 = {round(np.sum(y_valid==0)/len(y_valid) * 100,3)}%')
print(f'긍정 리뷰 = {round(np.count_nonzero(y_valid)/len(y_valid) * 100,3)}%')
print('--------테스트 데이터의 비율-----------')
print(f'부정 리뷰 = {round(np.sum(y_test==0)/len(y_test) * 100,3)}%')
print(f'긍정 리뷰 = {round(np.count_nonzero(y_test)/len(y_test) * 100,3)}%')
```

```python
--------학습 데이터의 비율-----------
부정 리뷰 = 50.238%
긍정 리뷰 = 49.762%
--------검증 데이터의 비율-----------
부정 리뷰 = 50.239%
긍정 리뷰 = 49.761%
--------테스트 데이터의 비율-----------
부정 리뷰 = 49.808%
긍정 리뷰 = 50.192%
```

<<< 분할 후에도 학습 데이터와 검증 데이터의 레이블 비율이 동일 >>> 한 것을 확인할 수 있습니다.

### 5) 단어 집합 만들기

기계가 텍스트를 숫자로 처리할 수 있도록 
훈련 데이터와 테스트 데이터에 정수 인코딩을 수행해야 합니다. 


우선, 훈련 데이터에 대해서 단어 집합(vocaburary)을 만들어봅시다.

```python
word_list = []
for sent in X_train:
    for word in sent:
      word_list.append(word)

word_counts = Counter(word_list)
print('총 단어수 :', len(word_counts))
```

```python
총 단어수 : 45296
```

단어가 45,000개가 넘게 존재합니다. 
등장 빈도를 카운트 하는 Counter()를 사용하였기 때문에 각 단어의 등장 빈도가 저장되어져 있습니다. 단어 `영화`와 `공감`의 등장 빈도를 출력합니다.

```python
print('훈련 데이터에서의 단어 영화의 등장 횟수 :', word_counts['영화'])
print('훈련 데이터에서의 단어 공감의 등장 횟수 :', word_counts['공감'])
```

```python
훈련 데이터에서의 단어 영화의 등장 횟수 : 45791
훈련 데이터에서의 단어 공감의 등장 횟수 : 756
```

등장 빈도수가 높은 순서대로 단어들을 정렬해봅시다.

```python
vocab = sorted(word_counts, key=word_counts.get, reverse=True)
print('등장 빈도수 상위 10개 단어')
print(vocab[:10])
```

```python
등장 빈도수 상위 10개 단어
['영화', '보', '있', '없', '좋', '나', '었', '만', '는데', '너무']
```

여기서는 빈도수가 낮은 단어들은 자연어 처리에서 배제하고자 합니다. 
등장 빈도수가 3회 미만인 단어들이 이 데이터에서 얼만큼의 비중을 차지하는지 확인해봅시다.

```python
threshold = 3
total_cnt = len(word_counts) # 단어의 수
rare_cnt = 0 # 등장 빈도수가 threshold보다 작은 단어의 개수를 카운트
total_freq = 0 # 훈련 데이터의 전체 단어 빈도수 총 합
rare_freq = 0 # 등장 빈도수가 threshold보다 작은 단어의 등장 빈도수의 총 합

# 단어와 빈도수의 쌍(pair)을 key와 value로 받는다.
for key, value in word_counts.items():
    total_freq = total_freq + value

    # 단어의 등장 빈도수가 threshold보다 작으면
    if(value < threshold):
        rare_cnt = rare_cnt + 1
        rare_freq = rare_freq + value

print('단어 집합(vocabulary)의 크기 :',total_cnt)
print('등장 빈도가 %s번 이하인 희귀 단어의 수: %s'%(threshold - 1, rare_cnt))
print("단어 집합에서 희귀 단어의 비율:", (rare_cnt / total_cnt)*100)
print("전체 등장 빈도에서 희귀 단어 등장 빈도 비율:", (rare_freq / total_freq)*100)
```

```python
단어 집합(vocabulary)의 크기 : 45296
등장 빈도가 2번 이하인 희귀 단어의 수: 26105
단어 집합에서 희귀 단어의 비율: 57.63202048746025
전체 등장 빈도에서 희귀 단어 등장 빈도 비율: 2.2769635638286716
```

등장 빈도가 threshold 값인 3회 미만. 즉, 2회 이하인 단어들은 단어 집합에서 무려 절반 이상을 차지합니다. 하지만, 실제로 훈련 데이터에서 등장 빈도로 차지하는 비중은 상대적으로 매우 적은 수치인 2.27%밖에 되지 않습니다. 아무래도 등장 빈도가 2회 이하인 단어들은 자연어 처리에서 별로 중요하지 않을 듯 합니다. 그래서 이 단어들은 정수 인코딩 과정에서 배제시키겠습니다.

등장 빈도수가 2이하인 단어들의 수를 제외한 단어의 개수를 단어 집합의 최대 크기로 제한하겠습니다.

```python
# 전체 단어 개수 중 빈도수 2이하인 단어는 제거.
vocab_size = total_cnt - rare_cnt
vocab = vocab[:vocab_size]
print('단어 집합의 크기 :', len(vocab))
```

```python
단어 집합의 크기 : 19191
```

단어 집합의 크기는 19,191개입니다. 

이제 패딩 토큰와 모르는 단어에 대응하기 위해서 실제 의미를 가지는 단어는 아니지만 임의로 단어 집합에 `<PAD>`와 `<UNK>`를 추가합니다. 

이렇게 특별한 용도로 사용되는 단어들을 <<< 스페셜 토큰(Special Token) >>> 이라고 합니다. 두 개의 스페셜 토큰은 각각 정수 0과 1에 할당하고, 두 개의 스페셜 토큰이 추가된 후의 단어 집합의 크기를 확인해봅시다.

```python
word_to_index = {}
word_to_index['<PAD>'] = 0
word_to_index['<UNK>'] = 1
```

```python
for index, word in enumerate(vocab) :
  word_to_index[word] = index + 2
```

```python
vocab_size = len(word_to_index)
print('패딩 토큰과 UNK 토큰을 고려한 단어 집합의 크기 :', vocab_size)
```

```python
패딩 토큰과 UNK 토큰을 고려한 단어 집합의 크기 : 19193
```

단어 집합의 크기는 19,193입니다.

```python
print('단어 <PAD>와 맵핑되는 정수 :', word_to_index['<PAD>'])
print('단어 <UNK>와 맵핑되는 정수 :', word_to_index['<UNK>'])
print('단어 영화와 맵핑되는 정수 :', word_to_index['영화'])
```

```python
단어 <PAD>와 맵핑되는 정수 : 0
단어 <UNK>와 맵핑되는 정수 : 1
단어 영화와 맵핑되는 정수 : 2
```



### 6) 정수 인코딩

이제 정수 인코딩을 진행해봅시다. 현재 등장 빈도가 2회 이하인 단어들은 단어 집합에서 제거하였으므로 정수 인코딩 과정에서는 단어 집합에 존재하지 않는 단어들은 일괄로 `<UNK>`로 맵핑합니다. 다시 말해서 정수 1로 맵핑합니다.

```python
def texts_to_sequences(tokenized_X_data, word_to_index):
  encoded_X_data = []
  for sent in tokenized_X_data:
    index_sequences = []
    for word in sent:
      try:
          index_sequences.append(word_to_index[word])
      except KeyError:
          index_sequences.append(word_to_index['<UNK>'])
    encoded_X_data.append(index_sequences)
  return encoded_X_data
```

훈련 데이터, 검증 데이터, 테스트 데이터에 대해서 전부 정수 인코딩을 진행합니다.

```python
encoded_X_train = texts_to_sequences(X_train, word_to_index)
encoded_X_valid = texts_to_sequences(X_valid, word_to_index)
encoded_X_test = texts_to_sequences(X_test, word_to_index)
```

정수 인코딩 후의 상위 2개 샘플을 출력해봅시다.

```python
# 상위 샘플 2개 출력
for sent in encoded_X_train[:2]:
  print(sent)
```

```python
[924, 1866, 128, 7, 80, 48, 34]
[2415, 3138, 4, 2095, 422, 87, 5768, 19, 307]
```

단어를 key로, 정수를 value로 가지는 단어 집합 딕셔너리인 word_to_index의 key와 value를 뒤집어서 정수가 key이고, 단어가 value인 index_to_word를 만들어봅시다. 

그리고 index_to_word를 이용하여 정수 인코딩 결과를 역으로 다시 텍스트로 변환해봅시다. 이를 디코딩이라고 합니다.

```python
index_to_word = {}
for key, value in word_to_index.items():
    index_to_word[value] = key
```

```python
decoded_sample = [index_to_word[word] for word in encoded_X_train[0]]
print('기존의 첫번째 샘플 :', X_train[0])
print('복원된 첫번째 샘플 :', decoded_sample)
```

```python
기존의 첫번째 샘플 : ['이야', '어쩜', '이렇게', '나', '지루', '할', '수']
복원된 첫번째 샘플 : ['이야', '어쩜', '이렇게', '나', '지루', '할', '수']
```

### 7) 패딩

서로 다른 길이의 샘플들의 길이를 동일하게 맞춰주는 패딩 작업을 진행해보겠습니다. 
전체 데이터에서 가장 길이가 긴 리뷰와 전체 데이터의 길이 분포를 알아보겠습니다.

```python
print('리뷰의 최대 길이 :',max(len(review) for review in encoded_X_train))
print('리뷰의 평균 길이 :',sum(map(len, encoded_X_train))/len(encoded_X_train))
plt.hist([len(review) for review in encoded_X_train], bins=50)
plt.xlabel('length of samples')
plt.ylabel('number of samples')
plt.show()
```

```python
리뷰의 최대 길이 : 74
리뷰의 평균 길이 : 12.296731261928917
```

![](https://static.wikidocs.net/images/page/217687/%EA%B8%B8%EC%9D%B4%EB%B6%84%ED%8F%AC.PNG)

가장 긴 리뷰의 길이는 74이며, 그래프를 봤을 때 전체 데이터의 길이 분포는 대체적으로 약 12내외의 길이를 가지는 것을 볼 수 있습니다. 

모델이 처리할 수 있도록 X_train과 X_test의 모든 샘플의 길이를 특정 길이로 동일하게 맞춰줄 필요가 있습니다. 특정 길이 변수를 max_len으로 정합니다. 

대부분의 리뷰가 내용이 잘리지 않도록 할 수 있는 최적의 max_len의 값은 몇일까요? 
전체 샘플 중 길이가 max_len 이하인 샘플의 비율이 몇 %인지 확인하는 함수를 만듭니다.

```python
def below_threshold_len(max_len, nested_list):
  count = 0
  for sentence in nested_list:
    if(len(sentence) <= max_len):
        count = count + 1
  print('전체 샘플 중 길이가 %s 이하인 샘플의 비율: %s'%(max_len, (count / len(nested_list))*100))
```

위의 분포 그래프를 봤을 때, max_len = 30이 적당할 것 같습니다. 
이 값이 얼마나 많은 리뷰 길이를 커버하는지 확인해봅시다.

```python
max_len = 30
below_threshold_len(max_len, X_train)
```

```python
전체 샘플 중 길이가 30 이하인 샘플의 비율: 92.49703389101914
```

전체 훈련 데이터 중 약 92%의 리뷰가 30이하의 길이를 가지는 것을 확인했습니다. 
모든 샘플의 길이를 30으로 맞추겠습니다.

```python
def pad_sequences(sentences, max_len):
  features = np.zeros((len(sentences), max_len), dtype=int)
  for index, sentence in enumerate(sentences):
    if len(sentence) != 0:
      features[index, :len(sentence)] = np.array(sentence)[:max_len]
  return features

padded_X_train = pad_sequences(encoded_X_train, max_len=max_len)
padded_X_valid = pad_sequences(encoded_X_valid, max_len=max_len)
padded_X_test = pad_sequences(encoded_X_test, max_len=max_len)

print('훈련 데이터의 크기 :', padded_X_train.shape)
print('검증 데이터의 크기 :', padded_X_valid.shape)
print('테스트 데이터의 크기 :', padded_X_test.shape)

# feature 개수를 맞춰줘야 matrix 연산 (텐서) 가능함
```

```python
훈련 데이터의 크기 : (116314, 30)
검증 데이터의 크기 : (29079, 30)
테스트 데이터의 크기 : (48852, 30)
```

훈련 데이터의 첫번째 샘플을 출력해봅시다.

```python
print('첫번째 샘플의 길이 :', len(padded_X_train[0]))
print('첫번째 샘플 :', padded_X_train[0])
```

```python
첫번째 샘플의 길이 : 30
첫번째 샘플 : [ 924 1866  128    7   80   48   34    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0    0]
```

훈련 데이터의 첫번째 샘플을 출력하면 길이 30을 맞추기 위해서 뒤에 숫자 0이 붙어있는 것을 확인할 수 있습니다.

## 2. LSTM을 이용한 네이버 영화 리뷰 분류 모델

이제 딥 러닝 프레임워크 PyTorch를 이용하여 LSTM 모델을 구현해봅시다.

```python
import torch
import torch.nn as nn
import torch.nn.functional as F
```

현재 실습 환경에서 GPU를 사용 가능한지 확인합니다.

```python
USE_CUDA = torch.cuda.is_available()
device = torch.device("cuda" if USE_CUDA else "cpu")
print("cpu와 cuda 중 다음 기기로 학습함:", device)
```

```python
cpu와 cuda 중 다음 기기로 학습함: cuda
```

저자의 경우 Colab에서 GPU를 선택하여 실습을 진행하여 cuda라는 출력 결과를 확인했습니다. 레이블 데이터를 파이토치의 텐서 타입으로 변환합니다. 
이후 훈련 데이터의 상위 5개의 레이블을 출력해보았습니다.

```python
train_label_tensor = torch.tensor(np.array(y_train))
valid_label_tensor = torch.tensor(np.array(y_valid))
test_label_tensor = torch.tensor(np.array(y_test))
print(train_label_tensor[:5])
```

```python
tensor([1, 1, 0, 0, 0])
```

이제 LSTM 모델을 클래스로 구현해봅시다. 
각 층을 지날 때마다 각 층의 출력의 크기를 이해하는 것이 중요합니다. 
예를 들어 입력은 (배치 크기, 문장 길이)의 크기를 가지는 텐서입니다. 임베딩 층을 지나고 나면 각 단어가 임베딩 벡터로 변환되면서 (배치 크기, 문장 길이, 임베딩 벡터의 차원)으로 텐서의 크기가 변환됩니다.

이 후 LSTM의 마지막 시점의 은닉 상태(hidden state) 값을 출력층과 연결시키는 작업을 해주어야 합니다. (현재 상태를 얼마나 보낼지, 장기기억으로 이어지는 hidden state 값을 출력층에 가산하는 과정이 필수적임)

이때 LSTM이 출력층으로 보는 결과값의 차원은 (배치 크기, 은닉 상태의 차원)을 가져야 합니다. 마지막 시점의 은닉 상태의 값만 전달하므로 은닉 상태는 모든 시점(문장 길이)만큼 존재하는 것이 아니라 단 하나만 있습니다. (절편을 캡처하는게아니라 연속해서 나감. 펼쳐진 RNN은 설명을 위한 것이고 셀 상태 자체는 스스로 순환하면서 sequencial 하게 나간다고이해하면될듯)

출력층은 지난 결과는 소프트맥스 회귀를 수행하므로 (배치 크기, 분류하고자하는 카테고리의 수)의 차원을 가지게 됩니다.

그 후 각 데이터를 배치 단위로 데이터 묶음을 꺼낼 수 있는 데이터로더로 전달합니다. 정리하면 다음과 같습니다. 아직 모델을 만들지는 않았지만, 단어 벡터의 차원을 100, 배치 크기를 32, 문장 길이를 500(패딩 후), LSTM의 은닉 상태의 차원을 128로 한다고 가정해보겠습니다.

```diff
- 단어 벡터의 차원 = 100 (클수록 모델의 data-적합도 높아짐 과적합 방향.)
- 문장 길이 = 500
- 배치 크기 = 32
- 데이터 개수 = 2만
- LSTM의 은닉층의 크기 = 128
- 분류하고자 하는 카테고리 개수 = 2개
```

위의 정보들을 고려하였을 때 모델 내부에서 데이터의 변화는 다음과 같습니다.

- (32, 500) => 입력 데이터의 형태 => 임베딩 층 통과 후 => (32, 500, 100) => LSTM 통과 후 => (32, 128) => Softmax 출력층 통과 후 => (32, 2)

```python
class TextClassifier(nn.Module):
    def __init__(self, vocab_size, embedding_dim, hidden_dim, output_dim):
        super(TextClassifier, self).__init__()
        self.embedding = nn.Embedding(vocab_size, embedding_dim)
        self.lstm = nn.LSTM(embedding_dim, hidden_dim, batch_first=True)
        self.fc = nn.Linear(hidden_dim, output_dim)

    def forward(self, x):
        # x: (batch_size, seq_length)
        embedded = self.embedding(x)  
        # (batch_size, seq_length, embedding_dim)
        # float32 (표준. 최적화 시 : float16 bfloat16)

        # LSTM은 (hidden state, cell state)의 튜플을 반환합니다
        lstm_out, (hidden, cell) = self.lstm(embedded)  # lstm_out: (batch_size, seq_length, hidden_dim), hidden: (1, batch_size, hidden_dim)

        last_hidden = hidden.squeeze(0)  # (batch_size, hidden_dim)
        logits = self.fc(last_hidden)  # (batch_size, output_dim)
        return logits
```

self.lstm 이 반환하는 값은 실습코드 주석으로 확인

#### 💡 실무/실전 팁 (`lstm_out`으로 감성 분석을 하는 또 다른 방법)

실제로 감성 분석을 할 때 `hidden` 대신 **`lstm_out`을 활용하는 기법**도 매우 자주 쓰입니다.

- **방법 1 (현재 코드 방식):** `hidden`을 가져와서 맨 마지막 시점의 은닉 상태만 Linear 층에 전달
    
- **방법 2 (`lstm_out` 활용):** `lstm_out`에서 모든 시점의 벡터를 가져온 뒤 평균(Mean Pooling) 또는 최대값(Max Pooling)을 내어 문장 전체의 의미를 압축한 후 Linear 층에 전달


```Python
# lstm_out을 활용한 감성 분석 예시 (Mean Pooling)
# (batch_size, seq_length, hidden_dim) -> (batch_size, hidden_dim)
out = lstm_out.mean(dim=1)
logits = self.fc(out)
```

따라서 지금은 **"단순 분류(감성 분석)에서는 `hidden`을 쓰고, 번역/태깅이나 어텐션을 붙일 때는 `lstm_out`을 쓰는구나"** 하고 정리해 두시면 완벽합니다!

(혹은 다층 LSTM, 매 단어마다 정답 출력해야 하는 경우에 lstm_out 이 사용되고
 ==트랜스포머 어텐션으로 이어질때의 발상이 여기서 나온거==라고 함)
####

훈련 데이터, 검증 데이터, 테스트 데이터에 대해서 파이토치 텐서로 변환하고 배치 단위 연산을 위해 데이터로더로 변환합니다.

```python
encoded_train = torch.tensor(padded_X_train).to(torch.int64)
train_dataset = torch.utils.data.TensorDataset(encoded_train, train_label_tensor)
train_dataloader = torch.utils.data.DataLoader(train_dataset, shuffle=True, batch_size=32)

encoded_test = torch.tensor(padded_X_test).to(torch.int64)
test_dataset = torch.utils.data.TensorDataset(encoded_test, test_label_tensor)
test_dataloader = torch.utils.data.DataLoader(test_dataset, shuffle=True, batch_size=1)

encoded_valid = torch.tensor(padded_X_valid).to(torch.int64)
valid_dataset = torch.utils.data.TensorDataset(encoded_valid, valid_label_tensor)
valid_dataloader = torch.utils.data.DataLoader(valid_dataset, shuffle=True, batch_size=1)
```

훈련 데이터의 샘플 개수가 116,314개 였으므로 배치 크기를 32로 할 경우에는 116,324/32=3,635 다시 말해 32개씩 묶인 데이터 묶음이 3,635개가 생깁니다. 

그리고 학습 시에는 32개씩 데이터가 들어가게 될 것입니다.

```python
total_batch = len(train_dataloader)
print('총 배치의 수 : {}'.format(total_batch))
```

```python
총 배치의 수 : 3635
```

모델 객체를 선언합니다.

```python
embedding_dim = 100
hidden_dim = 128
output_dim = 2
learning_rate = 0.01
num_epochs = 10

model = TextClassifier(vocab_size, embedding_dim, hidden_dim, output_dim)
model.to(device)
```

임베딩 벡터의 차원은 128, 출력층의 크기(분류해야 할 카테고리의 개수)는 2로 정했습니다. 
이렇게 사용자가 정해주는 값이면서 모델의 결과에 영향을 미치는 값들을 하이퍼파라미터라고 합니다. 

소프트맥스 회귀를 통해 분류 문제를 진행하므로 손실 함수는 nn.CrossEntropyLoss()를 사용합니다. 파이토치로 자연어 처리를 하게 되면 가장 많이 사용하게 되는 손실 함수입니다. 

하이퍼파라미터 중 하나인 학습률(learning rate)는 0.001로 정했습니다.

```python
criterion = nn.CrossEntropyLoss()
optimizer = torch.optim.Adam(model.parameters(), lr=0.001)
```



## 3. 평가 코드 작성

이후 평가를 진행하기 위해서 모델의 정확도를 측정하는 함수 `calculate_accuracy()`를 작성합니다.

```python
def calculate_accuracy(logits, labels):
    # _, predicted = torch.max(logits, 1)
    predicted = torch.argmax(logits, dim=1)
    # 행(dim=1) 방향으로 가장 큰 값과 그 값이 있는 위치(인덱스)를 함께 반환
    # (최대값 텐서, 인덱스 텐서)
    
    correct = (predicted == labels).sum().item()
    # sum()까지만하면 텐서연산 특징때문에 오류날수있음
    # 텐서 말고 int 로 저장하게해서 sum연산오류 방지
    
    total = labels.size(0)
    accuracy = correct / total
    return accuracy
```


검증 데이터와 테스트 데이터에 대한 성능을 측정하기 위한 함수 `evaluate()`를 작성합니다. 
아래의 함수에서 `model.eval()`과 `with torch.no_grad()`를 짚어봅시다. 이 두 개는 모델 평가를 수행할 때 중요한 역할을 합니다. 각각의 의미는 다음과 같습니다.

- model.eval(): 모델을 평가 모드로 설정합니다. 이렇게 하면 모델 내부의 모든 레이어에 대해 평가 모드가 활성화됩니다. 일부 레이어, 예를 들어 드롭아웃이나 배치 정규화는 학습과 평가 시 다르게 동작하기 때문에 이 설정이 중요합니다. 평가 모드가 설정되지 않으면, 이러한 레이어의 동작이 올바르지 않을 수 있으며, 이로 인해 평가 결과가 제대로 나오지 않을 수 있습니다.
	(review : 모델의 학습을 끄고 eval로 들어가는것. 역전파 안하기, 그래프 안그리기 이런거.)
    
- with torch.no_grad(): 이 문장은 자동 미분 엔진에서 기울기(gradient) 계산을 비활성화합니다. 평가 중에는 기울기를 계산할 필요가 없으므로, 이렇게 설정하면 메모리를 절약하고 속도를 높일 수 있습니다. 만약 이 설정이 적용되지 않으면, 평가 과정에서 기울기(gradient)가 계산되고 메모리를 차지하게 됩니다. 그러나 평가 결과 자체에는 직접적인 영향을 주지 않습니다. (이거랑 eval이랑 같이 관습적으로 쓰임. 위 설명과 같이 자동미분 그래프를 메모리에 안 올리고 오로지 평가만 하도록 세팅할 수 있음)
    

따라서 ==model.eval()은 평가 시 반드시 사용==해야 하며, 그렇지 않으면 평가 결과가 올바르게 나오지 않을 수 있습니다. with torch.no_grad():는 필수는 아니지만, 메모리와 속도 측면에서 권장됩니다.

```python
def evaluate(model, valid_dataloader, criterion, device):
    val_loss = 0
    val_correct = 0
    val_total = 0

    model.eval()
    with torch.no_grad():
        # 데이터로더로부터 배치 크기만큼의 데이터를 연속으로 로드
        for batch_X, batch_y in valid_dataloader:
            batch_X, batch_y = batch_X.to(device), batch_y.to(device)

            # 모델의 예측값
            logits = model(batch_X)

            # 손실을 계산
            loss = criterion(logits, batch_y)

            # 정확도와 손실을 계산함
            val_loss += loss.item()
            val_correct += calculate_accuracy(logits, batch_y) 
				            * batch_y.size(0)
            val_total += batch_y.size(0)

    val_accuracy = val_correct / val_total
    val_loss /= len(valid_dataloader)

    return val_loss, val_accuracy
```


## 4. 학습

이제 모델을 학습해봅시다. 
딥러닝 모델을 훈련하고 검증하는 과정을 반복하며, 검증 손실이 개선될 때마다 모델의 가중치를 저장합니다. 

각 에포크마다 훈련 손실과 정확도를 계산하고, 검증 데이터로 모델을 평가합니다. 검증 손실이 가장 낮은 경우 해당 모델의 가중치를 파일로 저장합니다.

```python
num_epochs = 5

# Training loop
best_val_loss = float('inf')

# Training loop
for epoch in range(num_epochs):
    # Training
    train_loss = 0
    train_correct = 0
    train_total = 0
    model.train()
    for batch_X, batch_y in train_dataloader:
        # Forward pass
        batch_X, batch_y = batch_X.to(device), batch_y.to(device)
        # batch_X.shape == (batch_size, max_len)
        logits = model(batch_X)

        # Compute loss
        loss = criterion(logits, batch_y)

        # Backward pass and optimization
        optimizer.zero_grad()
        loss.backward()
        optimizer.step()

        # Calculate training accuracy and loss
        train_loss += loss.item()
        train_correct += calculate_accuracy(logits, batch_y) * batch_y.size(0)
        train_total += batch_y.size(0)

    train_accuracy = train_correct / train_total
    train_loss /= len(train_dataloader)

    # Validation
    val_loss, val_accuracy = evaluate(model, valid_dataloader, criterion, device)

    print(f'Epoch {epoch+1}/{num_epochs}:')
    print(f'Train Loss: {train_loss:.4f}, Train Accuracy: {train_accuracy:.4f}')
    print(f'Validation Loss: {val_loss:.4f}, Validation Accuracy: {val_accuracy:.4f}')

    # 검증 손실이 최소일 때 체크포인트 저장
    if val_loss < best_val_loss:
        print(f'Validation loss improved from {best_val_loss:.4f} to {val_loss:.4f}. 체크포인트를 저장합니다.')
        best_val_loss = val_loss
        torch.save(model.state_dict(), 'best_model_checkpoint.pth')
```

num_epochs라는 변수를 통해 훈련을 몇 번 반복할지를 설정하며, 
여기서는 5번의 반복을 수행합니다.

best_val_loss는 모델의 검증 손실 중 가장 낮은 값을 추적하는 변수입니다. 초기값은 매우 큰 값으로 설정되며, 검증 손실이 이 값보다 작으면 모델의 상태를 저장합니다.

훈련 과정에서 train_dataloader는 데이터를 배치로 묶어 모델에 입력합니다. 
각 배치마다 배치 데이터(batch_X, batch_y)를 device(보통 GPU)에 올려서 모델에 입력하고, 
모델로부터 예측값(logits)을 계산합니다. 

이후 예측값과 실제 정답(batch_y) 사이의 손실을 계산하고, 
이를 바탕으로 역전파를 통해 모델의 가중치를 업데이트합니다.

훈련이 끝나면, 훈련 손실과 정확도를 계산하고 이를 출력합니다. 검증 과정에서는 모델을 valid_dataloader로 평가하고, 검증 손실과 검증 정확도를 계산합니다. 

검증 손실이 이전 최저 검증 손실보다 낮다면, 새로운 최저 검증 손실로 업데이트하고 해당 상태의 모델 가중치를 저장합니다.

이 과정은 전체 에포크 동안 반복되며, 최종적으로 성능이 가장 좋은 모델의 가중치가 저장됩니다.



## 5. 모델 로드 및 평가

이제 베스트 모델을 로드하여 테스트 데이터에 대한 성능을 측정해봅시다.

```python
# 모델 로드
model.load_state_dict(torch.load('best_model_checkpoint.pth'))

# 모델을 device에 올립니다.
model.to(device)
```

evaluate() 함수를 이용하여 검증 데이터에 대한 정확도와 손실을 출력해봅시다.

```python
# 검증 데이터에 대한 정확도와 손실 계산
val_loss, val_accuracy = evaluate(model, valid_dataloader, criterion, device)

print(f'Best model validation loss: {val_loss:.4f}')
print(f'Best model validation accuracy: {val_accuracy:.4f}')
```

```python
Best model validation loss: 0.3392
Best model validation accuracy: 0.8490
```

검증 데이터에 대한 정확도는 84.90%입니다. 

이제 테스트 데이터에 대한 정확도와 손실을 출력해봅시다.

```python
# 테스트 데이터에 대한 정확도와 손실 계산
test_loss, test_accuracy = evaluate(model, test_dataloader, criterion, device)

print(f'Best model test loss: {test_loss:.4f}')
print(f'Best model test accuracy: {test_accuracy:.4f}')
```

```python
Best model test loss: 0.3435
Best model test accuracy: 0.8492
```

테스트 데이터에 대한 정확도는 84.92%입니다.

## 6. 모델 테스트

이제 << 임의의 입력에 대해서 예측 >> 을 하는 predict() 함수를 만듭니다.

```python
index_to_tag = {0 : '부정', 1 : '긍정'}

def predict(text, model, word_to_index, index_to_tag):
    # Set the model to evaluation mode
    model.eval()

    # Tokenize the input text
    tokens = mecab.morphs(text) # 토큰화
    tokens = [word for word in tokens if not word in stopwords] # 불용어 제거
    token_indices = [word_to_index.get(token, 1) for token in tokens]

    # Convert tokens to tensor
    input_tensor = torch.tensor([token_indices], dtype=torch.long).to(device)  # (1, seq_length)

    # Pass the input tensor through the model
    with torch.no_grad():
        logits = model(input_tensor)  # (1, output_dim)

    # Get the predicted class index
    predicted_index = torch.argmax(logits, dim=1)

    # Convert the predicted index to its corresponding tag
    predicted_tag = index_to_tag[predicted_index.item()]

    return predicted_tag
```

먼저 model.eval()은 모델을 평가 모드로 전환합니다. 이 단계에서는 모델이 예측을 할 때 학습과정에서 사용되었던 드롭아웃 같은 기능이 비활성화됩니다. 

텍스트는 word_tokenize 함수를 사용해 단어 단위로 분리되며, 각 단어는 소문자로 변환된 후 사전에 정의된 word_to_index 사전에서 해당 단어의 인덱스를 찾아 정수로 변환됩니다. 

만약 사전에 없는 단어가 발견되면, 해당 단어는 `<UNK>`로 처리되어 인덱스 1이 할당됩니다.



변환된 정수 인덱스 리스트는 PyTorch 텐서로 변환되어 모델에 입력됩니다. 
이 텐서는 배치 크기가 1인 2차원 텐서로, 입력 텍스트의 각 단어가 인덱스로 변환된 결과를 담고 있습니다. (row=data cnt, col=feature cnt)

with torch.no_grad() 구문은 모델의 예측 과정에서 기울기 계산을 비활성화하여 메모리와 연산 속도를 최적화합니다. 이로써 입력된 텍스트에 대해 모델의 예측값을 계산합니다.

모델의 출력인 logits는 각 감정 클래스에 대한 점수로, 
이 중에서 가장 높은 점수를 가진 인덱스(predicted_index)가 예측된 감정 클래스로 선택됩니다. 

이 인덱스는 index_to_tag 사전을 통해 '긍정' 또는 '부정'이라는 문자열로 변환됩니다. 
최종적으로 예측된 감정 클래스를 반환합니다.

이제 임의의 입력에 대해서 예측을 해봅시다. 
영화에 대한 부정적인 리뷰를 predict() 함수의 입력으로 사용해봅시다.

```python
test_input = "이 영화 개꿀잼 ㅋㅋㅋ"
predict(test_input, model, word_to_index, index_to_tag)
```

```python
긍정
```

```python
test_input = "이딴게 영화냐 ㅉㅉ"
predict(test_input, model, word_to_index, index_to_tag)
```

```python
부정
```

```python
test_input = "감독 뭐하는 놈이냐?"
predict(test_input, model, word_to_index, index_to_tag)
```

```python
부정
```

```python
test_input = "와 개쩐다 정말 세계관 최강자들의 영화다"
predict(test_input, model, word_to_index, index_to_tag)
```

```python
긍정
```



# 13-03 GRU를 이용한 IMDB 리뷰 분류하기

1D CNN을 이용하여 IMDB 영화 리뷰 데이터를 분류해보겠습니다.

## 1. 데이터 로드 및 단어 토큰화

```python
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import nltk
import torch
import urllib.request
from tqdm import tqdm
from collections import Counter
from nltk.tokenize import word_tokenize
from sklearn.model_selection import train_test_split
```

```python
nltk.download('punkt')
```

```python
urllib.request.urlretrieve("https://raw.githubusercontent.com/ukairia777/pytorch-nlp-tutorial/main/10.%20RNN%20CNN%20Text%20Classification/dataset/IMDB%20Dataset.csv", filename="IMDB Dataset.csv")
```

영화 리뷰 데이터인 IMDB 리뷰 데이터를 로드합니다.

```python
df = pd.read_csv('IMDB Dataset.csv')
df
```

![578](https://static.wikidocs.net/images/page/217064/imdb.PNG)

데이터의 개수는 총 5만개입니다. 데이터에 결측값이 있는지 확인할 수 있는 방법 중 하나는 데이터프레임의 정보를 확인할 수 있는 `info()`를 사용하는 것입니다.

```python
df.info()
```

```python
<class 'pandas.core.frame.DataFrame'>
RangeIndex: 50000 entries, 0 to 49999
Data columns (total 2 columns):
 #   Column     Non-Null Count  Dtype 
---  ------     --------------  ----- 
 0   review     50000 non-null  object
 1   sentiment  50000 non-null  object
dtypes: object(2)
memory usage: 781.4+ KB
```

review 열과 sentiment 열 모두 non-null(결측값이 아닌) 데이터가 5만개로 확인되므로 결측값은 없습니다. 결측값을 확인할 수 있는 또 다른 방법인 `.isnull().values.any()`를 사용하여 결측값 여부를 확인합니다.

```python
print('결측값 여부 :',df.isnull().values.any())
```

```python
결측값 여부 : False
```

False가 출력된다면 결측값은 없다는 의미입니다. 
레이블이 균등한지 Bar Chart를 통해 확인합니다.

```python
df['sentiment'].value_counts().plot(kind='bar')
```

![](https://static.wikidocs.net/images/page/217064/labels.PNG)

레이블의 실제 개수를 확인해봅시다.

```python
print('레이블 개수')
print(df.groupby('sentiment').size().reset_index(name='count'))
```

```python
레이블 개수
  sentiment  count
0  negative  25000
1  positive  25000
```

두 레이블의 개수는 동일합니다. 레이블이 현재 'positive'와 'negative'로 구성되어져 있으므로 각각 1, 0으로 변환하고 정상 변환되었는지 확인하기 위해서 상위 5개의 행을 출력합니다.

```python
df['sentiment'] = df['sentiment'].replace(['positive','negative'],[1, 0])
df.head()
```

![](https://static.wikidocs.net/images/page/217064/imdb_data.PNG)

긍정 레이블은 1, 부정 레이블은 0으로 변환된 것을 확인하였습니다. 

'review' 열은 X_data, 레이블에 해당하는 'sentiment' 열은 y_data에 저장 후 정상 변환 되었는지 확인하기 위해서 다시 한 번 개수를 출력합니다.

```python
X_data = df['review']
y_data = df['sentiment']
print('영화 리뷰의 개수: {}'.format(len(X_data)))
print('레이블의 개수: {}'.format(len(y_data)))
```

```python
영화 리뷰의 개수: 50000
레이블의 개수: 50000
```

훈련 데이터와 검증 데이터, 테스트 데이터로 데이터를 나눕니다. 

우선 훈련 데이터와 테스트 데이터를 5:5 비율로 나누고, 
훈련 데이터를 다시 8:2 비율로 훈련 데이터와 검증 데이터로 나눕니다. 

sklearn의 train_test_split은 데이터를 나눌 때 굉장히 많이 사용하는 도구이므로 꼭 기억해둡시다. 데이터를 나눌 때 레이블의 비율을 유지하고 싶다면 레이블 데이터를 stratify에 명시해줄 수 있습니다.

```python
X_train, X_test, y_train, y_test = train_test_split(X_data, y_data, test_size=0.5, random_state=0, stratify=y_data)
X_train, X_valid, y_train, y_valid = train_test_split(X_train, y_train, test_size=.2, random_state=0, stratify=y_train)

print('--------훈련 데이터의 비율-----------')
print(f'부정 리뷰 = {round(y_train.value_counts()[0]/len(y_train) * 100,3)}%')
print(f'긍정 리뷰 = {round(y_train.value_counts()[1]/len(y_train) * 100,3)}%')
print('--------검증 데이터의 비율-----------')
print(f'부정 리뷰 = {round(y_valid.value_counts()[0]/len(y_valid) * 100,3)}%')
print(f'긍정 리뷰 = {round(y_valid.value_counts()[1]/len(y_valid) * 100,3)}%')
print('--------테스트 데이터의 비율-----------')
print(f'부정 리뷰 = {round(y_test.value_counts()[0]/len(y_test) * 100,3)}%')
print(f'긍정 리뷰 = {round(y_test.value_counts()[1]/len(y_test) * 100,3)}%')
```

```python
--------훈련 데이터의 비율-----------
부정 리뷰 = 50.0%
긍정 리뷰 = 50.0%
--------검증 데이터의 비율-----------
부정 리뷰 = 50.0%
긍정 리뷰 = 50.0%
--------테스트 데이터의 비율-----------
부정 리뷰 = 50.0%
긍정 리뷰 = 50.0%
```

훈련 데이터, 검증 데이터, 테스트 데이터 세 개의 데이터 모두 긍정 레이블과 부정 레이블 모두 50:50으로 레이블이 균등하게 유지된 채 분할된 것을 확인할 수 있습니다. 

훈련 데이터, 검증 데이터, 테스트 데이터에 대해서 토큰화를 진행해봅시다. 토큰화를 위해 토큰화 함수 `tokenize()`를 구현하였습니다. 

토큰화 진행 시에 선택적으로 소문자화를 진행하고 싶다면 소문자화도 진행할 수 있습니다. 
파이썬 문자열에 .lower()를 사용하면 해당 문자열을 소문자로 바꿔줍니다. 

해당 함수로 훈련 데이터, 검증 데이터, 테스트 데이터에 대해서 모두 토큰화를 진행합니다.

```python
def tokenize(sentences):
  tokenized_sentences = []
  for sent in tqdm(sentences):
    tokenized_sent = word_tokenize(sent)
    tokenized_sent = [word.lower() for word in tokenized_sent]
    tokenized_sentences.append(tokenized_sent)
  return tokenized_sentences

tokenized_X_train = tokenize(X_train)
tokenized_X_valid = tokenize(X_valid)
tokenized_X_test = tokenize(X_test)
```

토큰화가 진행된 후의 훈련 데이터의 상위 2개 샘플을 출력해봅시다.

```python
# 상위 샘플 2개 출력
for sent in tokenized_X_train[:2]:
  print(sent)
```

```python
['have', 'you', 'ever', ',', 'or', 'do', 'you', 'have', ',', 'a', 'pet', 'who', "'s", 'been', 'with', 'you', 'through', 'thick', 'and', 'thin', ',', 'who', 'you', "'d", 'be', 'lost', 'without', ',', 'and', 'who', 'you', 'love', 'no', 'matter', 'what', '?', 'betcha', 'never', 'thought', 'they', 'feel', 'the', 'same', 'way', 'about', 'you', '!', '<', 'br', '/', '>', '<', 'br', '/', '>', 'wonderful', ... 중략 ...]
['i', 'hate', 'football', '!', '!', 'i', 'hate', 'football', 'fans', '!', 'i', 'hate', 'cars', '!', 'but', 'this', 'film', 'was', 'the', 'funniest', 'thing', 'i', 'have', 'seen', 'in', 'quite', 'some', 'time', '.', '<', 'br', '/', '>', '<', 'br', '/', '>', 'i', 'was', 'given', 'the', 'great', 'opportunity', 'to', 'see', 'this', 'film', 'at', 'the', 'weekend', ',', 'and', 'all', 'i', 'have', 'to', 'say', 'is', 'i', ... 중략 ...]
```

정상적으로 토큰화 된 것을 확인하였습니다. 결과가 너무 길어 책의 지면에서는 중략했습니다.


## 2. Vocab 만들기

이제 토큰화 된 훈련 데이터로부터 정수 인코딩을 진행하기 위한 단어 집합(Vocabulary)을 만들어봅시다. Counter 모듈을 사용하면 현재 갖고 있는 데이터에 존재하는 단어 종류의 총 개수와 각 단어에 대해서 등장 빈도를 카운트 할 수 있습니다.

```python
word_list = []
for sent in tokenized_X_train:
    for word in sent:
      word_list.append(word)

word_counts = Counter(word_list)
print('총 단어수 :', len(word_counts))
```

```python
총 단어수 : 100586
```

Counter 모듈을 통해 확인한 훈련 데이터에 존재하는 총 단어수는 100,586개입니다. 
이 100,586는 단어들의 집합(set)에서의 단어의 개수를 의미하므로 훈련 데이터에 존재하는 총 단어의 종류의 개수입니다. 또한, 현재 word_counts에는 각 단어의 등장 빈도수가 기록되어져 있습니다. 영단어 'the'와 'love'의 등장 빈도수를 확인해봅시다.

```python
print('훈련 데이터에서의 단어 the의 등장 횟수 :', word_counts['the'])
print('훈련 데이터에서의 단어 love의 등장 횟수 :', word_counts['love'])
```

```python
훈련 데이터에서의 단어 the의 등장 횟수 : 265697
훈련 데이터에서의 단어 love의 등장 횟수 : 4984
```

word_counts에는 단어와 각 단어의 등장 빈도수가 기록되어져 있습니다. 그리고 이 정보가 총 100,586개 존재합니다. 단어의 등장 빈도수가 높은 순서대로 정렬하여 vocab이라는 변수에 저장한 후 빈도수가 가장 높은 상위 10개의 단어를 출력합니다.

```python
vocab = sorted(word_counts, key=word_counts.get, reverse=True)
print('등장 빈도수 상위 10개 단어')
print(vocab[:10])
```

```python
등장 빈도수 상위 10개 단어
['the', ',', '.', 'a', 'and', 'of', 'to', 'is', '/', '>']
```

여기서는 빈도수가 낮은 단어들은 자연어 처리에서 배제하고자 합니다. 등장 빈도수가 3회 미만인 단어들이 이 데이터에서 얼만큼의 비중을 차지하는지 확인해봅시다.

```python
threshold = 3
total_cnt = len(word_counts) # 단어의 수
rare_cnt = 0 # 등장 빈도수가 threshold보다 작은 단어의 개수를 카운트
total_freq = 0 # 훈련 데이터의 전체 단어 빈도수 총 합
rare_freq = 0 # 등장 빈도수가 threshold보다 작은 단어의 등장 빈도수의 총 합

# 단어와 빈도수의 쌍(pair)을 key와 value로 받는다.
for key, value in word_counts.items():
    total_freq = total_freq + value

    # 단어의 등장 빈도수가 threshold보다 작으면
    if(value < threshold):
        rare_cnt = rare_cnt + 1
        rare_freq = rare_freq + value

print('단어 집합(vocabulary)의 크기 :',total_cnt)
print('등장 빈도가 %s번 이하인 희귀 단어의 수: %s'%(threshold - 1, rare_cnt))
print("단어 집합에서 희귀 단어의 비율:", (rare_cnt / total_cnt)*100)
print("전체 등장 빈도에서 희귀 단어 등장 빈도 비율:", (rare_freq / total_freq)*100)
```

```python
단어 집합(vocabulary)의 크기 : 100586
등장 빈도가 2번 이하인 희귀 단어의 수: 61877
단어 집합에서 희귀 단어의 비율: 61.51651323245779
전체 등장 빈도에서 희귀 단어 등장 빈도 비율: 1.3294254426463437
```

등장 빈도가 threshold 값인 3회 미만. 즉, 2회 이하인 단어들은 단어 집합에서 무려 절반 이상을 차지합니다. 하지만, 실제로 훈련 데이터에서 등장 빈도로 차지하는 비중은 상대적으로 매우 적은 수치인 1.32%밖에 되지 않습니다. 아무래도 등장 빈도가 2회 이하인 단어들은 자연어 처리에서 별로 중요하지 않을 듯 합니다. 그래서 이 단어들은 정수 인코딩 과정에서 배제시키겠습니다.

등장 빈도수가 2이하인 단어들의 수를 제외한 단어의 개수를 단어 집합의 최대 크기로 제한하겠습니다.

```python
# 전체 단어 개수 중 빈도수 2이하인 단어는 제거.
vocab_size = total_cnt - rare_cnt
vocab = vocab[:vocab_size]
print('단어 집합의 크기 :', len(vocab))
```

```python
단어 집합의 크기 : 38709
```

등장 빈도수가 2번 이하인 단어를 제거하자 단어 집합의 크기가 100,586개에서 38,709개로 줄었습니다. 아직 각 단어에 고유한 정수를 부여하는 작업을 진행하지는 않았습니다. 해당 작업을 진행하기에 앞서 정수 0과 정수 1에는 특별한 용도의 단어를 부여하고자 합니다. 

정수 0은 패딩을 위해서 사용하는 패딩 토큰인 `<PAD>`를 할당하고, 정수 1은 OOV(Out-Of-Vocabulary) 문제 발생 시에 모르는 단어에 정수 1을 할당하는 용도인 `<UNK>`를 할당합니다.

```python
word_to_index = {}
word_to_index['<PAD>'] = 0
word_to_index['<UNK>'] = 1

for index, word in enumerate(vocab) :
  word_to_index[word] = index + 2

vocab_size = len(word_to_index)
print('패딩 토큰과 UNK 토큰을 고려한 단어 집합의 크기 :', vocab_size)
```

```python
패딩 토큰과 UNK 토큰을 고려한 단어 집합의 크기 : 38711
```



## 3. 정수 인코딩

최종 단어 집합(Vocabulary)인 word_to_index를 이용하여 정수 인코딩을 진행해봅시다. 

이를 위한 함수인 `texts_to_sequences()`를 구현합니다. 해당 함수는 주어진 데이터에서 각 단어를 word_to_index에 맵핑된 정수로 변환합니다. 이때 word_to_index에 존재하지 않는 단어가 등장한 경우에는 정수 1을 부여합니다.

```python
def texts_to_sequences(tokenized_X_data, word_to_index):
  encoded_X_data = []
  for sent in tokenized_X_data:
    index_sequences = []
    for word in sent:
      try:
          index_sequences.append(word_to_index[word])
      except KeyError:
          index_sequences.append(word_to_index['<UNK>'])
    encoded_X_data.append(index_sequences)
  return encoded_X_data

encoded_X_train = texts_to_sequences(tokenized_X_train, word_to_index)
encoded_X_valid = texts_to_sequences(tokenized_X_valid, word_to_index)
encoded_X_test = texts_to_sequences(tokenized_X_test, word_to_index)
```

정수 인코딩이 진행된 학습 데이터의 상위 2개 샘플을 출력해봅시다.

```python
# 상위 샘플 2개 출력
for sent in encoded_X_train[:2]:
  print(sent)
```

```python
[38, 29, 140, 3, 52, 54, 29, 38, 3, 5, 3406, 47, 19, 95, 22, 29, 161, 4059, 6, 1741, 3, 47, 29, 293, 39, 469, 218, 3, 6, 47, 29, 134, 71, 532, 61, 59, 25184, 130, 214, 44, 249, 2, 189, 114, 58, 29, 41, 12, 13, 10, 11, 12, 13, 10, 11, 384, 3, 384, 253, 26, 4, 57, 29, 38, 5, 2280, 1587, 23, 1477, 3, 17, 9, 5775, 8, 111, 29, 1440, 71, 532, 141, 677, 4, 16, 343, 8, 126, 17, 24, 43, 2, 75, 63, 16, 20 ... 중략 ...]
[16, 735, 2344, 41, 41, 16, 735, 2344, 467, 41, 16, 735, 1903, 41, 25, 17, 26, 20, 2, 1588, 165, 16, 38, 128, 15, 198, 62, 75, 4, 12, 13, 10, 11, 12, 13, 10, 11, 16, 20, 360, 2, 100, 1359, 8, 77, 17, 26, 42, 2, 2394, 3, 6, 43, 16, 38, 8, 147, 9, 16, 1445, 2395, 16, 3268, 3, 6, 63, 9, 14, 184, 8, 39, 1320, 15, 2, 2382, 6, 9728, 4, 520, 3, 17, 9, 40, 2344, 26, 29, 97, 354, 8, 77, 3, 109, 604, 41, 12, 13, 10, 11 ... 중략 ...]
```

정수 인코딩 된 결과를 역으로 복원해보기 위해서 
각 단어에 정수가 맵핑된 word_to_index를 반대로 만든 index_to_word를 구현해보고 첫번째 샘플에 대해서 복원해봅시다.

```python
index_to_word = {}
for key, value in word_to_index.items():
    index_to_word[value] = key

decoded_sample = [index_to_word[word] for word in encoded_X_train[0]]
print('기존의 첫번째 샘플 :', tokenized_X_train[0])
print('복원된 첫번째 샘플 :', decoded_sample)
```

```python
기존의 첫번째 샘플 : ['have', 'you', 'ever', ',', 'or', 'do', ... 중략 ... 'heart-swelling', 'feeling', '.', 'i', 'give', 'this', '9/10', '.', 'to', 'be', 'compared', 'to', '(', 'and', 'even', 'rated', 'better', 'than', ')', 'cats', 'and', 'dogs', 'and', 'babe', '.']
복원된 첫번째 샘플 : ['have', 'you', 'ever', ',', 'or', 'do', ... 중략 ... '<UNK>', 'feeling', '.', 'i', 'give', 'this', '9/10', '.', 'to', 'be', 'compared', 'to', '(', 'and', 'even', 'rated', 'better', 'than', ')', 'cats', 'and', 'dogs', 'and', 'babe', '.']
```

내용이 너무 길어서 중략했습니다. 기존의 첫번째 샘플과는 달리 정수 인코딩 후 다시 역으로 복원한 첫번째 샘플은 중간에 `<UNK>`이 있는 것을 확인할 수 있습니다.

## 4. 패딩

서로 다른 길이의 데이터들을 동일한 길이로 일치시켜주는 패딩 작업을 진행해봅시다. 
이를 위해서 훈련 데이터의 최대 길이, 평균 길이, 그리고 데이터의 길이 분포를 확인합니다.

```python
print('리뷰의 최대 길이 :',max(len(review) for review in encoded_X_train))
print('리뷰의 평균 길이 :',sum(map(len, encoded_X_train))/len(encoded_X_train))
plt.hist([len(review) for review in encoded_X_train], bins=50)
plt.xlabel('length of samples')
plt.ylabel('number of samples')
plt.show()
```

```python
리뷰의 최대 길이 : 2818
리뷰의 평균 길이 : 279.1958
```

![](https://static.wikidocs.net/images/page/217064/lengthofsamples.PNG)

가장 긴 샘플의 길이는 2,818이며, 그래프를 봤을 때 전체 데이터의 길이 분포는 대체적으로 약 1,000내외의 길이를 가지는 것을 볼 수 있습니다. 모델이 처리할 수 있도록 encoded_X_train과 encoded_X_test의 모든 샘플의 길이를 특정 길이로 동일하게 맞춰줄 필요가 있습니다. 

특정 길이 변수를 max_len으로 정합니다. 대부분의 리뷰가 내용이 잘리지 않도록 할 수 있는 최적의 max_len의 값은 몇일까요? 전체 샘플 중 길이가 max_len 이하인 샘플의 비율이 몇 %인지 확인하는 함수를 만듭니다.

```python
def below_threshold_len(max_len, nested_list):
  count = 0
  for sentence in nested_list:
    if(len(sentence) <= max_len):
        count = count + 1
  print('전체 샘플 중 길이가 %s 이하인 샘플의 비율: %s'%(max_len, (count / len(nested_list))*100))
```

최대 길이 2,818로 모든 샘플을 패딩하는 것은 조금 과한 처사일 것입니다. 
500으로 할 경우 몇 개의 샘플을 손상시키지 않는지 확인해봅시다.

```python
max_len = 500
below_threshold_len(max_len, encoded_X_train)
```

```python
전체 샘플 중 길이가 500 이하인 샘플의 비율: 87.795
```

500으로 패딩할 경우 약 88%의 샘플은 그대로 보존됩니다. 더 많은 샘플을 보존하기 위해서는 500보다 더 큰 길이로 패딩할 수도 있겠지만, 여기서는 500으로 진행해보겠습니다. 

이를 위해 패딩을 해주는 함수 `pad_sequences()`를 구현합니다. 해당 함수는 최대 길이를 정하면 해당 길이보다 긴 데이터는 뒷 부분을 잘라서 해당 길이로 맞추고, 해당 길이보다 짧은 데이터는 뒤에 0을 채워서 해당 길이의 데이터로 변환합니다. 결과적으로 길이 500으로 패딩을 하면 모든 데이터의 길이는 500이 됩니다.

```python
def pad_sequences(sentences, max_len):
  features = np.zeros((len(sentences), max_len), dtype=int)
  for index, sentence in enumerate(sentences):
    if len(sentence) != 0:
      features[index, :len(sentence)] = np.array(sentence)[:max_len]
  return features

padded_X_train = pad_sequences(encoded_X_train, max_len=max_len)
padded_X_valid = pad_sequences(encoded_X_valid, max_len=max_len)
padded_X_test = pad_sequences(encoded_X_test, max_len=max_len)

print('훈련 데이터의 크기 :', padded_X_train.shape)
print('검증 데이터의 크기 :', padded_X_valid.shape)
print('테스트 데이터의 크기 :', padded_X_test.shape)
```

```python
훈련 데이터의 크기 : (20000, 500)
검증 데이터의 크기 : (5000, 500)
테스트 데이터의 크기 : (25000, 500)
```


==~~ 여기까지는 LSTM 이랑 별다를거 없음. 데이터만 다를 뿐이고 전처리 과정은 전부 동일함.


## 5. 모델링 ~> 여기부터가 중요

이제 딥 러닝 프레임워크 PyTorch를 이용하여 GRU 모델을 구현해봅시다.

```python
import torch
import torch.nn as nn
import torch.nn.functional as F
```

현재 실습 환경에서 GPU를 사용 가능한지 확인합니다.

```python
USE_CUDA = torch.cuda.is_available()
device = torch.device("cuda" if USE_CUDA else "cpu")
print("cpu와 cuda 중 다음 기기로 학습함:", device)
```

```python
cpu와 cuda 중 다음 기기로 학습함: cuda
```

저자의 경우 Colab에서 GPU를 선택하여 실습을 진행하여 cuda라는 출력 결과를 확인했습니다. 레이블 데이터를 파이토치의 텐서 타입으로 변환합니다. 이후 훈련 데이터의 상위 5개의 레이블을 출력해보았습니다.

```python
train_label_tensor = torch.tensor(np.array(y_train))
valid_label_tensor = torch.tensor(np.array(y_valid))
test_label_tensor = torch.tensor(np.array(y_test))
print(train_label_tensor[:5])
```

```python
tensor([1, 1, 0, 0, 0])
```

GRU 모델을 클래스로 구현해봅시다. 

각 층을 지날 때마다 각 층의 출력의 크기를 이해하는 것이 중요합니다. 
예를 들어 입력은 (배치 크기, 문장 길이)의 크기를 가지는 텐서입니다. 

임베딩 층을 지나고 나면 각 단어가 임베딩 벡터로 변환되면서 
(배치 크기, 문장 길이, 임베딩 벡터의 차원)으로 텐서의 크기가 변환됩니다.

이 후 GRU의 마지막 시점의 은닉 상태(hidden state) 값을 출력층과 연결시키는 작업을 해주어야 합니다. 이때 GRU가 출력층으로 보는 결과값의 차원은 (배치 크기, 은닉 상태의 차원)을 가져야 합니다. 

마지막 시점의 은닉 상태의 값만 전달하므로 은닉 상태는 모든 시점(문장 길이)만큼 존재하는 것이 아니라 단 하나만 있습니다. 출력층은 지난 결과는 소프트맥스 회귀를 수행하므로 (배치 크기, 분류하고자하는 카테고리의 수)의 차원을 가지게 됩니다.

그 후 각 데이터를 배치 단위로 데이터 묶음을 꺼낼 수 있는 데이터로더로 전달합니다. 정리하면 다음과 같습니다. 아직 모델을 만들지는 않았지만, 단어 벡터의 차원을 100, 배치 크기를 32, 문장 길이를 500(패딩 후), GRU의 은닉 상태의 차원을 128로 한다고 가정해보겠습니다.

```diff
- 단어 벡터의 차원 = 100
- 문장 길이 = 500
- 배치 크기 = 32
- 데이터 개수 = 2만
- GRU의 은닉층의 크기 = 128
- 분류하고자 하는 카테고리 개수 = 2개
```

위의 정보들을 고려하였을 때 모델 내부에서 데이터의 변화는 다음과 같습니다.

- (32, 500) => 입력 데이터의 형태 => 임베딩 층 통과 후 => (32, 500, 100) => GRU 통과 후 => (32, 128) => Softmax 출력층 통과 후 => (32, 2)

```python
class TextClassifier(nn.Module):
    def __init__(self, vocab_size, embedding_dim, hidden_dim, output_dim):
        super(TextClassifier, self).__init__()
        self.embedding = nn.Embedding(vocab_size, embedding_dim)
        self.gru = nn.GRU(embedding_dim, hidden_dim, batch_first=True)
        self.fc = nn.Linear(hidden_dim, output_dim) # output_dim = 분류하고자하는 카테고리의 개수

    def forward(self, x):
        # x: (batch_size, seq_length) == (32, 500)
        embedded = self.embedding(x)  # (batch_size, seq_length, embedding_dim) == (32, 500, 100) == (데이터의 개수, 문장길이, 단어 벡터의 차원)
        gru_out, hidden = self.gru(embedded)  # gru_out: (batch_size, seq_length, hidden_dim), hidden: (1, batch_size, hidden_dim)
        last_hidden = hidden.squeeze(0)  # (batch_size, hidden_dim)
        logits = self.fc(last_hidden)  # (batch_size, output_dim)
        return logits
```

#### LSTM 과의 차이점?

- **업데이트 게이트 ($z_t$):** 이전의 은닉 상태 $h_{t-1}$를 얼마나 유지하고, 새로운 정보 $\tilde{h}_t$를 얼마나 반영할지 한 번에 결정합니다.
    
- **리셋 게이트 ($r_t$):** 새로운 정보를 만들 때 이전 은닉 상태 $h_{t-1}$를 얼마나 지울지 결정합니다.

출력 입력 삭제 => 업데이트 리셋  2개 게이트로 줄임.


첨부해 주신 이미지의 코드를 보면 `gru_out, hidden = self.gru(embedded)` 형태로 받아오고 있습니다.

- **LSTM:** `out, (hidden, cell) = self.lstm(...)` $\rightarrow$ `cell` 상태까지 튜플 형태로 감싸서 반환
    
- **GRU:** `out, hidden = self.gru(...)` $\rightarrow$ 별도의 `cell`이 없으므로 **`hidden` 하나만 반환**
####

훈련 데이터, 검증 데이터, 테스트 데이터에 대해서 파이토치 텐서로 변환하고 
배치 단위 연산을 위해 데이터로더로 변환합니다.

```python
encoded_train = torch.tensor(padded_X_train).to(torch.int64)
train_dataset = torch.utils.data.TensorDataset(encoded_train, train_label_tensor)
train_dataloader = torch.utils.data.DataLoader(train_dataset, shuffle=True, batch_size=32)

encoded_test = torch.tensor(padded_X_test).to(torch.int64)
test_dataset = torch.utils.data.TensorDataset(encoded_test, test_label_tensor)
test_dataloader = torch.utils.data.DataLoader(test_dataset, shuffle=True, batch_size=1)

encoded_valid = torch.tensor(padded_X_valid).to(torch.int64)
valid_dataset = torch.utils.data.TensorDataset(encoded_valid, valid_label_tensor)
valid_dataloader = torch.utils.data.DataLoader(valid_dataset, shuffle=True, batch_size=1)
```

훈련 데이터의 샘플 개수가 20,000개 였으므로 배치 크기를 32로 할 경우에는 20000/32=625 다시 말해 32개씩 묶인 데이터 묶음이 625개가 생깁니다. 

그리고 학습 시에는 32개씩 데이터가 들어가게 될 것입니다.

```python
total_batch = len(train_dataloader)
print('총 배치의 수 : {}'.format(total_batch))
```

```python
총 배치의 수 : 625
```

모델 객체를 선언합니다.

```python
embedding_dim = 100
hidden_dim = 128
output_dim = 2
learning_rate = 0.01
num_epochs = 10

model = TextClassifier(vocab_size, embedding_dim, hidden_dim, output_dim)
model.to(device)
```

임베딩 벡터의 차원은 128, 출력층의 크기(분류해야 할 카테고리의 개수)는 2로 정했습니다. 이렇게 사용자가 정해주는 값이면서 모델의 결과에 영향을 미치는 값들을 하이퍼파라미터라고 합니다. 

소프트맥스 회귀를 통해 분류 문제를 진행하므로 손실 함수는 nn.CrossEntropyLoss()를 사용합니다. 파이토치로 자연어 처리를 하게 되면 가장 많이 사용하게 되는 손실 함수입니다. 하이퍼파라미터 중 하나인 학습률(learning rate)는 0.001로 정했습니다.


```python
criterion = nn.CrossEntropyLoss()
optimizer = torch.optim.Adam(model.parameters(), lr=0.001)
```



## 6. 평가 코드 작성

이후 평가를 진행하기 위해서 모델의 정확도를 측정하는 함수 `calculate_accuracy()`를 작성합니다.

```python
def calculate_accuracy(logits, labels):
    # _, predicted = torch.max(logits, 1)
    predicted = torch.argmax(logits, dim=1)
    correct = (predicted == labels).sum().item()
    total = labels.size(0)
    accuracy = correct / total
    return accuracy
```

검증 데이터와 테스트 데이터에 대한 성능을 측정하기 위한 함수 `evaluate()`를 작성합니다. 
아래의 함수에서 `model.eval()`과 `with torch.no_grad()`를 짚어봅시다. 이 두 개는 모델 평가를 수행할 때 중요한 역할을 합니다. 각각의 의미는 다음과 같습니다.

- model.eval(): 모델을 평가 모드로 설정합니다. 이렇게 하면 모델 내부의 모든 레이어에 대해 평가 모드가 활성화됩니다. 일부 레이어, 예를 들어 드롭아웃이나 배치 정규화는 학습과 평가 시 다르게 동작하기 때문에 이 설정이 중요합니다. 평가 모드가 설정되지 않으면, 이러한 레이어의 동작이 올바르지 않을 수 있으며, 이로 인해 평가 결과가 제대로 나오지 않을 수 있습니다.
    
- with torch.no_grad(): 이 문장은 자동 미분 엔진에서 기울기(gradient) 계산을 비활성화합니다. 평가 중에는 기울기를 계산할 필요가 없으므로, 이렇게 설정하면 메모리를 절약하고 속도를 높일 수 있습니다. 만약 이 설정이 적용되지 않으면, 평가 과정에서 기울기(gradient)가 계산되고 메모리를 차지하게 됩니다. 그러나 평가 결과 자체에는 직접적인 영향을 주지 않습니다.
    

따라서 model.eval()은 평가 시 반드시 사용해야 하며, 그렇지 않으면 평가 결과가 올바르게 나오지 않을 수 있습니다. with torch.no_grad():는 필수는 아니지만, 메모리와 속도 측면에서 권장됩니다.

```python
def evaluate(model, valid_dataloader, criterion, device):
    val_loss = 0
    val_correct = 0
    val_total = 0

    model.eval()
    with torch.no_grad():
        # 데이터로더로부터 배치 크기만큼의 데이터를 연속으로 로드
        for batch_X, batch_y in valid_dataloader:
            batch_X, batch_y = batch_X.to(device), batch_y.to(device)

            # 모델의 예측값
            logits = model(batch_X)

            # 손실을 계산
            loss = criterion(logits, batch_y)

            # 정확도와 손실을 계산함
            val_loss += loss.item()
            val_correct += calculate_accuracy(logits, batch_y) * batch_y.size(0)
            val_total += batch_y.size(0)

    val_accuracy = val_correct / val_total
    val_loss /= len(valid_dataloader)

    return val_loss, val_accuracy
```


## 7. 학습

이제 모델을 학습해봅시다. 딥러닝 모델을 훈련하고 검증하는 과정을 반복하며, 검증 손실이 개선될 때마다 모델의 가중치를 저장합니다. 각 에포크마다 훈련 손실과 정확도를 계산하고, 검증 데이터로 모델을 평가합니다. 검증 손실이 가장 낮은 경우 해당 모델의 가중치를 파일로 저장합니다.

```python
num_epochs = 5  # 총 학습을 몇 번 반복할 것인지 설정하는 변수, 여기서는 5번 반복합니다.

# Training loop
best_val_loss = float('inf')  # 검증 손실의 최저 값을 추적하기 위한 변수로, 초기값은 매우 큰 값으로 설정합니다.

# Training loop
for epoch in range(num_epochs):  # 설정된 에포크 수만큼 반복합니다.
    # Training
    train_loss = 0  # 에포크 동안의 전체 훈련 손실을 저장할 변수입니다.
    train_correct = 0  # 에포크 동안 올바르게 예측된 샘플의 수를 저장할 변수입니다.
    train_total = 0  # 에포크 동안 처리된 총 샘플 수를 저장할 변수입니다.
    model.train()  # 모델을 훈련 모드로 설정합니다.

    for batch_X, batch_y in train_dataloader:  # 훈련 데이터셋을 배치 단위로 반복합니다.
        # Forward pass
        batch_X, batch_y = batch_X.to(device), batch_y.to(device)  # 배치 데이터를 GPU와 같은 장치에 올립니다.
        # batch_X.shape == (batch_size, max_len)
        logits = model(batch_X)  # 모델에 입력 데이터를 넣어 예측값(logits)을 계산합니다.

        # Compute loss
        loss = criterion(logits, batch_y)  # 예측값과 실제 값 간의 손실(loss)을 계산합니다.

        # Backward pass and optimization
        optimizer.zero_grad()  # 이전 배치에서 계산된 기울기(gradient)를 초기화합니다.
        loss.backward()  # 역전파를 통해 기울기를 계산합니다.
        optimizer.step()  # 계산된 기울기를 사용하여 모델의 파라미터를 업데이트합니다.

        # Calculate training accuracy and loss
        train_loss += loss.item()  # 현재 배치의 손실을 전체 훈련 손실에 추가합니다.
        train_correct += calculate_accuracy(logits, batch_y) * batch_y.size(0)  # 정확도를 계산하여 올바르게 예측된 샘플 수를 추가합니다.
        train_total += batch_y.size(0)  # 현재 배치의 샘플 수를 전체 샘플 수에 추가합니다.

    train_accuracy = train_correct / train_total  # 전체 훈련 데이터에 대한 정확도를 계산합니다.
    train_loss /= len(train_dataloader)  # 배치 수로 나누어 평균 훈련 손실을 계산합니다.

    # Validation
    val_loss, val_accuracy = evaluate(model, valid_dataloader, criterion, device)  # 검증 데이터로 모델을 평가하여 손실과 정확도를 계산합니다.

    print(f'Epoch {epoch+1}/{num_epochs}:')  # 현재 에포크 번호와 총 에포크 수를 출력합니다.
    print(f'Train Loss: {train_loss:.4f}, Train Accuracy: {train_accuracy:.4f}')  # 훈련 손실과 정확도를 출력합니다.
    print(f'Validation Loss: {val_loss:.4f}, Validation Accuracy: {val_accuracy:.4f}')  # 검증 손실과 정확도를 출력합니다.

    # 검증 손실이 최소일 때 체크포인트 저장
    if val_loss < best_val_loss:  # 현재 검증 손실이 이전의 최저 손실보다 낮으면
        print(f'Validation loss improved from {best_val_loss:.4f} to {val_loss:.4f}. 체크포인트를 저장합니다.')  # 손실이 개선되었음을 출력합니다.
        best_val_loss = val_loss  # 최저 검증 손실을 현재 손실로 업데이트합니다.
        torch.save(model.state_dict(), 'best_model_checkpoint.pth')  # 현재 모델의 가중치를 파일로 저장합니다.
```

num_epochs라는 변수를 통해 훈련을 몇 번 반복할지를 설정하며, 여기서는 5번의 반복을 수행합니다.

best_val_loss는 모델의 검증 손실 중 가장 낮은 값을 추적하는 변수입니다. 초기값은 매우 큰 값으로 설정되며, 검증 손실이 이 값보다 작으면 모델의 상태를 저장합니다.

훈련 과정에서 train_dataloader는 데이터를 배치로 묶어 모델에 입력합니다. 각 배치마다 배치 데이터(batch_X, batch_y)를 device(보통 GPU)에 올려서 모델에 입력하고, 모델로부터 예측값(logits)을 계산합니다. 이후 예측값과 실제 정답(batch_y) 사이의 손실을 계산하고, 이를 바탕으로 역전파를 통해 모델의 가중치를 업데이트합니다.

훈련이 끝나면, 훈련 손실과 정확도를 계산하고 이를 출력합니다. 검증 과정에서는 모델을 valid_dataloader로 평가하고, 검증 손실과 검증 정확도를 계산합니다. 검증 손실이 이전 최저 검증 손실보다 낮다면, 새로운 최저 검증 손실로 업데이트하고 해당 상태의 모델 가중치를 저장합니다.

이 과정은 전체 에포크 동안 반복되며, 최종적으로 성능이 가장 좋은 모델의 가중치가 저장됩니다.
(LSTM 코드랑 동일한 로직)

## 8. 모델 로드 및 평가

이제 베스트 모델을 로드하여 테스트 데이터에 대한 성능을 측정해봅시다.

```python
# 모델 로드
model.load_state_dict(torch.load('best_model_checkpoint.pth'))

# 모델을 device에 올립니다.
model.to(device)
```

evaluate() 함수를 이용하여 검증 데이터에 대한 정확도와 손실을 출력해봅시다.

```python
# 검증 데이터에 대한 정확도와 손실 계산
val_loss, val_accuracy = evaluate(model, valid_dataloader, criterion, device)

print(f'Best model validation loss: {val_loss:.4f}')
print(f'Best model validation accuracy: {val_accuracy:.4f}')
```

```python
Best model validation loss: 0.3153
Best model validation accuracy: 0.8678
```

검증 데이터에 대한 정확도는 86.78%입니다. 
이제 테스트 데이터에 대한 정확도와 손실을 출력해봅시다.

```python
# 테스트 데이터에 대한 정확도와 손실 계산
test_loss, test_accuracy = evaluate(model, test_dataloader, criterion, device)

print(f'Best model test loss: {test_loss:.4f}')
print(f'Best model test accuracy: {test_accuracy:.4f}')
```

```python
Best model test loss: 0.3245
Best model test accuracy: 0.8641
```

테스트 데이터에 대한 정확도는 86.41%입니다.


## 9. 모델 테스트

이제 임의의 입력에 대해서 예측을 하는 predict() 함수를 만듭니다.

```python
index_to_tag = {0 : '부정', 1 : '긍정'}

def predict(text, model, word_to_index, index_to_tag):
    # 모델 평가 모드
    model.eval()

    # 토큰화 및 정수 인코딩. OOV 문제 발생 시 <UNK> 토큰에 해당하는 인덱스 1 할당
    tokens = word_tokenize(text)
    token_indices = [word_to_index.get(token.lower(), 1) for token in tokens]

    # 리스트를 텐서로 변경
    input_tensor = torch.tensor([token_indices], dtype=torch.long).to(device)  # (1, seq_length)

    # 모델의 예측
    with torch.no_grad():
        logits = model(input_tensor)  # (1, output_dim)

    # 레이블 인덱스 예측
    _, predicted_index = torch.max(logits, dim=1)  # (1,)

    # 인덱스와 매칭되는 카테고리 문자열로 변경
    predicted_tag = index_to_tag[predicted_index.item()]

    return predicted_tag
```

먼저 model.eval()은 모델을 평가 모드로 전환합니다. 이 단계에서는 모델이 예측을 할 때 학습과정에서 사용되었던 드롭아웃 같은 기능이 비활성화됩니다. 텍스트는 word_tokenize 함수를 사용해 단어 단위로 분리되며, 각 단어는 소문자로 변환된 후 사전에 정의된 word_to_index 사전에서 해당 단어의 인덱스를 찾아 정수로 변환됩니다. 만약 사전에 없는 단어가 발견되면, 해당 단어는 `<UNK>`로 처리되어 인덱스 1이 할당됩니다.

변환된 정수 인덱스 리스트는 PyTorch 텐서로 변환되어 모델에 입력됩니다. 이 텐서는 배치 크기가 1인 2차원 텐서로, 입력 텍스트의 각 단어가 인덱스로 변환된 결과를 담고 있습니다. with torch.no_grad() 구문은 모델의 예측 과정에서 기울기 계산을 비활성화하여 메모리와 연산 속도를 최적화합니다. 이로써 입력된 텍스트에 대해 모델의 예측값을 계산합니다.

모델의 출력인 logits는 각 감정 클래스에 대한 점수로, 이 중에서 가장 높은 점수를 가진 인덱스가 예측된 감정 클래스로 선택됩니다. 이 인덱스는 index_to_tag 사전을 통해 '긍정' 또는 '부정'이라는 문자열로 변환됩니다. 최종적으로 예측된 감정 클래스를 반환합니다.

이제 임의의 입력에 대해서 예측을 해봅시다. 영화에 대한 부정적인 리뷰를 predict() 함수의 입력으로 사용해봅시다.

```python
test_input = "This movie was just way too overrated. The fighting was not professional and in slow motion. I was expecting more from a 200 million budget movie. The little sister of T.Challa was just trying too hard to be funny. The story was really dumb as well. Don't watch this movie if you are going because others say its great unless you are a Black Panther fan or Marvels fan."

predict(test_input, model, word_to_index, index_to_tag)
```

```python
부정
```

이번에는 영화에 대해서 극찬하는 리뷰를 predict() 함수의 입력으로 사용해봅시다.

```python
test_input = " I was lucky enough to be included in the group to see the advanced screening in Melbourne on the 15th of April, 2012. And, firstly, I need to say a big thank-you to Disney and Marvel Studios. Now, the film... how can I even begin to explain how I feel about this film? It is, as the title of this review says a 'comic book triumph'. I went into the film with very, very high expectations and I was not disappointed. Seeing Joss Whedon's direction and envisioning of the film come to life on the big screen is perfect. The script is amazingly detailed and laced with sharp wit a humor. The special effects are literally mind-blowing and the action scenes are both hard-hitting and beautifully choreographed."

predict(test_input, model, word_to_index, index_to_tag)
```

```python
긍정
```

---

## LSTM vs. GRU

![[Pasted image 20260831141255.png|515]]

![[Pasted image 20260831141232.png]]

LSTM => nn.GRU
임베딩 후 gru 에 넣는것 동일하지만
주석에 써져있듯  cell 상태를 취급하지 않는다!

근데 이론적으로, 근본적으로 별 차이가 없기 때문에
그냥 게이트 통합된 LSTM = GRU고 거기서 거기임.

모델 자체도 중요하지만, 아래 과정이 더 중요함.
모델은 그냥 nn. 에서 가져와서 클래스 구정해주면 되는거지만 (이론에 따라)
아래 과정의 흐름을 이해해야 전체적인 ML 과정을 이해할 수 있음.

데이터 전처리/확인(Null 값처리, 길이확인 : nunique, drop_duplicates, value_counts, isnull...)
=> 토큰화 (Mecab등 형태소 분석기 사용해서 토큰화, 불용어 제거 등) 
=> test에서 dev data(검증) 분리하기 (train_test_split() )
=> 중요하지 않은 희귀단어 쳐내고 vocab 생성 (정수매핑), wordtoidx, idxtoword list생성 
=> 패딩(zeros 한다음 채우기) => 모델 구현 & 텐서연산 위해 dataloader객체생성
=> 모델 학습(검증데이터 기준 교차엔트로피 loss 개선) => final predict test


# 13-04 1D CNN을 이용한 IMDB 리뷰 분류

합성곱 신경망을 자연어 처리에서 사용하기 위한 1D CNN을 이해하고, 1D CNN을 이용하여 IMDB 영화 리뷰 데이터를 분류해보겠습니다.

## 1. 1D CNN 이해하기

### 1) 2D 합성곱(2D Convolutions)

앞서 합성곱 신경망을 설명하며 합성곱 연산을 다음과 같이 정의했습니다.

```python
합성곱 연산이란 커널(kernel) 또는 필터(filter) 라는 n × m 크기의 행렬로 높이(height) × 너비(width) 크기의 이미지를 처음부터 끝까지 겹치며 훑으면서 n × m 크기의 겹쳐지는 부분의 각 이미지와 커널의 원소의 값을 곱해서 모두 더한 값을 출력으로 하는 것을 말합니다. 이때, 이미지의 가장 왼쪽 위부터 가장 오른쪽 아래까지 순차적으로 훑습니다.
```

위와 같은 << 이미지 처리에서의 합성곱 연산을 2D 합성곱 연산이라고 부릅>>니다.

### 2) 1D 합성곱(1D Convolutions)

자연어 처리에 사용되는 1D 합성곱 연산을 정리해봅시다. 

LSTM을 이용한 여러 실습을 상기해보면, 각 문장은 임베딩 층(embedding layer)을 지나서 각 단어가 임베딩 벡터가 된 상태로 LSTM의 입력이 되었습니다. 

이는 1D 합성곱 연산에서도 마찬가지입니다. 1D 합성곱 연산에서도 입력이 되는 것은 각 단어가 벡터로 변환된 문장 행렬로 LSTM과 입력을 받는 형태는 동일합니다.

'wait for the video and don't rent it'이라는 문장이 있을 때, 이 문장이 토큰화, 패딩, 임베딩 층(Embedding layer)을 거친다면 다음과 같은 문장 형태의 행렬로 변환될 것입니다. 아래 그림에서 은 문장의 길이, 는 임베딩 벡터의 차원입니다.

![](https://static.wikidocs.net/images/page/80437/sentence_matrix.PNG)

그리고 이 행렬이 만약 LSTM의 입력으로 주어진다면, LSTM은 첫번째 시점에는 첫번째 행을 입력으로 받고, 두번째 시점에는 두번째 행을 입력으로 받으며 순차적으로 단어를 처리합니다. 

그렇다면 1D 합성곱 연산의 경우에는 저 행렬을 어떻게 처리할까요?

1D 합성곱 연산에서 커널의 너비는 문장 행렬에서의 임베딩 벡터의 차원과 동일하게 설정됩니다. 그렇기 때문에 1D 합성곱 연산에서는 커널의 높이만으로 해당 커널의 크기라고 간주합니다. 가령, 커널의 크기가 2인 경우에는 아래의 그림과 같이 높이가 2, 너비가 임베딩 벡터의 차원인 커널이 사용됩니다. ( 커널의 너비는 단어임베딩벡터의 col=feature dim과 동일함. )

![](https://static.wikidocs.net/images/page/80437/1d_cnn.PNG)

커널의 너비가 임베딩 벡터의 차원이라는 의미는 커널이 2D 합성곱 연산때와는 달리 너비 방향으로는 더 이상 움직일 곳이 없다는 것을 의미합니다. 그래서 1D 합성곱 연산에서는 커널이 문장 행렬의 높이 방향으로만 움직이게 되어있습니다. 쉽게 설명하면, 위 그림에서 커널은 2D 합성곱 연산때와는 달리 오른쪽으로는 움직일 공간이 없으므로, 아래쪽으로만 이동해야 합니다.
(2d=>1d니까 당연한 결과이긴 함)

한 번의 연산을 1 스텝(step)이라고 하였을 때, 합성곱 연산의 네번째 스텝까지 표현한 이미지는 다음과 같습니다. 크기가 2인 커널은 처음에는 'wait for'에 대해서 합성곱 연산을 하고, 두번째 스텝에는 'for the'에 대해서 연산을, 세번째 스텝에는 'the video'에 대해서 연산을, 네번째 스텝에서는 'video and'에 대해서 연산을 하게 됩니다.

![](https://static.wikidocs.net/images/page/80437/%EB%84%A4%EB%B2%88%EC%A7%B8%EC%8A%A4%ED%85%9D.PNG)

이렇게 여덟번째 스텝까지 반복하였을 때, 결과적으로는 우측의 8차원 벡터를 1D 합성곱 연산의 결과로서 얻게될 것입니다. 

그런데 커널의 크기가 꼭 2일 필요가 있을까요? 2D 합성곱 연산에서 커널의 크기가 3 × 3 또는 5 × 5 또는 등등의 여러 크기의 커널을 자유자재로 사용할 수 있었듯이, 1D 합성곱 연산에서도 커널의 크기는 사용자가 변경할 수 있습니다. 

가령, 커널의 크기를 3으로 한다면, 네번째 스텝에서의 연산은 아래의 그림과 같을 것입니다.
(약간 Ngram, 슬라이딩윈도우 느낌..)
(N gram 이랑비슷하긴한데 얘는 빈도로만 하는거고.. CNN은 학습을하는거임. 
CNN쓰면 학습Ngram + n이다양한병렬n커널 돌려서 합칠 수도 있음.)

![](https://static.wikidocs.net/images/page/80437/%EC%BB%A4%EB%84%903.PNG)

커널의 크기가 달라진다는 것은 어떤 의미가 있을까요? CNN에서의 커널은 신경망 관점에서는 가중치 행렬이므로 커널의 크기에 따라 학습하게 되는 파라미터의 수는 달라집니다. 1D 합성곱 연산과 자연어 처리 관점에서는 커널의 크기에 따라서 참고하는 단어의 묶음의 크기가 달라집니다. 

이는 참고하는 n-gram이 달라진다고 볼 수 있습니다. 커널의 크기가 2라면 각 연산의 스텝에서 참고하는 것은 bigram입니다. 커널의 크기가 3이라면 각 연산의 스텝에서 참고하는 것은 trigram입니다.

### 3) 맥스 풀링(Max-pooling)

이미지 처리에서의 CNN에서 그랬듯이, 일반적으로 1D 합성곱 연산을 사용하는 1D CNN에서도 합성곱 층(합성곱 연산 + 활성화 함수) 다음에는 풀링 층을 추가하게됩니다. 

그 중 대표적으로 사용되는 것이 맥스 풀링(Max-pooling)입니다. 맥스 풀링은 각 합성곱 연산으로부터 얻은 결과 벡터에서 가장 큰 값을 가진 스칼라 값을 빼내는 연산입니다.

아래의 그림은 크기가 2인 커널과 크기가 3인 커널 두 개의 커널로부터 각각 결과 벡터를 얻고, 각 벡터에서 가장 큰 값을 꺼내오는 맥스 풀링 연산을 보여줍니다.

![](https://static.wikidocs.net/images/page/80437/%EB%A7%A5%EC%8A%A4%ED%92%80%EB%A7%81.PNG)

## 2. 1D CNN을 이용한 IMDB 리뷰 분류

지금까지 배운 개념들을 가지고 텍스트 분류를 위한 CNN을 설계해봅시다. 우선, 설계하고자 하는 신경망은 이진 분류를 위한 신경망입니다. 단, 시그모이드 함수가 아니라 소프트맥스 함수를 사용할 것이므로 출력층에서 뉴런의 개수가 2인 신경망을 설계합니다.

```
[방식 A: 뉴런 1개 + 시그모이드]
Logit (1개) ──> [Sigmoid] ──> P(부정일 확률 = 1 - p, 긍정일 확률 = p)

[방식 B: 뉴런 2개 + 소프트맥스]
Logits (2개) ──> [Softmax] ──> [P(부정일 확률), P(긍정일 확률)]
```

- **방식 A (뉴런 1개 + Sigmoid):** "긍정일 확률 $p$" 하나만 구하고, 부정일 확률은 자동으로 $1 - p$로 계산합니다.
- **방식 B (뉴런 2개 + Softmax):** "부정일 확률 $p_0$"과 "긍정일 확률 $p_1$"을 **각각 따로 출력**한 뒤, 두 확률의 합이 1이 되도록 맞춥니다.    

> 💡 **수학적 사실:** 2개 클래스일 때 소프트맥스 함수는 **시그모이드 함수를 일반화한 정확히 같은 수학 식**입니다. 따라서 성능이나 결과의 차이가 없습니다.

(확장성이나 교차엔트로피-CrossEntropyLoss 가 softmax 연산 포함하고있어서 
이진이든 다부류든 모든 분류를 CrossEntropy+Softmax(뉴런N개) 파이프라인으로 통일해서 작성하는 관습이 있다고 함.)


![](https://static.wikidocs.net/images/page/80437/conv1d.PNG)

커널은 크기가 4인 커널 2개, 3인 커널 2개, 2인 커널 2개를 사용합니다. 

문장의 길이가 9인 경우, 합성곱 연산을 한 후에는 각각 6차원 벡터 2개, 7차원 벡터 2개, 8차원 벡터 2개를 얻습니다. 

벡터가 6개므로 맥스 풀링을 한 후에는 6개의 스칼라 값을 얻는데, 일반적으로 이렇게 얻은 스칼라값들은 전부 연결(concatenate)하여 하나의 벡터로 만들어줍니다. 

이렇게 얻은 벡터는 1D CNN을 통해서 문장으로부터 얻은 벡터입니다. 
이를 뉴런이 2개인 출력층에 완전 연결시키므로서(`nn.Linear()`를 사용) 텍스트 분류를 수행합니다.


### 1) 데이터 로드 및 단어 토큰화

```python
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import nltk
import torch
import urllib.request
from tqdm import tqdm
from collections import Counter
from nltk.tokenize import word_tokenize
from sklearn.model_selection import train_test_split

# 다 위에서 쓰던 거임
```

```python
nltk.download('punkt')
```

```python
urllib.request.urlretrieve("https://raw.githubusercontent.com/ukairia777/pytorch-nlp-tutorial/main/10.%20RNN%20Text%20Classification/dataset/IMDB%20Dataset.csv", filename="IMDB Dataset.csv")
```

영화 리뷰 데이터인 IMDB 리뷰 데이터를 로드합니다.

```python
df = pd.read_csv('IMDB Dataset.csv')
df
```

![](https://static.wikidocs.net/images/page/217064/imdb.PNG)

데이터의 개수는 총 5만개입니다. 

데이터에 결측값이 있는지 확인할 수 있는 방법 중 하나는 
데이터프레임의 정보를 확인할 수 있는 `info()`를 사용하는 것입니다.

```python
df.info()
```

```python
<class 'pandas.core.frame.DataFrame'>
RangeIndex: 50000 entries, 0 to 49999
Data columns (total 2 columns):
 #   Column     Non-Null Count  Dtype 
---  ------     --------------  ----- 
 0   review     50000 non-null  object
 1   sentiment  50000 non-null  object
dtypes: object(2)
memory usage: 781.4+ KB
```

review 열과 sentiment 열 모두 non-null(결측값이 아닌) 데이터가 5만개로 확인되므로 결측값은 없습니다. 결측값을 확인할 수 있는 또 다른 방법인 `.isnull().values.any()`를 사용하여 결측값 여부를 확인합니다.

```python
print('결측값 여부 :',df.isnull().values.any())
```

```python
결측값 여부 : False
```

False가 출력된다면 결측값은 없다는 의미입니다. 
레이블이 균등한지 Bar Chart를 통해 확인합니다.

```python
df['sentiment'].value_counts().plot(kind='bar')
```

![](https://static.wikidocs.net/images/page/217064/labels.PNG)

레이블의 실제 개수를 확인해봅시다.

```python
print('레이블 개수')
print(df.groupby('sentiment').size().reset_index(name='count'))
```

```python
레이블 개수
  sentiment  count
0  negative  25000
1  positive  25000
```

두 레이블의 개수는 동일합니다. 
레이블이 현재 'positive'와 'negative'로 구성되어져 있으므로 각각 1, 0으로 변환하고 정상 변환되었는지 확인하기 위해서 상위 5개의 행을 출력합니다.

```python
df['sentiment'] = df['sentiment'].replace(['positive','negative'],[1, 0])
df.head()
```

![](https://static.wikidocs.net/images/page/217064/imdb_data.PNG)

긍정 레이블은 1, 부정 레이블은 0으로 변환된 것을 확인하였습니다. 'review' 열은 X_data, 레이블에 해당하는 'sentiment' 열은 y_data에 저장 후 정상 변환 되었는지 확인하기 위해서 다시 한 번 개수를 출력합니다.

```python
X_data = df['review']
y_data = df['sentiment']
print('영화 리뷰의 개수: {}'.format(len(X_data)))
print('레이블의 개수: {}'.format(len(y_data)))
```

```python
영화 리뷰의 개수: 50000
레이블의 개수: 50000
```

훈련 데이터와 검증 데이터, 테스트 데이터로 데이터를 나눕니다. 

우선 훈련 데이터와 테스트 데이터를 5:5 비율로 나누고, 훈련 데이터를 다시 8:2 비율로 훈련 데이터와 검증 데이터로 나눕니다. 

sklearn의 train_test_split은 데이터를 나눌 때 굉장히 많이 사용하는 도구이므로 꼭 기억해둡시다. 데이터를 나눌 때 레이블의 비율을 유지하고 싶다면 레이블 데이터를 stratify에 명시해줄 수 있습니다.

```python
X_train, X_test, y_train, y_test = train_test_split(X_data, y_data, test_size=0.5, random_state=0, stratify=y_data)
X_train, X_valid, y_train, y_valid = train_test_split(X_train, y_train, test_size=.2, random_state=0, stratify=y_train)

print('--------훈련 데이터의 비율-----------')
print(f'부정 리뷰 = {round(y_train.value_counts()[0]/len(y_train) * 100,3)}%')
print(f'긍정 리뷰 = {round(y_train.value_counts()[1]/len(y_train) * 100,3)}%')
print('--------검증 데이터의 비율-----------')
print(f'부정 리뷰 = {round(y_valid.value_counts()[0]/len(y_valid) * 100,3)}%')
print(f'긍정 리뷰 = {round(y_valid.value_counts()[1]/len(y_valid) * 100,3)}%')
print('--------테스트 데이터의 비율-----------')
print(f'부정 리뷰 = {round(y_test.value_counts()[0]/len(y_test) * 100,3)}%')
print(f'긍정 리뷰 = {round(y_test.value_counts()[1]/len(y_test) * 100,3)}%')
```

```python
--------훈련 데이터의 비율-----------
부정 리뷰 = 50.0%
긍정 리뷰 = 50.0%
--------검증 데이터의 비율-----------
부정 리뷰 = 50.0%
긍정 리뷰 = 50.0%
--------테스트 데이터의 비율-----------
부정 리뷰 = 50.0%
긍정 리뷰 = 50.0%
```

훈련 데이터, 검증 데이터, 테스트 데이터 세 개의 데이터 모두 긍정 레이블과 부정 레이블 모두 50:50으로 레이블이 균등하게 유지된 채 분할된 것을 확인할 수 있습니다. 훈련 데이터, 검증 데이터, 테스트 데이터에 대해서 토큰화를 진행해봅시다. 

토큰화를 위해 토큰화 함수 `tokenize()`를 구현하였습니다. 토큰화 진행 시에 선택적으로 소문자화를 진행하고 싶다면 소문자화도 진행할 수 있습니다. 파이썬 문자열에 .lower()를 사용하면 해당 문자열을 소문자로 바꿔줍니다. 해당 함수로 훈련 데이터, 검증 데이터, 테스트 데이터에 대해서 모두 토큰화를 진행합니다.

```python
def tokenize(sentences):
  tokenized_sentences = []
  for sent in tqdm(sentences):
    tokenized_sent = word_tokenize(sent)
    tokenized_sent = [word.lower() for word in tokenized_sent]
    tokenized_sentences.append(tokenized_sent)
  return tokenized_sentences

tokenized_X_train = tokenize(X_train)
tokenized_X_valid = tokenize(X_valid)
tokenized_X_test = tokenize(X_test)
```

토큰화가 진행된 후의 훈련 데이터의 상위 2개 샘플을 출력해봅시다.

```python
# 상위 샘플 2개 출력
for sent in tokenized_X_train[:2]:
  print(sent)
```

```python
['have', 'you', 'ever', ',', 'or', 'do', 'you', 'have', ',', 'a', 'pet', 'who', "'s", 'been', 'with', 'you', 'through', 'thick', 'and', 'thin', ',', 'who', 'you', "'d", 'be', 'lost', 'without', ',', 'and', 'who', 'you', 'love', 'no', 'matter', 'what', '?', 'betcha', 'never', 'thought', 'they', 'feel', 'the', 'same', 'way', 'about', 'you', '!', '<', 'br', '/', '>', '<', 'br', '/', '>', 'wonderful', ... 중략 ...]
['i', 'hate', 'football', '!', '!', 'i', 'hate', 'football', 'fans', '!', 'i', 'hate', 'cars', '!', 'but', 'this', 'film', 'was', 'the', 'funniest', 'thing', 'i', 'have', 'seen', 'in', 'quite', 'some', 'time', '.', '<', 'br', '/', '>', '<', 'br', '/', '>', 'i', 'was', 'given', 'the', 'great', 'opportunity', 'to', 'see', 'this', 'film', 'at', 'the', 'weekend', ',', 'and', 'all', 'i', 'have', 'to', 'say', 'is', 'i', ... 중략 ...]
```

정상적으로 토큰화 된 것을 확인하였습니다. 결과가 너무 길어 책의 지면에서는 중략했습니다.

### 2) Vocab 만들기

이제 토큰화 된 훈련 데이터로부터 정수 인코딩을 진행하기 위한 단어 집합(Vocabulary)을 만들어봅시다. Counter 모듈을 사용하면 현재 갖고 있는 데이터에 존재하는 단어 종류의 총 개수와 각 단어에 대해서 등장 빈도를 카운트 할 수 있습니다.

```python
word_list = []
for sent in tokenized_X_train:
    for word in sent:
      word_list.append(word)

word_counts = Counter(word_list)
print('총 단어수 :', len(word_counts))
```

```python
총 단어수 : 100586
```

Counter 모듈을 통해 확인한 훈련 데이터에 존재하는 총 단어수는 100,586개입니다. 

이 100,586는 단어들의 집합(set)에서의 단어의 개수를 의미하므로 훈련 데이터에 존재하는 총 단어의 종류의 개수입니다. 

또한, 현재 word_counts에는 각 단어의 등장 빈도수가 기록되어져 있습니다. 영단어 'the'와 'love'의 등장 빈도수를 확인해봅시다.

```python
print('훈련 데이터에서의 단어 the의 등장 횟수 :', word_counts['the'])
print('훈련 데이터에서의 단어 love의 등장 횟수 :', word_counts['love'])
```

```python
훈련 데이터에서의 단어 the의 등장 횟수 : 265697
훈련 데이터에서의 단어 love의 등장 횟수 : 4984
```

word_counts에는 단어와 각 단어의 등장 빈도수가 기록되어져 있습니다. 그리고 이 정보가 총 100,586개 존재합니다. 단어의 등장 빈도수가 높은 순서대로 정렬하여 vocab이라는 변수에 저장한 후 빈도수가 가장 높은 상위 10개의 단어를 출력합니다.

```python
vocab = sorted(word_counts, key=word_counts.get, reverse=True)
print('등장 빈도수 상위 10개 단어')
print(vocab[:10])
```

```python
등장 빈도수 상위 10개 단어
['the', ',', '.', 'a', 'and', 'of', 'to', 'is', '/', '>']
```

여기서는 빈도수가 낮은 단어들은 자연어 처리에서 배제하고자 합니다. 등장 빈도수가 3회 미만인 단어들이 이 데이터에서 얼만큼의 비중을 차지하는지 확인해봅시다.

```python
threshold = 3
total_cnt = len(word_counts) # 단어의 수
rare_cnt = 0 # 등장 빈도수가 threshold보다 작은 단어의 개수를 카운트
total_freq = 0 # 훈련 데이터의 전체 단어 빈도수 총 합
rare_freq = 0 # 등장 빈도수가 threshold보다 작은 단어의 등장 빈도수의 총 합

# 단어와 빈도수의 쌍(pair)을 key와 value로 받는다.
for key, value in word_counts.items():
    total_freq = total_freq + value

    # 단어의 등장 빈도수가 threshold보다 작으면
    if(value < threshold):
        rare_cnt = rare_cnt + 1
        rare_freq = rare_freq + value

print('단어 집합(vocabulary)의 크기 :',total_cnt)
print('등장 빈도가 %s번 이하인 희귀 단어의 수: %s'%(threshold - 1, rare_cnt))
print("단어 집합에서 희귀 단어의 비율:", (rare_cnt / total_cnt)*100)
print("전체 등장 빈도에서 희귀 단어 등장 빈도 비율:", (rare_freq / total_freq)*100)
```

```python
단어 집합(vocabulary)의 크기 : 100586
등장 빈도가 2번 이하인 희귀 단어의 수: 61877
단어 집합에서 희귀 단어의 비율: 61.51651323245779
전체 등장 빈도에서 희귀 단어 등장 빈도 비율: 1.3294254426463437
```

등장 빈도가 threshold 값인 3회 미만. 즉, 2회 이하인 단어들은 단어 집합에서 무려 절반 이상을 차지합니다. 

하지만, 실제로 훈련 데이터에서 등장 빈도로 차지하는 비중은 상대적으로 매우 적은 수치인 1.32%밖에 되지 않습니다. 아무래도 등장 빈도가 2회 이하인 단어들은 자연어 처리에서 별로 중요하지 않을 듯 합니다. 그래서 이 단어들은 정수 인코딩 과정에서 배제시키겠습니다.

등장 빈도수가 2이하인 단어들의 수를 제외한 단어의 개수를 단어 집합의 최대 크기로 제한하겠습니다.

```python
# 전체 단어 개수 중 빈도수 2이하인 단어는 제거.
vocab_size = total_cnt - rare_cnt
vocab = vocab[:vocab_size]
print('단어 집합의 크기 :', len(vocab))
```

```python
단어 집합의 크기 : 38709
```

등장 빈도수가 2번 이하인 단어를 제거하자 단어 집합의 크기가 100,586개에서 38,709개로 줄었습니다. 아직 각 단어에 고유한 정수를 부여하는 작업을 진행하지는 않았습니다. 

해당 작업을 진행하기에 앞서 정수 0과 정수 1에는 특별한 용도의 단어를 부여하고자 합니다. 정수 0은 패딩을 위해서 사용하는 패딩 토큰인 `<PAD>`를 할당하고, 정수 1은 OOV(Out-Of-Vocabulary) 문제 발생 시에 모르는 단어에 정수 1을 할당하는 용도인 `<UNK>`를 할당합니다.

```python
word_to_index = {}
word_to_index['<PAD>'] = 0
word_to_index['<UNK>'] = 1

for index, word in enumerate(vocab) :
  word_to_index[word] = index + 2

vocab_size = len(word_to_index)
print('패딩 토큰과 UNK 토큰을 고려한 단어 집합의 크기 :', vocab_size)
```

```python
패딩 토큰과 UNK 토큰을 고려한 단어 집합의 크기 : 38711
```

### 3) 정수 인코딩

최종 단어 집합(Vocabulary)인 word_to_index를 이용하여 정수 인코딩을 진행해봅시다. 이를 위한 함수인 `texts_to_sequences()`를 구현합니다. 해당 함수는 주어진 데이터에서 각 단어를 word_to_index에 맵핑된 정수로 변환합니다. 이때 word_to_index에 존재하지 않는 단어가 등장한 경우에는 정수 1을 부여합니다.

```python
def texts_to_sequences(tokenized_X_data, word_to_index):
  encoded_X_data = []
  for sent in tokenized_X_data:
    index_sequences = []
    for word in sent:
      try:
          index_sequences.append(word_to_index[word])
      except KeyError:
          index_sequences.append(word_to_index['<UNK>'])
    encoded_X_data.append(index_sequences)
  return encoded_X_data

encoded_X_train = texts_to_sequences(tokenized_X_train, word_to_index)
encoded_X_valid = texts_to_sequences(tokenized_X_valid, word_to_index)
encoded_X_test = texts_to_sequences(tokenized_X_test, word_to_index)
```

정수 인코딩이 진행된 학습 데이터의 상위 2개 샘플을 출력해봅시다.

```python
# 상위 샘플 2개 출력
for sent in encoded_X_train[:2]:
  print(sent)
```

```python
[38, 29, 140, 3, 52, 54, 29, 38, 3, 5, 3406, 47, 19, 95, 22, 29, 161, 4059, 6, 1741, 3, 47, 29, 293, 39, 469, 218, 3, 6, 47, 29, 134, 71, 532, 61, 59, 25184, 130, 214, 44, 249, 2, 189, 114, 58, 29, 41, 12, 13, 10, 11, 12, 13, 10, 11, 384, 3, 384, 253, 26, 4, 57, 29, 38, 5, 2280, 1587, 23, 1477, 3, 17, 9, 5775, 8, 111, 29, 1440, 71, 532, 141, 677, 4, 16, 343, 8, 126, 17, 24, 43, 2, 75, 63, 16, 20 ... 중략 ...]
[16, 735, 2344, 41, 41, 16, 735, 2344, 467, 41, 16, 735, 1903, 41, 25, 17, 26, 20, 2, 1588, 165, 16, 38, 128, 15, 198, 62, 75, 4, 12, 13, 10, 11, 12, 13, 10, 11, 16, 20, 360, 2, 100, 1359, 8, 77, 17, 26, 42, 2, 2394, 3, 6, 43, 16, 38, 8, 147, 9, 16, 1445, 2395, 16, 3268, 3, 6, 63, 9, 14, 184, 8, 39, 1320, 15, 2, 2382, 6, 9728, 4, 520, 3, 17, 9, 40, 2344, 26, 29, 97, 354, 8, 77, 3, 109, 604, 41, 12, 13, 10, 11 ... 중략 ...]
```

정수 인코딩 된 결과를 역으로 복원해보기 위해서 각 단어에 정수가 맵핑된 word_to_index를 반대로 만든 index_to_word를 구현해보고 첫번째 샘플에 대해서 복원해봅시다.

```python
index_to_word = {}
for key, value in word_to_index.items():
    index_to_word[value] = key

decoded_sample = [index_to_word[word] for word in encoded_X_train[0]]
print('기존의 첫번째 샘플 :', tokenized_X_train[0])
print('복원된 첫번째 샘플 :', decoded_sample)
```

```python
기존의 첫번째 샘플 : ['have', 'you', 'ever', ',', 'or', 'do', ... 중략 ... 'heart-swelling', 'feeling', '.', 'i', 'give', 'this', '9/10', '.', 'to', 'be', 'compared', 'to', '(', 'and', 'even', 'rated', 'better', 'than', ')', 'cats', 'and', 'dogs', 'and', 'babe', '.']
복원된 첫번째 샘플 : ['have', 'you', 'ever', ',', 'or', 'do', ... 중략 ... '<UNK>', 'feeling', '.', 'i', 'give', 'this', '9/10', '.', 'to', 'be', 'compared', 'to', '(', 'and', 'even', 'rated', 'better', 'than', ')', 'cats', 'and', 'dogs', 'and', 'babe', '.']
```

내용이 너무 길어서 중략했습니다. 기존의 첫번째 샘플과는 달리 정수 인코딩 후 다시 역으로 복원한 첫번째 샘플은 중간에 `<UNK>`이 있는 것을 확인할 수 있습니다.

### 4) 패딩

서로 다른 길이의 데이터들을 동일한 길이로 일치시켜주는 패딩 작업을 진행해봅시다. 이를 위해서 훈련 데이터의 최대 길이, 평균 길이, 그리고 데이터의 길이 분포를 확인합니다.

```python
print('리뷰의 최대 길이 :',max(len(review) for review in encoded_X_train))
print('리뷰의 평균 길이 :',sum(map(len, encoded_X_train))/len(encoded_X_train))
plt.hist([len(review) for review in encoded_X_train], bins=50)
plt.xlabel('length of samples')
plt.ylabel('number of samples')
plt.show()
```

```python
리뷰의 최대 길이 : 2818
리뷰의 평균 길이 : 279.1958
```

![](https://static.wikidocs.net/images/page/217064/lengthofsamples.PNG)

가장 긴 샘플의 길이는 2,818이며, 그래프를 봤을 때 전체 데이터의 길이 분포는 대체적으로 약 1,000내외의 길이를 가지는 것을 볼 수 있습니다. 모델이 처리할 수 있도록 encoded_X_train과 encoded_X_test의 모든 샘플의 길이를 특정 길이로 동일하게 맞춰줄 필요가 있습니다. 특정 길이 변수를 max_len으로 정합니다. 대부분의 리뷰가 내용이 잘리지 않도록 할 수 있는 최적의 max_len의 값은 몇일까요? 전체 샘플 중 길이가 max_len 이하인 샘플의 비율이 몇 %인지 확인하는 함수를 만듭니다.

```python
def below_threshold_len(max_len, nested_list):
  count = 0
  for sentence in nested_list:
    if(len(sentence) <= max_len):
        count = count + 1
  print('전체 샘플 중 길이가 %s 이하인 샘플의 비율: %s'%(max_len, (count / len(nested_list))*100))
```

최대 길이 2,818로 모든 샘플을 패딩하는 것은 조금 과한 처사일 것입니다. 500으로 할 경우 몇 개의 샘플을 손상시키지 않는지 확인해봅시다.

```python
max_len = 500
below_threshold_len(max_len, encoded_X_train)
```

```python
전체 샘플 중 길이가 500 이하인 샘플의 비율: 87.795
```

500으로 패딩할 경우 약 88%의 샘플은 그대로 보존됩니다. 더 많은 샘플을 보존하기 위해서는 500보다 더 큰 길이로 패딩할 수도 있겠지만, 여기서는 500으로 진행해보겠습니다. 이를 위해 패딩을 해주는 함수 `pad_sequences()`를 구현합니다. 해당 함수는 최대 길이를 정하면 해당 길이보다 긴 데이터는 뒷 부분을 잘라서 해당 길이로 맞추고, 해당 길이보다 짧은 데이터는 뒤에 0을 채워서 해당 길이의 데이터로 변환합니다. 결과적으로 길이 500으로 패딩을 하면 모든 데이터의 길이는 500이 됩니다.

```python
def pad_sequences(sentences, max_len):
  features = np.zeros((len(sentences), max_len), dtype=int)
  for index, sentence in enumerate(sentences):
    if len(sentence) != 0:
      features[index, :len(sentence)] = np.array(sentence)[:max_len]
  return features

padded_X_train = pad_sequences(encoded_X_train, max_len=max_len)
padded_X_valid = pad_sequences(encoded_X_valid, max_len=max_len)
padded_X_test = pad_sequences(encoded_X_test, max_len=max_len)

print('훈련 데이터의 크기 :', padded_X_train.shape)
print('검증 데이터의 크기 :', padded_X_valid.shape)
print('테스트 데이터의 크기 :', padded_X_test.shape)
```

```python
훈련 데이터의 크기 : (20000, 500)
검증 데이터의 크기 : (5000, 500)
테스트 데이터의 크기 : (25000, 500)
```

### 5) 모델링

이제 딥 러닝 프레임워크 PyTorch를 이용하여 1D CNN 모델을 구현해봅시다.

```python
import torch
import torch.nn as nn
import torch.nn.functional as F
```

현재 실습 환경에서 GPU를 사용 가능한지 확인합니다.

```python
USE_CUDA = torch.cuda.is_available()
device = torch.device("cuda" if USE_CUDA else "cpu")
print("cpu와 cuda 중 다음 기기로 학습함:", device)
```

```python
cpu와 cuda 중 다음 기기로 학습함: cuda
```

저자의 경우 Colab에서 GPU를 선택하여 실습을 진행하여 cuda라는 출력 결과를 확인했습니다. 레이블 데이터를 파이토치의 텐서 타입으로 변환합니다. 이후 훈련 데이터의 상위 5개의 레이블을 출력해보았습니다.

```python
train_label_tensor = torch.tensor(np.array(y_train))
valid_label_tensor = torch.tensor(np.array(y_valid))
test_label_tensor = torch.tensor(np.array(y_test))
print(train_label_tensor[:5])
```

```python
tensor([1, 1, 0, 0, 0])
```

아래는 합성곱 신경망의 동작 과정을 보여줍니다.

```python
# input.shape == (배치 크기, 임베딩 벡터의 차원, 문장 길이)
input = torch.randn(32, 16, 50)

# 선언 시 nn.Conv1d(임베딩 벡터의 차원, 커널의 개수, 커널 사이즈)
m = nn.Conv1d(16, 33, 3, stride=1)

# output.shape == (배치 크기, 커널의 개수, 컨볼루션 연산 결과 벡터)
output = m(input)
print(output.shape)
```

```python
torch.Size([32, 33, 48])
```

CNN 모델을 클래스로 구현해봅시다.

```python
class CNN(torch.nn.Module):
  def __init__(self, vocab_size, num_labels):
    super(CNN, self).__init__()

    # 오직 하나의 종류의 필터만 사용함.
    self.num_filter_sizes = 1 # 윈도우 5짜리 1개만 사용
    self.num_filters = 256

    self.word_embed = torch.nn.Embedding(num_embeddings=vocab_size, embedding_dim=128, padding_idx=0)
    # 윈도우 5짜리 1개만 사용
    self.conv1 = torch.nn.Conv1d(128, self.num_filters, 5, stride=1)
    self.dropout = torch.nn.Dropout(0.5)
    self.fc1 = torch.nn.Linear(1 * self.num_filters, num_labels, bias=True)

  def forward(self, inputs):
    # word_embed(inputs).shape == (배치 크기, 문장길이, 임베딩 벡터의 차원)
    # word_embed(inputs).permute(0, 2, 1).shape == (배치 크기, 임베딩 벡터의 차원, 문장 길이)
    embedded = self.word_embed(inputs).permute(0, 2, 1)

    # max를 이용한 maxpooling
    # conv1(embedded).shape == (배치 크기, 커널 개수, 컨볼루션 연산 결과) == ex) 32, 256, 496
    # conv1(embedded).permute(0, 2, 1).shape == (배치 크기, 컨볼루션 연산 결과, 커널 개수)
    # conv1(embedded).permute(0, 2, 1).max(1)[0]).shape == (배치 크기, 커널 개수)
    x = F.relu(self.conv1(embedded).permute(0, 2, 1).max(1)[0])

    # y_pred.shape == (배치 크기, 분류할 카테고리의 수)
    y_pred = self.fc1(self.dropout(x)) 

    return y_pred
```

우선, `__init__` 메서드에서는 모델의 구조와 층들을 초기화합니다. 

vocab_size는 어휘 크기를, num_labels는 분류할 카테고리의 수를 나타냅니다.
word_embed 층은 입력 단어를 128차원의 임베딩 벡터로 변환하고, conv1 층은 1D 합성곱 연산을 수행하여 << 256개의 특징 맵 >> 을 생성합니다. 

이때 커널 크기는 5, 스트라이드는 1로 설정되어 있습니다. dropout 층은 오버피팅을 방지하기 위해 사용되며, 드롭아웃 확률은 0.5입니다. 마지막으로 fc1 층은 합성곱 연산 결과를 받아 분류 카테고리의 수에 맞게 변환합니다.

forward 메서드는 모델의 순전파 과정을 정의합니다. 
입력 텐서 inputs를 word_embed 층에 통과시켜 임베딩 벡터로 변환한 후, 
conv1 층에서 합성곱 연산을 수행합니다. 
합성곱 연산 결과에 대해 맥스 풀링(max pooling)을 적용하여 가장 큰 값을 추출하고, ReLU 활성화 함수를 적용합니다. 그 다음, dropout 층을 통과시키고, fc1 층에서 최종 분류 결과를 계산합니다.

이 모델은 텍스트 분류 작업에 사용될 수 있으며, 입력으로 단어 시퀀스를 받아 해당 시퀀스가 어떤 카테고리에 속하는지 예측합니다. 합성곱 연산을 통해 << 지역적인 특징을 추출하고, 맥스 풀링을 통해 중요한 특징을 강조하여 분류 성능을 향상 >> 시킵니다.

훈련 데이터, 검증 데이터, 테스트 데이터에 대해서 파이토치 텐서로 변환하고 배치 단위 연산을 위해 데이터로더로 변환합니다.

```python
encoded_train = torch.tensor(padded_X_train).to(torch.int64)
train_dataset = torch.utils.data.TensorDataset(encoded_train, train_label_tensor)
train_dataloader = torch.utils.data.DataLoader(train_dataset, shuffle=True, batch_size=32)

encoded_test = torch.tensor(padded_X_test).to(torch.int64)
test_dataset = torch.utils.data.TensorDataset(encoded_test, test_label_tensor)
test_dataloader = torch.utils.data.DataLoader(test_dataset, shuffle=True, batch_size=1)

encoded_valid = torch.tensor(padded_X_valid).to(torch.int64)
valid_dataset = torch.utils.data.TensorDataset(encoded_valid, valid_label_tensor)
valid_dataloader = torch.utils.data.DataLoader(valid_dataset, shuffle=True, batch_size=1)
```

훈련 데이터의 샘플 개수가 20,000개 였으므로 배치 크기를 32로 할 경우에는 20000/32=625 다시 말해 32개씩 묶인 데이터 묶음이 625개가 생깁니다. 그리고 학습 시에는 32개씩 데이터가 들어가게 될 것입니다.

```python
total_batch = len(train_dataloader)
print('총 배치의 수 : {}'.format(total_batch))
```

```python
총 배치의 수 : 625
```

모델 객체를 선언합니다.

```python
model = CNN(vocab_size, num_labels = len(set(y_train)))
model.to(device)
```

임베딩 벡터의 차원은 128, 출력층의 크기(분류해야 할 카테고리의 개수)는 2로 정했습니다. 
이렇게 사용자가 정해주는 값이면서 모델의 결과에 영향을 미치는 값들을 하이퍼파라미터라고 합니다. 소프트맥스 회귀를 통해 분류 문제를 진행하므로 손실 함수는 nn.CrossEntropyLoss()를 사용합니다. 파이토치로 자연어 처리를 하게 되면 가장 많이 사용하게 되는 손실 함수입니다. 하이퍼파라미터 중 하나인 학습률(learning rate)는 0.001로 정했습니다.

```python
criterion = nn.CrossEntropyLoss()
optimizer = torch.optim.Adam(model.parameters(), lr=0.001)
```


### 6) 평가 코드 작성

이후 평가를 진행하기 위해서 모델의 정확도를 측정하는 함수 `calculate_accuracy()`를 작성합니다.

```python
def calculate_accuracy(logits, labels):
    # _, predicted = torch.max(logits, 1)
    predicted = torch.argmax(logits, dim=1)
    correct = (predicted == labels).sum().item()
    total = labels.size(0)
    accuracy = correct / total
    return accuracy
```

검증 데이터와 테스트 데이터에 대한 성능을 측정하기 위한 함수 `evaluate()`를 작성합니다. 아래의 함수에서 `model.eval()`과 `with torch.no_grad()`를 짚어봅시다. 이 두 개는 모델 평가를 수행할 때 중요한 역할을 합니다. 각각의 의미는 다음과 같습니다.

- model.eval(): 모델을 평가 모드로 설정합니다. 이렇게 하면 모델 내부의 모든 레이어에 대해 평가 모드가 활성화됩니다. 일부 레이어, 예를 들어 드롭아웃이나 배치 정규화는 학습과 평가 시 다르게 동작하기 때문에 이 설정이 중요합니다. 평가 모드가 설정되지 않으면, 이러한 레이어의 동작이 올바르지 않을 수 있으며, 이로 인해 평가 결과가 제대로 나오지 않을 수 있습니다.
    
- with torch.no_grad(): 이 문장은 자동 미분 엔진에서 기울기(gradient) 계산을 비활성화합니다. 평가 중에는 기울기를 계산할 필요가 없으므로, 이렇게 설정하면 메모리를 절약하고 속도를 높일 수 있습니다. 만약 이 설정이 적용되지 않으면, 평가 과정에서 기울기(gradient)가 계산되고 메모리를 차지하게 됩니다. 그러나 평가 결과 자체에는 직접적인 영향을 주지 않습니다.
    

따라서 model.eval()은 평가 시 반드시 사용해야 하며, 그렇지 않으면 평가 결과가 올바르게 나오지 않을 수 있습니다. with torch.no_grad():는 필수는 아니지만, 메모리와 속도 측면에서 권장됩니다.

```python
def evaluate(model, valid_dataloader, criterion, device):
    val_loss = 0
    val_correct = 0
    val_total = 0

    model.eval()
    with torch.no_grad():
        # 데이터로더로부터 배치 크기만큼의 데이터를 연속으로 로드
        for batch_X, batch_y in valid_dataloader:
            batch_X, batch_y = batch_X.to(device), batch_y.to(device)

            # 모델의 예측값
            logits = model(batch_X)

            # 손실을 계산
            loss = criterion(logits, batch_y)

            # 정확도와 손실을 계산함
            val_loss += loss.item()
            val_correct += calculate_accuracy(logits, batch_y) * batch_y.size(0)
            val_total += batch_y.size(0)

    val_accuracy = val_correct / val_total
    val_loss /= len(valid_dataloader)

    return val_loss, val_accuracy
```

### 7) 학습

```python
num_epochs = 5

# Training loop
best_val_loss = float('inf')

# Training loop
for epoch in range(num_epochs):
    # Training
    train_loss = 0
    train_correct = 0
    train_total = 0
    model.train()
    for batch_X, batch_y in train_dataloader:
        # Forward pass
        batch_X, batch_y = batch_X.to(device), batch_y.to(device)
        # batch_X.shape == (batch_size, max_len)
        logits = model(batch_X)

        # Compute loss
        loss = criterion(logits, batch_y)

        # Backward pass and optimization
        optimizer.zero_grad()
        loss.backward()
        optimizer.step()

        # Calculate training accuracy and loss
        train_loss += loss.item()
        train_correct += calculate_accuracy(logits, batch_y) * batch_y.size(0)
        train_total += batch_y.size(0)

    train_accuracy = train_correct / train_total
    train_loss /= len(train_dataloader)

    # Validation
    val_loss, val_accuracy = evaluate(model, valid_dataloader, criterion, device)

    print(f'Epoch {epoch+1}/{num_epochs}:')
    print(f'Train Loss: {train_loss:.4f}, Train Accuracy: {train_accuracy:.4f}')
    print(f'Validation Loss: {val_loss:.4f}, Validation Accuracy: {val_accuracy:.4f}')

    # 검증 손실이 최소일 때 체크포인트 저장
    if val_loss < best_val_loss:
        print(f'Validation loss improved from {best_val_loss:.4f} to {val_loss:.4f}. 체크포인트를 저장합니다.')
        best_val_loss = val_loss
        torch.save(model.state_dict(), 'best_model_checkpoint.pth')
```

학습은 정해진 횟수(num_epochs)만큼 반복되는데, 여기서는 5번 반복하도록 설정되어 있습니다. 학습 과정에서는 train_dataloader에서 배치(batch) 단위로 데이터를 가져와서 모델에 입력합니다. 모델은 입력 데이터를 처리하여 예측값(logits)을 출력하고, 이를 실제 정답(batch_y)과 비교하여 손실(loss)을 계산합니다. 그 다음, 손실을 기반으로 모델의 가중치를 조정하는 역전파(backward pass)와 최적화(optimization) 과정을 거칩니다.

각 배치마다 계산된 손실과 정확도는 에포크 단위로 누적되어 평균값으로 계산됩니다. 에포크가 끝날 때마다 학습 손실(train_loss), 학습 정확도(train_accuracy), 검증 손실(val_loss), 검증 정확도(val_accuracy)를 출력하여 모델의 성능을 모니터링합니다.

검증 손실(val_loss)이 이전에 기록된 최소 검증 손실(best_val_loss)보다 작아지면, 해당 에포크의 모델 가중치를 체크포인트(checkpoint)로 저장합니다. 이를 통해 가장 성능이 좋은 모델을 저장할 수 있습니다. 이 과정을 설정된 에포크 수만큼 반복하면서 모델을 학습시키고, 최종적으로 가장 좋은 성능을 보인 모델의 가중치를 얻게 됩니다.

### 8) 모델 로드 및 평가

학습 과정에서 검증 손실이 최소 일때 체크포인트를 저장해두었습니다. 해당 체크포인트를 베스트 모델로 판단하고 해당 체크포인트를 로드하여 모델 성능을 평가합니다.

```python
# 모델 로드
model.load_state_dict(torch.load('best_model_checkpoint.pth'))

# 모델을 device에 올립니다.
model.to(device)
```

```python
# 검증 데이터에 대한 정확도와 손실 계산
val_loss, val_accuracy = evaluate(model, valid_dataloader, criterion, device)

print(f'Best model validation loss: {val_loss:.4f}')
print(f'Best model validation accuracy: {val_accuracy:.4f}')
```

```python
Best model validation loss: 0.2951
Best model validation accuracy: 0.8816
```

검증 데이터에서의 평가 시 정확도는 88.16%, 손실은 0.2951입니다.

```python
# 테스트 데이터에 대한 정확도와 손실 계산
test_loss, test_accuracy = evaluate(model, test_dataloader, criterion, device)

print(f'Best model test loss: {test_loss:.4f}')
print(f'Best model test accuracy: {test_accuracy:.4f}')
```

```python
Best model test loss: 0.3060
Best model test accuracy: 0.8728
```

테스트 데이터에서의 평가 시 정확도는 87.28%, 손실은 0.3060입니다. 
다음의 13-03 실습에서 이 ==정확도와 손실을 좀 더 높여보는== 실습을 진행할 예정입니다.

### 9) 모델 테스트

이제 임의의 입력으로부터 모델의 성능을 예측해보는 테스트 함수 `predict()` 함수를 작성하고, 해당 함수에 전처리가 되어져 있지 않은 영화 리뷰 텍스트를 입력으로 넣어서 모델의 예측을 얻어봅시다.

```python
index_to_tag = {0 : '부정', 1 : '긍정'}

def predict(text, model, word_to_index, index_to_tag):
    # 모델 평가 모드
    model.eval()

    # 토큰화 및 정수 인코딩. OOV 문제 발생 시 <UNK> 토큰에 해당하는 인덱스 1 할당
    tokens = word_tokenize(text)
    token_indices = [word_to_index.get(token.lower(), 1) for token in tokens]

    # 리스트를 텐서로 변경
    input_tensor = torch.tensor([token_indices], dtype=torch.long).to(device)  # (1, seq_length)

    # 모델의 예측
    with torch.no_grad():
        logits = model(input_tensor)  # (1, output_dim)

    # 레이블 인덱스 예측
    _, predicted_index = torch.max(logits, dim=1)  # (1,)

    # 인덱스와 매칭되는 카테고리 문자열로 변경
    predicted_tag = index_to_tag[predicted_index.item()]

    return predicted_tag
```

부정적인 영화 리뷰를 넣어 모델이 부정이라고 잘 예측하는지 테스트합니다.

```python
test_input = "This movie was just way too overrated. The fighting was not professional and in slow motion. I was expecting more from a 200 million budget movie. The little sister of T.Challa was just trying too hard to be funny. The story was really dumb as well. Don't watch this movie if you are going because others say its great unless you are a Black Panther fan or Marvels fan."

predict(test_input, model, word_to_index, index_to_tag)
```

```python
부정
```

긍정적인 영화 리뷰를 넣어 모델이 부정이라고 잘 예측하는지 테스트합니다.

```python
test_input = " I was lucky enough to be included in the group to see the advanced screening in Melbourne on the 15th of April, 2012. And, firstly, I need to say a big thank-you to Disney and Marvel Studios. Now, the film... how can I even begin to explain how I feel about this film? It is, as the title of this review says a 'comic book triumph'. I went into the film with very, very high expectations and I was not disappointed. Seeing Joss Whedon's direction and envisioning of the film come to life on the big screen is perfect. The script is amazingly detailed and laced with sharp wit a humor. The special effects are literally mind-blowing and the action scenes are both hard-hitting and beautifully choreographed."

predict(test_input, model, word_to_index, index_to_tag)
```

```python
긍정
```




# 13-05 사전 훈련된 임베딩을 이용한 성능 상승 시키기

이전 실습인 '1D CNN을 이용하여 IMDB 영화 리뷰 분류' 실습에 사전 훈련된 임베딩을 이용하는 코드를 추가하여 좀 더 높은 성능을 얻어보겠습니다.

## 1. 데이터 로드 및 단어 토큰화 ~ 4. 패딩

이전 실습과 모든 과정이 동일한 과정을 진행합니다.

## 5. 사전 훈련된 임베딩

사전 훈련된 임베딩을 사용하기 위해서는 머신 러닝 라이브러리 gensim의 설치가 필요합니다.

```python
!pip install gensim
```

구글이 이미 학습해놓은 사전 훈련된 워드 임베딩을 다운로드 합니다.

```python
!pip install gdown

!gdown https://drive.google.com/uc?id=1Av37IVBQAAntSe1X3MOAl5gvowQzd2_j
```

구글의 사전 훈련된 Word2vec 모델을 gensim을 통해 로드합니다.

```python
word2vec_model = gensim.models.KeyedVectors.load_word2vec_format('GoogleNews-vectors-negative300.bin.gz', binary=True)
```

구글의 사전 훈련된 임베딩은 학습되었을 당시 각 단어가 300차원으로 학습된 상태입니다. 
우리는 우리가 만든 vocab_size만큼의 행을 가지고, 300차원의 열을 가지는 행렬을 만듭니다.



```python
embedding_matrix = np.zeros((vocab_size, 300))
```

그 후 사전 훈련된 임베딩을 우리가 만든 embedding_matrix에 맵핑합니다. 

예를 들어 우리가 앞서 만든 토크나이저 기준으로 36번이 단어 '사과'라면, embedding_matrix의 36번 행에 구글에서 만든 사전 훈련된 임베딩 벡터의 값이 '사과'인 벡터를 맵핑합니다.

```python
def get_vector(word):
    if word in word2vec_model:
        return word2vec_model[word]
    else:
        return None

# <PAD>를 위한 0번과 <UNK>를 위한 1번은 실제 단어가 아니므로 맵핑에서 제외
for word, i in word_to_index.items():
    if i > 2:
      temp = get_vector(word)
      if temp is not None:
          embedding_matrix[i] = temp
```

0번 임베딩 벡터를 출력해봅시다.

```python
# <PAD>나 <UNK>의 경우는 사전 훈련된 임베딩이 들어가지 않아서 0벡터임
embedding_matrix[0]
```

```python
array([0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
       0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
       0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
       0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
       0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
       0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
       0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
       0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
       0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
       0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
       0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
       0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
       0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
       0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
       0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
       0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
       0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
       0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.])
```

원소의 값이 전부 0인 0벡터임을 확인할 수 있습니다. 

현재 단어 집합에서 'apple'은 몇 번 정수로 맵핑되어져 있는지 확인해봅시다.

```python
word_to_index['apple']
```

```python
8053
```

구글의 사전 훈련된 Word2Vec에서의 'apple'의 임베딩 벡터값(`word2vec_model['apple']`)과 현재 임베딩 행렬의 8053번의 벡터가 일치하는지 확인합니다. 

이는 사전 훈련된 임베딩 벡터의 값이 우리의 임베딩 행렬에 정확하게 맵핑되었는지를 확인하기 위함입니다.

```python
# word2vec_model에서 'apple'의 임베딩 벡터
# embedding_matrix[8053]이 일치하는지 체크
np.all(word2vec_model['apple'] == embedding_matrix[8053])
```

```python
True
```

## 6. 모델링

위의 사전 훈련된 임베딩을 이용하여 모델을 만들어봅시다. 필요한 도구들을 임포트합니다.

```python
import torch
import torch.nn as nn
import torch.nn.functional as F
```

GPU를 사용 가능한 환경인지 확인합니다.

```python
USE_CUDA = torch.cuda.is_available()
device = torch.device("cuda" if USE_CUDA else "cpu")
print("cpu와 cuda 중 다음 기기로 학습함:", device)
```

```python
cpu와 cuda 중 다음 기기로 학습함: cuda
```

저자의 경우 Colab에서 GPU를 선택 후 실습하고 있어 'cuda'라고 출력되었으며 GPU가 사용 가능함을 의미합니다. 


사전 훈련된 임베딩을 사용하는 CNN 모델의 클래스는 다음과 같습니다. 
워드 임베딩 층에 위에서 사전 훈련된 임베딩으로 만들어놓은 임베딩 행렬인 `embedding_matrix`를 맵핑하고 있는 코드에 주목합시다.

```python
class CNN(torch.nn.Module):
  def __init__(self, vocab_size, num_labels):
    super(CNN, self).__init__()

    # 오직 하나의 종류의 필터만 사용함.
    self.num_filter_sizes = 1 # 윈도우 5짜리 1개만 사용
    self.num_filters = 256

    # 주석 처리된 코드는 기존의 임베딩 층을 사용할 경우
    # self.word_embed = nn.Embedding(num_embeddings=vocab_size, embedding_dim=128, padding_idx=0)
    self.word_embed = nn.Embedding(num_embeddings=vocab_size, embedding_dim=300)
    self.word_embed.weight = nn.Parameter(torch.tensor(embedding_matrix, dtype=torch.float32))
    self.word_embed.weight.requires_grad = True

    # 윈도우 5짜리 1개만 사용
    self.conv1 = torch.nn.Conv1d(300, self.num_filters, 5, stride=1)
    self.dropout = torch.nn.Dropout(0.5)
    self.fc1 = torch.nn.Linear(1 * self.num_filters, num_labels, bias=True)

  def forward(self, inputs):
    # word_embed(inputs).shape == (배치 크기, 문장길이, 임베딩 벡터의 차원)
    # word_embed(inputs).permute(0, 2, 1).shape == (배치 크기, 임베딩 벡터의 차원, 문장 길이)
    embedded = self.word_embed(inputs).permute(0, 2, 1)

    # max를 이용한 maxpooling
    # conv1(embedded).shape == (배치 크기, 커널 개수, 컨볼루션 연산 결과) == ex) 32, 256, 496
    # conv1(embedded).permute(0, 2, 1).shape == (배치 크기, 컨볼루션 연산 결과, 커널 개수)
    # conv1(embedded).permute(0, 2, 1).max(1)[0]).shape == (배치 크기, 커널 개수)
    x = F.relu(self.conv1(embedded).permute(0, 2, 1).max(1)[0])

    # y_pred.shape == (배치 크기, 분류할 카테고리의 수)
    y_pred = self.fc1(self.dropout(x))

    return y_pred
```

훈련 데이터, 검증 데이터, 테스트 데이터에 대해서 파이토치 텐서로 변환하고 배치 단위 연산을 위해 데이터로더로 변환합니다.

- 이 아래의 모든 코드는 이전 실습과 동일합니다.

```python
encoded_train = torch.tensor(padded_X_train).to(torch.int64)
train_dataset = torch.utils.data.TensorDataset(encoded_train, train_label_tensor)
train_dataloader = torch.utils.data.DataLoader(train_dataset, shuffle=True, batch_size=32)

encoded_test = torch.tensor(padded_X_test).to(torch.int64)
test_dataset = torch.utils.data.TensorDataset(encoded_test, test_label_tensor)
test_dataloader = torch.utils.data.DataLoader(test_dataset, shuffle=True, batch_size=1)

encoded_valid = torch.tensor(padded_X_valid).to(torch.int64)
valid_dataset = torch.utils.data.TensorDataset(encoded_valid, valid_label_tensor)
valid_dataloader = torch.utils.data.DataLoader(valid_dataset, shuffle=True, batch_size=1)
```

훈련 데이터의 샘플 개수가 20,000개 였으므로 배치 크기를 32로 할 경우에는 20000/32=625 다시 말해 32개씩 묶인 데이터 묶음이 625개가 생깁니다. 그리고 학습 시에는 32개씩 데이터가 들어가게 될 것입니다.

```python
total_batch = len(train_dataloader)
print('총 배치의 수 : {}'.format(total_batch))
```

```python
총 배치의 수 : 625
```

모델 객체를 선언합니다.

```python
model = CNN(vocab_size, num_labels = len(set(y_train)))
model.to(device)
```

임베딩 벡터의 차원은 300, 출력층의 크기(분류해야 할 카테고리의 개수)는 2로 정했습니다. 

이렇게 사용자가 정해주는 값이면서 모델의 결과에 영향을 미치는 값들을 하이퍼파라미터라고 합니다. 소프트맥스 회귀를 통해 분류 문제를 진행하므로 손실 함수는 nn.CrossEntropyLoss()를 사용합니다. 파이토치로 자연어 처리를 하게 되면 가장 많이 사용하게 되는 손실 함수입니다. 하이퍼파라미터 중 하나인 학습률(learning rate)는 0.001로 정했습니다.

```python
criterion = nn.CrossEntropyLoss()
optimizer = torch.optim.Adam(model.parameters(), lr=0.001)
```


## 7. 평가 코드 작성

이후 평가를 진행하기 위해서 모델의 정확도를 측정하는 함수 `calculate_accuracy()`를 작성합니다.

```python
def calculate_accuracy(logits, labels):
    # _, predicted = torch.max(logits, 1)
    predicted = torch.argmax(logits, dim=1)
    correct = (predicted == labels).sum().item()
    total = labels.size(0)
    accuracy = correct / total
    return accuracy
```

검증 데이터와 테스트 데이터에 대한 성능을 측정하기 위한 함수 `evaluate()`를 작성합니다. 아래의 함수에서 `model.eval()`과 `with torch.no_grad()`를 짚어봅시다. 이 두 개는 모델 평가를 수행할 때 중요한 역할을 합니다. 각각의 의미는 다음과 같습니다.

- model.eval(): 모델을 평가 모드로 설정합니다. 이렇게 하면 모델 내부의 모든 레이어에 대해 평가 모드가 활성화됩니다. 일부 레이어, 예를 들어 드롭아웃이나 배치 정규화는 학습과 평가 시 다르게 동작하기 때문에 이 설정이 중요합니다. 평가 모드가 설정되지 않으면, 이러한 레이어의 동작이 올바르지 않을 수 있으며, 이로 인해 평가 결과가 제대로 나오지 않을 수 있습니다.
    
- with torch.no_grad(): 이 문장은 자동 미분 엔진에서 기울기(gradient) 계산을 비활성화합니다. 평가 중에는 기울기를 계산할 필요가 없으므로, 이렇게 설정하면 메모리를 절약하고 속도를 높일 수 있습니다. 만약 이 설정이 적용되지 않으면, 평가 과정에서 기울기(gradient)가 계산되고 메모리를 차지하게 됩니다. 그러나 평가 결과 자체에는 직접적인 영향을 주지 않습니다.
    

따라서 model.eval()은 평가 시 반드시 사용해야 하며, 그렇지 않으면 평가 결과가 올바르게 나오지 않을 수 있습니다. with torch.no_grad():는 필수는 아니지만, 메모리와 속도 측면에서 권장됩니다.

```python
def evaluate(model, valid_dataloader, criterion, device):
    val_loss = 0
    val_correct = 0
    val_total = 0

    model.eval()
    with torch.no_grad():
        # 데이터로더로부터 배치 크기만큼의 데이터를 연속으로 로드
        for batch_X, batch_y in valid_dataloader:
            batch_X, batch_y = batch_X.to(device), batch_y.to(device)

            # 모델의 예측값
            logits = model(batch_X)

            # 손실을 계산
            loss = criterion(logits, batch_y)

            # 정확도와 손실을 계산함
            val_loss += loss.item()
            val_correct += calculate_accuracy(logits, batch_y) * batch_y.size(0)
            val_total += batch_y.size(0)

    val_accuracy = val_correct / val_total
    val_loss /= len(valid_dataloader)

    return val_loss, val_accuracy
```

## 8. 학습

```python
num_epochs = 5

# Training loop
best_val_loss = float('inf')

# Training loop
for epoch in range(num_epochs):
    # Training
    train_loss = 0
    train_correct = 0
    train_total = 0
    model.train()
    for batch_X, batch_y in train_dataloader:
        # Forward pass
        batch_X, batch_y = batch_X.to(device), batch_y.to(device)
        # batch_X.shape == (batch_size, max_len)
        logits = model(batch_X)

        # Compute loss
        loss = criterion(logits, batch_y)

        # Backward pass and optimization
        optimizer.zero_grad()
        loss.backward()
        optimizer.step()

        # Calculate training accuracy and loss
        train_loss += loss.item()
        train_correct += calculate_accuracy(logits, batch_y) * batch_y.size(0)
        train_total += batch_y.size(0)

    train_accuracy = train_correct / train_total
    train_loss /= len(train_dataloader)

    # Validation
    val_loss, val_accuracy = evaluate(model, valid_dataloader, criterion, device)

    print(f'Epoch {epoch+1}/{num_epochs}:')
    print(f'Train Loss: {train_loss:.4f}, Train Accuracy: {train_accuracy:.4f}')
    print(f'Validation Loss: {val_loss:.4f}, Validation Accuracy: {val_accuracy:.4f}')

    # 검증 손실이 최소일 때 체크포인트 저장
    if val_loss < best_val_loss:
        print(f'Validation loss improved from {best_val_loss:.4f} to {val_loss:.4f}. 체크포인트를 저장합니다.')
        best_val_loss = val_loss
        torch.save(model.state_dict(), 'best_model_checkpoint.pth')
```

학습은 정해진 횟수(num_epochs)만큼 반복되는데, 여기서는 5번 반복하도록 설정되어 있습니다. 학습 과정에서는 train_dataloader에서 배치(batch) 단위로 데이터를 가져와서 모델에 입력합니다. 모델은 입력 데이터를 처리하여 예측값(logits)을 출력하고, 이를 실제 정답(batch_y)과 비교하여 손실(loss)을 계산합니다. 그 다음, 손실을 기반으로 모델의 가중치를 조정하는 역전파(backward pass)와 최적화(optimization) 과정을 거칩니다.

각 배치마다 계산된 손실과 정확도는 에포크 단위로 누적되어 평균값으로 계산됩니다. 에포크가 끝날 때마다 학습 손실(train_loss), 학습 정확도(train_accuracy), 검증 손실(val_loss), 검증 정확도(val_accuracy)를 출력하여 모델의 성능을 모니터링합니다.

검증 손실(val_loss)이 이전에 기록된 최소 검증 손실(best_val_loss)보다 작아지면, 해당 에포크의 모델 가중치를 체크포인트(checkpoint)로 저장합니다. 이를 통해 가장 성능이 좋은 모델을 저장할 수 있습니다. 이 과정을 설정된 에포크 수만큼 반복하면서 모델을 학습시키고, 최종적으로 가장 좋은 성능을 보인 모델의 가중치를 얻게 됩니다.

## 9. 모델 로드 및 평가

```python
# 모델 로드
model.load_state_dict(torch.load('best_model_checkpoint.pth'))

# 모델을 device에 올립니다.
model.to(device)
```

검증 데이터와 테스트 데이터에 대한 정확도와 손실을 계산합니다. 전반적으로 이전 실습보다 더 높은 성능을 얻는 것을 확인할 수 있습니다.

```python
# 검증 데이터에 대한 정확도와 손실 계산
val_loss, val_accuracy = evaluate(model, valid_dataloader, criterion, device)

print(f'Best model validation loss: {val_loss:.4f}')
print(f'Best model validation accuracy: {val_accuracy:.4f}')
```

```python
Best model validation loss: 0.2523
Best model validation accuracy: 0.8996
```

```python
# 테스트 데이터에 대한 정확도와 손실 계산
test_loss, test_accuracy = evaluate(model, test_dataloader, criterion, device)

print(f'Best model test loss: {test_loss:.4f}')
print(f'Best model test accuracy: {test_accuracy:.4f}')
```

```python
Best model test loss: 0.2503
Best model test accuracy: 0.8983
```

## 10. 모델 테스트

```python
index_to_tag = {0 : '부정', 1 : '긍정'}

def predict(text, model, word_to_index, index_to_tag):
    # 모델 평가 모드
    model.eval()

    # 토큰화 및 정수 인코딩. OOV 문제 발생 시 <UNK> 토큰에 해당하는 인덱스 1 할당
    tokens = word_tokenize(text)
    token_indices = [word_to_index.get(token.lower(), 1) for token in tokens]

    # 리스트를 텐서로 변경
    input_tensor = torch.tensor([token_indices], dtype=torch.long).to(device)  # (1, seq_length)

    # 모델의 예측
    with torch.no_grad():
        logits = model(input_tensor)  # (1, output_dim)

    # 레이블 인덱스 예측
    _, predicted_index = torch.max(logits, dim=1)  # (1,)

    # 인덱스와 매칭되는 카테고리 문자열로 변경
    predicted_tag = index_to_tag[predicted_index.item()]

    return predicted_tag
```

```python
test_input = "This movie was just way too overrated. The fighting was not professional and in slow motion. I was expecting more from a 200 million budget movie. The little sister of T.Challa was just trying too hard to be funny. The story was really dumb as well. Don't watch this movie if you are going because others say its great unless you are a Black Panther fan or Marvels fan."

predict(test_input, model, word_to_index, index_to_tag)
```

```python
부정
```

```python
test_input = " I was lucky enough to be included in the group to see the advanced screening in Melbourne on the 15th of April, 2012. And, firstly, I need to say a big thank-you to Disney and Marvel Studios. Now, the film... how can I even begin to explain how I feel about this film? It is, as the title of this review says a 'comic book triumph'. I went into the film with very, very high expectations and I was not disappointed. Seeing Joss Whedon's direction and envisioning of the film come to life on the big screen is perfect. The script is amazingly detailed and laced with sharp wit a humor. The special effects are literally mind-blowing and the action scenes are both hard-hitting and beautifully choreographed."

predict(test_input, model, word_to_index, index_to_tag)
```

```python
긍정
```


![[Pasted image 20260831162227.png]]


![[Pasted image 20260831165414.png|496]]


0.8662 => 0.9014 수준으로 성능이 대폭 향상된 결과가 나옴.


# 용어 정리

## inf

컴퓨터가 `inf`를 다루는 원리를 2가지 핵심 포인트로 정리해 드립니다.

### 1. 메모리 레벨에서의 정의 (IEEE 754 표준)

컴퓨터는 부동소수점(`float32`, `float64`)을 메모리에 저장할 때 **\[부호비트 + 지수부 + 가수부]** 구조를 사용합니다.

이때 **`inf`만을 위한 전용 비트 패턴**이 규격으로 정해져 있습니다.

- **지수부(Exponent):** 비트를 전부 `1`로 채움
    
- **가수부(Fraction/Mantissa):** 비트를 전부 `0`으로 채움
    

즉, "숫자가 너무 커져서 어떤 유효숫자로 표현된 상태"가 아니라, **비트 패턴 자체가 "이 값은 무한대다"라는 표식**으로 지정되어 있는 것입니다.

### 2. 하드웨어(CPU/GPU) 및 연산 규격의 대응

컴퓨터의 연산 장치(ALU, FPU, GPU)에는 이 `inf` 비트 패턴을 만났을 때 **어떻게 대처해야 하는지에 대한 연산 규칙이 하드웨어 레벨에 이미 내장**되어 있습니다.

```Python
import numpy as np

# float32가 표현할 수 있는 진짜 '가장 큰 숫자'
max_float = np.finfo(np.float32).max  # 약 3.4028235e+38

inf_val = float('inf')

# 1. 가장 큰 숫자보다 inf가 엄연히 더 큽니다.
print(inf_val > max_float)  # True

# 2. 가장 큰 숫자에 1을 더하면 overflow가 나면서 inf로 바뀝니다.
print(max_float * 2)  # inf
```

- **비교 연산:** CPU/GPU는 `inf` 비트를 만나면 그 어떤 실수가 들어와도 무조건 `inf` 쪽이 더 크다고 판단하도록 동작합니다.
    
- **산술 연산:** `inf + 1 = inf`, `1 / inf = 0` 등의 특수 연산 결과가 하드웨어 규격(IEEE 754)에 의해 정의된 대로 반환됩니다.
    

### 💡 한 줄 요약

`inf`는 단순히 "아주 큰 숫자"가 아니라, **CPU/GPU 연산 장치가 "무한대"로 인식하고 대처하도록 비트 단위로 약속되어 있는 특수한 비트 상태**입니다!


---

### 딥러닝/머신러닝에서 `inf`가 나오는 대표적인 경우 3가지

#### ① 마스킹(Masking) 처리 시 (가장 흔함!)

어텐션(Attention) 메커니즘이나 패딩(Padding) 처리 시, **"특정 단어나 위치를 모델이 완전히 무시하게 만들고 싶을 때"** 마스크 자리에 `-inf` (음의 무한대)를 채워 넣습니다.

- **이유:** Softmax 함수에 $-\infty$가 들어가면 출력 확률이 정확히 `0`이 되기 때문입니다.    
$$e^{-\infty} = 0$$

#### ② 손실(Loss) 값이 폭발했을 때 (Grad Explosion)

모델 학습 중 기울기가 너무 커지거나 $0$으로 나누는 문제($1/0$)가 발생하면 손실(Loss) 값이 `inf`로 떠버리며 학습이 망가지게 됩니다.

#### ③ 최솟값을 찾는 알고리즘의 초기값

최솟값을 갱신해나가는 변수를 선언할 때, 초기값을 가장 큰 값인 `inf`로 설정해 둡니다.

```Python
best_loss = float('inf')  # 초기값을 무한대로 설정

if current_loss < best_loss:
    best_loss = current_loss  # 어떤 loss 값이 들어와도 무조건 첫 번째에 갱신됨
```

### 💡 한 줄 요약

`inf` = 무한대($\infty$)이며, 딥러닝에서는 **Softmax 확률을 0으로 만드는 마스킹**이나 **최솟값 비교 초기화**에 단골로 쓰입니다!



## CNN ReLU: 실무에서는 어떤 활성화함수를 쓸까

**실무에서는 standard ReLU를 기본(Baseline)으로 먼저 사용하며, 모델이나 데이터 특성에 따라 Leaky ReLU나 GELU 같은 변형들을 선택해서 사용합니다.**

"무조건 Leaky ReLU가 좋다"기보다는 상황과 필드(컴퓨터 비전 vs 자연어/멀티모달)에 따라 주로 쓰이는 활성화 함수가 달라집니다.

### 1. 실무에서 standard ReLU를 여전히 많이 쓰는 이유

- **기본 모델 및 라이브러리 표준:** ResNet, VGG, MobileNet 등 시대를 풍미한 대표적인 CNN 아키텍처 다수가 standard ReLU를 사용합니다.
    
- **빠른 연산 속도:** $f(x) = \max(0, x)$는 조건문이나 단순 비교 연산이라 GPU 연산 부담이 가장 적고 학습 속도가 매우 빠릅니다.
    
- **희소성(Sparsity) 제공:** 음수 입력값을 완전히 0으로 만들어 연산 효율성을 높이고 과적합을 방지하는 효과가 있습니다.
    

### 2. Leaky ReLU / PReLU를 사용하는 실무 상황

ReLU의 가장 큰 단점은 음수 기울기가 0이 되어 뉴런이 영구적으로 죽어버리는 **Dying ReLU 현상**입니다. 이를 방지하기 위해 아래 상황에서는 Leaky ReLU 변형을 선호합니다.

- **GAN (생성 모델):** 이미지 생성 모델(DCGAN 등)에서는 뉴런이 죽어버리면 생성 품질이 급격히 떨어지므로 Leaky ReLU(음수 기울기 0.2 등)가 필수적인 표준으로 쓰입니다.
    
- **Object Detection / Segmentation:** YOLO 시리즈나 일부 픽셀 단위 감지 모델에서는 정보 손실을 줄이기 위해 Leaky ReLU나 PReLU(기울기 자체도 학습하는 방식)를 적극 도입합니다.
    
- **모델 깊이가 깊고 Dying ReLU가 심할 때:** 학습 중 Loss가 떨어지지 않고 frozen되는 뉴런 비율이 높을 때 교체합니다.
    

### 3. 요즘(최신 CNN 및 멀티모달) 실무 트렌드: ==GELU와 SiLU (Swish)

최근 CNN 실무 환경에서는 단순 Leaky ReLU를 넘어 **GELU**나 **SiLU(Swish)** 같은 매끄러운(Smooth) 곡선형 활성화 함수의 사용이 크게 늘었습니다.

- **SiLU (Swish):** YOLOv5, EfficientNet, ConvNeXt 등 최신 CNN 아키텍처에서 아주 자주 쓰이며 performance 향상이 잘 나타납니다.
    
- **GELU:** Vision Transformer(ViT)나 최근의 현대적인 CNN(ConvNeXt 등)에서 표준으로 사용됩니다.
    

### 💡 실무 가이드라인 요약

1. **시작(Baseline):** 일단 **standard ReLU**로 시작하여 성능 기준점을 잡습니다.
    
2. **생성/특수 분야:** **GAN**이나 **YOLO 계열** 작업을 하거나 Dying ReLU 문제가 보이면 **Leaky ReLU**를 적용합니다.
    
3. **최신 SOTA 모델 설계:** 최신 아키텍처(ConvNeXt 등)나 트랜스포머 혼합 구조를 짠다면 **SiLU**나 **GELU**를 적용해 봅니다.



## 