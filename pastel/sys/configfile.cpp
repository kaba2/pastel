#include "pastel/sys/configfile.h"
#include "pastel/sys/log.h"

#include <fstream>
using std::istream;
using std::ostream;

#include <boost/spirit/include/classic.hpp>
#include <boost/bind.hpp>
using namespace boost::spirit::classic;

namespace Pastel
{

	class ConfigSemantic
	{
	public:
		void initialize(
			LoadConfig_Echo::Enum echoPolicy)
		{
			config_.clear();
			key_ = "";
			count_ = 0;
			size_ = 0;
			echoPolicy_ = echoPolicy;
		}

		void startDeclare()
		{
			key_ = "";
			count_ = 0;
			size_ = 1;
		}

		void endDeclare()
		{
			if (echoPolicy_ != LoadConfig_Echo::None)
			{
				log() << logNewLine;
			}
		}

		void declare(
			const char* keyBegin,
			const char* keyEnd)
		{
			key_ = std::string(keyBegin, keyEnd);
			if (echoPolicy_ != LoadConfig_Echo::None)
			{
				log() << key_ << " = ";
				if (echoPolicy_ == LoadConfig_Echo::OnlyFinite && size_ == -1)
				{
					log() << "...";
				}
			}
		}

		void declareSize(integer size)
		{
			size_ = size;
		}

		void defineInteger(
			integer value)
		{
			if ((echoPolicy_ == LoadConfig_Echo::OnlyFinite && size_ != -1) ||
				echoPolicy_ == LoadConfig_Echo::All)
			{
				if (size_ != -1 && count_ >= size_)
				{
					log() << ", (" << value << ")";
					return;
				}

				if (count_ > 0)
				{
					log() << ", ";
				}

				log() << value;
			}

			config_.insert<integer>(
				key_, value);
			++count_;
		}

		void defineReal(
			real value)
		{
			if ((echoPolicy_ == LoadConfig_Echo::OnlyFinite && size_ != -1) ||
				echoPolicy_ == LoadConfig_Echo::All)
			{
				if (size_ != -1 && count_ >= size_)
				{
					log() << ", (" << value << ")";
					return;
				}

				if (count_ > 0)
				{
					log() << ", ";
				}

				log() << value;
			}

			config_.insert<real>(
				key_, value);

			++count_;
		}

		void defineString(
			const char* valueBegin,
			const char* valueEnd)
		{
			const std::string value(valueBegin + 1, valueEnd - 1);

			if ((echoPolicy_ == LoadConfig_Echo::OnlyFinite && size_ != -1) ||
				echoPolicy_ == LoadConfig_Echo::All)
			{
				if (size_ != -1 && count_ >= size_)
				{
					log() << ", (" << value << ")";
					return;
				}

				if (count_ > 0)
				{
					log() << ", ";
				}

				log() << value;
			}

			config_.insert<std::string>(
				key_, value);
			++count_;
		}

		const Config& config() const
		{
			return config_;
		}

	private:
		Config config_;
		std::string key_;
		integer count_;
		integer size_;
		LoadConfig_Echo::Enum echoPolicy_;
	};

	ConfigSemantic configSemantic;

	struct ConfigGrammar
		: public grammar<ConfigGrammar>
	{
	public:
		template <typename Scanner>
		struct definition
		{
			definition(ConfigGrammar const& self)
			{
				/*
				Config file format
				------------------

				Commenting:
				Everything after a % or a // symbol to the end of the row will be ignored.
				/x x/ where x = * can be used for multi-line comments.

				<File> := 'Pastel Config File' <VariableDeclaration>*
				<VariableDeclaration> := <IntegerDeclaration> | <RealDeclaration> | <StringDeclaration>

				<IntegerDeclaration> := 'integer' [<VariableSize>] <VariableName> '=' <IntegerList>
				<RealDeclaration> := 'real' [<VariableSize>] <VariableName> '=' <RealList>
				<StringDeclaration> := 'string' [<VariableSize>] <VariableName> '=' <StringList>

				<VariableSize> := <PositiveInteger> | '*'
				<VariableName> := <String>

				<IntegerList> := <Integer> | <IntegerList> ',' <Integer>
				<RealList> := <Real> | <RealList> ',' <Real>
				<StringList> := <String> | <StringList> ',' <String>
				*/

				headerRule = str_p("Pastel Config File");

				variableSizeRule =
					uint_p[boost::bind(&ConfigSemantic::declareSize, boost::ref(configSemantic), _1)] |
					ch_p('*')[boost::bind(&ConfigSemantic::declareSize, boost::ref(configSemantic), -1)];

				variableNameRule =
					(alpha_p | '_') >> *alnum_p;

				stringRule =
					lexeme_d['"' >> *(anychar_p - '"') >> '"'];

				integerListRule =
					int_p[boost::bind(&ConfigSemantic::defineInteger, boost::ref(configSemantic), _1)] >>
					*(',' >> int_p[boost::bind(&ConfigSemantic::defineInteger, boost::ref(configSemantic), _1)]);

				realListRule =
					real_p[boost::bind(&ConfigSemantic::defineReal, boost::ref(configSemantic), _1)] >>
					*(',' >> real_p[boost::bind(&ConfigSemantic::defineReal, boost::ref(configSemantic), _1)]);

				stringListRule =
					stringRule[boost::bind(&ConfigSemantic::defineString, boost::ref(configSemantic), _1, _2)] >>
					*(',' >> stringRule[boost::bind(&ConfigSemantic::defineString, boost::ref(configSemantic), _1, _2)]);

				integerDeclarationRule =
					str_p("integer")[boost::bind(&ConfigSemantic::startDeclare, boost::ref(configSemantic))] >>
					!variableSizeRule >>
					variableNameRule[boost::bind(&ConfigSemantic::declare, boost::ref(configSemantic), _1, _2)] >>
					'=' >> integerListRule;

				realDeclarationRule =
					str_p("real")[boost::bind(&ConfigSemantic::startDeclare, boost::ref(configSemantic))] >>
					!variableSizeRule >>
					variableNameRule[boost::bind(&ConfigSemantic::declare, boost::ref(configSemantic), _1, _2)] >>
					'=' >> realListRule;

				stringDeclarationRule =
					str_p("string")[boost::bind(&ConfigSemantic::startDeclare, boost::ref(configSemantic))] >>
					!variableSizeRule >>
					variableNameRule[boost::bind(&ConfigSemantic::declare, boost::ref(configSemantic), _1, _2)] >>
					'=' >> stringListRule;

				declarationRule =
					integerDeclarationRule |
					realDeclarationRule |
					stringDeclarationRule;

				configFileRule =
					headerRule >>
					*(declarationRule[boost::bind(&ConfigSemantic::endDeclare, boost::ref(configSemantic))]) >>
					end_p;
			}

			rule<Scanner> const& start() const
			{
				return configFileRule;
			}

		private:
			rule<Scanner> headerRule;
			rule<Scanner> variableSizeRule;
			rule<Scanner> variableNameRule;
			rule<Scanner> stringRule;
			rule<Scanner> integerListRule;
			rule<Scanner> realListRule;
			rule<Scanner> stringListRule;
			rule<Scanner> integerDeclarationRule;
			rule<Scanner> realDeclarationRule;
			rule<Scanner> stringDeclarationRule;
			rule<Scanner> declarationRule;
			rule<Scanner> configFileRule;
		};
	};

