%Major approximations: estimaded Cd stays constanst at 2.2 even when 
%diameter changes
%Wind data of different time of year and not thorough 
%Object moves as fast as wind 
%Wind is always in the same direction

%% Initial conditions, measurements, variables, etc.
%units (m,kg)
clc
clear all 
close all
g_0 = 9.8;
m_nosecone = 4.535;
d_rocket_nosecone = .10551;
l_nosecone = .5588;
m_payload = 11.34;
d_rocket_payload = 0.152; 
l_payload = 1.021; 
m_engine = 27.2;
d_rocket_engine = d_rocket_payload;
l_engine =  1.374; 
a_fin = .088; %one fin

%Diameter of the parachute 
a_chute_nc = (0.9144.^2)/4 * pi;%36 inches
a_chute_pl = (1.2192.^2)/4 * pi;%48 inches
a_chute_eng =(2.1336.^2)/4 * pi;%84 inches 

%coefficient of drag for the parachutes
%Parachute drag based on circular parachute, from Knacke 5-25
cd_parachute = 2.2; % more like 1.5
cd_cylinder = 1;

%Calculates the areas of each part of the rocket
a_rocket_nosecone = l_nosecone * d_rocket_nosecone;
a_rocket_payload = l_nosecone * d_rocket_nosecone;
a_rocket_engine = l_nosecone * d_rocket_nosecone + 2 * a_fin;
a_rocket = a_rocket_nosecone + a_rocket_payload + a_rocket_engine;

%Horizontal Wind Velocities 
windspeed=0;


%% Density profile of the atmosphere (0-13000m) 
  
T_0 = 288.16; %sea level temperature (kelvin)
rho_0 = 1.225; %sea level density (kg/m^3)
a = -0.0065; %lapse rate (K/m)
R = 287.05; %gas constant (J/kg*K)
den = []; 

for h = 1:11000 
    T = T_0 + a*h; 
    den(h) = rho_0 * (T/T_0)^((-g_0/(a*R)) - 1 );
end 
T_isot = 217; %isothermal temp (K)
for h = 11001:13000
    den(h) = den(11000) * (exp(1))^((-g_0/(T_isot*R))* (h-11000));
end

%% Calculations of the Decent Profile 
%Using First-order Euler approximation to calculate velocities 
dt = 0.1;
horiz_displacements_NC = []; 
chuteDiameters = linspace(0,1,200);

nosecone_height=[];
nosecone_height(end+1)=12192;
time_vector=[];
time_vector(end+1)=0;
index_interest=58
counter=0;
for a_chute =(chuteDiameters.^2./4 .* pi);
    counter=counter+1;
    horiz_disp_NC = 0;
    v_i = 0; 
    v_curr = v_i;
    h_curr = 12192; %40000ft 
    windspeed = 0;
    timecounter=0;
    while (h_curr > 152.4) %cutting the reefing at 500ft, so only care about hDisp till then
    timecounter=timecounter+1;
    F_g = m_nosecone * g_0; 
    F_drag = .5 .* den(round(h_curr)) .* v_curr.^2 .* a_chute .* cd_parachute; 
    F_curr = F_drag - F_g; 
    dv = (F_curr/m_nosecone).*dt; 
    v_curr = v_i + dv;
    dh = v_i .* dt;
    %DEBUG:
    if(counter==58) %Chose diameter of 1 feet
        nosecone_height(end+1)=nosecone_height(end)+dh;
        time_vector(end+1)=timecounter*dt;
    end
    
    
    v_i = v_curr; 
    h_curr = h_curr + dh; 
       
    American_Pos=h_curr.*3.28084; %convert h_curr to ft 
    if American_Pos<5115
    windspeed=13.8;
    elseif American_Pos<10000
    windspeed=12.7;
    elseif American_Pos<14639
    windspeed=19.6;
    elseif American_Pos<20000
    windspeed=29.9;
    elseif American_Pos<25676
    windspeed=41.4;
    elseif American_Pos<30000
    windspeed=58.7;
    elseif American_Pos<35751
    windspeed=74.8;
    elseif American_Pos<40226
    windspeed=85.2;
    elseif American_Pos<45242
    windspeed=63.3;
    else
    error('Height more than 45,000ft')
    end
    
    windVel=windspeed.*0.44704; %Coversion of windspeed to SI units
    horiz_disp_NC = (windVel.*dt) + horiz_disp_NC; 
    end 
    horiz_displacements_NC = [horiz_displacements_NC horiz_disp_NC];
end

plot(time_vector,nosecone_height)
title('Nosecone 1ft dimeter, Height versus time')
xlabel('time (s)')
ylabel('height(meters)')
hold on;
figure;
%payload section 



payload_height=[];
payload_height(end+1)=12192;
time_vector=[];
time_vector(end+1)=0;
index_interest=89;%reefed diameter of 1.5 feet
counter=0;


