import scipy.io as sio
import numpy as np

def compute_mean_expected_deviation(q, q_exact):
	return np.mean(np.abs(q-q_exact))

def exact_method(phi, num_x, num_f):

	p = np.ones([2]*num_x)
	q = np.ones(num_x)

	for x1 in range(2):
		for x2 in range(2):
			for x3 in range(2):
				for x4 in range(2):
					x = [x1, x2, x3, x4]
					for pair, pair_phi in phi:
						i, j = pair - 1
						p[x1, x2, x3, x4] *= pair_phi[x[i], x[j]]
	
	z = np.sum(p)
	q[0] = np.sum(p[1, :, :, :])/z
	q[1] = np.sum(p[:, 1, :, :])/z
	q[2] = np.sum(p[:, :, 1, :])/z
	q[3] = np.sum(p[:, :, :, 1])/z
	return q
	

def mean_field_method(phi, num_x, num_f):

	def Eq(q, j, is_i, is_1):
		if is_i:
			return (1-q[j]) * np.log(pair_phi[is_1, 0]) + q[j] * np.log(pair_phi[is_1, 1])
		else:
			return (1-q[j]) * np.log(pair_phi[0, is_1]) + q[j] * np.log(pair_phi[1, is_1])

	T = 100
	eps = 0.0001
	q = np.ones(num_x) * 0.5
	prev_q = None

	for t in range(T):
		
		for k in range(num_x):
			q0, q1 = 0, 0
			for pair, pair_phi in phi:
				i, j = pair-1
				if k == i:
					q0 += Eq(q, j, is_i=True, is_1=0)
					q1 += Eq(q, j, is_i=True, is_1=1)
				if k == j:
					q0 += Eq(q, i, is_i=False, is_1=0)
					q1 += Eq(q, i, is_i=False, is_1=1)
			q[k] = np.exp(q1)/(np.exp(q0) + np.exp(q1))

		if prev_q is not None and np.linalg.norm(q - prev_q) < eps:
			break
		prev_q = q.copy()

	return q

def loopy_belief_propagation(phi, num_x, num_f):

	m_f2x = np.ones([num_f, num_x, 2]) #f->x
	m_x2f = np.ones([num_x, num_f, 2]) #x->f
	T = 100
	eps = 0.0001
	prev_q = None
	
	for _ in range(T):
		
		f = 0 # 1: 12, 2: 23, 3: 34, 4:, 41, 5: 13
		for pair, pair_phi in phi:
			for i in pair-1:
				m_x2f[i, f, :] = np.prod(np.concatenate((m_f2x[:f, i, :], m_f2x[f+1:, i, :])), axis=0)
			f += 1
		
		f = 0
		for pair, pair_phi in phi:
			i, j = pair-1
			for l in range(2):
				m_f2x[f, i, l] = np.sum(pair_phi[l, :].transpose() * m_x2f[j, f, :])
				m_f2x[f, j, l] = np.sum(pair_phi[:, l] * m_x2f[i, f, :])
			f += 1

		q0, q1 = np.prod(m_f2x[:, :, 0], axis=0), np.prod(m_f2x[:, :, 1], axis=0)
		q = q1/(q0+q1)
		if prev_q is not None and np.linalg.norm(q-prev_q) < eps:
			break
		prev_q = q.copy()

	return q


def main(phi, num_x, num_f):

	q_bp = loopy_belief_propagation(phi, num_x, num_f)
	q_mf = mean_field_method(phi, num_x, num_f)
	q_exact = exact_method(phi, num_x, num_f)

	ed_bp = compute_mean_expected_deviation(q_bp, q_exact)
	ed_mf = compute_mean_expected_deviation(q_mf, q_exact)


if __name__ == '__main__':
	mat = sio.loadmat('pMRF.mat')

	phi = []

	for pair, pair_phi in mat['phi'].squeeze():
		phi.append((pair.squeeze().astype(int), pair_phi.astype(float)))

	x = set([])
	for pair, _ in phi:
		for i in pair:
			if i not in x:
				x.add(i)

	num_x = len(x)
	num_f = len(phi)
	main(phi, num_x, num_f)