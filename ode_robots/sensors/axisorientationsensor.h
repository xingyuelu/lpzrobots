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
 *   Revision 1.2  2006-08-11 15:45:38  martius
 *   *** empty log message ***
 *
 *   Revision 1.1  2006/08/08 17:03:27  martius
 *   new sensors model
 *
 *                                                                 *
 ***************************************************************************/
#ifndef __AXISORIENTATIONSENSOR_H
#define __AXISORIENTATIONSENSOR_H

#include "sensor.h"

namespace lpzrobots {

  /** Class for sensing the axis orienation of a primitive (robot)
  */
  class AxisOrientationSensor : public Sensor {
  public:  
    //    TODO: check whether Doxygen can handle the following
    /// Sensor mode 
    enum Mode { OnlyZAxis //< Z axis in word coordinated
		, ZProjection //< z-komponent of each axis
		, XYZAxis //< 3x3 rotation matrix
    };

    AxisOrientationSensor(Mode mode);
    virtual ~AxisOrientationSensor() {}
    
    virtual void init(Primitive* own);  
    virtual int getSensorNumber() const;
  
    virtual bool sense(const GlobalData& globaldata);
    virtual std::list<sensor> get() const;
    virtual int get(sensor* sensors, int length) const;

  private:
    Mode mode;
    Primitive* own;
  };


}

#endif