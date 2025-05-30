// Copyright Epic Games, Inc. All Rights Reserved.

#include "TAPython.h"

#include "PythonScriptTypes.h"
#include "IPythonScriptPlugin.h"
#include "Interfaces/IPluginManager.h"


#define LOCTEXT_NAMESPACE "FTAPythonModule"

void FTAPythonModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	RunStartupScripts();
}

void FTAPythonModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

void FTAPythonModule::ExecutePythonScript()
{
	IPythonScriptPlugin* PythonPlugin = IPythonScriptPlugin::Get();
	if (!PythonPlugin || !PythonPlugin->IsPythonAvailable())
	{
		UE_LOG(LogTemp, Error, TEXT("[TAPython] PythonScriptPlugin not available or Python not available."));
		return;
	}

	FPythonCommandEx PythonCommand;
	PythonCommand.Command = TEXT("\"E:\\UE\\UE554Tool\\Plugins\\TAPython\\Source\\TAPython\\Scripts\\Startup.py\" --foo bar");
	PythonCommand.ExecutionMode = EPythonCommandExecutionMode::ExecuteFile;

	if (PythonPlugin->ExecPythonCommandEx(PythonCommand))
	{
		UE_LOG(LogTemp, Log, TEXT("[TAPython] Python script executed successfully: %s"), *PythonCommand.CommandResult);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[TAPython] Python script execution failed: %s"), *PythonCommand.CommandResult);
	}
}

void FTAPythonModule::RunStartupScripts()
{
	IPythonScriptPlugin* PythonPlugin = IPythonScriptPlugin::Get();
	if (!PythonPlugin)
	{
		UE_LOG(LogTemp, Error, TEXT("[TAPython] PythonScriptPlugin not available."));
		return;
	}

	PythonPlugin->OnPythonInitialized().AddRaw(this, &FTAPythonModule::ExecutePythonScript);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTAPythonModule, TAPython)