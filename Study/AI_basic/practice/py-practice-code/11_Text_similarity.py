
from konlpy.tag import Okt
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.metrics.pairwise import cosine_similarity

from nltk.corpus import stopwords
from math import log

import pandas as pd 
import numpy as np

from numpy import dot
from numpy.linalg import norm




okt = Okt()


def _01_Bag_of_Words():
    def build_bag_of_words(document):
        print('\n')
        
        document = document.replace('.', '')
        tokenized_document = okt.morphs(document)

        word_to_index = {}      # temp dic. (=>vocab)
        bow = []                # BoW vector

        for word in tokenized_document:  
            if word not in word_to_index.keys():
                word_to_index[word] = len(word_to_index)  
                bow.insert(len(word_to_index) - 1, 1)
            else:
                index = word_to_index.get(word)
                bow[index] = bow[index] + 1

        return word_to_index, bow


    doc1 = "정부가 발표하는 물가상승률과 소비자가 느끼는 물가상승률은 다르다."
    vocab, bow = build_bag_of_words(doc1)
    print('vocabulary :', vocab)
    print('bag of words vector :', bow, '\n')

    doc2 = '소비자는 주로 소비하는 상품을 기준으로 물가상승률을 느낀다.'
    vocab, bow = build_bag_of_words(doc2)
    print('vocabulary :', vocab)
    print('bag of words vector :', bow, '\n')

    doc3 = doc1 + ' ' + doc2
    vocab, bow = build_bag_of_words(doc3)
    print('vocabulary :', vocab)
    print('bag of words vector :', bow, '\n')



    # ==== CountVectorizer class ==== # 

    corpus = ['you know I want your love. because I love you.']
    vector = CountVectorizer()
    print('bag of words vector :', vector.fit_transform(corpus).toarray()) 
    print('vocabulary :',vector.vocabulary_)



    text = ["Family is not an important thing. It's everything."]
    vect = CountVectorizer(stop_words=["the", "a", "an", "is", "not"])
    print('bag of words vector :',vect.fit_transform(text).toarray())
    print('vocabulary :',vect.vocabulary_)

    vect = CountVectorizer(stop_words="english")
    print('bag of words vector :',vect.fit_transform(text).toarray())
    print('vocabulary :',vect.vocabulary_)

    stop_words = stopwords.words("english")
    vect = CountVectorizer(stop_words=stop_words)
    print('bag of words vector :',vect.fit_transform(text).toarray()) 
    print('vocabulary :',vect.vocabulary_)


    print('\n')


def _02_TF_IDF(): 
    # dataframe 실습을 위해 colab 에서 수행.]

    print('\n')
    docs = [
            '먹고 싶은 사과',
            '먹고 싶은 바나나',
            '길고 노란 바나나 바나나',
            '저는 과일이 좋아요'
           ] 
    
    vocab = list(set(w for doc in docs for w in doc.split()))
    vocab.sort()

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

    result = []

  
    for i in range(N):
        result.append([])
        d = docs[i]
        for j in range(len(vocab)):
            t = vocab[j]
            result[-1].append(tf(t, d))

    tf_ = pd.DataFrame(result, columns = vocab)
    # display(tf_)
    print('\n')

    result = []
    for j in range(len(vocab)):
        t = vocab[j]
        result.append(idf(t))

    idf_ = pd.DataFrame(result, index=vocab, columns=["IDF"])
    # display(idf_)
    print('\n')

    result = []
    for i in range(N):
        result.append([])
        d = docs[i]
        for j in range(len(vocab)):
            t = vocab[j]
            result[-1].append(tfidf(t,d))

    tfidf_ = pd.DataFrame(result, columns = vocab)
    # display(tfidf_)
    print('\n')



    # ==== DTM, TF-IDF : sklearn 사용 ==== # 

    corpus = [
        'you know I want your love',
        'I like you',
        'what should I do ',    
    ]

    vector = CountVectorizer()

    print(vector.fit_transform(corpus).toarray(), '\n')
    print(vector.vocabulary_, '\n\n')

    tfidfv = TfidfVectorizer().fit(corpus)
    print(tfidfv.transform(corpus).toarray(), '\n')
    print(tfidfv.vocabulary_, '\n')


def _03_cosine_similarity():
    def cos_sim(A, B):
        return dot(A, B)/(norm(A)*norm(B))

    doc1 = np.array([0,1,1,1])
    doc2 = np.array([1,0,1,1])
    doc3 = np.array([2,0,2,2])

    print('문서 1과 문서2의 유사도 :',cos_sim(doc1, doc2))
    print('문서 1과 문서3의 유사도 :',cos_sim(doc1, doc3))
    print('문서 2와 문서3의 유사도 :',cos_sim(doc2, doc3))


    # ==== 유사도 이용 추천 system 실습 ==== #

    dir = 'AI_basic/practice/py-practice-code/movies_metadata.csv'
    data = pd.read_csv(dir, low_memory=False)
    print('\n', data.head(2), '\n')

    data = data.head(20000)
    print('overview 열의 결측값의 수 (전처리 전):',data['overview'].isnull().sum())
    data['overview'] = data['overview'].fillna('')
    print('overview 열의 결측값의 수 (전처리 후):',data['overview'].isnull().sum(), '\n')

    tfidf = TfidfVectorizer(stop_words='english')
    tfidf_matrix = tfidf.fit_transform(data['overview'])
    print('TF-IDF 행렬의 크기(shape) :',tfidf_matrix.shape, '\n')

    cosine_sim = cosine_similarity(tfidf_matrix, tfidf_matrix)
    print('코사인 유사도 연산 결과 :',cosine_sim.shape, '\n')

    title_to_index = dict(zip(data['title'], data.index))
    idx = title_to_index['Father of the Bride Part II']
    print(idx)

    def get_recommendations(title, cosine_sim=cosine_sim):

        idx = title_to_index[title]

        sim_scores = list(enumerate(cosine_sim[idx]))
        sim_scores = sorted(sim_scores, key=lambda x: x[1], reverse=True)
        sim_scores = sim_scores[1:11]

        movie_indices = [i[0] for i in sim_scores]

        return data['title'].iloc[movie_indices]

    print(get_recommendations('The Dark Knight Rises'), '\n')


def _04__similarity():

    # ==== Euclidean distance ==== #

    def dist(x,y):   
        return np.sqrt(np.sum((x-y)**2))

    doc1 = np.array((2,3,0,1))
    doc2 = np.array((1,2,3,1))
    doc3 = np.array((2,1,2,2))
    docQ = np.array((1,1,0,1))

    print('\n문서1과 문서Q의 거리 :',dist(doc1,docQ))
    print('문서2과 문서Q의 거리 :',dist(doc2,docQ))
    print('문서3과 문서Q의 거리 :',dist(doc3,docQ), '\n')



    # ==== Jaccard similarity ==== #

    doc1 = "apple banana everyone like likey watch card holder"
    doc2 = "apple banana coupon passport love you"

    tokenized_doc1 = doc1.split()
    tokenized_doc2 = doc2.split()

    print('문서1 :',tokenized_doc1)
    print('문서2 :',tokenized_doc2, '\n')

    union = set(tokenized_doc1).union(set(tokenized_doc2))
    intersection = set(tokenized_doc1).intersection(set(tokenized_doc2))

    print('문서1과 문서2의 합집합 :',union)
    print('문서1과 문서2의 교집합 :',intersection, '\n')

    print('자카드 유사도 :',len(intersection)/len(union),'\n')




# _01_Bag_of_Words()
# _02_TF_IDF()
# _03_cosine_similarity()
_04__similarity()