# Makefile for iConSimulator (remote compilation)
# -- Property of Workgroup Martin, RWTH Aachen University --
# Developer: Philipp Hein


# -------------- Makro-Definitions --------------

# Executable names
EXENAME_SIM = iConSimulator
EXENAME_SEARCH = iConSearcher

# Directories
SOURCEDIR = src
BUILDDIR = bin
INTERNALDIRS = TKMCJob \
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

# Compiler
CXX = g++

# Compiler flags
ALL_CXXFLAGS = $(CXXFLAGS) -std=c++17 -Wall -O2 -I $(SOURCEDIR) -D "NON_DLL_BUILD"

# Linker flags
ALL_LDFLAGS = $(LDFLAGS) -O2 -static-libgcc -static-libstdc++


# -------------- FILE SETUP --------------

# Combined directories
ALL_SOURCEDIRS = $(SOURCEDIR) $(addprefix $(SOURCEDIR)/,$(INTERNALDIRS))
ALL_BUILDDIRS = $(BUILDDIR) $(addprefix $(BUILDDIR)/,$(INTERNALDIRS))

# Find all source files
ALL_CPP_FILES = $(sort $(foreach dir_name,$(ALL_SOURCEDIRS),$(wildcard $(dir_name)/*.cpp)))

# Create corresponding objects file paths
ALL_OBJ_FILES = $(patsubst $(SOURCEDIR)/%.cpp,$(BUILDDIR)/%.o,$(ALL_CPP_FILES))

# Create corresponding dependency file paths
ALL_DEP_FILES = $(patsubst $(SOURCEDIR)/%.cpp,$(BUILDDIR)/%.d,$(ALL_CPP_FILES))

# Create list of shared object file paths
SHARED_OBJ_FILES = $(filter-out $(BUILDDIR)/$(EXENAME_SIM).o $(BUILDDIR)/$(EXENAME_SEARCH).o,$(ALL_OBJ_FILES))


# -------------- COMPILATION TARGETS --------------

# Initial makefile target
.PHONY: all
all: create_dirs $(EXENAME_SIM).exe $(EXENAME_SEARCH).exe

# Create build directories (if not present)
.PHONY: create_dirs
create_dirs:
	mkdir -p $(ALL_BUILDDIRS)

# Link obj-files to the simulation executable
$(EXENAME_SIM).exe: $(SHARED_OBJ_FILES) $(BUILDDIR)/$(EXENAME_SIM).o
	$(CXX) $(ALL_LDFLAGS) $^ -o $@

# Link obj-files to the searcher executable
$(EXENAME_SEARCH).exe: $(SHARED_OBJ_FILES) $(BUILDDIR)/$(EXENAME_SEARCH).o
	$(CXX) $(ALL_LDFLAGS) $^ -o $@

# Compile all cpp-files to obj-files (and create dependency files)
$(ALL_OBJ_FILES): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.cpp
	$(CXX) $(ALL_CXXFLAGS) -c $< -o $@ -MMD -MP -MF $(BUILDDIR)/$*.d

# Include the dependency files
-include $(ALL_DEP_FILES)


# -------------- ADDITIONAL MAKE COMMANDS --------------

.PHONY: clean
clean:
	rm -f $(EXENAME_SIM).exe $(EXENAME_SEARCH).exe $(ALL_BUILDDIRS)


# End of Makefile