	PASTELSYS bool loadConfig(
		const std::string& fileName,
		Config& config,
		LoadConfig_Echo::Enum echoPolicy)
	{
		log() << "Loading config file " << fileName << "..." << logNewLine;

		std::ifstream file(fileName.c_str());
		if (!file)
		{
			log() << "Could not open config file "
				<< fileName << "." << logNewLine;
			return false;
		}

		// Read the file to memory.

		std::string fileContents;
		while(true)
		{
			std::string row;
			std::getline(file, row);
			if (!file)
			{
				break;
			}

			fileContents += "\n";
			fileContents += row;
			fileContents += "\n";
		}

		// Parse

		configSemantic.initialize(echoPolicy);

		ConfigGrammar configGrammar;

		if (!parse(fileContents.c_str(), configGrammar,
			space_p
			| ((ch_p('%') | "//") >> *(anychar_p - eol_p) >> eol_p)
			| ("/*" >> *(anychar_p - "*/") >> "*/")
			).full)
		{
			log() << "Syntax error. Aborting..." << logNewLine;
			//return false;
		}

		config = configSemantic.config();

		return true;
	}

	template <typename Type>
	void saveProperty(
		std::ofstream& file,
		const Config& config,
		const Config::ConstIterator& iter)
	{
		const std::vector<Type>& valueList = config.propertyList<Type>(iter);
		const integer maxValuesPerRow = 4;
		const integer values = valueList.size();

		if (values > 1)
		{
			file << "* ";
		}

		file << iter->first << " = ";

		if (values > 1)
		{
			file << std::endl;
		}

		const bool isString =
			config.ofType<std::string>(iter);

		integer valuesPerRow = 0;
		for (integer i = 0;i < values;++i)
		{
			if (isString)
			{
				file << "\"";
			}
			file << valueList[i];
			if (isString)
			{
				file << "\"";
			}

			if (i < values - 1)
			{
				file << ", ";
			}

			++valuesPerRow;
			if (valuesPerRow >= maxValuesPerRow)
			{
				file << std::endl;
				valuesPerRow = 0;
			}
		}

		if (valuesPerRow > 0)
		{
			file << std::endl;
		}

		file << std::endl;
	}

	PASTELSYS bool saveConfig(
		const Config& config,
		const std::string& fileName)
	{
		log() << "Saving a config to file " << fileName << "..." << logNewLine;

		std::ofstream file(fileName.c_str());
		if (!file)
		{
			log() << "saveConfig(): Could not open file " << fileName << " for writing. Aborting..." <<
				logNewLine;
			return false;
		}

		file << "Pastel Config File" << std::endl << std::endl;

		{
			Config::ConstIterator iter = config.begin();
			Config::ConstIterator iterEnd = config.end();

			while(iter != iterEnd)
			{
				if (config.ofType<integer>(iter))
				{
					file << "integer ";

					saveProperty<integer>(file, config, iter);
				}
				else if (config.ofType<real>(iter))
				{
					file << "real ";

					saveProperty<real>(file, config, iter);
				}
				else if (config.ofType<std::string>(iter))
				{
					file << "string ";

					saveProperty<std::string>(file, config, iter);
				}
				else
				{
					log() << "'" << iter->first << "' has unknown type, skipping... " << logNewLine;
				}

				++iter;
			}
		}


		return true;
	}

}
