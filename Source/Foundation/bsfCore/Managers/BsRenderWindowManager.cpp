//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Managers/BsRenderWindowManager.h"
#include "Platform/BsPlatform.h"
#include "BsCoreApplication.h"

using namespace std::placeholders;
using namespace bs;

SPtr<RenderWindow> RenderWindowManager::Create(const RenderWindowCreateInformation& createInformation, const SPtr<RenderWindow>& parentWindow)
{
	const u32 id = mNextWindowId++;

	SPtr<RenderWindow> renderWindow = CreateImplementation(createInformation, id, parentWindow);
	renderWindow->SetShared(renderWindow);

	mWindows[renderWindow->mWindowId] = renderWindow.get();

	if(renderWindow->GetRenderWindowProperties().IsModal)
		mModalWindowStack.push_back(renderWindow.get());

	renderWindow->Initialize();
	return renderWindow;
}

void RenderWindowManager::NotifyWindowDestroyed(RenderWindow& window)
{
	if(mWindowInFocus == &window)
		mWindowInFocus = nullptr;

	mWindows.erase(window.mWindowId);

	auto found = std::find(begin(mModalWindowStack), end(mModalWindowStack), &window);
	if(found != mModalWindowStack.end())
		mModalWindowStack.erase(found);
}

void RenderWindowManager::NotifyFocusReceived(RenderWindow& window)
{
	if(mWindowInFocus != &window)
	{
		if(mWindowInFocus != nullptr)
			OnFocusLost(*mWindowInFocus);

		OnFocusGained(window);
		mWindowInFocus = &window;
	}
}

void RenderWindowManager::NotifyFocusLost(RenderWindow& window)
{
	if(mWindowInFocus != nullptr)
	{
		OnFocusLost(*mWindowInFocus);
		mWindowInFocus = nullptr;
	}
}

void RenderWindowManager::NotifyMouseLeft(RenderWindow& window)
{
	OnMouseLeftWindow(window);
}

void RenderWindowManager::Update()
{
	// TODO - Can be removed, but keeping it for now in case there are ordering issues with render thread sync
}

Vector<RenderWindow*> RenderWindowManager::GetRenderWindows() const
{
	Vector<RenderWindow*> windows;
	for(auto& windowPair : mWindows)
		windows.push_back(windowPair.second);

	return windows;
}

RenderWindow* RenderWindowManager::GetTopMostModal() const
{
	if(mModalWindowStack.empty())
		return nullptr;

	return mModalWindowStack.back();
}

RenderWindow* RenderWindowManager::GetRenderProxyObject(const ct::RenderWindow* window) const
{
	auto iterFind = mWindows.find(window->mWindowId);

	if(iterFind != mWindows.end())
		return iterFind->second;

	return nullptr;
}
