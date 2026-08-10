
import nltk

from nltk.tokenize import word_tokenize
from nltk.tokenize import WordPunctTokenizer
from nltk.tokenize import TreebankWordTokenizer
from nltk.tokenize import RegexpTokenizer
from nltk.tokenize import sent_tokenize


from nltk.tag import pos_tag
from nltk.corpus import stopwords


# from tensorflow.keras.preprocessing.text import text_to_word_sequence
import kss
import re 
import textwrap
import spacy

from nltk import FreqDist
import urllib.request
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt


from konlpy.tag import Okt
from konlpy.tag import Kkma
from konlpy.tag import Mecab


okt = Okt()
kkma = Kkma()

nltk.download('punkt')
nltk.download('punkt_tab')
nltk.download('stopwords')



def _01_tokenization():
    print('\n단어 토큰화1 :',
          word_tokenize("Don't be fooled by the dark sounding name, "\
                        "Mr. Jone's Orphanage is as cheery as " \
                        "cheery goes for a pastry shop."), "\n")

    print('단어 토큰화2 :',
          WordPunctTokenizer().tokenize(
                        "Don't be fooled by the dark sounding name, "\
                        "Mr. Jone's Orphanage is as cheery as " \
                        "cheery goes for a pastry shop."), "\n")

    print('단어 토큰화3 :',
          text_to_word_sequence("Don't be fooled by the dark sounding name, "\
                        "Mr. Jone's Orphanage is as cheery as " \
                        "cheery goes for a pastry shop."), "\n")


    tokenizer = TreebankWordTokenizer()
    text = "Starting a home_based restaurant may be an ideal. " \
            "it doesn't have a food chain or restaurant of their own."
    print('Treebank Word Tokenizer(PTB) : ',tokenizer.tokenize(text), '\n')

    text = "His barber kept his word. " \
           "But keeping such a huge secret to himself was driving him crazy. " \
           "Finally, the barber went up a mountain and almost to the edge of a cliff. " \
           "He dug a hole in the midst of some reeds. " \
           "He looked about, to make sure no one was near."
    print('문장 토큰화1 :',sent_tokenize(text), '\n')

    text = "I am actively looking for Ph.D. students. and you are a Ph.D student."
    print('문장 토큰화2 :',sent_tokenize(text), '\n')

    text = '딥 러닝 자연어 처리가 재미있기는 합니다. ' \
            '그런데 문제는 영어보다 한국어로 할 때 너무 어렵습니다. 이제 해보면 알걸요?'
    print('한국어 문장 토큰화 :',kss.split_sentences(text), '\n')



    # 품사 태깅 실습
    text = "I am actively looking for Ph.D. students. and you are a Ph.D student."
    tokenized_sentence = word_tokenize(text)

    print('단어 토큰화 :',tokenized_sentence, '\n')
    print('품사 태깅 :',pos_tag(tokenized_sentence), '\n')

    print('OKT 형태소 분석 :',okt.morphs("열심히 코딩한 당신, 연휴에는 여행을 가봐요"), '\n')
    print('OKT 품사 태깅 :',okt.pos("열심히 코딩한 당신, 연휴에는 여행을 가봐요"), '\n')
    print('OKT 명사 추출 :',okt.nouns("열심히 코딩한 당신, 연휴에는 여행을 가봐요"), '\n') 

    print('꼬꼬마 형태소 분석 :',kkma.morphs("열심히 코딩한 당신, 연휴에는 여행을 가봐요"))
    print('꼬꼬마 품사 태깅 :',kkma.pos("열심히 코딩한 당신, 연휴에는 여행을 가봐요")) 
    print('꼬꼬마 명사 추출 :',kkma.nouns("열심히 코딩한 당신, 연휴에는 여행을 가봐요"))  

def _02_data_cleaning_and_normalization():
      text = "I was wondering if anyone out there could enlighten me on this car."

      shortword = re.compile(r'\W*\b\w{1,2}\b')
      print('\n',shortword.sub('', text))

