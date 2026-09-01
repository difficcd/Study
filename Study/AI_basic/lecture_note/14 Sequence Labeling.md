> 이번 챕터에서는 다대다 RNN을 이용한 시퀀스 레이블링(Sequence Labeling)에 대해서 학습합니다.


# 14-01 시퀀스 레이블링(Sequence Labeling)

이번 챕터에서는 파이토치(PyTorch)로 인공 신경망을 이용하여 태깅 작업(텍스트의 단어나 토큰에 역할, 정답라벨, 의미적 카테고리 붙이는 전처리과정)을 하는 모델을 만듭니다. 

개체명 인식기와 품사 태거를 만드는데, 이러한 두 작업의 공통점은 RNN의 다-대-다(Many-to-Many) 작업이면서 또한 앞, 뒤 시점의 입력을 모두 참고하는 양방향 RNN(Bidirectional RNN)을 사용한다는 점입니다.

실습 챕터를 진행하기 전에 전체적으로 실습이 어떻게 진행되는지 정리해보도록 하겠습니다. 
<< 텍스트 분류 개요 챕터 >> 와 겹치는 부분에 대해서는 요약하여 설명하므로, 이해가 되지 않는 부분이 있다면 해당 챕터를 참고바랍니다.

## 1. 훈련 데이터에 대한 이해

태깅 작업은 앞서 배운 텍스트 분류 작업과 동일하게 지도 학습(Supervised Learning)에 속합니다. 이 챕터에서는 태깅을 해야하는 단어 데이터를 X, 레이블에 해당되는 태깅 정보 데이터는 y라고 이름을 붙였습니다. X에 대한 훈련 데이터는 X_train, 테스트 데이터는 X_test라고 명명하고 y에 대한 훈련 데이터는 y_train, 테스트 데이터는 y_test라고 명명합니다.

이번 챕터에서 << X와 y데이터의 쌍(pair)은 병렬 구조를 가진다는 특징>> 을 가집니다. 
X와 y의 각 샘플의 길이는 같습니다. 

예를 들어 품사 태깅 작업을 한다고 가정해보겠습니다. 그리고 X_train와 y_train의 데이터 중 4개의 샘플만 확인해본다고 가정해보겠습니다. 이 때 데이터는 다음과 같은 구조를 가집니다.

| idx | X_train                                                                  | y_train                                                 | 길이  |
| --- | ------------------------------------------------------------------------ | ------------------------------------------------------- | --- |
| 0   | \['EU', 'rejects', 'German', 'call', 'to', 'boycott', 'British', 'lamb'] | \['B-ORG', 'O', 'B-MISC', 'O', 'O', 'O', 'B-MISC', 'O'] | 8   |
| 1   | \['peter', 'blackburn']                                                  | \['B-PER', 'I-PER']                                     | 2   |
| 2   | \['brussels', '1996-08-22' ]                                             | \['B-LOC', 'O']                                         | 2   |
| 3   | \['The', 'European', 'Commission']                                       | \['O', 'B-ORG', 'I-ORG']                                | 3   |

(분할 기준은 보통 문장 단위.)
가령, X_train\[3]의 'The'와 y_train\[3]의 'O'는 하나의 쌍(pair)입니다. 
또한, X_train\[3]의 'European'과 y_train\[3]의 'B-ORG'는 쌍의 관계를 가지며, X_train\[3]의 'Commision'과 y_train\[3]의 'I-ORG'는 쌍의 관계를 가집니다.

이렇게 << 병렬 관계를 가지는 각 데이터는 정수 인코딩 과정 >> 을 거친 후, 
모든 데이터의 길이를 동일하게 맞춰주기위한 패딩(Padding) 작업을 거칩니다.

## 2. 시퀀스 레이블링(Sequence Labeling)

위와 같이 입력 시퀀스 X = \[, , , ..., ]에 대하여 레이블 시퀀스 y = \[, , , ..., ]를 각각 부여하는 작업을 시퀀스 레이블링 작업(Sequence Labeling Task)이라고 합니다. (회귀나 분류처럼 딱 하나로 안 나오고 seq 자체가 label!)

태깅 작업은 대표적인 시퀀스 레이블링 작업입니다.
(태깅 == 목적에 따라 ML을 써서 하는 전처리가 될수도 있고 이거자체가 메인모델이 될수도있음.)

## 3. 양방향 RNN(Bidirectional RNN)

```python
nn.RNN(input_size = input_size, hidden_size = hidden_size, num_layers = 1, batch_first=True, bidirectional = True)
```

이번 챕터에서도 바닐라 RNN이 아니라 성능이 개선된 RNN인 LSTM이나 GRU 등을 사용합니다. 

텍스트 분류 챕터에서는 단방향 RNN을 사용하였지만, 이번 챕터에서는 양방향 RNN을 사용합니다. 이전 시점의 단어 정보 뿐만 아니라, 다음 시점의 단어 정보도 참고하기 위함입니다. 

양방향은 기존의 단방향 nn.RNN()에서 ==bidirectional 인자의 값으로 True==를 넣으면 됩니다.

## **4. RNN의 다-대-다(Many-to-Many) 문제**

이제 RNN이 어떻게 설계되는지 확인해보도록 하겠습니다.
예를 들어 위에서 설명한 데이터 중 첫번째 데이터에 해당되는 X_train\[0]를 가지고 4번의 시점(time steps)까지 RNN을 진행하였을 때의 그림은 다음과 같습니다.

