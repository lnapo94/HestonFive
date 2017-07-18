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
#ifndef OPTION_H
#define OPTION_H


class Option
{
    public:

	/**
	 * The constructor of an Option
	 * @param S0		The initial spot price of the option
	 * @param K		The strike price of the option
	 * @param r		The risk-free rate of the option
	 * @param T		The maturity time (in years) of the option
	 */
        Option(double S0, double K, double r, double T); /**< Constructor for the Option Type */

        virtual ~Option();                               /**< This will be a probably destructor */

	/**
	 * Simple getter to have the option initial spot price
	 */
        double getSpotPrice();                           /**< The get method for the Spot Price*/

	/**
	 * Simple getter to have the option strike price
	 */
        double getStrikePrice();                         /**< The get method for the Strike Price*/

	/**	
	 * Simple getter to have the risk-free rate of the option
	 */
        double getRiskFreeRate();                        /**< The get method for the Risk Free Rate */

	/**
	 * Simple getter to know the maturity of the option
	 */
        double getMaturity();                            /**< The get method for the Maturity */

	/**
	 * This is a method which must be implemented in the inherited classes
	 */  
      	virtual double optionCalculator(double) =0;

    protected:
        double S0;      /**< Represent the initial Spot Price */
        double K;       /**< Represent the Strike Price of the Option */
        double r;       /**< Represent the Risk Free Rate of the Option */
        double T;       /**< This is the Maturity of the Option expressed in year */

    private:

};

#endif // OPTION_H
