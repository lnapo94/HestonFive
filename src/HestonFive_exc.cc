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


#include "HestonFive_exc.h"

#include <cstdio>
#include <bbque/utils/utility.h>

/**
 * Implementation of the constructor of the HestonFour class. It needs all the Option parameters and the user's 
 * values for the number of the simulations and the correct discretization
 *
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
HestonFive::HestonFive(std::string const & name,
		std::string const & recipe,
		RTLIB_Services_t *rtlib, double S0, double K, double r, double T, double V0, double rho, double kappa, double theta, double xi,
		int todo_simulations, int discretization) :
	BbqueEXC(name, recipe, rtlib) {

	logger->Warn("New HestonFive::HestonFive()");

	// NOTE: since RTLib 1.1 the derived class construct should be used
	// mainly to specify instantiation parameters. All resource
	// acquisition, especially threads creation, should be palced into the
	// new onSetup() method.

	logger->Info("EXC Unique IDentifier (UID): %u", GetUniqueID());

	this->S0 = S0;
	this->K = K;
	this->r = r;
	this->T = T;
	this->V0 = V0;
	this->rho = rho;
	this->kappa = kappa;
	this->theta = theta;
	this->xi = xi;

	if(todo_simulations < WORKERS_SIM) {
		std::cout << "Lower than allowed number. Minimun is: " << WORKERS_SIM << std::endl;
		todo_simulations = WORKERS_SIM;
	}

	this->todo_simulations = todo_simulations;
	this->discretization = discretization;
	this->doneSimulations = 0;
	
	this->pricesToCompute = (int) (todo_simulations / WORKERS_SIM);
	this->computedPrices = new double[pricesToCompute];
	this->computedPricesIndex = 0;	

	std::cout << std::endl;

	std::cout << "S0: " << this->S0 << std::endl;
	std::cout << "K: " << this->K << std::endl;
	std::cout << "r: " << this->r << std::endl;
	std::cout << "T: " << this->T << std::endl;

	std::cout << "V0: " << this->V0 << std::endl;
	std::cout << "rho: " << this->rho << std::endl;
	std::cout << "kappa: " << this->kappa << std::endl;
	std::cout << "theta: " << this->theta << std::endl;
	std::cout << "xi: " << this->xi << std::endl;

	std::cout << "SIMULATIONS TO-DO: " << this->todo_simulations << std::endl;
	std::cout << "DISCRETIZATION: " << this->discretization << std::endl;

	std::cout << std::endl;


}

/**
 * Method used to do all the Setup operations
 */
RTLIB_ExitCode_t HestonFive::onSetup() {
	// This is just an empty method in the current implementation of this
	// testing application. However, this is intended to collect all the
	// application specific initialization code, especially the code which
	// acquire system resources (e.g. thread creation)
	logger->Warn("HestonFive::onSetup()");
	
	workersFinalSum = 0.0;

	/**
	 * @brief Number of max processor in the computer
	 */
	cpuNumber = (int) std::thread::hardware_concurrency();
	std::cout << "Number of detected processors: " << cpuNumber << std::endl;


	/**
	 * @brief Create the workers with the NUM_PROC variables
	 */	
	workers = new HestonWorker*[cpuNumber]; 	

	for(int i=0;i<cpuNumber; i++){
		logger->Warn("Creating new worker"); 
		workers[i] = new HestonWorker( S0, K, r, T, V0, rho, kappa, theta, xi);
	}
	
	return RTLIB_OK;
}

/**
 * Method used to do all the Configure operations. Every time the BarbequeRTRM changes the resources assigned to this application,
 * Resource Manager calls this method to reconfigure the application
 */
RTLIB_ExitCode_t HestonFive::onConfigure(int8_t awm_id) {

	logger->Warn("HestonFive::onConfigure(): EXC [%s] => AWM [%02d]",
		exc_name.c_str(), awm_id);

	int32_t proc_quota, proc_nr, mem;
	GetAssignedResources(PROC_ELEMENT, proc_quota);
	GetAssignedResources(PROC_NR, proc_nr);
	GetAssignedResources(MEMORY, mem);
	logger->Notice("TestWorkload::onConfigure(): "
		"EXC [%s], AWM[%02d] => R<PROC_quota>=%3d, R<PROC_nr>=%2d, R<MEM>=%3d",
		exc_name.c_str(), awm_id, proc_quota, proc_nr, mem);

	workersNumber = proc_nr;

	return RTLIB_OK;
}

/**
 * Method used to start the computation of an Option price after our app is configured correctly in onCofigure() method
 */
RTLIB_ExitCode_t HestonFive::onRun() {
	RTLIB_WorkingModeParams_t const wmp = WorkingModeParams();

	// Return when all the simulations are done
	if (doneSimulations >= todo_simulations){
		
		return RTLIB_EXC_WORKLOAD_NONE;
	}

	if(doneSimulations + workersNumber * WORKERS_SIM > todo_simulations) {
		workersNumber = (todo_simulations - doneSimulations) / WORKERS_SIM;
		if(workersNumber == 0)
			workersNumber = 1;
	}

	for(int i = 0; i < workersNumber; i++){
		workers[i]->start(WORKERS_SIM, discretization);
	}
	
	for(int i = 0; i < workersNumber; i++){
		workers[i]->join();
		doneSimulations += WORKERS_SIM;
		double temp =  ( ( workers[i]->getCalculus() / (double) ( WORKERS_SIM * 2)) * exp( -(r) * (T) ) );
		logger->Warn("Worker %d computed price: %f ", i, temp );

		workersFinalSum += workers[i]->getCalculus();
		computedPrices[computedPricesIndex] = temp;
		computedPricesIndex++;
	}

	// Do one more cycle
	logger->Warn("HestonFive::onRun()      : EXC [%s]  @ AWM [%02d]",
		exc_name.c_str(), wmp.awm_id);

	return RTLIB_OK;
}

/**
 * Method used to monitor every computation and to give a partial result
 */
RTLIB_ExitCode_t HestonFive::onMonitor() {
	RTLIB_WorkingModeParams_t const wmp = WorkingModeParams();

	logger->Warn("HestonFive::onMonitor()  : EXC [%s]  @ AWM [%02d], Cycle [%4d]",
		exc_name.c_str(), wmp.awm_id, Cycles());

	threadFinalPrice = ( ( workersFinalSum / (double) ((doneSimulations * 2))) * exp( -(r) * (T) ) );
	logger->Warn("ON_MONITOR: Price updated: %f", threadFinalPrice);

	return RTLIB_OK;
}

/**
 * Method used to do the final operations before the closing of the app
 */
RTLIB_ExitCode_t HestonFive::onRelease() {

	logger->Warn("HestonFive::onRelease()  : exit");
	
	for(int i=0; i < this->pricesToCompute; i++){
		computedPrices[i] = (computedPrices[i] - threadFinalPrice) * (computedPrices[i] - threadFinalPrice);	
	} 

	double variance = 0.0;
	for(int i=0; i < this->pricesToCompute; i++){
		variance += computedPrices[i];
	}
	variance = sqrt(variance / (this->pricesToCompute));	
	logger->Warn("Standard Deviation: %f", variance);	

	for(int i=0; i<cpuNumber; i++){
		delete workers[i];
	}
	delete[] workers;

	return RTLIB_OK;
}
