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