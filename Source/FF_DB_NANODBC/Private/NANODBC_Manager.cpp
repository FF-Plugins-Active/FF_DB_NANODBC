// Fill out your copyright notice in the Description page of Project Settings.

#include "NANODBC_Manager.h"

// Sets default values
ANANODBC_Manager::ANANODBC_Manager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ANANODBC_Manager::BeginPlay()
{
	Super::BeginPlay();
}

void ANANODBC_Manager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ANANODBC_Manager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ANANODBC_Manager::NANODBC_Connection_Create(FString& Out_Code, UNANODBC_Connection*& CreatedConnection, FString In_Server, FString In_UserName, FString In_Password)
{
	try
	{
		connection ConnectionRef = connection(TCHAR_TO_UTF8(*In_Server), TCHAR_TO_UTF8(*In_UserName), TCHAR_TO_UTF8(*In_Password));
		ConnectionRef.connect(TCHAR_TO_UTF8(*In_Server), TCHAR_TO_UTF8(*In_UserName), TCHAR_TO_UTF8(*In_Password));

		if (!ConnectionRef.connected())
		{
			Out_Code = "NANODBC could not be connect target server !";
			return false;
		}

		FString ConnectionId = In_Server + "&&" + In_UserName;

		UNANODBC_Connection* ConnectionObject = NewObject<UNANODBC_Connection>();
		ConnectionObject->SetConnection(ConnectionRef);
		ConnectionObject->SetConnectionId(ConnectionId);

		CreatedConnection = ConnectionObject;
		
		this->MAP_Connections.Add(ConnectionId, ConnectionObject);
	}

	catch (const std::exception& Exception)
	{
		Out_Code = Exception.what();
	}

	Out_Code = "NANODBC Connection to target database successfully created !";
	return true;
}

bool ANANODBC_Manager::NANODBC_Connection_Delete_Id(FString& Out_Code, FString ConnectionId)
{
	if (ConnectionId.IsEmpty())
	{
		return false;
	}

	this->MAP_Connections.Remove(ConnectionId);

	return true;
}

bool ANANODBC_Manager::NANODBC_Connection_Delete_Object(FString& Out_Code, UPARAM(ref) UNANODBC_Connection*& TargetConnection)
{
	if (!IsValid(TargetConnection))
	{
		return false;
	}

	const FString ConnectionId = TargetConnection->GetConnectionId();

	if (ConnectionId.IsEmpty())
	{
		return false;
	}

	this->MAP_Connections.Remove(ConnectionId);

	return true;
}