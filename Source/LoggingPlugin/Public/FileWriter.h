// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <fstream>		// ofstream
#include <Misc/DateTime.h> // get time and date for filename
#include <atomic>

/**
 * FileWriter holding the functionality for later analysis. Implemented as a Singleton element in order to guarantee a single point of logging. Primary purpose is the logging of position and orientation of actors. Therefore, all other (moving) actors should inherit from this class.
 */
class CAR2SIMULATE_API FileWriter 
{
public:
	static FileWriter* Singleton();

	void StartStopWriting();
	void openFile();
	void closeFile();

	void writeNewLine();
	void writeToFile(const FString _value);
	void writeToFile(const int _value);
	void writeToFile(const float _value);
	void writeToFile(const float a, const float b, const float c, const float d);
	void writeToFile(const FVector value);
	void writeToFile(const FRotator value);
	void writeToFile(const FString _name, const FVector _position, const FRotator _orientation);
	void writeToFile(const FString _name, const FVector _position, const FRotator _orientation, const float _forwardSpeed);
	void writeToFile(const FString _name, const FVector _position, const FRotator _orientation, const float _forwardSpeed, const int _gear);
	bool isWriteEnabled(){	return bWrite; };
	void writeTimestamp();

	void startWriting();
	void stopWriting();

	void setPath(FString _path);

	void setPlayerName(const FString _name);
	void setEnvironment(const FString _env);

	void setAtomicSteerValue(float _val);
	void setAtomicAccelarteValue(float _val);


	
	void initFile();

private:

	static FileWriter* s_instance;
	static int RefCount;

	int64 getTimestamp();


	FileWriter();													// private so no one can access it
	FileWriter(FileWriter const&){};								// copy constructor is private
	FileWriter& operator=(FileWriter const&){ return *s_instance; };	// assignment operator is private
	~FileWriter();

	
	bool doesLoggingDirectoryExist(const  FString _pathToCheck) const;
	bool isFileOpen(std::ofstream* _file);
	bool bWrite;
	std::ofstream Myfile;
	
	FString Path;
	FString PlayerName;
	FString Environment;

	/*
	std::atomic_long steer;
	std::atomic_long accelerate;
	*/
	float steer; 
	float accelerate;
	
	void createDirectory(FString _path);
};

