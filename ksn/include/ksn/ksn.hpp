
#ifndef _KSN_HPP_
#define _KSN_HPP_

#ifdef __cplusplus



import std;

import <string.h>;
import <stdlib.h>;
import <stddef.h>;
import <stdint.h>;
import <float.h>;
import <limits.h>;
import <assert.h>;



//Redefine some basic ksn defines

#undef _KSN
#undef _KSN_BEGIN
#undef _KSN_END
#undef _KSN_DETAIL_BEGIN
#undef _KSN_DETAIL_END
#undef _KSN_EXPORT_BEGIN
#undef _KSN_EXPORT_END

#define _KSN ::ksn::
#define _KSN_BEGIN namespace ksn{
#define _KSN_END }
#define _KSN_DETAIL_BEGIN namespace detail{
#define _KSN_DETAIL_END }
#define _KSN_EXPORT_BEGIN export namespace ksn {
#define _KSN_EXPORT_END }





//Check for current compiler

#if defined __GNUC__ || defined __GNUG__
#define _KSN_COMPILER_GNU
#endif


#if defined _MSVC_LANG

	#define _KSN_COMPILER_MSVC

#elif defined __clang__

	#define _KSN_COMPILER_CLANG

#elif defined _KSN_COMPILER_GNU

	#define _KSN_COMPILER_GCC

#else

	#define _KSN_COMPILER_UNKNOWN

#endif





//Do compiler-dependent preprocessor stuff
//Stage 1: do things depending on what is the current compiler

#ifdef _KSN_COMPILER_MSVC


	#define _KSN_CPP_VER _MSVC_LANG

	#ifndef _KSN_IS_64
		#ifdef _WIN64
			#define _KSN_IS_64 1
		#else
			#define _KSN_IS_64 0
		#endif
	#endif

	#ifndef LDBL_DECIMAL_DIG
		#define LDBL_DECIMAL_DIG DBL_DECIMAL_DIG
	#endif

	#define _KSN_RESTRICT __restrict


#elif defined _KSN_COMPILER_GNU


	#ifndef _KSN_IS_64
		#if defined __LP64__ || defined __LLP64__
			#define _KSN_IS_64 1
		#else
			#define _KSN_IS_64 0
		#endif
	#endif

	#define _KSN_RESTRICT __restrict


#else


	#ifndef _KSN_IS_64
		#if defined(__LP64__) || defined(_WIN64) || (defined(__x86_64__) && !defined(__ILP32__)) || defined(_M_X64) || defined(__ia64) || defined (_M_IA64) || defined(__aarch64__) || defined(__powerpc64__)
			#define _KSN_IS_64 1
		#else
			#define _KSN_IS_64 0
		#endif
	#endif


#endif





//Do compiler-dependent preprocessor stuff
//Stage 2: do things depending on what is NOT the current compiler

#ifndef _KSN_COMPILER_MSVC

	#define _KSN_CPP_VER __cplusplus

#endif





//Check if the current build is a debug or a release build

#if defined _DEBUG || defined DEBUG
	#define __KSN_DEBUG
#endif

#if defined NDEBUG || defined _NDEBUG
	#define __KSN_RELEASE
#endif


#if defined __KSN_DEBUG && !defined __KSN_RELEASE

	#define _KSN_IS_DEBUG_BUILD 1

#elif defined __KSN_RELEASE && !defined __KSN_DEBUG

	#define _KSN_IS_DEBUG_BUILD 0

#elif !defined __KSN_DEBUG && !defined __KSN_RELEASE

	#ifndef _KSN_NO_IS_DEBUG_WARNING
		#error Failed to define _KSN_IS_DEBUG_BUILD because neither debug nor release macro is defined. \
Define _KSN_IS_DEBUG_BUILD yourself to be 0 or 1 before any ksn header or reconfigure your project settings. \
You can #define _KSN_NO_IS_DEBUG_WARNING to acknowledge the compiler that you've recived this warning (release build will be assumed)
	#endif

	#define _KSN_IS_DEBUG_BUILD 0

#else //defined __KSN_DEBUG && defined __KSN_RELEASE

	#ifndef _KSN_IS_DEBUG_BUILD
		#error Failed to define _KSN_IS_DEBUG_BUILD because both debug and release macros are defined. \
Define _KSN_IS_DEBUG_BUILD yourself to be 0 or 1 before any ksn header or reconfigure your project settings
	#endif

