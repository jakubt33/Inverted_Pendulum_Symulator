
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

#include "Dynamics.h"

Ivp::Dynamics::Dynamics(void)
{
	this->mass = 1.0;
	this->cartMass = 10.0;
	this->rodLength = 1.0;
	this->gravity = 9.8;
	this->linFriction = 0.0; // Proportional to translational friction force
	this->angFriction = 1.0; // Proportional to rotational friction force
	this->currentForce = 0.0;
	this->previousForce = 0.0;
	this->angularPosition = 0.1; // z1
	this->angularVelocity = 0.0; // z2
	this->cartPosition = 0.0; // z3
	this->cartVelocity = 0.0; // z4
	this->timeStep = 0.05;
	this->timeTag = 0.0;
}

double Ivp::Dynamics::sqr(double value)
{
	return value*value;
}

void Ivp::Dynamics::integrateForwardEuler(void)
{
	this->integrateForwardEuler(this->timeStep);
}

void Ivp::Dynamics::integrateForwardRungeKutta4(void)
{
	this->integrateForwardRungeKutta4(this->timeStep);
}

void Ivp::Dynamics::integrateForwardEuler(double step)
{
	double m=this->mass, M=this->cartMass, l=this->rodLength, f_lin=this->linFriction, f_ang=this->angFriction, g=this->gravity, h=step;
	double z1=0.0, z2=0.0, z3=0.0, z4=0.0;

	// Integration using Forward Euler.
	z1 = this->angularPosition + h*this->angularVelocity;
	z2 = this->angularVelocity + h*(this->previousForce*cos(this->angularPosition) - (M+m)*g*sin(this->angularPosition) + m*l*cos(this->angularPosition)*sin(this->angularPosition)*sqr(this->angularVelocity) + f_lin*cos(this->angularPosition)*this->cartVelocity + (M+m)*f_ang/m*this->angularVelocity)/(m*l*sqr(cos(this->angularPosition)) - (M+m)*l);
	z3 = this->cartPosition + h*this->cartVelocity;
	z4 = this->cartVelocity + h*(this->previousForce + m*l*sin(this->angularPosition)*sqr(this->angularVelocity) - m*g*cos(this->angularPosition)*sin(this->angularPosition) + cos(this->angularPosition)*f_ang*this->angularVelocity)/(M + m - m*sqr(cos(this->angularPosition)));

	this->angularPosition = z1;
	this->angularVelocity = z2;
	this->cartPosition = z3;
	this->cartVelocity = z4;
	this->previousForce = this->currentForce;
	this->timeTag += this->timeStep;
}

