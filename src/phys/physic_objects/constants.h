#ifndef PHYSICS_CONSTANTS_H
#define PHYSICS_CONSTANTS_H

////////////////////////////////////////
//       Fundamental Constants        //
////////////////////////////////////////
#ifndef SPEED_OF_LIGHT
#define SPEED_OF_LIGHT 1
#endif // SPEED_OF_LIGHT

#ifndef ELEMENTARY_CHARGE
#define ELEMENTARY_CHARGE 1
#endif // ELEMENTARY_CHARGE

#ifndef ELEMENTARY_MASS
#define ELEMENTARY_MASS  1
#endif // ELEMENTARY_MASS


////////////////////////////////////////
//             Electron               //
////////////////////////////////////////
#define ELECTRON_CHARGE -ELEMENTARY_CHARGE
#define ELECTRON_MASS ELEMENTARY_MASS


////////////////////////////////////////
//              Proton                //
////////////////////////////////////////
#define PROTON_CHARGE ELEMENTARY_CHARGE
#define PROTON_MASS 1836 * ELEMENTARY_MASS

#endif // PHYSICS_CONSTANTS_H