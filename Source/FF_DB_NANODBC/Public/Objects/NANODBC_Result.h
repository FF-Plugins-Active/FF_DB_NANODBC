#pragma once

#include "CoreMinimal.h"

// Custom Includes.
#include "NANODBC_Includes.h"

#include "NANODBC_Result.generated.h"

using namespace nanodbc;

USTRUCT(BlueprintType)
struct FF_DB_NANODBC_API FNANODBC_DataValue
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, meta = (ToolTip = "Use it for \"datetime, nvarchar, text\""))
	FString String;

	UPROPERTY(BlueprintReadOnly, meta = (ToolTip = "Use it for \"int32\""))
	int32 Integer32 = 0;

	UPROPERTY(BlueprintReadOnly, meta = (ToolTip = "Use it for \"int32\""))
	int64 Integer64 = 0;

	UPROPERTY(BlueprintReadOnly, meta = (ToolTip = ""))
	double Double = (double)0.f;

	UPROPERTY(BlueprintReadOnly, meta = (ToolTip = ""))
	bool Bool = false;

	UPROPERTY(BlueprintReadOnly, meta = (ToolTip = ""))
	FDateTime DateTime;

	UPROPERTY(BlueprintReadOnly, meta = (ToolTip = ""))
	int32 DataType = 0;

	UPROPERTY(BlueprintReadOnly, meta = (ToolTip = ""))
	FString DataTypeName;

	UPROPERTY(BlueprintReadOnly, meta = (ToolTip = ""))
	FString ColumnName;

	UPROPERTY(BlueprintReadOnly, meta = (ToolTip = ""))
	FString Preview;

	UPROPERTY(BlueprintReadOnly, meta = (ToolTip = ""))
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
class FF_DB_NANODBC_API UNANODBC_Result : public UObject
{
	GENERATED_BODY()

protected:

	result QueryResult;
	TMap<FVector2D, FNANODBC_DataValue> Data_Pool;
	
	int32 Count_Row = 0;
	int32 Count_Column = 0;
	int32 Affected_Rows = 0;

public:

	virtual bool SetQueryResult(FString& Out_Code, result In_Result);

	UFUNCTION(BlueprintCallable)
	virtual bool Result_Record(FString& Out_Code);

	UFUNCTION(BlueprintCallable)
	virtual bool Result_Fetch(FString& Out_Code, TArray<FNANODBC_DataValue>& Out_Values, int32 Index_Column);

	UFUNCTION(BlueprintPure)
	virtual int32 GetAffectedRows();

	UFUNCTION(BlueprintPure)
	virtual int32 GetColumnsCount();

	UFUNCTION(BlueprintPure)
	virtual int32 GetRowsCount();

	UFUNCTION(BlueprintCallable)
	virtual bool GetMetaData(FString& Out_Code, TArray<FNANODBC_MetaData>& Out_MetaData);

	UFUNCTION(BlueprintCallable)
	virtual bool GetRow(FString& Out_Code, TArray<FNANODBC_DataValue>& Out_Value, const int32 Index_Row);

	UFUNCTION(BlueprintCallable)
	virtual bool GetColumnFromIndex(FString& Out_Code, TArray<FNANODBC_DataValue>& Out_Value, const int32 Index_Column);

	UFUNCTION(BlueprintCallable)
	virtual bool GetColumnFromName(FString& Out_Code, TArray<FNANODBC_DataValue>& Out_Value, FString ColumnName);

	UFUNCTION(BlueprintCallable)
	virtual bool GetSingleData(FString& Out_Code, FNANODBC_DataValue& Out_Value, FVector2D Position);

};