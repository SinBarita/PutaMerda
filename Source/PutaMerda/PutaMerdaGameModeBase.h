// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PutaMerdaGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PUTAMERDA_API APutaMerdaGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
		virtual void StartPlay() override;
	
	
};
