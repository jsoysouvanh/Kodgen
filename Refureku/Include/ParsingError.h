#pragma once

#include <clang-c/Index.h>
#include <string>

#include "Properties/PropertyParsingError.h"

namespace refureku
{
	class ParsingError
	{
		private:
			unsigned				_line					= 0;
			unsigned				_column					= 0;
			std::string				_filename				= "";
			PropertyParsingError	_propertyParsingError	= PropertyParsingError::Count;

		public:
			ParsingError()						= delete;
			ParsingError(CXSourceLocation errorSourceLocation, PropertyParsingError ppe) noexcept;
			ParsingError(ParsingError const&)	= default;
			ParsingError(ParsingError&&)		= default;
			~ParsingError()						= default;

			std::string const&		getFilename()	const noexcept;
			unsigned				getLine()		const noexcept;
			unsigned				getColumn()		const noexcept;
			PropertyParsingError	getErrorValue()	const noexcept;
	};
}