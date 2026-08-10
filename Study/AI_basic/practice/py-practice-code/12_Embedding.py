
import torch

from konlpy.tag import Okt  
okt = Okt()  

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




_01_One_hot_encoding()