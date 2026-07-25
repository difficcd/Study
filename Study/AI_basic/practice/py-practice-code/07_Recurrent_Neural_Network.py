

import numpy as np

import torch
import torch.nn as nn
import torch.optim as optim
import numpy as np


def _01_RNN_basic():
    
    def RNN_implement(): # RNN 구조 구현방식 (nn.RNN())
        timesteps = 10 
        input_size = 4
        hidden_size = 8 

        inputs = np.random.random((timesteps, input_size)) 
        hidden_state_t = np.zeros((hidden_size,)) 
        print(hidden_state_t) 

        Wx = np.random.random((hidden_size, input_size))  # (8, 4)
        Wh = np.random.random((hidden_size, hidden_size)) # (8, 8)
        b = np.random.random((hidden_size,))              # (8,)

        print(np.shape(Wx))
        print(np.shape(Wh))
        print(np.shape(b))
        print("\n")

        total_hidden_states = []

        for input_t in inputs: 
            # 은닉 상태값 h_t = tanh (W_x * X_t + W_h * H_t + b)
            # 출력층 y_t = f(W_y * h_t + b)
            output_t = np.tanh(np.dot(Wx,input_t) + np.dot(Wh,hidden_state_t) + b) 
            total_hidden_states.append(list(output_t)) 
            print(np.shape(total_hidden_states)) 

        total_hidden_states = np.stack(total_hidden_states, axis = 0) 
        print(total_hidden_states) 
    # RNN_implement()

    input_size = 5
    hidden_size = 8 
    
    def RNN():
        # (batch_size, time_steps, input_size)
        inputs = torch.Tensor(1, 10, 5)
        cell = nn.RNN(input_size, hidden_size, batch_first=True)
        outputs, _status = cell(inputs)

        print("\n 모든 time-step의 hidden_state: ", outputs.shape)
        print(" 최종 time-step의 hidden_state: ", _status.shape)
    RNN()

    def DRNN(): # num_layers
        # (batch_size, time_steps, input_size)
        inputs = torch.Tensor(1, 10, 5)
        cell = nn.RNN(input_size = 5, hidden_size = 8, 
                      num_layers = 2, batch_first=True)
        outputs, _status = cell(inputs)

        print("\n 모든 time-step의 hidden_state: ", outputs.shape)
        print(" 최종 time-step의 hidden_state: ", _status.shape, '\n')
        # (층의 개수, 배치 크기, 은닉 상태의 크기)
    DRNN()

    def Bi_RNN(): # bidirectional = True
        # (batch_size, time_steps, input_size)
        inputs = torch.Tensor(1, 10, 5)
        cell = nn.RNN(input_size = 5, hidden_size = 8, num_layers = 2, 
                      batch_first=True, bidirectional = True)
        outputs, _status = cell(inputs)

        print(outputs.shape) # (배치 크기, 시퀀스 길이, 은닉 상태의 크기 x 2)
        print(_status.shape) # (층의 개수 x 2, 배치 크기, 은닉 상태의 크기)
    Bi_RNN()