horiz_displacements_PL = []; 
for a_chute =(chuteDiameters.^2./4 .* pi);
    
     counter=counter+1;
    horiz_disp_PL = 0;
    v_i = 0; 
    v_curr = v_i;
    h_curr = 12192; %40000ft 
    windspeed = 0;
    timecounter=0;
    while (h_curr > 152.4) %cutting the reefing at 500ft, so only care about hDisp till then
    timecounter=timecounter+1;
    F_g = m_payload * g_0; 
    F_drag = .5 .* den(round(h_curr)) .* v_curr.^2 .* a_chute .* cd_parachute; 
    F_curr = F_drag - F_g; 
    dv = (F_curr/m_payload).*dt; 
    v_curr = v_i + dv;
    dh = v_i .* dt;
    v_i = v_curr; 
    h_curr = h_curr + dh; 
    
    if(counter==89) %Chose diameter of 1 feet
        payload_height(end+1)=payload_height(end)+dh;
        time_vector(end+1)=timecounter*dt;
    end
       
    American_Pos=h_curr.*3.28084; %convert h_curr to ft 
    if American_Pos<5115
    windspeed=13.8;
    elseif American_Pos<10000
    windspeed=12.7;
    elseif American_Pos<14639
    windspeed=19.6;
    elseif American_Pos<20000
    windspeed=29.9;
    elseif American_Pos<25676
    windspeed=41.4;
    elseif American_Pos<30000
    windspeed=58.7;
    elseif American_Pos<35751
    windspeed=74.8;
    elseif American_Pos<40226
    windspeed=85.2;
    elseif American_Pos<45242
    windspeed=63.3;
    else
    error('Height more than 45,000ft')
    end
    
    windVel=windspeed.*0.44704; %Coversion of windspeed to SI units
    horiz_disp_PL = (windVel.*dt) + horiz_disp_PL; 
    end 
    horiz_displacements_PL = [horiz_displacements_PL horiz_disp_PL];
end
plot(time_vector,payload_height)
title('Payload 1.5ft dimeter, Height versus time')
xlabel('time (s)')
ylabel('height(meters)')
hold on;
figure;

%engine section

engine_height=[];
engine_height(end+1)=12192;
time_vector=[];
time_vector(end+1)=0;
index_interest=148;%reefed diameter of 2.5 feet
counter=0;

horiz_displacements_ENG = []; 
for a_chute =(chuteDiameters.^2./4 .* pi);
    horiz_disp_ENG = 0;
    v_i = 0; 
    v_curr = v_i;
    h_curr = 12192; %40000ft 
    windspeed = 0
    counter=counter+1;
    timecounter=0;
    
    while (h_curr > 152.4) %cutting the reefing at 500ft, so only care about hDisp till then
    timecounter=timecounter+1;
    F_g = m_engine * g_0; 
    F_drag = .5 .* den(round(h_curr)) .* v_curr.^2 .* a_chute .* cd_parachute; 
    F_curr = F_drag - F_g; 
    dv = (F_curr/m_engine).*dt; 
    v_curr = v_i + dv;
    dh = v_i .* dt;
    v_i = v_curr; 
    h_curr = h_curr + dh; 
    
    if(counter==148) %Chose diameter of 2.5 feet
        engine_height(end+1)=engine_height(end)+dh;
        time_vector(end+1)=timecounter*dt;
    end
       
    American_Pos=h_curr.*3.28084; %convert h_curr to ft 
    if American_Pos<5115
    windspeed=13.8;
    elseif American_Pos<10000
    windspeed=12.7;
    elseif American_Pos<14639
    windspeed=19.6;
    elseif American_Pos<20000
    windspeed=29.9;
    elseif American_Pos<25676
    windspeed=41.4;
    elseif American_Pos<30000
    windspeed=58.7;
    elseif American_Pos<35751
    windspeed=74.8;
    elseif American_Pos<40226
    windspeed=85.2;
    elseif American_Pos<45242
    windspeed=63.3;
    else
    error('Height more than 45,000ft')
    end
    
    windVel=windspeed.*0.44704; %Coversion of windspeed to SI units
    horiz_disp_ENG = (windVel.*dt) + horiz_disp_ENG; 
    end 
    horiz_displacements_ENG = [horiz_displacements_ENG horiz_disp_ENG];
end

plot(time_vector,engine_height)
title('Engine 2.5ft dimeter, Height versus time')
xlabel('time (s)')
ylabel('height(meters)')
hold on;
figure;

hold on
plot(chuteDiameters .* 3.2808, horiz_displacements_NC ./ 1609.34 ) %converting to miles and feet
plot(chuteDiameters .* 3.2808, horiz_displacements_PL ./ 1609.34, 'r' )
plot(chuteDiameters .* 3.2808, horiz_displacements_ENG ./ 1609.34, 'g' )
title('Chute Diameter vs. Horizontal Displacement');
xlabel('Diameter (feet)'); 
ylabel('Horizontal Displacement (miles)'); 
legend('Nose Cone', 'Payload', 'Engine'); 

grid on