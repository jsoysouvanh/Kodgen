#include "Properties/SimpleProperty2.h"

using namespace kodgen;

SimpleProperty2::SimpleProperty2(std::string mainProperty, SimplePropertyRule2 const* matchingPropertyRule) noexcept:
	mainProperty{mainProperty},
	boundPropertyRule{matchingPropertyRule}
{
}