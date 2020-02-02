#pragma once

#include <CodeGen/GeneratedCodeTemplate.h>
#include <InfoStructures/FieldInfo.h>

class CppPropsCodeTemplate : public refureku::GeneratedCodeTemplate
{
	private:
		std::string generateGetter(refureku::FieldInfo const& fieldInfo, refureku::ComplexProperty const& complexProp)	const	noexcept;
		std::string generateSetter(refureku::FieldInfo const& fieldInfo, refureku::ComplexProperty const& complexProp)	const	noexcept;

	protected:
		virtual void generateCode(refureku::GeneratedFile& generatedFile, refureku::EntityInfo const& entityInfo) const noexcept override;
};