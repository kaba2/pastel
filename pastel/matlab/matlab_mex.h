// Description: Redirection file for Matlab's mex.h.

#ifndef PASTELMATLAB_MATLAB_MEX_H
#define PASTELMATLAB_MATLAB_MEX_H

// This is to fix Matlab clashing with the char16_t
// introduced in Visual Studio 2010.
#if (_MSC_VER >= 1600)
#include <yvals.h>
#define __STDC_UTF_16__
#endif

// This is the Matlab's mex header file.
#include "mex.h"

#endif
