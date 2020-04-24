import brml.*

[fuse drum toner poorpaper roller burning poorprint wrinkled multiple jam]=assign(1:10);
data = load('EMprinter.mat')
pot{fuse}=array(fuse);
pot{drum}=array(drum);
pot{toner}=array(toner);
pot{poorpaper}=array(poorpaper);
pot{roller}=array(roller);
pot{burning}=array([burning fuse]);
pot{poorprint}=array([poorprint poorpaper toner drum]);
pot{wrinkled}=array([wrinkled poorpaper fuse]);
pot{multiple}=array([multiple roller poorpaper]);
pot{jam}=array([jam fuse roller]);
for i=1:10
	pot{i}.table = myzeros(2 * ones(1,length(pot{i}.variables)));
end

% hyperparameters
[V N] =size(data.x);
[variables num_states]=potvariables(pot);
oldpot=pot;
epsilon=0.0001;
T=50;

% compute conditional probability p(i|pa{i})
for i=variables
    if all(pot{i}.table(:)==0)
        oldpot{i}.table=myrand(num_states(pot{i}.variables));
        oldpot{i}=condpot(oldpot{i},i,setdiff(pot{i}.variables,i));
    end
end
newpot=oldpot; % use to initialise any tables which are not learned.

% EM Algorithm
for t=1:T
    % E step
    % initialize all the summed potentials to zero
    loglik=0;
    for i=variables 
        smpot{i}=oldpot{i};
        smpot{i}.table=zeros(size(oldpot{i}.table))+0.001;
    end

    % compute expected log-likelihood
    for n=1:N
        missing = find(isnan(data.x(:,n)));
        present=setdiff(1:V,missing);
        
        % compute log-likelihood for visible and missing
        qx=multpots(setpot(oldpot,present,data.x(present,n))); 
        thisloglik=log(evalpot(sumpot(qx,missing))); 
        loglik = loglik + thisloglik;

        % compute statistics needed for the M step:
        for i=variables
            ivars=pot{i}.variables;
            ipresent=intersect(present,ivars);
            qiv=delta(array,ipresent,data.x(ipresent,n),num_states(ipresent));
            imissing=intersect(missing,ivars);
            if isempty(imissing)
                smpot{i}=sumpots([smpot{i} qiv]);
            else
                qihgv= condpot(qx,imissing);
                smpot{i}=sumpots([smpot{i} multpots([qihgv qiv])]);
            end
        end
    end
    
    % M step
    for i=variables
    	newpot{i} = condpot(smpot{i},i,setdiff(oldpot{i}.variables,i))
    end

    logloop(t)=loglik;
    if t>1 && logloop(end)-logloop(end-1)<epsilon
        break; 
    end
    oldpot=newpot;
end

variable(drum).name='drum'; variable(drum).domain={'no','yes'}; no=1; yes=2;
jointpot=multpots(newpot); 
disp('Probability of a drum unit problem given the evidence is:')
disptable(condpot(setpot(jointpot,[wrinkled burning poorprint], [no no yes]),drum),variable);