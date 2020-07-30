#include "Properties/ComplexProperty2.h"

using namespace kodgen;

ComplexProperty2::ComplexProperty2(std::string mainProperty, ComplexPropertyRule2 const* matchingPropertyRule) noexcept:
	mainProperty{std::move(mainProperty)},
	boundPropertyRule{matchingPropertyRule}
{
}