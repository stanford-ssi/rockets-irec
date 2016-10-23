%Calculating the mass and volume of weights in Despin Unit 
%Units are metric (m, kg) 

l = .225; %length of each chord, determined from solidworks 
I = .08709; %moment of Inertia of rocket (kg*m^2) (from rocksim)
r = .0762; %radius of rocket (3 inches) 
%l = 0.26; %model 
%I = .00503; %model
%r = .115; %model 

%for princeton equation: l = r* sqrt(1 + (I/(m*R^2)))
%solving for m gives: m = I/(l^2-r^2)
%M_princeton = I./(l^2 - r^2) %M is total mass of both weights and the two wires

M = I./(l+r)^2;  
each_weight_kg = M/2 

%calculating mass of both external and internal wire (mass = pi*r^2*l*rho)
r_int = 0.00159; %using 1/8" steel wire 
r_ext = 0.00159; 
rho_steel = 7860; %(kg/m^3)
l_int = 0.152; %(6 inches)
l_ext = 0.21; 
interior = pi*r_int^2*l_int*rho_steel; 
exterior = pi*r_ext^2*l_ext*rho_steel; 
m_wires = interior + exterior; 
m_weight = (M - m_wires)/2; %final mass of each weight in kg

%calculating volume needed for each weight using aluminum 
rho_al = 2700; 
dv = m_weight./rho_al; %in m^3