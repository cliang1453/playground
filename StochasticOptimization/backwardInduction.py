def main(S_len, A_len, T_len):
    """
    input: S_len: length of state space
           A_len: length of action space
           T_len: length of time steps
    
    output: D[T_len][S_len]: best policy at specific s and t

    """

    # U[s][t] denotes the value at state s at time t: U^*_t(s)
    # R[s][a] denotes the reward at state s with action: r(s, a)
    # P[s][a][s_] denotes the transitional probability: P(s_|s, a)
    # D[s][t] denotes the policy at state s at time t: D^*_t(s)
    U = [[0]*T_len for _ in range(S_len)]
    R = [[10, 0, 0],[5, -1, -10],[-1, -2, -10],[-5, -4, -10]]
    P = [[[0]*S_len for _ in range(A_len)] for _ in range(S_len)]
    D = [[0]*T_len for _ in range(S_len)]
    
    # initialization
    P[0][0][0], P[0][0][1] = 0.8, 0.2
    P[1][0][1], P[1][0][2] = 0.6, 0.4
    P[2][0][2], P[2][0][3] = 0.5, 0.5
    P[3][0][3] = 1
    P[1][1][0], P[1][1][1] = 0.7, 0.3
    P[2][1][1] = 1
    P[3][1][1], P[3][1][2] = 0.6, 0.4
    P[1][2][0] = 1
    P[2][2][0] = 1
    P[3][2][0] = 1
    U[0][-1], U[1][-1], U[2][-1], U[3][-1] = 10, 5, -1, -5 

    # back induction
    for t in range(T_len - 2, -1, -1):
        for s in range(S_len):
            U_curr = [sum([P[s][a][s_] * U[s_][t+1] for s_ in range(S_len)]) + R[s][a] for a in range(A_len)]
            U[s][t] = max(U_curr)
            D[s][t] = U_curr.index(U[s][t])

    return D, U



if __name__ == '__main__':
    
    S_len = 4
    A_len = 3
    T_len = 20
    
    D, U = main(S_len, A_len, T_len)