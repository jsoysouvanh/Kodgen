#include <iostream>
#include <cassert>

#include "Misc/Filesystem.h"
#include "Properties/PropertyRules.h"

int main()
{
	refureku::PropertyRules propertyRules;

	// Simple properties
	assert(propertyRules.addSimplePropertyRule("RefurekuProp"));
	assert(!propertyRules.addSimplePropertyRule("RefurekuProp"));
	assert(propertyRules.removeSimplePropertyRule("RefurekuProp"));
	assert(propertyRules.addSimplePropertyRule("RefurekuProp"));
	assert(propertyRules.addSimplePropertyRule("RefurekuProp2"));

	assert(propertyRules.getSimplePropertyRule("RefurekuProp") != nullptr);
	assert(propertyRules.getSimplePropertyRule("RefurekuProp2") != nullptr);
	assert(propertyRules.getSimplePropertyRule("RefurekuProp3") == nullptr);

	// Complex properties
	assert(propertyRules.addComplexPropertyRule("RefurekuProp", "Test(This|OrThis)"));
	assert(!propertyRules.addComplexPropertyRule("RefurekuProp", "Test(This|OrThis)"));
	assert(propertyRules.removeComplexPropertyRule("RefurekuProp", "Test(This|OrThis)"));
	assert(propertyRules.addComplexPropertyRule("RefurekuProp", "Test(This|OrThis)"));
	assert(propertyRules.addComplexPropertyRule("RefurekuProp2", "Test(This|OrThis)"));

	assert(propertyRules.getComplexPropertyRule("RefurekuProp") != nullptr);
	assert(propertyRules.getComplexPropertyRule("RefurekuProp2") != nullptr);
	assert(propertyRules.getComplexPropertyRule("RefurekuProp3") == nullptr);

	return EXIT_SUCCESS;
}