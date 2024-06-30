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

	UFUNCTION(BlueprintPure, meta = (DisplayName = "NANODBC - INSERT INTO TEMPLATE", Keywords = "odbc, nanodbc, statement, template, query, insert, into"), Category = "Frozen Forest|Database|NANODBC")
	static FString NANODBC_Insert_Into(FString Target, TMap<FString, FString> KeyValue);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "NANODBC - SELECT FROM TEMPLATE", Keywords = "odbc, nanodbc, statement, template, query, select, from"), Category = "Frozen Forest|Database|NANODBC")
	static FString NANODBC_Select_From(FString Target, FString Where, TSet<FString> PlaceHolders);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "NANODBC - Print MetaData", Keywords = "odbc, nanodbc, print, meta, data"), Category = "Frozen Forest|Database|NANODBC")
	static FJsonObjectWrapper NANODBC_Print_MetaData(FNANODBC_MetaData In_MetaData);
	
	UFUNCTION(BlueprintPure, meta = (DisplayName = "NANODBC - Convert Data Structure To Int32", Keywords = "odbc, nanodbc, convert, data, structure, int, int32"), Category = "Frozen Forest|Database|NANODBC")
	static int32 NANODBC_ConvertToInt(FNANODBC_DataValue In_Data);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "NANODBC - Convert Data Structure To String", Keywords = "odbc, nanodbc, convert, data, structure, fstring, string, text"), Category = "Frozen Forest|Database|NANODBC")
	static FString NANODBC_ConvertToString(FNANODBC_DataValue In_Data);

};