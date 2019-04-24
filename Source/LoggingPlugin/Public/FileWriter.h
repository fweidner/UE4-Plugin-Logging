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

	void openFile(bool bStartWriting = true);
	void closeFile();

	/*
	Adds a "\n" to the current line. Is locked.
	*/
	void WriteNewLine();

	/*
	Writes a string to the file. No new line. Ends with a delimiter. Is locked.
	*/
	void WriteToFile(const FString _value);

	/*
	Writes a string to the file. Writes a timestamp in front of the string if _addTimestamp is true. 
	Adds a new line if _addNewLine is true. Is not locked.
	*/
	void WriteToFile(const FString _value, bool _addTimestamp, bool _addNewLine);

	/*
	Writes a string to the file. Puts "Event" in front of _value. Writes a timestamp in front of the string if _addTimestamp is true.
	Adds a new line if _addNewLine is true. Is not locked.
	*/
	void WriteToFileEv(const FString _value, bool _addTimestamp, bool _addNewline);

	/*
	Writes a integer to the file. No new line. Ends with a delimiter. Is locked.
	*/
	void WriteToFile(const int _value);

	/*
	Writes a float to the file. No new line. Ends with a delimiter. Is locked.
	*/
	void WriteToFile(const float _value);

	/*
	Writes four floats to the file. Divided by a delimiter. No new line. Ends with a delimiter. Is locked.
	*/
	void WriteToFile(const float a, const float b, const float c, const float d);

	/*
	Writes a FVector to the file. Divided by a delimiter. No new line. Ends with a delimiter. Is locked.
	*/
	void WriteToFile(const FVector value);
	
	/*
	Writes a FVector (Yaw, Pitch, and Roll) to file. Values are divided by a delimiter. 
	No new line. Ends with a delimiter. Is locked.
	*/
	void WriteToFile(const FRotator value);

	/*
	This method is inteded for logging the current state of the car. 
	It writes a timestamp, an "L", the players name, the current position, orientation, speed and gear to file. 
	All values are divided by a delimiter. 
	There IS a new line at the end! 
	*/
	void WriteToFile(const FString _name, const FVector _position, const FRotator _orientation, const float _forwardSpeed, const int _gear);
	
	/*
	Checks if writing is enabled.
	@return true if writing is allowed.
	*/
	bool isWriteEnabled(){	return bWrite; };
	
	/*
	Writes a timestamp to the file (uses std::chrono). Ends with a delimiter. No new line. Is locked.
	*/
	void WriteTimestamp();

	void StartWriting();
	void StopWriting();
	
	/*
	Sets the path of the file. Works only before calling openFile().
	*/
	void SetPath(FString _path);
	
	/*
	Writes a header to the file. Should be called after openFile().

	@param _playerName	Name of the current player/participant.
	@param _condition	Name of the current experiment condition.
	*/
	void InitFile(FString _comment1 = "Level", FString _comment2 = "Explanation", FString _playerName = "Galadriel", FString _condition = "Lothlorien"); //use only after  player name and enviornment/condition is set.


	/*
	Sets the debug mode flag which enables additional log messages.
	*/
	void SetDebugMessages(bool _newValue);

private:

	void setPlayerName(const FString _name);
	void setEnvironment(const FString _env);


	static FileWriter* s_instance;

	/*
	Gets the local date and time on this computer since 1.1.1970 in milliseconds (uses std::chrono)
	*/
	int64 getTimestamp();

	FileWriter();														// private so no one can access it
	FileWriter(FileWriter const&){};									// copy constructor is private
	FileWriter& operator=(FileWriter const&){ return *s_instance; };	// assignment operator is private
	~FileWriter();

	
	bool doesLoggingDirectoryExist(const  FString _pathToCheck) const;
	bool isFileOpen(std::ofstream* _file);
	bool bWrite = false;
	std::ofstream Myfile;
	bool bIsInitialized = false;
	bool bIsFileOpen = false;
	
	FString Path;
	FString PlayerName;
	FString Environment;

	FString Delimiter = ";";
	
	FCriticalSection MyMutexWrite;

	void createDirectory(FString _path);
	std::string getDelimiter();

	bool bIsDebugMode = false;
};

