% Simple Pendulum Animation with Damping and LQR Control
clear; clc; % Clear workspace and command window

% Parameters
g = 9.81; % Acceleration due to gravity (m/s^2)
L = 1;    % Length of the pendulum (m)
theta0 = 0; % Initial angle (radians)
omega0 = 0;    % Initial angular velocity (rad/s)
tspan = [0 100]; % Time span for the simulation (0 to 10 seconds)
m = 1;    % Mass of the pendulum (kg)

% State-space representation (linearized system)
A = [0 1; -g/L 0];
B = [0; 1/(m*L^2)];
C = eye(2);
D = [0; 0];

% LQR design
Q = [5000 0; 0 10000]; % State cost matrix
R = 70;           % Control effort cost
K = lqr(A, B, Q, R); % LQR gain

% Desired final state (theta, omega)
theta_des = 3*pi/4;  % Desired final angle (radians)
omega_des = 0;  % Desired final angular velocity (rad/s)
y_des = [theta_des; omega_des];

% ODE function with LQR control towards desired state
pendulumODE = @(t, y) (A - B*K)*(y - y_des);

% Initial conditions
initialConditions = [theta0; omega0];

% Solve the ODE
[t, y] = ode45(pendulumODE, tspan, initialConditions);

% Create a figure for the animation
figure;
axis equal; % Set equal scaling for x and y axes
xlim([-1.5 1.5]);
ylim([-1.5 1.5]);
hold on;

% Create a line object for the pendulum
pendulumLine = line([0, L*sin(y(1,1))], [0, -L*cos(y(1,1))], 'LineWidth', 2, 'Color', 'b');
bob = plot(L*sin(y(1,1)), -L*cos(y(1,1)), 'ro', 'MarkerSize', 10); % Bob of the pendulum

% Animation loop
for i = 1:length(t)
    % Update pendulum position
    pendulumLine.XData = [0, L*sin(y(i, 1))];
    pendulumLine.YData = [0, -L*cos(y(i, 1))];
    bob.XData = L*sin(y(i, 1));
    bob.YData = -L*cos(y(i, 1));
    
    % Pause to create animation effect
    pause(0.05); % Adjust the pause duration for speed of animation
end

hold off;
title('Damped Simple Pendulum Simulation with LQR Control');
xlabel('X Position (m)');
ylabel('Y Position (m)');
grid on;