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
include Source/Smoothing/CMakeFiles/MedianFilter.dir/depend.make

# Include the progress variables for this target.
include Source/Smoothing/CMakeFiles/MedianFilter.dir/progress.make

# Include the compile flags for this target's objects.
include Source/Smoothing/CMakeFiles/MedianFilter.dir/flags.make

Source/Smoothing/CMakeFiles/MedianFilter.dir/MedianFilter.o: Source/Smoothing/CMakeFiles/MedianFilter.dir/flags.make
Source/Smoothing/CMakeFiles/MedianFilter.dir/MedianFilter.o: /home/liyun/V3D/2010_hackathon/li_yun/ITK-V3D-Plugins/Source/Smoothing/MedianFilter.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Source/Smoothing/CMakeFiles/MedianFilter.dir/MedianFilter.o"
	cd /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/Smoothing && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/MedianFilter.dir/MedianFilter.o -c /home/liyun/V3D/2010_hackathon/li_yun/ITK-V3D-Plugins/Source/Smoothing/MedianFilter.cxx

Source/Smoothing/CMakeFiles/MedianFilter.dir/MedianFilter.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MedianFilter.dir/MedianFilter.i"
	cd /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/Smoothing && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/liyun/V3D/2010_hackathon/li_yun/ITK-V3D-Plugins/Source/Smoothing/MedianFilter.cxx > CMakeFiles/MedianFilter.dir/MedianFilter.i

Source/Smoothing/CMakeFiles/MedianFilter.dir/MedianFilter.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MedianFilter.dir/MedianFilter.s"
	cd /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/Smoothing && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/liyun/V3D/2010_hackathon/li_yun/ITK-V3D-Plugins/Source/Smoothing/MedianFilter.cxx -o CMakeFiles/MedianFilter.dir/MedianFilter.s

Source/Smoothing/CMakeFiles/MedianFilter.dir/MedianFilter.o.requires:
.PHONY : Source/Smoothing/CMakeFiles/MedianFilter.dir/MedianFilter.o.requires

Source/Smoothing/CMakeFiles/MedianFilter.dir/MedianFilter.o.provides: Source/Smoothing/CMakeFiles/MedianFilter.dir/MedianFilter.o.requires
	$(MAKE) -f Source/Smoothing/CMakeFiles/MedianFilter.dir/build.make Source/Smoothing/CMakeFiles/MedianFilter.dir/MedianFilter.o.provides.build
.PHONY : Source/Smoothing/CMakeFiles/MedianFilter.dir/MedianFilter.o.provides

Source/Smoothing/CMakeFiles/MedianFilter.dir/MedianFilter.o.provides.build: Source/Smoothing/CMakeFiles/MedianFilter.dir/MedianFilter.o
.PHONY : Source/Smoothing/CMakeFiles/MedianFilter.dir/MedianFilter.o.provides.build

Source/Smoothing/CMakeFiles/MedianFilter.dir/moc_MedianFilter.o: Source/Smoothing/CMakeFiles/MedianFilter.dir/flags.make
Source/Smoothing/CMakeFiles/MedianFilter.dir/moc_MedianFilter.o: Source/Smoothing/moc_MedianFilter.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Source/Smoothing/CMakeFiles/MedianFilter.dir/moc_MedianFilter.o"
	cd /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/Smoothing && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/MedianFilter.dir/moc_MedianFilter.o -c /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/Smoothing/moc_MedianFilter.cxx

Source/Smoothing/CMakeFiles/MedianFilter.dir/moc_MedianFilter.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MedianFilter.dir/moc_MedianFilter.i"
	cd /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/Smoothing && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/Smoothing/moc_MedianFilter.cxx > CMakeFiles/MedianFilter.dir/moc_MedianFilter.i

Source/Smoothing/CMakeFiles/MedianFilter.dir/moc_MedianFilter.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MedianFilter.dir/moc_MedianFilter.s"
	cd /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/Smoothing && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/Smoothing/moc_MedianFilter.cxx -o CMakeFiles/MedianFilter.dir/moc_MedianFilter.s

Source/Smoothing/CMakeFiles/MedianFilter.dir/moc_MedianFilter.o.requires:
.PHONY : Source/Smoothing/CMakeFiles/MedianFilter.dir/moc_MedianFilter.o.requires

Source/Smoothing/CMakeFiles/MedianFilter.dir/moc_MedianFilter.o.provides: Source/Smoothing/CMakeFiles/MedianFilter.dir/moc_MedianFilter.o.requires
	$(MAKE) -f Source/Smoothing/CMakeFiles/MedianFilter.dir/build.make Source/Smoothing/CMakeFiles/MedianFilter.dir/moc_MedianFilter.o.provides.build
.PHONY : Source/Smoothing/CMakeFiles/MedianFilter.dir/moc_MedianFilter.o.provides

Source/Smoothing/CMakeFiles/MedianFilter.dir/moc_MedianFilter.o.provides.build: Source/Smoothing/CMakeFiles/MedianFilter.dir/moc_MedianFilter.o
.PHONY : Source/Smoothing/CMakeFiles/MedianFilter.dir/moc_MedianFilter.o.provides.build

