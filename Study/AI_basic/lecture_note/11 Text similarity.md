
> 자연어 처리에서 텍스트를 표현하는 방법으로는 여러가지 방법이 있습니다. 이번 챕터에서는 그 중 정보 검색과 텍스트 마이닝 분야에서 주로 사용되는 카운트 기반의 텍스트 표현 방법인 DTM(Document Term Matrix)과 TF-IDF(Term Frequency-Inverse Document Frequency)에 대해서 다룹니다.

> 텍스트를 위와 같은 방식으로 수치화를 하고나면, 통계적인 접근 방법을 통해 여러 문서로 이루어진 텍스트 데이터가 있을 때 어떤 단어가 특정 문서 내에서 얼마나 중요한 것인지를 나타내거나, 문서의 핵심어 추출, 검색 엔진에서 검색 결과의 순위 결정, 문서들 간의 유사도를 구하는 등의 용도로 사용할 수 있습니다.

# 11-01 단어의 표현 방법 

단어의 표현 방법은 크게 국소 표현(Local Representation) 방법과
분산 표현(Distributed Representation) 방법으로 나뉩니다. 

국소 표현 방법은 해당 단어 그 자체만 보고, 특정값을 맵핑하여 단어를 표현하는 방법이며, 
분산 표현 방법은 그 단어를 표현하고자 주변을 참고하여 단어를 표현하는 방법입니다.

예를 들어 puppy(강아지), cute(귀여운), lovely(사랑스러운)라는 단어가 있을 때 각 단어에 1번, 2번, 3번 등과 같은 숫자를 맵핑(mapping)하여 부여한다면 이는 국소 표현 방법에 해당됩니다. 

반면, 분산 표현 방법의 예를 하나 들어보면 해당 단어를 표현하기 위해 주변 단어를 참고합니다. puppy(강아지)라는 단어 근처에는 주로 cute(귀여운), lovely(사랑스러운)이라는 단어가 자주 등장하므로, puppy라는 단어는 cute, lovely한 느낌이다로 단어를 정의합니다. 

이렇게 되면 이 두 방법의 차이는 국소 표현 방법은 단어의 의미, 뉘앙스를 표현할 수 없지만, 분산 표현 방법은 단어의 뉘앙스를 표현할 수 있게 됩니다.

또한 비슷한 의미로 국소 표현 방법(Local Representation)을 이산 표현(Discrete Representation)이라고도 하며 ==> 불연속적/딱딱 나뉘는 할당이므로, 
분산 표현(Distributed Representation)을 연속 표현(Continuous Represnetation)이라고도 합니다. ==> 연속적이고 나뉘지 않는 느낌/뉘앙스적 할당이므로.

추가 의견으로 구글의 연구원 토마스 미코로브(Tomas Mikolov)는 
2016년에 한 발표에서 잠재 의미 분석(LSA)이나 잠재 디리클레 할당(LDA)과 같은 방법들은 단어의 의미를 표현할 수 있다는 점에서 연속 표현(Continuous Represnetation)이지만, 
엄밀히 말해서 다른 접근의 방법론을 사용하고 있는 워드투벡터(Word2vec)와 같은 분산 표현(Distributed Representation)은 아닌 것으로 분류하여 << 연속 표현을 분산 표현을 포괄하고 있는 더 큰 개념으로 설명하기도 했습니다. >> 

## 2. 단어 표현의 카테고리화

이 책에서는 아래와 같은 기준으로 단어 표현을 카테고리화하여 작성되었습니다.

