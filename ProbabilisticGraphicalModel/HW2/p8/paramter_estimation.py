import numpy as np
import math
import random

class Model():
	def __init__(self):
		self.IsSummer = np.random.uniform(0,1,2)
		self.IsSummer = self.IsSummer/self.IsSummer.sum()
		self.HasFlu = np.random.uniform(0,1,[2,2]) # given IsSummer
		self.HasFlu = self.HasFlu/self.HasFlu.sum(0)
		self.HasHayFever = np.random.uniform(0,1,[2,2]) # given IsSummer
		self.HasHayFever = self.HasHayFever/self.HasHayFever.sum(0)
		self.HasPneumonia = np.random.uniform(0,1,[2,2]) # given IsSummer
		self.HasPneumonia = self.HasPneumonia/self.HasPneumonia.sum(0)
		self.HasFoodPoisoning = np.random.uniform(0,1,2) 
		self.HasFoodPoisoning = self.HasFoodPoisoning/self.HasFoodPoisoning.sum()
		self.HasGastricProblem = np.random.uniform(0,1,[2,2]) # given HasFoodPoisoning
		self.HasGastricProblem = self.HasGastricProblem/self.HasGastricProblem.sum(0)
		self.HasRash = np.random.uniform(0,1,[2,4]) # given HasFlu, HasHayFever
		self.HasRash = self.HasRash/self.HasRash.sum(0)
		self.HasRespiratoryProblems = np.random.uniform(0,1,[2,8]) # given HasFlu, HasHayFever, HasPneumonia
		self.HasRespiratoryProblems = self.HasRespiratoryProblems/self.HasRespiratoryProblems.sum(0)
		self.IsFatigued = np.random.uniform(0,1,[2,8]) # given HasFlu, HasHayFever, HasPneumonia
		self.IsFatigued = self.IsFatigued/self.IsFatigued.sum(0)
		self.HasFever = np.random.uniform(0,1,[2,8]) # given HasFlu, HasPneumonia, HasGastricProblem
		self.HasFever = self.HasFever/self.HasFever.sum(0)
		self.Vomits = np.random.uniform(0,1,[2,2]) # given HasGastricProblem
		self.Vomits = self.Vomits/self.Vomits.sum(0)
		self.Coughs = np.random.uniform(0,1,[2,2]) # given HasRespiratoryProblems
		self.Coughs = self.Coughs/self.Coughs.sum(0)
		self.factors = [
				self.IsSummer,
				self.HasFlu,
				self.HasHayFever,
				self.HasPneumonia,
				self.HasFoodPoisoning,
				self.HasGastricProblem,
				self.HasRash,
				self.HasRespiratoryProblems,
				self.IsFatigued,
				self.HasFever,
				self.Vomits,
				self.Coughs]
		
	def prob(self,_issummer,_hasflu,_hasfoodpoisoning,_hashayfever,_haspneumonia,_hasrespiratoryproblems,\
			_hasgastricproblems, _hasrash, _coughs, _isfatigued, _vomits, _hasfever):
		return	self.IsSummer[_issummer] *\
				self.HasFlu[_hasflu,_issummer] *\
				self.HasFoodPoisoning[_hasfoodpoisoning] *\
				self.HasHayFever[_hashayfever, _issummer] *\
				self.HasPneumonia[_haspneumonia, _issummer] *\
				self.HasRespiratoryProblems[_hasrespiratoryproblems, _hasflu*4+_hashayfever*2+_haspneumonia] *\
				self.HasGastricProblem[_hasgastricproblems, _hasfoodpoisoning] *\
				self.HasRash[_hasrash, _hasflu*2+_hashayfever] *\
				self.Coughs[_coughs,_hasrespiratoryproblems] *\
				self.IsFatigued[_isfatigued, _hasflu*4+_hashayfever*2+_haspneumonia] *\
				self.Vomits[_vomits, _hasgastricproblems] *\
				self.HasFever[_hasfever, _hasflu*4+_haspneumonia*2+_hasgastricproblems]
	
	def prob_x(self,x):
		return self.prob(*self.num2vec(x))
		
	def init(self):
		for i in range(12) :
			self.factors[i] *= 0
		
	def normalize(self):
		for i in range(12) :
			self.factors[i] /= (self.factors[i].sum(0)+0.001)

	def num2vec(self,x):
		x = int(x)
		r = []
		for i in range(12):
			r = r+[x%2]
			x = x//2
		return r

	def update(self,x):
		_issummer,_hasflu,_hasfoodpoisoning,_hashayfever,_haspneumonia,_hasrespiratoryproblems,\
			_hasgastricproblems, _hasrash, _coughs, _isfatigued, _vomits, _hasfever = self.num2vec(x)
		self.IsSummer[_issummer] += 1 
		self.HasFlu[_hasflu,_issummer] += 1
		self.HasFoodPoisoning[_hasfoodpoisoning] += 1
		self.HasHayFever[_hashayfever, _issummer] += 1
		self.HasPneumonia[_haspneumonia, _issummer] += 1
		self.HasRespiratoryProblems[_hasrespiratoryproblems, _hasflu*4+_hashayfever*2+_haspneumonia] += 1
		self.HasGastricProblem[_hasgastricproblems, _hasfoodpoisoning] += 1
		self.HasRash[_hasrash, _hasflu*2+_hashayfever] += 1
		self.Coughs[_coughs,_hasrespiratoryproblems] += 1
		self.IsFatigued[_isfatigued, _hasflu*4+_hashayfever*2+_haspneumonia] += 1
		self.Vomits[_vomits, _hasgastricproblems] += 1
		self.HasFever[_hasfever, _hasflu*4+_haspneumonia*2+_hasgastricproblems] += 1


def main():

	joint = np.loadtxt("joint.dat")
	dataset = np.loadtxt("dataset.dat")

	print("Finish loading dataset")

	model = Model()
	model.init()
	for x in dataset:
		model.update(x)
	model.normalize()

	r = 0
	for x in joint:
		r += abs(x[1]-model.prob_x(int(x[0])))
	print("L1 distance:  ",r)

	true_prob = 0
	approx_prob = 0
	for x in dataset:
		x = int(x)
		true_prob += math.log(joint[x][1])
		approx_prob += math.log(model.prob_x(x))
	print("True Log-Probability:  ",true_prob, "  Estimated Log-Probability:  ",approx_prob)
	
if __name__ == '__main__':
	main()