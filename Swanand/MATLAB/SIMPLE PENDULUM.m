clear;
clc;

g=9.8;
l=1;
theta0=pi/4
omega0=0;
tspan = [0 10]

pendulumODE = @(t,y) [y(2); -g/l*sin(y(1))]
initialconditions = [theta0; omega0];
[t, y] = ode45(pendulumODE, tspan, initialconditions);

figure;
plot(t,y(:, 1));
xlabel('Time (s)');
ylabel('Angle (rad)');
title('pendulum');
grid on;