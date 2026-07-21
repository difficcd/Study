import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d

# [PASSAGE 35] 목적 함수 정의 (로젠브록 함수 변형)
def f(x, y):
    return (1 + x)**2 + 100*(y - x**2)**2

# [PASSAGE 35] 그레디언트(편미분) 계산 함수
def gradient(x, y):
    # f를 x로 편미분: 2(1+x) + 100 * 2(y-x^2) * (-2x)
    dx = -400*x*y + 400*x**3 + 2*x + 2  # 소스 코드의 수식을 그대로 반영
    # f를 y로 편미분: 100 * 2(y-x^2) * (1)
    dy = 200*y - 200*x**2
    return np.array([dx, dy])

# [PASSAGE 35] 경사 하강법 알고리즘 구현
def GradientDescent(Grad, x, y, eta=0.0012, epsilon=0.001, nMax=1000):
    i = 0
    pos_x, pos_y, pos_count = np.empty(0), np.empty(0), np.empty(0)
    error = 1
    sol = np.array([x, y]) # 초기값 설정

    while np.linalg.norm(error) > epsilon and i < nMax:
        i += 1
        pos_x = np.append(pos_x, x)
        pos_y = np.append(pos_y, y)
        pos_count = np.append(pos_count, i)
        
        sol_prev = sol
        # 현재 x, y 기반으로 그레디언트 계산 후 업데이트
        sol = sol_prev - eta * Grad(x, y) 
        
        error = sol - sol_prev
        
        # [수정 포인트] x와 y를 sol의 0번, 1번 인덱스에서 가져옴
        x, y = sol[0], sol[1] 

    print('최저점의 위치: ', sol)
    return sol, pos_x, pos_y, pos_count

# [PASSAGE 36] 실행 및 시각화 로직
if __name__ == "__main__":
    # 초기값 (-2, 2)에서 시작
    solution, pos_x, pos_y, pos_count = GradientDescent(gradient, -2, 2)

    # 시각화를 위한 그리드 설정
    x = np.linspace(-2, 2, 200)
    y = np.linspace(-3, 3, 300)
    X, Y = np.meshgrid(x, y)
    Z = f(X, Y)

    # 3D 그래프 설정
    fig = plt.figure(figsize=(16, 7))
    ax = fig.add_subplot(1, 2, 1, projection='3d')
    ax.plot_surface(X, Y, Z, rstride=5, cstride=5, cmap='jet', alpha=.4)
    ax.plot(pos_x, pos_y, f(pos_x, pos_y), color='r', marker='.', alpha=.4)
    ax.set_title('f(x, y) 3D Surface')

    # 등고선(Contour) 지도 설정
    ax2 = fig.add_subplot(1, 2, 2)
    ax2.contour(X, Y, Z, 40, cmap='jet')
    ax2.scatter(pos_x, pos_y, color='r', marker='.')
    ax2.set_title(f'Contour map: {len(pos_count)} iterations')

    plt.show()