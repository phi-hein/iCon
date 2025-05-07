# User manual

The basic steps for setting up a KMC simulation with the iCon GUI are described in the [tutorial](https://www.icon.pc.rwth-aachen.de/tutorial.html). The underlying theory of the KMC simulations can be found in our [article](https://doi.org/10.1016/j.matchemphys.2020.123767) and in the [theory section](https://www.icon.pc.rwth-aachen.de/theory.html) of the homepage. Further clarifications are added through the frequent tooltips implemented in the GUI. Here, the focus is on
- [How to execute the simulations](#how-to-execute-the-simulations)
- [How to retrieve the results](#how-to-retrieve-the-results)
- [Advanced usage scenarios](#advanced-usage-scenarios)

## How to execute the simulations
...

### Adaptation of the scripts

### For testing or manual job submission:
- edit the submit script to do nothing but return without error ("exit 0")
- start normal job submission, which will place all files on the remote cluster

## How to retrieve the results
...

### Collect results with the iConSearcher tool

## Advanced usage scenarios
...

### Continue simulations
Checkpoint file (restart with kmc file searches for nearby checkpoint file and uses it to continue, if respective flag is set)

### Dynamic normalization
Set entry to zero to just abort before main simulation and just yield the dyn norm list

### Simulation without doping
Needs additional vacancies