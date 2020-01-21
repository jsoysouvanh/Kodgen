#include "InfoStructures/ClassInfo.h"

using namespace refureku;

ClassInfo::ClassInfo(std::string&& entityName, PropertyGroup&& propertyGroup)	noexcept:
	StructClassInfo(std::forward<std::string>(entityName), std::forward<PropertyGroup>(propertyGroup), EType::Class)
{
}

std::ostream& refureku::operator<<(std::ostream& out_stream, ClassInfo const& classInfo) noexcept
{
	out_stream << "Class : " << classInfo.name;
	
	//Properties
	out_stream << " " << classInfo.properties << std::endl;

	//Fields
	out_stream << " - Fields" << std::endl;
	for (auto const& [access, fieldVector] : classInfo.fields)
	{
		out_stream << "   - " << toString(access) << std::endl;

		for (FieldInfo const& field : fieldVector)
		{
			out_stream << "       " << field << std::endl;
		}
	}

	//Methods
	out_stream << " - Methods" << std::endl;
	for (auto const& [access, methodVector] : classInfo.methods)
	{
		out_stream << "   - " << toString(access) << std::endl;

		for (MethodInfo const& method : methodVector)
		{
			out_stream << "       " << method << std::endl;
		}
	}

	return out_stream;
}