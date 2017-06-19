
/*
	Inverted Pendulum Project (IVP)
	Copyright (C) 2011 Gonçalo Morgado (goncalo.magno@gmail.com)

	This file is part of IVP.

    IVP is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    IVP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with IVP.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __DYNAMICS_H__
#define __DYNAMICS_H__

#include <iostream>
#include <cmath>

namespace Ivp
{
	// classes
	class Dynamics
	{
	public:
		// Interface
		// Constructor by default.
		Dynamics(void);
		void integrateForwardEuler(double step);
		void integrateForwardRungeKutta4(double step);
		void integrateForwardEuler(void);
		void integrateForwardRungeKutta4(void);

		double getTimeStep(void);
		void setTimeStep(double ts);
		double getTimeTag(void);
		void setTimeTag(double tt);

		double sqr(double value);

//	private:
		// Data Structure
		double mass;
		double cartMass;
		double rodLength;
		double gravity;
		double linFriction;
		double angFriction;
		double currentForce;
		double previousForce;
		double angularPosition;
		double angularVelocity;
		double cartPosition;
		double cartVelocity;
		double timeStep;
		double timeTag;
	};
}


#endif


