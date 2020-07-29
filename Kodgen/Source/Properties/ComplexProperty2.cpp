#include "Properties/ComplexProperty2.h"

using namespace kodgen;

ComplexProperty2::ComplexProperty2(std::string mainProperty) noexcept:
	mainProperty{std::move(mainProperty)}
{
}