# File specification

The primary files of iCon have the extension `*.kmc`.
They serve as input files and also as output files, as described in the [user manual](USER_MANUAL.md).
The input quantities are not documented here, because they are rather obvious from the GUI input fields (see `GlobalDefinitions.h` within `Source.zip` in the iCon directory for the definition of the identifiers).

## Results in the output file (*.kmc)

The following simulation results are written between `<Results>` and `</Results>`:
- `Conductivity`: Ionic conductivity in S/cm, as calculated from the mean displacement parallel to the electric field.
- `SimulatedTimespan`: Simulated timespan in s.
- `MCSP`: Monte-Carlo steps per particle of the moving species.
- `JumpAttempts`: Total number of jump attempts.
- `UndefinedEndPosition(dE_backward<=0)`: Number of rejected jumps with an undefined destination position ("nonsense jumps", count as jump attempts).
- `UndefinedStartPosition(dE_forward<=0)`: Number of accepted jumps with an undefined start position ("overkill jumps", count not as Monte-Carlo steps and not as jump attempts).
- `Site-Blocking`: Number of blocked jumps (due to vacancy on destination position, count as jump attempts).
- `Normalization`: Normalization factor.
- `Runtime`: Total runtime of the KMC simulation.
- Movement statistics of the moving species:
	- `Mov-<JumpCount>`: Average number of jumps.
	- `Mov-(<x>,<y>,<z>)`: Mean displacement vector in cm.
	- `Mov-<ParallelDisplacement>`: Mean displacement parallel to the electric field in cm.
	- `Mov-<PerpendicularDisplacement>`: Mean displacement perpendicular to the electric field in cm.
	- `Mov-<sqrt(x*x+y*y+z*z)>`: Mean displacement in cm (= average displacement distance).
	- `Mov-<x*x+y*y+z*z>`: Mean squared displacement in cm^2^.
	- `Mov-(sum(x),sum(y),sum(z))`: Center of gravity displacement vector in cm (= sum of the individual displacement vectors).
- Movement statistics of the vacancies:
	- `Vac-<JumpCount>`: Average number of jumps.
	- `Vac-(<x>,<y>,<z>)`: Mean displacement vector in cm.
	- `Vac-<ParallelDisplacement>`: Mean displacement parallel to the electric field in cm.
	- `Vac-<PerpendicularDisplacement>`: Mean displacement perpendicular to the electric field in cm.
	- `Vac-<sqrt(x*x+y*y+z*z)>`: Mean displacement in cm (= average displacement distance).
	- `Vac-<x*x+y*y+z*z>`: Mean squared displacement in cm^2^.
	- `Vac-(sum(x),sum(y),sum(z))`: Center of gravity displacement vector in cm (= sum of the individual displacement vectors).

## Checkpoint file (*.mcp)

The checkpoint file contains the initial lattice and the current (or final) information of the three simulation phases ("Pre-Run", "Dyn. Norm." and "Main").
The main purpose is to enable long-running simulations that exceed the usual time limits of remote queue systems.
In rare cases, one might also analyze the checkpoint files, for example to compare initial and final lattice.

The checkpoint file is structured into four sections:
- `<InitialLattice>` to `</InitialLattice>`: Initial placement of the atoms and vacancies. Each line corresponds to the list of `ElemID` for one unit cell.
- `<Prerun-Phase>` to `</Prerun-Phase>`: Information about the "Pre-Run" phase (see below).
- `<DynNorm-Phase>` to `</DynNorm-Phase>`: Information about the "Dyn. Norm." phase (see below).
- `<Main-Phase>` to `</Main-Phase>`: Information about the "Main" phase (see below).

Each phase section contains the following quantities:
- `MCSP`: Executed Monte-Carlo steps per particle in this phase (so far).
- `JumpAttempts`: Number of jump attempts in this phase (so far).
- `UndefinedEndPosition(dE_backward<=0)`: Number of "nonsense jumps" in this phase (so far).
- `UndefinedStartPosition(dE_forward<=0)`: Number of "overkill jumps" in this phase (so far).
- `Site-Blocking`: Number of blocked jumps in this phase (so far).
- `Normalization`: Normalization factor used in this phase.
- `Runtime`: Runtime of the simulation phase (so far).
- `Attempts-Paths-Ratio`: Cumulative ratio between the jump attempts and the number of jump directions (for time calculation).
- `<CurrentLattice>` to `</CurrentLattice>`: Current (or final) placement of the atoms and vacancies (same format as initial lattice).
- `<AtomMovements>` to `</AtomMovements>`: List of the individual movement of each mobile particle (atoms + vacancies, in the order in which they occur in the current lattice). Each line contains the particle's number of jumps and its displacement in x-, y- and z-direction.
- `<AttemptProbabilities>` to `</AttemptProbabilities>`: Sorted list of jump attempt probabilities (if `Recorded Prob.` > 0 in the job settings). Each line contains the probability, followed by the number of respective jump attempts.
- `<JumpProbabilities>` to `</JumpProbabilities>`: Sorted list of Monte-Carlo step probabilities (if `Recorded Prob.` > 0 in the job settings). Each line contains the probability, followed by the number of respective jumps.