
inline NamespaceParsingResult* NamespaceParser2::getParsingResult() noexcept
{
	return reinterpret_cast<NamespaceParsingResult*>(parsingContext.parsingResult);
}