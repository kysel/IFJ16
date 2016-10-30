/** Interpretr jazyka IFJ16
* @file hacks.h
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
*/

#ifndef HACKS_H_
#define HACKS_H_

#define GENERATE_ENUM2(ENUM, NAME) ENUM,
#define GENERATE_ENUM3(ENUM, NAME, VALUE) ENUM=VALUE,
#define GENERATE_CASE2(ENUM, STRING) case ENUM: return STRING;
#define GENERATE_CASE3(ENUM, STRING, VALUE) case ENUM: return STRING;

#define GET_MACRO(_1,_2,_3,NAME,...) NAME
#define EXPAND(x) x
#define GENERATE_ENUM(...) EXPAND( GET_MACRO(__VA_ARGS__, GENERATE_ENUM3, GENERATE_ENUM2) (__VA_ARGS__) )
#define GENERATE_CASE(...) EXPAND( GET_MACRO(__VA_ARGS__, GENERATE_CASE3, GENERATE_CASE2) (__VA_ARGS__) )

#endif
