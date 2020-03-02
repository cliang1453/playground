% reference https://lost-contact.mit.edu/afs/inf.ed.ac.uk/group/teaching/mlprdata/Barber/BRMLtoolkitSolutions/
function [alpha, beta, gamma, delta, loglik, logpvhstar]=predict(pattern, phghm, pvgh, ph1)
% [alpha,beta,gamma,delta,loglik,logpvhstar]=patternsearch(observations,transition,emission,prior,thres,<printtime>)
% returns:
% alpha: p(h_t|v_{1:t})
% beta: p(h_t|v_{t+1:T})
% gamma: p(h_t|v_{1:T})
% delta: most likely state
% loglik: log p(v_{1:T})
% logpvhstar: log p(v_{1:T},h^*_{1:T})  -- log of most likely explanation
import brml.*
t=cputime;
[alpha, loglik] = HMMforward(pattern, phghm, ph1, pvgh);
beta=HMMbackward(pattern, phghm, pvgh);
gamma=(alpha).*(beta);
gamma = bsxfun(@rdivide, gamma, sum(gamma));
fprintf(1,'\nHMM marginal inference takes %f seconds',cputime-t);
t=cputime;
[delta,logpvhstar]=HMMviterbi(pattern, phghm, ph1, pvgh);
fprintf(1,'\nHMM viterbi takes %f seconds\n',cputime-t);