![](https://static.wikidocs.net/images/page/33805/forwardrnn_ver2.PNG)

하지만 이번 실습에서는 양방향 RNN을 사용할 것이므로 아래의 그림과 같습니다.

![](https://static.wikidocs.net/images/page/33805/bidirectionalrnn_ver2.PNG)

# 14-02 양방향 LSTM을 이용한 개체명인식

PyTorch의 양방향 LSTM(Bidirectional LSTM)을 이용하여 개체명 인식 모델을 구현해보겠습니다.

코퍼스로부터 각 개체(entity)의 유형을 인식하는 개체명 인식(Named Entity Recognition)에 대해서 학습합니다. 개체명 인식을 사용하면 코퍼스로부터 어떤 단어가 사람, 장소, 조직 등을 의미하는 단어인지를 찾을 수 있습니다.

## 1. 개체명 인식(Named Entity Recognition)이란?

개체명 인식(Named Entity Recognition)이란 말 그대로 **이름을 가진 개체(named entity)** 를 인식하겠다는 것을 의미합니다. 좀 더 쉽게 설명하면, 어떤 이름을 의미하는 단어를 보고는 그 단어가 어떤 유형인지를 인식하는 것을 말합니다.

예를 들어 **유정이는 2018년에 골드만삭스에 입사했다.** 라는 문장이 있을 때, 사람(person), 조직(organization), 시간(time)에 대해 개체명 인식을 수행하는 모델이라면 다음과 같은 결과를 보여줍니다.

```yaml
유정 - 사람  
2018년 - 시간  
골드만삭스 - 조직
```

## 2. NLTK를 이용한 개체명 인식(Named Entity Recognition using NTLK)

NLTK에서는 개체명 인식기(NER chunker)를 지원하고 있으므로, 별도 개체명 인식기를 구현할 필요없이 NLTK를 사용해서 개체명 인식을 수행할 수 있습니다. 

만약 아래의 실습에서 nltk.download('maxent_ne_chunker'), nltk.download('words') 등의 설치를 요구하는 에러 문구가 뜬다면, 지시하는대로 설치하면 됩니다.

```python
from nltk import word_tokenize, pos_tag, ne_chunk

sentence = "James is working at Disney in London"
# 토큰화 후 품사 태깅
tokenized_sentence = pos_tag(word_tokenize(sentence))
print(tokenized_sentence)
```

```python
[('James', 'NNP'), ('is', 'VBZ'), ('working', 'VBG'), ('at', 'IN'), ('Disney', 'NNP'), ('in', 'IN'), ('London', 'NNP')]
```

```python
# 개체명 인식
ner_sentence = ne_chunk(tokenized_sentence)
print(ner_sentence)
```

```csharp
(S
  (PERSON James/NNP)
  is/VBZ
  working/VBG
  at/IN
  (ORGANIZATION Disney/NNP)
  in/IN
  (GPE London/NNP))
```

ne_chunk는 개체명을 태깅하기 위해서 앞서 품사 태깅(pos_tag)이 수행되어야 합니다. 위의 결과에서 James는 PERSON(사람), Disney는 조직(ORGANIZATION), London은 위치(GPE)라고 정상적으로 개체명 인식이 수행된 것을 볼 수 있습니다. 

이제 인공 신경망을 이용하여 개체명 인식 모델을 만들어보겠습니다.


## 3. 양방향 LSTM을 이용한 개체명 인식

### 1) 데이터 로드 및 단어 토큰화

```python
import urllib.request
import numpy as np
from tqdm import tqdm
import re
from collections import Counter
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
```

NLTK를 이용하면 영어 코퍼스에 << 토큰화와 품사 태깅 전처리를 진행한 >>  문장 데이터를 받아올 수 있습니다. 

해당 데이터를 훈련시켜 품사 태깅을 수행하는 모델을 만들어보겠습니다. 
저자의 깃허브로부터 데이터를 다운로드합니다.

```python
urllib.request.urlretrieve("https://raw.githubusercontent.com/ukairia777/tensorflow-nlp-tutorial/main/12.%20RNN%20Sequence%20Labeling/dataset/train.txt", filename="train.txt")
```

전처리 후 전체 문장 샘플의 개수를 확인합니다.

### 전처리 과정 설명 : 데이터의 생김새

`f`는 `train.txt` 파일을 열어둔 객체입니다. 이 파일 안의 원본 텍스트는 다음과 같이 생겼습니다.

```
-DOCSTART- -X- -X- O

EU NNP B-NP B-ORG
rejects VBZ B-VP O
German JJ B-NP B-MISC
call NN I-NP O
to TO B-VP O
boycott VB I-VP O
British JJ B-NP B-MISC
lamb NN I-NP O
. . O

Peter NNP B-NP B-PER
Blackburn NNP I-NP I-PER

BRUSSELS NNP B-NP B-LOC
1996-08-22 CD I-NP O
```

(POS Tag, Chunk Tag, NER Tag)
(품사태그, 구문태그, 개체명 태그)


#### 코드 줄별 흐름 예시

위 `train.txt` 파일을 한 줄씩 읽으면서 코드가 어떻게 동작하는지 보면 다음과 같습니다.

- **1행: `-DOCSTART- -X- -X- O\n`**
    
    - `line.startswith('-DOCSTART')` 조건에 걸려 `continue`로 건너뜁니다.
        
- **2행: `\n` (빈 줄)**
    
    - `line[0] == "\n"` 조건에 걸립니다.
        
    - 한 문장이 끝났다는 뜻이므로 지금까지 모은 `sentence`를 `tagged_sentences`에 넣고 `sentence`를 비웁니다.
        
- **3행: `EU NNP B-NP B-ORG\n`**
    
    - `line.split(' ')` 실행 → `['EU', 'NNP', 'B-NP', 'B-ORG\n']`
        
    - `splits[0]` = `'eu'` (소문자 변환)
        
    - `splits[-1]` = `'B-ORG'` (`\n` 제거 후)
        
    - `sentence.append(['eu', 'B-ORG'])` 수행
        

#### 결과 형태

이 loop를 다 돌고 나면 `tagged_sentences` 변수에는 다음과 같이 문장별로 `[단어, 개체명태그]` 짝(Pair)이 묶여서 들어가게 됩니다.

```Python
[
    [['eu', 'B-ORG'], ['rejects', 'O'], ['german', 'B-MISC'], ['call', 'O'], ['to', 'O'], ['boycott', 'O'], ['british', 'B-MISC'], ['lamb', 'O'], ['.', 'O']],
    [['peter', 'B-PER'], ['blackburn', 'I-PER']],
    [['brussels', 'B-LOC'], ['1996-08-22', 'O']]
]
```


###

```python
f = open('train.txt', 'r')
tagged_sentences = []
sentence = []

for line in f:
    if len(line)==0 or line.startswith('-DOCSTART') or line[0]=="\n":
        if len(sentence) > 0:
            tagged_sentences.append(sentence)
            sentence = []
        continue
    splits = line.split(' ') # 공백을 기준으로 속성을 구분한다.
    splits[-1] = re.sub(r'\n', '', splits[-1]) # 줄바꿈 표시 \n을 제거한다.
    word = splits[0].lower() # 단어들은 소문자로 바꿔서 저장한다.
    sentence.append([word, splits[-1]]) # 단어와 개체명 태깅만 기록한다.

print("전체 샘플 개수: ", len(tagged_sentences)) # 전체 샘플의 개수 출력
```

```python
전체 샘플 개수:  14041
```

첫번째 샘플만 출력해보겠습니다.

```python
print(tagged_sentences[0]) # 첫번째 샘플 출력
```

```python
[['eu', 'B-ORG'], ['rejects', 'O'], ['german', 'B-MISC'], ['call', 'O'], ['to', 'O'], ['boycott', 'O'], ['british', 'B-MISC'], ['lamb', 'O'], ['.', 'O']]
```

각 문장 샘플에 대해서 단어는 sentences에, 태깅 정보는 pos_tags에 저장하고 첫번째 문장 샘플을 출력해보겠습니다.

```python
sentences, ner_tags = [], [] 
for tagged_sentence in tagged_sentences: # 14,041개의 문장 샘플을 1개씩 불러온다.
    sentence, tag_info = zip(*tagged_sentence) # 각 샘플에서 단어들은 sentence에 개체명 태깅 정보들은 tag_info에 저장.
    
    # zip(*) 하면 언패킹으로 묶인거 풀어주는 unzip해주는거임.
    
    sentences.append(list(sentence)) # 각 샘플에서 단어 정보만 저장한다.
    ner_tags.append(list(tag_info)) # 각 샘플에서 개체명 태깅 정보만 저장한다.

print(sentences[0])
print(ner_tags[0])
```

```python
['eu', 'rejects', 'german', 'call', 'to', 'boycott', 'british', 'lamb', '.']
['B-ORG', 'O', 'B-MISC', 'O', 'O', 'O', 'B-MISC', 'O', 'O']
```

첫번째 샘플에 대해서 단어에 대해서 sentences\[0]에, 품사에 대해서만 pos_tags\[0]에 저장된 것을 볼 수 있습니다. 

뒤에서 보겠지만, sentences는 예측을 위한 X에 해당되며 pos_tags는 예측 대상인 y에 해당됩니다. 임의로 12번 인덱스 샘플에 대해서도 확인해보겠습니다.

```python
print(sentences[12])
print(ner_tags[12])
```

```python
['only', 'france', 'and', 'britain', 'backed', 'fischler', "'s", 'proposal', '.']
['O', 'B-LOC', 'O', 'B-LOC', 'O', 'B-PER', 'O', 'O', 'O']
```

단어에 대해서만 sentences\[12]에, 또한 품사에 대해서만 pos_tags\[12]에 저장된 것을 확인할 수 있습니다. 또한 첫번째 샘플과 길이가 다른 것을 볼 수 있습니다. 이제 훈련 데이터와 테스트 데이터를 분리해봅시다.

```python
X_train, X_test, y_train, y_test = train_test_split(sentences, ner_tags, test_size=.2, random_state=777)
```

학습이 진행되는 동안 성능을 확인하기 위한 검증 데이터 또한 분리합니다.

```python
X_train, X_valid, y_train, y_valid = train_test_split(X_train, y_train, test_size=.2, random_state=777)
```

학습 데이터, 검증 데이터, 테스트 데이터의 개수는 다음과 같습니다.

```python
print('훈련 데이터의 개수 :', len(X_train))
print('검증 데이터의 개수 :', len(X_valid))
print('테스트 데이터의 개수 :', len(X_test))
print('훈련 데이터 레이블의 개수 :', len(X_train))
print('검증 데이터 레이블의 개수 :', len(X_valid))
print('테스트 데이터 레이블의 개수 :', len(X_test))
```

```python
훈련 데이터의 개수 : 8985
검증 데이터의 개수 : 2247
테스트 데이터의 개수 : 2809
훈련 데이터 레이블의 개수 : 8985
검증 데이터 레이블의 개수 : 2247
테스트 데이터 레이블의 개수 : 2809
```

학습 데이터의 상위 2개 샘플만 출력해봅시다. 현재 데이터는 단어 토큰화가 된 상태입니다.

```python
# 상위 샘플 2개 출력
for sent in X_train[:2]:
  print(sent)
```

```python
['young', 'boys', '9', '1', '0', '8', '6', '19', '3']
['hentgen', '(', '17-7', ')', 'surrendered', 'just', 'three', 'doubles', 'and', 'a', 'pair', 'of', 'singles', 'in', 'tossing', 'his', 'major-league', 'leading', 'ninth', 'complete', 'game', '.']
```

### 2) Vocab 만들기

단어 집합을 만들어봅시다. 각 단어의 등장 빈도를 카운트해주는 Counter를 사용하여 각 단어별 빈도수를 기록합니다. 이렇게 기록된 단어의 총 종류를 출력하여 총 단어수를 확인해봅시다.

```python
word_list = []
for sent in X_train:
    for word in sent:
      word_list.append(word)

word_counts = Counter(word_list)
print('총 단어수 :', len(word_counts))
```

```python
총 단어수 : 16742
```

단어수는 16,742개입니다. 임의로 영단어 the와 love의 등장횟수를 확인해보겠습니다.

```python
print('훈련 데이터에서의 단어 the의 등장 횟수 :', word_counts['the'])
print('훈련 데이터에서의 단어 love의 등장 횟수 :', word_counts['love'])
```

```python
훈련 데이터에서의 단어 the의 등장 횟수 : 5410
훈련 데이터에서의 단어 love의 등장 횟수 : 7
```

영단어 the의 등장 횟수는 5,410회이며, 영단어 love의 등장 횟수는 7회입니다. 
word_counts를 정렬하고 등장 빈도 상위 10개 단어를 출력해봅시다.

```python
vocab = sorted(word_counts, key=word_counts.get, reverse=True)
print('등장 빈도수 상위 10개 단어')
print(vocab[:10])
```

```python
등장 빈도수 상위 10개 단어
['the', ',', '.', 'of', 'in', 'to', 'a', ')', '(', 'and']
```

이제 단어 집합을 만들기 위해서 패딩을 위한 토큰, 
그리고 OOV 문제(Out-Of-Vocabulary) 발생 시에 사용하는 UNK 토큰을 위한 정수 0과 1을 각각 단어 집합에 할당합니다.

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
패딩 토큰과 UNK 토큰을 고려한 단어 집합의 크기 : 16744
```

```python
print('단어 <PAD>와 맵핑되는 정수 :', word_to_index['<PAD>'])
print('단어 <UNK>와 맵핑되는 정수 :', word_to_index['<UNK>'])
print('단어 the와 맵핑되는 정수 :', word_to_index['the'])
```

```python
단어 <PAD>와 맵핑되는 정수 : 0
단어 <UNK>와 맵핑되는 정수 : 1
단어 the와 맵핑되는 정수 : 2
```

### 3) 정수 인코딩

텍스트를 정수로 변환해주는 함수를 만듭니다. 
해당 함수는 OOV 문제가 발생할 경우 해당 단어를`<UNK>` 토큰과 맵핑되는 정수인 1로 변환합니다.

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

훈련 데이터, 검증 데이터, 테스트 데이터에 대해서 정수 인코딩을 진행합니다.

```python
encoded_X_train = texts_to_sequences(X_train, word_to_index)
encoded_X_valid = texts_to_sequences(X_valid, word_to_index)
encoded_X_test = texts_to_sequences(X_test, word_to_index)
```

정수 인코딩 된 상위 샘플 2개만 출력해봅시다.

```python
# 상위 샘플 2개 출력
for sent in encoded_X_train[:2]:
  print(sent)
```

```python
[1260, 3215, 117, 17, 21, 123, 56, 539, 23]
[5456, 10, 8229, 9, 8230, 186, 84, 1815, 11, 8, 1073, 5, 421, 6, 8231, 35, 2043, 291, 790, 957, 267, 4]
```

정수로부터 단어로 변환하는 `word_to_index`의 key와 value를 반대로 저장하여 `index_to_word`를 만들고, 정수 인코딩 된 첫번째 샘플을 복원해봅시다.

```python
index_to_word = {}
for key, value in word_to_index.items():
    index_to_word[value] = key

decoded_sample = [index_to_word[word] for word in encoded_X_train[0]]
print('기존의 첫번째 샘플 :', X_train[0])
print('복원된 첫번째 샘플 :', decoded_sample)
```

```python
기존의 첫번째 샘플 : ['young', 'boys', '9', '1', '0', '8', '6', '19', '3']
복원된 첫번째 샘플 : ['young', 'boys', '9', '1', '0', '8', '6', '19', '3']
```

이제 레이블에 대해서도 정수 인코딩을 진행해야 합니다. 
레이블에 존재하는 모든 단어들의 집합을 구해봅시다.

```python
# y_train으로부터 존재하는 모든 태그들의 집합 구하기
flatten_tags = [tag for sent in y_train for tag in sent]
tag_vocab = list(set(flatten_tags))
print('태그 집합 :', tag_vocab)
print('태그 집합의 크기 :', len(tag_vocab))
```

```python
태그 집합 : ['B-PER', 'I-MISC', 'B-ORG', 'I-PER', 'B-LOC', 'I-LOC', 'I-ORG', 'O', 'B-MISC']
태그 집합의 크기 : 9
```

레이블의 각 단어에 정수를 부여하여 단어 집합(Vocabulary)를 만듭니다.

```python
tag_to_index = {}
tag_to_index['<PAD>'] = 0

for index, word in enumerate(tag_vocab) :
  tag_to_index[word] = index + 1

tag_vocab_size = len(tag_to_index)
# print('패딩 토큰까지 포함된 태그 집합의 크기 :', tag_vocab_size)
print('태그 집합 :', tag_to_index)
```

```python
태그 집합 : {'<PAD>': 0, 'B-PER': 1, 'I-MISC': 2, 'B-ORG': 3, 'I-PER': 4, 'B-LOC': 5, 'I-LOC': 6, 'I-ORG': 7, 'O': 8, 'B-MISC': 9}
```

many-to-many 문제의 경우에는 레이블도 시퀀스 데이터가 되므로 각 레이블을 정수 시퀀스로 변환해줍니다. 다시 말해 << 레이블에 대해서 정수 인코딩 >>을 진행합니다. 

이를 위해 `tag_to_index`를 이용하여 레이블의 각 단어를 정수로 변환하는 함수인 `encoding_label()` 함수를 구현합니다.

```python
def encoding_label(sequence, tag_to_index):
  label_sequence = []
  for seq in sequence:
    label_sequence.append([tag_to_index[tag] for tag in seq])
  return label_sequence
```

학습 데이터, 검증 데이터, 테스트 데이터의 레이블에 대해서 정수 인코딩을 진행합니다.

```python
encoded_y_train = texts_to_sequences(y_train, tag_to_index)
encoded_y_valid = texts_to_sequences(y_valid, tag_to_index)
encoded_y_test = texts_to_sequences(y_test, tag_to_index)
```

상위 2개의 샘플에 대해서 정수 인코딩 된 결과를 출력해봅시다.

```python
print('X 데이터 상위 2개')
print(encoded_X_train[:2])
print('-' * 50)
print('y 데이터 상위 2개')
print(encoded_y_train[:2])
print('-' * 50)
print('첫번째 샘플과 레이블의 길이')
print(len(encoded_X_train[0]))
print(len(encoded_y_train[0]))
```

```python
X 데이터 상위 2개
[[1260, 3215, 117, 17, 21, 123, 56, 539, 23], [5456, 10, 8229, 9, 8230, 186, 84, 1815, 11, 8, 1073, 5, 421, 6, 8231, 35, 2043, 291, 790, 957, 267, 4]]
--------------------------------------------------
y 데이터 상위 2개
[[3, 7, 8, 8, 8, 8, 8, 8, 8], [1, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8]]
--------------------------------------------------
첫번째 샘플과 레이블의 길이
9
9
```

### 4) 패딩

데이터의 길이를 동일하게 맞춰주는 작업인 패딩을 위해서는 각 데이터의 길이 분포를 확인할 필요가 있습니다.

```python
print('샘플의 최대 길이 : %d' % max(len(l) for l in encoded_X_train))
print('샘플의 평균 길이 : %f' % (sum(map(len, encoded_X_train))/len(encoded_X_train)))
plt.hist([len(s) for s in encoded_X_train], bins=50)
plt.xlabel('length of samples')
plt.ylabel('number of samples')
plt.show()
```

```python
샘플의 최대 길이 : 78
샘플의 평균 길이 : 14.518420
```

![](https://static.wikidocs.net/images/page/217054/%ED%8C%A8%EB%94%A9%EA%B8%B8%EC%9D%B4.PNG)

가장 긴 샘플의 길이는 78이며, 그래프를 봤을 때 전체 데이터의 길이 분포는 대체적으로 약 50내외의 길이를 가지는 것을 볼 수 있습니다. 모델이 처리할 수 있도록 encoded_X_train과 encoded_X_test의 모든 샘플의 길이를 특정 길이로 동일하게 맞춰줄 필요가 있습니다. 

특정 길이 변수를 max_len으로 정합니다. 대부분의 리뷰가 내용이 잘리지 않도록 할 수 있는 최적의 max_len의 값은 몇일까요? 전체 샘플 중 길이가 max_len 이하인 샘플의 비율이 몇 %인지 확인하는 함수를 만듭니다.

```python
def below_threshold_len(max_len, nested_list):
  count = 0
  for sentence in nested_list:
    if(len(sentence) <= max_len):
        count = count + 1
  print('전체 샘플 중 길이가 %s 이하인 샘플의 비율: %s'%(max_len, (count / len(nested_list))*100))
```

사실 최대 길이가 78이므로 78로 패딩해도 됩니다. 여기서는 80정도로 패딩해보겠습니다.
(패딩값은 학습시 임베딩층 수준에서 무시해버리기때문에 여유있게 해도 동일하긴 하지만 메모리는 그만큼 먹을수있으니 실무에서는 주의하자.)

```python
max_len = 80
below_threshold_len(max_len, encoded_X_train)
```

```python
전체 샘플 중 길이가 80 이하인 샘플의 비율: 100.0
```

모든 데이터의 길이를 80으로 패딩해보겠습니다.  
`max_len`을 인자로 입력받아서 `max_len`보다 짧은 데이터의 경우에는 
뒤에 0을 추가하는 함수인 `pad_sequences()`를 구현합니다.

```python
def pad_sequences(sentences, max_len):
    features = np.zeros((len(sentences), max_len), dtype=int)
    for index, sentence in enumerate(sentences):
        if len(sentence) != 0:
            features[index, :len(sentence)] = np.array(sentence)[:max_len]
    return features
```

함수 `pad_sequences()`로 훈련 데이터, 검증 데이터, 테스트 데이터를 패딩합니다. 
이때 개체명 인식과 같은 Many-to-Many 문제를 푸는 경우에는 레이블도 패딩해주어야 합니다.
패딩 후에 모든 데이터 길이가 80으로 패딩되었는지 확인합니다.

```python
padded_X_train = pad_sequences(encoded_X_train, max_len=max_len)
padded_X_valid = pad_sequences(encoded_X_valid, max_len=max_len)
padded_X_test = pad_sequences(encoded_X_test, max_len=max_len)

padded_y_train = pad_sequences(encoded_y_train, max_len=max_len)
padded_y_valid = pad_sequences(encoded_y_valid, max_len=max_len)
padded_y_test = pad_sequences(encoded_y_test, max_len=max_len)

print('훈련 데이터의 크기 :', padded_X_train.shape)
print('검증 데이터의 크기 :', padded_X_valid.shape)
print('테스트 데이터의 크기 :', padded_X_test.shape)
print('-' * 30)
print('훈련 데이터의 레이블 :', padded_y_train.shape)
print('검증 데이터의 레이블 :', padded_y_valid.shape)
print('테스트 데이터의 레이블 :', padded_y_test.shape)
```

```python
훈련 데이터의 크기 : (8985, 80)
검증 데이터의 크기 : (2247, 80)
테스트 데이터의 크기 : (2809, 80)
------------------------------
훈련 데이터의 레이블 : (8985, 80)
검증 데이터의 레이블 : (2247, 80)
테스트 데이터의 레이블 : (2809, 80)
```

패딩 후의 데이터를 확인해보겠습니다.

```python
print('훈련 데이터의 상위 샘플 2개')
print(padded_X_train[:2])
print('-' * 5 + '레이블' + '-' * 5)
print(padded_y_train[:2])
```

```python
훈련 데이터의 상위 샘플 2개
[[1260 3215  117   17   21  123   56  539   23    0    0    0    0    0
     0    0    0    0    0    0    0    0    0    0    0    0    0    0
     0    0    0    0    0    0    0    0    0    0    0    0    0    0
     0    0    0    0    0    0    0    0    0    0    0    0    0    0
     0    0    0    0    0    0    0    0    0    0    0    0    0    0
     0    0    0    0    0    0    0    0    0    0]
 [5456   10 8229    9 8230  186   84 1815   11    8 1073    5  421    6
  8231   35 2043  291  790  957  267    4    0    0    0    0    0    0
     0    0    0    0    0    0    0    0    0    0    0    0    0    0
     0    0    0    0    0    0    0    0    0    0    0    0    0    0
     0    0    0    0    0    0    0    0    0    0    0    0    0    0
     0    0    0    0    0    0    0    0    0    0]]
-----레이블-----
[[3 7 8 8 8 8 8 8 8 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
  0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
  0 0 0 0 0 0 0 0]
 [1 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 8 0 0 0 0 0 0 0 0 0 0 0 0 0 0
  0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
  0 0 0 0 0 0 0 0]]
```

### 5) 모델링

이제 모델을 구현해봅시다.

```python
import torch
import torch.nn as nn
import torch.optim as optim
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

cuda라고 출력된다면 GPU를 활용 가능한 환경입니다. 

만약, Colab에서 실습 중이고 cuda가 출력되지 않는다면 Colab 화면 상단에서 `런타임 > 런타임 유형 변경 > 하드웨어 가속기 > GPU 장비 선택`를 이용하여 GPU 장비를 선택하신 후에 실습하시기 바랍니다.

이제 개체명 인식 모델을 만들어봅시다. 
만약, 단방향 GRU를 모델로 사용할 경우 코드는 아래와 같습니다.

```python
class NERTagger(nn.Module):
    def __init__(self, vocab_size, embedding_dim, hidden_dim, output_dim):
        super(NERTagger, self).__init__()
        self.embedding = nn.Embedding(vocab_size, embedding_dim)
        self.gru = nn.GRU(embedding_dim, hidden_dim, batch_first=True)
        self.fc = nn.Linear(hidden_dim, output_dim)

    def forward(self, x):
        # x: (batch_size, seq_length)
        embedded = self.embedding(x)  # (batch_size, seq_length, embedding_dim)
        gru_out, _ = self.gru(embedded)  # (batch_size, seq_length, hidden_dim)
        logits = self.fc(gru_out)  # (batch_size, seq_length, output_dim)
        return logits
```

하지만 위의 GRU를 양방향 LSTM(Bidirectional LSTM) 2층짜리로 변경하려면 
다음과 같이 수정하면 됩니다.

```python
class NERTagger(nn.Module):
    def __init__(self, vocab_size, embedding_dim, hidden_dim, output_dim, num_layers=2):
        super(NERTagger, self).__init__()
        self.embedding = nn.Embedding(vocab_size, embedding_dim)
        self.lstm = nn.LSTM(embedding_dim, hidden_dim, num_layers=num_layers, batch_first=True, bidirectional=True)
        self.fc = nn.Linear(hidden_dim*2, output_dim)

    def forward(self, x):
        # x: (batch_size, seq_length)
        embedded = self.embedding(x)  # (batch_size, seq_length, embedding_dim)
        lstm_out, _ = self.lstm(embedded)  # (batch_size, seq_length, hidden_dim*2)
        logits = self.fc(lstm_out)  # (batch_size, seq_length, output_dim)
        return logits
```

위에서 작성한 GRU 코드와 양방향 LSTM 코드의 차이점을 봅시다.

- nn.GRU를 nn.LSTM으로 변경했습니다.
- num_layers 매개변수를 추가하고 이를 nn.LSTM 생성자에 전달했습니다. 기본값은 2입니다.
- bidirectional=True를 추가하여 양방향 LSTM을 사용하도록 설정했습니다.
- nn.Linear의 입력 차원을 hidden_dim*2로 변경하여 양방향 LSTM의 출력을 처리하도록 했습니다. (양방향이니까 당연히 출력처리량도 2배임.)

사용할 데이터를 파이토치의 텐서로 변환하고, 배치 단위 처리를 위해 데이터로더로 변환합니다.

```python
X_train_tensor = torch.tensor(padded_X_train, dtype=torch.long)
y_train_tensor = torch.tensor(padded_y_train, dtype=torch.long)
X_valid_tensor = torch.tensor(padded_X_valid, dtype=torch.long)
y_valid_tensor = torch.tensor(padded_y_valid, dtype=torch.long)
X_test_tensor = torch.tensor(padded_X_test, dtype=torch.long)
y_test_tensor = torch.tensor(padded_y_test, dtype=torch.long)

train_dataset = torch.utils.data.TensorDataset(X_train_tensor, y_train_tensor)
train_dataloader = torch.utils.data.DataLoader(train_dataset, shuffle=True, batch_size=32)
valid_dataset = torch.utils.data.TensorDataset(X_valid_tensor, y_valid_tensor)
valid_dataloader = torch.utils.data.DataLoader(valid_dataset, shuffle=False, batch_size=32)
test_dataset = torch.utils.data.TensorDataset(X_test_tensor, y_test_tensor)
test_dataloader = torch.utils.data.DataLoader(test_dataset, shuffle=False, batch_size=32)
```

이제 위에서 선언한 `NERTagger` 클래스로부터 모델 객체를 만들어봅시다. 현재 단어 집합의 크기는 다음과 같습니다.

```python
print('단어 집합의 크기:', vocab_size)
```

```python
단어 집합의 크기: 16744
```

모델 객체를 선언하기 위한 하이퍼파라미터 값은 다음과 같습니다. 
임베딩 벡터의 차원은 100, LSTM의 은닉 상태의 차원은 256, 출력층의 차원은 tag_vocab_size이며 앞에서 확인한 바와 같이 10이며, 학습률(learning rate)는 0.01, 학습 횟수에 해당하는 에포크는 10, LSTM의 은닉층 수는 2로 지정했습니다.

```python
embedding_dim = 100
hidden_dim = 256
output_dim = tag_vocab_size
learning_rate = 0.01
num_epochs = 10
num_layers = 2
```

이로부터 모델 객체를 선언합니다.

```python
# Model, loss, optimizer
model = NERTagger(vocab_size, embedding_dim, hidden_dim, output_dim, num_layers)
model.to(device)
```

앞으로 많이 사용하게 될 비용함수인 nn.CrossEntropyLoss에서는 ignore_index를 통해서 특정 인덱스에 대한 loss를 구하지 않을 수 있습니다. 

<<< ignore_index=0을 사용하면 패딩 위치에 대해서는 loss를 구하지 않습니다. >>> 

```python
criterion = nn.CrossEntropyLoss(ignore_index=0)
optimizer = optim.Adam(model.parameters(), lr=learning_rate)
```

### 6) 평가 코드 작성

학습하는 동안 학습 데이터와 검증 데이터에 대한 정확도와 loss를 구할 것이므로 학습하기 전에 평가 코드를 작성해야만 합니다. 우선 모델의 예측값과 실제값으로부터 정확도를 구하는 함수인 `calculate_accuracy()`를 작성합니다. 

해당 함수에서 고려해야할 점은 << 패딩 토큰이 있는 부분에 대해서는 계산을 하지 않는다 >> 는 점입니다.

```python
def calculate_accuracy(logits, labels, ignore_index=0):
    # 예측 레이블을 구합니다.
    predicted = torch.argmax(logits, dim=1)

    # 패딩 토큰은 무시합니다.
    mask = (labels != ignore_index)

    # 정답을 맞춘 경우를 집계합니다.
    correct = (predicted == labels).masked_select(mask).sum().item()
    total = mask.sum().item()

    accuracy = correct / total
    return accuracy
```

검증 데이터의 데이터로더로부터 모델의 성능을 측정하는 `evaluate()` 함수를 구현합니다. `evaluate()` 함수 내부에서는 위에서 작성한 `calculate_accuracy()`를 호출하여 사용하고 있습니다.

```python
def evaluate(model, valid_dataloader, criterion, device):
    val_loss = 0
    val_correct = 0
    val_total = 0

    model.eval()
    with torch.no_grad():
        for batch_X, batch_y in valid_dataloader:
            batch_X, batch_y = batch_X.to(device), batch_y.to(device)

            # Forward pass
            logits = model(batch_X)

            # Compute loss
            loss = criterion(logits.view(-1, output_dim), batch_y.view(-1))

            # Calculate validation accuracy and loss
            val_loss += loss.item()
            val_correct += calculate_accuracy(logits.view(-1, output_dim), batch_y.view(-1)) * batch_y.size(0)
            val_total += batch_y.size(0)

    val_accuracy = val_correct / val_total
    val_loss /= len(valid_dataloader)

    return val_loss, val_accuracy
```

### 7) 모델 학습하기

```python
# Training loop
best_val_loss = float('inf')

for epoch in range(num_epochs):
    # Training
    train_loss = 0
    train_correct = 0
    train_total = 0
    model.train()
    for batch_X, batch_y in train_dataloader:
        # Forward pass
        batch_X, batch_y = batch_X.to(device), batch_y.to(device)
        logits = model(batch_X)

        # Compute loss
        loss = criterion(logits.view(-1, output_dim), batch_y.view(-1))

        # Backward pass and optimization
        optimizer.zero_grad()
        loss.backward()
        optimizer.step()

        # Calculate training accuracy and loss
        train_loss += loss.item()
        train_correct += calculate_accuracy(logits.view(-1, output_dim), batch_y.view(-1)) * batch_y.size(0)
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

학습은 정해진 횟수(num_epochs)만큼 반복되는데, 여기서는 5번 반복하도록 설정되어 있습니다. 

학습 과정에서는 train_dataloader에서 배치(batch) 단위로 데이터를 가져와서 모델에 입력합니다. 모델은 입력 데이터를 처리하여 예측값(logits)을 출력하고, 이를 실제 정답(batch_y)과 비교하여 손실(loss)을 계산합니다. 그 다음, 손실을 기반으로 모델의 가중치를 조정하는 역전파(backward pass)와 최적화(optimization) 과정을 거칩니다.

각 배치마다 계산된 손실과 정확도는 에포크 단위로 누적되어 평균값으로 계산됩니다. 에포크가 끝날 때마다 학습 손실(train_loss), 학습 정확도(train_accuracy), 검증 손실(val_loss), 검증 정확도(val_accuracy)를 출력하여 모델의 성능을 모니터링합니다.

검증 손실(val_loss)이 이전에 기록된 최소 검증 손실(best_val_loss)보다 작아지면, 해당 에포크의 모델 가중치를 체크포인트(checkpoint)로 저장합니다. 이를 통해 가장 성능이 좋은 모델을 저장할 수 있습니다. 이 과정을 설정된 에포크 수만큼 반복하면서 모델을 학습시키고, 최종적으로 가장 좋은 성능을 보인 모델의 가중치를 얻게 됩니다.

(학습 과정 자체는 13챕터 기본 학습과정과 동일함.)

### 8) 모델 로드 및 평가

위에서 저장해둔 Best 모델을 로드하여 정상 로드되었는지 확인하기 위해서 검증 데이터에 대한 정확도와 손실을 출력하고, 테스트 데이터에 대해서도 평가를 진행합니다.

```python
# 모델 로드
model.load_state_dict(torch.load('best_model_checkpoint.pth'))

# 모델을 device에 올립니다.
model.to(device)

# 검증 데이터에 대한 정확도(accuracy)와 손실(loss) 계산
val_loss, val_accuracy = evaluate(model, valid_dataloader, criterion, device)

print(f'Best model validation loss: {val_loss:.4f}')
print(f'Best model validation accuracy: {val_accuracy:.4f}')
```

```python
Best model validation loss: 0.1606
Best model validation accuracy: 0.9560
```

테스트 데이터에 대해서도 정확도와 손실을 계산합니다.

```python
# 테스트 데이터에 대한 정확도와 손실 계산
test_loss, test_accuracy = evaluate(model, test_dataloader, criterion, device)

print(f'Best model test loss: {test_loss:.4f}')
print(f'Best model test accuracy: {test_accuracy:.4f}')
```

```python
Best model test loss: 0.1609
Best model test accuracy: 0.9566
```

### 9) 인퍼런스 및 테스트

모델을 서비스에 적용하게 되면 전처리가 전혀 되어있지 않은 임의의 텍스트 입력에 대해서 동작해야 할 것입니다. 임의의 텍스트 입력에 대해서 예측 레이블을 리턴하는 함수를 만들어봅시다.

```python
index_to_tag = {}
for key, value in tag_to_index.items():
    index_to_tag[value] = key

def predict_labels(text, model, word_to_ix, index_to_tag, max_len=150):
    # 단어 토큰화
    tokens = text.split()

    # 정수 인코딩
    token_indices = [word_to_ix.get(token, 1) for token in tokens]

    # 패딩
    token_indices_padded = np.zeros(max_len, dtype=int)
    token_indices_padded[:len(token_indices)] = token_indices[:max_len]

    # 텐서로 변환
    input_tensor = torch.tensor(token_indices_padded, dtype=torch.long).unsqueeze(0).to(device)

    # 모델의 입력으로 사용하고 예측값 리턴
    model.eval()
    with torch.no_grad():
        logits = model(input_tensor)

    # 가장 값이 높은 인덱스를 예측값으로 선택
    predicted_indices = torch.argmax(logits, dim=-1).squeeze(0).tolist()

    # 패딩 토큰 제거
    predicted_indices_no_pad = predicted_indices[:len(tokens)]

    # 패딩 토큰을 제외하고 정수 시퀀스를 예측 시퀀스로 변환
    predicted_tags = [index_to_tag[index] for index in predicted_indices_no_pad]

    return predicted_tags
```

학습에 사용되지 않은 테스트 데이터의 첫번째 샘플을 이용해봅시다. 현재 이 데이터는 이미 단어 토큰화가 된 상태라서 단어 토큰화 이전 상태로 되돌려 전처리가 전혀 되어있지 않은 입력을 가정하고 함수에 입력으로 사용하겠습니다.

```python
print(X_test[0])
```

```python
['feyenoord', 'rotterdam', 'suffered', 'an', 'early', 'shock', 'when', 'they', 'went', '1-0', 'down', 'after', 'four', 'minutes', 'against', 'de', 'graafschap', 'doetinchem', '.']
```

토큰화 이전 상태로 돌린 후는 다음과 같습니다.

```python
sample = ' '.join(X_test[0])
print(sample)
```

```python
feyenoord rotterdam suffered an early shock when they went 1-0 down after four minutes against de graafschap doetinchem .
```

실제 레이블과 예측값을 비교해봅시다.

```python
predicted_tags = predict_labels(sample, model, word_to_index, index_to_tag)
print('예측 :', predicted_tags)
print('실제값 :', y_test[0])
```

```python
예측 : ['B-ORG', 'I-ORG', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'B-ORG', 'I-ORG', 'I-ORG', 'O']
실제값 : ['B-ORG', 'I-ORG', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'B-ORG', 'I-ORG', 'O']
```

![[Pasted image 20260901222134.png|491]]