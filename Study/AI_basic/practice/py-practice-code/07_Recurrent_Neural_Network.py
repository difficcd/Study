

import numpy as np

import torch
import torch.nn as nn

def _01_RNN_basic():
    
    def RNN_implement():
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

    def DRNN():
        # (batch_size, time_steps, input_size)
        inputs = torch.Tensor(1, 10, 5)
        cell = nn.RNN(input_size = 5, hidden_size = 8, 
                      num_layers = 2, batch_first=True)
        outputs, _status = cell(inputs)

        print("\n 모든 time-step의 hidden_state: ", outputs.shape)
        print(" 최종 time-step의 hidden_state: ", _status.shape, '\n')
        # (층의 개수, 배치 크기, 은닉 상태의 크기)
    DRNN()

    def Bi_RNN():
        # (batch_size, time_steps, input_size)
        inputs = torch.Tensor(1, 10, 5)
        cell = nn.RNN(input_size = 5, hidden_size = 8, num_layers = 2, 
                      batch_first=True, bidirectional = True)
        outputs, _status = cell(inputs)

        print(outputs.shape) # (배치 크기, 시퀀스 길이, 은닉 상태의 크기 x 2)
        print(_status.shape) # (층의 개수 x 2, 배치 크기, 은닉 상태의 크기)
    Bi_RNN()





_01_RNN_basic()
