#pragma once

/**
*	You can redefine these macro names as you like,
*	but never change their expansion
*/

#ifdef KODGEN_PARSING
	#define KGClass(...)	__attribute__((annotate("KGC:"#__VA_ARGS__)))
	#define KGStruct(...)	__attribute__((annotate("KGS:"#__VA_ARGS__)))
	#define KGEnum(...)		__attribute__((annotate("KGE:"#__VA_ARGS__)))
	#define KGEnumVal(...)	__attribute__((annotate("KGEV:"#__VA_ARGS__)))
	#define KGField(...)	__attribute__((annotate("KGF:"#__VA_ARGS__)))
	#define KGMethod(...)	__attribute__((annotate("KGM:"#__VA_ARGS__)))
#else
	#define KGClass(...)
	#define KGStruct(...)
	#define KGEnum(...)
	#define KGEnumVal(...)
	#define KGField(...)
	#define KGMethod(...)
#endif