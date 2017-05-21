%%% Buckling Calculation
% Ruqayya Toorawa, Ian Gomez; May 19, 2017

clc;
% steel rod fixed on both ends taking a compressive loading
N2lbs = 0.224808942443;
motor = [1500, 2500, 1620]*N2lbs;

r = (0.3344)/2; % 3/8in diameter rod
L = [36, 26, 42]; % in, length of the rods
Ix = (pi/4).*r.^4; % in^4
E_steel = 29e6; % lbs/in^2

loading_type = 4; % 4 = both pinned, 2 = pinned and fixed
for i = 1:length(L)
    crit_load = loading_type.*pi.^2.*E_steel.*Ix./L(i).^2; % lbs
    SF(i) = crit_load/motor(i);
end

SF


30*100