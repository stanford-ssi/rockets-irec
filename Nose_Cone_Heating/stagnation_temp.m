% Stagnation Temperature Calculation
% Ian Gomez
% Input: Ma, Tatm
% Output: Plot of T0 v Ma
close all; clc;

Ma = 0:.01:1.5;
Tatm = 293; %K
Tmelt = 273+150; %K

k = 1.4;    %ratio of specific heats for air
Tmelt_array = Tmelt*ones(length(Ma)); %for plotting
T0overT = (1+((k-1)/2).*Ma.^2).^(1/(k-1));
T0 = T0overT*Tatm; %K

figure(1)
hold on
set(gcf,'color','w');
plot(Ma,T0,'b')
plot(Ma,Tmelt_array,'r','LineStyle','--')
title('T_0 vs changing Mach numbers')
xlabel('Mach Number'); ylabel('T_0 (K)')
legend('T_0',strcat('T_{melt} =  ',num2str(Tmelt),'K'),'Location',...
    'Northwest')
plotfixer

%% Tip Heating Calculation Explanation

% Assume air is a perfectly caloric and ideal gas, that the highest 
% temperature the tip of the rocket will experience will be the 
% stagnation temperature of the flow. In order to get the stagnation 
% temperature, we assume the flow of the air at the tip is slowed to 
% a complete stop isentropically (no entropy generation or heat loss/addition). 

% The flow is characterized by a few dimensionless parameters, in 
% particular we care about:

% Ma = velocity of the flow/speed of sound

% k = 1.4, which is the ratio of the constant 
% pressure specific heat and the constant volume specific heat of air

% There are plenty of proofs for the isentropic flow relations, so 
% I will leave that as an exercise for the reader. Equations used 
% are: the ideal gas equation of state, 2nd law of thermo, conservation 
% of mass, conservation of momentum, definition of enthalpy, and 
% finally the enthalpy equations. 
% https://www.grc.nasa.gov/www/k-12/airplane/isentrop.html

% T / Tt = [1 + Ma^2 * (gamma-1)/2]^-1 
% where Tt is our stagnation temperature, T is our ambient temperature, 
% Ma is our mach number, and gamma is our specific heats ratio 
% (which = 1.4 for air)

% Since the equation doesn't change for a particular geometry, we can't 
% relate it to our nose cone easily. There are a few ways to do this:

% 1. model some portion of the nose cone skin at that stagnation temperature 
% for some amount of time as the worst case scenario

% 2. run a cfd simulation that also does heat transfer analysis (model the 
% flow as extremely turbulent for an agressive estimate of temperature), or 
% in the same vein, do a FEA simulation with the boundary conditions specified in 1.

% It seems like our kind of composites (CF and fiber glass) will have a 
% tough time dealing with extended periods of exposure to the temperature 
% expected in the worst case scenario. 

% Final engineering decision: The temperatures that the flow will cause 
% is hot enough to weaken any composite nose cones significantly. However, this 
% highly depends on how long the nose cones experience the stagnation 
% temperatures predicted. Therefore, if above transonic flight is required, 
% replacing the tip with steel or aluminum is suggested (which will put the 
% factor of safety well above 2). If weight is an issue, more analysis will 
% be required to safely reduce our factor of safety. 