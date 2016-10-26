function[angleattack] = incomingangle(ptheta1, ptheta2, ptheta3) 

%pinf (this should be what altimeter is reading)
%pstag (pstag = pinf * ((((gamma + 1)Ma^2)) / 2) ^ (gamma / (gamma - 1)) * 
% ((gamma + 1) / (2 * gamma * Ma^2 - (gamma - 1))) ^ 1 / (gamma - 1)
%using latitude/longitude (lambda is equatorial, psi is down from top)
%1 is along lambda = 0, 2 is along lambda = 120 deg, 3 is along lambda = 240 deg
pinf = 101325; %random right now
pstag = 160000; %random right now
deg2rad = 2 * pi / 360;
ctheta = 10 * deg2rad; %angle we drill holes from center
pthetas = [ptheta1, ptheta2, ptheta3]; %pressure values read from holes
lambdas = [0, 2/3 * pi, 4/3 * pi]; %3 holes
psis = [pi/2 - ctheta, pi/2 - ctheta, pi/2 - ctheta];
thetas = acos(sqrt((pthetas / pinf - 1) / (pstag / pinf - 1))); %comes from paper

best = [10 ^ 9,0,0]; %loop through lamdas and psis and find the one that gives least error
%in angle from each hole
for lambdatemp = 0:2*pi
    for psitemp = pi/4:pi/2
        psitemp
        psis
        distanceerrors = acos(sin(psitemp).*sin(psis) + cos(psitemp) .* cos(psis) .* cos(lambdatemp - lambdas)) - thetas
        errorssq = sum(distanceerrors .^ 2)
        if errorssq < best(1)
            best = [errorssq, psitemp, lambdatemp];
        end
    end
end
angleattack = best(1) %angle of attack is psi value of oncoming air
end