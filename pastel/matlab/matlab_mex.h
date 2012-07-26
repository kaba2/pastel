#ifndef PASTEL_MATLAB_MEX_H
#define PASTEL_MATLAB_MEX_H

// This is to fix Matlab clashing with the char16_t
// introduced in Visual Studio 2010.
#include <yvals.h>
#if (_MSC_VER >= 1600)
#define __STDC_UTF_16__
#endif

// This is the Matlab's mex header file.
#include "mex.h"

#endif
