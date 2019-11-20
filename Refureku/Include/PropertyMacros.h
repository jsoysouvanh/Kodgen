#pragma once

#ifdef REFUREKU_PARSING
	#define RfrkClass(...)		__attribute__((annotate(RefurekuClass:#__VA_ARGS__)))
	#define RfrkStruct(...)		__attribute__((annotate(RefurekuStruct:#__VA_ARGS__)))
	#define RfrkEnum(...)		__attribute__((annotate(RefurekuEnum:#__VA_ARGS__)))
	#define RfrkEnumVal(...)	__attribute__((annotate(RefurekuEnumValue:#__VA_ARGS__)))
	#define RfrkField(...)		__attribute__((annotate(RefurekuField:#__VA_ARGS__)))
	#define RfrkMethod(...)		__attribute__((annotate(RefurekuMethod:#__VA_ARGS__)))
#else
	#define RfrkClass(...)
	#define RfrkStruct(...)
	#define RfrkEnum(...)
	#define RfrkEnumVal(...)
	#define RfrkField(...)
	#define RfrkMethod(...)
#endif