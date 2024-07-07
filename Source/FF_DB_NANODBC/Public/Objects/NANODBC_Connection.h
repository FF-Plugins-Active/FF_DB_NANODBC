#pragma once

#include "CoreMinimal.h"

// Custom Includes.
#include "NANODBC_Includes.h"
#include "Objects/NANODBC_Result.h"

#include "NANODBC_Connection.generated.h"

using namespace nanodbc;

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