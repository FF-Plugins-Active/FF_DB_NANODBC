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
	int32 ColumnScale = 0;

	UPROPERTY(BlueprintReadOnly, AdvancedDisplay)
	int32 ColumnPrecision = 0;

	UPROPERTY(BlueprintReadOnly, AdvancedDisplay)
	int32 ColumnDisplaySize = 0;

	UPROPERTY(BlueprintReadOnly, AdvancedDisplay)
	int32 ColumnLenght = 0;

	UPROPERTY(BlueprintReadOnly, AdvancedDisplay)
	int32 ColumnOctetLenght = 0;

	UPROPERTY(BlueprintReadOnly, AdvancedDisplay)
	int32 ColumnType = 0;

	UPROPERTY(BlueprintReadOnly, AdvancedDisplay)
	FString ColumnTypeName;

	UPROPERTY(BlueprintReadOnly, AdvancedDisplay)
	FString BaseColumnName;

	UPROPERTY(BlueprintReadOnly, AdvancedDisplay)
	FString ColumnName;

	UPROPERTY(BlueprintReadOnly, AdvancedDisplay)
	FString ColumnLabel;

	UPROPERTY(BlueprintReadOnly, AdvancedDisplay)
	FString BaseTableName;

	UPROPERTY(BlueprintReadOnly, AdvancedDisplay)
	FString TableName;

	UPROPERTY(BlueprintReadOnly, AdvancedDisplay)
	FString CatalogName;

	UPROPERTY(BlueprintReadOnly, AdvancedDisplay)
	FString SchemaName;

	UPROPERTY(BlueprintReadOnly, AdvancedDisplay)
	bool bIsNullable = false;

	UPROPERTY(BlueprintReadOnly, AdvancedDisplay)
	bool bIsAutoIncrement = false;

	UPROPERTY(BlueprintReadOnly, AdvancedDisplay)
	bool bIsNamed = false;

	UPROPERTY(BlueprintReadOnly, AdvancedDisplay)
	bool bIsCaseSensitive = false;

	UPROPERTY(BlueprintReadOnly, AdvancedDisplay)
	bool bIsReadOnly = false;

	UPROPERTY(BlueprintReadOnly, AdvancedDisplay)
	bool bIsSearchable = false;

	UPROPERTY(BlueprintReadOnly, AdvancedDisplay)
	bool bIsSigned = false;
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
	virtual bool Connection_Start(FString& Out_Code, FString In_Server, FString In_UserName, FString In_Password, bool bUseAutoCommit);

	UFUNCTION(BlueprintCallable)
	virtual bool Connection_Stop(FString& Out_Code);

	UFUNCTION(BlueprintCallable)
	virtual bool PrepareStatement(FString& Out_Code, UNANODBC_Statement*& Out_Statement, FString SQL_Statement);

};

UCLASS(BlueprintType)
class FF_DB_NANODBC_API UNANODBC_Statement : public UObject
{
	GENERATED_BODY()

protected:



public:

	UFUNCTION(BlueprintCallable)
	virtual void SetInt(int32 Value, int32 ParamIndex = 1);

	UFUNCTION(BlueprintCallable)
	virtual void SetString(FString Value, int32 ParamIndex = 1);

	UFUNCTION(BlueprintCallable)
	virtual void AddBatch();

	UFUNCTION(BlueprintCallable)
	virtual void ExecuteBatch(FString& Out_Code);

	UFUNCTION(BlueprintCallable)
	virtual bool CommitStatement(FString& Out_Code);

	UFUNCTION(BlueprintCallable)
	virtual bool ExecuteQuery(FString& Out_Code, UNANODBC_Result*& Out_Result);

};

UCLASS(BlueprintType)
class FF_DB_NANODBC_API UNANODBC_Result : public UObject
{
	GENERATED_BODY()

protected:

	

public:

	UFUNCTION(BlueprintCallable)
	virtual bool GetColumnCount(int32& ColumnCount);

	UFUNCTION(BlueprintCallable)
	virtual bool GetMetaDataStruct(FString& Out_Code, FNANODBC_MetaData& Out_MetaData, int32 ColumnIndex = 1);

	UFUNCTION(BlueprintCallable)
	virtual bool GetString(FString& Out_Code, TArray<FString>& Out_String, int32 ColumnIndex = 1);

	UFUNCTION(BlueprintCallable)
	virtual bool GetInt(FString& Out_Code, TArray<int32>& Out_String, int32 ColumnIndex = 1);

};