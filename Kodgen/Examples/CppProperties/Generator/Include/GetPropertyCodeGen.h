/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include <string>

#include "Kodgen/CodeGen/Macro/MacroPropertyCodeGen.h"

class GetPropertyCodeGen : public kodgen::MacroPropertyCodeGen
{
	public:
		virtual ~GetPropertyCodeGen() = default;

		virtual bool shouldGenerateCode(kodgen::EntityInfo const& entity, kodgen::Property const& property, kodgen::uint8 /* propertyIndex */) const noexcept override
		{
			return property.name == "Get" && entityTypeOverlap(entity.entityType, kodgen::EEntityType::Field);
		}

		virtual bool generateCode(kodgen::EntityInfo const& entity, kodgen::Property const& property, kodgen::uint8 propertyIndex,
								  kodgen::CodeGenEnv& env, std::string& inout_result) const noexcept override
		{
			std::string errorMessage;

			//Can't have * and & at the same time
			if (std::find(property.arguments.cbegin(), property.arguments.cend(), "*") != property.arguments.cend() &&
				std::find(property.arguments.cbegin(), property.arguments.cend(), "&") != property.arguments.cend())
			{
				errorMessage = "Get property can't accept both '*' and '&' at the same time.";
			}
			else
			{
				//Check that Get property arguments are valid
				for (std::string const& arg : property.arguments)
				{
					if (arg != "*" && arg != "&" && arg != "const" && arg != "explicit")
					{
						errorMessage = "Get property only accepts '*', '&' and 'explicit' arguments.";
						break;
					}
				}
			}

			if (!errorMessage.empty())
			{
				//Log error message and abort generation
				if (env.logger != nullptr)
				{
					env.logger->log(errorMessage, kodgen::ILogger::ELogSeverity::Error);
				}

				return false;
			}

			//If arguments are valid, dispatch the generation call normally
			return kodgen::MacroPropertyCodeGen::generateCode(entity, property, propertyIndex, env, inout_result);
		}

		virtual bool generateClassFooterCode(kodgen::EntityInfo const& entity, kodgen::Property const& property, kodgen::uint8 /* propertyIndex */,
											 kodgen::MacroCodeGenEnv& data, std::string& inout_result) const noexcept override
		{
			kodgen::FieldInfo const& field = static_cast<kodgen::FieldInfo const&>(entity);

			std::string postQualifiers;
			std::string rawReturnType	= field.type.getCanonicalName() + " ";
			std::string returnName		= field.name;
			bool		isConst			= false;
			bool		isRef			= false;
			bool		isPtr			= false;

			for (std::string const& subprop : property.arguments)
			{
				if (!isConst && subprop.at(0) == 'c')			//const
				{
					isConst = true;
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
			std::string methodName = field.name;
			methodName.replace(0, 1, 1, static_cast<char>(std::toupper(methodName.at(0))));
			methodName.insert(0, "get");
			methodName += "()";

			if (field.isStatic)
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

			inout_result += preTypeQualifiers + rawReturnType + methodName + postQualifiers + ";" + data.separator;

			return true;
		}

		virtual bool generateSourceFileHeaderCode(kodgen::EntityInfo const& entity, kodgen::Property const& property, kodgen::uint8 /* propertyIndex */,
												  kodgen::MacroCodeGenEnv& data, std::string& inout_result) const noexcept override
		{
			kodgen::FieldInfo const& field = static_cast<kodgen::FieldInfo const&>(entity);

			std::string postQualifiers;
			std::string rawReturnType	= field.type.getCanonicalName() + " ";
			std::string returnName		= field.name;
			bool		isConst			= false;
			bool		isRef			= false;
			bool		isPtr			= false;

			for (std::string const& subprop : property.arguments)
			{
				if (!isConst && subprop.at(0) == 'c')			//const
				{
					isConst = true;
				}
				else if (subprop.at(0) == 'e')	//explicit
				{
					//Don't generate getter definition if it is marked as explicit
					return true;
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
			
			//Upper case the first field info char if applicable
			std::string methodName = field.name;
			methodName.replace(0, 1, 1, static_cast<char>(std::toupper(methodName.at(0))));
			methodName.insert(0, "get");
			methodName += "()";

			if (!field.isStatic && (isConst || !(isRef || isPtr)))	//Field can't be const and static so else if is fine
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

			inout_result += rawReturnType + entity.outerEntity->getFullName() + "::" + methodName + postQualifiers + " { return " + returnName + "; }" + data.separator;

			return true;
		}
};