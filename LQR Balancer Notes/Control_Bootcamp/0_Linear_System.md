- Passive Control and active control
- ![[Pasted image 20240629012731.png|300]]
- Why feedback? 
	- Uncertainty
	- Instability
	- Disturbances
	- Efficient

***


>[!State Equation for control]
$$\dot{x}=Ax+Bu$$ state equation -> $x$ 
> actuator -> $u$
> $B$ is how the actuator affects the system

solution of simple equation is $x(t)=e^{At}x(0)$
$e^{At}=I+At+\frac{A^2t^2}{2!}+\frac{A^3t^3}{3!}+\cdots$ not practical
we use eigenvalues and eigenvectors 

***

Suppose T = $\begin{bmatrix}v_1 & v_2 & v_3 & \cdots & v_n \end{bmatrix}$  matrix of eigenvectors and D = $\begin{bmatrix} \lambda_{1} & 0 & 0 \\ 0 & \ddots & 0 \\ 0 & 0 & \lambda_{n} \end{bmatrix}$ 
Then $AT=TD$ 
converting to new system for simplification 
$x=Tz$    $z$ is in eigenvector directions given by columns of $T$

>[!Converting to different system]
$$\dot{x}=T\dot{z}=Ax$$
$$\text{Hence, }T^{-1}AT=D$$
$$T\dot{z}=ATz$$
$$\dot{z}=T^{-1}ATz$$
$$\boxed{\dot{z}=Dz}$$

>[!Simplifying]
>$$
\displaylines{ e^{At}=I+At+\frac{A^2t^2}{2!}+\frac{A^3t^3}{3!}+\cdots \\ A=TDT^{-1} \text{ using this to simplify above} \\ \\ e^{TDT^{-1}t} \\ = TT^{-1} + TDT^{-1} + \frac{(TDT^{-1}TDT^{-1})t^2}{2!}+\cdots \\ =\cdots + \frac{TD^2T^{-1}t^2}{2}+\cdots \\ \\ \text{Hence, } A^n=TD^nT^{-1} \\ \text{Series becomes, } \\ = T[I + Dt + \frac{D^2t^2}{2!}+\cdots]T^{-1} \\ \\ \boxed{ e^{At}=Te^{Dt}T^{-1} } \text{ which is easier to calculate}
}
$$

$$\boxed{ x(t)=Te^{Dt}T^{-1}x(0) }$$


