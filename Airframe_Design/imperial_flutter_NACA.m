function [fin] = imperial_flutter_NACA(RAD,fin)
% Fin Flutter Velocity
% Ian Gomez, 10/20/2016
% Source:
% https://www.apogeerockets.com/education/downloads/Newsletter291.pdf

m2ft = 3.28084; % ft/m
RAD.Ma = RAD.Ma;
RAD.h = RAD.h.*m2ft;

i = 5000;
RAD.Ma = RAD.Ma(1:i,:);
RAD.h  = RAD.h(1:i,:); % ft AGL

h = 0:max(RAD.h); % ft (from sea level)

% Atmospheric Properties
T = 59-0.00356.*h; % F
T = T + 459.7; % Rankine
P = (1/12^2).*2116.*(T./518.6).^5.256; % lbs/in^2
Rair_imperial = 1716; % ft*lb/slug*Rankine
k = 1.4; % ratio of specific heats
a = sqrt(T.*Rair_imperial.*k); % ft/s, should be around 1,126ft/s

fin.lambda = fin.tiplength/fin.rootlength;
G.CF = 5 *145000; % shear modulus of fin (psi)
G.Al = 24*145000;

scaling = 1e3;
line = 2;
figure
for i = 1:length(fin.t)
    x = 1.337.*(fin.AR.^3).*P.*(fin.lambda+1);
    y = 2.*(fin.AR+2).*(fin.t(i)./fin.rootlength).^3;
    vf = a.*sqrt(G.Al./(x./y)); %ft/s
    Ma_max = vf./a;
    hold on
    plot(h./scaling,Ma_max,'LineWidth',line)
end

set(gcf,'color','w');
plot(RAD.h./scaling,RAD.Ma,'LineWidth',line*1.5)
title('Flutter velocity with changing height')
ylabel('Speed (Ma)')
xlabel('Height(ft)')
xlim([0 max(RAD.h./scaling)])
legend('1/16"','1/8"','3/16"','1/4"','Flight Conditions')
grid on

rho.CF = 0.0578; %lbs/in^3
rho.Al = 0.098; 
V = fin.S*fin.t; % in^3
fin.mass = V*rho.Al; %lbs
fin.mass_total = fin.mass * fin.number;
fin.mass_unit = 'lb';

end