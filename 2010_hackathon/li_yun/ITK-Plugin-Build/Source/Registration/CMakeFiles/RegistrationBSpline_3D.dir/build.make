# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/liyun/V3D/2010_hackathon/li_yun/ITK-V3D-Plugins

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build

# Include any dependencies generated for this target.
include Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/depend.make

# Include the progress variables for this target.
include Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/progress.make

# Include the compile flags for this target's objects.
include Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/flags.make

Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/RegistrationBSpline_3D.o: Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/flags.make
Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/RegistrationBSpline_3D.o: /home/liyun/V3D/2010_hackathon/li_yun/ITK-V3D-Plugins/Source/Registration/RegistrationBSpline_3D.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/RegistrationBSpline_3D.o"
	cd /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/Registration && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/RegistrationBSpline_3D.dir/RegistrationBSpline_3D.o -c /home/liyun/V3D/2010_hackathon/li_yun/ITK-V3D-Plugins/Source/Registration/RegistrationBSpline_3D.cxx

Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/RegistrationBSpline_3D.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RegistrationBSpline_3D.dir/RegistrationBSpline_3D.i"
	cd /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/Registration && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/liyun/V3D/2010_hackathon/li_yun/ITK-V3D-Plugins/Source/Registration/RegistrationBSpline_3D.cxx > CMakeFiles/RegistrationBSpline_3D.dir/RegistrationBSpline_3D.i

Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/RegistrationBSpline_3D.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RegistrationBSpline_3D.dir/RegistrationBSpline_3D.s"
	cd /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/Registration && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/liyun/V3D/2010_hackathon/li_yun/ITK-V3D-Plugins/Source/Registration/RegistrationBSpline_3D.cxx -o CMakeFiles/RegistrationBSpline_3D.dir/RegistrationBSpline_3D.s

Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/RegistrationBSpline_3D.o.requires:
.PHONY : Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/RegistrationBSpline_3D.o.requires

Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/RegistrationBSpline_3D.o.provides: Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/RegistrationBSpline_3D.o.requires
	$(MAKE) -f Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/build.make Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/RegistrationBSpline_3D.o.provides.build
.PHONY : Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/RegistrationBSpline_3D.o.provides

Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/RegistrationBSpline_3D.o.provides.build: Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/RegistrationBSpline_3D.o
.PHONY : Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/RegistrationBSpline_3D.o.provides.build

Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/moc_RegistrationBSpline_3D.o: Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/flags.make
Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/moc_RegistrationBSpline_3D.o: Source/Registration/moc_RegistrationBSpline_3D.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/moc_RegistrationBSpline_3D.o"
	cd /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/Registration && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/RegistrationBSpline_3D.dir/moc_RegistrationBSpline_3D.o -c /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/Registration/moc_RegistrationBSpline_3D.cxx

Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/moc_RegistrationBSpline_3D.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RegistrationBSpline_3D.dir/moc_RegistrationBSpline_3D.i"
	cd /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/Registration && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/Registration/moc_RegistrationBSpline_3D.cxx > CMakeFiles/RegistrationBSpline_3D.dir/moc_RegistrationBSpline_3D.i

Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/moc_RegistrationBSpline_3D.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RegistrationBSpline_3D.dir/moc_RegistrationBSpline_3D.s"
	cd /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/Registration && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/Registration/moc_RegistrationBSpline_3D.cxx -o CMakeFiles/RegistrationBSpline_3D.dir/moc_RegistrationBSpline_3D.s

Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/moc_RegistrationBSpline_3D.o.requires:
.PHONY : Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/moc_RegistrationBSpline_3D.o.requires

Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/moc_RegistrationBSpline_3D.o.provides: Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/moc_RegistrationBSpline_3D.o.requires
	$(MAKE) -f Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/build.make Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/moc_RegistrationBSpline_3D.o.provides.build
.PHONY : Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/moc_RegistrationBSpline_3D.o.provides

Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/moc_RegistrationBSpline_3D.o.provides.build: Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/moc_RegistrationBSpline_3D.o
.PHONY : Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/moc_RegistrationBSpline_3D.o.provides.build

