// like... do what you want?! (But do not infring the EpicGames copyright notes)

#pragma once


#include "FileWriter.h"

#include "Object.h"
#include "FileWriterProxy.generated.h"


/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LOGGINGPLUGIN_API UFileWriterProxy : public USceneComponent
{
	GENERATED_BODY()

public:
		UFileWriterProxy(const FObjectInitializer& ObjectInitializer);
		~UFileWriterProxy();
	
		// Called when the game starts
		virtual void BeginPlay() override;

		virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

		// Called every frame
		virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


		UFUNCTION(BlueprintCallable, Category = "FileWriter")
			void writeTriggerLCT(const FString _name);
		
		UFUNCTION(BlueprintCallable, Category = "FileWriter")
			void closeFile();

		UFUNCTION(BlueprintCallable, Category = "FileWriter")
			void setPath(FString _path);

		UFUNCTION(BlueprintCallable, Category = "FileWriter")	
			void openFile();

		UFUNCTION(BlueprintCallable, Category = "FileWriter")
			void writeLaneLCT(const FString _lane);

		UFUNCTION(BlueprintCallable, Category = "FileWriter")
			void startWriting();

		UFUNCTION(BlueprintCallable, Category = "FileWriter")
			void stopWriting();

		UFUNCTION(BlueprintCallable, Category = "FileWriter")
			void writeDefaultString(const FString _string);


		FileWriter* f;

		static int32 index;

private:

	bool fileExist();


	
};
