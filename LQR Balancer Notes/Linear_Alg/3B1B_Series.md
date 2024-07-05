- In 2D, basis from 2 non-parallel vectors spans entire $xy$ plane 
- If they are parallel they span along the straight line
- Similarly in 3D if 3rd vector is in the plane formed by other 2 vectors they span in the respective plane
- Otherwise if all 3 vectors are non-parallel they span the 3D space
- The redundant vector is described as linearly dependent vector
- One of the vectors can be expressed as linear combination of others since its already in the span
***
- Linear transformation -> origin stays in place and grid lines parallel and straight
	- $\begin{bmatrix}a & b\\c & d\end{bmatrix}$ where $\begin{bmatrix}a \\c \end{bmatrix}$ is where $\hat{i}$ lands and $\begin{bmatrix}b \\d \end{bmatrix}$ where  $\hat{j}$ lands
- The **Factor** by which a linear transformation changes any area is called determinant of transformation.
	- If 0-> squishes onto a line
	- negative -> invert the orientation 
	- 3D -> volume
***
- Rank -> dimensions in output/column space (basis vector span the respective dimensions)
- Set of all possible output -> Column space (columns give where basis vectors land and their span gives space)
- $\begin{bmatrix}0 \\0 \end{bmatrix}$ always in the column space (the set of vectors which land on origin is kernel or null space)
- In linear equations $A\overrightarrow{x}=\begin{bmatrix}0 \\0 \end{bmatrix}$  gives all possible solution to equation
***
- $A \begin{bmatrix}x_j \\y_j \end{bmatrix}=\begin{bmatrix}x_o \\y_o \end{bmatrix}$  A is new coordinate system basis vectors, such equation translates new vectors in new system to traditional system 
	- $\begin{bmatrix}x_j \\y_j \end{bmatrix}= A^{-1}\begin{bmatrix}x_o \\y_o \end{bmatrix}$ does the opposite
	- $A^{-1}MA$ is transformation in another coordinate system where M is transformation in traditional way and A is basis of new system
***
- Eigenvector -> vectors which maintain their span after linear transformation and the factor by which they change are eigenvalues
	- Suppose a 3D rotation, if there is a eigenvector it is its axis of rotation.
	- $A\overrightarrow{v}=\lambda\overrightarrow{v}$  what this means transformation of v is same as scaling it by its eigenvalue.
	- Diagonal matrix directly give eigenvalues
	- Trace = Sum of eigenvalues, determinant = product of eigen values
	- eigenvalues = $m\pm\sqrt{m^2-p}$     m->mean of trace p-> product 
- Linear Transformation preserve addition and scalar multiplication
- $a_nx^n+a_{n-1}x^{n-1}+\cdots a_1x+a_0=\begin{bmatrix}a_0 \\a_1 \\ \vdots \\ a_{n} \\0\\ \vdots\end{bmatrix}$ ,derivative matrix is just infinite matrix of coefficient of derivative of basis functions