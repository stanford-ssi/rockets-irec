% Fin Flutter Velocity
% Ian Gomez, 10/20/2016
% Source:
% https://www.apogeerockets.com/education/downloads/Newsletter291.pdf
clear;

RAD = csvread('4in_CF.CSV',2,1);
RAD_Ma = RAD(:,2);
RAD_h  = RAD(:,18); % ft

i = 5000;
RAD_Ma = RAD_Ma(1:i,:);
RAD_h  = RAD_h(1:i,:); % ft AGL

h = 0:max(RAD_h); % ft (from sea level)

% Atmospheric Properties
T = 59-0.00356.*h; % F
T = T + 459.7; % Rankine
P = (1/12^2).*2116.*(T./518.6).^5.256; % lbs/in^2
Rair_imperial = 1716; % ft*lb/slug*Rankine
k = 1.4; % ratio of specific heats
a = sqrt(T.*Rair_imperial.*k); % ft/s, should be around 1,126ft/s

cr = 8; % root chord (in)
ct = 2; % tip chord (in)
t = (1/16):(1/16):(1/4); % thickness (in)
b = 4; % semi-span (in)
S = 0.5.*(cr + ct).*b; % in^2
AR = (b.^2)/S;
lambda = ct/cr;
G.CF = 5 *145000; % shear modulus of fin (psi)
G.Al = 24*145000; 

line = 2;
figure
for i = 1:length(t)
    x = 1.337.*(AR.^3).*P.*(lambda+1);
    y = 2.*(AR+2).*(t(i)./cr).^3;
    vf = a.*sqrt(G.Al./(x./y)); %ft/s
    Ma_max = vf./a;
    hold on
    plot(h,Ma_max,'LineWidth',line)
end

plot(RAD_h,RAD_Ma,'LineWidth',line*1.5)
title('Flutter velocity with changing height')
ylabel('Speed (Ma)')
xlabel('Height(ft)')
xlim([0 max(RAD_h)])
legend('1/16"','1/8"','3/16"','1/4"','Flight Conditions')

rho.CF = 0.0578; %lbs/in^3
rho.Al = 0.098; 
V = S*t; % in^3
fin.mass = V*rho.Al; %lbs
fin.num = 3;
fin.mass_total = fin.mass * fin.num;
fin.mass_unit = 'lb';
fin