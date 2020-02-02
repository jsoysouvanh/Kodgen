#include "CppPropsCodeTemplate.h"

#include <cctype>
#include <cassert>

#include <InfoStructures/StructClassInfo.h>

void CppPropsCodeTemplate::generateCode(refureku::GeneratedFile& generatedFile, refureku::EntityInfo const& entityInfo) const noexcept
{
	static std::string GetterPropName = "Get";
	static std::string SetterPropName = "Set";

	generatedFile.writeMacro(entityInfo.name + "_GENERATED", "DEFINE_GETTERS_AND_SETTERS");

	generatedFile.writeLine("#define DEFINE_GETTERS_AND_SETTERS		\\");
	generatedFile.writeLine("public:	\\");

	refureku::StructClassInfo const& classInfo = static_cast<refureku::StructClassInfo const&>(entityInfo);

	for (auto& [accessSpecifier, fields] : classInfo.fields)
	{
		for (refureku::FieldInfo const& fieldInfo : fields)
		{
			for (refureku::ComplexProperty const& complexProp : fieldInfo.properties.complexProperties)
			{
				//Getter
				if (complexProp.name == GetterPropName)
				{
					generatedFile.writeLine(generateGetter(fieldInfo, complexProp) + "	\\");
				}
				//Setter
				else if (complexProp.name == SetterPropName)
				{
					generatedFile.writeLine(generateSetter(fieldInfo, complexProp) + "	\\");
				}
			}
		}
	}

	generatedFile.writeLine("private:");
}

std::string CppPropsCodeTemplate::generateGetter(refureku::FieldInfo const& fieldInfo, refureku::ComplexProperty const& complexProp) const noexcept
{
	std::string postQualifiers;
	std::string rawReturnType		= fieldInfo.type.getName() + " ";
	std::string returnName			= fieldInfo.name;
	bool		isConst				= false;
	bool		isRef				= false;
	bool		isPtr				= false;
	bool		isExplicit			= false;

	for (std::string const& subprop : complexProp.subProperties)
	{
		if (!isConst && subprop.at(0) == 'c')			//const
		{
			isConst = true;
		}
		else if (!isExplicit && subprop.at(0) == 'e')	//explicit
		{
			isExplicit = true;
		}
		else if (!(isRef || isPtr))
		{
			if (subprop.at(0) == '&')					//&
			{
				isRef = true;
			}
			else if (subprop.at(0) == '*')				//*
			{
				isPtr = true;
			}
		}
	}

	std::string preTypeQualifiers;
		
	//Upper case the first field info char if applicable
	std::string methodName = fieldInfo.name;
	methodName.replace(0, 1, 1, std::toupper(methodName.at(0)));
	methodName.insert(0, "get");
	methodName += "()";

	if (fieldInfo.qualifiers.isStatic)
	{
		preTypeQualifiers = "static";
		postQualifiers.clear();	//remove the const
	}
	else if (isConst || !(isRef || isPtr))	//Field can't be const and static so else if is fine
	{
		postQualifiers = " const";	//A const field or a getter-by-value implies a const getter even if not specified
	}

	if (isConst && (isRef || isPtr))
	{
		rawReturnType += " const ";
	}

	if (isRef)
	{
		rawReturnType += "& ";
	}
	else if (isPtr)
	{
		rawReturnType += "* ";
		returnName.insert(0, "&");
	}

	if (isExplicit)
	{
		return preTypeQualifiers + rawReturnType + methodName + postQualifiers + ";";
	}
	else
	{
		return preTypeQualifiers + rawReturnType + methodName + postQualifiers + " { return " + returnName + "; }";
	}
}

std::string CppPropsCodeTemplate::generateSetter(refureku::FieldInfo const& fieldInfo, refureku::ComplexProperty const& complexProp) const noexcept
{
	//Can't generate any setter if the field is originally const qualified
	if (fieldInfo.type.qualifiers.isConst)
	{
		//TODO: issue error
		return "";
	}

	std::string rawReturnType	= fieldInfo.type.getName() + " ";
	std::string paramName		= "_refureku" + fieldInfo.name;
	bool		isExplicit		= false;

	if (!complexProp.subProperties.empty())	//explicit is the only supported subprop, so if it is not empty is must be explicit
			isExplicit = true;

	std::string preTypeQualifiers;

	//Upper case the first field info char if applicable
	std::string methodName = fieldInfo.name;
	methodName.replace(0, 1, 1, std::toupper(methodName.at(0)));
	methodName.insert(0, "set");
	methodName += "(";

	methodName += fieldInfo.type.getName();
	methodName += ((fieldInfo.type.sizeInBytes == 0u || fieldInfo.type.sizeInBytes > 4u) && !fieldInfo.type.isPointer && !fieldInfo.type.isReference) ? " const& " : " ";
	methodName += paramName;

	methodName += ")";

	if (fieldInfo.qualifiers.isStatic)
	{
		preTypeQualifiers = "static";
	}

	if (isExplicit)
	{
		return preTypeQualifiers + "void " + methodName + ";";
	}
	else
	{
		return preTypeQualifiers + "void " + methodName + " { " + fieldInfo.name + " = " + paramName + "; }";
	}
}