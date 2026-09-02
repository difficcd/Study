
> 이번 챕터에서는 두 개의 RNN 아키텍처를 사용해서 만드는 시퀀스투시퀀스 구조에 대해서 이해합니다.


# 16-01 시퀀스투시퀀스(Sequence-to-Sequence, seq2seq)


시퀀스-투-시퀀스(Sequence-to-Sequence)는 입력된 시퀀스로부터 다른 도메인의 시퀀스를 출력하는 다양한 분야에서 사용되는 모델입니다. 예를 들어 챗봇(Chatbot)과 기계 번역(Machine Translation)이 그러한 대표적인 예인데, 입력 시퀀스와 출력 시퀀스를 각각 질문과 대답으로 구성하면 챗봇으로 만들 수 있고, 입력 시퀀스와 출력 시퀀스를 각각 입력 문장과 번역 문장으로 만들면 번역기로 만들 수 있습니다. 그 외에도 << 내용 요약(Text Summarization), STT(Speech to Text) 등 >> 에서 쓰일 수 있습니다.

이번 챕터에서는 기계 번역을 예제로 시퀀스-투-시퀀스를 설명합니다. 앞으로는 줄여서 seq2seq이라는 이름으로 설명하겠습니다. seq2seq에 대한 구조를 이해하고, 파이토치(PyTorch)를 통해 직접 구현해봅시다.

## 1. 모델의 개요(Overview)

seq2seq는 번역기에서 대표적으로 사용되는 모델입니다. 앞으로의 설명 방식은 내부가 보이지 않는 커다란 블랙 박스에서 점차적으로 확대해가는 방식으로 설명합니다. 참고로 여기서 설명하는 내용의 대부분은 RNN 챕터에서 언급한 내용들입니다. 

단지 이것을 가지고 어떻게 조립했느냐에 따라서 seq2seq라는 구조가 만들어집니다.

