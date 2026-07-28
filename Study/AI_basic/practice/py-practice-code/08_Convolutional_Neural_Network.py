
import torch
import torch.nn as nn

import torchvision.datasets as dsets
import torchvision.transforms as transforms
import torch.nn.init


def CNN_Basic():

    def CNN_model():
        # 배치 크기 × 채널 × 높이(height) × 너비(widht) 크기 텐서 선언
        inputs = torch.Tensor(1, 1, 28, 28)
        print('\n 텐서의 크기 : {}'.format(inputs.shape))

        conv1 = nn.Conv2d(1, 32, 3, padding=1)
        conv2 = nn.Conv2d(32, 64, kernel_size=3, padding=1)
        pool = nn.MaxPool2d(2)      # kernel_size = stride = 2

        print("\n", conv1, "\n", conv2, "\n")
        print("\n", pool , "\n")



        out = conv1(inputs)  # inputs -> conv1 -> out1
        print(out.shape)     # 32 channel (28,28) tensor

        out = pool(out)      # out1 -> pool -> out2
        print(out.shape)     # 32 channel (14,14) tensor

        out = conv2(out)     # out2 -> conv2 -> out3
        print(out.shape)     # 64 channel (14,14) tensor

        out = pool(out)      # out3 -> pool -> out4
        print(out.shape)     # 64 channel (7,7) tensor

        out = out.view(out.size(0), -1) 
        print(out.shape)
        # 첫번째 차원인 배치 차원은 그대로 두고 나머지는 펼치기

        fc = nn.Linear(3136, 10)  # input_dim = 3,136, output_dim = 10
        out = fc(out)
        print(out.shape)
    # CNN_model()

    def CNN_Basic_MNIST():

        device = 'cuda' if torch.cuda.is_available() else 'cpu'
        torch.manual_seed(777)

        if device == 'cuda':
            torch.cuda.manual_seed_all(777)

        learning_rate = 0.001
        training_epochs = 15
        batch_size = 100

        mnist_train = dsets.MNIST(root='MNIST_data/', train=True, 
                                  transform=transforms.ToTensor(), 
                                  download=True)

        mnist_test = dsets.MNIST(root='MNIST_data/', train=False, 
                                transform=transforms.ToTensor(), 
                                download=True)

        data_loader = torch.utils.data.DataLoader(dataset=mnist_train,
                                        batch_size=batch_size,
                                        shuffle=True, drop_last=True)

        # 모델 설계 (2 Conv, 1 FC)
        class CNN(torch.nn.Module):

            def __init__(self):
                super(CNN, self).__init__()
                self.layer1 = torch.nn.Sequential(
                    torch.nn.Conv2d(1, 32, 
                                    kernel_size=3, stride=1, padding=1),
                    torch.nn.ReLU(),
                    torch.nn.MaxPool2d(kernel_size=2, stride=2)
                    )

                self.layer2 = torch.nn.Sequential(
                    torch.nn.Conv2d(32, 64, 
                                    kernel_size=3, stride=1, padding=1),
                    torch.nn.ReLU(),
                    torch.nn.MaxPool2d(kernel_size=2, stride=2)
                    )

                self.fc = torch.nn.Linear(7 * 7 * 64, 10, bias=True)
                torch.nn.init.xavier_uniform_(self.fc.weight) 

            def forward(self, x):
                out = self.layer1(x)
                out = self.layer2(out)
                out = out.view(out.size(0), -1) 
                out = self.fc(out)
                return out

        # 모델 설계 (3 Conv, 2 FC)
        class Deep_CNN(torch.nn.Module):

            def __init__(self):
                super(Deep_CNN, self).__init__()
                self.keep_prob = 0.5

                self.layer1 = torch.nn.Sequential(
                    torch.nn.Conv2d(1, 32, 
                                    kernel_size=3, stride=1, padding=1),
                    torch.nn.ReLU(),
                    torch.nn.MaxPool2d(kernel_size=2, stride=2)
                    )

                self.layer2 = torch.nn.Sequential(
                    torch.nn.Conv2d(32, 64, 
                                    kernel_size=3, stride=1, padding=1),
                    torch.nn.ReLU(),
                    torch.nn.MaxPool2d(kernel_size=2, stride=2)
                    )

                self.layer3 = torch.nn.Sequential(
                    torch.nn.Conv2d(64, 128, 
                                    kernel_size=3, stride=1, padding=1),
                    torch.nn.ReLU(),
                    torch.nn.MaxPool2d(kernel_size=2, stride=2, padding=1)
                    )

                self.fc1 = torch.nn.Linear(4 * 4 * 128, 625, bias=True)
                torch.nn.init.xavier_uniform_(self.fc1.weight) 

                self.layer4 = torch.nn.Sequential(
                    self.fc1,
                    torch.nn.ReLU(),
                    torch.nn.Dropout(p=1 - self.keep_prob)
                )

                self.fc2 = torch.nn.Linear(625, 10, bias=True)
                torch.nn.init.xavier_uniform_(self.fc2.weight) 

            def forward(self, x):
                out = self.layer1(x)
                out = self.layer2(out)
                out = self.layer3(out)
                out = out.view(out.size(0), -1) 
                out = self.layer4(out)
                out = self.fc2(out)
                return out

        
        # model = CNN().to(device)
        model = Deep_CNN().to(device)

        criterion = torch.nn.CrossEntropyLoss().to(device)   
        optimizer = torch.optim.Adam(model.parameters(), lr=learning_rate)

        total_batch = len(data_loader)
        print('총 배치의 수 : {}'.format(total_batch))

        for epoch in range(training_epochs): # TRAIN
            avg_cost = 0 

            for X, Y in data_loader:  
                X = X.to(device)  
                Y = Y.to(device) 

                optimizer.zero_grad()
                hypothesis = model(X) 
                cost = criterion(hypothesis, Y) 
                cost.backward()  
                optimizer.step()  

                avg_cost += cost / total_batch 

            print('[Epoch: {:>4}] cost = {:>.9}'.format(epoch + 1, avg_cost))

     
        with torch.no_grad(): # TEST
            
            X_test = mnist_test.test_data.view(
                len(mnist_test), 1, 28, 28).float().to(device) 
            Y_test = mnist_test.test_labels.to(device)  

            prediction = model(X_test) 
            correct_prediction = torch.argmax(prediction, 1) == Y_test  

            accuracy = correct_prediction.float().mean()  
            print('Accuracy:', accuracy.item())
    CNN_Basic_MNIST()


CNN_Basic()
