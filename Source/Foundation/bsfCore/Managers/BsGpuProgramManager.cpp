//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Managers/BsGpuProgramManager.h"
#include "RenderAPI/BsRenderAPI.h"

using namespace bs;

namespace bs { namespace ct
{
String sNullLang = "null";

/** Null GPU program used in place of GPU programs we cannot create. Null programs don't do anything. */
class NullProgram final : public GpuProgram
{
public:
	NullProgram()
		: GpuProgram(GpuProgramCreateInformation())
	{}

	~NullProgram() = default;

	bool IsSupported() const { return false; }
};

SPtr<GpuProgramBytecode> NullProgramFactory::CompileBytecode(const GpuProgramCreateInformation& desc)
{
	auto bytecode = B3DMakeShared<GpuProgramBytecode>();
	bytecode->CompilerId = "Null";

	return bytecode;
}

GpuProgramManager::GpuProgramManager()
{
	mNullFactory = B3DNew<NullProgramFactory>();
	AddFactory(sNullLang, mNullFactory);
}

GpuProgramManager::~GpuProgramManager()
{
	B3DDelete((NullProgramFactory*)mNullFactory);
}

void GpuProgramManager::AddFactory(const String& language, GpuProgramFactory* factory)
{
	Lock lock(mMutex);

	mFactories[language] = factory;
}

void GpuProgramManager::RemoveFactory(const String& language)
{
	Lock lock(mMutex);

	auto iter = mFactories.find(language);
	if(iter != mFactories.end())
		mFactories.erase(iter);
}

GpuProgramFactory* GpuProgramManager::GetFactory(const String& language)
{
	auto iter = mFactories.find(language);
	if(iter == mFactories.end())
		iter = mFactories.find(sNullLang);

	return iter->second;
}

bool GpuProgramManager::IsLanguageSupported(const String& lang)
{
	Lock lock(mMutex);

	auto iter = mFactories.find(lang);
	return iter != mFactories.end();
}

SPtr<GpuProgramBytecode> GpuProgramManager::CompileBytecode(const GpuProgramCreateInformation& desc)
{
	GpuProgramFactory* factory = GetFactory(desc.Language);
	return factory->CompileBytecode(desc);
}
}}
