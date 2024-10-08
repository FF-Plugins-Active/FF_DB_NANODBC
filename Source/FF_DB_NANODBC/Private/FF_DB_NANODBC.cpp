// Copyright Epic Games, Inc. All Rights Reserved.

#include "FF_DB_NANODBC.h"

#define LOCTEXT_NAMESPACE "FFF_DB_NANODBCModule"

void FFF_DB_NANODBCModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FFF_DB_NANODBCModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFF_DB_NANODBCModule, FF_DB_NANODBC)