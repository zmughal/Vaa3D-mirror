// This file has been generated by Py++.

#include "boost/python.hpp"
#include "wrappable_v3d.h"
#include "v3d_global_variables.pypp.hpp"

namespace bp = boost::python;

void register_global_variables(){

    bp::scope().attr("XAxis") = SimTK::XAxis;

    bp::scope().attr("YAxis") = SimTK::YAxis;

    bp::scope().attr("ZAxis") = SimTK::ZAxis;

}