Source/Smoothing/moc_MedianFilter.cxx: /home/liyun/V3D/2010_hackathon/li_yun/ITK-V3D-Plugins/Source/Smoothing/MedianFilter.h
	$(CMAKE_COMMAND) -E cmake_progress_report /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating moc_MedianFilter.cxx"
	cd /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/Smoothing && /usr/bin/moc-qt4 -I/home/liyun/V3D/v3d_main/basic_c_fun/../common_lib/include -I/home/liyun/ITK/InsightToolkit-3.20.0/Code/Review/Statistics -I/home/liyun/ITK/InsightToolkit-3.20.0/Code/Review -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities/gdcm/src -I/home/liyun/ITK/ITK-bin/Utilities/gdcm -I/home/liyun/ITK/ITK-bin/Utilities/vxl/core -I/home/liyun/ITK/ITK-bin/Utilities/vxl/vcl -I/home/liyun/ITK/ITK-bin/Utilities/vxl/v3p/netlib -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities/vxl/core -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities/vxl/vcl -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities/vxl/v3p/netlib -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities -I/home/liyun/ITK/ITK-bin/Utilities -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities/itkExtHdrs -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities/nifti/znzlib -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities/nifti/niftilib -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities/expat -I/home/liyun/ITK/ITK-bin/Utilities/expat -I/home/liyun/ITK/ITK-bin/Utilities/DICOMParser -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities/DICOMParser -I/home/liyun/ITK/ITK-bin/Utilities/NrrdIO -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities/NrrdIO -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities/MetaIO -I/home/liyun/ITK/InsightToolkit-3.20.0/Code/SpatialObject -I/home/liyun/ITK/InsightToolkit-3.20.0/Code/Numerics/NeuralNetworks -I/home/liyun/ITK/InsightToolkit-3.20.0/Code/Numerics/FEM -I/home/liyun/ITK/InsightToolkit-3.20.0/Code/IO -I/home/liyun/ITK/InsightToolkit-3.20.0/Code/Numerics -I/home/liyun/ITK/InsightToolkit-3.20.0/Code/Common -I/home/liyun/ITK/InsightToolkit-3.20.0/Code/BasicFilters -I/home/liyun/ITK/InsightToolkit-3.20.0/Code/Algorithms -I/home/liyun/ITK/ITK-bin -I/usr/include/qt4 -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtCore -I/home/liyun/V3D/2010_hackathon/li_yun/ITK-V3D-Plugins/Source/Common -I/home/liyun/V3D/v3d_main/basic_c_fun -I/home/liyun/V3D/2010_hackathon/li_yun/ITK-V3D-Plugins -DQT_GUI_LIB -DQT_CORE_LIB -o /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/Smoothing/moc_MedianFilter.cxx /home/liyun/V3D/2010_hackathon/li_yun/ITK-V3D-Plugins/Source/Smoothing/MedianFilter.h

# Object files for target MedianFilter
MedianFilter_OBJECTS = \
"CMakeFiles/MedianFilter.dir/MedianFilter.o" \
"CMakeFiles/MedianFilter.dir/moc_MedianFilter.o"

# External object files for target MedianFilter
MedianFilter_EXTERNAL_OBJECTS =

bin/libMedianFilter.so: Source/Smoothing/CMakeFiles/MedianFilter.dir/MedianFilter.o
bin/libMedianFilter.so: Source/Smoothing/CMakeFiles/MedianFilter.dir/moc_MedianFilter.o
bin/libMedianFilter.so: bin/libV3DInterface.a
bin/libMedianFilter.so: bin/libV3DITKCommon.a
bin/libMedianFilter.so: /usr/lib/libQtGui.so
bin/libMedianFilter.so: /usr/lib/libQtCore.so
bin/libMedianFilter.so: Source/Smoothing/CMakeFiles/MedianFilter.dir/build.make
bin/libMedianFilter.so: Source/Smoothing/CMakeFiles/MedianFilter.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared library ../../bin/libMedianFilter.so"
	cd /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/Smoothing && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MedianFilter.dir/link.txt --verbose=$(VERBOSE)
	cd /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/Smoothing && /usr/bin/cmake -E copy /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/bin/libMedianFilter.so /home/liyun/V3D/v3d/plugins/ITK//Smoothing/MedianFilter/MedianFilter.so

# Rule to build all files generated by this target.
Source/Smoothing/CMakeFiles/MedianFilter.dir/build: bin/libMedianFilter.so
.PHONY : Source/Smoothing/CMakeFiles/MedianFilter.dir/build

Source/Smoothing/CMakeFiles/MedianFilter.dir/requires: Source/Smoothing/CMakeFiles/MedianFilter.dir/MedianFilter.o.requires
Source/Smoothing/CMakeFiles/MedianFilter.dir/requires: Source/Smoothing/CMakeFiles/MedianFilter.dir/moc_MedianFilter.o.requires
.PHONY : Source/Smoothing/CMakeFiles/MedianFilter.dir/requires

Source/Smoothing/CMakeFiles/MedianFilter.dir/clean:
	cd /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/Smoothing && $(CMAKE_COMMAND) -P CMakeFiles/MedianFilter.dir/cmake_clean.cmake
.PHONY : Source/Smoothing/CMakeFiles/MedianFilter.dir/clean

Source/Smoothing/CMakeFiles/MedianFilter.dir/depend: Source/Smoothing/moc_MedianFilter.cxx
	cd /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/liyun/V3D/2010_hackathon/li_yun/ITK-V3D-Plugins /home/liyun/V3D/2010_hackathon/li_yun/ITK-V3D-Plugins/Source/Smoothing /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/Smoothing /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/Smoothing/CMakeFiles/MedianFilter.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Source/Smoothing/CMakeFiles/MedianFilter.dir/depend

