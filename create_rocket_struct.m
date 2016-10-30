rocket.alpha = 0:0.1:8;              % angle of attack in deg
rocket.cg = 79;                      % in from nosecone tip
rocket.length = 129;                 % in
%Fin area of 2/3 fins (two will be presented to the oncoming air, one will
%be "hidden". Both sides of the fin count

bodytube.lengthunit = 'in';
bodytube.OD = 4;                     % in
bodytube.OR = bodytube.OD/2;         % in
bodytube.t = 0.08;                   % in 
bodytube.ID = bodytube.OD-bodytube.t;% in 
bodytube.IR = bodytube.ID/2;         % in
bodytube.S_ref = pi * bodytube.OR^2; % in^2

fin.lengthunit = 'in';
fin.t = (1/16):(1/16):(1/4);         % thickness
fin.h = 4;                           % span
fin.rootlength = 8;                  % root chord
fin.tiplength = 2;                   % tip chord
fin.sweepdistance = fin.rootlength/2 - fin.tiplength/2; % from top of root chord
fin.S = 0.5.*(fin.rootlength + fin.tiplength).*fin.h   % in^2
fin.AR = (fin.sweepdistance.^2)/fin.S;                  % aspect ratio
fin.number = 3;

rocket.bodytube = bodytube; clear bodytube
rocket.fin = fin; clear fin