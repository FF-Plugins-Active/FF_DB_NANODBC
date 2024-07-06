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
struct FF_DB_NANODBC_API FNANODBC_DataValue
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, meta = (ToolTip = "Use it for \"datetime, nvarchar, text\""))
	FString String;
	
	UPROPERTY(BlueprintReadOnly, meta = (ToolTip = "Use it for \"int32, timestamp\""))
	int32 Integer32 = 0;

	UPROPERTY(BlueprintReadOnly, meta = (ToolTip = "Use it for \"int32, timestamp\""))
	int64 Integer64 = 0;
	
	UPROPERTY(BlueprintReadOnly, meta = (ToolTip = ""))
	float Double = (double)0.f;
	
	UPROPERTY(BlueprintReadOnly, meta = (ToolTip = ""))
	bool Bool = false;
	
	UPROPERTY(BlueprintReadOnly, meta = (ToolTip = ""))
	FDateTime DateTime;

	UPROPERTY(BlueprintReadOnly)
	int32 DataType = 0;

	UPROPERTY(BlueprintReadOnly)
	FString DataTypeName;

	UPROPERTY(BlueprintReadOnly)
	FString ColumnName;

	UPROPERTY(BlueprintReadOnly)
	FString Preview;

	UPROPERTY(BlueprintReadOnly)
	FString Note;

};

USTRUCT(BlueprintType)
struct FF_DB_NANODBC_API FNANODBC_MetaData
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, AdvancedDisplay)
	FString ColumnName;

	UPROPERTY(BlueprintReadOnly, AdvancedDisplay)
	int32 ColumnNumber = 0;

	UPROPERTY(BlueprintReadOnly, AdvancedDisplay)
	int32 ColumnType = 0;

	UPROPERTY(BlueprintReadOnly, AdvancedDisplay)
	FString ColumnTypeName;

	UPROPERTY(BlueprintReadOnly, AdvancedDisplay)
	int32 ColumnDecimalDigit = 0;

	UPROPERTY(BlueprintReadOnly, AdvancedDisplay)
	int32 ColumnSize = 0;

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
	virtual bool SetConnectionId(FString In_Id);
	virtual connection GetConnection();

	UFUNCTION(BlueprintCallable)
	virtual bool IsConnectionValid();	
	
	UFUNCTION(BlueprintCallable)
	virtual FString GetConnectionId();

	UFUNCTION(BlueprintCallable)
	virtual bool JustExecute(FString& Out_Code, FString SQL_Query);

	UFUNCTION(BlueprintCallable)
	virtual bool ExecuteAndGetResult(FString& Out_Code, UNANODBC_Result*& Out_Result, FString SQL_Query);

};

UCLASS(BlueprintType)
class FF_DB_NANODBC_API UNANODBC_Result : public UObject
{
	GENERATED_BODY()

protected:

	result QueryResult;
	TMap<FVector2D, FNANODBC_DataValue> All_Data;
	int32 RowsCount = 0;

public:

	virtual bool SetQueryResult(FString& Out_Code, result In_Result);

	UFUNCTION(BlueprintPure)
	virtual bool GetAffectedRows(FString& Out_Code, int32& AffectedRows);

	UFUNCTION(BlueprintPure)
	virtual int32 GetColumnsCount(FString& Out_Code);

	UFUNCTION(BlueprintPure)
	virtual int32 GetRowsCount(FString& Out_Code);

	UFUNCTION(BlueprintCallable)
	virtual bool GetMetaData(FString& Out_Code, TArray<FNANODBC_MetaData>& Out_MetaData);

	UFUNCTION(BlueprintCallable)
	virtual bool GetDataFromRow(FString& Out_Code, TArray<FNANODBC_DataValue>& Out_Value, const int32 RowIndex);

	UFUNCTION(BlueprintCallable)
	virtual bool GetDataFromColumnIndex(FString& Out_Code, TArray<FNANODBC_DataValue>& Out_Value, const int32 ColumnIndex);

	UFUNCTION(BlueprintCallable)
	virtual bool GetDataFromColumnName(FString& Out_Code, TArray<FNANODBC_DataValue>& Out_Value, FString ColumnName);

	UFUNCTION(BlueprintCallable)
	virtual bool GetSingleData(FString& Out_Code, FNANODBC_DataValue& Out_Value, FVector2D Position);

};