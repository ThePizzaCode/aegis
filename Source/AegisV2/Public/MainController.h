#pragma once

#include "Obj.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainController.generated.h"

UCLASS()
class AEGISV2_API AMainController : public APlayerController
{
	AMainController();

	float ElapsedTime;
	float UpdateInterval;

	int iterator;


	GENERATED_BODY()

	void Parse();

	void Debug();
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Objects")
	class AObj* FirstObj;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Objects")
	class AObj* SecondObj;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Objects")
	class AObj* ThirdObj;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Objects")
	class AObj* FourthObj;

};
