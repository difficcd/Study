
import torch
import torch.nn as nn

import re
import urllib.request
import zipfile
import pandas as pd
import matplotlib.pyplot as plt
import gensim
import nltk

from lxml import etree
from nltk.tokenize import word_tokenize, sent_tokenize

from gensim.models import Word2Vec
from gensim.models import FastText
from gensim.models import KeyedVectors
from tqdm import tqdm

from konlpy.tag import Okt  
okt = Okt()  

nltk.download('punkt_tab')


def _01_One_hot_encoding():
    token = okt.morphs("나는 자연어 처리를 배운다")  
    print(token)

    word2index = {}
    for voca in token:
        if voca not in word2index.keys():
            word2index[voca] = len(word2index)
    print(word2index, '\n')


    def one_hot_encoding(word, word2index):
       one_hot_vector = [0]*(len(word2index))
       index = word2index[word]
       one_hot_vector[index] = 1
       return one_hot_vector

    one_hot_encoding("자연어",word2index)

    
    dog = torch.FloatTensor([1, 0, 0, 0, 0])
    cat = torch.FloatTensor([0, 1, 0, 0, 0])
    computer = torch.FloatTensor([0, 0, 1, 0, 0])
    netbook = torch.FloatTensor([0, 0, 0, 1, 0])
    book = torch.FloatTensor([0, 0, 0, 0, 1])

    print(torch.cosine_similarity(dog, cat, dim=0))
    print(torch.cosine_similarity(cat, computer, dim=0))
    print(torch.cosine_similarity(computer, netbook, dim=0))
    print(torch.cosine_similarity(netbook, book, dim=0), '\n')

