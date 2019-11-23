#include "InfoStructures/ClassInfo.h"

using namespace refureku;

ClassInfo::ClassInfo(std::string&& entityName, PropertyGroup&& propertyGroup)	noexcept:
	StructClassInfo(std::forward<std::string>(entityName), std::forward<PropertyGroup>(propertyGroup))
{
}

std::ostream& refureku::operator<<(std::ostream& out_stream, ClassInfo const& classInfo) noexcept
{
	out_stream << "Class : " << classInfo.name;
	
	//Properties
	out_stream << " " << classInfo.properties << std::endl;

	//Fields

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