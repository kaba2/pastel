// Description: PlyLoader class
// Detail: Loading of .ply files
// Documentation: needs_more_thought.txt

#ifndef PASTEL_PLYLOADER_H
#define PASTEL_PLYLOADER_H

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/geometrylibrary.h"
#include "pastel/sys/smallmap.h"

#include <string>
#include <vector>

namespace Pastel
{

	class PASTELGEOMETRY PlyLoaderCallback
	{
	public:
		virtual ~PlyLoaderCallback();

		virtual bool receive(
			integer element,
			const std::vector<double>& values) = 0;
	};

	class PASTELGEOMETRY PlyLoaderProperty
	{
	public:
	private:
		// No data..
	};

	class PASTELGEOMETRY PlyLoaderElement
	{
	public:
		PlyLoaderElement()
			: count_(0)
			, properties_()
		{
		}

		PlyLoaderElement(
			integer count,
			const SmallMap<std::string, PlyLoaderProperty> properties)
			: count_(count)
			, properties_(properties)
		{
		}

		integer count() const
		{
			return count_;
		}

		const SmallMap<std::string, PlyLoaderProperty>& properties() const
		{
			return properties_;
		}

	private:
		integer count_;
		SmallMap<std::string, PlyLoaderProperty> properties_;
	};

	class PASTELGEOMETRY PlyLoaderInfo
	{
	public:
		PlyLoaderInfo()
			: elements_()
		{
		}

		explicit PlyLoaderInfo(
			const SmallMap<std::string, PlyLoaderElement>& elements)
			: elements_(elements)
		{
		}

		const SmallMap<std::string, PlyLoaderElement>& elements() const
		{
			return elements_;
		}

	private:
		SmallMap<std::string, PlyLoaderElement> elements_;
	};

	class PASTELGEOMETRY PlyLoader
	{
	public:
		static PlyLoader& create();

		bool loadInfo(
			const std::string& fileName,
			PlyLoaderInfo& info);

		bool loadElement(
			const std::string& fileName,
			const std::string& elementName,
			const std::string& propertyName,
			PlyLoaderCallback& elementLoader);

	private:
		PlyLoader();
		// Prohibited
		PlyLoader(const PlyLoader&);
		// Prohibited
		PlyLoader& operator=(const PlyLoader&);
		~PlyLoader();
	};

	PlyLoader& plyLoader();

}

#include "pastel/geometry/plyloader.hpp"

#endif
