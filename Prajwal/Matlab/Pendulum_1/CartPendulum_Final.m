system = CartPendulum;
y_initial = [1; 0; 0; 0];
y_final = [1; pi; 0; 0];
system = system.setPara(0.135,0.1,0.5,y_initial,y_final);
[t,y]=system.trylqr();
system.printAB();
%[t,y]=system.ideal_system();
for k=1:length(t)
    system.drawSystem(y(k,:))
end
