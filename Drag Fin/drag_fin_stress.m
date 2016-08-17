% Drag Fin Analysis
% Ian Gomez,  08/17/16
% This script attempts to do a simplified analysis of a the drag fins in
% order to characterize the order of magnitudes of stress 

P_sl  = 101300; % Pa, sea level pressure 
P_atm = @(h) P_sl.*exp((-m_air.*g.*h)./(k_boltz.*T_atm));

Ma = 0.81;

P0 = P_atm.*(1+((k-1)/2).*Ma.^2).^(k/(k-1));