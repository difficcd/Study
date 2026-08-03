
from nltk.tokenize import word_tokenize
from nltk.tokenize import WordPunctTokenizer
from nltk.tokenize import TreebankWordTokenizer
from nltk.tokenize import RegexpTokenizer
from nltk.tokenize import sent_tokenize


from nltk.tag import pos_tag
from nltk.corpus import stopwords

from tensorflow.keras.preprocessing.text import text_to_word_sequence
import kss
import re 
import textwrap

from konlpy.tag import Okt
from konlpy.tag import Kkma

okt = Okt()
kkma = Kkma()




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





# _01_tokenization()
# _02_data_cleaning_and_normalization()
# _03_Stopword()
_04_Reg_ex()