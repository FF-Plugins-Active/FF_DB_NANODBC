// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

// Custom Includes
#include "NANODBC_Connection.h"

#include "NANODBC_BPLibrary.generated.h"

UCLASS()
class UNANODBC_BPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintPure, meta = (DisplayName = "NANODBC - INSERT INTO STATEMENT TEMPLATE", Keywords = "odbc, sap, statement, template, insert, into"), Category = "Frozen Forest|Database|NANODBC")
	static FString NANODBC_Insert_Into(FString Target, TSet<FString> Placeholders);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "NANODBC - SELECT FROM", Keywords = "odbc, sap, statement, template, insert, into"), Category = "Frozen Forest|Database|NANODBC")
	static FString NANODBC_Select_From(FString Target, FString Where, TSet<FString> Placeholders);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "NANODBC - Print MetaData", Keywords = "odbc, sap, print, metadata"), Category = "Frozen Forest|Database|NANODBC")
	static FJsonObjectWrapper NANODBC_Print_MetaData(FNANODBC_MetaData In_MetaData);

};