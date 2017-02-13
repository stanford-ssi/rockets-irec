function [parachute] = parachute_design(ground_alt, open_alt, mass, descent_velocity, type, freefall_velocity, forebody_area, total_porosity, temp)
% Logan Herrera
% Stanford Student Space Initiative - IREC
%
% Returns a parachute object describing a physical parachute capable of 
% descending at a particular rate while suspending a particular mass at
% the given operating conditions.
%
% Units:
%       ground_alt: meters above MSL [m]
%       open_alt: meters above MSL [m]
%       mass: suspended mass [kg]
%       descent_velocity: descent velocity while parachute is open [m/s]
%       type: parachute type selected from list below. case insensitive [string]
%       freefall_velocity: freefall velocity before parachute opens [m/s]
%       forebody_area: area of bodies in front of parachute projected onto
%                           parachute area [m^2]
%       total_porosity: total porosity (lambda) of canopy fabric [%]
%       temp: operating condition temperature [C]
%
% TODO: 
% 
% References:
% Parachute Recovery Systems Design Manual - T. W. Knacke - 1992
% 
% This code has the following magic constant design assumptions
% 1. Factor of safety is 2
% 2. Use 12 gores
% 3. Vent area is 0.5% of equivalent surface area

g = 9.780318;
factorofsafety = 2;

if isempty(freefall_velocity); freefall_velocity = 5 * descent_velocity; end;
if isempty(forebody_area); forebody_area = 0; end;
if isempty(total_porosity); total_porosity = 0.05; end;  
if isempty(temp); temp_delta = 0; else temp_delta = temp - 15; end;
type = lower(type);

% assume 80th percentile CDo from table of values
% assume 50th percentile opening force coefficient from table of values
switch type
    case 'flat-circular'
        parachute.CDo = 0.2*0.75 + 0.8*0.80;
        parachute.DpDo = 0.5*0.64 + 0.5*0.70;
        parachute.openingforcecoefficient = 0.5*1.70 + 0.5*1.70;
        parachute.canopyfillconstant = 8;
        parachute.n = 2;
    case 'conical'
        parachute.CDo = 0.2*0.75 + 0.8*0.90;
        parachute.DpDo = 0.5*0.70 + 0.5*0.70;
        parachute.openingforcecoefficient = 0.5*1.80 + 0.5*1.80;
        parachute.canopyfillconstant = 9;       % guess
        parachute.n = 2;
    case 'biconical'
        parachute.CDo = 0.2*0.75 + 0.8*0.92;
        parachute.DpDo = 0.5*0.70 + 0.5*0.70;
        parachute.openingforcecoefficient = 0.5*1.80 + 0.5*1.80;
        parachute.canopyfillconstant = 9;       % guess
        parachute.n = 2;
    case 'triconical'
        parachute.CDo = 0.2*0.80 + 0.8*0.96;
        parachute.DpDo = 0.5*0.70 + 0.5*0.70;
        parachute.openingforcecoefficient = 0.5*1.80 + 0.5*1.80;
        parachute.canopyfillconstant = 9;       % guess
        parachute.n = 2;
    case 'extended-skirt-10'
        parachute.CDo = 0.2*0.78 + 0.8*0.87;
        parachute.DpDo = 0.5*0.66 + 0.5*0.70;
        parachute.openingforcecoefficient = 0.5*1.40 + 0.5*1.40;
        parachute.canopyfillconstant = 10;
        parachute.n = 2;
    case 'extended-skirt-14_3'
        parachute.CDo = 0.2*0.75 + 0.8*0.90;
        parachute.DpDo = 0.5*0.66 + 0.5*0.70;
        parachute.openingforcecoefficient = 0.5*1.40 + 0.5*1.40;
        parachute.canopyfillconstant = 12;
        parachute.n = 2;
    case 'hemispherical'
        parachute.CDo = 0.2*0.62 + 0.8*0.77;
        parachute.DpDo = 0.5*0.66 + 0.5*0.66;
        parachute.openingforcecoefficient = 0.5*1.60 + 0.5*1.60;
        parachute.canopyfillconstant = 9;       % guess
        parachute.n = 2;
    case 'guide-surface-ribbed'
        parachute.CDo = 0.2*0.28 + 0.8*0.42;
        parachute.DpDo = 0.5*0.62 + 0.5*0.62;
        parachute.openingforcecoefficient = 0.5*1.20 + 0.5*1.20;
        parachute.canopyfillconstant = 9;       % guess
        parachute.n = 2;
    case 'guide-surface-ribless'
        parachute.CDo = 0.2*0.30 + 0.8*0.34;
        parachute.DpDo = 0.5*0.63 + 0.5*0.63;
        parachute.openingforcecoefficient = 0.5*1.40 + 0.5*1.40;
        parachute.canopyfillconstant = 5;
        parachute.n = 2;
    case 'annular'
        parachute.CDo = 0.2*0.85 + 0.8*0.95;
        parachute.DpDo = 0.5*0.94 + 0.5*0.94;
        parachute.openingforcecoefficient = 0.5*1.40 + 0.5*1.40;
        parachute.canopyfillconstant = 12;       % guess
        parachute.n = 2;
    case 'cross'
        parachute.CDo = 0.2*0.60 + 0.8*0.85;
        parachute.DpDo = 0.5*0.66 + 0.5*0.72;
        parachute.openingforcecoefficient = 0.5*1.10 + 0.5*1.20;
        parachute.canopyfillconstant = 11.7;
        parachute.n = 1;                        % guess
    case 'rotafoil'
        parachute.CDo = 0.2*0.85 + 0.8*0.99;
        parachute.DpDo = 0.5*0.90 + 0.5*0.90;
        parachute.openingforcecoefficient = 0.5*1.05 + 0.5*1.05;
        parachute.canopyfillconstant = 10;       % guess
        parachute.n = 2;
    case 'vortex-ring'
        parachute.CDo = 0.2*1.50 + 0.8*1.80;
        parachute.DpDo = 0.5*1.00 + 0.5*1.00;
        parachute.openingforcecoefficient = 0.5*1.10 + 0.5*1.20;
        parachute.canopyfillconstant = 14;       % guess
        parachute.n = 0.5;                       % guess
    case 'sandia-rfd'
        parachute.CDo = 0.2*1.25 + 0.8*1.25;
        parachute.DpDo = 0.5*0.90 + 0.5*0.90;
        parachute.openingforcecoefficient = 0.5*1.10 + 0.5*1.10;
        parachute.canopyfillconstant = 9;       % guess
        parachute.n = 2;                        % same as triconical
