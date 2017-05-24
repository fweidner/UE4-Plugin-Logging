// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine.h"

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

	UFUNCTION(BlueprintCallable, Category = "Logger", meta = (DisplayName = "Test", Keywords = "Test"))
		static void Test();

/*	UFUNCTION(BlueprintCallable, Category = "FileWriter")
		static void writeTriggerLCT(const FString _name);

	UFUNCTION(BlueprintCallable, Category = "FileWriter")
		static void closeFile();

	UFUNCTION(BlueprintCallable, Category = "FileWriter")
		static void setPath(FString _path);

	UFUNCTION(BlueprintCallable, Category = "FileWriter")
		static void openFile();

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
