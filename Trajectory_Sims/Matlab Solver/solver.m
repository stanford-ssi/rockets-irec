function [r, u, a] = solver(rocket, f_x, f_y, f_theta, r, u, a, t, i)

    

    %x: initial position (number-like object)
    %v: initial velocity (number-like object)
    %a: acceleration function a(x,v,dt) (must be callable)
    x1 = r(i)
    v1 = u(i)
    a1 = f_x(i) / rocket.mass(i)

    x2 = x + 0.5*v1*TIME_STEP
    v2 = v + 0.5*a1*TIME_STEP
    a2 = F(x2, v2, TIME_STEP/2.0) / (ROCKET_MASS)

    x3 = x + 0.5*v2*TIME_STEP
    v3 = v + 0.5*a2*TIME_STEP
    a3 = F(x3, v3, TIME_STEP/2.0) / (ROCKET_MASS)

    x4 = x + v3*TIME_STEP
    v4 = v + a3*TIME_STEP
    a4 = F(x4, v4, TIME_STEP) / (ROCKET_MASS)

    xf = x + (TIME_STEP/6.0)*(v1 + 2*v2 + 2*v3 + v4)
    vf = v + (TIME_STEP/6.0)*(a1 + 2*a2 + 2*a3 + a4)



end
