#include <iostream>
#include <cassert>

#include "Parsing/Parser.h"
#include "CodeGen/FileGenerator.h"

#include "TestGeneratedCodeTemplate.h"

static std::string const TestFile =
"#pragma once																																				\n"
"																																							\n"
"#ifdef REFUREKU_PARSING																																	\n"
"#define RfrkClass(...)		__attribute__((annotate(\"RfrkClass:\"#__VA_ARGS__)))																			\n"
"#define RfrkStruct(...)	__attribute__((annotate(\"RfrkStruct:\"#__VA_ARGS__)))																			\n"
"#define RfrkEnum(...)		__attribute__((annotate(\"RfrkEnum:\"#__VA_ARGS__)))																			\n"
"#define RfrkEnumVal(...)	__attribute__((annotate(\"RfrkEnumVal:\"#__VA_ARGS__)))																			\n"
"#define RfrkField(...)		__attribute__((annotate(\"RfrkField:\"#__VA_ARGS__)))																			\n"
"#define RfrkMethod(...)	__attribute__((annotate(\"RfrkMethod:\"#__VA_ARGS__)))																			\n"
"#else																																						\n"
"#define RfrkClass(...)																																		\n"
"#define RfrkStruct(...)																																	\n"
"#define RfrkEnum(...)																																		\n"
"#define RfrkEnumVal(...)																																	\n"
"#define RfrkField(...)																																		\n"
"#define RfrkMethod(...)																																	\n"
"#endif																																						\n"
"//#include \"Generated/TestClass.refureku.h\"																												\n"
"																																							\n"
"#include <filesystem>																																		\n"
"#include <vector>																																			\n"
"#include <string>																																			\n"
"																																							\n"
"namespace TestNamespace																																	\n"
"{																																							\n"
"	class Class1{};																																			\n"
"	using Class1Typename = Class1*;																															\n"
"																																							\n"
"	class RfrkClass(GenTemplate[TestTemplate], ClassProp1, ClassProp2[ClassSubProp21, ClassSubProp22], ClassProp3) TestClass final : public Class1			\n"
"	{																																						\n"
"		//_RFRK_GENERATED_TestClass																															\n"
"																																							\n"
"		private:																																			\n"
"		#pragma region Variables																															\n"
"																																							\n"
"		RfrkField(FieldProp1, FieldProp2[FieldSubProp21, FieldSubProp22], FieldProp3)																		\n"
"		const volatile int* const&	var1;																													\n"
"																																							\n"
"		#pragma endregion																																	\n"
"																																							\n"
"		protected:																																			\n"
"		RfrkField(FieldProp2[])																																\n"
"		Class1Typename*		var4 = nullptr;																													\n"
"																																							\n"
"		public:																																				\n"
"		RfrkField(FieldProp1)																																\n"
"		static int var2 : 1;																																\n"
"																																							\n"
"		#pragma region Methods																																\n"
"																																							\n"
"		RfrkMethod(MethodProp1, MethodProp2[MethodSubProp21, MethodSubProp22], MethodProp3)																	\n"
"		virtual const volatile int* const& someMethod(int param1, Class1Typename& param2) const final;														\n"
"																																							\n"
"		RfrkMethod(MethodProp1, MethodProp2[MethodSubProp21, MethodSubProp22], MethodProp3)																	\n"
"		virtual int someMethod2(int param1, class Hey* param2) final																						\n"
"		{																																					\n"
"																																							\n"
"		}																																					\n"
"																																							\n"
"		int dsomeMethod3(int) noexcept;																														\n"
"		//int someMethosd3(int) override;																													\n"
"		virtual int someMethodd3(int) const;																												\n"
"																																							\n"
"		#pragma endregion																																	\n"
"																																							\n"
"		RfrkField()																																			\n"
"		float const		var3;																																\n"
"	};																																						\n"
"																																							\n"
"	enum class RfrkEnum(GenTemplate[TestTemplate], EnumProp1, EnumProp2[EnumSubProp21, EnumSubProp22], EnumProp3) MyEnum : uint8_t							\n"
"	{																																						\n"
"		EVal1 RfrkEnumVal(EnumValueProp1, EnumValueProp2[EnumValueSubProp21, EnumValueSubProp22], EnumValueProp3) = 1 << 0,									\n"
"			EVal2 = 1 << 1,																																	\n"
"			a,																																				\n"
"			b,																																				\n"
"			Count = 42,																																		\n"
"			c																																				\n"
"	};																																						\n"
"																																							\n"
"	class RfrkClass() TestClass2																															\n"
"	{																																						\n"
"		//_RFRK_GENERATED_TestClass2																														\n"
"																																							\n"
"		private:																																			\n"
"		#pragma region Variables																															\n"
"																																							\n"
"		RfrkField()																																			\n"
"		int				var1 = 42;																															\n"
"		float			var2;																																\n"
"																																							\n"
"		#pragma endregion																																	\n"
"																																							\n"
"		protected:																																			\n"
"																																							\n"
"		public:																																				\n"
"		#pragma region Methods																																\n"
"																																							\n"
"		RfrkMethod()																																		\n"
"		static int someMethod(int);																															\n"
"		int dsomeMethod3(int) noexcept;																														\n"
"		virtual int someMethodd3(int) const;																												\n"
"																																							\n"
"		#pragma endregion																																	\n"
"	};																																						\n"
"}																																							\n";

