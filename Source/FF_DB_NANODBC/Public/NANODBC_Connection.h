#pragma once

#include "CoreMinimal.h"

// UE Includes.
#include "JsonObjectWrapper.h"
#include "JsonUtilities.h"

// Custom Includes.
#include "NANODBC_Includes.h"

#include "NANODBC_Connection.generated.h"

using namespace nanodbc;

USTRUCT(BlueprintType)
struct FF_DB_NANODBC_API FNANODBC_MetaData
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, AdvancedDisplay)
	int32 ColumnDecimalDigit = 0;

	UPROPERTY(BlueprintReadOnly, AdvancedDisplay)
	int32 ColumnSize = 0;

	UPROPERTY(BlueprintReadOnly, AdvancedDisplay)
	int32 ColumnNumber = 0;

	UPROPERTY(BlueprintReadOnly, AdvancedDisplay)
	int32 ColumnType = 0;

	UPROPERTY(BlueprintReadOnly, AdvancedDisplay)
	FString ColumnTypeName;

	UPROPERTY(BlueprintReadOnly, AdvancedDisplay)
	FString ColumnName;

	UPROPERTY(BlueprintReadOnly, AdvancedDisplay)
	bool bIsNull = false;

};


UCLASS(BlueprintType)
class FF_DB_NANODBC_API UNANODBC_Connection : public UObject
{
	GENERATED_BODY()

protected:

	FString ConnectionId;
	connection NANODBC_Connection;

public:

	virtual void SetConnection(connection In_Connection);

	UFUNCTION(BlueprintCallable)
	virtual bool SetConnectionId(FString In_Id);
	
	UFUNCTION(BlueprintCallable)
	virtual FString GetConnectionId();

	UFUNCTION(BlueprintCallable)
	virtual bool JustExecute(FString& Out_Code, FString SQL_Statement);

	UFUNCTION(BlueprintCallable)
	virtual bool ExecuteAndGetResult(FString& Out_Code, int32& AffectedRows, UNANODBC_Result*& Out_Result, FString SQL_Statement);

};

UCLASS(BlueprintType)
class FF_DB_NANODBC_API UNANODBC_Result : public UObject
{
	GENERATED_BODY()

protected:

	result QueryResult;

public:

	virtual bool SetQueryResult(result In_Result);

	UFUNCTION(BlueprintCallable)
	virtual bool GetColumnsCount(int32& Out_Count);

	UFUNCTION(BlueprintCallable)
	virtual bool GetMetaData(FString& Out_Code, FNANODBC_MetaData& Out_MetaData, int32 ColumnIndex);

	UFUNCTION(BlueprintCallable)
	virtual bool GetInt(FString& Out_Code, TArray<int32>& Out_Value, const FString In_ColumnName);

	UFUNCTION(BlueprintCallable)
	virtual bool GetString(FString& Out_Code, TArray<FString>& Out_Value, const FString In_ColumnName);

	UFUNCTION(BlueprintCallable)
	virtual bool GetBool(FString& Out_Code, TArray<bool>& Out_Value, const FString In_ColumnName);

};