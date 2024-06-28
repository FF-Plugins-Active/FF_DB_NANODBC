// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "NANODBC_Thread.h"
#include "NANODBC_Connection.h"

#include "NANODBC_Manager.generated.h"

UCLASS()
class FF_DB_NANODBC_API ANANODBC_Manager : public AActor
{
	GENERATED_BODY()
	
protected:
	
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	// Called when the game end or when destroyed.
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	
	// Sets default values for this actor's properties.
	ANANODBC_Manager();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual bool NANODBC_Init_Environment();

	UFUNCTION(BlueprintCallable)
	virtual bool NANODBC_Connection_Create(FString& Out_Code, UNANODBC_Connection*& CreatedConnection, FString In_Server, FString In_UserName, FString In_Password, bool bUseAutoCommit);

	UFUNCTION(BlueprintCallable)
	virtual bool NANODBC_Connection_Delete_Id(FString& Out_Code, FString ConnectionId);

	UFUNCTION(BlueprintCallable)
	virtual bool NANODBC_Connection_Delete_Object(FString& Out_Code, UPARAM(ref) UNANODBC_Connection*& TargetConnection);

	UPROPERTY(BlueprintReadOnly)
	TMap<FString, UNANODBC_Connection*> MAP_Connections;

};