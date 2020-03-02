% References: https://github.com/taheris/BRML.jl/blob/master/matlab/DemosExercises/demoJTree.m
import brml.*
load('diseaseNet.mat');
pot=str2cell(setpotclass(pot,'array'));  % convert to cell array
figure; drawNet(dag(pot), variable); title('Belief Net');

%% Problem 5.1 Construct the junction tree and compute marginals of the symptoms
start_t=tic;

[jtpot jtsep infostruct]=jtree(pot); % setup the Junction Tree
jtpot=absorption(jtpot, jtsep, infostruct); % do full round of absorption
for s=21:60
	jtpotnum = whichpot(jtpot,s,1); % find a single JT potential that contains s
	margpot=sumpot(jtpot(jtpotnum),s,0); % sum over everything but s
	jtmargs_s(s-20) = margpot.table(1);
end;

jttime=toc(start_t);

figure; drawJTree(infostruct, variable); title('Junction Tree (separators not shown)');
for s=21:60
	disp(['p(s', num2str(s-20), '=1) = ' num2str(jtmargs_s(s-20))]);
end;
disp(['The total marginal computation time by using junction tree algorithm is ', num2str(jttime), 'seconds']);

%% Problem 5.2 Compute marginals of the symptoms more efficiently by considering the structural information
start_t=tic;

sipot=dag(pot); % setup DAG
for s=21:60
	margpot=sumpot(multpots([pot(s) pot(parents(sipot, s))]),s,0); % sum over parents of s
	simargs_s(s-20) = margpot.table(1);
end;

sitime=toc(start_t);

disp(['The total marginal computation time by using structural information is ', num2str(sitime), 'seconds']);

%% Problem 5.3 Compute marginals of the diseases given certain symptoms observations
observed_pot=setpot(pot, [21:30], [1 1 1 1 1 2 2 2 2 2]);
[jtpot jtsep]=jtassignpot(observed_pot,infostruct);
jtpot=absorption(jtpot, jtsep, infostruct); % do full round of absorption
for d=1:20
	jtpotnum = whichpot(jtpot,d,1); % find a single JT potential that contains d
	margpot=condpot(sumpot(jtpot(jtpotnum),d,0)); % sum over everything but d
	jtmargs_d(d) = margpot.table(1);
	disp(['p(d', num2str(d), '=1|s_{1:10}) = ' num2str(jtmargs_d(d))]);
end;