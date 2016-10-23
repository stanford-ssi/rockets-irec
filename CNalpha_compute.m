function [CNalpha_total] = CNalpha_compute( fin, bodytube, M )
% Logan Herrera
% Stanford Student Space Initiative - IREC
% 2016-40-21
% 
% This function uses the modified Barrowman method of Sampo Niskanen and
% OpenRocket to calculate the coefficient of normal force (CNalpha) of a
% set of fins at a given Mach number M. 
% 
% Function currently assumes subsonic flight, at least 3 fins, and a single
% airframe diameter. It does not require the fins be physically realizable,
% so user beware. Use of dimensions in units other than inches has not been
% tested.

%     % these are all of the input struct parameters used
%     fin.rootlength;
%     fin.tiplength;
%     fin.height;
%     fin.sweepdistance;
%     fin.lengthunit;
%     fin.number;
%     bodytube.outerdiameter;
%     bodytube.lengthunit;

% start by doing some basic error checking.
    if ~isstruct(fin)
        error('fin input must be a struct')
    elseif ~isstruct(bodytube)
        error('bodytube input must be a struct')
    elseif (fin.lengthunit ~= bodytube.lengthunit)
        error('fin and bodytube length units must match')
    elseif ~isnumeric(M)
        error('Mach number input must be numeric')
    end            
    
    beta = sqrt(1-M^2);
    area_fin = 0.5*(fin.rootlength+fin.tiplength)*fin.height;
    Gammac = atan( (fin.sweepdistance+fin.tiplength/2-fin.rootlength/2) / fin.height );  % radians
    bodytube_outerradius = bodytube.outerdiameter/2;
    KTB = 1 + bodytube_outerradius / (fin.height + bodytube_outerradius);
    ARef = pi * bodytube_outerradius^2;
% turns out not all of these are necessary. There's a simplified formula I
% missed at first. gamma and K's are for transonic/low supersonic 
% correction, which is not implemented
%     CNalphanot = 2*pi/beta;
%     FD = AR*2*pi / (CNalphanot * cos(Gammac) );
%     AR = 2*fin.height^2 / area_fin;
%     gamma = 1.400;
%     K1 = 2/beta;
%     K2 = ((gamma+1)*M^4 - 4*beta^2)/(4*beta^4);
%     K3 = ((gamma+1)*M^8 + (2*gamma^2-7*gamma-5)*M^6 + 10*(gamma+1)*M^4 + 8) / (6*beta^7);

    
    % Sampo Niskanen thesis equation 3.40 with equation 3.56
    CNalpha = KTB * (2*pi*fin.height^2/ARef) / ( 1+sqrt(1+(beta*fin.height^2 / (area_fin*cos(Gammac)))^2) );
    
    % Sampo Niskanen thesis equation 3.53    
    CNalpha_total = 0.5 * fin.number * CNalpha;

end