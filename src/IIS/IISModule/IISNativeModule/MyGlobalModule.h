#pragma once

class MyGlobalModule : public CGlobalModule
{
public:

	auto OnGlobalConfigurationChange(IN IGlobalConfigurationChangeProvider * pProvider) -> GLOBAL_NOTIFICATION_STATUS
	{
		HRESULT hr = S_OK;
		return GL_NOTIFICATION_CONTINUE;
	}

	auto OnGlobalStopListening(IN IGlobalStopListeningProvider * pProvider) -> GLOBAL_NOTIFICATION_STATUS
	{
		HRESULT hr = S_OK;
		return GL_NOTIFICATION_CONTINUE;
	}

	VOID Terminate()
	{
		delete this;
	}

	MyGlobalModule()
	{
	}

	~MyGlobalModule()
	{
	}
};