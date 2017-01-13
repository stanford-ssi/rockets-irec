function[goredesign] = designgore_triconical(nominaldiameter, numgores, ventdiameter, aspectratio)
% Logan Herrera
% Stanford Student Space Initiative - IREC
% 
% References:
% Parachute Recovery Systems Design Manual - T. W. Knacke - 1992

% figure 6-21
if isempty(aspectratio); aspectratio = 4; end;

angles = [20, 41, 80];
x0 = [0 nominaldiameter/2];

ellipseintersect = @(x, Do, a, WH) sqrt(Do^2/WH^2 - 4/WH^2*x.^2) - tand(a)*x;
Do = nominaldiameter;
WH = aspectratio;

for i = 1:3
    a = angles(i);
    fun = @(x) ellipseintersect(x, Do, a, WH);
    P(i) = fzero(fun, x0);
end

ellipse = @(x, Do, WH) sqrt(Do^2/WH^2 - 4/WH^2*x.^2);

P = [nominaldiameter/2, P, ventdiameter/2];
P = P';
P = [P, ellipse(P, Do, WH)];

dist = @(P0, P1) sqrt( (P1(1,1)-P0(1,1))^2 + (P1(1,2)-P0(1,2))^2 );
traparea = @(a, b, h) 0.5*(a + b) * h;

goredesign.h1 = dist(P(1,1:2),P(2,1:2));
goredesign.h2 = dist(P(2,1:2),P(3,1:2)) + goredesign.h1;
goredesign.h3 = dist(P(3,1:2),P(4,1:2)) + goredesign.h2;
goredesign.h4 = dist(P(4,1:2),P(5,1:2)) + goredesign.h3;

goredesign.es = 2*pi*P(1,1) / numgores;
goredesign.e1 = 2*pi*P(2,1) / numgores;
goredesign.e2 = 2*pi*P(3,1) / numgores;
goredesign.e3 = 2*pi*P(4,1) / numgores;
goredesign.ev = 2*pi*P(5,1) / numgores;

goredesign.area = traparea(goredesign.es, goredesign.e1, goredesign.h1) + ...
                  traparea(goredesign.e1, goredesign.e2, goredesign.h2-goredesign.h1) + ...
                  traparea(goredesign.e2, goredesign.e3, goredesign.h3-goredesign.h2) + ...
                  traparea(goredesign.e3, goredesign.ev, goredesign.h4-goredesign.h3);
