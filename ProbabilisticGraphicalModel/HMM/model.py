import sys
import numpy as np
from collections import defaultdict, deque
import math

IDX = {"O": 0, "I-GENE": 1, "*": 2, "STOP": 3}
TAG = {0: "O", 1: "I-GENE", 2: "*", 3: "STOP"}

class BioHmm(object):
    """docstring for BioHmm"""
    def __init__(self, mode = 0):
        super(BioHmm, self).__init__()
        self.mode = mode
        self.tag_cnt = {}
        self.two_gram_cnt = {}
        self.three_gram_cnt = {}
        self.emission_cnt = {}
        self.emission_prob = {}
        self.trigram_prob = [[[0]*len(TAG) for _ in range(len(TAG))] for _ in range(len(TAG))]

    def compute_emission_prob(self):

        for tag, tag_cnt in self.tag_cnt.items():
            for word, word_cnt in self.emission_cnt[tag].items():
                if word not in self.emission_prob:
                    self.emission_prob[word] = [0]*len(IDX)
                self.emission_prob[word][IDX[tag]] = word_cnt/tag_cnt

    def compute_trigram_prob(self):
        
        if self.mode == 0:
            return

        for three_gram, cnt in self.three_gram_cnt.items():
            tag1, tag2, tag3 = three_gram
            two_gram = (tag1, tag2)
            if two_gram in self.two_gram_cnt:
                t1, t2, t3 = IDX[tag1], IDX[tag2], IDX[tag3]
                self.trigram_prob[t1][t2][t3] = self.three_gram_cnt[three_gram]/self.two_gram_cnt[two_gram]
        self.trigram_prob = np.array(self.trigram_prob)

    def viterbi_decoding0(self, old_sentence):

        # obs -- word
        # state -- tag
        # t=0 --> sentence[0]
        # V[T][S][S]: value function at stage "tag" at step t
        # B[T][S][S]: backpointer that store the best tag
        # Y[T]: output tag sequence
        sentence = old_sentence.copy()
        T = len(sentence)
        for t in range(T):
            if sentence[t] not in self.emission_prob:
                sentence[t] = "_RARE_"

        V = {}
        B = [[[0] * len(TAG) for _ in  range(len(TAG))] for _ in range(T)]
        Y = [0] * T
        
        # initialization
        for s3 in range(len(TAG)):
            V[(0,0,s3)] = np.log(self.trigram_prob[IDX["*"], IDX["*"], s3]+1e-300) + np.log(np.array(self.emission_prob[sentence[0]][s3])+1e-300)
        for s2 in range(len(TAG)):
            for s3 in range(len(TAG)):
                V[(1,s2,s3)] = V[(0,0,s2)] + np.log(self.trigram_prob[IDX["*"], s2, s3]+1e-300) + np.log(self.emission_prob[sentence[1]][s3]+1e-300)

        # forward
        for t in range(2, T):
            for s2 in range(len(TAG)):
                for s3 in range(len(TAG)):
                    curr = np.array([V[(t-1,s1,s2)] + np.log(self.trigram_prob[s1, s2, s3]+1e-300) + np.log(self.emission_prob[sentence[t]][s3]+1e-300) for s1 in range(len(TAG))])
                    V[(t,s2,s3)] = np.max(curr)
                    B[t][s2][s3] = np.argmax(curr)

        curr = np.array([[V[(T-1,s2,s3)] + np.log(self.trigram_prob[s2, s3, IDX["STOP"]]+1e-300) for s2 in range(len(TAG))] for s3 in range(len(TAG))])
        Y[T-2], Y[T-1] = np.unravel_index(curr.argmax(), curr.shape)

        for t in range(T-3, -1, -1):
            Y[t] = B[t+2][Y[t+1]][Y[t+2]]
        return Y
    
    def train(self, file_name = ""):
        """
        Train the model
        """
        data = open(file_name,"r")
        l = data.readline()
        while l:
            line = l.strip()
            if not line:
                sys.stderr.write("Invalid input")
            
            fields = line.split(" ")
            
            if fields[1] == "WORDTAG":
                cnt, _, tag, word = fields
                if tag not in self.tag_cnt:
                    self.tag_cnt[tag] = 0
                    self.emission_cnt[tag] = {}
                if word not in self.emission_cnt[tag]:
                    self.emission_cnt[tag][word] = 0
                self.tag_cnt[tag] += int(cnt)
                self.emission_cnt[tag][word] += int(cnt)

            if self.mode == 1 and fields[1] == "2-GRAM":
                cnt, _, tag1, tag2 = fields
                if (tag1, tag2) not in self.two_gram_cnt:
                    self.two_gram_cnt[(tag1, tag2)] = 0
                self.two_gram_cnt[(tag1, tag2)] += int(cnt)

            if self.mode == 1 and fields[1] == "3-GRAM":
                cnt, _, tag1, tag2, tag3 = fields
                if (tag1, tag2, tag3) not in self.three_gram_cnt:
                    self.three_gram_cnt[(tag1, tag2, tag3)] = 0
                self.three_gram_cnt[(tag1, tag2, tag3)] += int(cnt)

            l = data.readline()

    def predict(self, file_name = ""):

        """
        Make prediction
        mode = 0: emission probability only
        """

        data = open(file_name,"r")

        if self.mode == 0:
            l = data.readline()
            while l:
                word = l.strip()
                if not word:
                    print("")
                else:
                    if word in self.emission_prob:
                        tag = TAG[np.argmax(np.array(self.emission_prob[word]))]
                    else:
                        tag = TAG[np.argmax(np.array(self.emission_prob["_RARE_"]))]
                    print(word + " " + tag)
                l = data.readline()
        elif self.mode == 1:
            l = data.readline()
            sentence = []
            while l:
                word = l.strip()
                if not word:
                    pred = self.viterbi_decoding0(sentence)
                    for word, tag in zip(sentence, pred):
                        print(word + " " + TAG[tag])
                    print("")
                    sentence = []
                else:
                    sentence.append(word)
                l = data.readline()
