
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d
from cvxopt import matrix, solvers

# ==========================================
# 2. 목적 함수 및 그레디언트 정의 
# [Slide 12] 프로그래밍 실습: 경사 하강법
# ==========================================

def f(x, y):
    """
    목적 함수 정의: f(x, y) = (1 + x)^2 + 100 * (y - x^2)^2
    이 함수는 전역 최솟값을 찾기 까다로운 Rosenbrock 함수와 유사한 형태를 가집니다.
    """
    return (1 + x)**2 + 100 * (y - x**2)**2

def gradient(x, y):
    """
    그레디언트(편미분 벡터) 정의:
    dx = ∂f/∂x = -400xy + 400x^3 + 2x + 2   # [버그 수정] 상수항 -2 -> +2
    dy = ∂f/∂y = 200y - 200x^2
    """
    # [버그 수정] f = (1+x)^2 + 100(y-x^2)^2 이므로
    #   ∂f/∂x = 2(1+x) - 400x(y-x^2) = 2x + 2 - 400xy + 400x^3
    #   기존 코드는 상수항이 -2 로 잘못돼 엉뚱한 점으로 수렴했음 (정답은 x=-1, y=1)
    dx = -400 * x * y + 400 * x**3 + 2 * x + 2
    dy = 200 * y - 200 * x**2
    return np.array([dx, dy])

# ==========================================
# 3. 경사 하강법 알고리즘 구현 
# [Slide 12] 프로그래밍 실습: 경사 하강법
# ==========================================

def GradientDescent(Grad, x, y, eta=0.0012, epsilon=0.001, nMax=1000):
    """
    경사 하강법 핵심 알고리즘.
    교육적 관점:
    - np.append를 사용한 데이터 수집은 실습 코드의 원형을 유지하기 위함입니다.
    - 대규모 데이터 처리 시에는 Python 리스트를 사용한 후 최종적으로 변환하는 것이 효율적입니다.
    """
    i = 0
    pos_x, pos_y, pos_count = np.empty(0), np.empty(0), np.empty(0)
    error = 1
    sol = np.array([x, y])

    # 수렴 조건(Convergence Criteria): 위치 변화량(error)이 허용 오차(epsilon)보다 작아질 때까지 반복
    while np.linalg.norm(error) > epsilon and i < nMax:
        i += 1
        
        # 현재 위치 데이터 기록
        pos_x = np.append(pos_x, x)
        pos_y = np.append(pos_y, y)
        pos_count = np.append(pos_count, i)
        
        sol_prev = sol
        # 해 업데이트 로직: 현재 위치에서 그레디언트 반대 방향으로 학습률(eta)만큼 이동
        # x(t+1) = x(t) - η * ∇f(x(t))
        sol = sol - eta * Grad(x, y)
        
        # 오차 계산: 이전 위치와 현재 위치의 유클리드 거리를 통해 수렴 여부 판단
        error = sol - sol_prev
        
        # 좌표 업데이트
        x, y = sol[0], sol[1]

    print('최저점의 위치(Solution): ', sol)
    return sol, pos_x, pos_y, pos_count

# ==========================================
# 4 & 5. 알고리즘 실행 및 시각화 
# [Slide 15] 프로그래밍 실습: 경사 하강법
# ==========================================

