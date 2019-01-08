import numpy as np

def predict(adj, mask, z, W):
    """
    Prediction of heldout test links
    :param adj: NxN adjacency matrix for observed graph
    :param mask: NxN matrix where negative values indicate test entries
    :param z: 1xN vector of community assignments
    :param W: KxK matrix of community interaction probabilities
    :return: [vector of true test labels, vector of probabilities that labels equal one]
    """
    N = adj.shape[0]
    K = W.shape[0]

    Ntest = np.sum(np.sum(mask < 0))
    labels = np.zeros(Ntest)
    prob = np.zeros(Ntest)
    index = 0
    for ii in range(N):
        for jj in range(N):
            if mask[ii, jj] < 0:
                labels[index] = adj[ii, jj]
                """
                TODO: Compute probability that adj[ii,jj]=1
                """
                prob[index] = np.power(W[z[ii]][z[jj]], adj[ii][jj]) * np.power(1-W[z[ii]][z[jj]], 1-adj[ii][jj])
                #prob[index] = W[z[ii]][z[jj]]
                index += 1
    return [labels, prob]
