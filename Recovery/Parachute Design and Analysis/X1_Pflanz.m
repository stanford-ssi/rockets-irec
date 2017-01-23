function[X1] = X1_Pflanz(A, n)
% Logan Herrera
% Stanford Student Space Initiative - IREC
% 
% References:
% Parachute Recovery Systems Design Manual - T. W. Knacke - 1992

% digitize Figure 5-51

if A > 10
    X1 = 1;
    
else
    figure5_51 = csvread('openingforcereductionfactorX1_vs_ballisticparameterA_givenn.csv');
    
    F = scatteredInterpolant(figure5_51(:,1), figure5_51(:,2), figure5_51(:,3),'linear','linear');
    X1 = F(A, n);
    
end