void setupParser(refureku::Parser& parser)
{
	//Setup parser settings
	parser.parsingSettings.shouldAbortParsingOnFirstError = false;

	parser.parsingSettings.propertyParsingSettings.ignoredCharacters.insert(' ');	//Ignore white space
	parser.parsingSettings.propertyParsingSettings.subPropertySeparator = ',';

	//ClassProp1, ClassProp2[ClassSubProp21, ClassSubProp22], ClassProp3
	refureku::PropertyRules& classPropertyRules = parser.parsingSettings.propertyParsingSettings.classPropertyRules;

	classPropertyRules.addSimplePropertyRule("ClassProp1");
	classPropertyRules.addSimplePropertyRule("ClassProp2");
	classPropertyRules.addComplexPropertyRule("ClassProp2", "ClassSubProp2[1-9]");
	classPropertyRules.addSimplePropertyRule("ClassProp3");

	//MethodProp1, MethodProp2[MethodSubProp21, MethodSubProp22], MethodProp3
	refureku::PropertyRules& methodPropertyRules = parser.parsingSettings.propertyParsingSettings.methodPropertyRules;

	methodPropertyRules.addSimplePropertyRule("MethodProp1");
	methodPropertyRules.addSimplePropertyRule("MethodProp2");
	methodPropertyRules.addComplexPropertyRule("MethodProp2", "MethodSubProp2[1-9]");
	methodPropertyRules.addSimplePropertyRule("MethodProp3");

	//FieldProp1, FieldProp2[FieldSubProp21, FieldSubProp22], FieldProp3
	refureku::PropertyRules& fieldPropertyRules = parser.parsingSettings.propertyParsingSettings.fieldPropertyRules;

	fieldPropertyRules.addSimplePropertyRule("FieldProp1");
	fieldPropertyRules.addSimplePropertyRule("FieldProp2");
	fieldPropertyRules.addComplexPropertyRule("FieldProp2", "FieldSubProp2[1-9]");
	fieldPropertyRules.addSimplePropertyRule("FieldProp3");

	//EnumProp1, EnumProp2[EnumSubProp21, EnumSubProp22], EnumProp3
	refureku::PropertyRules& enumPropertyRules = parser.parsingSettings.propertyParsingSettings.enumPropertyRules;

	enumPropertyRules.addSimplePropertyRule("EnumProp1");
	enumPropertyRules.addSimplePropertyRule("EnumProp2");
	enumPropertyRules.addComplexPropertyRule("EnumProp2", "EnumSubProp2[1-9]");
	enumPropertyRules.addSimplePropertyRule("EnumProp3");

	//EnumValueProp1, EnumValueProp2[EnumValueSubProp21, EnumValueSubProp22], EnumValueProp3
	refureku::PropertyRules& enumValuePropertyRules = parser.parsingSettings.propertyParsingSettings.enumValuePropertyRules;

	enumValuePropertyRules.addSimplePropertyRule("EnumValueProp1");
	enumValuePropertyRules.addSimplePropertyRule("EnumValueProp2");
	enumValuePropertyRules.addComplexPropertyRule("EnumValueProp2", "EnumValueSubProp2[1-9]");
	enumValuePropertyRules.addSimplePropertyRule("EnumValueProp3");
}

int main()
{
	//Create the file we are going to read
	std::ofstream testFileStream((fs::current_path() / "TestClass.h").string(), std::ios::out | std::ios::trunc);
	testFileStream << TestFile;
	testFileStream.close();

	//Setup parser
	refureku::Parser parser;

	setupParser(parser);

	//Setup FileGenerator
	refureku::FileGenerator fg;

	fs::path includeDirPath	= fs::current_path();
	fs::path pathToFile		= includeDirPath / "TestClass.h";

	fg.pathToGeneratedFilesFolder = includeDirPath / "Generated";

	fg.addGeneratedCodeTemplate("TestTemplate", new refureku::TestGeneratedCodeTemplate());
	fg.setDefaultClassTemplate("TestTemplate");

	//Add files/directories to parse
	fg.addFile(pathToFile);

	refureku::FileGenerationResult fgr = fg.generateFiles(parser, true);

	std::cout << "File generation: " << ((fgr.completed) ? "Success" : "Failure") << std::endl;

	for (refureku::ParsingError& pe : fgr.parsingErrors)
	{
		std::cout << pe << std::endl;
	}

	for (refureku::FileGenerationError& fge : fgr.fileGenerationErrors)
	{
		std::cout << fge << std::endl;
	}

	return EXIT_SUCCESS;
}