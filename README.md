# Heston Model on BarbequeRTRM
_**Authors:** [Claudio Montanari](https://github.com/claudioMontanari)  **&** [Luca Napoletano](https://github.com/lnapo94)_

### Welcome to our repository of the Project on the BarbequeRTRM framework!
This is our README file. We create this document to help you to understand the usage and the general idea of our project.

### What is our application about?
Nowadays options are one of the most traded financial derivatives. Different kinds of this contract types exist, such as European, American and Exotic. 
The process to price them is not trivial, thus, a portfolio strategy requires an accurate and quick pricing tool in order to be successful. To describe options value oscillations as a function of time, in the most appropriate mathematical way, we need to use stochastic volatility models. Among them the Heston model is the best one in describing the market behavior. 
Due to the complexity of the Heston model, only European options may be analytically evaluated, hence, Monte Carlo simulations are used to overcome this obstacle.
Since every simulation is independent from each other, this algorithm can be executed in parallels

### Why we have developed this algorithm under the BarbequeRTRM?
This is a project done during our bachelor degree, and we have chosen this algorithm like an example of _approximate calculus model_ 
to create a case study for the BarbequeRTRM. We have developed a reconfigurable application that changes its precision according to the 
resources parameters given by the framework 

### How our application work?
HestonFive application is divided into two main parts, Option class and HestonWorker class. These two classes are created to reach two main goals: the expandability of our code with new kind of option and the run-time reconfiguration. In fact, to reach the first goal there is the Option class; it is the base class for all the options. If you want to add a new option, you can easily override the virtual method _optionCalculator()_ with the correct operations to calculate the final price of your option.
While, to reach the second goal, we have used the HestonWorker class, that create a thread to compute on packet of simulations. Each HestonWorker is created in the setup function of the application considering the processors number in the machine. After that, in the configuration function of the app, we take from the BarbequeRTRM platform the processor quote assigned to us, and with that parameter we configure the exact number of workers to start for the computation. Moreover, every time the BarbequeRTRM reconfigure our application, we always start the correct number of worker to do all the required simulations in the shortest time.

### Would you like more information?
If you want more information about some classes or some methods, please, check out our [documentation pages](https://lnapo94.github.io/HestonFive).
If you have more questions about our application, feel free to contact us.
