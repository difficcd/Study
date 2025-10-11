

#quicksort partition과 완전히 동일한 알고리즘
#직접 정렬이 아니라, 부분적으로 값을 재조정하는 과정임
def partition(A, left, right):
  pivot = A[right]  # 맨 오른쪽 원소를 피벗으로 선택
  i = left - 1
  for j in range(left, right):
      if A[j] <= pivot:
          i += 1
          A[i], A[j] = A[j], A[i] # 작으면 swap 
  A[i+1], A[right] = A[right], A[i+1] # pivot 제자리에 가게 swap
  return i + 1  # 피벗 위치 반환


# 평균 선형시간 선택 알고리즘 
def quickselect(A, left, right, k):
  if left == right:
      return A[left]
  pos = partition(A, left, right) # 분할하고 피벗 위치 받음

  # pos의 위치가 k번째 원소인지 확인
  if pos == left + k - 1:
      return A[pos]
  elif pos > left + k - 1:
      return quickselect(A, left, pos - 1, k)
  else:
      return quickselect(A, pos + 1, right, k - (pos - left + 1))
    
#pos가 k번째 원소라면 바로 반환 
#pos가 k보다 크면 → 왼쪽 부분 배열만 재귀
#pos가 k보다 작으면 → 오른쪽 부분 배열만 재귀
# Lecture 5 - 17page 참조 ::

#    11 8 15 3 20 29
#    3 8 11 15 20 29  >> 8 ...

arr = [7, 10, 4, 3, 20, 15]
k = 3
result = quickselect(arr, 0, len(arr)-1, k)
print(f"{k} 번째로 작은 원소:", result)
