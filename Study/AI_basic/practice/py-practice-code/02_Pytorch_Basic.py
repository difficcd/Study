#### numpy tensor (비교용)
import numpy as np
import torch

def Tensor_Manipulation():

    ## (1) 1d tensor
    def _01_1d_tensor():
        t = np.array([0., 1., 2., 3., 4., 5., 6.])
        # 파이썬으로 설명하면 List를 생성해서 np.array로 1차원 array로 변환함.

        print(t)

        print('\nRank of t: ', t.ndim)
        print('Shape of t: ', t.shape)

        print('\nt[0] t[1] t[-1] = ', t[0], t[1], t[-1]) # 인덱스를 통한 원소 접근
        print('t[2:5] t[4:-1]  = ', t[2:5], t[4:-1])
                            # [시작 번호 : 끝 번호]로 범위 지정을 통해 가져온다
        print('t[:2] t[3:]     = ', t[:2], t[3:], '\n')
                            # 시작 번호를 생략한 경우와 끝 번호를 생략한 경우


    ## (2) 2d tensor
    def _02_2d_tensor():
        t = np.array([[1., 2., 3.], [4., 5., 6.],
                    [7., 8., 9.], [10., 11., 12.]])
        print(t)

        print('\nRank  of t: ', t.ndim)
        print('Shape of t: ', t.shape)


    #### torch tensor

    ## (1) 1d tensor
    def _t01_1d_tensor():
        t = torch.FloatTensor([0., 1., 2., 3., 4., 5., 6.])
        print(t)

        print('\n', t.dim())  # rank. 즉, 차원
        print(t.shape)  # shape
        print(t.size()) # shape

        print('\n', t[0], t[1], t[-1])  # 인덱스로 접근
        print(t[2:5], t[4:-1])    # 슬라이싱
        print(t[:2], t[3:])       # 슬라이싱



    ## (2) 2d tensor
    def _t02_2d_tensor():
        t = torch.FloatTensor([[1., 2., 3.],
                            [4., 5., 6.],
                            [7., 8., 9.],
                            [10., 11., 12.]
                            ])
        print(t)

        print('\n', t.dim())  # rank. 즉, 차원
        print(t.size()) # shape

        print('\n', t[:, 1])
        # 첫번째 차원을 전체 선택한 상황에서 두번째 차원의 첫번째 것만 가져온다.
        print(t[:, 1].size()) # ↑ 위의 경우의 크기

        print(t[:, :-1])



    ## (3) 브로드캐스팅 (크기 다른 행렬 연산)
    def _t03_broadcasting():
        m1 = torch.FloatTensor([[3, 3]])
        m2 = torch.FloatTensor([[2, 2]])
        print('\n', m1 + m2) # 크기 동일 행렬

        # Vector + scalar
        m1 = torch.FloatTensor([[1, 2]])
        m2 = torch.FloatTensor([3]) # [3] -> [3, 3] (shape (1,2))
        print('\n', m1 + m2) # 벡터 + 스칼라 연산


        # 2 x 1 Vector + 1 x 2 Vector
        m1 = torch.FloatTensor([[1, 2]])
        m2 = torch.FloatTensor([[3], [4]])
        print('\n', m1 + m2)



    ## (4) 자주 사용하는 기능

    # 1) 행렬 곱셈 (matmul, mul)
    def _t04_1_matmul_mul():
        m1 = torch.FloatTensor([[1, 2], [3, 4]])
        m2 = torch.FloatTensor([[1], [2]])
        print('\nShape of Matrix 1: ', m1.shape) # 2 x 2
        print('Shape of Matrix 2: ', m2.shape) # 2 x 1
        print(m1.matmul(m2)) # 2 x 1

            # 동일 크기 행렬 동일 위치 "원소곱" (mul)
        m1 = torch.FloatTensor([[1, 2], [3, 4]])
        m2 = torch.FloatTensor([[1], [2]])
        print('\nShape of Matrix 1: ', m1.shape) # 2 x 2
        print('Shape of Matrix 2: ', m2.shape) # 2 x 1
        print('\n', m1 * m2) # 2 x 2
        print('\n', m1.mul(m2))


    # 2) 평균 Mean
    def _t04_2_mean():
        t = torch.FloatTensor([1, 2]) # 1d mean
        print('\n', t.mean())

        t = torch.FloatTensor([[1, 2], [3, 4]]) # 2d mean
        print('\n',t)

        print('\n', t.mean(dim=0)) # 행 (차원) 제거 (열기준 mean)
        print(t.mean(dim=1)) # 열 제거 (행기준 mean)
        print(t.mean(dim=-1)) # row x "col" 제거 (= dim=1, 행기준 mean)


    # 3) 덧셈 Sum
    def _t04_3_sum():
        t = torch.FloatTensor([[1, 2], [3, 4]])
        print('\n', t)

        print(t.sum()) # 단순히 원소 전체의 덧셈을 수행
        print('\n', t.sum(dim=0)) # 행 (차원) 제거
        print(t.sum(dim=1)) # 열 제거
        print(t.sum(dim=-1)) # 열 제거


    # 4) 최대(Max), ArgMax
    def _t04_4_max_argmax():
        t = torch.FloatTensor([[1, 2], [3, 4]])
        print('\n', t)

        print('\n', t.max()) # Returns one value: max
        print('\n', t.max(dim=0)) # Returns two values: max and argmax
                # max에 dim 인자를 주면 argmax 도 함께 리턴: [1,1]
                # dim=0이니 열기준 max값이 리턴되어 3,4 반환
        print('\nMax: ', t.max(dim=0)[0])
        print('Argmax: ', t.max(dim=0)[1])

        print('\n', t.max(dim=1))
        print('\n', t.max(dim=-1))


    # 5) View ***
    def _t04_5_view():
        t = np.array([[[0, 1, 2],
                    [3, 4, 5]],
                    [[6, 7, 8],
                    [9, 10, 11]]])
        ft = torch.FloatTensor(t)
            # numpy arr: torch tensor로 변환 가능
            #  (실무에서 자주 쓰임: numpy로 전처리 후 학습 torch tensor 변환)

        print('\n', ft.shape)
            # 2,2,3 (2d tensor 개수(요소 수) x 2d_row개수 x 2d_col개수)

        print('\n', ft.view([-1, 3])) # ft라는 텐서를 (?, 3)의 크기로 변경
        print(ft.view([-1, 3]).shape) # 3d tensor => 2d tensor 변경 (shape:4x3)

        print('\n', ft.view([-1, 1, 3]))
        print(ft.view([-1, 1, 3]).shape) # 3d => 3d (배치 기준 : shape 바꾸기)


    # 6) Squeeze
    def _t04_6_squeeze():
        ft = torch.FloatTensor([[0], [1], [2]]) # 2d shape:(3,1)
        print('\n', ft)
        print('\n', ft.shape)

        print('\n', ft.squeeze())
        print('\n', ft.squeeze().shape)
            # 1인 차원 제거: (3,1)=>(3,)  (2d=>1d compaction)


    # 7) Unsqueeze
    def _t04_7_unsqueeze():
        ft = torch.Tensor([0, 1, 2]) # 1d shape:(3,)
        print('\n', ft.shape)

        print('\n', ft.unsqueeze(0)) # 0은 첫번째 차원을 의미! (dim과 같은 맥락)
        print('\n', ft.unsqueeze(0).shape) # row기준 []: [[0.,1.,2.]] (1x3)

        print('\n', ft.view(1, -1)) # view 로 unsqueeze 대체하기
        print('\n', ft.view(1, -1).shape)

        print('\n', ft.unsqueeze(1)) # col에 1 추가 : col기준 []
        print('\n', ft.unsqueeze(1).shape) # [[0],[1],[2]] (3x1)

            # view, squeeze, unsqueeze는 텐서 원소수 유지하며 모양/차원 조절


    # 8) Type Casting
    def _t04_8_type_casting():
        lt = torch.LongTensor([1, 2, 3, 4])
        print('\n', lt)
        print('\n', lt.float()) # 바로 Long=>float로 타입 변환됨.

        bt = torch.ByteTensor([True, False, False, True])
        print('\n', bt)

        print('\n', bt.long()) # Byte=>Long
        print('\n', bt.float()) # Byte=>Float


    # 9) 연결 (concatenate)
    def _t04_9_concatenate():
        x = torch.FloatTensor([[1, 2],
                            [3, 4]])

        y = torch.FloatTensor([[5, 6],
                            [7, 8]])

        print('\n\n', torch.cat([x, y], dim=0)) # row 기준으로 차원 늘리기: (4,2)
        print('\n', torch.cat([x, y], dim=1)) # col 기준으로 차원 늘리기: (2,4)


    # 10) 스택킹 (stacking) : 연결의 일종.
    def _t04_10_stacking():
        x = torch.FloatTensor([1, 4])
        y = torch.FloatTensor([2, 5])
        z = torch.FloatTensor([3, 6]) # x,y,z: 1d (2,) tensor (vec)

        print('\n\n', torch.stack([x, y, z])) # 벡터 쌓아 (3,2) 2d 변환
        print('\n', torch.cat([x.unsqueeze(0), y.unsqueeze(0), z.unsqueeze(0)], dim=0))
            # stack 연산은 cat&unsqueeze 의 축약 (default: row 기준 cat)
        print('\n', torch.stack([x, y, z], dim=1)) # col 기준 cat&unsqueeze(cat+1d=>2d)
                                            # (2,)끼리 엮어 (2,3) 2d 변환


    # 11) ones_like, zeros_like
    def _t04_11_ones_zeros_like():
        x = torch.FloatTensor([[0, 1, 2], [2, 1, 0]])
        print('\n\n', x)
        print('\n', torch.ones_like(x)) # 입력 텐서와 크기 동일, 값 1로 채우기
        print('\n', torch.zeros_like(x)) # 입력 텐서와 크기 동일, 값 0으로 채우기


    # 12) In-place Operation
    def _t04_11_In_place_Operation():
        x = torch.FloatTensor([[1, 2], [3, 4]])

        print(x.mul(2.)) # 곱하기 2를 수행한 결과를 출력
        print(x) # 기존의 값 출력

        print(x.mul_(2.))  
            # 곱하기 2를 수행한 결과를 변수 x에 값을 저장하면서 결과를 출력
        print(x) # 기존의 값 출력 (overwrite)

def Python_Class():
    
    def function_calculator():
        result = 0
        def add(num):
            global result
            result += num
            return result
        print(add(3)); print(add(4))

    def function_two_calculator():
        result1 = 0
        result2 = 0
        
        def add1(num):
            global result1
            result1 += num
            return result1
        def add2(num):
            global result2
            result2 += num
            return result2

        print(add1(3)); print(add1(4))
        print(add2(3)); print(add2(7))
    

    class Calculator:
        def __init__(self): # 생성자
            self.result = 0

        def add(self, num): # 객체 생성 후 사용할 수 있는 함수
            self.result += num
            return self.result
    
    cal1 = Calculator()
    cal2 = Calculator()

    print(cal1.add(3)); print(cal1.add(4))
    print(cal2.add(3)); print(cal2.add(7))