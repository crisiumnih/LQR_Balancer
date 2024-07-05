### Continuous time

In continuous time,

$$ D = \begin{bmatrix} \lambda_{1} & 0 & 0 \\ 0 & \ddots & 0 \\ 0 & 0 & \lambda_{n} \end{bmatrix}  \qquad \quad e^{Dt} =\begin{bmatrix} e^{\lambda_{1}t} & 0 & 0 \\ 0 & \ddots & 0 \\ 0 & 0 & e^{\lambda_{n}t} \end{bmatrix}$$
$$ \text{each } \lambda \text{ will have a real and imaginary part} $$

$$\displaylines{\lambda = a+\iota b  \\ e^{\lambda t} = e^{at}[cos(bt)+isin(bt)] \\ \text{Notice how magnitude of trig terms is 1 hence ecp term decides stability} \\ \text{if } a>0,} $$

```desmos-graph

left=-2; right=2;
top=3; bottom=-1;
---
y=e^x
```


$$\text{if } a<0,$$
```desmos-graph

left=-2; right=2;
top=3; bottom=-1;
---
y=e^{-x}
```

Now the system is stable if all the real part of all eigenvalues are negative as equation approaches zero at infinity

Now if eigenvalues positive we need an extra part which drive the system from disorder to stability 

***
### Discrete time

In Discrete time,

$$x_{k+1}=\tilde{A}x_k \qquad ,x_k=x(k\Delta t) $$
$$\tilde{A} = e^{A\Delta t}$$
$$x_n=\tilde{A}^{n}x_o$$radius of eigenvalues wither grows or decays,
any $\lambda$ can be written as $Re^{i\theta}$ 
and we take $\lambda^{n}$ radius either increases or decreases
Hence, if R>1 it goes to infinity and not stable
while R<1 it shrinks and becomes stable

***
Trying to get a linear system from nonlinear dynamic system
$$\dot{x}=f(x) \quad \Rightarrow \quad \dot{x}=Ax$$
1. find fixed points
	1. $\bar{x} \ni f(\bar{x})=0$ 
2. Linearize about $\bar{x}$
	1. $\large{\left.\frac{Df}{Dx}\right|_{\bar{x}}=[\frac{\partial f_i}{\partial x_j}]}$   it is jacobian matrix of the dynamic

