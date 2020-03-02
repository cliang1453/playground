function n=name2idx(x)
for i=1:length(x)
    n(i)=find('abcdefghijklmnopqrstuvwxyz'==x(i));
end
