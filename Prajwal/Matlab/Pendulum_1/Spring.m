classdef Spring
    properties
        m % Mass
        k % Length
        
        y_final
        y_initial
        
    end
    methods

        function obj=setPara(obj,mass,const,y_i,y_f)
            obj.m=mass;
            obj.k=const;
            obj.y_initial=y_i;
            obj.y_final=y_f;
            
        end
        function drawSpring(obj,y)
            x_pos = y(1);
            y_pos = 0;
            hold on;
            clf;
            axis equal;
            rectangle('Position',[x_pos-(1/2),y_pos,1,1],'Curvature',0,'FaceColor','k');
            line ([-5 x_pos], [(y_pos+1)/2 (y_pos+1)/2], "linestyle", "-", "color", "k");
            xlim([-5 5])
            ylim([0 5])
            drawnow
            hold off
        end

        function dy = setSpring(obj,y,u)
            dy(1,1)=y(2);
            dy(2,1)=-obj.k*y(1)/obj.m + u/obj.m;
        end

        function [A,B]=ABSpring(obj)
            A=[0 1; -obj.k/obj.m 0];
            B = [0; 1/obj.m];
        end

        function [t,y] = ideal_spring(obj)
            time = 0:0.1:10;
            u=0;
            [t,y] = ode45(@(t,y)obj.setSpring(y, u),time,obj.y_initial);
        end

        function [t,y]=trylqr(obj)
            [A,B]=obj.ABSpring();
            Q=[1 0; 0 0.1];
            R=1;
            K = lqr(A,B,Q,R); 
            steps = 0:0.01:50;
            y0 = obj.y_initial;
            odeFunc = @(t, y) obj.setSpring(y, -K * (y - obj.y_final));
            [t, y] = ode45(odeFunc, steps, y0);
        end

    end
end