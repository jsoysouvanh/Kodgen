
inline FileParsingResult* FileParser2::getParsingResult() noexcept
{
	return reinterpret_cast<FileParsingResult*>(parsingContext.parsingResult);
}