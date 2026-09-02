
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



def _01_seq2seq_translator():
    num_samples = 33000

    def unicode_to_ascii(s):
        return ''.join(c for c in unicodedata.normalize('NFD', s) 
                         if unicodedata.category(c) != 'Mn')

    def preprocess_sentence(sent):
        sent = unicode_to_ascii(sent.lower())
        sent = re.sub(r"([?.!,¿])", r" \1", sent)
        sent = re.sub(r"[^a-zA-Z!.?]+", r" ", sent)
        sent = re.sub(r"\s+", " ", sent)
        return sent

    def load_preprocessed_data():
        encoder_input, decoder_input, decoder_target = [], [], []

        with open("fra.txt", "r") as lines:
            for i, line in enumerate(lines):
                src_line, tar_line, _ = line.strip().split('\t')
                src_line = [w for w in preprocess_sentence(src_line).split()]
                tar_line = preprocess_sentence(tar_line)
                tar_line_in = [w for w in ("<sos> " + tar_line).split()]
                tar_line_out = [w for w in (tar_line + " <eos>").split()]

                encoder_input.append(src_line)
                decoder_input.append(tar_line_in)
                decoder_target.append(tar_line_out)

                if i == num_samples - 1:
                    break

        return encoder_input, decoder_input, decoder_target

    # 전처리 테스트
    en_sent = u"Have you had dinner?"
    fr_sent = u"Avez-vous déjà diné?"

    print('전처리 전 영어 문장 :', en_sent)
    print('전처리 후 영어 문장 :',preprocess_sentence(en_sent))
    print('전처리 전 프랑스어 문장 :', fr_sent)
    print('전처리 후 프랑스어 문장 :', preprocess_sentence(fr_sent))

    sents_en_in, sents_fra_in, sents_fra_out = load_preprocessed_data()
    print('인코더의 입력 :',sents_en_in[:5])
    print('디코더의 입력 :',sents_fra_in[:5])
    print('디코더의 레이블 :',sents_fra_out[:5])


    def build_vocab(sents):
        word_list = []

        for sent in sents:
            for word in sent:
                word_list.append(word)

        word_counts = Counter(word_list)
        vocab = sorted(word_counts, key=word_counts.get, reverse=True)

        word_to_index = {}
        word_to_index['<PAD>'] = 0
        word_to_index['<UNK>'] = 1

        for index, word in enumerate(vocab) :
            word_to_index[word] = index + 2

        return word_to_index


    src_vocab = build_vocab(sents_en_in)
    tar_vocab = build_vocab(sents_fra_in + sents_fra_out)

    src_vocab_size = len(src_vocab)
    tar_vocab_size = len(tar_vocab)
    print("영어 단어 집합의 크기 : {:d}, " \
    "프랑스어 단어 집합의 크기 : {:d}".format(src_vocab_size, tar_vocab_size))




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

    encoder_input = texts_to_sequences(sents_en_in, src_vocab)
    decoder_input = texts_to_sequences(sents_fra_in, tar_vocab)
    decoder_target = texts_to_sequences(sents_fra_out, tar_vocab)

    for i, (item1, item2) in zip(range(5), zip(sents_en_in, encoder_input)):
        print(f"Index: {i}, 정수 인코딩 전: {item1}, 정수 인코딩 후: {item2}")



    def pad_sequences(sentences, max_len=None):
        if max_len is None:
            max_len = max([len(sentence) for sentence in sentences])

        features = np.zeros((len(sentences), max_len), dtype=int)
        for index, sentence in enumerate(sentences):
            if len(sentence) != 0:
                features[index, :len(sentence)] = np.array(sentence)[:max_len]
        return features

    encoder_input = pad_sequences(encoder_input)
    decoder_input = pad_sequences(decoder_input)
    decoder_target = pad_sequences(decoder_target)


    print('인코더의 입력의 크기(shape) :',encoder_input.shape)
    print('디코더의 입력의 크기(shape) :',decoder_input.shape)
    print('디코더의 레이블의 크기(shape) :',decoder_target.shape)

    indices = np.arange(encoder_input.shape[0])
    np.random.shuffle(indices)
    print('랜덤 시퀀스 :',indices)

    encoder_input = encoder_input[indices]
    decoder_input = decoder_input[indices]
    decoder_target = decoder_target[indices]

    print([index_to_src[word] for word in encoder_input[30997]])
    print([index_to_tar[word] for word in decoder_input[30997]])
    print([index_to_tar[word] for word in decoder_target[30997]])





            