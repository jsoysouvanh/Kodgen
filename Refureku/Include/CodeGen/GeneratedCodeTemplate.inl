
template <typename... Lines>
void GeneratedCodeTemplate::expandWriteMacroLines(std::string&& line, Lines&&... otherLines) noexcept
{
	write("\t" + std::forward<std::string>(line) + "\t\\");
	expandWriteMacroLines(std::forward<Lines>(otherLines)...);
}

template <typename... Lines>
void GeneratedCodeTemplate::writeMacro(std::string&& macroPrototype, Lines&&... lines) noexcept
{
	write("#define " + std::forward<std::string>(macroPrototype) + "\t\\");
	expandWriteMacroLines(std::forward<Lines>(lines)...);
}