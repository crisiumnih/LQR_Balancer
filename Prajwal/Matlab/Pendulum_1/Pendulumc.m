classdef Pendulumc
    properties
        m % Mass
        L % Length
        g=9.81
        theta_final
        theta_initial
        dt
    end
    methods

        function obj=setPara(obj,Length,mass,theta_i,theta_f)
            obj.m=mass;
            obj.L=Length;
            obj.theta_initial=theta_i;
            obj.theta_final=theta_f;
            
        end
        function drawPendulum(obj,theta)
            x_pos = obj.L*sin(theta(1));
            y_pos = 1-obj.L*cos(theta(1));
            d= 0.1;
            hold on;
            clf;
            axis equal;
            rectangle('Position',[x_pos-(d/2),y_pos-(d/2),d,d],'Curvature',1,'FaceColor','k');
            line ([0 x_pos], [1 y_pos], "linestyle", "-", "color", "k");
            xlim([-2 2])
            ylim([-0.5 2])
            drawnow
            hold off
        end

        function dy = setPendulum(obj,y,u)
            dy(1,1)=y(2);
            dy(2,1)=-obj.g*sin(y(1))/obj.L +u/(obj.m*obj.L^2);

            
        end

        function [A,B]=ABPendulum(obj)
            A=[0 1; obj.g/obj.L 0];
            B = [0; 1/(obj.m*obj.L^2)];
        end

        function [time,y]=trylqr(obj)
            [A,B]=obj.ABPendulum();
            Q=[5000 0; 0 0.5];
            R=0.0001;
            K = lqr(A,B,Q,R); 
            steps = 0:0.01:10;
            y0 = obj.theta_initial;
            odeFunc = @(t, y) obj.setPendulum(y, -K * (y - obj.theta_final));
            [time, y] = ode45(odeFunc, steps, y0);
        end

    end
end