% Subcale weight
% in grams
% Ian Gomez, 03/07/17
clc;

nose = 211.64;
body = 155.56; 
recovery1 = 113.38;
recovery2 = 100.76;
aft = 254.07; 
avbay = 225.15+52.84;
fins = 191.86;
motorretainer = 17.3;
notmotor = nose + body + recovery1 + recovery2 + aft + avbay +...
    fins + motorretainer;

% jcasing = 335.55;
% jproponly = 591;
jtotal = 1111;

% hcasing = 122.18;
% hproponly = 115;
htotal = 264; 

hconfig = notmotor + htotal % g
jconfig = notmotor + jtotal % g

g2lb = 0.00220462;  % lb/g
hlbs = hconfig*g2lb % lb
jlbs = jconfig*g2lb % lb