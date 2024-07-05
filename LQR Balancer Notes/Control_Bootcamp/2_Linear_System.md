## Pendulum

![[1582557358.png|250]]

$$\ddot{\theta}=-\frac{g}{L}sin(\theta) -\delta\dot{\theta} \qquad \text{Non linear function, }\delta\dot{\theta} \text{ so that system stabalizes considering friction}$$

$$\displaylines{\text{For simplification assume that g/L = 1} \\\begin{bmatrix}x_1 \\x_2 \end{bmatrix} = \begin{bmatrix}\theta \\\dot{\theta} \end{bmatrix} \\ \frac{d}{dt}\begin{bmatrix}x_1 \\x_2 \end{bmatrix} = \begin{bmatrix}x_2 \\-sin(x_1)-\delta x_2 \end{bmatrix} \\ \\ \\ \text{Step 1, Fixed points}\Rightarrow \\ \bar{x}=\begin{bmatrix}0 \\0 \end{bmatrix},\begin{bmatrix}\pi \\0 \end{bmatrix}  \\ \text{2 States pendulum down and up.} \\ \\ \text{Step 2 Linearize, } \\ \frac{Df}{Dx} = \begin{bmatrix}0 & 1 \\ -cos(x_1) & -\delta \end{bmatrix} \\ A_d = \begin{bmatrix}0 &1 \\-1 & -\delta \end{bmatrix} \quad \text{When down,} \\ A_u=\begin{bmatrix}0 &1 \\1 & -\delta \end{bmatrix}\\ \\ \text{Now have to find eigenvalues of both these matrices,} \\ \text{ in case of down Eigenvalues are }\pm i \text{ which means system will stabalize}\\ \text{For up position eigenvalues are }\pm1 \text{which means its a saddle point.}}$$

***
## Controllability 

Adding control to manipulate system
$$\dot{x}=Ax+Bu \qquad x\in \mathbb{R}^{n}, u\in \mathbb{R}^{q}, A\in \mathbb{R}^{n \times m},B\in \mathbb{R}^{n \times q}$$
$$y=Cx \quad \text{measurements}$$
A basic control law is $u=-Kx$ 
$$\displaylines{\dot{x} = Ax-BKx \\ \dot{x}=(A-BK)x}$$Most of the times, A and B are fixed and you only get to control u

Test of Controllability,
$$\mathcal{C} = \begin{bmatrix} B & AB & A^2B & \cdots A^{n-1}B\end{bmatrix}, \quad rank(\mathcal{C}) \iff n, \text{ Then sysetm is controllable}$$   matlab command -> ctrb(A,B)


- If the system is controllable you can place the eigenvalue of close loop system anywhere to make it stable. You can pick any pole/eigenvalue as there exists a matrix of gain K so that system will have specified eigenvalue (matlab comm -> K = place(A,B,eigs)) 
- Reachable set $R_t = \left\{\xi \in \mathbb{R}^n \mid \text{there is an input u(t) so that } x(t)=\xi\right\}$ set which i can drive to with control

***
## Discrete time impulse

Discrete time Impulse response,

$$x_{k+1}=Ax_k+Bu_k$$
Suppose,
$$\displaylines{u_0=1 \qquad x_0=0\\ u_1=0 \qquad x_1=B \\u_2=0 \qquad x_2=AB \\u_3=0 \qquad x_3=A^2B \\ \vdots \\ u_m=0 \qquad x_m=A^mB}$$
Impulse in u, some states in $\mathbb{R}^n$ are not in space to control. If they are in $\mathbb{R}^n$  they can be controlled. 