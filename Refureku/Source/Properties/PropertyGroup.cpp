#include "Properties/PropertyGroup.h"

using namespace refureku;

std::ostream& refureku::operator<<(std::ostream& out_stream, PropertyGroup const& propertyGroup) noexcept
{
	out_stream << "Properties: " << std::endl;

	for (SimpleProperty const& simpleProp : propertyGroup.simpleProperties)
	{
		out_stream << "  " << simpleProp << std::endl;
	}

	for (ComplexProperty const& complexProp : propertyGroup.complexProperties)
	{
		out_stream << "  " << complexProp << std::endl;
	}

	return out_stream;
}