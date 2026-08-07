
from konlpy.tag import Okt
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.feature_extraction.text import TfidfVectorizer

from nltk.corpus import stopwords
from math import log

import pandas as pd 

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
# _01_Bag_of_Words()


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
    display(tf_)
    print('\n')

    result = []
    for j in range(len(vocab)):
        t = vocab[j]
        result.append(idf(t))

    idf_ = pd.DataFrame(result, index=vocab, columns=["IDF"])
    display(idf_)
    print('\n')

    result = []
    for i in range(N):
        result.append([])
        d = docs[i]
        for j in range(len(vocab)):
            t = vocab[j]
            result[-1].append(tfidf(t,d))

    tfidf_ = pd.DataFrame(result, columns = vocab)
    display(tfidf_)
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



_02_TF_IDF()