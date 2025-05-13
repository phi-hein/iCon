# Changelog

<!--- 
CHANGELOG STYLE GUIDE

Use the following change categories for each release (in that order):
###Changed : for changes in existing functionality
###Added   : for new functionality
###Removed : for removed functionality
###Fixed   : for bug fixes

Describe changes in imperative, e.g. "modify exception handling for ..."

Prefix breaking changes with **Breaking:** (and list them before other changes)

Syntax for links to issues (or pull requests):
([#1](https://github.com/phi-hein/iCon/issues/1))
--->

## [1.0.0] - May 2025 (_first public release_)

Compatibility of input/output files: 0.9.0.5 and later

### Changed:
- Pre-compiled simulation executables replaced by automatic compiling on the remote system (build script)
- File identifiers translated to english (the german variants are still used for loading older files)
- Upgrade to .NET 9, C++ 17 and C# 13
- Ignore revision number in file compatibility check

### Added:
- Remote cluster access via keyboard-interactive authentication (2FA)
- SSH fingerprint check
- User manual and further documentation

### Removed:
- Restrictive licensing system removed in favor of open-source distribution
- MergeModule of Visual C++ Redistributable removed from Setup, since deprecated by Microsoft  
  (it was decided against a bundle with .NET framework and VC++ redist installers)

### Fixed:
- Simulations with all energies zero no longer run forever
- Remote profile selection when loading ini file does not rely on order of the filenames anymore
- Creating and deleting remote profiles no longer throws exceptions when underlying files are not already present
- Prevent overwriting of existing simulation files in submission process

<!--- List of links to releases: --->
[1.0.0]: https://github.com/phi-hein/iCon/releases/tag/v1.0.0

<!--- List of links to pull requests and issues: --->
<!--- Example: [#1]: https://github.com/phi-hein/iCon/issues/1 --->

# Changelog before migration from Azure DevOps to GitHub (roughly)

## 0.9.4 - February 2018

Compatibility of input/output files: 0.9.0.5 - 0.9.4

### Changed:
- **Breaking**: change TVVShell to TInteraction with custom interaction ID
- set IsSaved to false when KMCDLL changed
- trim strings during validation
- make KMC-GUI a singleton
- C++11 Header <random> instead of GSL

### Added:
- **Breaking**: multiline project description
- **Breaking**: "additive" flag for all environment atoms (no sorting according to active or additive)
- **Breaking**: "no doping" is now possible
- display KMCDLL errors from BWorker
- make a table for all different jobs and add jobs to it (like script version)
- validation of string to int, double binding (empty field = 0)
- global exception handling
- checkpoint file for job stopping and continuing (start job with checkpoint file present)

### Fixed:
- title of B.O.H. Grope is updated to MBA

## 0.9.0.5 - May 2014

Compatibility of input/output files: 0.9.0.5

### Changed:
- dopand and vacancy numbers are derived by rounding instead of truncating the double-type amounts
- increased possible lattice sizes to [1, 65]  
  -> clarification for self-interaction due to periodic boundary conditions: 
     - self-interaction may occur when small lattice size is combined with a small unit cell and/or large interaction radii
     - self-interaction 1: a vacancy will not jump through a periodic boundary on her own position (site blocking)
     - self-interaction 2: a jumping vacancy can simultanously be part of her jump environment and therefore influence her jump like a neighboring, different vacancy

### Fixed:
- file double precision changed to 17 digits (this is the number of mantissa digits required to distinguish double values)
- double output floatfield flag set to "unset" for proper significant digits, double padding adjusted accordingly
- more reliable unique jump and interaction sorting (absolute equality threshold)
- wrong if-condition in TInteraction saving routine

## 0.9.0.4 - May 2014

Compatibility of input/output files: 0.9.0.0 - 0.9.0.4

### Changed:
- JobScript now contains no standard queue specification (is commented out)
- increased maximum number of unit cell atoms to 500
- increased height of info window
- adding jobs now automatically selects the new job in the job list

### Added:
- job dublication

### Fixed:
- TResults now permits loading of negative conductivities (may occur with very small electric fields)
- lattice parameter c now has the correct label "c"
- changed title of Benjamin O.H. Grope from M.Sc. to M.A. in info window

## 0.9.0.3 - April 2014

Compatibility of input/output files: 0.9.0.0 - 0.9.0.3

### Changed:
- loading equality threshold is fractional instead of fixed

### Fixed:
- loading file failed due to 360 degree != 0 degree (360 degree is now set to 0 when calculating jump environments)
- invalid vector index when creating dopand map in MakeUniqueCodes

## 0.9.0.2 - April 2014

Compatibility of input/output files: 0.9.0.0 - 0.9.0.2

### Added:
- added file version compatibility list (enables loading of older files if the file format did not change)
- added iConSearcher (for Windows and Linux): a tool for summarising finished simulations into one csv-file

### Fixed:
- corrected exitcode evaluation in JobScript for cluster simulation

## 0.9.0.1 - April 2014

Compatibility of input/output files: 0.9.0.0 - 0.9.0.1

### Changed:
- changed random number generator initialization from time(NULL)-seed to seed-sequence from std::random_device
- added custom intel compiler lib path to LD_LIBRARY_PATH in JobScript

### Added:
- added version changelog

## 0.9.0 - April 2014

_Initial internal release._
