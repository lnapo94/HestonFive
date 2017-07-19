# Heston Model on BarbequeRTRM
_**Authors:** [Claudio Montanari](https://github.com/claudioMontanari)  **&** [Luca Napoletano](https://github.com/lnapo94)_

### Welcome to our repository of the Project on the BarbequeRTRM framework!
This is our README file. We create this document to help you to understand the usage and the general idea of our project.

### What is our application about?
Nowadays options are one of the most traded financial derivatives. Different kinds of this contract types exist, such as European, American and Exotic. 
The process to price them is not trivial, thus, a portfolio strategy requires an accurate and quick pricing tool in order to be successful. To describe options value oscillations as a function of time, in the most appropriate mathematical way, we need to use stochastic volatility models. Among them the Heston model is the best one in describing the market behavior. 
Due to the complexity of the Heston model, only European options may be analytically evaluated, hence, Monte Carlo simulations are used to overcome this obstacle.
Since every simulation is independent from each other, this algorithm can be executed in parallel

### Why we have developed this algorithm under the BarbequeRTRM?
This is a project done during our bachelor degree, and we have chosen this algorithm like an example of _approximate calculus model_ 
to create a case study for the BarbequeRTRM. So, we have developed a reconfigurable version of an Options pricing tool that changes its precision according to the 
resources parameters given by the framework 

### What is the Heston Model?
The Heston model is a mathematical model used in financial world to describe the oscillations of the option price. Basically, it is composed by three stochastic diferential equations: one for the option price value, one for the volatility of the option and another for the correlation between the price and the volatility.   
It is useful because, before that model, options were modeled with a constant volatility, and in this way the forecast was misleading. But, with the Heston Model, the volatility is considered as a stochastic process, so the computed option price is very similiar to the real option price. However, this model is far more complex and for this reason it can not be analitically evaluated (exception made for some particular occasions) 

### What is the Monte Carlo method?
The Monte Carlo method is a simple, but very powerful, algorithm used to compute many stochastic equations thanks to a simple idea: simulating the equation. If an equation cannot be computed, but has stochastics variables (and respects some statistical theorems), it can be simulated. This algorithm is used in a wide range of fields: mathematics, physics, engineering, finance and so on. The main problem of this algorithm is the error: since it is a simulation it gives only an approximation and not the exact result of the equation. But, fortunately, thanks to the Central Limit Theorem: the more is the number of simulations, the less is the error (it decreases as _O(n^-1/2)_ with _n_ the number of simulations).
For our financial problem, the Heston Model solution, is a very good algorithm, because it give us a very good result in a good time.

### How our application works?
HestonFive application is divided into two main parts: the Option class and the HestonWorker class. These two classes are created to reach two main goals: the expandability of our code with new kind of options and the run-time reconfiguration. In fact, to reach the first goal there is the Option class; it is the base class for all the options. If you want to add a new option, you can easily override the virtual method `optionCalculator(double currentValue)` with the correct operations to calculate the payoff value of your option.
While, to reach the second goal, we have used the HestonWorker class, who creates a thread to compute on a set of simulations. Each HestonWorker is created in the setup function of the application considering the processors number in the machine. After that, in the configuration function of the app, we take from the BarbequeRTRM platform the processor quote assigned to us, and with that parameter we configure the exact number of workers to start. Moreover, every time the BarbequeRTRM reconfigure our application, we always start the correct number of workers to do all the required simulations in the shortest time.
The HestonWorker has a fixed number of simulations, and all the created workers do the same number for the needed time to complete all the required simulations. 

### How to start our application?
First of all, clone this git repository in the BOSP directory: /BOSP/contrib/user/. After that, use `make bootstrap` (in this way BarbequeRTRM search our application and add it to the BOSP files), then use `make menuconfig` to select our application and add it to the BarbequeRTRM selected apps. Finally, start Barbeque and then start our application typing `hestonfive` in the BOSP CLI.

### How to setup new parameters in the application?
You can setup new option values from command line, simply typing one, or more, of this command:
* `-n [--sims]`: Setup the number of simulations to do (60000 by default)
* `-d [--discr]`: Setup the discretization value (300 by default)
* `-r [--real]`: Setup the correct option value to know the error (34.9998 by default)

* `-s [--spot]`: Setup the spot price of the option (100.0 by default)
* `-K [--strike]`: Setup the strike price of the option (100.0 by default)
* `-R [--risk]`: Setup the risk-free rate value of the option (0.05 by default)
* `-T [--time]`: Setup the maturity time of the option, in years (5.0 by default)

* `-v [--vol]`: Setup the volatility of the option (0.09 by default)
* `-r [--rho]`: Setup the correlation coefficient of the option (-0.3 by default)
* `-k [--kappa]`: Setup the mean reversion rate of the option (2.0 by default)
* `-t [--theta]`: Setup the long-term volatility of the option (0.09 by default)
* `-x [--xi]`: Setup the volatility value of the option volatility (1.0 by default)

Remember that the minimum supported value of simulations is 10000 (since going under this value would certainly lead to a wrong forecast). Moreover, if you change the option parameters, remember to change also the exact value of the option. In this way you will have the good computation of the error.

If you can't remeber all of these settings, don't worry, you can just type `hestonfive -h` on console.

### Would you like more information?
If you want more information about some classes or some methods, please, check out our [documentation pages](https://lnapo94.github.io/HestonFive). 
If you want more information about the BarbequeRTRM project, go to [this site](https://bosp.dei.polimi.it/doku.php).

Otherwise, if you have more questions about our application, feel free to contact us.
