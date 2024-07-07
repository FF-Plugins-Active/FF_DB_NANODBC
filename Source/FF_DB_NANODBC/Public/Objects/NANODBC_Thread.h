// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// UE Includes.
#include "HAL/Runnable.h"

#include "Objects/NANODBC_Connection.h"

// Fordward Declerations.
class FRunnableThread;
class ANANODBC_Manager;

class FF_DB_NANODBC_API FDB_Thread_NANODBC : public FRunnable
{

public:

	// Sets default values for this actor's properties
	FDB_Thread_NANODBC(ANANODBC_Manager* In_Parent_Actor);

	// Destructor.
	virtual ~FDB_Thread_NANODBC() override;

	virtual bool Init() override;

	virtual uint32 Run() override;

	virtual void Stop() override;

	virtual bool Toggle(bool bIsPause);

	ANANODBC_Manager* Parent_Actor = nullptr;

private:

	bool bStartThread = false;
	FRunnableThread* RunnableThread = nullptr;

	virtual bool CallbackProcessQuery(FString& Out_Code, result& Out_Result);
	virtual bool ParseResult(FString& Out_Code, result In_Result);
};