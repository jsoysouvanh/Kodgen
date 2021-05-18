/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include <string>

#include "Kodgen/CodeGen/PropertyCodeGen.h"

class SetPropertyCodeGen : public kodgen::MacroPropertyCodeGen
{
	public:
		virtual ~SetPropertyCodeGen() = default;

		virtual bool generateCode(kodgen::EntityInfo const& entity, kodgen::Property const& property, kodgen::uint8 propertyIndex,
								  kodgen::CodeGenData& data, std::string& inout_result) const noexcept override
		{
			std::string errorMessage;
			
			//Check that Set property arguments are valid
			if (property.arguments.size() > 1)
			{
				errorMessage = "Set property can't take more than one argument.";
			}
			else if (property.arguments.size() > 0 && property.arguments[0] != "explicit")
			{
				errorMessage = "Set property only valid argument is 'explicit'.";
			}

			if (!errorMessage.empty())
			{
				//Log error message and abort generation
				if (data.logger != nullptr)
				{
					data.logger->log(errorMessage, kodgen::ILogger::ELogSeverity::Error);
				}

				return false;
			}

			//If arguments are valid, dispatch the generation call normally
			return kodgen::MacroPropertyCodeGen::generateCode(entity, property, propertyIndex, data, inout_result);
		}

		virtual bool shouldGenerateCode(kodgen::EntityInfo const& entity, kodgen::Property const& property, kodgen::uint8 /* propertyIndex */) const noexcept override
		{
			return property.name == "Set" && entityTypeOverlap(entity.entityType, kodgen::EEntityType::Field);
		}

		virtual bool generateClassFooterCode(kodgen::EntityInfo const& entity, kodgen::Property const& property, kodgen::uint8 /* propertyIndex */,
											 kodgen::MacroCodeGenData& data, std::string& inout_result) const noexcept override
		{
			kodgen::FieldInfo const& field = static_cast<kodgen::FieldInfo const&>(entity);

			//Can't generate any setter if the field is originally const qualified
			if (field.type.typeParts.back().descriptor & kodgen::ETypeDescriptor::Const)
			{
				if (data.logger != nullptr)
				{
					data.logger->log("Can't generate setter to the field " + entity.getFullName() + " because is const qualified. Abort generation.", kodgen::ILogger::ELogSeverity::Error);
				}

				return false;
			}

			std::string paramName	= "_kodgen" + field.name;
			bool		isExplicit	= !property.arguments.empty();	//explicit is the only supported subprop, so if it is not empty is must be explicit

			std::string preTypeQualifiers;

			//Upper case the first field info char if applicable
			std::string methodName = field.name;
			methodName.replace(0, 1, 1, static_cast<char>(std::toupper(methodName.at(0)))); 
			methodName.insert(0, "set");
			methodName += "(";

			methodName += field.type.getName();
			methodName += ((field.type.sizeInBytes == 0u || field.type.sizeInBytes > 4u) &&
						   !(field.type.typeParts.back().descriptor & kodgen::ETypeDescriptor::Ptr)	&&
						   !(field.type.typeParts.back().descriptor & kodgen::ETypeDescriptor::LRef)) ? " const& " : " ";
			methodName += paramName;

			methodName += ")";

			if (field.isStatic)
			{
				preTypeQualifiers = "static";
			}

			inout_result += isExplicit ? preTypeQualifiers + "void " + methodName + ";" :
				preTypeQualifiers + "void " + methodName + " { " + field.name + " = " + paramName + "; }";

			inout_result += data.separator;

			return true;
		}
};