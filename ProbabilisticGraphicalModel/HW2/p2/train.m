% reference https://lost-contact.mit.edu/afs/inf.ed.ac.uk/group/teaching/mlprdata/Barber/BRMLtoolkitSolutions/
function [phghm, pvgh, ph1, start_pattern_idx, end_pattern_idx, other_idx]=train(pattern, pattern_states, other_states, T, pattern_E, other_E, prior)

if ~iscell(pattern_E)
    CorruptionProb=pattern_E;
    clear pattern_E;
    for pt=pattern_states
        for l=1:length(pattern{pt})
            pattern_E{pt,l}=CorruptionProb(:,pattern{pt}(l)); % probability distribution for position l of pattern pt
        end
    end
end

start_pattern_idx(1)=1;
end_pattern_idx(1)=length(pattern{1});
for p=2:length(pattern)
    start_pattern_idx(p)=end_pattern_idx(p-1)+1; % end counter for the pattern
    end_pattern_idx(p)=start_pattern_idx(p)+length(pattern{p})-1; % start of the counter for the pattern
end
H=end_pattern_idx(end)+length(other_states);

phghm=zeros(H);
Gstart=end_pattern_idx(end)+1;
other_idx=Gstart:(Gstart+length(other_states)-1);

% setup the prior (distribution of initial time):
ph1=zeros(H,1);
if isempty(prior); ph1=ones(H,1)/H; end % uniform if ph1==0
if ~isfield(prior,'other_states') & ~isfield(prior,'pattern_states')
    ph1=prior;
end
if isfield(prior,'other_states')
    for gt=1:length(other_states);
        ph1(Gstart+gt-1,1)=prior.other_states(gt);
    end
end
if isfield(prior,'pattern_states')
    for pt=pattern_states
        if prior.startatbeginningpattern
            ph1(start_pattern_idx(pt),1)=prior.pattern_states(pt); % start at pattern
        else
            ph1(start_pattern_idx(pt):end_pattern_idx(pt),1)=prior.pattern_states(pt)/length(pattern{pt}); % start at pattern
        end
    end
end
ph1=sparse(ph1);

% pattern<-pattern:
for ptm=pattern_states
    for pt=pattern_states
        phghm(start_pattern_idx(pt),end_pattern_idx(ptm))=T(pt,ptm);
    end
end
% internal pattern incrementor:
for pt=1:length(pattern_states)
    s=start_pattern_idx(pt);
    for l=1:length(pattern{pt})-1
        phghm(s+1,s)=1;
        s=s+1;
    end
end

%pattern<-general
for pt=pattern_states
    for g=1:length(other_states);
        phghm(start_pattern_idx(pt),Gstart+g-1)=T(pt,other_states(g));
    end
end

%general<-pattern
for pt=pattern_states
    for g=1:length(other_states);
        phghm(Gstart+g-1,end_pattern_idx(pt))=T(other_states(g),pt);
    end
end

%general<-general
for gt=1:length(other_states);
    for gtm=1:length(other_states);
        phghm(Gstart+gt-1,Gstart+gtm-1)=T(other_states(gt),other_states(gtm));
    end
end

% pattern emissions:
for pt=pattern_states
    for l=1:length(pattern{pt})
        pvgh(:,start_pattern_idx(pt)+l-1)= pattern_E{pt,l};
    end
end

for g=1:length(other_states)
    pvgh(:,Gstart+g-1)=other_E(:,g);
end
phghm=sparse(phghm);