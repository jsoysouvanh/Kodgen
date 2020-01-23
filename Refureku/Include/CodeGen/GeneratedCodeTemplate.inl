
template <typename... Lines>
void GeneratedCodeTemplate::expandWriteMacroLines(std::string&& line, Lines&&... otherLines) noexcept
{
	writeLine("\t" + std::forward<std::string>(line) + "\t\\");
	expandWriteMacroLines(std::forward<Lines>(otherLines)...);
}

template <typename... Lines>
void GeneratedCodeTemplate::writeMacro(std::string&& macroPrototype, Lines&&... lines) noexcept
{
	writeLine("#define " + std::forward<std::string>(macroPrototype) + "\t\\");
	expandWriteMacroLines(std::forward<Lines>(lines)...);
}

template <typename... Lines>
void GeneratedCodeTemplate::writeLines(std::string const& line, Lines&&... otherLines) noexcept
{
	writeLine(line);
	writeLines(std::forward<Lines>(otherLines)...);
}

template <typename... Lines>
void GeneratedCodeTemplate::writeLines(std::string&& line, Lines&&... otherLines) noexcept
{
	writeLine(std::forward<std::string>(line));
	writeLines(std::forward<Lines>(otherLines)...);
}