

list = [1,2,3,4,5]
value = 3

# 반복 구조 binary_search
def binary_search_linear(list, value, low, high) : 
  while low <= high :
    middle = (low + high) // 2
    if value == list[middle] :
      return middle
    elif value < list[middle] :
      high = middle-1
    else :
      low = middle+1 
  return -1 

result = binary_search_linear(list, value, 0, 4)
print("index:", result)




list = [1,2,3,4,5]
value = 3

# 재귀/순환 구조 binary_search
def binary_search_recursive(list, value, low, high) : 
  middle = (low + high) // 2
  if value == list[middle] :
    return middle
  elif value < list[middle] :
    binary_search_recursive(list, value, low, middle-1)
  else :
    binary_search_recursive(list, value, middle+1, high)
  return -1 

result = binary_search_recursive(list, value, 0, 4)
print("index:", result)






list = [1,2,3,4,5]
arr = list
value = 3

#보간탐색 알고리즘
def interpolation_search(lst, value, low, high):
  while low <= high and lst[low] != lst[high] and value >= lst[low] and value <= lst[high]:
      pos = low + (value - lst[low]) * (high - low) // (lst[high] - lst[low])

      if lst[pos] == value:
          return pos
      elif lst[pos] < value:
          low = pos + 1
      else:
          high = pos - 1

  # 마지막 확인
  if low <= high and lst[low] == value:
      return low
  return -1


result = interpolation_search(list, value, 0, 4)
print("index:", result)