![](https://static.wikidocs.net/images/page/31767/wordrepresentation.PNG)

이번 챕터의 Bag of Words는 국소 표현에(Local Representation)에 속하며, 단어의 빈도수를 카운트(Count)하여 단어를 수치화하는 단어 표현 방법입니다. 

이 챕터에서는 BoW와 그의 확장인 DTM(또는 TDM)에 대해서 학습하고, 
이러한 << 빈도수 기반 단어 표현에 단어의 중요도에 따른 가중치를 줄 수 있는 TF-IDF >> 에 대해서 학습합니다.

워드 임베딩 챕터에서는 연속 표현(Continuous Representation)에 속하면서, 
예측(prediction)을 기반으로 단어의 뉘앙스를 표현하는 워드투벡터(Word2Vec)와 그의 확장인 패스트텍스트(FastText)를 학습하고, 예측과 카운트라는 두 가지 방법이 모두 사용된 글로브(GloVe)에 대해서 학습합니다.





# 11-02 백 오브 워즈(Bag of Words) 방법

## 1. Bag of Words란?

Bag of Words란 단어들의 << 순서는 전혀 고려하지 않고, >> => 맥락정보x , 국소표현.
단어들의 출현 빈도(frequency)에만 집중하는 텍스트 데이터의 수치화 표현 방법입니다.  ^88178d

Bag of Words를 직역하면 단어들의 가방이라는 의미입니다. 
단어들이 들어있는 가방을 상상해봅시다. 
갖고있는 어떤 텍스트 문서에 있는 단어들을 가방에다가 전부 넣습니다. 
그 후에는 이 가방을 흔들어 단어들을 섞습니다. 

만약, 해당 문서 내에서 특정 단어가 N번 등장했다면, 이 가방에는 그 특정 단어가 N개 있게됩니다. 또한 가방을 흔들어서 단어를 섞었기 때문에 더 이상 단어의 순서는 중요하지 않습니다.

BoW를 만드는 과정을 이렇게 두 가지 과정으로 생각해보겠습니다.

```scss
(1) 각 단어에 고유한 정수 인덱스를 부여합니다.  # 단어 집합 생성.
(2) 각 인덱스의 위치에 단어 토큰의 등장 횟수를 기록한 벡터를 만듭니다.  
```

한국어 예제를 통해서 BoW에 대해서 이해해보도록 하겠습니다.

**문서1 : 정부가 발표하는 물가상승률과 소비자가 느끼는 물가상승률은 다르다.**

문서1에 대해서 BoW를 만들어보겠습니다. 
아래의 함수는 입력된 문서에 대해서 
단어 집합(vocaburary)을 만들어 각 단어에 정수 인덱스를 할당하고, BoW를 만듭니다.

```python
from konlpy.tag import Okt

okt = Okt()

def build_bag_of_words(document):
  # 온점 제거 및 형태소 분석 (morphs 넣기전에 .을 빼서 노이즈 방지 필요.)
  document = document.replace('.', '')
  tokenized_document = okt.morphs(document)

  word_to_index = {}
  bow = []

  for word in tokenized_document:  
    if word not in word_to_index.keys():
      word_to_index[word] = len(word_to_index)  
      # BoW에 전부 기본값 1을 넣는다.
      bow.insert(len(word_to_index) - 1, 1)
    else:
      # 재등장하는 단어의 인덱스
      index = word_to_index.get(word)
      # 재등장한 단어는 해당하는 인덱스의 위치에 1을 더한다.
      bow[index] = bow[index] + 1

  return word_to_index, bow
```

해당 함수에 문서1을 입력으로 넣어봅시다.

```python
doc1 = "정부가 발표하는 물가상승률과 소비자가 느끼는 물가상승률은 다르다."
vocab, bow = build_bag_of_words(doc1)
print('vocabulary :', vocab)
print('bag of words vector :', bow)
```

```python
vocabulary : {'정부': 0, '가': 1, '발표': 2, '하는': 3, '물가상승률': 4, '과': 5, '소비자': 6, '느끼는': 7, '은': 8, '다르다': 9}
bag of words vector : [1, 2, 1, 1, 2, 1, 1, 1, 1, 1]
```

문서1에 각 단어에 대해서 인덱스를 부여한 결과는 첫번째 출력 결과입니다. 문서1의 BoW는 두번째 출력 결과입니다. 

두번째 출력 결과를 보면, 인덱스 4에 해당하는 물가상승률은 두 번 언급되었기 때문에 인덱스 4에 해당하는 값이 2입니다. 
인덱스는 0부터 시작됨에 주의합니다. 

다시 말해 물가상승률은 BoW에서 다섯번째 값입니다. 만약, 한국어에서 불용어에 해당되는 조사들 또한 제거한다면 더 정제된 BoW를 만들 수도 있습니다.


## 2. Bag of Words의 다른 예제들

**문서2 : 소비자는 주로 소비하는 상품을 기준으로 물가상승률을 느낀다.**

위의 함수에 임의의 문서2를 입력으로 하여 결과를 확인해봅시다.

```python
doc2 = '소비자는 주로 소비하는 상품을 기준으로 물가상승률을 느낀다.'

vocab, bow = build_bag_of_words(doc2)
print('vocabulary :', vocab)
print('bag of words vector :', bow)
```

```python
vocabulary : {'소비자': 0, '는': 1, '주로': 2, '소비': 3, '하는': 4, '상품': 5, '을': 6, '기준': 7, '으로': 8, '물가상승률': 9, '느낀다': 10}
bag of words vector : [1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1]
```

문서1과 문서2를 합쳐서 문서 3이라고 명명하고, BoW를 만들 수도 있습니다.

**문서3: 정부가 발표하는 물가상승률과 소비자가 느끼는 물가상승률은 다르다. 
소비자는 주로 소비하는 상품을 기준으로 물가상승률을 느낀다.**

```python
doc3 = doc1 + ' ' + doc2
vocab, bow = build_bag_of_words(doc3)
print('vocabulary :', vocab)
print('bag of words vector :', bow)
```

```python
vocabulary : {'정부': 0, '가': 1, '발표': 2, '하는': 3, '물가상승률': 4, '과': 5, '소비자': 6, '느끼는': 7, '은': 8, '다르다': 9, '는': 10, '주로': 11, '소비': 12, '상품': 13, '을': 14, '기준': 15, '으로': 16, '느낀다': 17}
bag of words vector : [1, 2, 1, 2, 3, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1]
```

문서3의 단어 집합은 문서1과 문서2의 단어들을 모두 포함하고 있는 것들을 볼 수 있습니다. 

BoW는 종종 여러 문서의 단어 집합을 합친 뒤에, 해당 단어 집합에 대한 각 문서의 BoW를 구하기도 합니다. 가령, 문서3에 대한 단어 집합을 기준으로 문서1, 문서2의 BoW를 만든다고 한다면 결과는 아래와 같습니다.

```python
문서3 단어 집합에 대한 문서1 BoW : [1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0]  
문서3 단어 집합에 대한 문서2 BoW : [0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 2, 1, 1, 1]  
```

문서3 단어 집합에서 물가상승률이라는 단어는 인덱스가 4에 해당됩니다. 물가상승률이라는 단어는 문서1에서는 2회 등장하며, 문서2에서는 1회 등장하였기 때문에 두 BoW의 인덱스 4의 값은 각각 2와 1이 되는 것을 볼 수 있습니다.

BoW는 각 단어가 등장한 << 횟수를 수치화하는 텍스트 표현 방법이므로 주로 어떤 단어가 얼마나 등장했는지를 기준으로 문서가 어떤 성격의 문서인지를 판단하는 작업에 쓰입 >> 니다. 

즉, 분류 문제나 여러 문서 간의 유사도를 구하는 문제에 주로 쓰입니다. 가령, '달리기', '체력', '근력'과 같은 단어가 자주 등장하면 해당 문서를 체육 관련 문서로 분류할 수 있을 것이며, '미분', '방정식', '부등식'과 같은 단어가 자주 등장한다면 수학 관련 문서로 분류할 수 있습니다.

## 3. CountVectorizer 클래스로 BoW 만들기

사이킷 런에서는 단어의 빈도를 Count하여 Vector로 만드는 CountVectorizer 클래스를 지원합니다. 

이를 이용하면 영어에 대해서는 손쉽게 BoW를 만들 수 있습니다. 
CountVectorizer로 간단하고 빠르게 BoW를 만드는 실습을 진행해보도록 하겠습니다.

```python
from sklearn.feature_extraction.text import CountVectorizer

corpus = ['you know I want your love. because I love you.']
vector = CountVectorizer()

# 코퍼스로부터 각 단어의 빈도수를 기록
print('bag of words vector :', vector.fit_transform(corpus).toarray()) 

# 각 단어의 인덱스가 어떻게 부여되었는지를 출력
print('vocabulary :',vector.vocabulary_)
```

```python
bag of words vector : [[1 1 2 1 2 1]]
vocabulary : {'you': 4, 'know': 1, 'want': 3, 'your': 5, 'love': 2, 'because': 0}
```

예제 문장에서 you와 love는 두 번씩 언급되었으므로 각각 인덱스 2와 인덱스 4에서 2의 값을 가지며, 그 외의 값에서는 1의 값을 가지는 것을 볼 수 있습니다. 

또한 알파벳 I는 BoW를 만드는 과정에서 사라졌는데, 이는 CountVectorizer가 기본적으로 길이가 2이상인 문자에 대해서만 토큰으로 인식하기 때문입니다. 
정제(Cleaning) 챕터에서 언급했듯이, 영어에서는 길이가 짧은 문자를 제거하는 것 또한 전처리 작업으로 고려되기도 합니다. ( 불용어 )

주의할 것은 CountVectorizer는 단지 << 띄어쓰기만을 기준으로 단어를 자르는 낮은 수준의 토큰화를 진행하고 BoW를 만든다 >> 는 점입니다. 

이는 << 영어의 경우 띄어쓰기만으로 토큰화가 수행되기 때문에 문제가 없지만 한국어에 CountVectorizer를 적용하면, 조사 등의 이유로 제대로 BoW가 만들어지지 않음 >> 을 의미합니다.

예를 들어, 앞서 BoW를 만드는데 사용했던 '정부가 발표하는 물가상승률과 소비자가 느끼는 물가상승률은 다르다.' 라는 문장을 CountVectorizer를 사용하여 BoW로 만들 경우, CountVectorizer는 '물가상승률'이라는 단어를 인식하지 못 합니다. CountVectorizer는 띄어쓰기를 기준으로 분리한 뒤에 '물가상승률과'와 '물가상승률은' 으로 조사를 포함해서 하나의 단어로 판단하기 때문에 서로 다른 두 단어로 인식합니다. 그리고 '물가상승률과'와 '물가상승률은'이 각자 다른 인덱스에서 1이라는 빈도의 값을 갖게 됩니다.


## 4. 불용어를 제거한 BoW 만들기

앞서 불용어는 자연어 처리에서 별로 의미를 갖지 않는 단어들이라고 언급한 바 있습니다. 
BoW를 사용한다는 것은 그 문서에서 각 단어가 얼마나 자주 등장했는지를 보겠다는 것입니다. 그리고 각 단어에 대한 빈도수를 수치화 하겠다는 것은 결국 텍스트 내에서 어떤 단어들이 중요한지를 보고싶다는 의미를 함축하고 있습니다. 

그렇다면 BoW를 만들때 불용어를 제거하는 일은 자연어 처리의 정확도를 높이기 위해서 선택할 수 있는 전처리 기법입니다.

영어의 BoW를 만들기 위해 사용하는 CountVectorizer는 불용어를 지정하면, 불용어는 제외하고 BoW를 만들 수 있도록 불용어 제거 기능을 지원하고 있습니다.

```python
from sklearn.feature_extraction.text import CountVectorizer
from nltk.corpus import stopwords
```

### 1) 사용자가 직접 정의한 불용어 사용

```python
text = ["Family is not an important thing. It's everything."]
vect = CountVectorizer(stop_words=["the", "a", "an", "is", "not"])
print('bag of words vector :',vect.fit_transform(text).toarray())
print('vocabulary :',vect.vocabulary_)
```

```python
bag of words vector : [[1 1 1 1 1]]
vocabulary : {'family': 1, 'important': 2, 'thing': 4, 'it': 3, 'everything': 0}
```

### 2) CountVectorizer에서 제공하는 자체 불용어 사용

```python
text = ["Family is not an important thing. It's everything."]
vect = CountVectorizer(stop_words="english")
print('bag of words vector :',vect.fit_transform(text).toarray())
print('vocabulary :',vect.vocabulary_)
```

```python
bag of words vector : [[1 1 1]]
vocabulary : {'family': 0, 'important': 1, 'thing': 2}
```

### 3) NLTK에서 지원하는 불용어 사용

```python
text = ["Family is not an important thing. It's everything."]
stop_words = stopwords.words("english")
vect = CountVectorizer(stop_words=stop_words)
print('bag of words vector :',vect.fit_transform(text).toarray()) 
print('vocabulary :',vect.vocabulary_)
```

```python
bag of words vector : [[1 1 1 1]]
vocabulary : {'family': 1, 'important': 2, 'thing': 3, 'everything': 0}
```



# 11-03 DTM과 TF-IDF 행렬

서로 다른 문서들의 BoW들을 결합한 표현 방법인 
문서 단어 행렬(Document-Term Matrix, DTM) 표현 방법을 배워보겠습니다. 

이하 DTM이라고 명명합니다. 행과 열을 반대로 선택하면 TDM이라고 부르기도 합니다. 이렇게 하면 서로 다른 문서들을 비교할 수 있게 됩니다.

## 1. 문서 단어 행렬(Document-Term Matrix, DTM)의 표기법

문서 단어 행렬(Document-Term Matrix, DTM)이란 다수의 문서에서 등장하는 각 단어들의 빈도를 행렬로 표현한 것을 말합니다. 쉽게 생각하면 각 문서에 대한 BoW를 하나의 행렬로 만든 것으로 생각할 수 있으며, BoW와 다른 표현 방법이 아니라 BoW 표현을 다수의 문서에 대해서 행렬로 표현하고 부르는 용어입니다. 예를 들어서 이렇게 4개의 문서가 있다고 합시다.

**문서1 : 먹고 싶은 사과  
문서2 : 먹고 싶은 바나나  
문서3 : 길고 노란 바나나 바나나  
문서4 : 저는 과일이 좋아요**

띄어쓰기 단위 토큰화를 수행한다고 가정하고, 문서 단어 행렬로 표현하면 다음과 같습니다.

|     | 과일이 | 길고  | 노란  | 먹고  | 바나나 | 사과  | 싶은  | 저는  | 좋아요 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 문서1 | 0   | 0   | 0   | 1   | 0   | 1   | 1   | 0   | 0   |
| 문서2 | 0   | 0   | 0   | 1   | 1   | 0   | 1   | 0   | 0   |
| 문서3 | 0   | 1   | 1   | 0   | 2   | 0   | 0   | 0   | 0   |
| 문서4 | 1   | 0   | 0   | 0   | 0   | 0   | 0   | 1   | 1   |

각 문서에서 등장한 단어의 빈도를 행렬의 값으로 표기합니다. 문서 단어 행렬은 문서들을 서로 비교할 수 있도록 수치화할 수 있다는 점에서 의의를 갖습니다. 

만약 필요에 따라서는 형태소 분석기로 단어 토큰화를 수행하고, 불용어에 해당되는 조사들 또한 제거하여 더 정제된 DTM을 만들 수도 있을 것입니다.


## 2. 문서 단어 행렬(Document-Term Matrix)의 한계

DTM은 매우 간단하고 구현하기도 쉽지만, 본질적으로 가지는 몇 가지 한계들이 있습니다.

### 1) 희소 표현(Sparse representation)

원-핫 벡터는 단어 집합의 크기가 벡터의 차원이 되고 대부분의 값이 0이 되는 벡터입니다. 
원-핫 벡터는 공간적 낭비와 계산 리소스를 증가시킬 수 있다는 점에서 단점을 가집니다. DTM도 마찬가지입니다. 

DTM에서의 각 행을 문서 벡터라고 해봅시다. 
각 문서 벡터의 차원은 원-핫 벡터와 마찬가지로 전체 단어 집합의 크기를 가집니다. 
만약 가지고 있는 전체 코퍼스가 방대한 데이터라면 문서 벡터의 차원은 수만 이상의 차원을 가질 수도 있습니다. 또한 많은 문서 벡터가 대부분의 값이 0을 가질 수도 있습니다. 
당장 위에서 예로 들었던 문서 단어 행렬의 모든 행이 0이 아닌 값보다 0의 값이 더 많은 것을 볼 수 있습니다.

원-핫 벡터나 DTM과 같은 대부분의 값이 0인 표현을 희소 벡터(sparse vector) 또는 희소 행렬(sparse matrix)라고 부르는데, << 희소 벡터는 많은 양의 저장 공간과 높은 계산 복잡도를 요구합니다.  >> (연산할 때 0을 곱하고 더하는 무의미한 연산이 많아지며 낭비비율이 커짐.)

이러한 이유로 전처리를 통해 단어 집합의 크기를 줄이는 일은 BoW 표현을 사용하는 모델에서 중요할 수 있습니다. 앞서 배운 텍스트 전처리 방법을 사용하여 구두점, 빈도수가 낮은 단어, 불용어를 제거하고, 어간이나 표제어 추출을 통해 단어를 정규화하여 단어 집합의 크기를 줄일 수 있습니다.

### 2) 단순 빈도 수 기반 접근

여러 문서에 등장하는 모든 단어에 대해서 빈도 표기를 하는 이런 방법은 때로는 한계를 가지기도 합니다. 예를 들어 영어에 대해서 DTM을 만들었을 때, << 불용어인 the는 어떤 문서이든 자주 등장할 수 밖에 없습니다. 그런데 유사한 문서인지 비교하고 싶은 문서1, 문서2, 문서3에서 동일하게 the가 빈도수가 높다고 해서 이 문서들이 유사한 문서라고 판단해서는 안 됩니다. >> 

각 문서에는 중요한 단어와 불필요한 단어들이 혼재되어 있습니다. 
앞서 불용어(stopwords)와 같은 단어들은 빈도수가 높더라도 자연어 처리에 있어 의미를 갖지 못하는 단어라고 언급한 바 있습니다. 

그렇다면 DTM에 불용어와 중요한 단어에 대해서 <<가중치>>를 줄 수 있는 방법은 없을까요? 

이러한 아이디어를 적용한 TF-IDF를 이어서 학습해봅시다. 
사이킷런의 CountVectorizer를 사용하여 DTM을 만드는 실습 또한 TF-IDF를 설명하면서 진행하겠습니다.

이번에는 DTM 내에 있는 각 << 단어에 대한 중요도를 계산할 수 있는 TF-IDF 가중치 >> 에 대해서 알아보겠습니다. 
TF-IDF를 사용하면, 기존의 DTM을 사용하는 것보다 보다 많은 정보를 고려하여 문서들을 비교할 수 있습니다. 

TF-IDF가 DTM보다 항상 좋은 성능을 보장하는 것은 아니지만, 
<< 많은 경우에서 DTM보다 더 좋은 성능 >> 을 얻을 수 있습니다.


## 3. TF-IDF (단어 빈도-역 문서 빈도, Term Frequency-Inverse Document Frequency)

TF-IDF(Term Frequency-Inverse Document Frequency)는 
단어의 빈도와 역 문서 빈도(문서의 빈도에 특정 식을 취함)를 사용하여 DTM 내의 각 단어들마다 중요한 정도를 가중치로 주는 방법입니다. 우선 DTM을 만든 후, TF-IDF 가중치를 부여합니다.

TF-IDF는 주로 << 문서의 유사도 >> 를 구하는 작업, 검색 시스템에서 << 검색 결과의 중요도를 정하는 작업, 문서 내에서 특정 단어의 중요도 >> 를 구하는 작업 등에 쓰일 수 있습니다.

(문서별 빈도, 문서간 빈도 등을 분석해서 내는 단어의 중요도 도출방식이라고 생각하면 됨.)

TF-IDF는 TF와 IDF를 곱한 값을 의미하는데 이를 식으로 표현해보겠습니다. 
문서를 d, 단어를 t, 문서의 총 개수를 n이라고 표현할 때 
TF, DF, IDF는 각각 다음과 같이 정의할 수 있습니다.

![[Pasted image 20260807201232.png|427]]


### 1) tf(d,t) : 특정 문서 d에서의 특정 단어 t의 등장 횟수.

생소한 글자때문에 어려워보일 수 있지만, 잘 생각해보면 TF는 이미 앞에서 구한 적이 있습니다. TF는 앞에서 배운 DTM의 예제에서 각 단어들이 가진 값들입니다. DTM이 각 문서에서의 각 단어의 등장 빈도를 나타내는 값이었기 때문입니다.

### 2) df(t) : 특정 단어 t가 등장한 문서의 수.

여기서 특정 단어가 각 문서, 또는 문서들에서 몇 번 등장했는지는 관심가지지 않으며 오직 특정 단어 t가 등장한 << 문서의 수 >> 에만 관심을 가집니다. 

앞서 배운 DTM에서 바나나는 문서2와 문서3에서 등장했습니다. 이 경우, 바나나의 df는 2입니다. 문서3에서 바나나가 두 번 등장했지만, 그것은 중요한 게 아닙니다. 심지어 바나나란 단어가 문서2에서 100번 등장했고, 문서3에서 200번 등장했다고 하더라도 바나나의 df는 2가 됩니다.

### 3) idf(t) : df(t)에 반비례하는 수.

$$idf(t) = log(\frac{n}{1+df(t)})$$

IDF라는 이름을 보고 DF의 역수가 아닐까 생각했다면, 
IDF는 DF의 역수를 취하고 싶은 것이 맞습니다. 

그런데 log와 분모에 1을 더해주는 식에 의아하실 수 있습니다. 
log를 사용하지 않았을 때, IDF를 DF의 역수(라는 식)로 사용한다면 총 << 문서의 수 n이 커질 수록, IDF의 값은 기하급수적으로 커지게 됩니다. 그렇기 때문에 log를 사용 >> 합니다. 
=> "결정 요인" 이 아니라 "경향성" 인자로 사용하기 위함.

(+ df는 그냥 끼어있는거고 이름대로 tf, idf 가 중요한거임.)
$$\text{TF-IDF} = \text{TF (문서 내 빈도)} \times \text{IDF (희귀도)}$$"이 문서에서 자주 나오면서(TF ↑), 다른 문서들에서는 잘 나오지 않는 희귀한 단어(IDF ↑)일수록 TF-IDF 점수가 높아져 해당 문서의 핵심 단어(키워드)로 도출된다."

---



왜 log가 필요한지 n=1,000,000일 때의 예를 들어봅시다. 
log의 밑은 10을 사용한다고 가정하였을 때 결과는 아래와 같습니다.

  
$idf(t) = log(n/df(t))$  
$n=1,000,000$


|단어 $t$|$df(t)$|$idf(d, t)$|
| ----- | --------- | --- |
| word1 | 1         | 6   |
| word2 | 100       | 4   |
| word3 | 1,000     | 3   |
| word4 | 10,000    | 2   |
| word5 | 100,000   | 1   |
| word6 | 1,000,000 | 0   |



그렇다면 log를 사용하지 않으면 idf의 값이 어떻게 커지는지 보겠습니다.

  
$idf(t) = n/df(t)$  
$n=1,000,000$

|단어 $t$|$df(t)$|$idf(d, t)$|
| ----- | --------- | --------- |
| word1 | 1         | 1,000,000 |
| word2 | 100       | 10,000    |
| word3 | 1,000     | 1,000     |
| word4 | 10,000    | 100       |
| word5 | 100,000   | 10        |
| word6 | 1,000,000 | 1         |


또 다른 직관적인 설명은 << 불용어 등과 같이 자주 쓰이는 단어들은 비교적 자주 쓰이지 않는 단어들보다 최소 수십 배 자주 등장 >> 합니다. 


그런데 비교적 자주 쓰이지 않는 단어들조차 희귀 단어들과 비교하면 또 최소 수백 배는 더 자주 등장하는 편입니다. 

이 때문에 log를 씌워주지 않으면, << 희귀 단어들에 엄청난 가중치가 부여될 수 있습니다 >> ( 약간 lr 조정이랑 비슷한 느낌으로 log 씌워서 보정해주는 느낌. 영향력을 적정수준으로 맞추기 위한 것임. ) 로그를 씌우면 이런 격차를 줄이는 효과가 있습니다. 

log 안의 식에서 분모에 1을 더해주는 이유는 

첫번째 이유로는 특정 단어가 전체 문서에서 등장하지 않을 경우에 분모가 0이 되는 상황을 방지하기 위함입니다. (0으로 나누기 오류 방지)

TF-IDF는 <<< 모든 문서에서 자주 등장하는 단어는 중요도가 낮다고 판단하며, 특정 문서에서만 자주 등장하는 단어는 중요도가 높다 >>> 고 판단합니다. 

TF-IDF 값이 낮으면 중요도가 낮은 것이며, TF-IDF 값이 크면 중요도가 큰 것입니다. 즉, the나 a와 같이 불용어의 경우에는 모든 문서에 자주 등장하기 마련이기 때문에 자연스럽게 불용어의 TF-IDF의 값은 다른 단어의 TF-IDF에 비해서 낮아지게 됩니다.

|     | 과일이 | 길고  | 노란  | 먹고  | 바나나 | 사과  | 싶은  | 저는  | 좋아요 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 문서1 | 0   | 0   | 0   | 1   | 0   | 1   | 1   | 0   | 0   |
| 문서2 | 0   | 0   | 0   | 1   | 1   | 0   | 1   | 0   | 0   |
| 문서3 | 0   | 1   | 1   | 0   | 2   | 0   | 0   | 0   | 0   |
| 문서4 | 1   | 0   | 0   | 0   | 0   | 0   | 0   | 1   | 1   |

앞서 DTM을 설명하기위해 들었던 위의 예제를 가지고 TF-IDF에 대해 이해해보겠습니다. 
우선 TF는 앞서 사용한 DTM을 그대로 사용하면, 그것이 각 문서에서의 각 단어의 TF가 됩니다. 

이제 구해야할 것은 TF와 곱해야할 값인 IDF입니다. 
로그는 자연 로그를 사용하도록 하겠습니다. 자연 로그는 로그의 밑을 자연 상수 e(e=2.718281...)를 사용하는 로그를 말합니다. 

IDF 계산을 위해 사용하는 로그의 << 밑은 TF-IDF를 사용하는 사용자가 임의로 정할 수 있는데, 여기서 로그는 마치 기존의 값에 곱하여 값의 크기를 조절하는 상수의 역할 >> 을 합니다. 

각종 프로그래밍 언어에서 패키지로 지원하는 TF-IDF의 로그는 대부분 자연 로그를 사용합니다. 여기서도 자연 로그를 사용하겠습니다. 자연 로그는 보통 log라고 표현하지 않고, ln이라고 표현합니다.

|단어|IDF(역 문서 빈도)|
|---|---|
|과일이|ln(4/(1+1)) = 0.693147|
|길고|ln(4/(1+1)) = 0.693147|
|노란|ln(4/(1+1)) = 0.693147|
|먹고|ln(4/(2+1)) = 0.287682|
|바나나|ln(4/(2+1)) = 0.287682|
|사과|ln(4/(1+1)) = 0.693147|
|싶은|ln(4/(2+1)) = 0.287682|
|저는|ln(4/(1+1)) = 0.693147|
|좋아요|ln(4/(1+1)) = 0.693147|

문서의 총 수는 4이기 때문에 ln 안에서 분자는 늘 4으로 동일합니다. 
분모의 경우에는 각 단어가 등장한 문서의 수(DF)를 의미하는데, 예를 들어서 '먹고'의 경우에는 총 2개의 문서(문서1, 문서2)에 등장했기 때문에 2라는 값을 가집니다. 

각 단어에 대해서 IDF의 값을 비교해보면 문서 1개에만 등장한 단어와 문서2개에만 등장한 단어는 값의 차이를 보입니다. IDF는 여러 문서에서 등장한 단어의 가중치를 낮추는 역할을 하기 때문입니다.

TF-IDF를 계산해보겠습니다. 각 단어의 TF는 DTM에서의 각 단어의 값과 같으므로, 앞서 사용한 DTM에서 단어 별로 위의 IDF값을 곱해주면 TF-IDF 값을 얻습니다.

| |과일이|길고|노란|먹고|바나나|사과|싶은|저는|좋아요|
|---|---|---|---|---|---|---|---|---|---|
|문서1|0|0|0|0.287682|0|0.693147|0.287682|0|0|
|문서2|0|0|0|0.287682|0.287682|0|0.287682|0|0|
|문서3|0|0.693147|0.693147|0|0.575364|0|0|0|0|
|문서4|0.693147|0|0|0|0|0|0|0.693147|0.693147|

사실 예제 문서가 굉장히 간단하기 때문에 계산은 매우 쉽습니다. 

문서3에서의 바나나만 TF 값이 2이므로 IDF에 2를 곱해주고, 
나머진 TF 값이 1이므로 그대로 IDF 값을 가져오면 됩니다. 

문서2에서의 바나나의 TF-IDF 가중치와 문서3에서의 바나나의 TF-IDF 가중치가 다른 것을 볼 수 있습니다. 
수식적으로 말하면, TF가 각각 1과 2로 달랐기 때문인데 TF-IDF에서의 관점에서 보자면 TF-IDF는 << 특정 문서에서 자주 등장하는 단어는 그 문서 내에서 중요한 단어로 판단 >> 하기 때문입니다. 

문서2에서는 바나나를 한 번 언급했지만, 문서3에서는 바나나를 두 번 언급했기 때문에 
문서3에서의 바나나를 더욱 중요한 단어라고 판단하는 것입니다.



## 4. 파이썬으로 TF-IDF 직접 구현하기

위의 계산 과정을 파이썬으로 직접 구현해보겠습니다. 
앞의 설명에서 사용한 4개의 문서를 docs에 저장합니다.

```python
import pandas as pd # 데이터프레임 사용을 위해
from math import log # IDF 계산을 위해

docs = [
  '먹고 싶은 사과',
  '먹고 싶은 바나나',
  '길고 노란 바나나 바나나',
  '저는 과일이 좋아요'
] 
vocab = list(set(w for doc in docs for w in doc.split()))
vocab.sort()
```

TF, IDF, 그리고 TF-IDF 값을 구하는 함수를 구현합니다.

```python
# 총 문서의 수
N = len(docs) 

def tf(t, d):
  return d.count(t)

def idf(t):
  df = 0
  for doc in docs:
    df += t in doc
  return log(N/(df+1))

def tfidf(t, d):
  return tf(t,d)* idf(t)
```

TF를 구해보겠습니다. 다시 말해 DTM을 데이터프레임에 저장하여 출력해보겠습니다.

```python
result = []

# 각 문서에 대해서 아래 연산을 반복
for i in range(N):
  result.append([])
  d = docs[i]
  for j in range(len(vocab)):
    t = vocab[j]
    result[-1].append(tf(t, d))

tf_ = pd.DataFrame(result, columns = vocab)

```

![](https://static.wikidocs.net/images/page/31698/tf_.PNG)

정상적으로 DTM이 출력되었습니다. 각 단어에 대한 IDF 값을 구해봅시다.

```python
result = []
for j in range(len(vocab)):
    t = vocab[j]
    result.append(idf(t))

idf_ = pd.DataFrame(result, index=vocab, columns=["IDF"])
idf_
```

![](https://static.wikidocs.net/images/page/31698/idf_.PNG)

위에서 수기로 구한 IDF 값들과 정확히 일치합니다. TF-IDF 행렬을 출력해봅시다.

```python
result = []
for i in range(N):
  result.append([])
  d = docs[i]
  for j in range(len(vocab)):
    t = vocab[j]
    result[-1].append(tfidf(t,d))

tfidf_ = pd.DataFrame(result, columns = vocab)
tfidf_
```

![](https://static.wikidocs.net/images/page/31698/tfidf_.PNG)

TF-IDF의 가장 기본적인 식에 대해서 학습하고 실제로 구현하는 실습을 진행해보았습니다. 

사실 실제 TF-IDF 구현을 제공하고 있는 많은 머신 러닝 패키지들은 패키지마다 식이 조금씩 상이하지만, 위에서 배운 식과는 다른 조정된 식을 사용합니다. 
그 이유는 위의 기본적인 식을 바탕으로 한 구현에는 몇 가지 문제점이 존재하기 때문입니다. 

만약 전체 문서의 수 $n$이 4인데, $df(t)$의 값이 3인 경우에는 어떤 일이 벌어질까요? 
$df(t)$에 1이 더해지면서 log항의 분자와 분모의 값이 같아지게 됩니다. 
이는 $log$의 진수값이 1이 되면서 $idf(d, t)$의 값이 0이 됨을 의미합니다. 
식으로 표현하면 $idf(d, t) = log(n/(df(t)+1)) = 0$입니다. 

IDF의 값이 0이라면 더 이상 가중치의 역할을 수행하지 못합니다. 
아래에서 실습할 사이킷런의 TF-IDF 구현체 또한 위의 식에서 조정된 식을 사용하고 있습니다.

## 5. 사이킷런을 이용한 DTM과 TF-IDF 실습

사이킷런을 통해 DTM과 TF-IDF를 만들어보겠습니다. 
BoW를 설명하며 배운 CountVectorizer를 사용하면 DTM을 만들 수 있습니다.

```python
from sklearn.feature_extraction.text import CountVectorizer

corpus = [
    'you know I want your love',
    'I like you',
    'what should I do ',    
]

vector = CountVectorizer()

# 코퍼스로부터 각 단어의 빈도수를 기록
print(vector.fit_transform(corpus).toarray())

# 각 단어와 맵핑된 인덱스 출력
print(vector.vocabulary_)
```

```python
[[0 1 0 1 0 1 0 1 1]
 [0 0 1 0 0 0 0 1 0]
 [1 0 0 0 1 0 1 0 0]]
{'you': 7, 'know': 1, 'want': 5, 'your': 8, 'love': 3, 'like': 2, 'what': 6, 'should': 4, 'do': 0}
```

DTM이 완성되었습니다. 
DTM에서 각 단어의 인덱스가 어떻게 부여되었는지를 확인하기 위해, 인덱스를 확인해보았습니다. 첫번째 열의 경우에는 0의 인덱스를 가진 do입니다. 

do는 세번째 문서에만 등장했기 때문에, 세번째 행에서만 1의 값을 가집니다. 두번째 열의 경우에는 1의 인덱스를 가진 know입니다. know는 첫번째 문서에만 등장했으므로 첫번째 행에서만 1의 값을 가집니다.

사이킷런은 << TF-IDF를 자동 계산해주는 TfidfVectorizer를 제공 >> 합니다. 사이킷런의 TF-IDF는 위에서 배웠던 보편적인 TF-IDF 기본 식에서 조정된 식을 사용합니다. 
요약하자면, IDF의 로그항의 분자에 1을 더해주며, 로그항에 1을 더해주고, TF-IDF에 L2 정규화라는 방법으로 값을 조정하는 등의 차이로 TF-IDF가 가진 의도는 여전히 그대로 갖고 있습니다.

```python
from sklearn.feature_extraction.text import TfidfVectorizer

corpus = [
    'you know I want your love',
    'I like you',
    'what should I do ',    
]

tfidfv = TfidfVectorizer().fit(corpus)
print(tfidfv.transform(corpus).toarray())
print(tfidfv.vocabulary_)
```

```python
[[0.         0.46735098 0.         0.46735098 0.         0.46735098 0.         0.35543247 0.46735098]
 [0.         0.         0.79596054 0.         0.         0.         0.         0.60534851 0.        ]
 [0.57735027 0.         0.         0.         0.57735027 0.         0.57735027 0.         0.        ]]
{'you': 7, 'know': 1, 'want': 5, 'your': 8, 'love': 3, 'like': 2, 'what': 6, 'should': 4, 'do': 0}
```

BoW, DTM, TF-IDF에 대해서 전부 학습했습니다. 
문서들 간의 유사도를 구하기 위한 재료 손질하는 방법을 배운 셈입니다. 
다음 챕터에서 유사도를 구하는 방법과 이를 이용한 실습을 진행해보겠습니다.

사이킷런의 TF-IDF의 수식을 이해하고 싶은 분들을 위해서 
위키독스 웹 사이트에 댓글로 설명해놨습니다. 궁금하신 분들은 참고하세요.



# 11-04 코사인 유사도를 이용한 추천 시스템

BoW에 기반한 단어 표현 방법인 DTM, TF-IDF, 
또는 뒤에서 배우게 될 Word2Vec 등과 같이 << 단어를 수치화 >> 할 수 있는 방법을 이해했다면 
이러한 표현 방법에 대해서 코사인 유사도를 이용하여 문서의 유사도를 구하는 게 가능합니다.

## 1. 코사인 유사도(Cosine Similarity)

코사인 유사도는 두 벡터 간의 코사인 각도를 이용하여 구할 수 있는 두 벡터의 유사도를 의미합니다. 
두 벡터의 방향이 << 완전히 동일한 경우에는 1의 값을 가지며, 90°의 각을 이루면 0, 180°로 반대의 방향을 가지면 -1의 값을 갖게 됩니다.>> 
즉, 결국 코사인 유사도는 -1 이상 1 이하의 값을 가지며 값이 1에 가까울수록 유사도가 높다고 판단할 수 있습니다. 이를 직관적으로 이해하면 << 두 벡터가 가리키는 방향이 얼마나 유사한가를 의미 >> 합니다.

![](https://static.wikidocs.net/images/page/24603/%EC%BD%94%EC%82%AC%EC%9D%B8%EC%9C%A0%EC%82%AC%EB%8F%84.PNG)

두 벡터 A, B에 대해서 코사인 유사도는 식으로 표현하면 다음과 같습니다.

$$similarity=cos(Θ)=\frac{A⋅B}{||A||\ ||B||}=\frac{\sum_{i=1}^{n}{A_{i}×B_{i}}}{\sqrt{\sum_{i=1}^{n}(A_{i})^2}×\sqrt{\sum_{i=1}^{n}(B_{i})^2}}$$

문서 단어 행렬이나 TF-IDF 행렬을 통해서 문서의 유사도를 구하는 경우에는 
문서 단어 행렬이나 TF-IDF 행렬이 각각의 특징 벡터 A, B가 됩니다. 
예시를 통해 문서 단어 행렬에 대해서 코사인 유사도를 구해봅시다.

문서1 : 저는 사과 좋아요  
문서2 : 저는 바나나 좋아요  
문서3 : 저는 바나나 좋아요 저는 바나나 좋아요

뛰어쓰기 기준 토큰화를 진행했다고 가정하고, 위의 세 문서에 대해서 문서 단어 행렬을 만들면 이와 같습니다.

| 바나나 | 사과  | 저는  | 좋아요 |     |
| --- | --- | --- | --- | --- |
| 문서1 | 0   | 1   | 1   | 1   |
| 문서2 | 1   | 0   | 1   | 1   |
| 문서3 | 2   | 0   | 2   | 2   |

Numpy를 사용해서 코사인 유사도를 계산하는 함수를 구현하고 
각 문서 벡터 간의 코사인 유사도를 계산해보겠습니다.

```python
import numpy as np
from numpy import dot
from numpy.linalg import norm

def cos_sim(A, B):
  return dot(A, B)/(norm(A)*norm(B))

doc1 = np.array([0,1,1,1])
doc2 = np.array([1,0,1,1])
doc3 = np.array([2,0,2,2])

print('문서 1과 문서2의 유사도 :',cos_sim(doc1, doc2))
print('문서 1과 문서3의 유사도 :',cos_sim(doc1, doc3))
print('문서 2와 문서3의 유사도 :',cos_sim(doc2, doc3))
```

```python
문서 1과 문서2의 유사도 : 0.67
문서 1과 문서3의 유사도 : 0.67
문서 2과 문서3의 유사도 : 1.00
```

문서1 : 저는 사과 좋아요  
문서2 : 저는 바나나 좋아요  
문서3 : 저는 바나나 좋아요 저는 바나나 좋아요


눈여겨볼만한 점은 문서1과 문서2의 코사인 유사도와 문서1과 문서3의 코사인 유사도가 같다는 점과 문서2와 문서3의 코사인 유사도가 1이 나온다는 것입니다. 

앞서 1은 두 벡터의 <<방향>>이 완전히 동일한 경우에 1이 나오며, 
코사인 유사도 관점에서는 유사도의 값이 최대임을 의미한다고 언급한 바 있습니다.

문서3은 문서2에서 단지 모든 단어의 빈도수가 1씩 증가했을 뿐입니다. 
다시 말해 한 문서 내의 모든 단어의 빈도수가 동일하게 증가하는 경우에는 기존의 문서와 코사인 유사도의 값이 1이라는 것입니다. 이것이 시사하는 점은 무엇일까요? 예를 들어보겠습니다. 

문서 A와 B가 동일한 주제의 문서. 문서 C는 다른 주제의 문서라고 해봅시다. 
그리고 문서 A와 문서 C의 문서의 길이는 거의 차이가 나지 않지만, 문서 B의 경우 문서 A의 길이보다 두 배의 길이를 가진다고 가정하겠습니다. 

이런 경우 유클리드 거리로 유사도를 연산하면 문서 A가 문서 B보다 문서 C와 유사도가 더 높게 나오는 상황이 발생할 수 있습니다. 이는 유사도 연산에 문서의 길이가 영향을 받았기 때문인데, 이런 경우 코사인 유사도가 해결책이 될 수 있습니다. 
코사인 유사도는 유사도를 구할 때 벡터의 방향(패턴)에 초점을 두므로 코사인 유사도는 문서의 길이가 다른 상황에서 비교적 공정한 비교를 할 수 있도록 도와줍니다.
(2번 동일문장 반복이면 2배 크기의 벡터가 되는 것.)



## 2. 유사도를 이용한 추천 시스템 구현하기

캐글에서 사용되었던 영화 데이터셋을 가지고 영화 추천 시스템을 만들어보겠습니다. TF-IDF와 코사인 유사도만으로 영화의 줄거리에 기반해서 영화를 추천하는 추천 시스템을 만들 수 있습니다.

다운로드 링크 : https://www.kaggle.com/rounakbanik/the-movies-dataset

원본 파일은 위 링크에서 movies_metadata.csv 파일을 다운로드 받으면 됩니다. 
해당 데이터는 총 24개의 열을 가진 45,466개의 샘플로 구성된 영화 정보 데이터입니다.

```python
import pandas as pd
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.metrics.pairwise import cosine_similarity

data = pd.read_csv('movies_metadata.csv', low_memory=False)
data.head(2)
```

다운로드 받은 훈련 데이터에서 상위 2개의 샘플만 출력하여 데이터의 형식을 확인합니다.

| |...|original_title|overview|...|title|video|vote_average|vote_count|
|---|---|---|---|---|---|---|---|---|
|0|...|Toy Story|Led by Woody, Andy's toys live happily in his ... 중략 ...|...|Toy Story|False|7.7|5415.0|
|1|...|Jumanji|When siblings Judy and Peter discover an encha ... 중략 ...|...|Jumanji|False|6.9|2413.0|

훈련 데이터는 총 24개의 열을 갖고있으나 책의 지면의 한계로 일부 생략합니다. 
여기서 코사인 유사도에 사용할 데이터는 
영화 제목에 해당하는 title 열과 줄거리에 해당하는 overview 열입니다. 

좋아하는 영화를 입력하면, 
해당 영화의 줄거리와 유사한 줄거리의 영화를 찾아서 추천하는 시스템을 만들 것입니다.

```python
# 상위 2만개의 샘플을 data에 저장
data = data.head(20000)
```

만약 훈련 데이터의 양을 줄이고 학습을 진행하고자 한다면 위와 같이 데이터를 줄여서 재저장할 수 있습니다.  여기서는 상위 20,000개의 샘플만 사용하겠습니다. 

TF-IDF를 연산할 때 데이터에 Null 값이 들어있으면 에러가 발생합니다. 
TF-IDF의 대상이 되는 data의 overview 열에 결측값에 해당하는 Null 값이 있는지 확인합니다.

```python
# overview 열에 존재하는 모든 결측값을 전부 카운트하여 출력
print('overview 열의 결측값의 수:',data['overview'].isnull().sum())
```

```python
overview 열의 결측값의 수: 135
```

135개의 Null 값이 있다고 합니다. 

이 경우 결측값을 가진 행을 제거하는 pandas의 dropna()나 결측값이 있던 행에 특정값으로 채워넣는 pandas의 fillna()를 사용할 수 있습니다. 
괄호 안에 Null 대신 넣고자하는 값을 넣으면 되는데, 여기서는 빈 값(empty value)으로 대체하였습니다.

```python
# 결측값을 빈 값으로 대체
data['overview'] = data['overview'].fillna('')
```

Null 값을 빈 값으로 대체하였습니다. 


overview열에 대해서 TF-IDF 행렬을 구한 후 행렬의 크기를 출력해봅시다.

```python
tfidf = TfidfVectorizer(stop_words='english')
tfidf_matrix = tfidf.fit_transform(data['overview'])
print('TF-IDF 행렬의 크기(shape) :',tfidf_matrix.shape)
```

```python
TF-IDF 행렬의 크기(shape) : (20000, 47487)
```

TF-IDF 행렬의 크기는 20,000의 행을 가지고 47,847의 열을 가지는 행렬입니다. 
다시 말해 20,000개의 영화를 표현하기 위해서 총 47,487개의 단어가 사용되었음을 의미합니다. 

또는 47,847차원의 문서 벡터가 20,000개가 존재한다고도 표현할 수 있을 겁니다. 
이제 20,000개의 문서 벡터에 대해서 상호 간의 코사인 유사도를 구합니다.

```python
cosine_sim = cosine_similarity(tfidf_matrix, tfidf_matrix)
print('코사인 유사도 연산 결과 :',cosine_sim.shape)
```

```python
코사인 유사도 연산 결과 : (20000, 20000)
```

코사인 유사도 연산 결과로는 20,000행 20,000열의 행렬을 얻습니다. 
이는 20,000개의 각 문서 벡터(영화 줄거리 벡터)와 자기 자신을 포함한 20,000개의 문서 벡터 간의 유사도가 기록된 행렬입니다. 모든 20,000개 영화의 상호 유사도가 기록되어져 있습니다. 
-> 자기자신과 유사도연산하면 당연히 각 문장별의 유사도가 20000 20000 2d 표형태의 텐서로 나오게됨. ( from sklearn.metrics.pairwise import cosine_similarity => cosine_sim은 사이킷런이니까 numpy ndarray반환함. )

이제 기존 데이터프레임으로부터 영화의 타이틀을 key, 영화의 인덱스를 value로 하는 
딕셔너리 title_to_index를 만들어둡니다.

```python
title_to_index = dict(zip(data['title'], data.index))

# 영화 제목 Father of the Bride Part II의 인덱스를 리턴
idx = title_to_index['Father of the Bride Part II']
print(idx)
```

```python
4
```

선택한 영화의 제목을 입력하면 
코사인 유사도를 통해 << 가장 overview가 유사 >> 한 10개의 영화를 찾아내는 함수를 만듭니다.
(enumerate : [[07 Recurrent Neural Network#^d3b476]])

```python
def get_recommendations(title, cosine_sim=cosine_sim):
    # 선택한 영화의 타이틀로부터 해당 영화의 인덱스를 받아온다.
    idx = title_to_index[title]

    # 해당 영화와 모든 영화와의 유사도를 가져온다.
    sim_scores = list(enumerate(cosine_sim[idx]))

    # 유사도에 따라 영화들을 정렬한다.
    sim_scores = sorted(sim_scores, key=lambda x: x[1], reverse=True)

    # 가장 유사한 10개의 영화를 받아온다.
    sim_scores = sim_scores[1:11]

    # 가장 유사한 10개의 영화의 인덱스를 얻는다.
    movie_indices = [idx[0] for idx in sim_scores]

    # 가장 유사한 10개의 영화의 제목을 리턴한다.
    return data['title'].iloc[movie_indices]
```

sorted (데이터,  key=정렬 기준 함수,  reverse=정렬 순서)
`[(0, 1.0), (2, 0.81), (1, 0.35), (3, 0.05)]` 처럼 내림차순 정렬이 됨.


영화 다크 나이트 라이즈와 overview가 유사한 영화들을 찾아보겠습니다.

```python
get_recommendations('The Dark Knight Rises')
```

```python
12481                            The Dark Knight
150                               Batman Forever
1328                              Batman Returns
15511                 Batman: Under the Red Hood
585                                       Batman
9230          Batman Beyond: Return of the Joker
18035                           Batman: Year One
19792    Batman: The Dark Knight Returns, Part 1
3095                Batman: Mask of the Phantasm
10122                              Batman Begins
Name: title, dtype: object
```

가장 유사한 영화가 출력되는데, 영화 다크 나이트가 첫번째고, 
그 외에도 전부 배트맨 영화를 찾아낸 것을 확인할 수 있습니다.




# 11-05 단어와 문서의 유사도를 구하는 다양한 방법

문서의 유사도를 구하기 위한 방법으로는 
코사인 유사도 외에도 여러가지 방법들이 있습니다. 
여기서는 문서의 유사도를 구할 수 있는 다른 방법들을 학습합니다.

## 1. 유클리드 거리(Euclidean distance)

유클리드 거리(euclidean distance)는 문서의 유사도를 구할 때 
자카드 유사도나 코사인 유사도만큼, 유용한 방법은 아닙니다. 

하지만 여러 가지 방법을 이해하고, 시도해보는 것 자체만으로 
다른 개념들을 이해할 때 도움이 되므로 의미가 있습니다.

다차원 공간에서 두개의 점 $p$와 $q$가 각각 $p=(p_{1}, p_{2}, p_{3}, ... , p_{n})$과 $q=(q_{1}, q_{2}, q_{3}, ..., q_{n})$의 좌표를 가질 때 두 점 사이의 거리를 계산하는 유클리드 거리 공식은 다음과 같습니다.

$$\sqrt{(q_{1}-p_{1})^{2}+(q_{2}-p_{2})^{2}+\ ...\ +(q_{n}-p_{n})^{2}}=\sqrt{\sum_{i=1}^{n}(q_{i}-p_{i})^{2}}$$

다차원 공간이라고 가정하면, 처음 보는 입장에서는 식이 너무 복잡해보입니다. 

좀 더 쉽게 이해하기위해서 2차원 공간이라고 가정하고 두 점 사이의 거리를 좌표 평면 상에서 시각화해보겠습니다.

![[Pasted image 20260810210132.png]]

2차원 좌표 평면 상에서 두 점 $p$와 $q$사이의 << 직선 거리 >> 를 구하는 문제입니다. 
위의 경우에는 직각 삼각형으로 표현이 가능하므로, 중학교 수학 과정인 피타고라스의 정리를 통해 $p$와 $q$ 사이의 거리를 계산할 수 있습니다. 

즉, 2차원 좌표 평면에서 두 점 사이의 유클리드 거리 공식은 피타고라스의 정리를 통해 두 점 사이의 거리를 구하는 것과 동일합니다.

다시 원점으로 돌아가서 여러 문서에 대해서 유사도를 구하고자 유클리드 거리 공식을 사용한다는 것은, 앞서 본 << 2차원을 단어의 총 개수만큼의 차원으로 확장하는 것과 같습니다. >> 
예를 들어 아래와 같은 DTM이 있다고 합시다.

| |바나나|사과|저는|좋아요|
|---|---|---|---|---|
|문서1|2|3|0|1|
|문서2|1|2|3|1|
|문서3|2|1|2|2|

단어의 개수가 4개이므로, 이는 4차원 공간에 문서1, 문서2, 문서3을 배치하는 것과 같습니다. 
이때 다음과 같은 문서Q에 대해서 문서1, 문서2, 문서3 중 가장 유사한 문서를 찾아내고자 합니다.

| |바나나|사과|저는|좋아요|
|---|---|---|---|---|
|문서Q|1|1|0|1|

이때 유클리드 거리를 통해 유사도를 구하려고 한다면, 문서Q 또한 다른 문서들처럼 4차원 공간에 배치시켰다는 관점에서 4차원 공간에서의 각각의 문서들과의 유클리드 거리를 구하면 됩니다. 이를 파이썬 코드로 구현해보겠습니다.

```python
import numpy as np

def dist(x,y):   
    return np.sqrt(np.sum((x-y)**2))

doc1 = np.array((2,3,0,1))
doc2 = np.array((1,2,3,1))
doc3 = np.array((2,1,2,2))
docQ = np.array((1,1,0,1))

print('문서1과 문서Q의 거리 :',dist(doc1,docQ))
print('문서2과 문서Q의 거리 :',dist(doc2,docQ))
print('문서3과 문서Q의 거리 :',dist(doc3,docQ))
```

```python
문서1과 문서Q의 거리 : 2.23606797749979
문서2과 문서Q의 거리 : 3.1622776601683795
문서3과 문서Q의 거리 : 2.449489742783178
```

유클리드 거리의 값이 가장 작다는 것은 문서 간 거리가 가장 가깝다는 것을 의미합니다. 
즉, 문서1이 문서Q와 가장 유사하다고 볼 수 있습니다.



## 2. 자카드 유사도(Jaccard similarity)

A와 B 두개의 집합이 있다고 합시다. 
이때 교집합은 두 개의 집합에서 공통으로 가지고 있는 원소들의 집합을 말합니다. 
즉, << 합집합에서 교집합의 비율을 구한다면 두 집합 A와 B의 유사도를 구할 수 있다 >> 는 것이 자카드 유사도(jaccard similarity)의 아이디어입니다. 

자카드 유사도는 0과 1사이의 값을 가지며, 
만약 두 집합이 동일하다면 1의 값을 가지고, 두 집합의 공통 원소가 없다면 0의 값을 갖습니다.
자카드 유사도를 구하는 함수를 $J$라고 하였을 때, 자카드 유사도 함수 $J$는 아래와 같습니다.

$$J(A,B)=\frac{|A∩B|}{|A∪B|}=\frac{|A∩B|}{|A|+|B|-|A∩B|}$$

두 개의 비교할 문서를 각각 $doc_{1}$, $doc_{2}$라고 했을 때 $doc_{1}$과 $doc_{2}$의 문서의 유사도를 구하기 위한 자카드 유사도는 이와 같습니다.

$$J(doc_{1},doc_{2})=\frac{doc_{1}∩doc_{2}}{doc_{1}∪doc_{2}}$$

두 문서 $doc_{1}$, $doc_{2}$ 사이의 자카드 유사도 $J(doc_{1},doc_{2})$는 두 집합의 교집합 크기를 두 집합의 합집합 크기로 나눈 값으로 정의됩니다. 간단한 예를 통해서 이해해보겠습니다.

```python
doc1 = "apple banana everyone like likey watch card holder"
doc2 = "apple banana coupon passport love you"

# 토큰화
tokenized_doc1 = doc1.split()
tokenized_doc2 = doc2.split()

print('문서1 :',tokenized_doc1)
print('문서2 :',tokenized_doc2)
```

```python
문서1 : ['apple', 'banana', 'everyone', 'like', 'likey', 'watch', 'card', 'holder']
문서2 : ['apple', 'banana', 'coupon', 'passport', 'love', 'you']
```

문서1과 문서2의 합집합을 구해보겠습니다.

```python
union = set(tokenized_doc1).union(set(tokenized_doc2))
print('문서1과 문서2의 합집합 :',union)
```

```python
문서1과 문서2의 합집합 : {'you', 'passport', 'watch', 'card', 'love', 'everyone', 'apple', 'likey', 'like', 'banana', 'holder', 'coupon'}
```

문서1과 문서2의 합집합의 단어의 총 개수는 12개입니다. 
이제 문서1과 문서2의 교집합을 구해보겠습니다. 

문서1과 문서2에서 둘 다 등장한 단어를 찾으면 됩니다.

```python
intersection = set(tokenized_doc1).intersection(set(tokenized_doc2))
print('문서1과 문서2의 교집합 :',intersection)
```

```python
문서1과 문서2의 교집합 : {'apple', 'banana'}
```

문서1과 문서2에서 둘 다 등장한 단어는 banana와 apple 총 2개입니다. 
이제 교집합의 크기를 합집합의 크기로 나누면 자카드 유사도가 계산됩니다.

```python
print('자카드 유사도 :',len(intersection)/len(union))
```

```python
자카드 유사도 : 0.16666666666666666
```