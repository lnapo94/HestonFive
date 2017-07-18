/**
 *       @file  EuropeanCall.cpp
 *      @brief  The HestonFive BarbequeRTRM application
 *
 * Description: This is the European Call class, used to define the optionCalculator() method inherited by the Option class
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
#include "EuropeanCall.h"

/**
 * The constructor of an European Call option, it used the constructor of the Option base class
 * @param S0		The initial spot price of the option
 * @param K		The strike price of the option
 * @param r		The risk-free rate of the option
 * @param T		The maturity time (in years) of the option
 */
EuropeanCall::EuropeanCall(double S0, double K, double r, double T) : Option(S0, K, r, T)
{

}

/**
 * Method used to compute the option price given the current spot price
 * @param	The current spot price to calculate
 */
double EuropeanCall::optionCalculator(double S) {
    if (S - K > 0)
        return S - K;
    else
        return 0.0;
}
