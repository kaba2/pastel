#include "pastel/geometry/plyloader.h"

#include "pastel/sys/log.h"

#include <rply.h>

#include <vector>

namespace Pastel
{

	namespace
	{

		integer currentElement__ = 0;
		std::vector<double> currentValues__;
		PlyLoaderCallback* currentLoader__ = 0;

		int plyCallBack(p_ply_argument argument)
		{
			ASSERT(currentLoader__);

			ply_get_argument_user_data(argument, 0, 0);

			int result = 1;

			long length = 0;
			long valueIndex = 0;
			ply_get_argument_property(argument, NULL,
				&length, &valueIndex);

			// Skip the header of the list.
			if (valueIndex != -1)
			{
				if (valueIndex == 0)
				{
					currentValues__.clear();
				}

				currentValues__.push_back(
					(double)ply_get_argument_value(argument));

				if (valueIndex == length - 1)
				{
					bool wantToContinue =
						currentLoader__->receive(
						currentElement__, currentValues__);
					result = wantToContinue ? 1 : 0;
					++currentElement__;
				}
			}

			return result;
		}

		inline void plyErrorCallBack(const char* message)
		{
			log() << message << logNewLine;
		}

	}

	PlyLoaderCallback::~PlyLoaderCallback()
	{
	}

	PlyLoader& PlyLoader::create()
	{
		static PlyLoader thePlyLoader;
		return thePlyLoader;
	}

	PlyLoader::PlyLoader()
	{
	}

	PlyLoader::~PlyLoader()
	{
	}


	bool PlyLoader::loadInfo(
		const std::string& fileName,
		PlyLoaderInfo& info)
	{
		log() << "Loading info from a ply file \""
			<< fileName << "\"." << logNewLine;

		p_ply ply = ply_open(fileName.c_str(),
			plyErrorCallBack);

		if (!ply)
		{
			return false;
		}

		if (!ply_read_header(ply))
		{
			return false;
		}

		SmallMap<std::string, PlyLoaderElement> elements;

		p_ply_element element = ply_get_next_element(ply, 0);

		while (element)
		{
			const char* elementName = 0;
			long elementCount = 0;
			int result = ply_get_element_info(
				element, &elementName, &elementCount);
			ENSURE(result);

			SmallMap<std::string, PlyLoaderProperty> properties;

			p_ply_property prop = ply_get_next_property(element, 0);
			while (prop)
			{
				const char* propertyName = 0;
				result = ply_get_property_info(
					prop, &propertyName, 0, 0, 0);
				ENSURE(result);

				properties.insert(
					std::string(propertyName),
					PlyLoaderProperty());

				prop = ply_get_next_property(element, prop);
			}

			elements.insert(
				std::string(elementName),
				PlyLoaderElement(
				elementCount,
				properties));

			element = ply_get_next_element(ply, element);
		}

		ply_close(ply);

		log() << "Ply info loading complete." << logNewLine;

		info = PlyLoaderInfo(elements);

		return true;
	}

	bool PlyLoader::loadElement(
		const std::string& fileName,
		const std::string& elementName,
		const std::string& propertyName,
		PlyLoaderCallback& elementLoader)
	{
		log() << "Loading from a ply file \""
			<< fileName << "\", " << logNewLine;
		log() << "with element name \""
			<< elementName << "\"" << logNewLine;
		log() << "and property name \""
			<< propertyName << "\"." << logNewLine;

		p_ply ply = ply_open(fileName.c_str(),
			plyErrorCallBack);

		if (!ply)
		{
			return false;
		}

		if (!ply_read_header(ply))
		{
			return false;
		}

		ply_set_read_cb(
			ply, elementName.c_str(),
			propertyName.c_str(),
			&plyCallBack,
			0, 0);

		currentElement__ = 0;
		currentValues__.clear();
		currentLoader__ = &elementLoader;

		if (!ply_read(ply))
		{
			log() << "Error reading ply" << logNewLine;
			return false;
		}

		ply_close(ply);

		log() << "Ply loading complete." << logNewLine;

		return true;
	}

}
