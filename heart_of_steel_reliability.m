% recovery reliability
p = 0.99;
n = 1;
k = n;
P = nchoosek(n,k).*p.^k.*(1-p).^(n-k)