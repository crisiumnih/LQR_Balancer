system = CartPendulum;
y_initial = [1; pi/6; 0; 0];
y_final = [-2; 0; 0; 0];
system = system.setPara(2,0.5,1,y_initial,y_final);
[t,y]=system.trylqr();
system.printAB();
%[t,y]=system.ideal_system();
for k=1:length(t)
    system.drawSystem(y(k,:))
end
