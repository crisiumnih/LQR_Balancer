spring = Spring;
y_initial=[4;0];
y_final=[0;0];

spring = spring.setPara(0.5,5,y_initial,y_final);
[t,y]=spring.trylqr();
%[t,y]=spring.ideal_spring();

for k=1:length(t)
    spring.drawSpring(y(k,:))
end
