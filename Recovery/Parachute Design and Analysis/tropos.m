function [rho,a,temp,press,kvisc]=tropos(h_in,tOffset)
% Copyright (c) 2016, Sky Sartorius
% All rights reserved.
% 
% Redistribution and use in source and binary forms, with or without
% modification, are permitted provided that the following conditions are
% met:
% 
%     * Redistributions of source code must retain the above copyright
%       notice, this list of conditions and the following disclaimer.
%     * Redistributions in binary form must reproduce the above copyright
%       notice, this list of conditions and the following disclaimer in
%       the documentation and/or other materials provided with the distribution
% 
% THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
% AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
% IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
% ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
% LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
% CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
% SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
% INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
% CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
% ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
% POSSIBILITY OF SUCH DAMAGE.
% 
% TROPOS  Stripped-down version of atmos, applicable only to the troposphere
% (covers the vast majority of atmospheric flight), for when speed is key.
% 
%   [rho,a,T,P,nu] = ATMOS(h)
%   [rho,a,T,P,nu] = ATMOS(h,dT)
%   
%   See also ATMOS.


if nargin < 2
    tOffset = 0;
end
if nargin < 1
    h_in = 0;
end

dimVarOut = false;
if isa(h_in,'DimVar')
    h_in = h_in/u.m;
    dimVarOut = true;
end
if isa(tOffset,'DimVar')
    tOffset = tOffset/u.K;
    % It is allowed to mix DimVar h_in and double tOffset (or reverse). 
end

% h_in(h_in>11000 | h_in<0) = NaN;

TonTi=1-2.255769564462953e-005*h_in;
press=101325*TonTi.^(5.255879812716677);
temp = TonTi*288.15 + tOffset;
rho = press./temp/287.05287;

a = sqrt(401.874018 * temp);
kvisc = (1.458e-6 * temp.^1.5 ./ (temp + 110.4)) ./ rho;

if dimVarOut
    rho = rho*u.kg/(u.m^3);
    a = a*u.m/u.s;
    temp = temp*u.K;
    press = press*u.Pa;
    kvisc = kvisc*u.m^2/u.s;
end