void Ivp::Dynamics::integrateForwardRungeKutta4(double step)
{
	double K1=0.0f, K2=0.0f, K3=0.0f, K4=0.0f;
	double L1=0.0f, L2=0.0f, L3=0.0f, L4=0.0f;
	double M1=0.0f, M2=0.0f, M3=0.0f, M4=0.0f;
	double N1=0.0f, N2=0.0f, N3=0.0f, N4=0.0f;
	double m=this->mass, M=this->cartMass, l=this->rodLength, f_lin=this->linFriction, f_ang=this->angFriction, g=this->gravity, h=step;
	double z1=0.0, z2=0.0, z3=0.0, z4=0.0;

    // Integration using Forward Runge-Kutta.
	K1 = this->angularVelocity;
	L1 = (this->previousForce*cos(this->angularPosition) - (M+m)*g*sin(this->angularPosition) + m*l*cos(this->angularPosition)*sin(this->angularPosition)*sqr(this->angularVelocity) + f_lin*cos(this->angularPosition)*this->cartVelocity + (M+m)*f_ang/m*this->angularVelocity)/(m*l*sqr(cos(this->angularPosition)) - (M+m)*l);
	M1 = this->cartVelocity;
	N1 = (this->previousForce + m*l*sin(this->angularPosition)*sqr(this->angularVelocity) - m*g*cos(this->angularPosition)*sin(this->angularPosition) + cos(this->angularPosition)*f_ang*this->angularVelocity)/(M+m - m*sqr(cos(this->angularPosition)));
    
	K2 = this->angularVelocity + h/2.0*L1;
	L2 = ((this->previousForce+this->currentForce)/2.0*cos(this->angularPosition+h/2.0*K1) - (M+m)*g*sin(this->angularPosition+h/2.0*K1) + m*l*cos(this->angularPosition+h/2.0*K1)*sin(this->angularPosition+h/2.0*K1)*sqr(this->angularVelocity+h/2.0*L1) + f_lin*cos(this->angularPosition+h/2.0*K1)*(this->cartVelocity+h/2.0*N1) + (M+m)*f_ang/m*(this->angularVelocity+h/2.0*L1))/(m*l*sqr(cos(this->angularPosition+h/2.0*K1)) - (M+m)*l);
	M2 = this->cartVelocity + h/2.0*N1;
	N2 = ((this->previousForce+this->currentForce)/2.0 + m*l*sin(this->angularPosition+h/2.0*K1)*sqr(this->angularVelocity+h/2.0*L1) - m*g*cos(this->angularPosition+h/2.0*K1)*sin(this->angularPosition+h/2.0*K1) + cos(this->angularPosition+h/2.0*K1)*f_ang*(this->angularVelocity+h/2.0*L1))/(M+m - m*sqr(cos(this->angularPosition+h/2.0*K1)));
    
	K3 = this->angularVelocity + h/2.0*L2;
	L3 = ((this->previousForce+this->currentForce)/2.0*cos(this->angularPosition+h/2.0*K2) - (M+m)*g*sin(this->angularPosition+h/2.0*K2) + m*l*cos(this->angularPosition+h/2.0*K2)*sin(this->angularPosition+h/2.0*K2)*sqr(this->angularVelocity+h/2.0*L2) + f_lin*cos(this->angularPosition+h/2.0*K2)*(this->cartVelocity+h/2.0*N2) + (M+m)*f_ang/m*(this->angularVelocity+h/2.0*L2))/(m*l*sqr(cos(this->angularPosition+h/2.0*K2)) - (M+m)*l);
	M3 = this->cartVelocity + h/2.0*N2;
	N3 = ((this->previousForce+this->currentForce)/2.0 + m*l*sin(this->angularPosition+h/2.0*K2)*sqr(this->angularVelocity+h/2.0*L2) - m*g*cos(this->angularPosition+h/2.0*K2)*sin(this->angularPosition+h/2.0*K2) + cos(this->angularPosition+h/2.0*K2)*f_ang*(this->angularVelocity+h/2.0*L2))/(M+m - m*sqr(cos(this->angularPosition+h/2.0*K2)));
    
	K4 = this->angularVelocity + h*L3;
	L4 = (this->previousForce *cos(this->angularPosition+h*K3) - (M+m)*g*sin(this->angularPosition+h*K3) + m*l*cos(this->angularPosition+h*K3)*sin(this->angularPosition+h*K3)*sqr(this->angularVelocity+h*L3) + f_lin*cos(this->angularPosition+h*K3)*(this->cartVelocity+h*N3) + (M+m)*f_ang/m*(this->angularVelocity+h*L3))/(m*l*sqr(cos(this->angularPosition+h*K3)) - (M+m)*l);
	M4 = this->cartVelocity + h*N3;
	N4 = (this->previousForce + m*l*sin(this->angularPosition+h*K3)*sqr(this->angularVelocity+h*L3) - m*g*cos(this->angularPosition+h*K3)*sin(this->angularPosition+h*K3) + cos(this->angularPosition+h*K3)*f_ang*(this->angularVelocity+h*L3))/(M+m - m*sqr(cos(this->angularPosition+h*K3)));

	z1 = this->angularPosition + h*(1.0/6.0*K1 + 2.0/6.0*K2 + 2.0/6.0*K3 + 1.0/6.0*K4);
	z2 = this->angularVelocity + h*(1.0/6.0*L1 + 2.0/6.0*L2 + 2.0/6.0*L3 + 1.0/6.0*L4);
	z3 = this->cartPosition	   + h*(1.0/6.0*M1 + 2.0/6.0*M2 + 2.0/6.0*M3 + 1.0/6.0*M4);
	z4 = this->cartVelocity    + h*(1.0/6.0*N1 + 2.0/6.0*N2 + 2.0/6.0*N3 + 1.0/6.0*N4);

	this->angularPosition = z1;
	this->angularVelocity = z2;
	this->cartPosition = z3;
	this->cartVelocity = z4;
	this->previousForce = this->currentForce;
	this->timeTag += this->timeStep;
}

double Ivp::Dynamics::getTimeStep(void)
{
	return this->timeStep;
}

void Ivp::Dynamics::setTimeStep(double ts)
{
	this->timeStep = ts;
}

double Ivp::Dynamics::getTimeTag(void)
{
	return this->timeTag;
}

void Ivp::Dynamics::setTimeTag(double tt)
{
	this->timeTag = tt;
}

