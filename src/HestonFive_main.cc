/**
 *       @file  HestonFive_exc.cc
 *      @brief  The HestonFour BarbequeRTRM application
 *
 * Description: It is the main class of the HestonFive project. The main take from the user all the parameters of the desired option
 *		and start the computation. If the user doesn't insert any parameter, the computation starts with default values
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

#include <cstdio>
#include <iostream>
#include <random>
#include <cstring>
#include <memory>

#include <libgen.h>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include "version.h"
#include "HestonFive_exc.h"
#include <bbque/utils/utility.h>
#include <bbque/utils/logging/logger.h>

// Setup logging
#undef  BBQUE_LOG_MODULE
#define BBQUE_LOG_MODULE "HestonFive"

namespace bu = bbque::utils;
namespace po = boost::program_options;

/**
 * @brief A pointer to an EXC
 */
std::unique_ptr<bu::Logger> logger;

/**
 * @brief A pointer to an EXC
 */
typedef std::shared_ptr<BbqueEXC> pBbqueEXC_t;

/**
 * The decription of each HestonFive parameters
 */
po::options_description opts_desc("HestonFive Configuration Options");

/**
 * The map of all HestonFive parameters values
 */
po::variables_map opts_vm;

/**
 * The services exported by the RTLib
 */
RTLIB_Services_t *rtlib;

/**
 * @brief The application configuration file
 */
std::string conf_file = BBQUE_PATH_PREFIX "/" BBQUE_PATH_CONF "/HestonFive.conf" ;

/**
 * @brief The recipe to use for all the EXCs
 */
std::string recipe;

/**
 * @brief The EXecution Context (EXC) registered
 */
pBbqueEXC_t pexc;

/**
 * @brief The spot price of the option. By default the value is 100.0
 */
double S0;

/**
 * @brief The strike price of the option. By default the value is 100.0
 */
double K;

/**
 * @brief The risk-free rate of the option. By default the value is 0.05
 */
double r;

/**
 * @brief The maturity time, in years, of the option. By default the value is 5
 */
double T;

/**
 * @brief The initial volatility of the option. By default the value is 0.09
 */
double V0;

/**
 * @brief The correlation coefficient of the option. By default the value is -0.30
 */
double rho;

/**
 * @brief The mean-reversion rate of the option. By default the value is 2.0
 */
double kappa;

/**
 * @brief The long-term volatility value of the option. By default the value is 0.09
 */
double theta;

/**
 * @brief The volatility of the option volatility. By default the value is 1.0
 */
double xi;

/**
 * @brief The number of the simulations to do. By default the value is 60000
 */
int simulationNumber;

/**
 * @brief The discretization value of each simulation. By default the value is 300
 */
int discretization;


void ParseCommandLine(int argc, char *argv[]) {
	// Parse command line params
	try {
	po::store(po::parse_command_line(argc, argv, opts_desc), opts_vm);
	} catch(...) {
		std::cout << "Usage: " << argv[0] << " [options]\n";
		std::cout << opts_desc << std::endl;
		::exit(EXIT_FAILURE);
	}
	po::notify(opts_vm);

	// Check for help request
	if (opts_vm.count("help")) {
		std::cout << "Usage: " << argv[0] << " [options]\n";
		std::cout << opts_desc << std::endl;
		::exit(EXIT_SUCCESS);
	}

	// Check for version request
	if (opts_vm.count("version")) {
		std::cout << "HestonFive (ver. " << g_git_version << ")\n";
		std::cout << "Copyright (C) 2011 Politecnico di Milano\n";
		std::cout << "\n";
		std::cout << "Built on " <<
			__DATE__ << " " <<
			__TIME__ << "\n";
		std::cout << "\n";
		std::cout << "This is free software; see the source for "
			"copying conditions.  There is NO\n";
		std::cout << "warranty; not even for MERCHANTABILITY or "
			"FITNESS FOR A PARTICULAR PURPOSE.";
		std::cout << "\n" << std::endl;
		::exit(EXIT_SUCCESS);
	}
}

/**
 * The main method of our application, it is used only to start the computation once the parameters is given by the user
 */
int main(int argc, char *argv[]) {

	opts_desc.add_options()
		("help,h", "print this help message")
		("version,v", "print program version")

		("conf,C", po::value<std::string>(&conf_file)->
			default_value(conf_file),
			"HestonFive configuration file")

		("recipe,r", po::value<std::string>(&recipe)->
			default_value("HestonFive"),
			"recipe name (for all EXCs)")

		("sims,n", po::value<int>(&simulationNumber)->
			default_value(60000),
			"Number of simulations")

		("discr,d", po::value<int>(&discretization)->
			default_value(300),
			"Discretization value")

		("spot,s", po::value<double>(&S0)->
			default_value(100.0),
			"Option Spot Price")
		("strike,K", po::value<double>(&K)->
			default_value(100.0),
			"Option Strike Price")
		("risk,R", po::value<double>(&r)->
			default_value(0.05),
			"Risk-Free Rate")
		("time,T", po::value<double>(&T)->
			default_value(5.0),
			"Maturity Time [In Years]")

		("vol,v", po::value<double>(&V0)->
			default_value(0.09),
			"Volatility")
		("rho,r", po::value<double>(&rho)->
			default_value(-0.30),
			"Correlation Coefficient")
		("kappa,k", po::value<double>(&kappa)->
			default_value(2.0),
			"Mean Reversion")
		("theta,th", po::value<double>(&theta)->
			default_value(0.09),
			"Long-Term volatility")
		("xi,x", po::value<double>(&xi)->
			default_value(1.0),
			"Volatility of volatility")
	;

	// Setup a logger
	bu::Logger::SetConfigurationFile(conf_file);
	logger = bu::Logger::GetLogger("hestonfive");

	ParseCommandLine(argc, argv);

	// Welcome screen
	logger->Info(".:: HestonFive (ver. %s) ::.", g_git_version);
	logger->Info("Built: " __DATE__  " " __TIME__);

	// Initializing the RTLib library and setup the communication channel
	// with the Barbeque RTRM
	logger->Info("STEP 0. Initializing RTLib, application [%s]...",
			::basename(argv[0]));

	if ( RTLIB_Init(::basename(argv[0]), &rtlib) != RTLIB_OK) {
		logger->Fatal("Unable to init RTLib (Did you start the BarbequeRTRM daemon?)");
		return RTLIB_ERROR;
	}

	assert(rtlib);

	logger->Info("STEP 1. Registering EXC using [%s] recipe...",
			recipe.c_str());
	pexc = pBbqueEXC_t(new HestonFive("HestonFive", recipe, rtlib, S0, K, r, T, V0, rho, kappa, theta, xi, simulationNumber, discretization));
	if (!pexc->isRegistered()) {
		logger->Fatal("Registering failure.");
		return RTLIB_ERROR;
	}


	logger->Info("STEP 2. Starting EXC control thread...");
	pexc->Start();


	logger->Info("STEP 3. Waiting for EXC completion...");
	pexc->WaitCompletion();


	logger->Info("STEP 4. Disabling EXC...");
	pexc = NULL;

	logger->Info("===== HestonFive DONE! =====");
	return EXIT_SUCCESS;

}

