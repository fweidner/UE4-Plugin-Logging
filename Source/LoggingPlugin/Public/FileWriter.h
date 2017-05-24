// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <fstream>		// ofstream
#include <Misc/DateTime.h> // get time and date for filename
#include <atomic>

#include "Runtime/Core/Public/HAL/ThreadingBase.h"
#include "Engine.h"


/**
 * FileWriter holding the functionality for later analysis. Implemented as a Singleton element in order to guarantee a single point of logging. Primary purpose is the logging of position and orientation of actors. Therefore, all other (moving) actors should inherit from this class.
 */
class LOGGINGPLUGIN_API FileWriter
{
public:
	static FileWriter* Singleton();

	void StartStopWriting();
	void openFile();
	void closeFile();

	void WriteNewLine();
	void WriteToFile(const FString _value);
	void WriteToFile(const int _value);
	void WriteToFile(const float _value);
	void WriteToFile(const float a, const float b, const float c, const float d);
	void WriteToFile(const FVector value);
	void WriteToFile(const FRotator value);
	void WriteToFile(const FString _name, const FVector _position, const FRotator _orientation);
	void WriteToFile(const FString _name, const FVector _position, const FRotator _orientation, const float _forwardSpeed);
	void WriteToFile(const FString _name, const FVector _position, const FRotator _orientation, const float _forwardSpeed, const int _gear);
	bool isWriteEnabled(){	return bWrite; };
	void writeTimestamp();

	void StartWriting();
	void StopWriting();

	void SetPath(FString _path);

	void SetPlayerName(const FString _name);
	void SetEnvironment(const FString _env);
		
	void InitFile(FString _playerName = "Galadriel", FString _condition = "Lothlorien"); //use only after  player name and enviornment/condition is set.

private:

	static FileWriter* s_instance;
	static int RefCount;

	int64 getTimestamp();


	FileWriter();														// private so no one can access it
	FileWriter(FileWriter const&){};									// copy constructor is private
	FileWriter& operator=(FileWriter const&){ return *s_instance; };	// assignment operator is private
	~FileWriter();

	
	bool doesLoggingDirectoryExist(const  FString _pathToCheck) const;
	bool isFileOpen(std::ofstream* _file);
	bool bWrite;
	std::ofstream Myfile;
	
	FString Path;
	FString PlayerName;
	FString Environment;

	float steer; 
	float accelerate;
	
	FCriticalSection MyMutexWrite;

	void createDirectory(FString _path);
};

