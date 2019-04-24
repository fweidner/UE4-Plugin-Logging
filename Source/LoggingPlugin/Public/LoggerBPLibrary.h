// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine.h"

#include "FileWriter.h"

#include "LoggerBPLibrary.generated.h"

/*
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu.
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/

UCLASS()
class LOGGINGPLUGIN_API ULoggerBPLibrary : public UBlueprintFunctionLibrary
{
public:

	GENERATED_UCLASS_BODY()

	//It is essential to call this method first!
	UFUNCTION(BlueprintCallable, Category = "Car2IXS|Logging")
		static void InitLogger();

	UFUNCTION(BlueprintCallable, Category = "Car2IXS|Logging")
		static void Test();

	//Opens file. Does not start writing.
	UFUNCTION(BlueprintCallable, Category = "Car2IXS|Logging")
		static void OpenFile(bool bStartWriting = true);

	UFUNCTION(BlueprintCallable, Category = "Car2IXS|Logging")
		static void SetPath(FString _path);

	UFUNCTION(BlueprintCallable, Category = "Car2IXS|Logging")
		static void WriteString(const FString _string, bool _addTimestamp, bool _addNewLine);

	UFUNCTION(BlueprintCallable, Category = "Car2IXS|Logging")
		static void WriteStringEvent(const FString _string, bool _addTimestamp, bool _addNewLine);

	UFUNCTION(BlueprintCallable, Category = "Car2IXS|Logging")
		static void CloseFile();


	UFUNCTION(BlueprintCallable, Category = "Car2IXS|Logging")
		static void WriteNewLine();

	UFUNCTION(BlueprintCallable, Category = "Car2IXS|Logging")
		static void InitFile(FString Comment1 = "Level", FString Comment2 ="Explanation", FString _playerName = "Arwen Undomiel", FString _condition = "Rivendell");

	UFUNCTION(BlueprintCallable, Category = "Car2IXS|Logging")
		static void StartWriting();

	UFUNCTION(BlueprintCallable, Category = "Car2IXS|Logging")
		static void StopWriting();

	UFUNCTION(BlueprintCallable, Category = "Car2IXS|Logging")
		static void WriteTimestamp();

	UFUNCTION(BlueprintCallable, Category = "Car2IXS|Logging")
	static void SetDebugMessages(bool _newVal);

	static FileWriter* f;
/*
	UFUNCTION(BlueprintCallable, Category = "FileWriter")
		static void writeTriggerLCT(const FString _name);

	UFUNCTION(BlueprintCallable, Category = "FileWriter")
		static void closeFile();




	UFUNCTION(BlueprintCallable, Category = "FileWriter")
		static void writeLaneLCT(const FString _lane);

	UFUNCTION(BlueprintCallable, Category = "FileWriter")
		static void startWriting();

	UFUNCTION(BlueprintCallable, Category = "FileWriter")
		static void stopWriting();

	UFUNCTION(BlueprintCallable, Category = "FileWriter")
		static void writeDefaultString(const FString _string);
		*/
};
