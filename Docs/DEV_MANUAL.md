# Developer information

### Required dev tools
- Visual Studio 2022 (or later)
  - Installed workloads:
	- .NET desktop development
	- C++ desktop development  
	  (select "C++/CLI support for v143 build tools" as additional option)
  - Installed extensions:
	- Heatwave for VS 2022

### Functionality conventions
- ElemID of the moving species: 0 (always)
- ElemID of vacancies: 1 (always)
- Atoms in the unit cell (`TStructure`) are sorted by ElemID
- "No doping" is allowed (then additional vacancies are needed)
- "No electric field" is allowed (then iCon just yields mean squared displacement)

### Coding rules
- Exceptions lead to imidiate program termination, therefore only throw exception if unsolvable error occurs
- Do not throw exceptions in UDLL-projects
- Get-Methods of UDLL (that are not based on an item index) should always return a valid value
- UDLL writes detailed error information to cout and returns corresponding error code, GUI only directs user attention to console

### How to advance version numbers  
- Update version number and file compatibility in `GlobalDefinitions.h`
- Update version number in `*.rc` files of the C++ projects
- Update version number in `AssemblyInfo.cpp` of C++/CLI projects
- Update version number in iCon project settings (or `AssemblyInfo.cs`)

### How to handle updates of used NuGet packages
- Update resource strings for "About" window with the new version number (`PackageTitle_...`)
- If the license changed, update the license text in `Resources/ThirdPartyLicenses` folder
- If a package is added or removed, update the `TVMLicenseWindow` constructor accordingly  
  (this also applies to package dependencies, which may change due to updates)