def _03_Stopword():
      stop_words_list = stopwords.words('english')
      print('\n불용어 개수 :', len(stop_words_list))
      print('불용어 10개 출력 :',stop_words_list[:10], '\n')


      # NLTK를 통해 불용어 제거
      example = "Family is not an important thing. It's everything."
      stop_words = set(stopwords.words('english')) 

      word_tokens = word_tokenize(example)

      result = []
      for word in word_tokens: 
            if word not in stop_words: 
                  result.append(word) 

      print('\n불용어 제거 전 :',word_tokens) 
      print('불용어 제거 후 :',result)


      # 한국어 불용어 제거 (okt 사용)
      okt = Okt()
      example = "고기를 아무렇게나 구우려고 하면 안 돼. 고기라고 다 같은 게 아니거든. " \
                "예컨대 삼겹살을 구울 때는 중요한 게 있지."
      stop_words = "를 아무렇게나 구 우려 고 안 돼 같은 게 구울 때 는"

      stop_words = set(stop_words.split(' '))
      word_tokens = okt.morphs(example)

      result = [word for word in word_tokens if not word in stop_words]

      print('\n불용어 제거 전 :',word_tokens) 
      print('불용어 제거 후 :',result)

def _04_Reg_ex():

      def basic_regex():
            print('\n')

            r = re.compile("a.c")
            print(r.search("kkk"))
            print(r.search("abc"), '\n')

            r = re.compile("ab?c")
            print(r.search("abbc"))
            print(r.search("abc"))
            print(r.search("ac"), '\n')

            r = re.compile("ab*c")
            print(r.search("a")) 
            print(r.search("ac"))
            print(r.search("abc"))
            print(r.search("abbbbc"), '\n') 

            r = re.compile("ab+c")
            print(r.search("a")) 
            print(r.search("abc"))
            print(r.search("abbbbc"), '\n') 

            r = re.compile("^ab")
            print(r.search("bbc")) 
            print(r.search("zab"))
            print(r.search("abz"), '\n') 

            r = re.compile("ab{2}c")
            print(r.search("ac")) 
            print(r.search("abc"))
            print(r.search("abbc"), '\n') 

            r = re.compile("ab{2,8}c")
            print(r.search("ac")) 
            print(r.search("abbbbbbbbbbbc"))
            print(r.search("abbbbc"), '\n') 

            r = re.compile("a{2,}bc")
            print(r.search("bc")) 
            print(r.search("abc"))
            print(r.search("aaabc"), '\n') 

            r = re.compile("[abc]")
            print(r.search("zzz")) 
            print(r.search("abccba"))
            print(r.search("aaa"), '\n') 

            r = re.compile("[a-zA-Z]")
            print(r.search("291ab")) 
            print(r.search("ab"))
            print(r.search("aBc"), '\n') 

            r = re.compile("[^abc]")
            print(r.search("ac")) 
            print(r.search("abc"))
            print(r.search("defg"), '\n') 
      # basic_regex()

      def basic_function():

            # match(), search()
            r = re.compile("ab.")
            print('\n', r.match("kkkabc")) 
            print(r.search("kkkabc")) 
            print(r.match("abckkk"), '\n')

            # split()
            text = "사과 딸기 수박 메론 바나나"
            print(re.split(" ", text))
            
            text = textwrap.dedent("""\
            사과
            딸기
            수박
            메론
            바나나""")

            print(re.split("\n", text))

            text = "사과+딸기+수박+메론+바나나"
            print(re.split("\+", text), '\n')


            # findall()
            text =   """이름 : 김철수
                        전화번호 : 010 - 1234 - 1234
                        나이 : 30
                        성별 : 남"""

            print(re.findall("\d+", text))
            print(re.findall("\d+", "문자열입니다."), '\n')



            # sub()
            text = "Regular expression : A regular expression, " \
            "regex or regexp[1] (sometimes called a rational expression)[2][3] is, " \
            "in theoretical computer science and formal language theory, " \
            "a sequence of characters that define a search pattern."

            preprocessed_text = re.sub('[^a-zA-Z]', ' ', text)
            print(preprocessed_text, '\n')


            # preprocess example

            text =   """100 John    PROF
                        101 James   STUD
                        102 Mac   STUD"""
            
            print(re.split('\s+', text))
            print(re.findall('\d+',text))

            print(re.findall('[A-Z]',text))
            print(re.findall('[A-Z]{4}',text))
            print(re.findall('[A-Z][a-z]+',text), '\n')


            # RegexpTokenizer : custom tokenizer

            text = "Don't be fooled by the dark sounding name, " \
            "Mr. Jone's Orphanage is as cheery as cheery goes for a pastry shop"

            tokenizer1 = RegexpTokenizer("[\w]+")             # 문자,숫자기준
            tokenizer2 = RegexpTokenizer("\s+", gaps=True)    # 공백 기준

            print(tokenizer1.tokenize(text))
            print(tokenizer2.tokenize(text))
            





      basic_function()

