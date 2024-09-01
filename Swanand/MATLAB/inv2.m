% Parameters of the system
m1 = 0.3;  % mass of the pendulum (kg)
m2 = 0.03; % mass of the wheel (kg)
I1 = 7;    % moment of inertia of the pendulum (kg*m^2)
I2 = 1.5;  % moment of inertia of the wheel (kg*m^2)
L = 1.0;   % length of the pendulum (m)
r = 0.06;  % radius of the wheel (m)
h = 0.07;  % distance from pivot to center of mass (m)
g = 9.81;  % acceleration due to gravity (m/s^2)

% Coefficients calculated based on the new model
a = g * h * m1 / (I1 + I2 + h^2 * m1 + 2 * h * m1 * r + m1 * r^2 + m2 * r^2);
b = (-I2 - h * m1 * r - m1 * r^2 - m2 * r^2) / (I1 + I2 + h^2 * m1 + 2 * h * m1 * r + m1 * r^2 + m2 * r^2);

% State-space representation
% x = [theta; theta_dot; phi_dot]
A = [0 1 0; 
     a 0 0; 
     0 0 0];
B = [0; b; 1];
C = eye(3); % Identity matrix to observe all states
D = zeros(3, 1);

% Desired pole locations for LQR
Q = [0.5 0 0;
     0 5 0;
     0 0 0.1];
R = 0.03;

% LQR gain
K = lqr(A, B, Q, R);

% Simulation parameters
dt = 0.01; % time step
T = 100; % total time
t = 0:dt:T; % time vector

% Initial conditions
x0 = [pi/4; 0; 0]; % initial state (theta, theta_dot, phi_dot)
x = x0;
cart_position = 0; % initial position of the cart

% State history
x_hist = zeros(length(t), length(x0));
u_hist = zeros(length(t), 1);
cart_position_hist = zeros(length(t), 1);

% Simulation loop
for i = 1:length(t)
    u = -K * x; % control input (angular acceleration of the wheel)
    x_dot = A * x + B * u; % state derivatives
    x = x + x_dot * dt; % update state
    cart_position = cart_position + r * (x(2) + x(3)) * dt; % update cart position
    x_hist(i, :) = x';
    u_hist(i) = u;
    cart_position_hist(i) = cart_position;
end

% Plot results
figure;
subplot(3,1,1);
plot(t, x_hist(:, 1) * 57.3, 'LineWidth', 1.5); % Convert theta to degrees
title('Pendulum Angle (\theta)');
xlabel('Time (s)');
ylabel('Angle (deg)');
grid on;

subplot(3,1,2);
plot(t, x_hist(:, 2) * 57.3, 'LineWidth', 1.5); % Convert theta_dot to deg/s
title('Angular Velocity (\dot{\theta})');
xlabel('Time (s)');
ylabel('Angular Velocity (deg/s)');
grid on;

subplot(3,1,3);
plot(t, u_hist, 'LineWidth', 1.5);
title('Control Input');
xlabel('Time (s)');
ylabel('Angular Acceleration (rad/s^2)');
grid on;

% Visualization with cart
figure;
for i = 1:20:length(t)
    clf;
    hold on;
    % Draw the cart
    rectangle('Position', [cart_position_hist(i)-0.2, -0.05, 0.4, 0.1], 'Curvature', [0, 0], 'FaceColor', [0 0.5 0.5]);
    % Draw the pendulum
    plot([cart_position_hist(i), cart_position_hist(i) + L*sin(x_hist(i, 1))], [0, L*cos(x_hist(i, 1))], 'k', 'LineWidth', 2);
    % Draw the bob
    rectangle('Position', [cart_position_hist(i) + L*sin(x_hist(i, 1))-0.05, L*cos(x_hist(i, 1))-0.05, 0.1, 0.1], 'Curvature', [1, 1], 'FaceColor', [1 0 0]);
    axis([-2 2 -1.5 1.5]);
    title('Inverted Pendulum on Cart');
    xlabel('X Position (m)');
    ylabel('Y Position (m)');
    grid on;
    drawnow;
end
