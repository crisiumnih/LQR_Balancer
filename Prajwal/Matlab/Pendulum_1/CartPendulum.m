classdef CartPendulum
    properties
        m_cart % Mass
        m_blob
        L % Length
        g=9.81;
        y_final
        y_initial
        
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
            x_pos_blob = x_pos_cart-obj.L*sin(y(2));
            y_pos_blob = obj.L*cos(y(2));
            d=0.1;
            hold on;
            clf;
            axis equal;
            rectangle('Position',[x_pos_cart-(1/2),y_pos_cart,0.5,0.5],'Curvature',0,'FaceColor','k');
            rectangle('Position',[x_pos_blob-(d/2),y_pos_blob-(d/2),d,d],'Curvature',1,'FaceColor','k');
            line ([x_pos_cart x_pos_blob], [y_pos_cart y_pos_blob], "linestyle", "-", "color", "k");
            xlim([-10 10])
            ylim([-5 5])
            drawnow
            hold off
        end

        function dy = setSystem(obj,y,u)
            
            dy(1,1)=y(3);
            dy(2,1)=y(4);
            theta=y(2);
            dy(3,1)=(-obj.m_blob*obj.g*theta)/obj.m_cart + u/obj.m_cart;
            dy(4,1)=(obj.m_cart+obj.m_blob)*obj.g*theta/(obj.m_cart*obj.L) - u/(obj.m_cart*obj.L);
        end

        function [A,B]=ABSystem(obj)
            A=[0 0 1 0; 
               0 0 0 1; 
               0 (-obj.m_blob*obj.g)/obj.m_cart 0 0; 
               0 (obj.m_cart+obj.m_blob)*obj.g/(obj.m_cart*obj.L) 0 0];
            B = [0; 0;1/obj.m_cart; 1/(obj.m_cart*obj.L)];
        end

        

        function [t,y]=trylqr(obj)
            [A,B]=obj.ABSystem();
            Q = [1 0 0 0;0 1 0 0;0 0 1 0;0 0 0 1];
            R = 1;
            K = lqr(A,B,Q,R); 
            steps = 0:0.01:50;
            y0 = obj.y_initial;
            odeFunc = @(t, y)obj.setSystem(y, -K*(y - obj.y_final));
            [t, y] = ode45(odeFunc, steps, y0);
        end

    end
end
