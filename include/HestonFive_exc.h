/**
 *       @file  HestonFive_exc.cc
 *      @brief  The HestonFive BarbequeRTRM application
 *
 * Description: This file represents all the mainly method implementation of our application.
 * 		In this file you can file all the initialize operations for our code. But, the most importants operations
 *		are implemented in HestonWorker.cc (you can find it in src/).
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

#ifndef HESTONFIVE_EXC_H_
#define HESTONFIVE_EXC_H_

#include <bbque/bbque_exc.h>

#include "HestonWorker.h"

#include <iostream>
#include <random>
#include <time.h>
#include <math.h>

using bbque::rtlib::BbqueEXC;

class HestonFive : public BbqueEXC {

public:

	/**
 	 * Implementation of the constructor of the HestonFour class. It needs all the Option parameters and the user's 
 	 *			preferred values for the number of the simulations and the correct discretization
 	 * @param name		The name of the application
 	 * @param recipe	A reference to the recipe of the application
 	 * @param S0		The spot price of the option
 	 * @param K		The strike price of the option
 	 * @param r		The risk-free rate of the option
 	 * @param T		The maturity time of the option (in years)
 	 * @param V0		The initial volatility of the option
 	 * @param rho		The Correlation Coefficient parameter of Heston model for the specified option
 	 * @param kappa		The mean reversion rate of the Heston Model for the considered option
	 * @param theta		The long-term volatility value
 	 * @param xi		The volatility of volatility (V0)
 	 * @param N_SIM		The number of wanted simulations
 	 * @param DISCR		The discretization value
 	 */
	HestonFive(std::string const & name,
			std::string const & recipe,
			RTLIB_Services_t *rtlib, double, double, double, double, double, double, double, double, double, int, int);

private:

	HestonWorker** workers;
	int workersNumber;
	int doneSimulations;
	int todo_simulations;
	int discretization;
	int cpuNumber;
	const int WORKERS_SIM = 10000;

	double finalPrice;
	int pricesToCompute;
	double* computedPrices;
	int computedPricesIndex;

	/**
	 * Variable used to accumulate the results from each run
	 */
	double workersFinalSum;
	double threadFinalPrice;
	
	/**
	 *  Variables used to setup the heston simulation
	 */
	double V0;
	double rho;
	double kappa;
	double theta;
	double xi;

	/**
	 * Variables used to setup the option
	 */
	double S0;
	double K;
	double r;
	double T;
	
	/**
 	 * Method used to do all the Setup operations
 	 */
	RTLIB_ExitCode_t onSetup();

	/**
 	 * Method used to do all the Configure operations. Every time the BarbequeRTRM changes the resources assigned to this application,
 	 * the Resource Manager calls this method to reconfigure the application
 	 */
	RTLIB_ExitCode_t onConfigure(int8_t awm_id);

	/**
 	 * Method used to start the computation of an Option price after our app is configured correctly in onCofigure() method
	 */
	RTLIB_ExitCode_t onRun();

	/**
	 * Method used to monitor every computation and to give a partial result
	 */
	RTLIB_ExitCode_t onMonitor();

	/**
	 * Method used to do the final operations before the closing of the app
	 */
	RTLIB_ExitCode_t onRelease();

};

#endif // HESTONFIVE_EXC_H_
