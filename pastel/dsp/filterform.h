#ifndef PASTELDSP_FILTERFORM_H
#define PASTELDSP_FILTERFORM_H

#include "pastel/dsp/dsplibrary.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/view.h"

#include "pastel/sys/vector.h"

namespace Pastel
{

	template <
		typename Real,
		typename FilterWindow,
		typename Array>
	void setSincLowPass(
		const Real& cutOff,
		FilterWindow filterWindow,
		Array& output);

	template <
		typename Real,
		typename FilterWindow,
		typename Array>
	void setSincHighPass(
		const Real& cutOff,
		FilterWindow filterWindow,
		Array& output);

	template <
		typename Real,
		typename FilterWindow,
		typename Output_View>
	void setSincLowPass(
		const PASTEL_NO_DEDUCTION(Real)& cutOff,
		FilterWindow filterWindow,
		const View<2, Real, Output_View>& output);

	template <
		typename Real,
		typename FilterWindow,
		typename Output_View>
	void setSincHighPass(
		const PASTEL_NO_DEDUCTION(Real)& cutOff,
		FilterWindow filterWindow,
		const View<2, Real, Output_View>& output);

	template <
		typename FilterWindow,
		typename Real,
		typename Output_View>
	void setFilter(
		FilterWindow filterWindow,
		const View<2, Real, Output_View>& output);

	template <typename Real>
	Real boxWindow(const Real& time);

	template <typename Real>
	Real bartlettWindow(const Real& time);

	template <typename Real>
	Real hanningWindow(const Real& time);

	template <typename Real>
	Real hammingWindow(const Real& time);

	template <typename Real>
	Real blackmanWindow(const Real& time);

	template <typename Real>
	Real mitchellNetravali(const Real& time);

	template <typename Real>
	Real gaussian(const Real& distance,
		const PASTEL_NO_DEDUCTION(Real)& deviation);

}

#include "pastel/dsp/filterform.hpp"

#endif
