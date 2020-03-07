import numpy as np
import math
import random
import time
import copy

class Model():
	def __init__(self):
		self.factors = {}
		self.factors["0"] = np.random.uniform(0,1,2)
		
		self.factors["1|0"] = np.random.uniform(0,1,[2,2])
		self.factors["2|0"] = np.random.uniform(0,1,[2,2])
		self.factors["3|0"] = np.random.uniform(0,1,[2,2])
		self.factors["4|0"] = np.random.uniform(0,1,[2,2])
		
		self.factors["5|1234"] = np.random.uniform(0,1,[2,2,2,2,2])
		self.factors["6|1234"] = np.random.uniform(0,1,[2,2,2,2,2])
		self.factors["7|1234"] = np.random.uniform(0,1,[2,2,2,2,2])
		self.factors["8|1234"] = np.random.uniform(0,1,[2,2,2,2,2])
		self.factors["9|1234"] = np.random.uniform(0,1,[2,2,2,2,2])
		self.factors["10|1234"] = np.random.uniform(0,1,[2,2,2,2,2])
		self.factors["11|1234"] = np.random.uniform(0,1,[2,2,2,2,2])
		
		self.normalize()
		
	def prob(self,x):
		return	self.factors["0"][x[0]] *\
				self.factors["1|0"][x[1],x[0]] *\
				self.factors["2|0"][x[2],x[0]] *\
				self.factors["3|0"][x[3],x[0]] *\
				self.factors["4|0"][x[4],x[0]] *\
				self.factors["5|1234"][x[5],x[1],x[2],x[3],x[4]] *\
				self.factors["6|1234"][x[6],x[1],x[2],x[3],x[4]] *\
				self.factors["7|1234"][x[7],x[1],x[2],x[3],x[4]] *\
				self.factors["8|1234"][x[8],x[1],x[2],x[3],x[4]] *\
				self.factors["9|1234"][x[9],x[1],x[2],x[3],x[4]] *\
				self.factors["10|1234"][x[10],x[1],x[2],x[3],x[4]] *\
				self.factors["11|1234"][x[11],x[1],x[2],x[3],x[4]]
	
	def prob_x(self,x):
		return self.prob(self.num2vec(x))
		
	def init(self):
		for i in self.factors:
			self.factors[i] *= 0
		
	def normalize(self):
		for i in self.factors:
			self.factors[i] /= (self.factors[i].sum(0)+0.001)

	def num2vec(self,x):
		x = int(x)
		r = []
		for i in range(12):
			r = r+[x%2]
			x = x//2
		return r
	
	def update(self,x):
		x = self.num2vec(x)		
		self.factors["0"][x[0]] += 1
		self.factors["1|0"][x[1],x[0]] += 1
		self.factors["2|0"][x[2],x[0]] += 1
		self.factors["3|0"][x[3],x[0]] += 1
		self.factors["4|0"][x[4],x[0]] += 1
		self.factors["5|1234"][x[5],x[1],x[2],x[3],x[4]] += 1
		self.factors["6|1234"][x[6],x[1],x[2],x[3],x[4]] += 1
		self.factors["7|1234"][x[7],x[1],x[2],x[3],x[4]] += 1
		self.factors["8|1234"][x[8],x[1],x[2],x[3],x[4]] += 1
		self.factors["9|1234"][x[9],x[1],x[2],x[3],x[4]] += 1
		self.factors["10|1234"][x[10],x[1],x[2],x[3],x[4]] += 1
		self.factors["11|1234"][x[11],x[1],x[2],x[3],x[4]] += 1
	
	def query(self,x):
		if x[0]==-1:
			phi_1234 = np.einsum("qi,wi,ei,ri->qwer",self.factors["1|0"],self.factors["2|0"],self.factors["3|0"],self.factors["4|0"])
		else:
			phi_1234 = np.einsum("q,w,e,r->qwer",self.factors["1|0"][:,x[0]],self.factors["2|0"][:,x[0]],self.factors["3|0"][:,x[0]],self.factors["4|0"][:,x[0]])
		for i in range(5,12):
			if x[i]==-1:
				phi_1234 *= self.factors[str(i)+"|1234"][0,:,:,:,:]+self.factors[str(i)+"|1234"][1,:,:,:,:]
			else:
				phi_1234 *= self.factors[str(i)+"|1234"][x[i],:,:,:,:]
		if x[4]==-1:
			phi_123 = np.einsum("jikl->jik",phi_1234)
		else:
			phi_123 = phi_1234[:,:,:,x[4]]
		if x[3]==-1:
			phi_12 = np.einsum("ijk->ij",phi_123)
		else:
			phi_12 = phi_1234[:,:,x[3]]
		if x[2]==-1:
			phi_1 = np.einsum("ij->i",phi_12)
		else:
			phi_1 = phi_1234[:,x[2]]
		if x[1]==-1:
			phi = phi_1.sum()
		else:
			phi = phi_1[x[1]]
		return phi

def compute_second_layer(model, x, i):
	x1 = copy.copy(x)
	x2 = copy.copy(x)
	x1[i]=0
	x2[i]=1
	return [model.query(x1),model.query(x2)]

def answer_query(model):
	start = time.time()
	x = [-1]*12
	x[8] = x[11] = x[1] = 1
	p_hasflu_t = model.query(x)
	x[1] = 0
	p_hasflu_f = model.query(x)

	x = [-1]*12
	x[4] = 1
	p_hasrash = compute_second_layer(model, x, 7)
	p_coughs = compute_second_layer(model, x, 8)
	p_isfatigued = compute_second_layer(model, x, 9)
	p_vomits = compute_second_layer(model, x, 10)
	p_hasfever = compute_second_layer(model, x, 11)

	x = [-1]*12
	x[0] = 1
	p_vomits_insummer = compute_second_layer(model, x, 10)

	print("Query 1: %.4f"%(p_hasflu_t/(p_hasflu_t+p_hasflu_f)))
	print("Query 2: Given the patient has pneumonia, the probability distribution over the symptoms is (HasRash:%.4f, Coughs:%.4f, IsFatigued:%.4f, Vomits:%.4f, and HasFever:%.4f)"\
		%(p_hasrash[1]/(p_hasrash[0]+p_hasrash[1]),p_coughs[1]/(p_coughs[0]+p_coughs[1]),\
			p_isfatigued[1]/(p_isfatigued[0]+p_isfatigued[1]),p_vomits[1]/(p_vomits[0]+p_vomits[1]),p_hasfever[1]/(p_hasfever[0]+p_hasfever[1])) )
	print("Query 3: %.4f"%(p_vomits_insummer[1]/(p_vomits_insummer[0]+p_vomits_insummer[1])))
	print("Timing: ", time.time()-start, "seconds")


def main():

	dataset = np.loadtxt("dataset.dat")
	model = Model()
	model.init()
	for x in dataset:
		model.update(x)
	model.normalize()
	print("Variable Elimination Querying:  ")
	answer_query(model)

if __name__ == '__main__':
	main()