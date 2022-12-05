# Penning Trap Project

It's a project for MIPT Computer Science classes.  

This project will be a numeric model of Penning trap with one particle in space (in future - more than one) and external EM fields.

The goal of this project is to perform our skills in Object-Oriented Programming and create sandbox with electromagnetic interactions between at least point particles in external field. In plans, making interactions between particles.

Electrical quadratic potential was defined as

$$\Phi = C \left(z^2 - \dfrac{x^2 + y^2}{2} - \varepsilon \dfrac{x^2 - y^2}{2} \right),$$

where $C$ is proportionality factor, $\varepsilon$ - ellipticity parameter.
Therefore, electrical and magntic fields are

$$\overline{E} = - \nabla \Phi = C
    \left( \begin{array}{cc}
        (1 + \varepsilon)x \\
        (1 - \varepsilon)y \\
        - 2 z
    \end{array}\right),
~~
\overline{B} = B_0 \left(  \begin{array}{cc} 
        \sin(\theta) \cos(\phi) \\ 
        \sin(\theta) \sin(\phi) \\ 
        \cos(\phi)
    \end{array}\right)$$