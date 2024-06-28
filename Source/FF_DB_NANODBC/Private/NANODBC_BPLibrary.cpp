// Copyright Epic Games, Inc. All Rights Reserved.

#include "NANODBC_BPLibrary.h"
#include "FF_DB_NANODBC.h"

UNANODBC_BPLibrary::UNANODBC_BPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

FString UNANODBC_BPLibrary::NANODBC_Insert_Into(FString Target, TSet<FString> Placeholders)
{
	FString MainStatement = "INSERT INTO " + Target + " (";

	const int32 PlaceHoldersLenght = Placeholders.Num();

	FString ValueArea;
	for (int32 Index_PlaceHolder = 0; Index_PlaceHolder < PlaceHoldersLenght; Index_PlaceHolder++)
	{
		FString EachPlaceHolder = Placeholders.Array()[Index_PlaceHolder];
		MainStatement += EachPlaceHolder;

		ValueArea += "?";

		if (Index_PlaceHolder != PlaceHoldersLenght - 1)
		{
			MainStatement += ", ";

			ValueArea += ", ";
		}
	}

	ValueArea += ")";
	MainStatement += ") VALUES (" + ValueArea;

	return MainStatement;
}

FString UNANODBC_BPLibrary::NANODBC_Select_From(FString Target, FString Where, TSet<FString> Placeholders)
{
	return FString();
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

	JSonObject.JsonObject->SetBoolField("Is Nullable", In_MetaData.bIsNull);

	return JSonObject;
}