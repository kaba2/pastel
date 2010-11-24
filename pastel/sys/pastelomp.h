// Description: Conditionally includes omp.h
// Documentation: environment.txt

#ifndef PASTEL_PASTELOMP_H
#define PASTEL_PASTELOMP_H

#ifdef PASTEL_ENABLE_OMP
#include "omp.h"
#endif

#endif
