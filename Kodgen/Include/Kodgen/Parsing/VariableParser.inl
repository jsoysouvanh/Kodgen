
inline VariableParsingResult* VariableParser::getParsingResult() noexcept
{
	return reinterpret_cast<VariableParsingResult*>(getContext().parsingResult);
}