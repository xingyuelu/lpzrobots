/***************************************************************************
 *   Copyright (C) 2005 by Robot Group Leipzig                             *
 *    martius@informatik.uni-leipzig.de                                    *
 *    fhesse@informatik.uni-leipzig.de                                     *
 *    der@informatik.uni-leipzig.de                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************
 *                                                                         *
 *  DESCRIPTION                                                            *
 *                                                                         *
 *   $Log$
 *   Revision 1.1  2007-03-16 10:50:32  martius
 *   initial version
 *
 *
 *   

This file concerns material implementation in the simulator.

Terminology:
Since "Material" is used for OpenGL stuff and also in OSG 
 we should use something else for the physical material: substance

So how to implement substance to cover all collission cases and requirements.
It would be nice to keep the collisioncontrol as it is for compatibility reasons.
In general:

- every ode geom should get a user data pointer to the primitive 
- primitive has a certain substance 
- 2 substance define collision parameters
- also an optional callback function for user defined handling
- collission detection is done globally. 
- via joint connected geoms are stored in a set (globally)
- spaces are globally registered



 ***************************************************************************/
#ifndef __SUBSTANCE_H
#define __SUBSTANCE_H

#include<ode/common.h>
#include<ode/contact.h> 

namespace lpzrobots {

  class GlobalData;
  class Substance;

  /** function to be called at a collision event between the two geoms. 
      @param params surface parameter, which should be changed by this function
      @param globaldata global information
      @param userdata pointer to user data for this callback (stored in substance)
      @param o1 geom corresponding to substance of this callback
      @param o2 other geom 
      @param s1 substance of this callback
      @param s2 other substance 
      @return 0 if collision should not be treated;
              1 if collision should be treated otherwise (by other callback or standart methods);
              2 if collision to be treated and parameters for collision are set in params
   */
  typedef int (*CollisionCallback)(dSurfaceParameters& params, GlobalData& globaldata, void *userdata, 
				    dGeomID o1, dGeomID o2, const Substance* s1, const Substance* s2);

  /**
     Physical substance definition, used for collision detection/treatment
     What we need is mu, slip and kp,kd parameter for the collision
     Definition of substance parameters:
     <pre>
       Parameter    interval  collision_parameter
       roughness:  [0-]      mu   = roughness1*roughness2 
       slip:       [0-]      slip = slip1+slip2
       hardness:   [0-]      kp   = hardness1 * hardness2 / (hardness1 + hardness2) (two springs serial)
       elasticity: [0-1]     kd   = (1-elasticity1) * s2.hardness + (1-elasticity2) * s1.hardness) / 
                                    (s1.hardness + s2.hardness);
     </pre>
     For the calculation of the spring/damping constant we use the following schema:
     The collision can be considered as 2 springs serially connected. 
     The spring constant of each collision side is given by hardness (here kp). The spring constant of
     the entire spring is given by \f[ 1/kp = 1/kp_1 + 1/kp_2\f]. 
     The damping (kd) is derived from the elasticity (e), but it is more difficult to compute. 
     Consider the damping in form of energy lost.
     We can write the energy or work done by each spring as: \f[ W_i = F*s_i  = F^2/p \f] with \f[s_i=F*kp_i\f].     
     The energy lost though damping is \f[ W_1^D = W_i*(1-e_i) \f]. 
     The final damping is now: \f[ kd = (1-e) = W^D/W = \frac{(1-e_1)/kp_1 + (1-e_2)/kp_2}{1/kp_1 + 1/kp_2} 
     = \frac{(1-e_1)kp_2 + (1-e_2)kp_1}{kp_1+kp_2}\f].
  */
  class Substance {
  public:
    Substance();
    Substance( float roughness, float slip, float hardness, float elasticity);

  public:
    
    float roughness;
    float slip;
    float hardness;
    float elasticity;    

    void setCollisionCallback(CollisionCallback func, void* userdata);

    CollisionCallback callback;
    void* userdata;

  public:
    /// Combination of two surfaces
    static dSurfaceParameters getSurfaceParams(const Substance& s1, const Substance& s2);

    static void printSurfaceParams(const dSurfaceParameters& surfParams);

    //// Factory methods 

    /// default substance is plastic with roughness=0.8
    static Substance getDefaultSubstance();
    void toDefaultSubstance();

    /// very hard and elastic 
    static Substance getMetal(float roughness);
    /// very hard and elastic
    void toMetal(float roughness);

    /// high roughness, no slip, a bit elastic, hardness : [0.1-0.5]
    static Substance getRubber(float hardness);
    /// high roughness, no slip, a bit elastic, hardness : [0.1-0.5]
    void toRubber(float hardness);

    /// medium slip, a bit elastic, medium hardness, roughness [0.5-5]
    static Substance getPlastic(float roughness);
    /// medium slip, a bit elastic, medium hardness, roughness [0.5-5]
    void toPlastic(float roughness);

    /// large slip, not elastic, low hardness [0.01-0.5], high roughness
    Substance getFoam(float _hardness);    
    /// large slip, not elastic, low hardness [0.01-0.5], high roughness
    void toFoam(float _hardness);
      
  };

}

#endif

