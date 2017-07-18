/**
 *       @file  HestonWorker.cc
 *
 * Description: The most important part of our application. This class calculates the Heston simulations option price.
 *		Every worker has thread to calculate the option price, in this way the application can run in a parallel
 *		way
 *     @author  Luca Napoletano luca.napoletano@mail.polimi.it, Claudio Montanari claudio1.montanari@mail.polimi.it
 *
 *     Company  Politecnico di Milano
 *   Copyright  Copyright (c) 2017, Luca Napoletano, Claudio Montanari
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License as published by the Free Software Foundation.
 * =====================================================================================
 */
#ifndef HESTONWORKER_H_
#define HESTONWORKER_H_

#include <bbque/bbque_exc.h>

#include <iostream>
#include <random>
#include <time.h>
#include <math.h>
#include <thread>

#include "Option.h"
#include "EuropeanCall.h"

using bbque::rtlib::BbqueEXC;

class HestonWorker {

public:
	/**
	 * The constructor of the HestonWorker class
	 *
	 * @param S0		The spot price of the option
	 * @param K		The strike price of the option
	 * @param r		The risk-free rate of the option
	 * @param T		The maturity time of the option (in years)
	 * @param V0		The initial volatility of the option
	 * @param rho		The Correlation Coefficient parameter of Heston model for the specified option
	 * @param kappa		The mean reversion rate of the Heston Model for the considered option	
	 * @param theta		The long-term volatility value
	 * @param xi		The volatility of volatility (V0)
	 */
	HestonWorker(double S0, double K, double r, double T, double V0, double rho, double kappa, double theta, double xi);

	/**
	 * Distructor of the HestonWorker, used to delete the created option
 	 */
	~HestonWorker();

	/**
	 * Method used to start a simulation
	 * @param simulationToDo	The number of the simulations that a single worker has to do
	 * @param discretization	The value of discretization of the simulation
	 */
	void start(int simulationToDo, int discretization);

	/**
	 * Method used to stop a worker
	 */
	int stop();

	/**
	 * Method used to wait a computation of a worker
	 */
	void join();

	/**
	 * Method used to do an Heston Simulation. It is used for the thread function
	 */
	void hestonSimulation();

	/**
	 * Method used to get the calculated value stored in the worker
	 */
	double getCalculus();

	/**
	 * Method used to get the number of the simulations done by a worker
	 */
	int getSimulationsDone();

private:
	
	int todo_simulations;
	int done_simulations;
	int discretization;
	bool hasToWork;

	double finalPrice;
	/**
	 * Variable used to accumulate the results from each run
	 */
	double totalSum;
	/**
	 *  Variables used to setup the heston simulation
	 */

	double V0;
	double rho;
	double kappa;
	double theta;
	double xi;

	/**
	 * Variable used to setup the option
	 */
	
	Option* option;
	
	/**
	 * Random Generator 
	 */
	
	std::mt19937 generator;	
	std::thread worker;	

	/**
	 * Method used to calculate an approximation of the passed value
	 * @param t		The number to approximate	
	 */
	double rationalApproximation(double t);

	/**
	 * Method used to calculate the inverse of the standard normal function
	 * @param p		A value between 0 and 1 (exclused) that represents a value of a standard normal distribution
	 */
	double normalCDFInverse(double p);

	/**
	 * Method used to get the max given to values
	 * @param x	The first parameter to check
	 * @param y	The second parameter to check
	 */
	double maxValue(double, double);
};

#endif // HESTONWORKER_H_
