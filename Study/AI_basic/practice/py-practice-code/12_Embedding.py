
import torch
import torch.nn as nn
import torch.optim as optim
from torch.optim import Adam
from torch.utils.data import DataLoader, TensorDataset

import re
import urllib.request
import pandas as pd
import matplotlib.pyplot as plt
import gensim
import nltk
import numpy as np

from collections import Counter
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

def _04_Pretrained_Embedding():

    """ sentiment analysis """

    sentences = ['nice great best amazing', 'stop lies', 
                'pitiful nerd', 'excellent work', 'supreme quality', 
                'bad', 'highly respectable']
    y_train = [1, 0, 0, 1, 1, 0, 1]

    tokenized_sentences = [sent.split() for sent in sentences]
    print('\n단어 토큰화 된 결과 :', tokenized_sentences,'\n')

    word_list = []
    for sent in tokenized_sentences:
        for word in sent:
            word_list.append(word)
            
    word_counts = Counter(word_list)
    print('총 단어수 :', len(word_counts), '\n')

    vocab = sorted(word_counts, key=word_counts.get, reverse=True)
    print(vocab,'\n')


    word_to_index = {}
    word_to_index['<PAD>'] = 0
    word_to_index['<UNK>'] = 1

    for index, word in enumerate(vocab) :
        word_to_index[word] = index + 2

    vocab_size = len(word_to_index)
    print('패딩 토큰, UNK 토큰을 고려한 단어 집합의 크기 :', vocab_size, '\n')
    print(word_to_index, '\n')

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

    X_encoded = texts_to_sequences(tokenized_sentences, word_to_index)
    print('X_encoded: ', X_encoded)

    max_len = max(len(l) for l in X_encoded)
    print('최대 길이 :',max_len,'\n')

    device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')


    def pad_sequences(sentences, max_len):
        features = np.zeros((len(sentences), max_len), dtype=int)

        for index, sentence in enumerate(sentences):
            if len(sentence) != 0:
                features[index, :len(sentence)] = np.array(sentence)[:max_len]
        return features

    X_train = pad_sequences(X_encoded, max_len=max_len)
    y_train = np.array(y_train)

    print('패딩 결과 :')
    print(X_train, '\n')


    def normal_embedding():

        class SimpleModel(nn.Module):
            def __init__(self, vocab_size, embedding_dim):
                super(SimpleModel, self).__init__()
                self.embedding = nn.Embedding(vocab_size, embedding_dim)
                self.flatten = nn.Flatten()
                self.fc = nn.Linear(embedding_dim * max_len, 1)
                self.sigmoid = nn.Sigmoid()

            def forward(self, x):
                embedded = self.embedding(x)
                flattened = self.flatten(embedded)

                output = self.fc(flattened)
                return self.sigmoid(output)


        embedding_dim = 100
        simple_model = SimpleModel(vocab_size, embedding_dim).to(device)

        criterion = nn.BCELoss()
        optimizer = Adam(simple_model.parameters())

        train_dataset = TensorDataset(torch.tensor(X_train, dtype=torch.long), 
                                    torch.tensor(y_train, dtype=torch.float32))
        train_dataloader = DataLoader(train_dataset, batch_size=2)

        print(len(train_dataloader), '\n')

        for epoch in range(10):
            for inputs, targets in train_dataloader:
                inputs, targets = inputs.to(device), targets.to(device)

                optimizer.zero_grad()

                outputs = simple_model(inputs).view(-1) 

                loss = criterion(outputs, targets)
                loss.backward()

                optimizer.step()

            print(f"Epoch {epoch+1}, Loss: {loss.item()}")
        print('\n')
    # normal_embedding()


    def pretrained_embedding(): # colab
        """
        !pip install gdown
        !gdown https://drive.google.com/uc?id=1Av37IVBQAAntSe1X3MOAl5gvowQzd2_j
        """
        word2vec_model = gensim.models.KeyedVectors.load_word2vec_format(
                        'GoogleNews-vectors-negative300.bin.gz', binary=True
                        ) 
        
        embedding_matrix = np.zeros((vocab_size, 300))
        print('\n임베딩 행렬의 크기 :', embedding_matrix.shape, '\n')

        def get_vector(word):
            if word in word2vec_model:
                return word2vec_model[word]
            else:
                return None

        for word, i in word_to_index.items():
            if i > 2:
                temp = get_vector(word)
                if temp is not None:
                    embedding_matrix[i] = temp

        print(embedding_matrix[0], '\n')
        word_to_index['great']

        np.all(word2vec_model['great'] == embedding_matrix[3])

        class PretrainedEmbeddingModel(nn.Module):
            def __init__(self, vocab_size, embedding_dim):
                super(PretrainedEmbeddingModel, self).__init__()
                self.embedding = nn.Embedding(vocab_size, embedding_dim)
                self.embedding.weight = nn.Parameter(torch.tensor(
                                        embedding_matrix, dtype=torch.float32))
                self.embedding.weight.requires_grad = True

                self.flatten = nn.Flatten()

                self.fc = nn.Linear(embedding_dim * max_len, 1)
                self.sigmoid = nn.Sigmoid()

            def forward(self, x):
                embedded = self.embedding(x)
                flattened = self.flatten(embedded)
                output = self.fc(flattened)
                return self.sigmoid(output)


        pretraiend_embedding_model = PretrainedEmbeddingModel(vocab_size, 300).to(device)

        criterion = nn.BCELoss()
        optimizer = Adam(pretraiend_embedding_model.parameters())


        train_dataset = TensorDataset(torch.tensor(X_train, dtype=torch.long), 
                                      torch.tensor(y_train, dtype=torch.float32))
        train_dataloader = DataLoader(train_dataset, batch_size=2)

        print(len(train_dataloader), '\n')


        for epoch in range(10):
            for inputs, targets in train_dataloader:
                inputs, targets = inputs.to(device), targets.to(device)

                optimizer.zero_grad()

                outputs = pretraiend_embedding_model(inputs).view(-1) 

                loss = criterion(outputs, targets)
                loss.backward()

                optimizer.step()

            print(f"Epoch {epoch+1}, Loss: {loss.item()}")
    pretrained_embedding()