![](https://static.wikidocs.net/images/page/24996/%EC%8B%9C%ED%80%80%EC%8A%A4%ED%88%AC%EC%8B%9C%ED%80%80%EC%8A%A4.PNG)

위의 그림은 seq2seq 모델로 만들어진 번역기가 'I am a student'라는 영어 문장을 입력받아서, 'je suis étudiant'라는 프랑스 문장을 출력하는 모습을 보여줍니다. 그렇다면, seq2seq 모델 내부의 모습은 어떻게 구성되었을까요?

![](https://static.wikidocs.net/images/page/24996/seq2seq%EB%AA%A8%EB%8D%B811.PNG)

seq2seq는 크게 두 개로 구성된 아키텍처로 구성되는데, 바로 인코더와 디코더입니다. 

인코더는 << 입력 문장의 모든 단어들을 순차적으로 입력받은 뒤에 마지막에 이 모든 단어 정보들을 압축해서 하나의 벡터 >> 로 만드는데(그냥 인코딩 과정 그 자체를 수행해서 인코더라고 생각하면됨. 모델이 입력데이터 받아서 벡터화시키는 과정 - 콘텍스트 - 디코더 이렇게 이해하면됨.), 이를 ==컨텍스트 벡터(context vector)==라고 합니다. 

컨텍스트란 한국어로는 '문맥'입니다. 입력 문장의 정보가 하나의 컨텍스트 벡터로 모두 압축되면 인코더는 컨텍스트 벡터를 디코더로 전송합니다. 

디코더는 컨텍스트 벡터를 받아서 번역된 단어를 한 개씩 순차적으로 출력합니다.

![](https://static.wikidocs.net/images/page/24996/%EC%BB%A8%ED%85%8D%EC%8A%A4%ED%8A%B8_%EB%B2%A1%ED%84%B0.PNG)

위의 그림에서는 컨텍스트 벡터를 4의 사이즈로 표현하였지만, << 실제 현업에서 사용되는 seq2seq 모델에서는 보통 수백 이상의 차원 >> 을 갖고있습니다. 이제 인코더와 디코더의 내부를 좀 더 확대해보겠습니다.

## 2. seq2seq의 동작 과정

![](https://static.wikidocs.net/images/page/24996/%EC%9D%B8%EC%BD%94%EB%8D%94%EB%94%94%EC%BD%94%EB%8D%94%EB%AA%A8%EB%8D%B8.PNG)

인코더 아키텍처와 디코더 아키텍처의 내부는 사실 두 개의 RNN 아키텍처 입니다. 
=> 인코더, 디코더 단위체가 각각 RNN 모델인 것

입력 문장을 받는 RNN 셀을 인코더라고 하고, 출력 문장을 출력하는 RNN 셀을 디코더라고 합니다. 

이번 챕터에서는 인코더의 RNN 셀을 주황색으로, 디코더의 RNN 셀을 초록색으로 표현합니다. 물론, 성능 문제로 인해 실제로는 바닐라 RNN이 아니라 **LSTM 셀** 또는 **GRU 셀**들로 구성됩니다. 

우선 인코더를 자세히보면, 입력 문장은 단어 토큰화를 통해서 단어 단위로 쪼개지고 
단어 토큰 각각은 RNN 셀의 각 시점의 입력이 됩니다. 

인코더 RNN 셀은 모든 단어를 입력받은 뒤에 **인코더 RNN 셀의 마지막 시점의 은닉 상태**를 디코더 RNN 셀로 넘겨주는데 이를 **컨텍스트 벡터**라고 합니다. 

컨텍스트 벡터는 **디코더 RNN 셀의 첫번째 은닉 상태**로 사용됩니다.

### 1) 테스트 단계

디코더는 초기 입력으로 << 문장의 시작을 의미하는 심볼 \<sos>가 >> (start of sequence) 들어갑니다. 
디코더는 \<sos>가 입력되면, 다음에 등장할 확률이 높은 단어를 예측합니다. 
첫번째 시점(time step)의 디코더 RNN 셀은 다음에 등장할 단어로 je를 예측하였습니다. 첫번째 시점의 디코더 RNN 셀은 예측된 단어 je를 다음 시점의 RNN 셀의 입력으로 입력합니다. 그리고 두번째 시점의 디코더 RNN 셀은 입력된 단어 je로부터 다시 다음에 올 단어인 suis를 예측하고, 또 다시 이것을 다음 시점의 RNN 셀의 입력으로 보냅니다. 

(입력 문장을 반영한 lstm 의 output을 sos 자리에서 받아서 그 자리에 해당하는 확률이 가장 높은 토큰을 뱉음. 즉 인코더의 지금까지의 문맥이 디코더로 들어온 그 시작점을 일단 디코더가 끊고 그 시작점에 대한 정보도 참조해서 인코더+디코더정보있는 수적인 정보를통해서 디코더 다음 출력들도 하나하나 내는 것.)

디코더는 이런 식으로 기본적으로 다음에 올 단어를 예측하고, 그 예측한 단어를 다음 시점의 RNN 셀의 입력으로 넣는 행위를 반복합니다. 이 행위는 문장의 끝을 의미하는 심볼인 \<eos>가 (end of sequence) 다음 단어로 예측될 때까지 반복됩니다. **지금 설명하는 것은 테스트 과정** 동안의 이야기입니다.

(만약 eof 없이 계속 생성하면? => 실제 분야에서도 자주 발생. 그래서 최대길이 지정, 빔서치탐색(그리디 대신 :확률높은애 고르는건 잘못된길가면 오류율이 높아져서.), 반복 루프 방지를 위해 여러 기법을 적용한다고 함.)


### 2) 훈련 단계와 교사 강요

seq2seq는 훈련 과정과 테스트 과정(또는 실제 번역기를 사람이 쓸 때)의 작동 방식이 조금 다릅니다. 훈련 과정에서는 디코더에게 인코더가 보낸 컨텍스트 벡터와 실제 정답인 상황인 \<sos> je suis étudiant를 입력 받았을 때, je suis étudiant \<eos>가 나와야 된다고 정답을 알려주면서 훈련합니다. 이를 **교사 강요(teacher forcing)** 라고 합니다.

반면 테스트 과정에서는 앞서 설명한 과정과 같이 디코더는 오직 ==컨텍스트 벡터와 \<go>만을(\<go>=\<sos>=**`<bos>`**: Beginning Of Sequence) 입력으로 받은 후에 다음에 올 단어를 예측==하고, 그 단어를 다음 시점의 RNN 셀의 입력으로 넣는 행위를 반복합니다.


### 3. 임베딩 층(Embedding layer) - 이미 배운 내용

![](https://static.wikidocs.net/images/page/24996/%EB%8B%A8%EC%96%B4%ED%86%A0%ED%81%B0%EB%93%A4%EC%9D%B4.PNG)

기계는 텍스트보다 숫자를 잘 처리합니다. 그리고 자연어 처리에서 텍스트를 벡터로 바꾸는 방법으로 워드 임베딩(9챕터 참고 + 12챕터참고 : [[12 Embedding#^222324]] 복습하면 잘 기억날 거임. Word2Vec:CBOW/skip-gram 요런 식으로 pre-trained 임베딩을 불러올 수도 있고 Embedding : torch.nn class design 으로 자동 역전파 학습해서 임베딩 시킬 수도 있고 ) 이 사용된다고 설명한 바 있습니다. 

즉, seq2seq에서 사용되는 모든 단어들은 워드 임베딩을 통해 임베딩 벡터로서 표현된 임베딩 벡터입니다. 

위 그림은 모든 단어에 대해서 임베딩 과정을 거치게 하는 단계인 ==임베딩 층(embedding layer)==의 모습을 보여줍니다.

![](https://static.wikidocs.net/images/page/24996/%EC%9E%84%EB%B2%A0%EB%94%A9%EB%B2%A1%ED%84%B0.PNG)

예를 들어 I, am, a, student라는 단어들에 대한 임베딩 벡터는 위와 같은 모습을 가집니다. 여기서는 그림으로 표현하고자 사이즈를 4로 하였지만, << 보통 실제 임베딩 벡터는 수백 개의 차원을 가질 수 있습니다 >>. (단어 feature 가 많을수록 적합도가 높아짐! 당연한 이야기. 물론 단어 사전의 길이 등도 생각해야 하긴 함) 

이제 RNN 셀에 대해서 확대해보겠습니다.


### 4. RNN 셀 - 이미 배운 내용

이미 RNN에 대해서 배운 적이 있지만, 다시 복습을 해보겠습니다. 하나의 RNN 셀은 각 시점(time step)마다 두 개의 입력을 받습니다.

![](https://static.wikidocs.net/images/page/24996/rnn%EA%B7%BC%ED%99%A9.PNG)

현재 시점(time step)을 t라고 할 때, RNN 셀은 t-1에서의 은닉 상태와 t에서의 입력 벡터를 입력으로 받고, t에서의 은닉 상태를 만듭니다. 

이때 t에서의 은닉 상태는 바로 위에 또 다른 은닉층이나 출력층이 존재할 경우에는 위의 층으로 보내거나, 필요없으면 값을 무시할 수 있습니다. 
(망각 게이트로 조절하는 것.)

그리고 RNN 셀은 다음 시점에 해당하는 t+1의 RNN 셀의 입력으로 현재 t에서의 은닉 상태를 입력으로 보냅니다.

RNN 챕터에서도 언급했지만, 이런 구조에서 현재 시점 t에서의 은닉 상태는 과거 시점의 동일한 RNN 셀에서의 모든 은닉 상태의 값들의 영향을 누적해서 받아온 값이라고 할 수 있습니다. (이게 RNN구조의 특징 그 자체니까)

그렇기 때문에 앞서 우리가 언급했던 ==컨텍스트 벡터는 사실 인코더에서의 마지막 RNN 셀의 은닉 상태값==을 말하는 것이며, 이는 << 입력 문장의 모든 단어 토큰들의 정보를 요약해서 담고있다 >> 고 할 수 있습니다.
(== RNN 원리. 물론 바닐라 말고 개선으로 써야함..)

### 5. 디코더

디코더는 인코더의 마지막 RNN 셀의 은닉 상태인 컨텍스트 벡터를 첫번째 은닉 상태의 값으로 사용합니다. 

디코더의 첫번째 RNN 셀은 이 첫번째 은닉 상태의 값과, 현재 t에서의 입력값인 ==\<sos>로부터, 다음에 등장할 단어를 예측==합니다. 그리고 이 예측된 단어는 다음 시점인 t+1 RNN에서의 입력값이 되고, 이 t+1에서의 RNN 또한 이 입력값과 t에서의 은닉 상태로부터 t+1에서의 출력 벡터. 즉, 또 다시 다음에 등장할 단어를 예측하게 될 것입니다. 

이제 디코더가 다음에 등장할 단어를 예측하는 부분을 확대해보도록 하겠습니다.

![](https://static.wikidocs.net/images/page/24996/decodernextwordprediction.PNG)

출력 단어로 나올 수 있는 단어들은 다양한 단어들이 있습니다. 

seq2seq 모델은 선택될 수 있는 모든 단어들로부터 하나의 단어를 골라서 예측해야 합니다. 이를 예측하기 위해서 쓸 수 있는 함수로는 뭐가 있을까요? 바로 소프트맥스 함수입니다. 

디코더에서 각 시점(time step)의 RNN 셀에서 출력 벡터가 나오면, 해당 벡터는 소프트맥스 함수를 통해 출력 시퀀스의 각 단어별 확률값을 반환하고, 디코더는 출력 단어를 결정합니다.
(이래서 확률적인 언어 생성기라는 것!! 물론 RNN이 트랜스포머랑은 많이 다르긴함. 
seq2seq 위상은? : 트랜스포머의 뿌리이자 핵심으로 남아있는 정도. 개념적으로... RNN기반 seq2seq 위상은 좀 낮아졌지만, 개념 자체는 그대로 T5, BART-인코더디코더구조, GPT 계열 모델들이 그래도 계승하고 있음. 구현체가 달라졌을뿐임 : RNN, LSTM 기반이 아니라 어텐션!)

## 3. 다양한 변형들

가장 기본적인 seq2seq에 대해서 배워보았습니다. 사실 seq2seq는 어떻게 구현하느냐에 따라서 충분히 더 복잡해질 수 있습니다. 

컨텍스트 벡터를 디코더의 초기 은닉 상태로만 사용할 수도 있고, 거기서 더 나아가 컨텍스트 벡터를 디코더가 단어를 예측하는 매 시점마다 하나의 입력으로 사용할 수도 있으며 거기서 더 나아가면 ==어텐션 메커니즘==이라는 방법을 통해 지금 알고있는 컨텍스트 벡터보다 << 더욱 문맥을 반영할 수 있는 컨텍스트 벡터를 구하여 매 시점마다 하나의 입력으로 사용할 수도 있습니다. >> 어텐션 메커니즘에 대해서는 다음 챕터에서 배웁니다.




# 16-02 Seq2Seq를 이용한 번역기 구현하기

seq2seq를 이용해서 기계 번역기를 만들어보겠습니다. 

실제 서비스에 사용되는 번역기는 뒤의 챕터에서 배우게 될 ==어텐션== 메커니즘을 사용해야 하고, 
최소 ==수백만== 개의 데이터가 필요합니다. 하지만 그럼에도 번역기를 만드는 간단한 토이 프로젝트를 사용해서 seq2seq 구조와 인코더와 디코더의 역할을 이해할 수 있습니다.

## 1. 데이터 로드 및 전처리

실제 성능이 좋은 기계 번역기를 구현하려면 방대한 데이터가 필요하므로 여기서는 seq2seq를 간단히 실습해보는 수준의 간단한 기계 번역기를 구현해보겠습니다. 

기계 번역기를 훈련시키기 위해서는 훈련 데이터로 병렬 코퍼스(parallel corpus)가 필요합니다. 병렬 코퍼스란, 두 개 이상의 언어가 병렬적으로 구성된 코퍼스를 의미합니다. (seq, seq...)

링크 : http://www.manythings.org/anki

이번 실습에서는 프랑스어-영어 병렬 코퍼스인 fra-eng.zip 파일을 사용합니다. 

위의 링크에서 해당 파일을 다운받은 후 압축을 풀면 fra.txt라는 파일을 얻을 수 있는데 
해당 파일을이 실습에서 사용합니다.

병렬 코퍼스 데이터에 대해서 이해해봅시다. 병렬 데이터라고 하면 앞서 수행한 태깅 작업 챕터의 개체명 인식과 같은 데이터를 생각할 수 있지만, 앞서 수행한 태깅 작업의 병렬 데이터와 seq2seq가 사용하는 병렬 데이터는 성격이 다릅니다. 

태깅 작업의 병렬 데이터는 쌍이 되는 데이터와 레이블이 길이가 동일하였으나 여기서는 쌍이 된다고 해서 반드시 길이가 같지는 않습니다. (전자는 각 seq 요소마다 pair일수밖에없고 후자는 번역이니까 당연히 일대일대응이 안될수있음.)

실제 번역기를 생각해보면 구글 번역기에 '나는 학생이다.'라는 토큰의 개수가 2인 문장을 넣었을 때 'I am a student.'라는 토큰의 개수가 4인 문장이 나오는 것과 같은 이치입니다. 

seq2seq는 기본적으로 입력 시퀀스와 출력 시퀀스의 길이가 다를 수 있다고 가정합니다. 
지금 구현 예제는 기계 번역기이지만 seq2seq로 구현할 수 있는 또 다른 예제인 챗봇을 만든다고 가정해보면, 대답의 길이가 질문의 길이와 항상 똑같아야 한다고하면 그 또한 이상합니다. 

여기서 사용할 fra.txt 데이터는 아래와 같이 왼쪽의 영어 문장과 오른쪽의 프랑스어 문장 사이에 탭으로 구분되는 형식이 하나의 샘플입니다.

```vbnet
Watch me.           Regardez-moi !
```

데이터는 위와 동일한 형식의 약 19만개의 병렬 문장 샘플을 포함하고 있습니다. 
데이터를 읽고 전처리를 진행해보겠습니다. 

앞으로의 코드에서 src는 source의 줄임말로 입력 문장을 나타내며, 
tar는 target의 줄임말로 번역하고자 하는 문장을 나타냅니다.

```python
import re
import os
import unicodedata
import urllib3
import zipfile
import shutil
import numpy as np
import pandas as pd
import torch
from collections import Counter
from tqdm import tqdm
from torch.utils.data import DataLoader, TensorDataset
```

이번 실습에서는 약 19만개의 데이터 중 33,000개의 샘플만을 사용할 예정입니다.

```python
num_samples = 33000
```

fra-eng.zip 파일을 다운로드하고 압축을 풀겠습니다.

```python
!wget -c http://www.manythings.org/anki/fra-eng.zip && unzip -o fra-eng.zip
```

전처리 함수들을 구현합니다. 
구두점 등을 제거하거나 단어와 구분해주기 위한 전처리입니다.

```python
def unicode_to_ascii(s):
  # 프랑스어 악센트(accent) 삭제
  # 예시 : 'déjà diné' -> deja dine
  return ''.join(c for c in unicodedata.normalize('NFD', s) if unicodedata.category(c) != 'Mn')
```

```python
def preprocess_sentence(sent):
  # 악센트 삭제 함수 호출
  sent = unicode_to_ascii(sent.lower())

  # 단어와 구두점 사이에 공백을 만듭니다.
  # Ex) "he is a boy." => "he is a boy ."  
  sent = re.sub(r"([?.!,¿])", r" \1", sent)

  # (a-z, A-Z, ".", "?", "!", ",") 이들을 제외하고는 전부 공백으로 변환합니다.
  sent = re.sub(r"[^a-zA-Z!.?]+", r" ", sent)

  # 다수 개의 공백을 하나의 공백으로 치환
  sent = re.sub(r"\s+", " ", sent)
  return sent
```

### `\1` (첫 번째 캡처 그룹)

- 정규표현식으로 찾은 문자열 중 첫 번째 괄호 `()`로 묶은 텍스트(Group 1)를 가져와 대입하라는 의미입니다.-
    
- `r" \1"`은 "첫 번째 괄호에서 찾은 텍스트 앞에 공백(띄어쓰기) 한 칸을 붙여서 치환하라"는 명령어입니다.



```python
def load_preprocessed_data():
  encoder_input, decoder_input, decoder_target = [], [], []

  with open("fra.txt", "r") as lines:
    for i, line in enumerate(lines):
      # source 데이터와 target 데이터 분리
      src_line, tar_line, _ = line.strip().split('\t')

      # source 데이터 전처리
      src_line = [w for w in preprocess_sentence(src_line).split()]

      # target 데이터 전처리
      tar_line = preprocess_sentence(tar_line)
      tar_line_in = [w for w in ("<sos> " + tar_line).split()]
      tar_line_out = [w for w in (tar_line + " <eos>").split()]

      encoder_input.append(src_line)
      decoder_input.append(tar_line_in)
      decoder_target.append(tar_line_out)

      if i == num_samples - 1:
        break

  return encoder_input, decoder_input, decoder_target
```

구현한 전처리 함수들을 임의의 문장을 입력으로 테스트해봅시다.

```python
# 전처리 테스트
en_sent = u"Have you had dinner?"
fr_sent = u"Avez-vous déjà diné?"

print('전처리 전 영어 문장 :', en_sent)
print('전처리 후 영어 문장 :',preprocess_sentence(en_sent))
print('전처리 전 프랑스어 문장 :', fr_sent)
print('전처리 후 프랑스어 문장 :', preprocess_sentence(fr_sent))
```

```python
전처리 전 영어 문장 : Have you had dinner?
전처리 후 영어 문장 : have you had dinner ?
전처리 전 프랑스어 문장 : Avez-vous déjà diné?
전처리 후 프랑스어 문장 : avez vous deja dine ?
```

전체 데이터에서 33,000개의 샘플에 대해서 전처리를 수행합니다. 

또한 훈련 과정에서 교사 강요(Teacher Forcing)를 사용할 예정이므로, 훈련 시 사용할 디코더의 입력 시퀀스와 실제값. 즉, 레이블에 해당되는 출력 시퀀스를 따로 분리하여 저장합니다. 

입력 시퀀스에는 시작을 의미하는 토큰인 `<sos>`를 추가하고, 출력 시퀀스에는 종료를 의미하는 토큰인 `<eos>`를 추가합니다. 이렇게 얻은 3개의 데이터셋 인코더의 입력, 디코더의 입력, 디코더의 레이블을 상위 5개 샘플만 출력해봅시다.

```python
sents_en_in, sents_fra_in, sents_fra_out = load_preprocessed_data()
print('인코더의 입력 :',sents_en_in[:5])
print('디코더의 입력 :',sents_fra_in[:5])
print('디코더의 레이블 :',sents_fra_out[:5])
```

```python
인코더의 입력 : [['go', '.'], ['go', '.'], ['go', '.'], ['hi', '.'], ['hi', '.']]
디코더의 입력 : [['<sos>', 'va', '!'], ['<sos>', 'marche', '.'], ['<sos>', 'bouge', '!'], ['<sos>', 'salut', '!'], ['<sos>', 'salut', '.']]
디코더의 레이블 : [['va', '!', '<eos>'], ['marche', '.', '<eos>'], ['bouge', '!', '<eos>'], ['salut', '!', '<eos>'], ['salut', '.', '<eos>']]
```

모델을 설계하기 전 의아한 점이 있을 수 있습니다. 

현재 시점의 디코더 셀의 입력은 오직 이전 디코더 셀의 출력을 입력으로 받는다고 설명하였는데 << 디코더의 입력에 해당하는 데이터인 sents_fra_in이 왜 필요할까요? >> 

훈련 과정에서는 << 이전 시점의 디코더 셀의 출력을 현재 시점의 디코더 셀의 입력으로 넣어주지 않고, 이전 시점의 ==실제값==을 현재 시점의 디코더 셀의 입력값 >> 으로 하는 방법을 사용할 겁니다. 

그 이유는 이전 시점의 디코더 셀의 예측이 틀렸는데 이를 현재 시점의 디코더 셀의 입력으로 사용하면 현재 시점의 디코더 셀의 예측도 잘못될 가능성이 높고 이는 연쇄 작용으로 디코더 전체의 예측을 어렵게 합니다. 

이런 상황이 반복되면 훈련 시간이 느려집니다. 만약 이 상황을 원하지 않는다면 이전 시점의 디코더 셀의 예측값 대신 실제값(label)을 현재 시점의 디코더 셀의 입력으로 사용하는 방법을 사용할 수 있습니다. 이와 같이 RNN의 모든 시점에 대해서 이전 시점의 예측값 대신 실제값을 입력으로 주는 방법을 ==교사 강요==!!라고 합니다.

단어로부터 정수를 얻는 딕셔너리. 즉, 단어 집합(Vocabulary)을 만들어봅시다. 

이를 위한 함수로 build_vocab()을 구현합니다. 
build_vocab은 입력된 데이터로부터 단어의 << 등장 빈도순으로 정렬 후에 등장 빈도가 높은 순서일 수록 낮은 정수 >> 를 부여합니다. 이때, 패딩 토큰을 위한 `<PAD>` 토큰은 0번, OOV에 대응하기 위한 `<UNK>` 토큰은 1번에 할당합니다. 

이렇게 되면 빈도수가 가장 높은 단어는 정수가 2번, 빈도수가 두번 째로 많은 단어는 정수 3번이 할당됩니다.

```python
def build_vocab(sents):
  word_list = []

  for sent in sents:
      for word in sent:
        word_list.append(word)

  # 각 단어별 등장 빈도를 계산하여 등장 빈도가 높은 순서로 정렬
  word_counts = Counter(word_list)
  vocab = sorted(word_counts, key=word_counts.get, reverse=True)

  word_to_index = {}
  word_to_index['<PAD>'] = 0
  word_to_index['<UNK>'] = 1

  # 등장 빈도가 높은 단어일수록 낮은 정수를 부여
  for index, word in enumerate(vocab) :
    word_to_index[word] = index + 2

  return word_to_index
```

영어를 위한 단어 집합 src_vocab과 프랑스어를 이용한 단어 집합 tar_vocab를 만들어봅시다. 

구현 방식에 따라서는 하나의 단어 집합으로 만들어도 상관없으며 이는 선택의 차이입니다.

```python
src_vocab = build_vocab(sents_en_in)
tar_vocab = build_vocab(sents_fra_in + sents_fra_out)
# sents_fra_out 이 더해지는 이유 == 교사 강요 사용하기 위함!

src_vocab_size = len(src_vocab)
tar_vocab_size = len(tar_vocab)
print("영어 단어 집합의 크기 : {:d}, 프랑스어 단어 집합의 크기 : {:d}".format(src_vocab_size, tar_vocab_size))
```

```yaml
영어 단어 집합의 크기 : 4517, 프랑스어 단어 집합의 크기 : 7908
```

정수로부터 단어를 얻는 딕셔너리를 각각 만들어줍니다. 

이들은 훈련을 마치고 예측값과 실제값을 비교하는 단계에서 사용됩니다.

```python
index_to_src = {v: k for k, v in src_vocab.items()}
index_to_tar = {v: k for k, v in tar_vocab.items()}

def texts_to_sequences(sents, word_to_index):
  encoded_X_data = []
  for sent in tqdm(sents):
    index_sequences = []
    for word in sent:
      try:
          index_sequences.append(word_to_index[word])
      except KeyError:
          index_sequences.append(word_to_index['<UNK>'])
    encoded_X_data.append(index_sequences)
  return encoded_X_data
```

```python
encoder_input = texts_to_sequences(sents_en_in, src_vocab)
decoder_input = texts_to_sequences(sents_fra_in, tar_vocab)
decoder_target = texts_to_sequences(sents_fra_out, tar_vocab)
```

```python
# 상위 5개의 샘플에 대해서 정수 인코딩 전, 후 문장 출력
# 인코더 입력이므로 <sos>나 <eos>가 없음
for i, (item1, item2) in zip(range(5), zip(sents_en_in, encoder_input)):
    print(f"Index: {i}, 정수 인코딩 전: {item1}, 정수 인코딩 후: {item2}")
```

```python
Index: 0, 정수 인코딩 전: ['go', '.'], 정수 인코딩 후: [28, 2]
Index: 1, 정수 인코딩 전: ['go', '.'], 정수 인코딩 후: [28, 2]
Index: 2, 정수 인코딩 전: ['go', '.'], 정수 인코딩 후: [28, 2]
Index: 3, 정수 인코딩 전: ['go', '.'], 정수 인코딩 후: [28, 2]
Index: 4, 정수 인코딩 전: ['hi', '.'], 정수 인코딩 후: [746, 2]
```

```python
def pad_sequences(sentences, max_len=None):
    # 최대 길이 값이 주어지지 않을 경우 데이터 내 최대 길이로 패딩
    if max_len is None:
        max_len = max([len(sentence) for sentence in sentences])

    features = np.zeros((len(sentences), max_len), dtype=int)
    for index, sentence in enumerate(sentences):
        if len(sentence) != 0:
            features[index, :len(sentence)] = np.array(sentence)[:max_len]
    return features
```

```python
encoder_input = pad_sequences(encoder_input)
decoder_input = pad_sequences(decoder_input)
decoder_target = pad_sequences(decoder_target)
```

데이터의 크기(shape)를 확인합니다.

```python
print('인코더의 입력의 크기(shape) :',encoder_input.shape)
print('디코더의 입력의 크기(shape) :',decoder_input.shape)
print('디코더의 레이블의 크기(shape) :',decoder_target.shape)
```

```python
인코더의 입력의 크기(shape) : (33000, 7)
디코더의 입력의 크기(shape) : (33000, 16)
디코더의 레이블의 크기(shape) : (33000, 16)
```

테스트 데이터를 분리하기 전 데이터를 섞어줍니다. 
이를 위해서 순서가 섞인 정수 시퀀스 리스트를 만듭니다.

```python
indices = np.arange(encoder_input.shape[0])  # indices == ndarray
np.random.shuffle(indices)
print('랜덤 시퀀스 :',indices)
```

```python
랜덤 시퀀스 : [29443 12274 30297 ... 24517  9984 32323]
```

이를 데이터셋의 순서로 지정해주면 샘플들이 기존 순서와 다른 순서로 섞이게 됩니다.

```python
encoder_input = encoder_input[indices]
decoder_input = decoder_input[indices]
decoder_target = decoder_target[indices]
```

임의로 30,997번째 샘플을 출력해봅시다. 이때 decoder_input과 decoder_target은 데이터의 구조상으로 앞에 붙은 `<sos>` 토큰과 뒤에 붙은 `<eos>`을 제외하면 동일한 시퀀스를 가져야 합니다.

```python
print([index_to_src[word] for word in encoder_input[30997]])
print([index_to_tar[word] for word in decoder_input[30997]])
print([index_to_tar[word] for word in decoder_target[30997]])
```

```python
['give', 'me', 'the', 'phone', '.', '<PAD>', '<PAD>']
['<sos>', 'donne', 'moi', 'le', 'telephone', '.', '<PAD>', '<PAD>', '<PAD>', '<PAD>', '<PAD>', '<PAD>', '<PAD>', '<PAD>', '<PAD>', '<PAD>']
['donne', 'moi', 'le', 'telephone', '.', '<eos>', '<PAD>', '<PAD>', '<PAD>', '<PAD>', '<PAD>', '<PAD>', '<PAD>', '<PAD>', '<PAD>', '<PAD>']
```


# 여기부터 시작하기


33,000개의 10%에 해당되는 3,300개의 데이터를 테스트 데이터로 사용합니다.

```python
n_of_val = int(33000*0.1)
print('검증 데이터의 개수 :',n_of_val)
```

```python
검증 데이터의 개수 : 3300
```

```python
encoder_input_train = encoder_input[:-n_of_val]
decoder_input_train = decoder_input[:-n_of_val]
decoder_target_train = decoder_target[:-n_of_val]

encoder_input_test = encoder_input[-n_of_val:]
decoder_input_test = decoder_input[-n_of_val:]
decoder_target_test = decoder_target[-n_of_val:]
```

```python
array([ 74,   4, 438,   5,   0,   0,   0])
array([   3,   80,   19, 2172,    7,    0,    0,    0,    0,    0,    0,
          0,    0,    0,    0,    0])
array([  80,   19, 2172,    7,    4,    0,    0,    0,    0,    0,    0,
          0,    0,    0,    0,    0])
```

훈련 데이터와 테스트 데이터의 크기(shape)를 출력해봅시다.

```python
print('훈련 source 데이터의 크기 :',encoder_input_train.shape)
print('훈련 target 데이터의 크기 :',decoder_input_train.shape)
print('훈련 target 레이블의 크기 :',decoder_target_train.shape)
print('테스트 source 데이터의 크기 :',encoder_input_test.shape)
print('테스트 target 데이터의 크기 :',decoder_input_test.shape)
print('테스트 target 레이블의 크기 :',decoder_target_test.shape)
```

```python
훈련 source 데이터의 크기 : (29700, 7)
훈련 target 데이터의 크기 : (29700, 16)
훈련 target 레이블의 크기 : (29700, 16)
테스트 source 데이터의 크기 : (3300, 7)
테스트 target 데이터의 크기 : (3300, 16)
테스트 target 레이블의 크기 : (3300, 16)
```

## 2. 기계 번역기 만들기

```python
import torch
import torch.nn as nn
import torch.optim as optim

embedding_dim = 256
hidden_units = 256

class Encoder(nn.Module):
    def __init__(self, src_vocab_size, embedding_dim, hidden_units):
        super(Encoder, self).__init__()
        self.embedding = nn.Embedding(src_vocab_size, embedding_dim, padding_idx=0)
        self.lstm = nn.LSTM(embedding_dim, hidden_units, batch_first=True)

    def forward(self, x):
        # x.shape == (batch_size, seq_len, embedding_dim)
        x = self.embedding(x)
        # hidden.shape == (1, batch_size, hidden_units), cell.shape == (1, batch_size, hidden_units)
        _, (hidden, cell) = self.lstm(x)
        # 인코더의 출력은 hidden state, cell state
        return hidden, cell

class Decoder(nn.Module):
    def __init__(self, tar_vocab_size, embedding_dim, hidden_units):
        super(Decoder, self).__init__()
        self.embedding = nn.Embedding(tar_vocab_size, embedding_dim, padding_idx=0)
        self.lstm = nn.LSTM(embedding_dim, hidden_units, batch_first=True)
        self.fc = nn.Linear(hidden_units, tar_vocab_size)

    def forward(self, x, hidden, cell):

        # x.shape == (batch_size, seq_len, embedding_dim)
        x = self.embedding(x)

        # 디코더의 LSTM으로 인코더의 hidden state, cell state를 전달.
        # output.shape == (batch_size, seq_len, hidden_units)
        # hidden.shape == (1, batch_size, hidden_units)
        # cell.shape == (1, batch_size, hidden_units)
        output, (hidden, cell) = self.lstm(x, (hidden, cell))

        # output.shape: (batch_size, seq_len, tar_vocab_size)
        output = self.fc(output)

        # 디코더의 출력은 예측값, hidden state, cell state
        return output, hidden, cell

class Seq2Seq(nn.Module):
    def __init__(self, encoder, decoder):
        super(Seq2Seq, self).__init__()
        self.encoder = encoder
        self.decoder = decoder

    def forward(self, src, trg):
        hidden, cell = self.encoder(src)

        # 훈련 중에는 디코더의 출력 중 오직 output만 사용한다.
        output, _, _ = self.decoder(trg, hidden, cell)
        return output

encoder = Encoder(src_vocab_size, embedding_dim, hidden_units)
decoder = Decoder(tar_vocab_size, embedding_dim, hidden_units)
model = Seq2Seq(encoder, decoder)

loss_function = nn.CrossEntropyLoss(ignore_index=0)
optimizer = optim.Adam(model.parameters())
```

모델의 구조를 출력해봅시다.

```python
print(model)
```

```python
Seq2Seq(
  (encoder): Encoder(
    (embedding): Embedding(4517, 256, padding_idx=0)
    (lstm): LSTM(256, 256, batch_first=True)
  )
  (decoder): Decoder(
    (embedding): Embedding(7908, 256, padding_idx=0)
    (lstm): LSTM(256, 256, batch_first=True)
    (fc): Linear(in_features=256, out_features=7908, bias=True)
  )
)
```

Encoder 클래스는 입력 시퀀스를 받아 해당 시퀀스의 정보를 압축하여 context vector로 변환하는 역할을 합니다. Encoder는 임베딩 레이어와 LSTM 레이어로 구성되어 있습니다. 임베딩 레이어는 입력 시퀀스의 각 토큰을 고정 크기의 벡터로 변환하고, LSTM 레이어는 시퀀스의 순서 정보를 고려하여 해당 시퀀스를 요약합니다. Encoder의 forward 메서드는 입력 시퀀스를 받아 LSTM의 hidden state와 cell state를 반환합니다.

Decoder 클래스는 Encoder에서 생성된 context vector(인코더의 마지막 은닉 상태)를 기반으로 출력 시퀀스를 생성하는 역할을 합니다. Decoder 또한 임베딩 레이어와 LSTM 레이어로 구성되어 있습니다. Decoder의 LSTM은 Encoder에서 전달받은 hidden state와 cell state를 초기 상태로 사용하여 출력 시퀀스를 생성합니다. 생성된 출력 시퀀스는 fully connected 레이어를 통과하여 각 시점의 출력 토큰에 대한 확률 분포를 얻습니다. Decoder의 forward 메서드는 입력 시퀀스, hidden state, cell state를 받아 출력 시퀀스, 업데이트된 hidden state와 cell state를 반환합니다.

Seq2Seq 클래스는 Encoder와 Decoder를 결합하여 전체 모델을 구성합니다. Seq2Seq 모델의 forward 메서드는 입력 시퀀스(src)와 출력 시퀀스(trg)를 받아 Encoder에서 생성된 은닉 상태(hidden state)와 셀 상태(cell state)를 Decoder로 전달하고, Decoder에서 생성된 출력 시퀀스를 반환합니다.

Seq2Seq의 디코더는 기본적으로 각 시점마다 다중 클래스 분류 문제를 풀고있습니다. 매 시점마다 프랑스어 단어 집합의 크기(tar_vocab_size)의 선택지에서 단어를 1개 선택하여 이를 이번 시점에서 예측한 단어로 택합니다. 다중 클래스 분류 문제이므로 모델 학습을 위해 CrossEntropyLoss 함수를 사용하여 손실을 계산하고, Adam 옵티마이저를 사용하여 모델의 파라미터를 최적화합니다. CrossEntropyLoss의 ignore_index 파라미터는 패딩 토큰에 해당하는 인덱스를 무시하도록 설정합니다.

이 코드에서는 임베딩 차원(embedding_dim)과 LSTM의 은닉 상태 크기(hidden_units)를 256으로 설정하였습니다. Encoder와 Decoder의 인스턴스를 생성한 후, 이를 Seq2Seq 모델로 결합하여 전체 모델을 구성합니다. 이렇게 구현된 Seq2Seq 모델은 기계 번역이나 챗봇과 같은 시퀀스-투-시퀀스 문제를 해결하는 데 사용될 수 있습니다. 입력 시퀀스가 Encoder를 통과하여 context vector로 변환되고, 이를 기반으로 Decoder에서 출력 시퀀스를 생성합니다. 모델의 학습은 입력 시퀀스와 해당하는 출력 시퀀스의 쌍을 사용하여 이루어집니다.

```python
def evaluation(model, dataloader, loss_function, device):
    model.eval()
    total_loss = 0.0
    total_correct = 0
    total_count = 0

    with torch.no_grad():
        for encoder_inputs, decoder_inputs, decoder_targets in dataloader:
            encoder_inputs = encoder_inputs.to(device)
            decoder_inputs = decoder_inputs.to(device)
            decoder_targets = decoder_targets.to(device)

            # 순방향 전파
            # outputs.shape == (batch_size, seq_len, tar_vocab_size)
            outputs = model(encoder_inputs, decoder_inputs)

            # 손실 계산
            # outputs.view(-1, outputs.size(-1))의 shape는 (batch_size * seq_len, tar_vocab_size)
            # decoder_targets.view(-1)의 shape는 (batch_size * seq_len)
            loss = loss_function(outputs.view(-1, outputs.size(-1)), decoder_targets.view(-1))
            total_loss += loss.item()

            # 정확도 계산 (패딩 토큰 제외)
            mask = decoder_targets != 0
            total_correct += ((outputs.argmax(dim=-1) == decoder_targets) * mask).sum().item()
            total_count += mask.sum().item()

    return total_loss / len(dataloader), total_correct / total_count
```

평가 함수의 입력으로는 평가할 모델(model), 데이터로더(dataloader), 손실 함수(loss_function), 그리고 모델을 실행할 디바이스(device)가 주어집니다. 먼저, 모델을 평가 모드로 설정합니다. 이는 model.eval()을 호출하여 이루어지며, 드롭아웃(dropout)이나 배치 정규화(batch normalization)와 같은 층의 동작을 조정합니다.

다음으로, 총 손실(total_loss), 총 정확도(total_correct), 그리고 총 토큰 수(total_count)를 초기화합니다. 이 변수들은 전체 데이터셋에 대한 평가 결과를 누적하는 데 사용됩니다.

그 후, torch.no_grad() 컨텍스트 매니저 내에서 데이터로더를 순회합니다. 이는 기울기(gradient) 계산을 비활성화하여 메모리 사용량을 줄이고 평가 속도를 향상시킵니다.

각 배치(batch)에 대해, 인코더 입력(encoder_inputs), 디코더 입력(decoder_inputs), 그리고 디코더 타겟(decoder_targets)을 디바이스로 이동시킵니다. 그런 다음, 모델에 인코더 입력과 디코더 입력을 전달하여 순방향 전파(forward pass)를 수행합니다. 이를 통해 모델의 출력(outputs)을 얻습니다. 그 후, 출력과 디코더 타겟을 사용하여 손실을 계산합니다. 이때, 출력과 타겟의 차원을 조정하기 위해 view() 함수를 사용합니다. 계산된 손실을 총 손실에 누적합니다.

정확도를 계산하기 위해, 패딩 토큰(padding token)을 제외한 실제 토큰들에 대해서만 고려합니다. 이를 위해 디코더 타겟이 0이 아닌 위치에 대한 마스크(mask)를 생성합니다. 출력의 argmax를 취하여 예측된 토큰을 얻고, 이를 디코더 타겟과 비교하여 정확한 예측 수를 계산합니다. 정확한 예측 수와 전체 토큰 수를 누적합니다.

마지막으로, 평균 손실(average loss)과 정확도(accuracy)를 계산하여 반환합니다. 평균 손실은 총 손실을 데이터로더의 배치 수로 나누어 계산하고, 정확도는 총 정확도를 총 토큰 수로 나누어 계산합니다. 이 평가 함수를 사용하여 모델의 성능을 측정할 수 있습니다. 평균 손실이 낮을수록, 그리고 정확도가 높을수록 모델의 성능이 좋다는 것을 나타냅니다. 이를 통해 모델의 학습 진행 상황을 모니터링하고, 최적의 모델을 선택할 수 있습니다.

```python
encoder_input_train_tensor = torch.tensor(encoder_input_train, dtype=torch.long)
decoder_input_train_tensor = torch.tensor(decoder_input_train, dtype=torch.long)
decoder_target_train_tensor = torch.tensor(decoder_target_train, dtype=torch.long)

encoder_input_test_tensor = torch.tensor(encoder_input_test, dtype=torch.long)
decoder_input_test_tensor = torch.tensor(decoder_input_test, dtype=torch.long)
decoder_target_test_tensor = torch.tensor(decoder_target_test, dtype=torch.long)

# 데이터셋 및 데이터로더 생성
batch_size = 128

train_dataset = TensorDataset(encoder_input_train_tensor, decoder_input_train_tensor, decoder_target_train_tensor)
train_dataloader = DataLoader(train_dataset, batch_size=batch_size, shuffle=True)

valid_dataset = TensorDataset(encoder_input_test_tensor, decoder_input_test_tensor, decoder_target_test_tensor)
valid_dataloader = DataLoader(valid_dataset, batch_size=batch_size, shuffle=False)
```

먼저, 학습 데이터셋과 테스트 데이터셋의 인코더 입력, 디코더 입력, 디코더 타겟을 PyTorch 텐서로 변환합니다. 이때 데이터 타입은 `torch.long`으로 설정됩니다. 다음으로, PyTorch의 `TensorDataset`을 사용하여 학습 데이터셋과 테스트 데이터셋을 생성합니다. `TensorDataset`은 텐서들을 묶어서 데이터셋으로 만들어주는 역할을 합니다. 그 후, PyTorch의 `DataLoader`를 사용하여 학습 데이터로더와 테스트 데이터로더를 생성합니다. `DataLoader`는 데이터셋을 배치 크기 단위로 나누어 모델에 입력할 수 있도록 해줍니다. 배치 크기는 128로 설정되었으며, 학습 데이터로더는 `shuffle=True`로 설정하여 데이터를 에포크마다 랜덤하게 섞어주고, 테스트 데이터로더는 shuffle=False로 설정하여 데이터의 순서를 유지합니다.

```python
# 학습 설정
num_epochs = 30
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
model.to(device)
```

학습 설정에서는 학습 에포크 수를 30으로 설정하고, 학습에 사용할 디바이스를 설정합니다. GPU가 사용 가능한 경우 "cuda"로 설정하고, 그렇지 않은 경우 "cpu"로 설정합니다. 그 후, model.to(device)를 사용하여 모델을 설정한 디바이스로 이동시킵니다. 이를 통해 모델의 계산을 해당 디바이스에서 수행할 수 있습니다.

이제 모델을 훈련합니다. 128개의 배치 크기(128개씩 데이터를 병렬로 학습)로 총 30 에포크 학습합니다. 검증 데이터로 훈련이 제대로 되고있는지 모니터링하겠습니다.

```python
# Training loop
best_val_loss = float('inf')

for epoch in range(num_epochs):
    # 훈련 모드
    model.train()

    for encoder_inputs, decoder_inputs, decoder_targets in train_dataloader:
        encoder_inputs = encoder_inputs.to(device)
        decoder_inputs = decoder_inputs.to(device)
        decoder_targets = decoder_targets.to(device)

        # 기울기 초기화
        optimizer.zero_grad()

        # 순방향 전파
        # outputs.shape == (batch_size, seq_len, tar_vocab_size)
        outputs = model(encoder_inputs, decoder_inputs)

        # 손실 계산 및 역방향 전파
        # outputs.view(-1, outputs.size(-1))의 shape는 (batch_size * seq_len, tar_vocab_size)
        # decoder_targets.view(-1)의 shape는 (batch_size * seq_len)
        loss = loss_function(outputs.view(-1, outputs.size(-1)), decoder_targets.view(-1))
        loss.backward()

        # 가중치 업데이트
        optimizer.step()

    train_loss, train_acc = evaluation(model, train_dataloader, loss_function, device)
    valid_loss, valid_acc = evaluation(model, valid_dataloader, loss_function, device)

    print(f'Epoch: {epoch+1}/{num_epochs} | Train Loss: {train_loss:.4f} | Train Acc: {train_acc:.4f} | Valid Loss: {valid_loss:.4f} | Valid Acc: {valid_acc:.4f}')

    # 검증 손실이 최소일 때 체크포인트 저장
    if valid_loss < best_val_loss:
        print(f'Validation loss improved from {best_val_loss:.4f} to {valid_loss:.4f}. 체크포인트를 저장합니다.')
        best_val_loss = valid_loss
        torch.save(model.state_dict(), 'best_model_checkpoint.pth')
```

저자의 학습 기록은 다음과 같습니다.

```python
Epoch: 1/30 | Train Loss: 2.9014 | Train Acc: 0.5312 | Valid Loss: 3.0343 | Valid Acc: 0.5257
Validation loss improved from inf to 3.0343. 체크포인트를 저장합니다.
Epoch: 2/30 | Train Loss: 2.2466 | Train Acc: 0.6030 | Valid Loss: 2.5037 | Valid Acc: 0.5886
Validation loss improved from 3.0343 to 2.5037. 체크포인트를 저장합니다.
Epoch: 3/30 | Train Loss: 1.8302 | Train Acc: 0.6487 | Valid Loss: 2.2069 | Valid Acc: 0.6181
Validation loss improved from 2.5037 to 2.2069. 체크포인트를 저장합니다.
Epoch: 4/30 | Train Loss: 1.5223 | Train Acc: 0.6869 | Valid Loss: 2.0138 | Valid Acc: 0.6424
Validation loss improved from 2.2069 to 2.0138. 체크포인트를 저장합니다.
Epoch: 5/30 | Train Loss: 1.2775 | Train Acc: 0.7241 | Valid Loss: 1.8763 | Valid Acc: 0.6582
Validation loss improved from 2.0138 to 1.8763. 체크포인트를 저장합니다.
Epoch: 6/30 | Train Loss: 1.0680 | Train Acc: 0.7643 | Valid Loss: 1.7626 | Valid Acc: 0.6766
Validation loss improved from 1.8763 to 1.7626. 체크포인트를 저장합니다.
Epoch: 7/30 | Train Loss: 0.8900 | Train Acc: 0.7895 | Valid Loss: 1.6930 | Valid Acc: 0.6852
Validation loss improved from 1.7626 to 1.6930. 체크포인트를 저장합니다.
Epoch: 8/30 | Train Loss: 0.7457 | Train Acc: 0.8253 | Valid Loss: 1.6228 | Valid Acc: 0.6969
Validation loss improved from 1.6930 to 1.6228. 체크포인트를 저장합니다.
Epoch: 9/30 | Train Loss: 0.6195 | Train Acc: 0.8557 | Valid Loss: 1.5719 | Valid Acc: 0.7071
Validation loss improved from 1.6228 to 1.5719. 체크포인트를 저장합니다.
Epoch: 10/30 | Train Loss: 0.5193 | Train Acc: 0.8748 | Valid Loss: 1.5415 | Valid Acc: 0.7120
Validation loss improved from 1.5719 to 1.5415. 체크포인트를 저장합니다.
Epoch: 11/30 | Train Loss: 0.4450 | Train Acc: 0.8905 | Valid Loss: 1.5235 | Valid Acc: 0.7165
Validation loss improved from 1.5415 to 1.5235. 체크포인트를 저장합니다.
Epoch: 12/30 | Train Loss: 0.3804 | Train Acc: 0.9010 | Valid Loss: 1.5223 | Valid Acc: 0.7142
Validation loss improved from 1.5235 to 1.5223. 체크포인트를 저장합니다.
Epoch: 13/30 | Train Loss: 0.3334 | Train Acc: 0.9090 | Valid Loss: 1.5129 | Valid Acc: 0.7180
Validation loss improved from 1.5223 to 1.5129. 체크포인트를 저장합니다.
Epoch: 14/30 | Train Loss: 0.2965 | Train Acc: 0.9146 | Valid Loss: 1.5222 | Valid Acc: 0.7213
... 중략 ... 이후 validation_loss는 계속 증가
```

검증 데이터 손실이 가장 최소일 때의 모델을 로드하고 다시 재평가해봅시다.

```python
# 모델 로드
model.load_state_dict(torch.load('best_model_checkpoint.pth'))

# 모델을 device에 올립니다.
model.to(device)

# 검증 데이터에 대한 정확도와 손실 계산
val_loss, val_accuracy = evaluation(model, valid_dataloader, loss_function, device)

print(f'Best model validation loss: {val_loss:.4f}')
print(f'Best model validation accuracy: {val_accuracy:.4f}')
```

```python
Best model validation loss: 1.5129
Best model validation accuracy: 0.7180
```

로드 후 재평가를 진행하였더니, 저장할 당시와 검증 데이터의 손실과 정확도가 동일하므로 저장 및 로드가 원활히 되었습니다. `<sos>`와 `<eos>` 토큰의 정수는 각각 3과 4입니다.

```python
print(tar_vocab['<sos>'])
print(tar_vocab['<eos>'])
```

```python
3
4
```

## 3. seq2seq 기계 번역기 동작시키기

seq2seq는 훈련 과정(교사 강요)과 테스트 과정에서의 동작 방식이 다릅니다. 그래서 테스트 과정을 위해 모델을 다시 설계해주어야 합니다. 특히 디코더를 수정해야 합니다. 이번에는 번역 단계를 위해 모델을 수정하고 동작시켜보겠습니다.

전체적인 번역 단계를 정리하면 아래와 같습니다.

1) 번역하고자 하는 입력 문장이 인코더로 입력되어 인코더의 마지막 시점의 은닉 상태와 셀 상태를 얻습니다. 2) 인코더의 은닉 상태와 셀 상태, 그리고 토큰 `<sos>`를 디코더로 보냅니다. 3) 디코더가 토큰 `<eos>`가 나올 때까지 다음 단어를 예측하는 행동을 반복합니다.

```python
index_to_src = {v: k for k, v in src_vocab.items()}
index_to_tar = {v: k for k, v in tar_vocab.items()}

# 원문의 정수 시퀀스를 텍스트 시퀀스로 변환
def seq_to_src(input_seq):
  sentence = ''
  for encoded_word in input_seq:
    if(encoded_word != 0):
      sentence = sentence + index_to_src[encoded_word] + ' '
  return sentence

# 번역문의 정수 시퀀스를 텍스트 시퀀스로 변환
def seq_to_tar(input_seq):
  sentence = ''
  for encoded_word in input_seq:
    if(encoded_word != 0 and encoded_word != tar_vocab['<sos>'] and encoded_word != tar_vocab['<eos>']):
      sentence = sentence + index_to_tar[encoded_word] + ' '
  return sentence
```

```python
print(encoder_input_test[25])
print(decoder_input_test[25])
print(decoder_target_test[25])
```

```python
array([  4,  22, 931,   2,   0,   0,   0])
array([   3,   19,   36, 2007,    2,    0,    0,    0,    0,    0,    0,
          0,    0,    0,    0,    0])
array([  19,   36, 2007,    2,    4,    0,    0,    0,    0,    0,    0,
          0,    0,    0,    0,    0])
```

decode_sequence() 함수를 봅시다. 테스트 단계에서는 디코더를 매 시점 별로 컨트롤 하게 됩니다. 각 시점을 for문을 통해서 컨트롤하게 되며, 현재 시점의 예측은 다음 시점의 입력으로 사용됩니다. 여기서 사용될 변수는 decoder_input입니다.

```python
def decode_sequence(input_seq, model, src_vocab_size, tar_vocab_size, max_output_len, int_to_src_token, int_to_tar_token):
    encoder_inputs = torch.tensor(input_seq, dtype=torch.long).unsqueeze(0).to(device)

    # 인코더의 초기 상태 설정
    hidden, cell = model.encoder(encoder_inputs)

    # 시작 토큰 <sos>을 디코더의 첫 입력으로 설정
    # unsqueeze(0)는 배치 차원을 추가하기 위함.
    decoder_input = torch.tensor([3], dtype=torch.long).unsqueeze(0).to(device)

    decoded_tokens = []

    # for문을 도는 것 == 디코더의 각 시점
    for _ in range(max_output_len):
        output, hidden, cell = model.decoder(decoder_input, hidden, cell)

        # 소프트맥스 회귀를 수행. 예측 단어의 인덱스
        output_token = output.argmax(dim=-1).item()

        # 종료 토큰 <eos>
        if output_token == 4:
            break

        # 각 시점의 단어(정수)는 decoded_tokens에 누적하였다가 최종 번역 시퀀스로 리턴합니다.
        decoded_tokens.append(output_token)

        # 현재 시점의 예측. 다음 시점의 입력으로 사용된다.
        decoder_input = torch.tensor([output_token], dtype=torch.long).unsqueeze(0).to(device)

    return ' '.join(int_to_tar_token[token] for token in decoded_tokens)
```

결과 확인을 위한 함수를 만듭니다. seq_to_src 함수는 영어 문장에 해당하는 정수 시퀀스를 입력받으면 정수로부터 영어 단어를 리턴하는 index_to_src를 통해 영어 문장으로 변환합니다. seq_to_tar은 프랑스어에 해당하는 정수 시퀀스를 입력받으면 정수로부터 프랑스어 단어를 리턴하는 index_to_tar을 통해 프랑스어 문장으로 변환합니다. 훈련 데이터에 대해서 임의로 선택한 인덱스의 샘플의 결과를 출력해봅시다.

```python
for seq_index in [3, 50, 100, 300, 1001]:
  input_seq = encoder_input_train[seq_index]
  translated_text = decode_sequence(input_seq, model, src_vocab_size, tar_vocab_size, 20, index_to_src, index_to_tar)

  print("입력문장 :",seq_to_src(encoder_input_train[seq_index]))
  print("정답문장 :",seq_to_tar(decoder_input_train[seq_index]))
  print("번역문장 :",translated_text)
  print("-"*50)
```

```python
입력문장 : you re fortunate . 
정답문장 : tu es chanceux . 
번역문장 : tu es chanceuse .
--------------------------------------------------
입력문장 : run for it ! 
정답문장 : taillez vous ! 
번역문장 : sauvez vous !
--------------------------------------------------
입력문장 : pass me the water . 
정답문장 : passe moi l eau . 
번역문장 : passe moi l eau .
--------------------------------------------------
입력문장 : i couldn t fight . 
정답문장 : je ne pourrais pas me battre . 
번역문장 : je ne pourrais pas me battre .
--------------------------------------------------
입력문장 : get real ! 
정답문장 : sois realiste ! 
번역문장 : sois realiste !
--------------------------------------------------
```

테스트 데이터에 대해서 임의로 선택한 인덱스의 샘플의 결과를 출력해봅시다.

```python
for seq_index in [3, 50, 100, 300, 1001]:
  input_seq = encoder_input_test[seq_index]
  translated_text = decode_sequence(input_seq, model, src_vocab_size, tar_vocab_size, 20, index_to_src, index_to_tar)

  print("입력문장 :",seq_to_src(encoder_input_test[seq_index]))
  print("정답문장 :",seq_to_tar(decoder_input_test[seq_index]))
  print("번역문장 :",translated_text)
  print("-"*50)
```

```python
입력문장 : you re good . 
정답문장 : tu es bonne . 
번역문장 : vous etes bon .
--------------------------------------------------
입력문장 : you cheated . 
정답문장 : tu as triche . 
번역문장 : vous avez triche .
--------------------------------------------------
입력문장 : put it there . 
정답문장 : mettez le la . 
번역문장 : mets le la .
--------------------------------------------------
입력문장 : get your gear . 
정답문장 : allez chercher votre materiel ! 
번역문장 : va chercher tes affaires !
--------------------------------------------------
입력문장 : i m a stutterer . 
정답문장 : je suis begue . 
번역문장 : je suis un coureur .
--------------------------------------------------
```

번역기를 통해서 입력 문장과 정답 문장 번역 문장의 실제 내용을 확인해보았습니다.

```python
입력문장 (영어) : you re good .
번역문장 (한국어) : 너 정말 잘하네.

정답문장 (프랑스어) : tu es bonne .
번역문장 (한국어) : 너 정말 잘하네.

번역문장 (프랑스어) : vous etes bon .
번역문장 (한국어) : 당신은 좋습니다.

---

입력문장 (영어) : you cheated .
번역문장 (한국어) : 너 속였어.

정답문장 (프랑스어) : tu as triche .
번역문장 (한국어) : 너 속였어.

번역문장 (프랑스어) : vous avez triche .
번역문장 (한국어) : 당신은 속였어.

---

입력문장 (영어) : put it there .
번역문장 (한국어) : 거기에 놓으세요.

정답문장 (프랑스어) : mettez le la .
번역문장 (한국어) : 거기에 놓으세요.

번역문장 (프랑스어) : mets le la .
번역문장 (한국어) : 거기에 놓아.

---

입력문장 (영어) : get your gear .
번역문장 (한국어) : 네 물건 가져와.

정답문장 (프랑스어) : allez chercher votre materiel !
번역문장 (한국어) : 네 물건 가져와!

번역문장 (프랑스어) : va chercher tes affaires !
번역문장 (한국어) : 네 물건 가져와!

---

입력문장 (영어) : i m a stutterer .
번역문장 (한국어) : 나는 말더듬이야.

정답문장 (프랑스어) : je suis begue .
번역문장 (한국어) : 나는 말더듬이야.

번역문장 (프랑스어) : je suis un coureur .
번역문장 (한국어) : 나는 달리기 선수입니다.
```

17-03에서 이 번역기를 좀 더 개선한 버전을 구현합니다.


# 16-03 BLEU Score(Bilingual Evaluation Understudy Score)



앞서 언어 모델(Language Model)의 성능 측정을 위한 평가 방법으로 펄플렉서티(perplexity, PPL)를 소개한 바 있습니다. 기계 번역기에도 PPL을 평가에 사용할 수는 있지만, PPL은 번역의 성능을 직접적으로 반영하는 수치라 보기엔 어렵습니다.

자연어 처리에서는 그 외에도 수많은 평가 방법들이 존재하는데, 기계 번역의 성능이 얼마나 뛰어난가를 측정하기 위해 사용되는 대표적인 방법인 BLEU(Bilingual Evaluation Understudy) 대해서 학습해보겠습니다. 앞으로 진행되는 설명은 논문 BLEU: a Method for Automatic Evaluation of Machine Translation를 참고로 하여 작성되었습니다.

```python
import numpy as np
from collections import Counter
from nltk import ngrams
```

## 1. BLEU(Bilingual Evaluation Understudy)

BLEU는 기계 번역 결과와 사람이 직접 번역한 결과가 얼마나 유사한지 비교하여 번역에 대한 성능을 측정하는 방법입니다. 측정 기준은 n-gram에 기반합니다. n-gram의 정의는 언어 모델 챕터를 참고하시기 바랍니다.

BLEU는 완벽한 방법이라고는 할 수는 없지만 몇 가지 이점을 가집니다. 언어에 구애받지 않고 사용할 수 있으며, 계산 속도가 빠릅니다. BLEU는 PPL과는 달리 높을 수록 성능이 더 좋음을 의미합니다. BLEU를 이해하기 위해 기계 번역 성능 평가를 위한 몇 가지 직관적인 방법을 먼저 제시하고, 문제점을 보완해나가는 방식으로 설명합니다.

### 1) 단어 개수 카운트로 측정하기(Unigram Precision)

한국어-영어 번역기의 성능을 측정한다고 가정해봅시다. 두 개의 기계 번역기가 존재하고 두 기계 번역기에 같은 한국어 문장을 입력하여 번역된 영어 문장의 성능을 측정하고자 합니다. 번역된 문장을 각각 Candidate1, 2라고 해봅시다. 이 문장의 성능을 평가하기 위해서는 정답으로 비교되는 문장이 있어야 합니다. 세 명의 사람에게 한국어를 보고 영작해보라고 하여 세 개의 번역 문장을 만들어냈습니다. 이 세 문장을 각각 Reference1, 2, 3라고 해봅시다.

#### Example 1

- Candidate1 : It is a guide to action which ensures that the military always obeys the commands of the party.
- Candidate2 : It is to insure the troops forever hearing the activity guidebook that party direct.
- Reference1 : It is a guide to action that ensures that the military will forever heed Party commands.
- Reference2 : It is the guiding principle which guarantees the military forces always being under the command of the Party.
- Reference3 : It is the practical guide for the army always to heed the directions of the party.

편의상 Candidate를 Ca로, Reference를 Ref로 축약하여 부르겠습니다. Ca 1, 2를 Ref 1, 2, 3과 비교하여 성능을 측정하고자 합니다. 가장 직관적인 성능 평가 방법은 Ref 1, 2, 3 중 어느 한 문장이라도 등장한 단어의 개수를 Ca에서 세는 것입니다. 그리고 그 후에 Ca의 모든 단어의 카운트의 합. 즉, Ca에서의 총 단어의 수으로 나눠줍니다.

이러한 측정 방법을 **유니그램 정밀도(Unigram Precision)**라고 합니다. 이를 식으로 표현하면 다음과 같습니다.

$$\text{Unigram Precision =}\frac{\text{the number of Ca words(unigrams) which occur in any Ref}}{\text{the total number of words in the Ca}}$$

Ca1의 단어들은 얼추 훑어만봐도 Ref1, Ref2, Ref3에서 전반적으로 등장하는 반면, Ca2는 그렇지 않습니다. 이는 Ca1이 Ca2보다 더 좋은 번역 문장임을 의미합니다. 예를 들어 Ca1의 It is a guide to action은 Ref1에서, which는 Ref2에서, ensures that the militrary는 Ref1에서, always는 Ref2와 Ref3에서, commands는 Ref1에서, of the party는 Ref2에서 등장하였습니다. (대소문자 구분은 없다고 합시다.) Ca1에 있는 단어 중 Ref1, Ref2, Ref3 어디에도 등장하지 않은 단어는 obeys뿐입니다. 반면, Ca2는 Ca1과 비교하여 상대적으로 Ref1, 2, 3에 등장한 단어들이 적습니다.

위의 계산 방법에 따르면 Ca1과 Ca2의 유니그램 정밀도는 각각 아래와 같습니다.

$$\text{Ca1 Unigram Precision =} \frac{17}{18}$$ $$\text{Ca2 Unigram Precision =} \frac{8}{14}$$

이제부터는 단어라는 표현보다는 유니그램이라는 용어로 설명하겠습니다. 지금까지 설명한 유니그램 정밀도는 나름 의미있는 측정 방법으로 보이지만 사실 허술한 점이 있습니다. 아래와 같은 새로운 예가 있다고 해봅시다.

### 2) 중복을 제거하여 보정하기(Modified Unigram Precision)

#### Example 2

- Candidate : the the the the the the the
- Reference1 : the cat is on the mat
- Reference2 : there is a cat on the mat

위의 Ca는 the만 7개가 등장한 터무니 없는 번역입니다. 하지만 이 번역은 앞서 배운 유니그램 정밀도에 따르면 $\frac{7}{7}=1$이라는 최고의 성능 평가를 받게 됩니다. 이에 유니그램 정밀도를 다소 보정할 필요를 느낍니다. 이를 보정하기 위해서는 정밀도의 분자를 계산하기 위해 Ref와 매칭하며 카운트하는 과정에서 Ca의 유니그램이 이미 Ref에서 매칭된 적이 있었는지를 고려해야 합니다.

$$\text{Unigram Precision =}\frac{\text{Ref들과 Ca를 고려한 새로운 카운트 방법이 필요!}}{\text{Ca의 총 유니그램 수}}$$

정밀도의 분자를 계산하기 위한 각 유니그램의 카운트는 다음과 같이 수정합시다. 우선, 유니그램이 하나의 Ref에서 최대 몇 번 등장했는지를 카운트합니다. 이 값을 maximum reference count를 줄인 의미에서 Max_Ref_Count라고 부르겠습니다. Max_Ref_Count가 기존의 단순 카운트한 값보다 작은 경우에는 이 값을 최종 카운트 값으로 대체합니다. 정밀도의 분자 계산을 위한 새로운 카운트 방식을 식으로 표현하면 다음과 같습니다.

$Count_{clip}\ =\ min(Count,\ Max$__$Ref$__$Count)$

위의 카운트를 사용하여 분자를 계산한 정밀도를 **보정된 유니그램 정밀도(Modified Unigram Precision)**라고 합니다.

$$\text{Modified Unigram Precision =}\frac{\sum_{unigram∈Candidate}\ Count_{clip}(unigram)} {\sum_{unigram∈Candidate}\ Count(unigram)}$$

분모의 경우에는 이전과 동일하게 Ca의 모든 유니그램에 대해서 각각 $Count$하고 모두 합한 값을 사용합니다.

보정된 유니그램 정밀도를 예제를 통해 이해해봅시다. **Example 2**를 볼까요? the의 경우에는 Ref1에서 총 두 번 등장하였으므로, the의 카운트는 2로 보정됩니다. Ca의 기존 유니그램 정밀도는 $\frac{7}{7}=1$이었으나 보정된 유니그램 정밀도는 $\frac{2}{7}$와 같이 변경됩니다.

다른 예로 **Example 1**에서의 Ca1의 보정된 유니그램 정밀도를 계산해보면 보정되기 이전과 동일하게 $\frac{17}{18}$이지만 결과를 얻는 과정은 다릅니다. Ca1에서 the는 3번 등장하지만, Re2와 Ref3에서 the가 4번 등장하므로 3이 4보다 작으므로 the는 3으로 카운트 됩니다. the 외에 Ca1의 모든 유니그램은 전부 1개씩 등장하므로 보정 전과 동일하게 카운트하면 됩니다. 결과적으로 보정 이전의 정밀도와 동일하게 $\frac{17}{18}$의 값을 가집니다.

### 3) 보정된 유니그램 정밀도 (Modified Unigram Precision) 구현하기

보정된 유니그램 정밀도를 파이썬 함수로 구현해보겠습니다. 보정된 유니그램 정밀도를 구현하기 위해서는 유니그램을 카운트 하는 $Count$ 함수와 $Count_{clip}$ 함수 두 가지 함수를 구현해야 합니다. 분모를 구하기 위해서 $Count$ 함수를 사용하고, 분자를 구하기 위해서 $Count_{clip}$ 함수를 사용하면 보정된 유니그램 정밀도를 구할 수 있습니다. 우선 유니그램을 단순히 $Count$하는 함수를 simple_count라는 이름의 아래 함수로 구현합니다.

```python
# 토큰화 된 문장(tokens)에서 n-gram을 카운트
def simple_count(tokens, n):
  return Counter(ngrams(tokens, n))
```

위 함수는 토큰화 된 문장을 입력받아서 문장 내의 n-gram의 개수를 카운트하는 함수입니다. 구하고자 하는 것은 유니그램 정밀도이므로 카운트하고자 하는 n-gram의 단위를 결정하는 simple_count 함수의 두번째 인자인 n의 값을 1로 하여 함수를 실행하면 됩니다. **Example 1**의 Ca1를 가져와 함수가 어떤 결과를 출력하는지 확인해봅시다.

```python
candidate = "It is a guide to action which ensures that the military always obeys the commands of the party."
tokens = candidate.split() # 토큰화
result = simple_count(tokens, 1) # n = 1은 유니그램
print('유니그램 카운트 :',result)
```

```python
유니그램 카운트 : Counter({('the',): 3, ('It',): 1, ('is',): 1, ('a',): 1, ('guide',): 1, ('to',): 1, ('action',): 1, ('which',): 1, ('ensures',): 1, ('that',): 1, ('military',): 1, ('always',): 1, ('obeys',): 1, ('commands',): 1, ('of',): 1, ('party.',): 1})
```

위의 출력 결과는 모든 유니그램을 카운트한 결과를 보여줍니다. 대부분의 유니그램이 1개씩 카운트되었으나 유니그램 the는 문장에서 3번 등장하였으므로 유일하게 3의 값을 가집니다. 이번에는 **Example 2**의 Ca를 가지고 함수를 수행해봅시다.

```python
candidate = 'the the the the the the the'
tokens = candidate.split() # 토큰화
result = simple_count(tokens, 1) # n = 1은 유니그램
print('유니그램 카운트 :',result)
```

```python
유니그램 카운트 : Counter({('the',): 7})
```

simple_count 함수는 단순 카운트를 수행하므로 the에 대해서 7이라는 카운트 값을 리턴합니다. $Count$에 대한 함수를 구현하였으니 이번에는 $Count_{clip}$을 아래의 count_clip 이름을 가진 함수로 구현해보겠습니다.

```python
def count_clip(candidate, reference_list, n):
  # Ca 문장에서 n-gram 카운트
  ca_cnt = simple_count(candidate, n)
  max_ref_cnt_dict = dict()

  for ref in reference_list: 
    # Ref 문장에서 n-gram 카운트
    ref_cnt = simple_count(ref, n)

    # 각 Ref 문장에 대해서 비교하여 n-gram의 최대 등장 횟수를 계산.
    for n_gram in ref_cnt: 
      if n_gram in max_ref_cnt_dict:
        max_ref_cnt_dict[n_gram] = max(ref_cnt[n_gram], max_ref_cnt_dict[n_gram])
      else:
        max_ref_cnt_dict[n_gram] = ref_cnt[n_gram]

  return {
        # count_clip = min(count, max_ref_count)
        n_gram: min(ca_cnt.get(n_gram, 0), max_ref_cnt_dict.get(n_gram, 0)) for n_gram in ca_cnt
     }
```

count_clip 함수는 candidate 문장과 reference 문장들, 그리고 카운트 단위가 되는 n-gram에서의 n의 값 이 세 가지를 인자로 입력받아서 $count_{clip}$을 수행합니다. 여기서는 유니그램 정밀도를 구현하고 있으므로 역시나 n=1로 하여 함수를 실행하면 됩니다.

또한 count_clip 함수 내부에는 기존에 구현했던 simple_count 함수가 사용된 것을 확인할 수 있습니다. $Count_{clip}$을 구하기 위해서는 $Max$__$Ref$__$Count$값과 비교하기 위해 $Count$값이 필요하기 때문입니다. **Example 2**를 통해 함수가 정상 작동되는지 확인해봅시다.

```python
candidate = 'the the the the the the the'
references = [
    'the cat is on the mat',
    'there is a cat on the mat'
]
result = count_clip(candidate.split(),list(map(lambda ref: ref.split(), references)),1)
print('보정된 유니그램 카운트 :',result)
```

```
보정된 유니그램 카운트 : {('the',): 2}
```

동일한 예제 문장에 대해서 위의 simple_count 함수는 the가 7개로 카운트되었던 것과는 달리 이번에는 2개로 카운트되었습니다. 위의 두 함수를 사용하여 예제 문장에 대해서 보정된 정밀도를 연산하는 함수를 modified_precision란 이름의 함수로 구현해봅시다.

```python
def modified_precision(candidate, reference_list, n):
  clip_cnt = count_clip(candidate, reference_list, n) 
  total_clip_cnt = sum(clip_cnt.values()) # 분자

  cnt = simple_count(candidate, n)
  total_cnt = sum(cnt.values()) # 분모

  # 분모가 0이 되는 것을 방지
  if total_cnt == 0: 
    total_cnt = 1

  # 분자 : count_clip의 합, 분모 : 단순 count의 합 ==> 보정된 정밀도
  return (total_clip_cnt / total_cnt)
```

```python
result = modified_precision(candidate.split(), list(map(lambda ref: ref.split(), references)), n=1)
print('보정된 유니그램 정밀도 :',result)
```

```
보정된 유니그램 정밀도 : 0.2857142857142857
```

소수 값이 나오는데 이는 $\frac{2}{7}$의 값을 의미합니다. 이는 앞서 육안으로 계산했던 **Example 2**에서 Ca의 보정된 정밀도와 동일합니다. 지금까지 보정된 유니그램 정밀도에 대해서 설명하고, 직접 구현까지 해보았습니다.

이제부터 설명에서 언급하는 '정밀도'는 기본적으로 **보정된 정밀도(Modified Precision)**라고 가정합니다. 정밀도를 보정하므로서 Ca에서 발생하는 단어 중복에 대한 문제점은 해결되었습니다. 하지만 유니그램 정밀도가 가지는 본질적인 문제점이 있기에 유니그램을 넘어 바이그램, 트라이그램 등과 같이 n-gram으로 확장해야 합니다. 문제점이 무엇인지 이해하고, 어떻게 n-gram으로 확장하는지 학습해봅시다.

### 4) 순서를 고려하기 위해서 n-gram으로 확장하기

BoW 표현과 유사하게, 유니그램 정밀도와 같이 각 단어의 빈도수로 접근하는 방법은 결국 단어의 순서를 고려하지 않는다는 특징이 있습니다. **Example 1**에 Ca3이라는 새로운 문장을 추가해보고 기존의 Ca1과 비교해봅시다.

#### Example 1

- Candidate1 : It is a guide to action which ensures that the military always obeys the commands of the party.
- Candidate2 : It is to insure the troops forever hearing the activity guidebook that party direct.
- **Candidate3 : the that military a is It guide ensures which to commands the of action obeys always party the.**
- Reference1 : It is a guide to action that ensures that the military will forever heed Party commands.
- Reference2 : It is the guiding principle which guarantees the military forces always being under the command of the Party.
- Reference3 : It is the practical guide for the army always to heed the directions of the party.

Ca3은 사실 Ca1에서 모든 유니그램의 순서를 랜덤으로 섞은 실제 영어 문법에 맞지 않은 문장입니다. 하지만 Ref 1, 2, 3과 비교하여 유니그램 정밀도를 적용하면 Ca1과 Ca3의 두 정밀도는 동일합니다. 유니그램 정밀도는 유니그램의 순서를 전혀 고려하지 않기 때문입니다. 이를 위한 대안으로 개별적인 유니그램/단어로서 카운트하는 유니그램 정밀도에서 다음에 등장한 단어까지 함께 고려하여 카운트하도록 유니그램 외에도 Bigram, Trigram, 4-gram 단위 등으로 계산한 정밀도. 즉, n-gram을 이용한 정밀도를 도입하고자 합니다.

이들 각각은 카운트 단위를 2개, 3개, 4개로 보느냐의 차이로 2-gram Precision, 3-gram Precision, 4-gram Precision이라고 하기도 합니다. 어떤 의미인지 바이그램(Bigram) 단위로 카운트하여 **Example 1, 2**의 바이그램 정밀도(Bigram Precision)를 계산해보겠습니다. 우선 좀 더 쉬운 **Example 2**부터 볼까요?

#### Example 2

- Candidate1 : the the the the the the the
- Candidate2 : the cat the cat on the mat
- Reference1 : the cat is on the mat
- Reference2 : there is a cat on the mat

이해를 돕고자 **Example 2**에 Ca2를 새로 추가했습니다. Ca2 바이그램의 $Count$와 $Count_{clip}$은 아래와 같습니다.

|바이그램|the cat|cat the|cat on|on the|the mat|SUM|
|---|---|---|---|---|---|---|
|$Count$|2|1|1|1|1|**6**|
|$Count_{clip}$|1|0|1|1|1|**4**|

결과적으로 Ca2의 바이그램 정밀도는 $\frac{4}{6}$가 됩니다. 반면, 당연하게도 Ca1의 바이그램 정밀도는 0입니다. **Example 1**은 어떨까요? **Example 1**에서 Ca1의 바이그램 정밀도는 $\frac{10}{17}$이며, Ca2의 바이그램 정밀도는 $\frac{1}{13}$입니다. Ca1에서 단어의 순서를 뒤섞은 Ca3의 바이그램 정밀도는 독자분들의 숙제로 남깁니다.

보정된 정밀도를 식으로 정의해보겠습니다. $p_{n}$에서 $n$은 n-gram에서의 $n$을 의미한다고 하였을 때, 앞서 배운 보정된 유니그램 정밀도의 식을 상기해봅시다.

$$p_{1}=\frac{\sum_{unigram∈Candidate}\ Count_{clip}(unigram)} {\sum_{unigram∈Candidate}\ Count(unigram)}$$

이를 n-gram으로 일반화하면 아래와 같습니다.

$$p_{n}=\frac{\sum_{n\text{-}gram∈Candidate}\ Count_{clip}(n\text{-}gram)} {\sum_{n\text{-}gram∈Candidate}\ Count(n\text{-}gram)}$$

유니그램 정밀도에서는 $n$이 1이므로 $p_{1}$로 표현하였으나, 일반화 된 식에서는 $p_{n}$으로 표현한 것을 볼 수 있습니다.

여기서는 보정된 바이그램 정밀도 $p_{2}$, 보정된 트라이그램 정밀도 $p_{3}$ 등에 대한 파이썬 실습은 생략합니다. 사실 $p_{n}$을 계산하기 위한 함수를 별도로 다시 구현할 필요는 없는데, 앞서 구현한 함수 simple_count, count_clip, modified_precision은 모두 n-gram의 n을 함수의 인자로 받으므로, n을 1대신 다른 값을 넣어서 실습해보면 바이그램, 트라이그램 등에 대해서도 보정된 정밀도를 구할 수 있습니다.

n-gram 정밀도 식을 이해하였다면 BLEU의 최종 식까지 다 왔습니다. BLEU는 보정된 정밀도 $p_{1}, p_{2}, ..., p_{n}$를 모두 조합하여 사용합니다. 이를 모두 조합한 BLEU의 식은 아래와 같습니다.

$$BLEU = exp(\sum_{n=1}^{N}w_{n}\ \text{log}\ p_{n})$$

$p_{n}$ : 각 gram의 보정된 정밀도입니다.  
$N$ : n-gram에서 $n$의 최대 숫자입니다. 보통은 4의 값을 가집니다. $N$이 4라는 것은 $p_{1}, p_{2}, p_{3}, p_{4}$를 사용한다는 것을 의미합니다.  
$w_{n}$ : 각 gram의 보정된 정밀도에 서로 다른 가중치를 줄 수 있습니다. 이 가중치의 합은 1로 합니다. 예를 들어 $N$이 4라고 하였을 때, $p_{1}, p_{2}, p_{3}, p_{4}$에 대해서 동일한 가중치를 주고자한다면 모두 0.25를 적용할 수 있습니다.

BLEU의 최종식에 거의 다 도달했습니다. 즉, 여전히 위의 BLEU식에도 문제점이 존재합니다.

### 5) 짧은 문장 길이에 대한 패널티(Brevity Penalty)

n-gram으로 단어의 순서를 고려한다고 하더라도 여전히 남아있는 문제가 있는데, 바로 Ca의 길이에 BLEU의 점수가 과한 영향을 받을 수 있다는 점입니다. 기존 **Example 1**에 다음의 Ca를 추가한다고 해보겠습니다.

#### Example 1

**Candidate4 : it is**

이 문장은 유니그램 정밀도나 바이그램 정밀도가 각각 $\frac{2}{2}$, $\frac{1}{1}$로 두 정밀도 모두 1이라는 높은 정밀도를 얻습니다. 이과 같이 제대로 된 번역이 아님에도 문장의 길이가 짧다는 이유로 높은 점수를 받는 것은 이상합니다. 그래서 Ca가 Ref보다 문장의 길이가 짧은 경우에는 점수에 패널티를 줄 필요가 있습니다. 이를 **브레버티 패널티(Brevity Penalty)**라고 합니다. (직역하면 짧음 패널티) 이에 대해서 배우기 전에, 만약 반대로 Ca의 길이가 Ref보다 긴 경우에도 문제가 생길 수 있는지 보겠습니다.

#### Example 3

- Candidate 1: I always invariably perpetually do.
- Candidate 2: I always do.
- Reference 1: I always do.
- Reference 2: I invariably do.
- Reference 3: I perpetually do.

**Example 3**에서 Ca1은 가장 많은 단어를 사용했지만 Ca2보다 좋지 못한 번역입니다. 다시 말해 Ref의 단어를 가장 많이 사용한 것이 꼭 좋은 번역이라는 의미는 아닙니다. 그런데 다행히도 위와 같이 Ca의 길이가 불필요하게 Ref보다 긴 경우에는 BLEU 수식에서 정밀도를 n-gram으로 확장하여 바이그램, 트라이그램 정밀도 등을 모두 계산에 사용하고 있는 것만으로도 이미 패널티를 받고 있습니다. 즉, 브레버티 패널티를 설계할 때, 이 경우까지 고려할 필요는 없습니다.

다시 Ref보다 Ca의 길이가 짧을 경우에 패널티를 주는 브레버티 패널티의 이야기로 돌아보겠습니다. 브레버티 패널티는 앞서 배운 BLEU의 식에 곱하는 방식으로 사용합니다. 브레버티 패널티를 줄여서 $BP$라고 하였을 때, 최종 BLEU의 식은 아래와 같습니다.

$$BLEU = BP × exp(\sum_{n=1}^{N}w_{n}\ \text{log}\ p_{n})$$

위의 수식은 패널티를 줄 필요가 없는 경우에는 $BP$의 값이 1이어야 함을 의미합니다. 이를 반영한 $BP$의 수식은 아래와 같습니다.

$$BP = \begin{cases}1&\text{if}\space c>r\\\ e^{(1-r/c)}&\text{if}\space c \leq r \end{cases} $$

$c$ : Candidate의 길이  
$r$ : Candidate와 가장 길이 차이가 작은 Reference의 길이

Ref가 1개라면 Ca와 Ref의 두 문장의 길이만을 가지고 계산하면 되겠지만 여기서는 Ref가 여러 개일 때를 가정하고 있으므로 $r$은 모든 Ref들 중에서 Ca와 가장 길이 차이가 작은 Ref의 길이로 합니다. $r$을 구하는 코드는 아래와 같습니다.

```python
# Ca 길이와 가장 근접한 Ref의 길이를 리턴하는 함수
def closest_ref_length(candidate, reference_list):
  ca_len = len(candidate) # ca 길이
  ref_lens = (len(ref) for ref in reference_list) # Ref들의 길이
  # 길이 차이를 최소화하는 Ref를 찾아서 Ref의 길이를 리턴
  closest_ref_len = min(ref_lens, key=lambda ref_len: (abs(ref_len - ca_len), ref_len))
  return closest_ref_len
```

만약 Ca와 길이가 정확히 동일한 Ref가 있다면 길이 차이가 0인 최고 수준의 매치(best match length)입니다. 또한 만약 서로 다른 길이의 Ref이지만 Ca와 길이 차이가 동일한 경우에는 더 작은 길이의 Ref를 택합니다. 예를 들어 Ca가 길이가 10인데, Ref 1, 2가 각각 9와 11이라면 길이 차이는 동일하게 1밖에 나지 않지만 9를 택합니다. closest_ref_length 함수를 통해 $r$을 구했다면, $BP$를 구하는 함수 brevity_penalty를 구현해봅시다.

```python
def brevity_penalty(candidate, reference_list):
  ca_len = len(candidate)
  ref_len = closest_ref_length(candidate, reference_list)

  if ca_len > ref_len:
    return 1

  # candidate가 비어있다면 BP = 0 → BLEU = 0.0
  elif ca_len == 0 :
    return 0
  else:
    return np.exp(1 - ref_len/ca_len)
```

위 함수는 앞서 배운 $BP$의 수식처럼 $c$가 $r$보다 클 경우에는 1을 리턴하고, 그 외의 경우에는 $e^{1-r/c}$를 리턴합니다. 최종적으로 BLEU 점수를 계산하는 함수 bleu_score를 구현해봅시다.

```python
def bleu_score(candidate, reference_list, weights=[0.25, 0.25, 0.25, 0.25]):
  bp = brevity_penalty(candidate, reference_list) # 브레버티 패널티, BP

  p_n = [modified_precision(candidate, reference_list, n=n) for n, _ in enumerate(weights,start=1)] 
  # p1, p2, p3, ..., pn
  score = np.sum([w_i * np.log(p_i) if p_i != 0 else 0 for w_i, p_i in zip(weights, p_n)])
  return bp * np.exp(score)
```

위의 bleu_score 함수는 기본적으로는 $N$이 4에 각 gram에 대한 가중치는 동일하게 0.25라 주어진다고 가정합니다. 또한 함수 내에서는 $BP$를 구하고 bp에, $p_{1}, p_{2}, ..., p_{n}$를 구하여 p_n에 저장하도록 구현되어져 있습니다. 그리고 앞서 배운 BLEU의 식에 따라 추가 연산하여 최종 계산한 값을 리턴합니다.

위 함수가 동작하기 위해서는 앞서 구현한 simple_count, count_clip, modified_precision, brevity_penalty 4개의 함수 또한 모두 구현되어져 있어야 합니다. 지금까지 구현한 BLEU 코드로 계산된 점수와 NLTK 패키지에 이미 구현되어져 있는 BLEU 코드로 계산된 점수를 비교해봅시다.

## 2. NLTK를 사용한 BLEU 측정하기

파이썬에서는 NLTK 패키지를 사용하여 BLEU를 계산할 수 있습니다.

```python
import nltk.translate.bleu_score as bleu

candidate = 'It is a guide to action which ensures that the military always obeys the commands of the party'
references = [
    'It is a guide to action that ensures that the military will forever heed Party commands',
    'It is the guiding principle which guarantees the military forces always being under the command of the Party',
    'It is the practical guide for the army always to heed the directions of the party'
]

print('실습 코드의 BLEU :',bleu_score(candidate.split(),list(map(lambda ref: ref.split(), references))))
print('패키지 NLTK의 BLEU :',bleu.sentence_bleu(list(map(lambda ref: ref.split(), references)),candidate.split()))
```

```python
실습 코드의 BLEU : 0.5045666840058485
패키지 NLTK의 BLEU : 0.5045666840058485
```