def _02_Word2Vec():

    def Word2Vec_eng():
        urllib.request.urlretrieve("https://raw.githubusercontent.com/ukairia777/" \
                                    "tensorflow-nlp-tutorial/main/09.%20Word%20Embedding/" \
                                    "dataset/ted_en-20160408.xml", 
                                    filename="ted_en-20160408.xml")

        targetXML = open('ted_en-20160408.xml', 'r', encoding='UTF8')
        target_text = etree.parse(targetXML)

        parse_text = '\n'.join(target_text.xpath('//content/text()'))
        content_text = re.sub(r'\([^)]*\)', '', parse_text)

        sent_text = sent_tokenize(content_text)

        normalized_text = []
        
        for string in sent_text:
            tokens = re.sub(r"[^a-z0-9]+", " ", string.lower())
            normalized_text.append(tokens)

        result = [word_tokenize(sentence) for sentence in normalized_text]

        print('\n총 샘플의 개수 : {}'.format(len(result)), '\n')


        for line in result[:3]:
            print(line)


        model = Word2Vec(sentences=result, vector_size=100, 
                        window=5, min_count=5, workers=4, sg=0)
        model_result = model.wv.most_similar("man")
        print('\n', model_result)

        # ==== Word2Vec model save/load ==== #
        model.wv.save_word2vec_format('eng_w2v') 
        loaded_model = KeyedVectors.load_word2vec_format("eng_w2v") 

        model_result = loaded_model.most_similar("man")
        print(model_result)

        print('\n')
    # Word2Vec_eng()


    def Word2Vec_kor(): #colab 실습 (display)
        urllib.request.urlretrieve("https://raw.githubusercontent.com/" \
                                    "e9t/nsmc/master/ratings.txt", 
                                    filename="ratings.txt")
        train_data = pd.read_table('ratings.txt')

        display(train_data[:5])
        # print(train_data[:5])
        print('\n', len(train_data), '\n') 
        print(train_data.isnull().values.any(), '\n')

        train_data = train_data.dropna(how = 'any') 
        print(train_data.isnull().values.any(), '\n') 
        print(len(train_data), '\n')


        train_data['document'] = train_data['document'].str.replace(
                                "[^ㄱ-ㅎㅏ-ㅣ가-힣 ]","", regex=True
                                )

        display(train_data[:5])
        print('\n')
        # print(train_data[:5], '\n')


        stopwords = ['의','가','이','은','들','는','좀','잘','걍',
                     '과','도','를','으로','자','에','와','한','하다']

        okt = Okt()

        tokenized_data = []
        for sentence in tqdm(train_data['document']):
            tokenized_sentence = okt.morphs(sentence, stem=True) 
            stopwords_removed_sentence = [word for word in tokenized_sentence 
                                          if not word in stopwords] 
            tokenized_data.append(stopwords_removed_sentence)


        print('\n리뷰의 최대 길이 :',max(len(review) for review in tokenized_data))
        print('리뷰의 평균 길이 :',sum(map(len, tokenized_data))/len(tokenized_data))

        plt.hist([len(review) for review in tokenized_data], bins=50)
        plt.xlabel('length of samples')
        plt.ylabel('number of samples')
        plt.show()

        model = Word2Vec(sentences = tokenized_data, vector_size = 100, 
                         window = 5, min_count = 5, workers = 4, sg = 0)

        print(model.wv.vectors.shape, '\n')
        print(model.wv.most_similar("최민식"))
        print(model.wv.most_similar("히어로"))
    # Word2Vec_kor()


    def FastText_test(): #colab 실습 (display)
        urllib.request.urlretrieve("https://raw.githubusercontent.com/ukairia777/" \
                                            "tensorflow-nlp-tutorial/main/09.%20Word%20Embedding/" \
                                            "dataset/ted_en-20160408.xml", 
                                            filename="ted_en-20160408.xml")
        
        targetXML = open('ted_en-20160408.xml', 'r', encoding='UTF8')
        target_text = etree.parse(targetXML)

        parse_text = '\n'.join(target_text.xpath('//content/text()'))
        content_text = re.sub(r'\([^)]*\)', '', parse_text)

        sent_text = sent_tokenize(content_text)

        normalized_text = []
        
        for string in sent_text:
            tokens = re.sub(r"[^a-z0-9]+", " ", string.lower())
            normalized_text.append(tokens)

        result = [word_tokenize(sentence) for sentence in normalized_text]

        print('\n총 샘플의 개수 : {}'.format(len(result)), '\n')


        for line in result[:3]:
            print(line)


        model = FastText(result, vector_size=100, window=5, min_count=5, workers=4, sg=1)
        print('\n', model.wv.most_similar("electrofishing"))

        print('\n')
    FastText_test()


    def pretrained_Word2Vec(): # colab 실습
        urllib.request.urlretrieve("https://s3.amazonaws.com/dl4j-distribution/" \
                                    "GoogleNews-vectors-negative300.bin.gz", \
                                    filename="GoogleNews-vectors-negative300.bin.gz")
        word2vec_model = gensim.models.KeyedVectors.load_word2vec_format(
                            'GoogleNews-vectors-negative300.bin.gz', binary=True
                            )

        print(word2vec_model.vectors.shape)

        print(word2vec_model.similarity('this', 'is'))
        print(word2vec_model.similarity('post', 'book'))
        print(word2vec_model['book'])
    # pretrained_Word2Vec

def _03_Embedding():

    train_data = 'you need to know how to code'
    word_set = set(train_data.split())

    vocab = {word: i+2 for i, word in enumerate(word_set)}
    vocab['<unk>'] = 0
    vocab['<pad>'] = 1

    print(vocab, '\n')


    embedding_table = torch.FloatTensor([
                            [ 0.0,  0.0,  0.0],
                            [ 0.0,  0.0,  0.0],
                            [ 0.2,  0.9,  0.3],
                            [ 0.1,  0.5,  0.7],
                            [ 0.2,  0.1,  0.8],
                            [ 0.4,  0.1,  0.1],
                            [ 0.1,  0.8,  0.9],
                            [ 0.6,  0.1,  0.1]])
    
    sample = 'you need to run'.split()
    idxes = []

    for word in sample:
        try:
            idxes.append(vocab[word])
        except KeyError:
            idxes.append(vocab['<unk>'])
    idxes = torch.LongTensor(idxes)
    lookup_result = embedding_table[idxes, :]
    print(lookup_result, '\n')


    # ==== nn.Embedding() ==== #

    embedding_layer = nn.Embedding(num_embeddings=len(vocab), 
                                    embedding_dim=3,
                                    padding_idx=1)

    print(embedding_layer.weight, '\n')




#_01_One_hot_encoding()
#_02_Word2Vec()
_03_Embedding()