% Description: PastelGeometryMatlab release build script

mex pastelgeometrymatlab.cpp ...
    -D_ITERATOR_DEBUG_LEVEL=0 ...
    -DPASTEL_ENABLE_OMP ...
    -I'..\..\..\pastel\' ...
    -I'..\..\..\..\external\boost_1_51_0\' ...
    -L'..\..\..\pastel\build\vs2010\lib\release\' ...
    -lPastelGeometryMatlab ...
    -lPastelGeometry ...
    -lPastelMath ...
    -lPastelMatlab ...
    -lPastelSys
