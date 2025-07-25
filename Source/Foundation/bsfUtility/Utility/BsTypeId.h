//************************************ B3D Framework - Copyright 2025 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsUtilityPrerequisites.h"
#include "String/BsHashedString.h"

namespace b3d
{
	/** @addtogroup General
	 *  @{
	 */

	using TypeId = u64;
	using TypeHash = u64;

	/** Returns a unique type identifier for the provided type. Note the identifiers will NOT remain constant across application runs. */
#if 0 // Note: This doesn't work if the type gets defined in a plugin
#if B3D_UTILITY_EXPORTS
	template <typename T>
	B3D_UTILITY_EXPORT TypeId B3DGetRuntimeTypeId() noexcept
	{
		static char const kTypeId = 0;

		return (u64)&kTypeId;
	}
#else
	template <typename T>
	B3D_UTILITY_EXPORT TypeId B3DGetRuntimeTypeId() noexcept;
#endif
#else // Note: This will report different type IDs across plugins
	template <typename T>
	TypeId B3DGetRuntimeTypeId() noexcept
	{
		static char const kTypeId = 0;

		return (u64)&kTypeId;
	}
#endif

	/** Returns the type name for the class from which this function is called from, using the 'pretty function' macro provided by many compilers. */
	constexpr std::string_view B3DGetTypeNameFromPrettyFunction()
	{
		const std::string_view prettyFunctionString(B3D_PRETTY_FUNCTION);
		auto typeNameStart = prettyFunctionString.find_first_not_of(' ', prettyFunctionString.find_first_of(B3D_PRETTY_FUNCTION_PREFIX) + 1);
		return prettyFunctionString.substr(typeNameStart, prettyFunctionString.find_last_of(B3D_PRETTY_FUNCTION_SUFFIX) - typeNameStart);
	}

	/** Computes a hash value for the current class name. Uses B3DGetTypeNameFromPrettyFunction() to compute the hash from. */
	template<typename HashType = TypeHash>
	constexpr HashType B3DGetCurrentTypeHash()
	{
		constexpr std::string_view typeName = B3DGetTypeNameFromPrettyFunction();
		return THashedString<char, HashType>::CalculateHash(typeName);
	}

	/** @} */
} // namespace b3d
