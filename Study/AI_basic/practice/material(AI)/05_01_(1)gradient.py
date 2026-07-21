import numpy as np
import matplotlib.pyplot as plt

# 1. 원본 함수 정의
def f(x, y):
    return 2*x**2 + 4*x*y + 5*y**2 - 6*x + 2*y + 10

# 2. x에 대한 편미분 함수 (Gradient의 x성분)
def dx(x, y):
    return 4*x + 4*y - 6

# 3. y에 대한 편미분 함수 (Gradient의 y성분)
def dy(x, y):
    return 4*x + 10*y + 2

# 4. 등고선(Contour)을 위한 데이터 생성
xi = np.linspace(-5, 20, 100)
yi = np.linspace(-6, 6, 100)
X, Y = np.meshgrid(xi, yi)
Z = f(X, Y)

# 5. 화살표(Quiver)를 위한 데이터 생성 (격자를 더 성기게 설정)
xj = np.linspace(-5, 20, 13)
yj = np.linspace(-6, 6, 7)
X1, Y1 = np.meshgrid(xj, yj)
Dx = dx(X1, Y1)
Dy = dy(X1, Y1)

# 6. 시각화
plt.figure(figsize=(10, 5))

# 등고선 그리기 (logspace를 이용해 레벨 설정)
plt.contour(X, Y, Z, levels=np.logspace(0, 3, 10))

# 그레디언트 벡터 화살표 그리기
plt.quiver(X1, Y1, Dx, Dy, color='red', scale=500, minshaft=4)

plt.xlabel('x')
plt.ylabel('y')
plt.show()