Source/Registration/moc_RegistrationBSpline_3D.cxx: /home/liyun/V3D/2010_hackathon/li_yun/ITK-V3D-Plugins/Source/Registration/RegistrationBSpline_3D.h
	$(CMAKE_COMMAND) -E cmake_progress_report /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating moc_RegistrationBSpline_3D.cxx"
	cd /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/Registration && /usr/bin/moc-qt4 -I/home/liyun/V3D/v3d_main/basic_c_fun/../common_lib/include -I/home/liyun/ITK/InsightToolkit-3.20.0/Code/Review/Statistics -I/home/liyun/ITK/InsightToolkit-3.20.0/Code/Review -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities/gdcm/src -I/home/liyun/ITK/ITK-bin/Utilities/gdcm -I/home/liyun/ITK/ITK-bin/Utilities/vxl/core -I/home/liyun/ITK/ITK-bin/Utilities/vxl/vcl -I/home/liyun/ITK/ITK-bin/Utilities/vxl/v3p/netlib -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities/vxl/core -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities/vxl/vcl -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities/vxl/v3p/netlib -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities -I/home/liyun/ITK/ITK-bin/Utilities -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities/itkExtHdrs -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities/nifti/znzlib -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities/nifti/niftilib -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities/expat -I/home/liyun/ITK/ITK-bin/Utilities/expat -I/home/liyun/ITK/ITK-bin/Utilities/DICOMParser -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities/DICOMParser -I/home/liyun/ITK/ITK-bin/Utilities/NrrdIO -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities/NrrdIO -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities/MetaIO -I/home/liyun/ITK/InsightToolkit-3.20.0/Code/SpatialObject -I/home/liyun/ITK/InsightToolkit-3.20.0/Code/Numerics/NeuralNetworks -I/home/liyun/ITK/InsightToolkit-3.20.0/Code/Numerics/FEM -I/home/liyun/ITK/InsightToolkit-3.20.0/Code/IO -I/home/liyun/ITK/InsightToolkit-3.20.0/Code/Numerics -I/home/liyun/ITK/InsightToolkit-3.20.0/Code/Common -I/home/liyun/ITK/InsightToolkit-3.20.0/Code/BasicFilters -I/home/liyun/ITK/InsightToolkit-3.20.0/Code/Algorithms -I/home/liyun/ITK/ITK-bin -I/usr/include/qt4 -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtCore -I/home/liyun/V3D/2010_hackathon/li_yun/ITK-V3D-Plugins/Source/Common -I/home/liyun/V3D/v3d_main/basic_c_fun -I/home/liyun/V3D/2010_hackathon/li_yun/ITK-V3D-Plugins -DQT_GUI_LIB -DQT_CORE_LIB -o /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/Registration/moc_RegistrationBSpline_3D.cxx /home/liyun/V3D/2010_hackathon/li_yun/ITK-V3D-Plugins/Source/Registration/RegistrationBSpline_3D.h

# Object files for target RegistrationBSpline_3D
RegistrationBSpline_3D_OBJECTS = \
"CMakeFiles/RegistrationBSpline_3D.dir/RegistrationBSpline_3D.o" \
"CMakeFiles/RegistrationBSpline_3D.dir/moc_RegistrationBSpline_3D.o"

# External object files for target RegistrationBSpline_3D
RegistrationBSpline_3D_EXTERNAL_OBJECTS =

bin/libRegistrationBSpline_3D.so: Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/RegistrationBSpline_3D.o
bin/libRegistrationBSpline_3D.so: Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/moc_RegistrationBSpline_3D.o
bin/libRegistrationBSpline_3D.so: bin/libV3DInterface.a
bin/libRegistrationBSpline_3D.so: bin/libV3DITKCommon.a
bin/libRegistrationBSpline_3D.so: /usr/lib/libQtGui.so
bin/libRegistrationBSpline_3D.so: /usr/lib/libQtCore.so
bin/libRegistrationBSpline_3D.so: Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/build.make
bin/libRegistrationBSpline_3D.so: Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared library ../../bin/libRegistrationBSpline_3D.so"
	cd /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/Registration && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/RegistrationBSpline_3D.dir/link.txt --verbose=$(VERBOSE)
	cd /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/Registration && /usr/bin/cmake -E copy /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/bin/libRegistrationBSpline_3D.so /home/liyun/V3D/v3d/plugins/ITK//Registration/RegistrationBSpline_3D/RegistrationBSpline_3D.so

# Rule to build all files generated by this target.
Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/build: bin/libRegistrationBSpline_3D.so
.PHONY : Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/build

Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/requires: Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/RegistrationBSpline_3D.o.requires
Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/requires: Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/moc_RegistrationBSpline_3D.o.requires
.PHONY : Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/requires

Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/clean:
	cd /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/Registration && $(CMAKE_COMMAND) -P CMakeFiles/RegistrationBSpline_3D.dir/cmake_clean.cmake
.PHONY : Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/clean

Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/depend: Source/Registration/moc_RegistrationBSpline_3D.cxx
	cd /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/liyun/V3D/2010_hackathon/li_yun/ITK-V3D-Plugins /home/liyun/V3D/2010_hackathon/li_yun/ITK-V3D-Plugins/Source/Registration /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/Registration /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Source/Registration/CMakeFiles/RegistrationBSpline_3D.dir/depend

