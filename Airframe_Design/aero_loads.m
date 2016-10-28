function [compression, bending] = aero_loads(maxq,rocket)

% calculate moment based on alpha

inertia_body = pi.*(rocket.bodytube.OD.^4 - rocket.bodytube.ID.^4)./64;    
c = rocket.bodytube.OR;

compression = zeros(1,length(rocket.alpha));
bending = zeros(1,length(rocket.alpha));
for i = 1:length(rocket.alpha)
    
    compression(i) = maxq .* rocket.alpha(i);
    bending = moment.*c./inertia_body;

end

end