function [CNalpha_total] = CNalpha_compute( fin, bodytube, Ma )
% Logan Herrera
% Stanford Student Space Initiative - IREC
% 2016-40-21
% 
% This function uses the modified Barrowman method of Sampo Niskanen and
% OpenRocket to calculate the coefficient of normal force (CNalpha) of a
% set of fins at a given Mach number Ma. 
% 
% Function currently assumes subsonic flight, at least 3 fins, and a single
% airframe diameter. It does not require the fins be physically realizable,
% so user beware. Use of dimensions in units other than inches has not been
% tested.

%     % these are all of the input struct parameters used
%     fin.rootlength;
%     fin.tiplength;
%     fin.h;
%     fin.sweepdistance;
%     fin.lengthunit;
%     fin.number;
%     bodytube.OD;
%     bodytube.lengthunit;

% start by doing some basic error checking.
    if ~isstruct(fin)
        error('fin input must be a struct')
    elseif ~isstruct(bodytube)
        error('bodytube input must be a struct')
    elseif (fin.lengthunit ~= bodytube.lengthunit)
        error('fin and bodytube length units must match')
    elseif ~isnumeric(Ma)
        error('Mach number input must be numeric')
    end            
    
    beta = sqrt(1-Ma^2);
    Gammac = atan( (fin.sweepdistance+fin.tiplength/2-fin.rootlength/2) / fin.h );  % radians
    KTB = 1 + bodytube.OR / (fin.h + bodytube.OR);
% turns out not all of these are necessary. There's a simplified formula I
% missed at first. gamma and K's are for transonic/low supersonic 
% correction, which is not implemented
%     CNalphanot = 2*pi/beta;
%     FD = AR*2*pi / (CNalphanot * cos(Gammac) );
%     AR = 2*fin.h^2 / fin.S;
%     gamma = 1.400;
%     K1 = 2/beta;
%     K2 = ((gamma+1)*Ma^4 - 4*beta^2)/(4*beta^4);
%     K3 = ((gamma+1)*Ma^8 + (2*gamma^2-7*gamma-5)*Ma^6 + 10*(gamma+1)*Ma^4 + 8) / (6*beta^7);

    
    % Sampo Niskanen thesis equation 3.40 with equation 3.56
    CNalpha = KTB * (2*pi*fin.h^2/bodytube.S_ref) /...
        ( 1+sqrt(1+(beta*fin.h^2 / (fin.S*cos(Gammac)))^2) );
    
    % Sampo Niskanen thesis equation 3.53    
    CNalpha_total = 0.5 * fin.number * CNalpha;

end