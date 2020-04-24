import brml.*
data = load('ChowLiuData.mat');
data = squeezestates(data.X);
num_states = maxarray(data, 2); 
num_vars = size(data,1);
cnt = 1; 
for i = 1:num_vars
    for j = i+1:num_vars
        MI(cnt) = MIemp(data(i,:),data(j,:),num_states(i),num_states(j)); % compute pairwise mutual information
        edge(cnt,:) = [i j]; 
        cnt = cnt + 1;
    end
end

[dum idx] = sort(MI,'descend'); % sort multual information in descending order
edgelist = edge(idx(1:cnt-1),:); % sort the edges 
tree = spantree(edgelist); % find a spanning tree
figure; drawNet(triu(tree)); title('MLE Chow Liu Tree')