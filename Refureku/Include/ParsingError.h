#pragma once

#include <clang-c/Index.h>
#include <string>

#include "Properties/EParsingError.h"

namespace refureku
{
	class ParsingError
	{
		private:
			unsigned		_line					= 0;
			unsigned		_column					= 0;
			std::string		_filename				= "";
			EParsingError	_propertyParsingError	= EParsingError::Count;

		public:
			ParsingError()						= delete;
			ParsingError(EParsingError parsingError, CXSourceLocation errorSourceLocation = clang_getNullLocation()) noexcept;
			ParsingError(ParsingError const&)	= default;
			ParsingError(ParsingError&&)		= default;
			~ParsingError()						= default;

			std::string const&		getFilename()	const noexcept;
			unsigned				getLine()		const noexcept;
			unsigned				getColumn()		const noexcept;
			EParsingError			getErrorValue()	const noexcept;
	};
}