% Parameters of the inverted pendulum system
M = 1.0;  % Mass of the cart (kg)
m = 0.1;  % Mass of the pendulum (kg)
l = 0.5;  % Length of the pendulum (m)
g = 9.81; % Acceleration due to gravity (m/s^2)

% Linearized state-space matrices around the upright position
A = [0, 1, 0, 0;
     0, 0, -m*g/M, 0;
     0, 0, 0, 1;
     0, 0, (M+m)*g/(M*l), 0];

B = [0;
     1/M;
     0;
     -1/(M*l)];

% Define the Q and R matrices for LQR
Q = diag([1000, 1, 1000, 1]); % State error weightings (customize as needed)
R = 0.0001; % Control effort weighting

% Compute the LQR controller gain matrix K
K = lqr(A, B, Q, R);

% Initial state [x; theta; dx; dtheta]
x0 = [0; 0; pi + 0.1; 0]; % Initial condition with pendulum slightly off the inverted position

% Desired end state
target_position = 0; % meters (cart position)
target_angle = pi; % radians (pendulum upright position)

% Define a function for the system dynamics with LQR control
function dxdt = pendulum_dynamics(t, x, A, B, K, target_position, target_angle)
    % Tracking control: Reference position and angle
    ref = [target_position; 0; target_angle; 0]; % Desired state [x; theta; dx; dtheta]
    u = -K * (x - ref); % LQR control law
    dxdt = A * x + B * u; % State derivative
end

% Simulation time
t = 0:0.01:10; % Time vector from 0 to 10 seconds with 0.01s step

% Use ODE45 to simulate the system
[T, X] = ode45(@(t, x) pendulum_dynamics(t, x, A, B, K, target_position, target_angle), t, x0);

% Animation parameters
cart_width = 0.4; % Width of the cart
cart_height = 0.2; % Height of the cart
bob_radius = 0.05; % Radius of the pendulum bob

% Create figure for animation
figure;
axis equal;
xlim([-2 2]);
ylim([-1 1.5]);
hold on;
grid on;
title('Inverted Pendulum on a Cart');

% Initialize plot objects
cart = rectangle('Position', [X(1,1)-cart_width/2, -cart_height/2, cart_width, cart_height], ...
                 'Curvature', 0.1, 'FaceColor', [0 0.5 1]);
rod = line([X(1,1), X(1,1) + l*sin(X(1,2))], [0, -l*cos(X(1,2))], ...
           'LineWidth', 2, 'Color', 'k');
bob = rectangle('Position', [X(1,1) + l*sin(X(1,2)) - bob_radius, -l*cos(X(1,2)) - bob_radius, ...
                 2*bob_radius, 2*bob_radius], 'Curvature', [1,1], 'FaceColor', 'r');

% Animation loop
for k = 1:length(T)
    % Update cart position
    cart.Position = [X(k,1)-cart_width/2, -cart_height/2, cart_width, cart_height];
    
    % Update rod position
    set(rod, 'XData', [X(k,1), X(k,1) + l*sin(X(k,2))], 'YData', [0, -l*cos(X(k,2))]);
    
    % Update bob position
    bob.Position = [X(k,1) + l*sin(X(k,2)) - bob_radius, -l*cos(X(k,2)) - bob_radius, ...
                    2*bob_radius, 2*bob_radius];
    
    % Adjust x-axis limits based on cart position
    xlim([-2 + X(k,1), 2 + X(k,1)]);
    
    % Pause to control the animation speed
    pause(0.01);
end
