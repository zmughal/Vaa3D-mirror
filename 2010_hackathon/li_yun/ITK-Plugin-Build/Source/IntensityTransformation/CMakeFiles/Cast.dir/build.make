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
include Source/IntensityTransformation/CMakeFiles/Cast.dir/depend.make

# Include the progress variables for this target.
include Source/IntensityTransformation/CMakeFiles/Cast.dir/progress.make

# Include the compile flags for this target's objects.
include Source/IntensityTransformation/CMakeFiles/Cast.dir/flags.make

Source/IntensityTransformation/CMakeFiles/Cast.dir/Cast.o: Source/IntensityTransformation/CMakeFiles/Cast.dir/flags.make
Source/IntensityTransformation/CMakeFiles/Cast.dir/Cast.o: /home/liyun/V3D/2010_hackathon/li_yun/ITK-V3D-Plugins/Source/IntensityTransformation/Cast.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Source/IntensityTransformation/CMakeFiles/Cast.dir/Cast.o"
	cd /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/IntensityTransformation && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Cast.dir/Cast.o -c /home/liyun/V3D/2010_hackathon/li_yun/ITK-V3D-Plugins/Source/IntensityTransformation/Cast.cxx

Source/IntensityTransformation/CMakeFiles/Cast.dir/Cast.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Cast.dir/Cast.i"
	cd /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/IntensityTransformation && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/liyun/V3D/2010_hackathon/li_yun/ITK-V3D-Plugins/Source/IntensityTransformation/Cast.cxx > CMakeFiles/Cast.dir/Cast.i

Source/IntensityTransformation/CMakeFiles/Cast.dir/Cast.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Cast.dir/Cast.s"
	cd /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/IntensityTransformation && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/liyun/V3D/2010_hackathon/li_yun/ITK-V3D-Plugins/Source/IntensityTransformation/Cast.cxx -o CMakeFiles/Cast.dir/Cast.s

Source/IntensityTransformation/CMakeFiles/Cast.dir/Cast.o.requires:
.PHONY : Source/IntensityTransformation/CMakeFiles/Cast.dir/Cast.o.requires

Source/IntensityTransformation/CMakeFiles/Cast.dir/Cast.o.provides: Source/IntensityTransformation/CMakeFiles/Cast.dir/Cast.o.requires
	$(MAKE) -f Source/IntensityTransformation/CMakeFiles/Cast.dir/build.make Source/IntensityTransformation/CMakeFiles/Cast.dir/Cast.o.provides.build
.PHONY : Source/IntensityTransformation/CMakeFiles/Cast.dir/Cast.o.provides

Source/IntensityTransformation/CMakeFiles/Cast.dir/Cast.o.provides.build: Source/IntensityTransformation/CMakeFiles/Cast.dir/Cast.o
.PHONY : Source/IntensityTransformation/CMakeFiles/Cast.dir/Cast.o.provides.build

Source/IntensityTransformation/CMakeFiles/Cast.dir/moc_Cast.o: Source/IntensityTransformation/CMakeFiles/Cast.dir/flags.make
Source/IntensityTransformation/CMakeFiles/Cast.dir/moc_Cast.o: Source/IntensityTransformation/moc_Cast.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Source/IntensityTransformation/CMakeFiles/Cast.dir/moc_Cast.o"
	cd /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/IntensityTransformation && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Cast.dir/moc_Cast.o -c /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/IntensityTransformation/moc_Cast.cxx

Source/IntensityTransformation/CMakeFiles/Cast.dir/moc_Cast.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Cast.dir/moc_Cast.i"
	cd /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/IntensityTransformation && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/IntensityTransformation/moc_Cast.cxx > CMakeFiles/Cast.dir/moc_Cast.i

Source/IntensityTransformation/CMakeFiles/Cast.dir/moc_Cast.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Cast.dir/moc_Cast.s"
	cd /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/IntensityTransformation && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/IntensityTransformation/moc_Cast.cxx -o CMakeFiles/Cast.dir/moc_Cast.s

Source/IntensityTransformation/CMakeFiles/Cast.dir/moc_Cast.o.requires:
.PHONY : Source/IntensityTransformation/CMakeFiles/Cast.dir/moc_Cast.o.requires

Source/IntensityTransformation/CMakeFiles/Cast.dir/moc_Cast.o.provides: Source/IntensityTransformation/CMakeFiles/Cast.dir/moc_Cast.o.requires
	$(MAKE) -f Source/IntensityTransformation/CMakeFiles/Cast.dir/build.make Source/IntensityTransformation/CMakeFiles/Cast.dir/moc_Cast.o.provides.build
.PHONY : Source/IntensityTransformation/CMakeFiles/Cast.dir/moc_Cast.o.provides

Source/IntensityTransformation/CMakeFiles/Cast.dir/moc_Cast.o.provides.build: Source/IntensityTransformation/CMakeFiles/Cast.dir/moc_Cast.o
.PHONY : Source/IntensityTransformation/CMakeFiles/Cast.dir/moc_Cast.o.provides.build