end

% Figure 5-62
% reduce effective CDo based on fabric porosity. As porosity increases
% CDo decreases
parachute.CDo = parachute.CDo * 2*(0.5-total_porosity);

air_density_ground = tropos(ground_alt, temp_delta);
air_pressure_open = tropos(open_alt, temp_delta);
dynamic_pressure_ground = 0.5 * air_density_ground * descent_velocity^2;
dynamic_pressure_open = 0.5 * air_pressure_open * freefall_velocity^2;

% find the reference surface area based on necessary drag for ground hit
% velocity at ground altitude and temp. Reference area is not identical
% to actual fabric area. The parachute type determines the relationship.
%
% the projected diameter is smaller than the nominal diameter. This
% corresponds to the difference between the parachute resting on the ground
% versus the parachute in operation suspending a load in air
parachute.surfacearea = mass*g / (dynamic_pressure_ground*parachute.CDo);
parachute.nominaldiameter = sqrt(4*parachute.surfacearea / pi);
parachute.projecteddiameter = parachute.nominaldiameter * parachute.DpDo;
parachute.dragarea = pi/4 * parachute.projecteddiameter^2;

parachute.numgores = 12;
geometry_factor = 1/parachute.numgores;

% canopy loading is weight per area. Unclear if this is the reference area
% or drag area. Latter makes more sense to Logan H. 
parachute.canopyloading = g * mass / parachute.dragarea;

% every type of parachute has a cone of constant aspect ratio the projected
% area of the parachute will traverse as it opens. The length is the
% canopy fill constant * nominal diameter. The time to traverse this
% distance is no faster than this distanec divided bu the freefall velocity 
parachute.inflationtime = parachute.canopyfillconstant * parachute.nominaldiameter / freefall_velocity;

% these are two different methods for determining the maximum opening force.
% The worst case opening force "infinite mass condition" is the drag area *
% freefall velocity * opening force coefficient. The opening force is
% reduced if the parachute - mass systems slows down as it opens "finite
% mass condition". An additional opening force reduction factor (X1) term
% is multiplied to the previous expression to take this into account. A
% numerical approach is very useful if there is significant horizontal
% velocity (eg cargo deployed from back of plane). It is not considered
% here.
%
% X1_method1 depends only on canopy loading. 
%
% The Pflanz method (named after Pflanz) uses a "ballistic parameter" A and
% the shape of the inflation surface to fins max force
%
% Finally, to be maximally conservative, the greatest opening force of
% the two methods is returned as the maximum opening force

