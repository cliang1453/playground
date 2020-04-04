import brml.*
load p
p=setpotclass(p,'array');
x = p.variables(1);
y = p.variables(2);
z = p.variables(3);
size = p.size;

q_z = rand([size(3) 1]);
q_z = array(z, q_z./sum(sum(q_z)));

T = 100;
for t=1:T
    q_xy = condpot(exppot(sumpot(multpots([logpot(p) q_z]),z)));
    q_z = condpot(exppot(sumpot(multpots([logpot(p) q_xy]),[x y])));
    q_xyz = multpots([q_xy q_z]);
    KL(t) = KLdiv(q_xyz, p);
end;
fprintf(1,'Minimum KL-divergence is = %g\n',KL(T));
approx = table(q_xyz);
figure; plot(approx(:),'x'); hold on; plot(p.table(:),'o'); legend('q','p');
fprintf(1,'Mean deviation between p and q approx = %g\n',mean(abs(approx(:)-p.table(:))))