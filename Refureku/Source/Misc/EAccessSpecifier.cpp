#include "Misc/EAccessSpecifier.h"

using namespace refureku;

std::string refureku::toString(EAccessSpecifier error) noexcept
{
	std::string result;

	switch (error)
	{
		case EAccessSpecifier::Public:
			result = "Public";
			break;

		case EAccessSpecifier::Protected:
			result = "Protected";
			break;

		case EAccessSpecifier::Private:
			result = "Private";
			break;

		case EAccessSpecifier::Invalid:
			result = "Invalid";
			break;
	}

	return result;
}