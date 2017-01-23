function[mass] = calculateparachutemass(parachute)
% Logan Herrera
% Stanford Student Space Initiative - IREC
% 

fabricareafudgefactor = 1.1;
riserlengthfudgefactor = 1.1;

totalfabricarea = parachute.numgores * parachute.goredesign.area * fabricareafudgefactor;

if strcmp(parachute.fabric.ripstop1_1oz_OK,'TRUE')
    fabricmass =  0.02835 * 1.1 * totalfabricarea;
elseif strcmp(parachute.fabric.ripstop2_2oz_OK,'TRUE')
    fabricmass =  0.02835 * 2.2 * totalfabricarea;
end

totalriserlength = parachute.numgores * parachute.nominaldiameter * riserlengthfudgefactor;

% need to switch on all possible paracord types
if strcmp(parachute.riser.type_II_OK,'TRUE')
    risermass = 1.57e-3 * totalriserlength;
else
end

% thread length is a total guess
totalthreadlength = 10 * totalriserlength;
threadmass = 8.268e-5 * totalthreadlength;

mass = fabricmass + risermass + threadmass;