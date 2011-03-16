% Description: PastelGeometryMatlab develop build script

mex pastelgeometrymatlab.cpp ...
    -D_HAS_ITERATOR_DEBUGGING=0 ...
    -DPASTEL_ENABLE_PENSURES ...
    -I'..\..\..\pastel\' ...
    -I'..\..\..\..\external\boost_1_45_0\' ...
    -L'..\..\..\pastel\build\vs2008\lib\' ...
    -lPastelGeometryMatlab_v ...
    -lPastelGeometry_v ...
    -lPastelMath_v ...
    -lPastelMatlab_v ...
    -lPastelSys_v
