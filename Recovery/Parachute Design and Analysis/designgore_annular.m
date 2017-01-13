function[goredesign] = designgore_annular(nominaldiameter, numgores)
% Logan Herrera
% Stanford Student Space Initiative - IREC
% 
% References:
% Parachute Recovery Systems Design Manual - T. W. Knacke - 1992

% see figure 6-19

Do = 1.000 * nominaldiameter;
D = 1.040 * nominaldiameter;
Dc = 0.940 * nominaldiameter;
Dv = 0.611 * nominaldiameter;
L = 1.250 * nominaldiameter;
hx = 0.209 * nominaldiameter;
a = 0.319 * nominaldiameter;
b = 0.200 * nominaldiameter;

goredesign.Cv = pi * Dv / numgores;
goredesign.Cs = pi * D / numgores;
goredesign.h = 0.304 * nominaldiameter;

goredesign.area = 0.5*(goredesign.Cv + goredesign.Cs) * goredesign.h;
