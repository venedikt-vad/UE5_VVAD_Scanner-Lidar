// Copyright Epic Games, Inc. All Rights Reserved.

#include "VVAD_Scanner.h"

#define LOCTEXT_NAMESPACE "FVVAD_ScannerModule"

void FVVAD_ScannerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FVVAD_ScannerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FVVAD_ScannerModule, VVAD_Scanner)