def _05_word_RNN_Embedding():
    sentence = "Repeat is the best medicine for memory".split()

    vocab = list(set(sentence))
    print('\nvocab: ', vocab, '\n')


    word2index = {tkn: i for i, tkn in enumerate(vocab, 1)} 
    word2index['<unk>']=0
    print('word2index: ', word2index, '\n')
    # print(word2index['memory'], '\n')

    index2word = {v: k for k, v in word2index.items()}
    print('index2word: ', index2word, '\n')
    # print(index2word[2], '\n')


    def build_data(sentence, word2index):
        encoded = [word2index[token] for token in sentence]  

        input_seq, label_seq = encoded[:-1], encoded[1:] 
        input_seq = torch.LongTensor(input_seq).unsqueeze(0) 
        label_seq = torch.LongTensor(label_seq).unsqueeze(0) 

        return input_seq, label_seq

    X, Y = build_data(sentence, word2index)

    print(X)
    print(Y, '\n')


    class Net(nn.Module):
        def __init__(self, vocab_size, input_size, hidden_size, batch_first=True):
            super(Net, self).__init__()
            self.embedding_layer = nn.Embedding(num_embeddings=vocab_size, 
                                                embedding_dim=input_size)
            self.rnn_layer = nn.RNN(input_size, hidden_size,
                                    batch_first=batch_first)
            self.linear = nn.Linear(hidden_size, vocab_size) 

        def forward(self, x):
            
            output = self.embedding_layer(x)
            output, hidden = self.rnn_layer(output)
            output = self.linear(output)
        
            return output.view(-1, output.size(2))

    vocab_size = len(word2index)  
    input_size = 5  
    hidden_size = 20 

    model = Net(vocab_size, input_size, hidden_size, batch_first=True)
    loss_function = nn.CrossEntropyLoss() 
    optimizer = optim.Adam(params=model.parameters())

    output = model(X) 
    print('output test:\n',output,'\n')
    print('output shape: ', output.shape, '\n')

    decode = lambda y: [index2word.get(x) for x in y]

    for step in range(201):

        optimizer.zero_grad()

        output = model(X)

        loss = loss_function(output, Y.view(-1))
        loss.backward()

        optimizer.step()

        if step % 40 == 0:
            print("[{:02d}/201] {:.4f} ".format(step+1, loss))

            pred = output.softmax(-1).argmax(-1).tolist()

            print(" ".join(["Repeat"] + decode(pred)))
            print()


    # 임베딩 벡터 test 출력
    word_idx = word2index['memory']
    memory_vector = model.embedding_layer.weight[word_idx]
    print("\n'memory'의 학습된 임베딩 벡터:")
    print(memory_vector)



#_01_One_hot_encoding()
#_02_Word2Vec()
#_03_Embedding()
#_04_Pretrained_Embedding()
_05_word_RNN_Embedding()