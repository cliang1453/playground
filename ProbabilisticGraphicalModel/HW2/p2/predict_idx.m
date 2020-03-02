% reference https://lost-contact.mit.edu/afs/inf.ed.ac.uk/group/teaching/mlprdata/Barber/BRMLtoolkitSolutions/
function [pnum gnum]=predict_idx(delta, start_pattern_idx, other_idx)

pnum=zeros(1,length(delta));
gnum=zeros(1,length(delta));

for i=1:length(delta)
    k=find(delta(i)==start_pattern_idx);
    if ~isempty(k); pnum(i)=k; end
    l=find(delta(i)==other_idx);
    if ~isempty(l); gnum(i)=l; end
    if isempty(k)&isempty(l)
        pnum(i)=pnum(i-1);
        gnum(i)=gnum(i-1);
    end
end