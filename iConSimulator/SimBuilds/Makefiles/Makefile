# Makefile for iConSimulator for Linux
# Version: 0.9.0.5
# -- Property of Workgroup Martin, RWTH Aachen University --
# Developer: Philipp Hein

 

# -------------- Makro-Definitions --------------

# Executable base name
export EXENAME_BASE = iConSimulator_Lin

# Directories
export SOURCEDIR = src
export BUILDDIR = build
export INTERNALDIRS = TKMCJob \
					  TElements \
					  TInteraction \
					  TJump \
					  TJumps \
					  TResults \
					  TSettings \
					  TSimulation \
					  TStructure \
					  TUniqueJump \
					  TUniqueJumps


# -------------- FILE SETUP --------------

# Find all sub-makefiles
ALL_MF_FILES = $(sort $(wildcard *.mf))


# -------------- COMPILATION TARGETS --------------

# Initial makefile target
.PHONY: all
all: $(ALL_MF_FILES)

# Invoke sub-makefiles
.PHONY: $(ALL_MF_FILES)
$(ALL_MF_FILES):
	$(MAKE) -f $@


# -------------- ADDITIONAL MAKE COMMANDS --------------

.PHONY: clean
clean:
	for mkfile in $(ALL_MF_FILES); do \
	  $(MAKE) -f $$mkfile clean; \
	done


# End of Makefile