def run_gradient_descent_with_plot():
    # 1단계: 알고리즘 실행 (시작점 -2, 2)
    solution, pos_x, pos_y, pos_count = GradientDescent(gradient, -2, 2)

    # 2단계: 배경 그리드 데이터 생성 (x: -2~2, y: -3~3)
    x_range = np.linspace(-2, 2, 200)
    y_range = np.linspace(-3, 3, 300)
    X, Y = np.meshgrid(x_range, y_range)
    Z = f(X, Y)

    # 3단계: 탐색 경로 시각화를 위한 방향 벡터(Quiver) 계산
    anglesx = pos_x[1:] - pos_x[:-1]
    anglesy = pos_y[1:] - pos_y[:-1]

    # 4단계: 시각화 구현
    fig = plt.figure(figsize=(16, 7))

    # (1) 3D 표면 플롯 (3D Surface Plot)
    ax1 = fig.add_subplot(1, 2, 1, projection='3d')
    # rstride, cstride를 5로 설정하여 슬라이드 15의 격자 밀도를 재현합니다.
    ax1.plot_surface(X, Y, Z, rstride=5, cstride=5, cmap='jet', alpha=0.4)
    ax1.plot(pos_x, pos_y, f(pos_x, pos_y), color='r', marker='.', alpha=0.4, label='Path')
    ax1.view_init(50, 280)  # 원본 자료와 동일한 시점 설정
    ax1.set_xlabel('x')
    ax1.set_ylabel('y')
    ax1.set_title('f(x,y) 3D Surface Path')

    # (2) 등고선 지도 (Contour Map)
    ax2 = fig.add_subplot(1, 2, 2)
    ax2.contour(X, Y, Z, 40, cmap='jet')
    ax2.scatter(pos_x, pos_y, color='r', marker='.')
    
    # 교육적 보완: 반복 횟수가 많을 경우 화살표가 겹치지 않도록 일정 간격(thinning)으로 시각화합니다.
    step = 1 if len(pos_x) < 50 else len(pos_x) // 40
    ax2.quiver(pos_x[:-1:step], pos_y[:-1:step], anglesx[::step], anglesy[::step], 
               scale_units='xy', angles='xy', scale=1, color='r', width=0.005)
    
    ax2.set_title('Contour Map: {} Iterations'.format(len(pos_count)))
    ax2.set_xlabel('x')
    ax2.set_ylabel('y')
    
    plt.show()

# ==========================================
# 6. 제약 조건 최적화 및 이차 계획법 
# [Slide 26] 이차 계획법 문제 패키지
# ==========================================

def solve_quadratic_programming():
    """
    cvxopt 패키지를 활용한 이차 계획법(QP) 문제 해결.
    
    목적함수: 1/2 * (x1^2 + x2^2)
    제약조건:
      1) x1 + x2 = 1 (등식 제약: 1 - x1 - x2 = 0)
      2) -x2 <= -3/4 (부등식 제약: 3/4 - x2 <= 0)
    """
    
    # H (Hessian matrix): 목적함수의 2차항 계수
    H = matrix([[1.0, 0.0], [0.0, 1.0]])
    
    # f: 목적함수의 1차항 계수
    f = matrix([0.0, 0.0])
    
    # 부등식 제약 조건 (A * α <= a)
    # Slide 26 기준: A = [[0, 0], [0, -1]], a = [[0], [-3/4]]
    # CVXOPT의 matrix는 열(column) 우선 방식으로 데이터를 처리함에 유의하십시오.
    A = matrix([[0.0, 0.0], [0.0, -1.0]]) 
    a = matrix([0.0, -0.75])
    
    # 등식 제약 조건 (B * α = b)
    # x1 + x2 = 1  ->  B 는 1행 2열(변수 2개) 이어야 함.
    # FIX: matrix([[1.0, 1.0]]) 는 CVXOPT 열 우선 규칙상 2x1 로 만들어져
    #      "'A' must be a 'd' matrix with 2 columns" 에러 발생.
    #      각 원소를 별도 열로 넣어 1x2 로 생성한다.
    B = matrix([[1.0], [1.0]])
    b = matrix([1.0])
    
    # 솔버 호출 및 결과 출력
    sol = solvers.qp(H, f, A, a, B, b)
    
    print("\n[이차 계획법(QP) 최적화 결과]")
    # 결과 해석: sol['x']는 최적해 변수 벡터를 반환합니다.
    print(sol['x']) # 최적해 x_1 근사치: 0.2499998949427826 (Slide 26 참조)

# ==========================================
# 7. 메인 실행 블록
# ==========================================

if __name__ == "__main__":
    print("="*50)
    print("실습 1: 경사 하강법(Gradient Descent) 시뮬레이션")
    print("="*50)
    run_gradient_descent_with_plot()
    
    print("\n" + "="*50)
    print("실습 2: 제약 조건 최적화(CVXOPT Quadratic Programming)")
    print("="*50)
    solve_quadratic_programming()
