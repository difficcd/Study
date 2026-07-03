# [PASSAGE 132] 패키지 설치 (81p)
# !pip install spacy

# [PASSAGE 133] spaCy 기초 실습 (82p)
import spacy
nlp = spacy.load('en_core_web_sm')

doc = nlp("I have seen the handsome boy with an apple.")

# 토큰별 속성 출력
for tok in doc:
    print(tok.text, tok.lemma_, tok.pos_, tok.tag_, tok.dep_, tok.shape_,
          tok.is_alpha, tok.is_stop)

# [PASSAGE 134] networkx 기초 실습 (83p)
import networkx as nx
g1 = nx.DiGraph() # 방향 그래프(directed graph)
g1.add_node('a') # 노드 추가
g1.add_node(1)
g1.add_node('b')
print('Node: ', g1.nodes())
g1.add_edge(1, 'a') # 에지 추가
g1.add_edge(1, 'b')
print('Edges: ', g1.edges())

# 그래프 시각화 (pydot 및 IPython 사용 - 환경에 따라 생략 가능)
from IPython.core.display import Image
from networkx.drawing.nx_pydot import to_pydot
d1 = to_pydot(g1)
d1.set_dpi(400)
d1.set_rankdir('LR')
d1.set_margin(1)
# Image(d1.create_png(), width=300)

# [PASSAGE 135] 지식 그래프 구성을 위한 환경 설정 및 데이터 로드 (84p)
import re
import pandas as pd
import bs4
import requests
import spacy
from spacy import displacy
nlp = spacy.load('en_core_web_sm')

from spacy.matcher import Matcher
from spacy.tokens import Span

import networkx as nx
import matplotlib.pyplot as plt
from tqdm import tqdm

pd.set_option('display.max_colwidth', 200)
# %matplotlib inline

# wikipedia 문서 파일 로드 (PASSAGE 135 하단)
# 파일이 같은 경로에 있어야 합니다.
candidate_sentences = pd.read_csv("wiki_sentences_v2.csv")
print('데이터 크기:', candidate_sentences.shape)

# [PASSAGE 136] 개체(Entity) 추출 함수 정의 (85p)
def get_entities(sent):
    ent1 = ""
    ent2 = ""
    prv_tok_dep = "" # 문장에서 직전 토큰의 의존 파싱 태그
    prv_tok_text = "" # 문장에서 직전 토큰
    prefix = ""
    modifier = ""
    
    for tok in nlp(sent):
        # 토큰이 구두점(punctuation mark)이면 다음 토큰으로 이동
        if tok.dep_ != "punct":
            # 토큰이 복합어인 경우
            if tok.dep_ == "compound":
                prefix = tok.text
                if prv_tok_dep == "compound":
                    prefix = prv_tok_text + " " + tok.text
            
            # 토큰이 수식어(modifier)인 경우
            if tok.dep_.endswith("mod") == True:
                modifier = tok.text
                if prv_tok_dep == "compound":
                    modifier = prv_tok_text + " " + tok.text

            # 주어(subject)인 경우
            # [버그 수정] str.find()는 인덱스(-1~)를 반환하므로 == True(==1)는 잘못됨.
            #   substring이 우연히 인덱스1에 있을 때만 동작 -> "subj" in ... 으로 교체
            if "subj" in tok.dep_:
                ent1 = modifier + " " + prefix + " " + tok.text
                prefix = ""
                modifier = ""
                prv_tok_dep = ""
                prv_tok_text = ""
            
            # 목적어인 경우
            # [버그 수정] 위와 동일 (find()==True -> in)
            if "obj" in tok.dep_:
                ent2 = modifier + " " + prefix + " " + tok.text

        prv_tok_dep = tok.dep_
        prv_tok_text = tok.text
        
    return [ent1.strip(), ent2.strip()] # 식별된 개체명 반환

# 함수 테스트
print('테스트 결과:', get_entities("the film had 200 patents"))

# [PASSAGE 137] 관계(Relation) 추출 및 지식 그래프 생성 (86p~87p)
# 모든 문장에서 개체 쌍 추출
entity_pairs = []
for i in tqdm(candidate_sentences["sentence"]):
    entity_pairs.append(get_entities(i))

# 관계 추출 함수 정의
def get_relation(sent):
    doc = nlp(sent)
    matcher = Matcher(nlp.vocab)
    # 패턴 정의
    pattern = [{'DEP':'ROOT'},
               {'DEP':'prep','OP':"?"},
               {'DEP':'agent','OP':"?"},
               {'POS':'ADJ','OP':"?"}]
    matcher.add("matching_1", [pattern])
    matches = matcher(doc)
    print('matches :', matches) # 매칭 결과 출력
    k = len(matches) - 1
    # 소스 코드의 인덱싱 유지
    span = doc[matches[k][1]:matches[k][2]]
    return(span.text)

# 관계 추출 및 데이터프레임 생성 (87p)
relations = [get_relation(i) for i in tqdm(candidate_sentences['sentence'])]

source = [i[0] for i in entity_pairs]
target = [i[1] for i in entity_pairs]
kg_df = pd.DataFrame({'source':source, 'target':target, 'edge':relations})

# 방향 그래프 생성 및 시각화
G = nx.from_pandas_edgelist(kg_df, "source", "target",
                             edge_attr=True, create_using=nx.MultiDiGraph())

plt.figure(figsize=(12,12))
pos = nx.spring_layout(G)
nx.draw(G, with_labels=True, node_color='skyblue', edge_cmap=plt.cm.Blues, pos = pos)
plt.show()

# 특정 관계 필터링 시각화 (예: "written by")
G_written = nx.from_pandas_edgelist(kg_df[kg_df['edge']=="written by"], "source", "target",
                                     edge_attr=True, create_using=nx.MultiDiGraph())
plt.figure(figsize=(12,12))
pos_w = nx.spring_layout(G_written, k = 0.5)
nx.draw(G_written, with_labels=True, node_color='skyblue', node_size=1500, edge_cmap=plt.cm.Blues, pos = pos_w)
# FIX: edge_labels 는 문자열이 아니라 {(u, v): 라벨} 형태의 dict 여야 함.
#      (networkx 가 .items() 를 호출하므로 문자열을 넘기면 AttributeError 발생)
edge_labels_w = {(u, v): "written by" for u, v in G_written.edges()}
nx.draw_networkx_edge_labels(G_written, pos_w, edge_labels=edge_labels_w)
plt.show()

# 특정 관계 필터링 시각화 (예: "composed by")
G_composed = nx.from_pandas_edgelist(kg_df[kg_df['edge']=="composed by"], "source", "target",
                                     edge_attr=True, create_using=nx.MultiDiGraph())
plt.figure(figsize=(12,12))
pos_c = nx.spring_layout(G_composed, k = 0.5) # k로 노드 간격 조절
nx.draw(G_composed, with_labels=True, node_color='skyblue', node_size=1500, edge_cmap=plt.cm.Blues, pos = pos_c)
plt.show()