#endif


//Temporary macros
#undef __KSN_DEBUG
#undef __KSN_RELEASE





//Feature testing

#if __cpp_attributes != 0 && defined __has_cpp_attribute

	#if __has_cpp_attribute(nodiscard) >= 201603L
		#define _KSN_NODISCARD [[nodiscard]]
	#endif

	#if __has_cpp_attribute(nodiscard) >= 201907L
		#define _KSN_NODISCARD_BECAUSE(reason) [[nodiscard(#reason)]]
	#endif

	#if __has_cpp_attribute(likely) >= 201803L
		#define _KSN_LIKELY [[likely]]
	#endif

	#if __has_cpp_attribute(unlikely) >= 201803L
		#define _KSN_UNLIKELY [[unlikely]]
	#endif

#endif





//Expression that will only be executed on debug builds
#define _KSN_DEBUG_EXPR(expression) if constexpr (_KSN_IS_DEBUG_BUILD) { expression; } else []{}()





//Conditional exceptions handling

#if defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND)
#define _KSN_HAS_EXCEPTIONS 1
#define _KSN_TRY try {
#define _KSN_CATCH_UNNAMED(type) } catch(type) {
#define _KSN_CATCH(type, name) } catch(type name) { ((void)(name));
#define _KSN_CATCH_ALL } catch(...) {
#define _KSN_CATCH_END }
#define _KSN_RAISE(exception) []{ throw (exception); }()
#define _KSN_RERAISE []{ throw; }()
#else
#define _KSN_HAS_EXCEPTIONS 0
#define _KSN_TRY if _KSN_CONSTEXPR_CONDITION (true) {
#define _KSN_CATCH_UNNAMED(type) } else if _KSN_CONSTEXPR_CONDITION (false) {
#define _KSN_CATCH(type, name) } else if _KSN_CONSTEXPR_CONDITION (false) { type name = *(std::add_pointer_t<std::remove_reference_t<type>>)nullptr;
#define _KSN_CATCH_ALL } else if _KSN_CONSTEXPR_CONDITION (false) {
#define _KSN_CATCH_END }
#define _KSN_RAISE(exception) abort()
#define _KSN_RERAISE abort()
#endif





//Check all the necessary definitions

#ifndef _KSN_IS_64

	#error Failed to define _KSN_IS_64. Please predefine it as 1 if compiling for 64 bits and 0 for 32 bits (Note: other platforms are not supported)

#else

	#define _KSN_IS_64_FAILURE_MESSAGE "Failed to guess whether compiling for 64 bits or not. #define _KSN_IS_64 to be 1 or 0 according to the enviroment your are compiling for"

	#if _KSN_IS_64
		static_assert(sizeof(void*) == 8, _KSN_IS_64_FAILURE_MESSAGE);
		static_assert(SIZE_MAX == UINT64_MAX, _KSN_IS_64_FAILURE_MESSAGE);
	#else
		static_assert(sizeof(void*) == 4, _KSN_IS_64_FAILURE_MESSAGE);
		static_assert(SIZE_MAX == UINT32_MAX, _KSN_IS_64_FAILURE_MESSAGE);
	#endif

	#undef _KSN_IS_64_FAILURE_MESSAGE

#endif


#ifndef _KSN_NO_BYTE_SIZE_CHECK
	static_assert(CHAR_BIT == 8, "The size of a byte does not equal to 8 bits. You can suppress this warning by #define _KSN_NO_BYTE_SIZE_CHECK before any ksn header but you should not expect this library to work properly");
#endif





#ifndef _KSN_NODISCARD
	#define _KSN_NODISCARD
#endif

#ifndef _KSN_NODISCARD_BECAUSE
	#define _KSN_NODISCARD_BECAUSE(reason)
#endif

#ifndef _KSN_LIKELY
	#define _KSN_LIKELY
#endif

#ifndef _KSN_UNLIKELY
	#define _KSN_UNLIKELY
#endif

#ifndef _KSN_RESTRICT
	#define _KSN_RESTRICT
#endif





//Minimal definition of ksn namespace

namespace ksn{

	namespace detail{

		template<bool test>
		consteval void _ct_assert()
		{
			static_assert(test);
		}

	}

}





#else //if !__cplusplus

#error <ksn/ksn.hpp> can only be included into files compiled with a C++ compiler

#endif //!__cplusplus



#endif //!_KSN_HPP_
