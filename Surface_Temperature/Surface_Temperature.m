%Take in parameters
function T_layer_history = Surface_Temperature(h_vec, v_vec, dt, length,...
    specific_heat, thermal_cond, surface_thickness, density)
%dt should be a multiple of .1, is time step between h and v data points
%Length is the length down the rocket that we are considering a point at
%specific heat of surface material
%thermal conductivity of surface material
%thickness of surface material
%density of surface material

%Define variables
g = 9.81;
boltzmann = 5.6703 * 10^-8;                 %Stefan Boltzmann constant
emissivity = .8;

n_layers = 5;                               %Layers to the surface for the FEA
thickness_layer = surface_thickness / n_layers;
T_surr = 273;                               %K
T_layers = zeros(1, n_layers);
thermal_capacity = thickness_layer * density * specific_heat

for i=1:n_layers
    T_layers(i) = 293;
end

dyn_viscosity = 1.4  * 10^-5;               %dynamic viscosity, kg/m/s
kin_viscosity = 1.48 * 10^-5;               %kinematic viscosity, m^2/s. Is variable, write a function?
specific_heat_air = 1020;                   %Changes depending on temp
%Prandtl # approx (changes due to temp a bit)
Pr = .69;
thermal_cond_air = .04;                     %Changes depending on temp

[blah, n_steps] = size(h_vec);

T_layer_history = zeros(n_steps, n_layers);

%Begin for loop through each altitude
for i=1:n_steps
    h = h_vec(i);
    v = v_vec(i);
    
%Get pressure, density and temperature for this altitude (using standard
%model)
    [P, rho, T] = Atmosphere_Model(h);
    
%Get Reynolds + Prandtl #
    Re = v * length / kin_viscosity;

%Get stagnation temp, pressure, density
%Tr is the recovery temp, from http://www.thermopedia.com/content/291/
    [T_stag, P_stag, rho_stag, Tr] = Stagnation_Properties(v, P, rho, T, Pr, Re);
    h
    T
    T_stag
    Tr

%Convection into the surface
    %From http://www.dtic.mil/dtic/tr/fulltext/u2/462332.pdf
    h = .332 * thermal_cond_air / length * sqrt(Re) * Pr^(1/3)

%Conduction into the surface
    time_step = .001;
    steps = dt / time_step;
    for j=1:steps
        %Holds heat into and out of each layer
        q_layers = zeros(1, n_layers);
        
        %Radiation out of the surface
        q_r = emissivity * boltzmann * (T_layers(1)^4 - T_surr^4);
        
        %Convection into the surface
        %t_diff = Tr - T_layers(1)
        q_conv = h * (Tr - T_layers(1));
        
        %Conduction through the material
        q_cond_old = 0;
        for k=1:n_layers
            if k < n_layers
                q_cond = thermal_cond / thickness_layer * (T_layers(k) - T_layers(k + 1));
            else
                q_cond = 0;
            end
            
            %For layer one is convection in - radiation out - conduction down
            if k == 1
                q_layers(k) = q_conv - q_r - q_cond;
            %For layers 2-4 is conduction into - conduction out of
            %Layer 5 conduction out of is 0
            else
                q_layers(k) = q_cond_old - q_cond;
            end
            
            q_cond_old = q_cond;
        end
        %temp_increase_per_step = q_layers / thermal_capacity * time_step
        T_layers = T_layers + q_layers / thermal_capacity * time_step;
    end
    %T_layers
    %End for loop
    in_C = T_layers - 273;
    in_F = in_C * 9 / 5 + 32;
    T_layer_history(i,:) = T_layers;
end
end