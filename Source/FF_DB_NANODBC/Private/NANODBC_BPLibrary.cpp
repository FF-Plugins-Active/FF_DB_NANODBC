// Copyright Epic Games, Inc. All Rights Reserved.

#include "NANODBC_BPLibrary.h"
#include "FF_DB_NANODBC.h"

// UE Includes.
#include "Kismet/KismetStringLibrary.h"

UNANODBC_BPLibrary::UNANODBC_BPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

FString UNANODBC_BPLibrary::NANODBC_Insert_Into(FString Target, TMap<FString, FString> KeyValue)
{
	TArray<FString> Array_Keys;
	KeyValue.GenerateKeyArray(Array_Keys);

	TArray<FString> Array_Values;
	KeyValue.GenerateValueArray(Array_Values);

	const FString PlaceHoldersString = " (" + UKismetStringLibrary::JoinStringArray(Array_Keys, ", ") + ") ";
	const FString ValuesString = " (" + UKismetStringLibrary::JoinStringArray(Array_Values, ", ") + ") ";
	const FString MainQuery = "INSERT INTO " + Target + PlaceHoldersString + "VALUES" + ValuesString;

	return MainQuery;
}

FString UNANODBC_BPLibrary::NANODBC_Select_From(FString Target, FString Where, TSet<FString> PlaceHolders)
{
	TArray<FString> Array_PlaceHolders = PlaceHolders.Array();
	const FString PlaceHolderString = " " + UKismetStringLibrary::JoinStringArray(Array_PlaceHolders, ", ") + " ";
	const FString MainQuery = "SELECT" + PlaceHolderString + "FROM " + Target + " " + "WHERE " + Where;
	
	return MainQuery;
}

FJsonObjectWrapper UNANODBC_BPLibrary::NANODBC_Print_MetaData(FNANODBC_MetaData In_MetaData)
{
	FJsonObjectWrapper JSonObject;

	JSonObject.JsonObject->SetStringField("Column Name", In_MetaData.ColumnName);
	JSonObject.JsonObject->SetStringField("Column Type Name", In_MetaData.ColumnTypeName);

	JSonObject.JsonObject->SetNumberField("Column Type", In_MetaData.ColumnType);
	JSonObject.JsonObject->SetNumberField("Column Lenght", In_MetaData.ColumnDecimalDigit);
	JSonObject.JsonObject->SetNumberField("Column Display Size", In_MetaData.ColumnSize);
	JSonObject.JsonObject->SetNumberField("Column Octet Lenght", In_MetaData.ColumnNumber);

	return JSonObject;
}