<h1 align="center">Traffic Lights</h1>

<img alt="Creative Commons Licence" style="border-width:0" src="docs/logo.png" height="20"/>  <img src="https://img.shields.io/badge/license-Mit%20-blue" alt="license"/>

<p align="center">
  <img height="300" src="docs/logo.png" alt="logo">
</p>

---

This is a simple traffic light system done as a project for the [egFWD](https://egfwd.com/) track [Professional Embedded Systems](https://egfwd.com/specializtion/professional-embedded-systems/) in partnership with [Udacity](https://www.udacity.com/) and [Sprints](https://sprints.ai/).

---

## Directory Structure

1. [`code`](code/): Contains the source code for the application.
2. [`screenshots`](screenshots/): Contains the screenshots of the application header files of the application.
3. [`recordings`](recordings/): Contains the recordings of testing the application.

---

## How to run the application

**To run drivers testing**:

1. In [code\APP\driverTest.c](code/APP/driverTest.c), uncomment only one line of the required test in main function
2. Open the [`code.atsln`](code/code.atsln) using Microchip Studio
3. Build the project
4. Then run the simulation on either Microchip Studio, or Proteus simulation
   1. To simulate on Proteus, open the [`simulation/simulation.pdsprj`](simulation/simulation.pdsprj) file using Proteus, then Run the simulation
5. Repeat from step 1 to test other drivers

**To run application**:

1. Open the [`code.atsln`](code/code.atsln) using Microchip Studio
2. Build the project
3. Simulate the system on proteus. Open the [`simulation/simulation.pdsprj`](simulation/simulation.pdsprj) file using Proteus, then Run the simulation

**Thanks**