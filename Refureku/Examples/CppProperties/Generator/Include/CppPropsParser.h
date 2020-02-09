#pragma once

#include <Parsing/FileParser.h>

class CppPropsParser : public refureku::FileParser
{
	protected:
	virtual void preParse(fs::path const& parseFile)											noexcept override;
	virtual void postParse(fs::path const& parseFile, refureku::ParsingResult const& result)	noexcept override;

	public:
		CppPropsParser()						noexcept;
		CppPropsParser(CppPropsParser const&)	= default;
		CppPropsParser(CppPropsParser&&)		= default;
		~CppPropsParser()						= default;
};