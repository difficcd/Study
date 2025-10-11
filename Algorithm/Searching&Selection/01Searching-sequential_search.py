
list = [1, 2, 3, 4, 5]
value = 3

# 순차 탐색 알고리즘 (sequential search)
def sequential_search(lst, value):
    for i in range(len(lst)):
        if lst[i] == value:
            return i
    return -1

print(sequential_search(list, value))




# 전위법(Move-to-Front)
def sequential_search_mtf(lst, value):
    for i in range(len(lst)):
        if lst[i] == value:
            # 찾은 원소를 맨 앞으로 이동
            lst.insert(0, lst.pop(i))
            return i
    return -1


# 전진이동법(Transpose)
def sequential_search_transpose(lst, value):
    for i in range(len(lst)):
        if lst[i] == value:
            # 찾은 원소를 바로 앞 원소와 교환
            if i > 0:
                lst[i], lst[i-1] = lst[i-1], lst[i]
            return i
    return -1


# 빈도계수법(Frequency Count)
def sequential_search_frequency(lst, value, freq):
    # freq : 각 원소별 검색 횟수를 저장하는 dictionary

    for i in range(len(lst)):
        if lst[i] == value:
            # 검색 횟수 증가
            freq[value] = freq.get(value, 0) + 1
          
            # 검색 횟수 기준으로 정렬 (내림차순)
            freq_list = []
            for x in lst:
                count = freq.get(x, 0)  # x의 검색 횟수, 없으면 0
                freq_list.append((x, count))  # (원소, 빈도) 튜플로 저장
  
            # 2. 빈도를 기준으로 내림차순 정렬
            freq_list.sort(key=lambda item: item[1], reverse=True)
  
            # 3. 정렬된 원소만 다시 lst에 저장
            lst[:] = [item[0] for item in freq_list]
            return i
    return -1


# 테스트
data_mtf = [1, 2, 3, 4, 5]
data_transpose = [1, 2, 3, 4, 5]
data_freq = [1, 2, 3, 4, 5]
freq = {} # 3: 1 이면 3을 1회 검색했다는 것

value = 3

print("전위법:", sequential_search_mtf(data_mtf, value))
print("MTF 결과  ", data_mtf)
print("\n")

print("전진이동법:", sequential_search_transpose(data_transpose, value))
print("Transpose 결과  ", data_transpose)
print("\n")

print("빈도 계수법:", sequential_search_frequency(data_freq, value, freq))
print("Frequency 결과  ", data_freq)
print("\n")

print("검색 빈도:", freq)
