// Fill out your copyright notice in the Description page of Project Settings.

#include "NANODBC_Thread.h"
#include "NANODBC_Manager.h"

// UE Includes.
#include "Async/Async.h"

FDB_Thread_NANODBC::FDB_Thread_NANODBC(ANANODBC_Manager* In_Parent_Actor)
{
	this->Parent_Actor = In_Parent_Actor;

	this->RunnableThread = FRunnableThread::Create(this, TEXT("Test"));
}

FDB_Thread_NANODBC::~FDB_Thread_NANODBC()
{
	if (this->RunnableThread)
	{
		this->RunnableThread->Kill(true);
		delete this->RunnableThread;
	}
}

bool FDB_Thread_NANODBC::Init()
{
	this->bStartThread = true;

	return true;
}

uint32 FDB_Thread_NANODBC::Run()
{
	while (this->bStartThread)
	{
		FString Out_Code;
		result Out_Result;

		if (!this->CallbackProcessQuery(Out_Code, Out_Result))
		{
			UE_LOG(LogTemp, Warning, TEXT("NANODBC Bulk Query Error = %s"), *Out_Code);
		}

		if (!this->ParseResult(Out_Code, Out_Result))
		{
			UE_LOG(LogTemp, Warning, TEXT("NANODBC Bulk Query Error = %s"), *Out_Code);
		}
	}

	return 0;
}

void FDB_Thread_NANODBC::Stop()
{
	this->bStartThread = false;
}

bool FDB_Thread_NANODBC::Toggle(bool bIsPause)
{
	if (!this->RunnableThread)
	{
		return false;
	}

	this->RunnableThread->Suspend(bIsPause);

	return true;
}

bool FDB_Thread_NANODBC::CallbackProcessQuery(FString& Out_Code, result& Out_Result)
{
	if (this->Parent_Actor->SQL_Query_Queue.IsEmpty())
	{
		return false;
	}

	FNANODBC_Bulk_Query_Struct CurrentQuery;
	if (!this->Parent_Actor->SQL_Query_Queue.Dequeue(CurrentQuery))
	{
		return false;
	}

	if (CurrentQuery.SQL_Query.IsEmpty())
	{
		return false;
	}

	const ANSICHAR* QueryString = NANODBC_TEXT(TCHAR_TO_UTF8(*CurrentQuery.SQL_Query));
	connection ConenctionRef = CurrentQuery.TargetConnection->GetConnection();

	if (CurrentQuery.bRequestResult)
	{
		result TempResult;

		try
		{
			TempResult = execute(ConenctionRef, QueryString);
		}

		catch (const std::exception& Exception)
		{
			Out_Code = Exception.what();
			return false;
		}

		if (!TempResult)
		{
			return false;
		}

		Out_Result = TempResult;
		return true;
	}

	else
	{
		try
		{
			just_execute(ConenctionRef, QueryString);
		}

		catch (const std::exception& Exception)
		{
			Out_Code = Exception.what();
			return false;
		}

		return true;
	}
}

bool FDB_Thread_NANODBC::ParseResult(FString& Out_Code, result In_Result)
{
	if (!In_Result)
	{
		return false;
	}

	return true;
}