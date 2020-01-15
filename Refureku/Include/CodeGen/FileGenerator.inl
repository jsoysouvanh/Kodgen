
template <typename T>
void FileGenerator::addGeneratedCodeTemplate(std::string templateName, bool isDefault) noexcept
{
	static_assert(std::is_base_of<GeneratedCodeTemplate, T>::value, "Provided type must derive from GeneratedCodeTemplate");
	static_assert(std::is_default_constructible<T>::value, "Provided type must be default constructible");

	GeneratedCodeTemplate* codeTemplate = new T();

	_generatedCodeTemplates[templateName] = codeTemplate;

	if (isDefault)
		_defaultCodeTemplate = codeTemplate;

	updateSupportedCodeTemplateRegex();
}