# User manual

The basic steps for setting up a KMC simulation with the iCon GUI are described in the [tutorial](https://www.icon.pc.rwth-aachen.de/tutorial.html). The underlying theory of the KMC simulations can be found in our [article](https://doi.org/10.1016/j.matchemphys.2020.123767) and in the [theory section](https://www.icon.pc.rwth-aachen.de/theory.html) of the homepage. Further clarifications are added through the frequent tooltips implemented in the GUI. Here, the focus is on
- [How to execute simulations on a remote cluster](#how-to-execute-simulations-on-a-remote-cluster)
- [How to retrieve the results](#how-to-retrieve-the-results)
- [Advanced usage scenarios](#advanced-usage-scenarios)

## How to execute simulations on a remote cluster
After adding KMC jobs to the list in iCon's `Job Settings` tab, these jobs can be submitted to a remote queue system. Before hitting the `Submit` button, several preparations are necessary, as will be described further [below](#preparations-for-remote-job-submission). 

### Automatic remote submission process of iCon:  
For a proper understanding of the required settings, please see how iCon submits remote calculations:
- Connects to the remote cluster via SSH and SFTP. This requires to authenticate twice, which can be accelerated by using private key authentication.
- Checks whether the required `iConSimulator.exe` and `iConSearcher.exe` are present and up-to-date. If not, then their source code is uploaded to the specified `Build directory` and they are compiled by executing the specified `Build script`.
- Copies the executables to the `Base directory` of the jobs.
- Places the specified `Submit script` and `Job script` in the `Base directory`.
- For each KMC job in the job list:
	- Creates an individual job directory (`Job prefix` + `ID`) in the `Base directory`.
	- Places the input file of the job (*.kmc) in the job directory.
	- Executes the `Submit script`, which submits the `Job script` to the remote queue system. The `Job script` carries out the simulation with the `iConSimulator.exe`.

### Preparations for remote job submission:
Create a remote profile in the `Extended Settings` tab ("+" button, see tooltips for further explanations):
1. Enter a `Name` for the profile, under which it will be saved for later use.
2. Set the `Workspace` directory on the remote system, where the iCon simulations shall be stored.
3. Set the `Build directory`, relative to the workspace directory, where the executables will be created (or use the default "build").
4. Specify the authentication settings for the remote cluster (host name, port, user name, etc.).
5. Adjust the `Submit script` for the respective remote queue system (can be reset by `Right click` -> `Load default script`):
	- Replace `sbatch` by the appropriate submission command.
	- Adjust the job settings (like memory requirements, time limit, etc.). Note that iCon's KMC jobs are single-core simulations.
	- Do not change the overall functionality! The parameters `$1`, `$2` and `$5` must be passed to the job script. The script must return the exitcode of the submission command.
6. Adjust the `Job script`, which is submitted to the remote queue (can be reset by `Right click` -> `Load default script`):
	- Replace the SLURM-specific environment variables by those of the targeted queue system.
	- Do not change the overall functionality! The line `$1 $2 >> $3` executes the simulation. The script must return the exitcode of the simulation.
7. Adjust the `Build script`, which compiles the remote executables (can be reset by `Right click` -> `Load default script`):
	- This script typically requires no or just minor modifications. Be careful to not change the functionality!
	- If required on the remote system: Add commands to enable a C++ compiler (e.g. load modules).
	- For using a different C++ compiler than `g++`: Overwrite the CXX variable by changing the line `make &> build.log` into for example `make CXX=icpc &> build.log` (for the `icpc` compiler).
	- If desired: Add compiler or linker options by setting environment variables with `export CXXFLAGS="..."` or `export LDFLAGS="..."` (e.g. `-march=sandybridge` for a specific CPU). Note that the compilation always uses `-O2` for optimization.

Set the required options in the `Job Settings` tab:
1. Select `Cluster` as submission type.
2. Enter the `Job prefix` for the individual job directories (or use the default "Job").
3. Specify the `Base directory` (typically relative to the remote workspace), where the job directories will be created. If this directory already contains previous simulations, then it is important to adjust the `ID` of each new KMC jobs in the job list to an unused value.

Now the submission process can be started by clicking `Submit Jobs` !

## How to retrieve the results
The `iConSimulator.exe` reads the KMC input file (*.kmc), carries out the simulation and then writes the results to the same file.
This ensures that the results are not separated from the input parameters.
Additional information about the course of the simulation is written to a nearby log-file.
After the simulation, the KMC file contains a new element `<Results>`, which contains the resulting quantities as described [here](FILE_SPEC.md).
In order to facilitate the collection of the results from a large number of KMC jobs, the `iConSearcher.exe` is provided in the base directory of the jobs.
This tool searches all *.kmc files in the current working directory and its sub-directories, and collects their results in a `Summary.csv` file in the current working directory.
Use it by navigating to the base directory with `cd ...` and by calling `./iConSearcher.exe` (in a remote shell, e.g. PuTTY).

## Advanced usage scenarios
Use `-help` to get further info on how to use `iConSimulator.exe` and `iConSearcher.exe`.

### No doping or no electric field
If no doping is specified in the `Structure` tab, then `Additional vacancies` must be added in the `Job Settings` tab.
This allows to simulate pure compounds with a defined number of vacancies.

If no electric field is specified in the `Job Settings` tab, then the simulations only yield the movement statistics (e.g. mean squared displacements), but no conductivity.

### Dynamic normalization
The "dynamic normalization" feature allows to record the probabilities from a certain number of jump attempts and set the normalization based on the resulting probability list (see tooltips in `Job Settings` and our research article for further info).
Without prior experience, it can be challenging to set the number of jump attempts, the number of recorded probabilities and the selected list entry.
By setting `Norm. Prob.-Entry` to "0" in `Job Settings`, it is possible to carry out the simulation just until the end of the "dynamic normalization" step.
Then the probability list in the log-file can be examined to find a suitable normalization entry that does not invalidate a significant number of jumps.
Afterwards, either the simulation can be restarted after editing the `DynNorm-ListEntry` in the KMC file from "0" to the selected entry number, or new simulations can be started with this entry number.

### How to continue simulations
If saving and loading of checkpoint files is activated in the `Job Settings` tab, then the simulations can be continued, for example after being killed by the queue system due to exceeding the time limit.
The `iConSimulator.exe` regularly saves the current simulation state to a checkpoint file (*.mcp, see [here](FILE_SPEC.md) for a description of its contents).
If the simulation was aborted before it finished, it can be continued by re-submitting the same job (for example by manually calling the `Submit script` with the appropriate parameters).
When the simulation is started again, the `iConSimulator.exe` searches for a nearby checkpoint file and uses it to continue from the saved state.

### Testing or manual job submission
If it is desired to just place the KMC files on a remote system, without submitting the simulations automatically, the `Submit script` can be edited to do nothing except returning without error (`exit 0`).
