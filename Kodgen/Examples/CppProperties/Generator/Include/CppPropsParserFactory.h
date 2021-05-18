#pragma once

#include <Kodgen/Parsing/FileParserFactory.h>

#include "CppPropsParser.h"

class CppPropsParserFactory : public kodgen::FileParserFactory<CppPropsParser>
{
	public:
		CppPropsParserFactory()				noexcept;
		virtual ~CppPropsParserFactory()	= default;
};