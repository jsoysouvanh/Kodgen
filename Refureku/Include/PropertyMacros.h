#pragma once

#ifdef REFUREKU_PARSING
	#define RfrkClass(...)		__attribute__((annotate("RfrkClass:"#__VA_ARGS__)))
	#define RfrkStruct(...)		__attribute__((annotate("RfrkStruct:"#__VA_ARGS__)))
	#define RfrkEnum(...)		__attribute__((annotate("RfrkEnum:"#__VA_ARGS__)))
	#define RfrkEnumVal(...)	__attribute__((annotate("RfrkEnumVal:"#__VA_ARGS__)))
	#define RfrkField(...)		__attribute__((annotate("RfrkField:"#__VA_ARGS__)))
	#define RfrkMethod(...)		__attribute__((annotate("RfrkMethod:"#__VA_ARGS__)))
#else
	#define RfrkClass(...)
	#define RfrkStruct(...)
	#define RfrkEnum(...)
	#define RfrkEnumVal(...)
	#define RfrkField(...)
	#define RfrkMethod(...)
#endif