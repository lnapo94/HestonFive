/**
 *       @file  EuropeanPut.cpp
 *      @brief  The HestonFive BarbequeRTRM application
 *
 * Description: This is the European Put class, used to define the optionCalculator() method inherited by the Option class
 *
 *     @author  Luca Napoletano luca.napoletano@mail.polimi.it, Claudio Montanari claudio1.montanari@mail.polimi.it
 *
 *     Company  Politecnico di Milano
 *   Copyright  Copyright (c) 2017, Luca Napoletano, Claudio Montanari
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License as published by the Free Software Foundation.
 * =====================================================================================
 */
#ifndef EUROPEANPUT_H
#define EUROPEANPUT_H

#include "Option.h"

class EuropeanPut : public Option
{
public:

    /**
     * The constructor of an European Put option, it used the constructor of the Option base class
     * @param S0	The initial spot price of the option
     * @param K		The strike price of the option
     * @param r		The risk-free rate of the option
     * @param T		The maturity time (in years) of the option
     */
    EuropeanPut(double S0, double K, double r, double T);

    /**
     * Method used to compute the option price given the current spot price
     * @param	The current spot price to calculate
     */
    double optionCalculator(double);
};

#endif // EUROPEANPUT_H
