from durable.lang import *

# --- 1. Hello World 실습 (PASSAGE 69) ---
with ruleset('testRS'):
    @when_all(m.subject == 'World')
    def say_hello(c):
        print('Hello {0}'.format(c.m.subject))

post('testRS', { 'subject': 'World' })


# --- 2. 동물 분류 규칙 (PASSAGE 70, 71) ---
with ruleset('animal'):
    # 개구리 판별 및 사실 추가
    @when_all(c.first << (m.predicate == 'eats') & (m.object == 'flies'),
              (m.predicate == 'lives') & (m.object == 'water') & (m.subject == c.first.subject))
    def frog(c):
        c.assert_fact({ 'subject': c.first.subject, 'predicate': 'is', 'object': 'frog' })

    # 카멜레온 판별
    @when_all(c.first << (m.predicate == 'eats') & (m.object == 'flies'),
              (m.predicate == 'lives') & (m.object == 'land') & (m.subject == c.first.subject))
    def chameleon(c):
        c.assert_fact({ 'subject': c.first.subject, 'predicate': 'is', 'object': 'chameleon' })

    # 새 판별
    @when_all((m.predicate == 'eats') & (m.object == 'worms'))
    def bird(c):
        c.assert_fact({ 'subject': c.m.subject, 'predicate': 'is', 'object': 'bird' })

    # 하위 규칙: 종에 따른 색상 추론
    @when_all((m.predicate == 'is') & (m.object == 'frog'))
    def green(c):
        c.assert_fact({ 'subject': c.m.subject, 'predicate': 'is', 'object': 'green' })

    @when_all((m.predicate == 'is') & (m.object == 'chameleon'))
    def grey(c):
        c.assert_fact({ 'subject': c.m.subject, 'predicate': 'is', 'object': 'grey' })

    @when_all((m.predicate == 'is') & (m.object == 'bird'))
    def black(c):
        c.assert_fact({ 'subject': c.m.subject, 'predicate': 'is', 'object': 'black' })

    # 출력 규칙
    @when_all(+m.subject)
    def output(c):
        print('Fact: {0} {1} {2}'.format(c.m.subject, c.m.predicate, c.m.object))

# 데이터 입력
assert_fact('animal', { 'subject': 'Kermit', 'predicate': 'eats', 'object': 'flies' })
assert_fact('animal', { 'subject': 'Kermit', 'predicate': 'lives', 'object': 'water' })
assert_fact('animal', { 'subject': 'Greedy', 'predicate': 'eats', 'object': 'flies' })
assert_fact('animal', { 'subject': 'Greedy', 'predicate': 'lives', 'object': 'land' })
assert_fact('animal', { 'subject': 'Tweety', 'predicate': 'eats', 'object': 'worms' })


# --- 3. 이상거래 탐지 (PASSAGE 72) ---
with ruleset('risk'):
    @when_all(c.first << m.t == 'purchase',
              c.second << m.location != c.first.location)
    def fraud(c):
        print('이상거래 탐지 -> {0}, {1}'.format(c.first.location, c.second.location))

post('risk', { 't': 'purchase', 'location': 'US'})
post('risk', { 't': 'purchase', 'location': 'CA'})


# --- 4. 서점 관리 및 사실 삭제 (PASSAGE 73, 74) ---
with ruleset('bookstore'):
    @when_all(+m.status)
    def event(c):
        print('bookstore-> Reference {0} status {1}'.format(c.m.reference, c.m.status))

    @when_all(+m.name)
    def fact(c):
        print('bookstore-> Added "{0}"'.format(c.m.name))

    @when_all(none(+m.name))
    def empty(c):
        print('bookstore-> No books')

# 사실 추가 및 삭제 테스트
assert_fact('bookstore', {
    'name': 'The new book',
    'seller': 'bookstore',
    'reference': '75323',
    'price': 500
})

try:
    assert_fact('bookstore', { 'reference': '75323', 'name': 'The new book', 'price': 500, 'seller': 'bookstore' })
except BaseException as e:
    # [버그 수정] Python3 예외에는 .message 속성이 없음 -> str(e) 사용
    #   (안 그러면 예외 발생 시 핸들러가 다시 AttributeError를 냄)
    print('Error: {0}'.format(str(e)))

post('bookstore', { 'reference': '75323', 'status': 'Active' })
retract_fact('bookstore', { 'reference': '75323', 'name': 'The new book', 'price': 500, 'seller': 'bookstore' })