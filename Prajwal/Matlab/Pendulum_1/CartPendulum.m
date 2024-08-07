classdef CartPendulum
    properties
        m_cart % Mass
        m_blob
        L % Length
        g=9.81;
        y_final
        y_initial
        dy_sys
        b=0.000078;
        c=0.63;
        I=0;

        
    end
    methods

        function obj=setPara(obj,mass_c,mass_b,length,y_i,y_f)
            obj.m_cart=mass_c;
            obj.m_blob=mass_b;
            obj.L=length;
            obj.y_initial=y_i;
            obj.y_final=y_f;
            
        end
        function drawSystem(obj,y)
            x_pos_cart = y(1);
            y_pos_cart = 0;
            x_pos_blob = x_pos_cart+obj.L*sin(y(2));
            y_pos_blob = obj.L*cos(y(2));
            d=0.1;
            hold on;
            clf;
            axis equal;
            rectangle('Position',[x_pos_cart,y_pos_cart,0.25,0.25],'Curvature',0,'FaceColor','k');
            rectangle('Position',[x_pos_blob-(d/2),y_pos_blob-(d/2),d,d],'Curvature',1,'FaceColor','k');
            line ([x_pos_cart x_pos_blob], [y_pos_cart y_pos_blob], "linestyle", "-", "color", "k");
            xlim([-10 10])
            ylim([-2 2])
            drawnow
            hold off
            % Extract position and angle from the state vector
    % x_pos_cart = y(1);          % Cart position (x coordinate)
    % theta = y(2);               % Pendulum angle (radians)
    % 
    % % Calculate position of the pendulum bob
    % x_pos_blob = x_pos_cart + obj.L * sin(theta);
    % y_pos_blob = obj.L * cos(theta);
    % 
    % % Dimensions
    % cart_width = 0.25;          % Cart width
    % cart_height = 0.25;         % Cart height
    % bob_diameter = 0.1;         % Diameter of pendulum bob
    % 
    % % Clear current figure and set up axis properties
    % clf;
    % hold on;
    % axis equal;
    % xlim([-10 10]);
    % ylim([-2 2]);
    % 
    % % Draw cart
    % rectangle('Position', [x_pos_cart - cart_width/2, -cart_height/2, cart_width, cart_height], ...
    %           'Curvature', 0, 'FaceColor', 'k');
    % 
    % % Draw pendulum bob
    % rectangle('Position', [x_pos_blob - bob_diameter/2, y_pos_blob - bob_diameter/2, bob_diameter, bob_diameter], ...
    %           'Curvature', 1, 'FaceColor', 'k');
    % 
    % % Draw rod connecting cart and pendulum bob
    % line([x_pos_cart, x_pos_blob], [0, y_pos_blob], 'Color', 'k', 'LineWidth', 2);
    % 
    % % Set plot properties
    % xlabel('X');
    % ylabel('Y');
    % title('Cart-Pendulum System');
    % drawnow;
    % 
    % % Release hold
    % hold off;


    
        end

        function dy = setSystem(obj,y,u)
            
            dy(1,1)=y(3);
            dy(2,1)=y(4);
            theta=y(2);
            dy(3,1)=(-obj.m_blob*obj.g*theta)/obj.m_cart + u/obj.m_cart;
            dy(4,1)=(obj.m_cart+obj.m_blob)*obj.g*theta/(obj.m_cart*obj.L) - u/(obj.m_cart*obj.L);
        end

        % function dy = setSystem2(obj,y,u)
        % 
        %     dy(1,1)=y(3); dy(2,1)=y(4); theta=y(2);
        %     dy(3,1)=((obj.m_cart+obj.m_blob)*obj.g*sin(theta)-obj.m_blob*obj.L*(dy(2,1))^2*sin(theta)*cos(theta)-u*cos(theta))/(obj.L(obj.m_blob+obj.m_cart)-obj.m_blob*obj.L*(cos(theta))^2);
        %     dy(4,1)=(u+obj.m_blob*(y(4))^2*sin(theta)-obj.m_blob*obj.L*dy(3,1)*cos(theta))/(obj.m_blob+obj.m_cart);
        %     obj.dy_sys(1,1)=dy(1,1); obj.dy_sys(2,1)=dy(2,1);
        %     obj.dy_sys(3,1)=dy(3,1); obj.dy_sys(4,1)=dy(4,1);
        % end

        function dy = setSystem2Sym(obj, y, u)
            
            dy(1,1) = y(3);
            dy(2,1) = y(4);
            theta = y(2);
            dy(3,1) = ((obj.b * obj.m_blob * y(4) * cos(theta))+ (obj.m_blob^2 * obj.L^2 * obj.g * sin(theta)*cos(theta)) + (obj.I +obj.m_blob*obj.L^2)*(u-obj.c*y(3)+obj.m_blob*obj.L*(y(4))^2*sin(theta)))/(obj.m_blob^2 * obj.L^2 * (sin(theta))^2+obj.m_cart*obj.m_blob*obj.L^2+(obj.m_cart+obj.m_blob)*obj.I);
            dy(4,1) = (-(u*obj.m_blob*obj.L*cos(theta)-obj.c*obj.m_blob*obj.L*y(3)*cos(theta)+obj.m_blob^2 * obj.L^2*(y(4))^2*sin(theta)*cos(theta)+(obj.m_cart+obj.m_blob)*(obj.b*y(4) + obj.m_blob*obj.g*obj.L*sin(theta)) ))/(obj.m_blob^2 * obj.L^2 * (sin(theta))^2+obj.m_cart*obj.m_blob*obj.L^2+(obj.m_cart+obj.m_blob)*obj.I);
            
        end


        function [A,B]=ABSystem(obj)
            A=[0 0 1 0; 
               0 0 0 1; 
               0 (-obj.m_blob*obj.g)/obj.m_cart 0 0; 
               0 (obj.m_cart+obj.m_blob)*obj.g/(obj.m_cart*obj.L) 0 0];
            B = [0; 0;1/obj.m_cart; 1/(obj.m_cart*obj.L)];
        end

        function [A,B]=ABSystem2(obj)
            syms x theta dx dtheta u
            y = [x; theta; dx; dtheta];
        
            dy = obj.setSystem2Sym(y, u);
            A_sym = jacobian(dy, y);
            B_sym = jacobian(dy, u);
           
            % Convert symbolic Jacobians to numeric
            A = double(subs(A_sym, {x, theta, dx, dtheta, u}, {0, 0, 0, 0, 0}));
            B = double(subs(B_sym, {x, theta, dx, dtheta, u}, {0, 0, 0, 0, 0}));
            
        end

        function printAB(obj)
            [A, B] = obj.ABSystem2();
            disp('Matrix A:');
            disp(A);
            disp('Poles:');
            disp(eig(A));
            disp('Matrix B:');
            disp(B);
        end

        function [t,y]=ideal_system(obj)
            steps = 0:0.01:50;
            u=0;
            y0 = obj.y_initial;
            odeFunc = @(t, y)obj.setSystem2Sym(y, u);

            [t,y]=ode45(odeFunc,steps,y0);
        end

        

        function [t,y]=trylqr(obj)
            [A,B]=obj.ABSystem2();
            
            Q = [500 0 0 0;
                0 1000 0 0;
                0 0 0 0;
                0 0 0 0];
            R = 0.008;
            K = lqr(A,B,Q,R); 
            steps = 0:0.01:50;
            y0 = obj.y_initial;
            odeFunc = @(t, y)obj.setSystem2Sym(y, -K*(y - obj.y_final));
            [t, y] = ode45(odeFunc, steps, y0);
        end

    end
end
