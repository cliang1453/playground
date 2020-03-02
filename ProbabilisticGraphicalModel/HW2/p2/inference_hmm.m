import brml.*

%% Let define state indices for first name to be 1~5, surname to be 6~12, start state to be 13, 
% intermediate state to be 14, and end state to be 15
N=15;
T=zeros(N, N);
firstnames=1:5;
surnames=6:12;
sta=13;
im=14;
en=15;

names{1}='david';
names{2}='anton';
names{3}='fred';
names{4}='jim';
names{5}='barry';
names{6}='barber';
names{7}='ilsung';
names{8}='fox';
names{9}='chain';
names{10}='fitzwilliam';
names{11}='quinceadams';
names{12}='grafvonunterhosen';

for idx=[firstnames surnames]
    name_pattern{idx}=name2idx(names{idx});
end
prior.other_states = [1 0 0]';

%% Assign values to the transition matrix
T(sta, sta)=0.8; % start -> start
for state=firstnames
    T(state, sta)=(1-0.8)*1/length(firstnames); % start -> firstname
    T(im, state)=1; % firstname -> intermediate
end;
T(im, im)=0.8;
for state=surnames
    T(state, im)=(1-0.8)*1/length(surnames); % imtermediate -> surname
    T(en, state)=1; % surname -> end
end;
T(sta, en)=1;

%% Assign values to the emission matrix
for i=1:26
    for j=1:26
        if i==j
            pattern_E(i,j)=0.3;
        else
            pattern_E(i,j)=0.7*1/25;
        end;
    end;
end;

for i=1:3
    other_E(:,i)=ones(26,1)/26;
end;

%% Training
[phghm, pvgh, ph1, start_pattern_idx, end_pattern_idx, other_idx]=train(name_pattern, [firstnames surnames], [sta im en], T, pattern_E, other_E, prior);

%% Inference
load noisystring.mat;
noisyname_pattern = name2idx(noisystring);
num_steps=10000;
[alpha, beta, gamma, delta, loglik, loglikpvhstar]=predict(noisyname_pattern, phghm, pvgh, ph1);
[pnum gnum]=predict_idx(delta, start_pattern_idx, other_idx); 

%% 
pcount=zeros(length(names),length(names));
fname=false; sname=false;
fprintf(1,'observation: [name_states other_states]')
for i=1:length(noisyname_pattern)
    fprintf(1,'\n          %s:         [%d        %d]',idx2name(noisyname_pattern(i)),pnum(i),gnum(i));
    if pnum(i)>0 
        if gnum(i-1)==1;
        fname=true; fnamep=pnum(i);
        end
        if gnum(i-1)==2;
        sname=true; snamep=pnum(i);
        end            
        fprintf(1,' %s',names{pnum(i)})
        if fname && sname
            pcount(fnamep,snamep)=pcount(fnamep,snamep)+1;
            fname=false; sname=false;
        end
    end
end
pcount(1:5,6:end)
fprintf(1,'\n\nlikelihood log p(v_{1:T})=%f\n',loglik) % log probability of the sequence
fprintf(1,'likelihood log p(v_{1:T},h^*_{1:T})=%f\n',loglikpvhstar) % log probabilty of the joint seq and most likely explanation