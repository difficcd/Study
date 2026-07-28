
from nltk.tokenize import word_tokenize
from nltk.tokenize import WordPunctTokenizer
from tensorflow.keras.preprocessing.text import text_to_word_sequence


def _01_tokenization():
    print('단어 토큰화1 :',
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


_01_tokenization()