pendulum = Pendulumc;

t_initial=[0 ;0]; % initial state position and angular vel
t_final=[-pi/2;0]; % final state position and angular vel

pendulum = pendulum.setPara(1,1,t_initial,t_final);
pendulum.m
pendulum.L
[time,y]=pendulum.trylqr();
for k = 1:length(time) 
    %draw_pendulum(y(k, :), L);
    pendulum.drawPendulum(y(k, :));
 end
%pendulum.drawPendulum;