Source/IntensityTransformation/moc_Cast.cxx: /home/liyun/V3D/2010_hackathon/li_yun/ITK-V3D-Plugins/Source/IntensityTransformation/Cast.h
	$(CMAKE_COMMAND) -E cmake_progress_report /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating moc_Cast.cxx"
	cd /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/IntensityTransformation && /usr/bin/moc-qt4 -I/home/liyun/V3D/v3d_main/basic_c_fun/../common_lib/include -I/home/liyun/ITK/InsightToolkit-3.20.0/Code/Review/Statistics -I/home/liyun/ITK/InsightToolkit-3.20.0/Code/Review -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities/gdcm/src -I/home/liyun/ITK/ITK-bin/Utilities/gdcm -I/home/liyun/ITK/ITK-bin/Utilities/vxl/core -I/home/liyun/ITK/ITK-bin/Utilities/vxl/vcl -I/home/liyun/ITK/ITK-bin/Utilities/vxl/v3p/netlib -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities/vxl/core -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities/vxl/vcl -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities/vxl/v3p/netlib -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities -I/home/liyun/ITK/ITK-bin/Utilities -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities/itkExtHdrs -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities/nifti/znzlib -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities/nifti/niftilib -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities/expat -I/home/liyun/ITK/ITK-bin/Utilities/expat -I/home/liyun/ITK/ITK-bin/Utilities/DICOMParser -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities/DICOMParser -I/home/liyun/ITK/ITK-bin/Utilities/NrrdIO -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities/NrrdIO -I/home/liyun/ITK/InsightToolkit-3.20.0/Utilities/MetaIO -I/home/liyun/ITK/InsightToolkit-3.20.0/Code/SpatialObject -I/home/liyun/ITK/InsightToolkit-3.20.0/Code/Numerics/NeuralNetworks -I/home/liyun/ITK/InsightToolkit-3.20.0/Code/Numerics/FEM -I/home/liyun/ITK/InsightToolkit-3.20.0/Code/IO -I/home/liyun/ITK/InsightToolkit-3.20.0/Code/Numerics -I/home/liyun/ITK/InsightToolkit-3.20.0/Code/Common -I/home/liyun/ITK/InsightToolkit-3.20.0/Code/BasicFilters -I/home/liyun/ITK/InsightToolkit-3.20.0/Code/Algorithms -I/home/liyun/ITK/ITK-bin -I/usr/include/qt4 -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtCore -I/home/liyun/V3D/2010_hackathon/li_yun/ITK-V3D-Plugins/Source/Common -I/home/liyun/V3D/v3d_main/basic_c_fun -I/home/liyun/V3D/2010_hackathon/li_yun/ITK-V3D-Plugins -DQT_GUI_LIB -DQT_CORE_LIB -o /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/IntensityTransformation/moc_Cast.cxx /home/liyun/V3D/2010_hackathon/li_yun/ITK-V3D-Plugins/Source/IntensityTransformation/Cast.h

# Object files for target Cast
Cast_OBJECTS = \
"CMakeFiles/Cast.dir/Cast.o" \
"CMakeFiles/Cast.dir/moc_Cast.o"

# External object files for target Cast
Cast_EXTERNAL_OBJECTS =

bin/libCast.so: Source/IntensityTransformation/CMakeFiles/Cast.dir/Cast.o
bin/libCast.so: Source/IntensityTransformation/CMakeFiles/Cast.dir/moc_Cast.o
bin/libCast.so: bin/libV3DInterface.a
bin/libCast.so: bin/libV3DITKCommon.a
bin/libCast.so: /usr/lib/libQtGui.so
bin/libCast.so: /usr/lib/libQtCore.so
bin/libCast.so: Source/IntensityTransformation/CMakeFiles/Cast.dir/build.make
bin/libCast.so: Source/IntensityTransformation/CMakeFiles/Cast.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared library ../../bin/libCast.so"
	cd /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/IntensityTransformation && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Cast.dir/link.txt --verbose=$(VERBOSE)
	cd /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/IntensityTransformation && /usr/bin/cmake -E copy /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/bin/libCast.so /home/liyun/V3D/v3d/plugins/ITK//IntensityTransformation/Cast/Cast.so

# Rule to build all files generated by this target.
Source/IntensityTransformation/CMakeFiles/Cast.dir/build: bin/libCast.so
.PHONY : Source/IntensityTransformation/CMakeFiles/Cast.dir/build

Source/IntensityTransformation/CMakeFiles/Cast.dir/requires: Source/IntensityTransformation/CMakeFiles/Cast.dir/Cast.o.requires
Source/IntensityTransformation/CMakeFiles/Cast.dir/requires: Source/IntensityTransformation/CMakeFiles/Cast.dir/moc_Cast.o.requires
.PHONY : Source/IntensityTransformation/CMakeFiles/Cast.dir/requires

Source/IntensityTransformation/CMakeFiles/Cast.dir/clean:
	cd /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/IntensityTransformation && $(CMAKE_COMMAND) -P CMakeFiles/Cast.dir/cmake_clean.cmake
.PHONY : Source/IntensityTransformation/CMakeFiles/Cast.dir/clean

Source/IntensityTransformation/CMakeFiles/Cast.dir/depend: Source/IntensityTransformation/moc_Cast.cxx
	cd /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/liyun/V3D/2010_hackathon/li_yun/ITK-V3D-Plugins /home/liyun/V3D/2010_hackathon/li_yun/ITK-V3D-Plugins/Source/IntensityTransformation /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/IntensityTransformation /home/liyun/V3D/2010_hackathon/li_yun/ITK-Plugin-Build/Source/IntensityTransformation/CMakeFiles/Cast.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Source/IntensityTransformation/CMakeFiles/Cast.dir/depend

