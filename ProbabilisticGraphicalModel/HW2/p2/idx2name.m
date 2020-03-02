function n=idx2name(x)
c='abcdefghijklmnopqrstuvwxyz';
for i=1:length(x)
    n(i)=c(x(i));
end