% study Figures 5-64 and 5-65 for porosity correction factor
X1 = X1_method1(mass, parachute.dragarea);               
maxforce_method1 = parachute.dragarea * dynamic_pressure_open * parachute.openingforcecoefficient * X1;

A = 2*mass / (parachute.dragarea*air_pressure_open*freefall_velocity*parachute.inflationtime);
X1 = X1_Pflanz(A, parachute.n);                          
maxforce_Pflanz = parachute.dragarea * dynamic_pressure_open * parachute.openingforcecoefficient * X1;

parachute.maxforce = max(maxforce_method1, maxforce_Pflanz);

% see Figure 5-20 and 5-21
forebody_diameter = sqrt(4*forebody_area / pi);
% this forebody correction factor is unfinished
% it corrects for the size of the object in front of the parachute
% disturbing the flow

% now start designing the actual fabric and ropes of the parachute

parachute.ventarea = 0.005 * parachute.surfacearea;
parachute.ventdiameter = sqrt(4*parachute.ventarea / pi);

% check section 6.4 there is a magic constant here (12)
parachute.fabricstress = (parachute.maxforce/g) / (12 * parachute.projecteddiameter);

MIL_C_7020_Type_1_breakingstrength = 750;       % kgf / m
MIL_C_7350_Type_1_breakingstrength = 1607;      % kgf / m

MIL_C_5040H_Type_I_tensilestrength = 43*g;      % N
MIL_C_5040H_Type_IA_tensilestrength = 45*g;
MIL_C_5040H_Type_II_tensilestrength = 181*g;
MIL_C_5040H_Type_IIA_tensilestrength = 102*g;
MIL_C_5040H_Type_III_tensilestrength = 249*g;
MIL_C_5040H_Type_IV_tensilestrength = 340*g;

if parachute.fabricstress*factorofsafety < MIL_C_7020_Type_1_breakingstrength
    parachute.fabric.ripstop1_1oz_OK = 'TRUE';
else
    parachute.fabric.ripstop1_1oz_OK = 'FALSE';
end
if parachute.fabricstress*factorofsafety < MIL_C_7350_Type_1_breakingstrength
    parachute.fabric.ripstop2_2oz_OK = 'TRUE';
else
    parachute.fabric.ripstop2_2oz_OK = 'FALSE';
end

%--------------------------------------------------------------------------

if parachute.maxforce*geometry_factor*factorofsafety < MIL_C_5040H_Type_I_tensilestrength
    parachute.riser.type_I_OK = 'TRUE';
else
    parachute.riser.type_I_OK = 'FALSE';
end
if parachute.maxforce*geometry_factor*factorofsafety < MIL_C_5040H_Type_IA_tensilestrength
    parachute.riser.type_IA_OK = 'TRUE';
else
    parachute.riser.type_IA_OK = 'FALSE';
end
if parachute.maxforce*geometry_factor*factorofsafety < MIL_C_5040H_Type_II_tensilestrength
    parachute.riser.type_II_OK = 'TRUE';
else
    parachute.riser.type_II_OK = 'FALSE';
end
if parachute.maxforce*geometry_factor*factorofsafety < MIL_C_5040H_Type_IIA_tensilestrength
    parachute.riser.type_IIA_OK = 'TRUE';
else
    parachute.riser.type_IIA_OK = 'FALSE';
end
if parachute.maxforce*geometry_factor*factorofsafety < MIL_C_5040H_Type_III_tensilestrength
    parachute.riser.type_III_OK = 'TRUE';
else
    parachute.riser.type_III_OK = 'FALSE';
end
if parachute.maxforce*geometry_factor*factorofsafety < MIL_C_5040H_Type_IV_tensilestrength
    parachute.riser.type_IV_OK = 'TRUE';
else
    parachute.riser.type_IV_OK = 'FALSE';
end

if strcmp(type, 'annular')
        parachute.goredesign = designgore_annular(parachute.nominaldiameter, parachute.numgores);
else if strcmp(type, 'triconical')
        parachute.goredesign = designgore_triconical(parachute.nominaldiameter, parachute.numgores, parachute.ventdiameter, 4);
    end
end


