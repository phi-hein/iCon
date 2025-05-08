# User manual

The basic steps for setting up a KMC simulation with the iCon GUI are described in the [tutorial](https://www.icon.pc.rwth-aachen.de/tutorial.html). The underlying theory of the KMC simulations can be found in our [article](https://doi.org/10.1016/j.matchemphys.2020.123767) and in the [theory section](https://www.icon.pc.rwth-aachen.de/theory.html) of the homepage. Further clarifications are added through the frequent tooltips implemented in the GUI. Here, the focus is on
- [How to execute simulations on a remote cluster](#how-to-execute-simulations-on-a-remote-cluster)
- [How to retrieve the results](#how-to-retrieve-the-results)
- [Advanced usage scenarios](#advanced-usage-scenarios)

## How to execute simulations on a remote cluster
After adding KMC jobs to the list in iCon's "Settings" tab, these jobs can be submitted to a remote queue system. Before hitting the "Submit" button, several preparations are necessary:  
1. Create a remote profile in the "Extended Settings" tab ("+" button)

### Adaptation of the scripts
Single core simulations


## How to retrieve the results
...

### Collect results with the iConSearcher tool

## Advanced usage scenarios
...

### Testing or manual job submission:
- edit the submit script to do nothing but return without error ("exit 0")
- start normal job submission, which will place all files on the remote cluster

### Continue simulations
Checkpoint file (restart with kmc file searches for nearby checkpoint file and uses it to continue, if respective flag is set)

### Dynamic normalization
Set entry to zero to just abort before main simulation and just yield the dyn norm list

### Simulation without doping
Needs additional vacancies