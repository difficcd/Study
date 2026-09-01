


from nltk import word_tokenize, pos_tag, ne_chunk

sentence = "James is working at Disney in London"
tokenized_sentence = pos_tag(word_tokenize(sentence))
print(tokenized_sentence)

