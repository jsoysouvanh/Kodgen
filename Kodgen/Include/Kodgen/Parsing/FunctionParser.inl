
inline FunctionParsingResult* FunctionParser::getParsingResult() noexcept
{
	return reinterpret_cast<FunctionParsingResult*>(getContext().parsingResult);
}