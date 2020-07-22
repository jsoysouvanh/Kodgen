
inline MethodParsingResult* MethodParser2::getParsingResult() noexcept
{
	return reinterpret_cast<MethodParsingResult*>(getContext().parsingResult);
}