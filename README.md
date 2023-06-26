
# Semaphores Synchronisation

This project was done as part of course **CS-3523** which was organized by **Dr. Sathya Peri** which aims to develop a **Synchronisation program using Semaphores**

## About the project

In this project I have used **Semaphores** to implement Synchronisation of the classic **Jurassic Park problem** in **Galvin Textbook** 

### Overview of the question

We need to synchronize **m passengers** and **n safari cars** which can accomodate one person at a time. Semaphores should used to ensure proper synchronization between passengers and cars. Passengers wait for available cars, while car wait to be used by a passenger. Graphs are plotted to visualize the relationship between average time of whole time of passenger to complete his all rides considering different passenger and car configurations.





## Authors

- [@anudeep512](https://www.github.com/anudeep512)


## Tech Stack

**Command Line:** C++
## Prerequisite

Check if a suitable compiler exists for compiling the C++ code

for g++ compiler:

    g++ --version

for clang compiler:

    clang --version

If there isn't any compiler installed, install it using the documentation for the compiler of C++ based on your OS.

## Install and Usage

### Installing project

Clone from repo:

    git clone https://github.com/anudeep512/Semaphores-Synchronisation/

### Running the Program

After cloning is done redirect control to project folder using:

    cd ./Semaphores-Synchronisation

For compiling the program run:

    g++ -std=c++20 -o sync.out  synchronisation.cpp -lpthread

For executing the program:

    ./sync.out	
    
### Input to the Program

Input to the program is read from the file **input.txt**. The input should be given in the format:

P C &lambda;(P) &lambda;(C) 

- **P:** Number of passenger threads
- **C:** Number of car threads
- **&lambda;(P):** Parameter for exponential wait between 2 successive ride request made by the passenger
- **&lambda;(C):** Parameter for exponential wait between 2 successive ride request accpeted by a car

### Output of the Program 

The output of the program is printed to **output.txt** file. Logs for all the rides are written to this file for each ride taken.



## Documentation

The explanation and report for the whole project can be found in **report.pdf**