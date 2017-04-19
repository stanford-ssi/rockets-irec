% Pull RASAero data
csvnum1 = 2500; csvnum2 = csvnum1*2;
RASdata = csvread('RASAero_N2900.csv', 1, 0);
RASMa = RASdata(1:csvnum1,1);
CD0 = RASdata(1:csvnum1,3);
CD2 = RASdata(csvnum1+1:csvnum2,3);
CD4 = RASdata(csvnum2:end,3);
CL0 = RASdata(1:csvnum1,8);
CL2 = RASdata(csvnum1+1:csvnum2,8);
CL4 = RASdata(csvnum2:end,8);
aerodata = [RASMa, CD0, CD2, CD4, CL0, CL2, CL4];

aoa = 2;
mach = 1.2;

% Check for mach number; 
k = 1; tol = 0.01;
while(abs(mach-aerodata(k,1)) > tol)
    k = k + 1;
end

% Compare angle of attack and use weighted average
% aerodata = [RASMa, CD0, CD2, CD4, CL0, CL2, CL4];
if abs(aoa) <= 2
    weight = aoa/2;
    CD = weight * aerodata(k,3) + (1-weight) * aerodata(k,2);
    CL = weight * aerodata(k,6) + (1-weight) * aerodata(k,5);
elseif abs(aoa) <= 4
    weight = (aoa-2)/2;
    CD = weight * aerodata(k,4) + (1-weight) * aerodata(k,3);
    CL = weight * aerodata(k,7) + (1-weight) * aerodata(k,6);
else
    disp('yo, aoa > 4')
end