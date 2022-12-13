# Penning Trap Project

It's a project for MIPT Computer Science classes.  

This project will be a numeric model of Penning trap with one particle in space (in future - more than one) and external EM fields.

The goal of this project is to perform our skills in Object-Oriented Programming and create sandbox with electromagnetic interactions between at least point particles in external field. In plans, making interactions between particles.

Electrical quadratic potential constists of 2 parts:

$$\Phi = \Phi^{\text{stat}} + \Phi^{\text{rot}},$$

$$\Phi^{\text{stat}} = \dfrac{m}{q} \dfrac{\omega^2_z}{2} \left(z^2 - \dfrac{x^2 + y^2}{2} - \varepsilon \dfrac{x^2 - y^2}{2} \right),$$

$$\Phi^{\text{rot}} = \dfrac{m}{q} a \cdot z \cdot r \cdot \cos(\theta + \omega_r t)$$

where $\omega_z$ is frequency of axial fluctiations, $\varepsilon$ - ellipticity parameter,
$a$ - amplitude of rotating field, $\omega_r$ - frequency of rotating field.  
Therefore, electrical and magntic fields are

$$\overline{E} = - \nabla \Phi = \dfrac{m}{q} \dfrac{\omega^2_z}{2}
    \left( \begin{array}{cc}
        (1 + \varepsilon)x \\
        (1 - \varepsilon)y \\
        - 2 z
    \end{array} \right) + \dfrac{m}{q} a \cdot z \cdot r
    \left( \begin{array}{cc}
        -z \cos(\omega_r t) \\
        z \cos(\omega_r t) \\
        x \cos(\omega_r t) - y \sin(\omega_r t)
    \end{array} \right)$$

$$\overline{B} = \text{const}$$