def _05_NLP_preprocessing():

      def Tokenization():
            en_text = "A Dog Run back corner near spare bedrooms"

            ## 1) spaCy lib
            spacy_en = spacy.load('en_core_web_sm')

            def tokenize(en_text):
                  return [tok.text for tok in spacy_en.tokenizer(en_text)]
            print('\n', tokenize(en_text), '\n')

            ## 2) NLTK lib
            nltk.download('punkt')
            print(word_tokenize(en_text), '\n')

            ## 3) 공백 기준 토큰화 (split())
            print(en_text.split(), '\n\n')

            ## 4) 문자 토큰화
            print(list(en_text))


            # 한국어 토큰화 (window 호환성 문제 : colab 사용)
            kor_text = "사과의 놀라운 효능이라는 글을 봤어. " \
            "그래서 오늘 사과를 먹으려고 했는데 " \
            "사과가 썩어서 슈퍼에 가서 사과랑 오렌지 사왔어"

            print(kor_text.split(), '\n')

            tokenizer = Mecab()
            print(tokenizer.morphs(kor_text), '\n')
      # Tokenization()

      def Vocabulary_preprocess_basic():
            urllib.request.urlretrieve("https://raw.githubusercontent"
            ".com/e9t/nsmc/master/ratings.txt", filename="ratings.txt")
            data = pd.read_table('ratings.txt') # 데이터프레임에 저장
            data[:10]

            print('전체 샘플의 수 : {}'.format(len(data)))

            sample_data = data[:100] 

            sample_data['document'] = sample_data['document'].str.replace(
                                    "[^ㄱ-ㅎㅏ-ㅣ가-힣 ]","", regex=True
                                    )
            # display(sample_data[:10])

            stopwords=['의','가','이','은','들','는','좀','잘','걍',
                       '과','도','를','으로','자','에','와','한','하다']

            tokenizer = Mecab()
            tokenized=[]
            for sentence in sample_data['document']:
                  temp = tokenizer.morphs(sentence) 
                  temp = [word for word in temp if not word in stopwords]
                  tokenized.append(temp)
            print(tokenized[:10])

            vocab = FreqDist(np.hstack(tokenized))
            print('단어 집합의 크기 : {}'.format(len(vocab)))
            vocab['재밌']

            vocab_size = 500
            vocab = vocab.most_common(vocab_size)
            print('단어 집합의 크기 : {}'.format(len(vocab)))


            # 단어에 고유한 정수 부여
            word_to_index = {word[0] : index + 2 for index, word in enumerate(vocab)}
            word_to_index['pad'] = 1
            word_to_index['unk'] = 0

            encoded = []
            for line in tokenized: 
                  temp = []
                  for w in line:
                        try:
                              temp.append(word_to_index[w]) 
                        except KeyError: 
                              temp.append(word_to_index['unk']) 

                  encoded.append(temp)

            # padding (문장 길이 통일)
            max_len = max(len(l) for l in encoded)

            print('리뷰의 최대 길이 : %d' % max_len)
            print('리뷰의 최소 길이 : %d' % min(len(l) for l in encoded))
            print('리뷰의 평균 길이 : %f' % (sum(map(len, encoded))/len(encoded)))

            plt.hist([len(s) for s in encoded], bins=50)
            plt.xlabel('length of sample')
            plt.ylabel('number of sample')
            plt.show()

            for line in encoded:
                  if len(line) < max_len: 
                        line += [word_to_index['pad']] * (max_len - len(line)) 

            print('리뷰의 최대 길이 : %d' % max_len)
            print('리뷰의 최소 길이 : %d' % min(len(l) for l in encoded))
            print('리뷰의 평균 길이 : %f' % (sum(map(len, encoded))/len(encoded)))

            print(encoded[:3])                       
      Vocabulary_preprocess_basic()



# _01_tokenization()
# _02_data_cleaning_and_normalization()
# _03_Stopword()
# _04_Reg_ex()
_05_NLP_preprocessing()