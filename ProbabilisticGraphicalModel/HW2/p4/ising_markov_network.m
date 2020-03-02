import brml.*
N=10;

for xi=1:2
	for xj=1:2
		get_phi(xi, xj)=exp(1*(xi==xj));
	end
end

node_map=reshape(1 : N*N, N, N);
x=0;
for x1=1: N*N
	% find position for node x1
	[c1 r1]=find(node_map==x1);
	for x2=x1 + 1:N*N
		% find position for node x2
		[c2 r2]=find(node_map==x2);

		% check if node x1 and node x2 are neighbours and x2 > x1
		if (c1==c2)&(r2-r1==1) | (r1==r2)&(c2-c1==1)

			% compute potential for current var x
			x=x + 1;
			phi{x}=array([x1 x2], get_phi);
			
			% associate edge position with current var x
			pos2var(c1,c2,r1,r2)=x;
		end
	end
end

% assign column potentials
for c=1:N
	col_phi{c}=const(1);
	if c < N
		for r=1:N-1
			col_phi{c}=multpots([col_phi{c} phi{pos2var(c,c+1,r,r)}]);
			col_phi{c}=multpots([col_phi{c} phi{pos2var(c,c,r,r+1)}]);
		end
		col_phi{c}=multpots([col_phi{c} phi{pos2var(c,c+1,N,N)}]);
	else
		for r=1:N-1
			col_phi{c-1}=multpots([col_phi{c-1} phi{pos2var(c,c,r,r+1)}]);
		end
	end
end

% implement message passing algorithm via variable elimination on column potentials
max_msg=[];
vars_to_eliminate=setdiff(col_phi{1}.variables, intersect(col_phi{1}.variables, col_phi{2}.variables));
msg=sumpot(col_phi{1}, vars_to_eliminate);

for c=2:N-2
	
	% variable elimination
	vars_to_eliminate=setdiff(col_phi{c}.variables, intersect(col_phi{c}.variables, col_phi{c+1}.variables));
	msg=sumpot(multpots([msg col_phi{c}]),vars_to_eliminate);

	% compute normalization factor Z on the fly
	max_msg=[max_msg max(msg.table(:))];
	msg.table=msg.table./max_msg(end); 
end

logZ=log(table(sumpot(multpots([msg col_phi{N-1}]),[],0)));
logZ=logZ + sum(log(max_msg))