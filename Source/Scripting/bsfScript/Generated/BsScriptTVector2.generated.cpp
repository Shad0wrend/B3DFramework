//********************************* bs::framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsScriptTVector2.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"

namespace bs
{
	ScriptTVector2_float_::ScriptTVector2_float_()
	{ }

	MonoObject* ScriptTVector2_float_::Box(const TVector2<float>& value)
	{
		return MonoUtil::Box(sInteropMetaData.ScriptClass->GetInternalClass(), (void*)&value);
	}

	TVector2<float> ScriptTVector2_float_::Unbox(MonoObject* value)
	{
		return *(TVector2<float>*)MonoUtil::Unbox(value);
	}


	ScriptTVector2_double_::ScriptTVector2_double_()
	{ }

	MonoObject* ScriptTVector2_double_::Box(const TVector2<double>& value)
	{
		return MonoUtil::Box(sInteropMetaData.ScriptClass->GetInternalClass(), (void*)&value);
	}

	TVector2<double> ScriptTVector2_double_::Unbox(MonoObject* value)
	{
		return *(TVector2<double>*)MonoUtil::Unbox(value);
	}


	ScriptTVector2_int32_t_::ScriptTVector2_int32_t_()
	{ }

	MonoObject* ScriptTVector2_int32_t_::Box(const TVector2<int32_t>& value)
	{
		return MonoUtil::Box(sInteropMetaData.ScriptClass->GetInternalClass(), (void*)&value);
	}

	TVector2<int32_t> ScriptTVector2_int32_t_::Unbox(MonoObject* value)
	{
		return *(TVector2<int32_t>*)MonoUtil::Unbox(value);
	}


	ScriptTVector2_uint32_t_::ScriptTVector2_uint32_t_()
	{ }

	MonoObject* ScriptTVector2_uint32_t_::Box(const TVector2<uint32_t>& value)
	{
		return MonoUtil::Box(sInteropMetaData.ScriptClass->GetInternalClass(), (void*)&value);
	}

	TVector2<uint32_t> ScriptTVector2_uint32_t_::Unbox(MonoObject* value)
	{
		return *(TVector2<uint32_t>*)MonoUtil::Unbox(value);
	}

}