def _02_CharRNN():

    def Basic_Test_RNN():
        input_str = 'apple'
        label_str = 'pple!'
        char_vocab = sorted(list(set(input_str+label_str)))
        vocab_size = len(char_vocab)
        print ('문자 집합의 크기 : {}'.format(vocab_size))

        input_size = vocab_size # 입력의 크기는 문자 집합의 크기
        hidden_size = 5
        output_size = 5
        learning_rate = 0.1

        char_to_index = dict((c, i) for i, c in enumerate(char_vocab)) 
        print(char_to_index)

        index_to_char = dict(enumerate(char_vocab)) 
        print(index_to_char, '\n')


        x_data = [char_to_index[c] for c in input_str]
        y_data = [char_to_index[c] for c in label_str]
        print("x data: ", x_data)
        print("y data: ", y_data, "\n")


        # 배치 차원 추가
        # 텐서 연산인 unsqueeze(0)를 통해 해결할 수도 있었음.
        x_data = [x_data]
        y_data = [y_data]
        print("x data: ", x_data)
        print("y data: ", y_data, "\n")


        x_one_hot = [np.eye(vocab_size)[x] for x in x_data]
        x_one_hot = np.array(x_one_hot)
        print(x_one_hot, "\n")    # 원 핫 인코딩

        X = torch.FloatTensor(x_one_hot)
        Y = torch.LongTensor(y_data)

        print('훈련 데이터의 크기 : {}'.format(X.shape))
        print('레이블의 크기 : {}'.format(Y.shape), "\n")

        class Net(torch.nn.Module):
            def __init__(self, input_size, hidden_size, output_size):
                super(Net, self).__init__()
                self.rnn = torch.nn.RNN(input_size, hidden_size, batch_first=True) 
                self.fc = torch.nn.Linear(hidden_size, output_size, bias=True) 

            def forward(self, x): # 구현한 RNN 셀과 출력층을 연결
                x, _status = self.rnn(x)
                x = self.fc(x)
                return x        # forward: fc 결과만 반환 (_status x)

        net = Net(input_size, hidden_size, output_size)
        outputs = net(X)
        print("outputs.shape: ", outputs.shape, "\n") 

        print(outputs.view(-1, input_size).shape, "\n") 

        print("Y shapes : ", Y.shape)
        print("1d Y shapes : ", Y.view(-1).shape, "\n")


        criterion = torch.nn.CrossEntropyLoss() # 다부류 분류, 교차 엔트로피
        optimizer = optim.Adam(net.parameters(), learning_rate)

        for i in range(100):
            optimizer.zero_grad()
            outputs = net(X)
            loss = criterion(outputs.view(-1, input_size), Y.view(-1))

            loss.backward() 
            optimizer.step() 
            
            result = outputs.data.numpy().argmax(axis=2)
            result_str = ''.join([index_to_char[c] for c in np.squeeze(result)])

            print(i, "loss: ", loss.item(), "prediction: ", result,
                    "true Y: ", y_data, "prediction str: ", result_str)
    # Basic_Test_RNN()

    def Basic_sentence_RNN():
        sentence = ("if you want to build a ship, don't drum up people together to "
            "collect wood and don't assign them tasks and work, but rather "
            "teach them to long for the endless immensity of the sea.")

        char_set = list(set(sentence)) 
        char_dic = {c: i for i, c in enumerate(char_set)} 
        print("char_dic\n", char_dic, '\n') 

        dic_size = len(char_dic)
        print('문자 집합의 크기 : {}'.format(dic_size), '\n')

        # 하이퍼파라미터 설정
        hidden_size = dic_size
        sequence_length = 10  # 임의 숫자 지정
        learning_rate = 0.1

        # 데이터 구성
        x_data = []
        y_data = []

        for i in range(0, len(sentence) - sequence_length):
            x_str = sentence[i:i + sequence_length]
            y_str = sentence[i + 1: i + sequence_length + 1]
            print(i, x_str, '->', y_str)

            x_data.append([char_dic[c] for c in x_str])  # x str to index
            y_data.append([char_dic[c] for c in y_str])  # y str to index

        print(x_data[0])
        print(y_data[0])

        x_one_hot = [np.eye(dic_size)[x] for x in x_data] # x 데이터는 원-핫 인코딩
        X = torch.FloatTensor(x_one_hot)
        Y = torch.LongTensor(y_data)

        print('훈련 데이터의 크기 : {}'.format(X.shape))
        print('레이블의 크기 : {}'.format(Y.shape), '\n')

        print("1st X sample\n", X[0], '\n')
        print(Y[0])

        class Net(torch.nn.Module): # 상단 예제랑 동일한 구조
            def __init__(self, input_dim, hidden_dim, layers):
                super(Net, self).__init__()
                self.rnn = torch.nn.RNN(input_dim, hidden_dim, 
                                        num_layers=layers, batch_first=True)
                self.fc = torch.nn.Linear(hidden_dim, hidden_dim, bias=True)

            def forward(self, x):
                x, _status = self.rnn(x)
                x = self.fc(x)
                return x
    
        net = Net(dic_size, hidden_size, 2) # 은닉층 2개

        criterion = torch.nn.CrossEntropyLoss() # 다부류이니 교차 엔트로피
        optimizer = optim.Adam(net.parameters(), learning_rate)

        outputs = net(X)
        print("outputs shape: ", outputs.shape) 

        print(outputs.view(-1, dic_size).shape) 

        print("Y shapes : ", Y.shape)
        print("1d Y shapes : ", Y.view(-1).shape, "\n")


        for i in range(100):
            optimizer.zero_grad()
            outputs = net(X) 
            loss = criterion(outputs.view(-1, dic_size), Y.view(-1))
            loss.backward()
            optimizer.step()

            results = outputs.argmax(dim=2)
            predict_str = ""
            for j, result in enumerate(results):
                if j == 0: 
                    predict_str += ''.join([char_set[t] for t in result])
                else: 
                    predict_str += char_set[result[-1]]

            print(predict_str)
    Basic_sentence_RNN()






# _01_RNN_basic()
_02_CharRNN()
