clear
clc

data1 = xlsread('DensityValues(metric)modified');
altitudes = data1(1:261,1);

Cd = 1.5;
d_chute = 0.9144; %36 inches to meters
A_chute = pi*d_chute^2/4;
m_section = 2; %kg
g = 9.81;
Fg = m_section*g;

h0 = 13000;
v0 = 0;
p = 1.12; 

t_i = 0;
h_i = h0;
v_i = v0;
dt = 0.1;

counter = 1;

while (h_i > 5)
    
[c index] = min(abs(altitudes-h_i));
p = data1(index,2);

F_Di = 0.5*p*v_i^2*A_chute*Cd;

h_i1 = h_i + v_i*dt;
v_i1 = v_i + ((F_Di - Fg)/m_section)*dt;

h_i = h_i1;
v_i = v_i1;
t_i = t_i + dt;

h_vector(counter) = h_i;
v_vector(counter) = v_i;
t_vector(counter) = t_i/60;
p_vector(counter) = p;
F_vector(counter) = F_Di;

counter = counter + 1;
    
end

h_i
t_i/60
v_i
F_Di

plot(t_vector,h_vector)
figure();
plot(t_vector,-v_vector)
figure();
plot(t_vector,F_vector)
figure();


