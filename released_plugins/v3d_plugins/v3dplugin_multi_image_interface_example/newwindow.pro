
TEMPLATE      = lib
CONFIG       += plugin warning_off 
INCLUDEPATH  += ../../basic_c_fun
HEADERS       = newwindowplugin.h
SOURCES       = newwindowplugin.cpp
TARGET        = $$qtLibraryTarget(newwindow)
DESTDIR       = ../../v3d/plugins/V3D_Plugin_Multi_Image_Interface_demo #win32 qmake couldn't handle space in path

