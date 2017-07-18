/**
 *       @file  Option.cpp
 *      @brief  The HestonFive BarbequeRTRM application
 *
 * Description: This is the main abstract class that represent an Option. This class has a virtual method, called optionCalculator()
 *		used by the subclasses to implement correctly the calculus of the various options
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
#include "Option.h"

/**
 * The constructor of an Option
 * @param S0		The initial spot price of the option
 * @param K		The strike price of the option
 * @param r		The risk-free rate of the option
 * @param T		The maturity time (in years) of the option
 */
Option::Option(double S0, double K, double r, double T)
{
    this->S0 = S0;
    this->K = K;
    this->r = r;
    this->T = T;
}

Option::~Option()
{

}

/**
 * Simple getter to have the option initial spot price
 */
double Option::getSpotPrice() {
    return this->S0;
}

/**
 * Simple getter to have the option strike price
 */
double Option::getStrikePrice() {
    return this->K;
}

/**
 * Simple getter to have the risk-free rate of the option
 */
double Option::getRiskFreeRate() {
    return this->r;
}

/**
 * Simple getter to know the maturity of the option
 */
double Option::getMaturity() {
    return this->T;
}
