// **************************************************************** //
//																	//
//	Class: TConsole													//
//	Author: Philipp Hein											//
//  Description:													//
//    Creates and manages a console window							//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//  Distributed under GPL v3 license								//
//  (see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

/*
Explanations for low-level I/O = communication of a program with a "file":
("file" can be a normal file or another device like a terminal/console window)

Several objects and functions are relevant:
- File name (type: string): can be a normal file name, special values for a console a "file"
  - "CONOUT$" = output "file" of the console (= screen buffer of the active console, always)
  - "CONIN$" = input "file" of the console (= input buffer of the active console, always)
- CRT file descriptor (type: int): lowest-level identifier for a "file"
  - open function: creates a file descriptor for a file name (= "opens the file")
  - close function: deallocates a file descriptor (= "closes this connection to the file", there might be still other file descriptors to the same "file")
  - dup function: dublicates a file descriptor (= creates a new file descriptor that is connected to the same "file")
  - dup2 function: copies a file descriptor over another (= connects a file descriptor to the same "file" as another descriptor)
- CRT file stream (type: FILE*): layered on top of a file descriptor, provides advanced I/O operations and buffering
  - standard C streams: stdout, stderr, stdin
  - fileno function: extract the file descriptor of a stream
  - fopen function: create a stream to a certain file name (= combination of open + fdopen)
  - fclose function: close a stream and both the underlying file descriptor (like "close") and the OS handle (like "CloseHandle")
  - fdopen function: create a stream that is associated with a certain file descriptor
  - freopen function: closes a stream (like "fclose") and then reopens it to a certain file name (like "fopen")
- OS handle (type: HANDLE): identifier of a certain device (or object) in the operating system
  - the current standard I/O devices are identified by the constants STD_OUTPUT_HANDLE, STD_ERROR_HANDLE and STD_INPUT_HANDLE
  - GetStdHandle function: yields a handle to the specified standard device
	(returns INVALID_HANDLE_VALUE on error and NULL when the process has no standard I/O devices)
  - SetStdHandle function: sets a standard device to a certain handle (= redirects I/O to another device or file)
  - CloseHandle function: closes a handle (can be done but not mandatory because also done when process is destroyed)
- STL streams (type: std::iostream): common streams in C++
  - standard C++ streams: std::cout, std::cerr, std::cin
  - the C++ streams can be synchronized to the respective standard C streams by calling std::sync_with_stdio(true)

AllocConsole function:
- creates a new Console window (only one allowed per process), whose buffers are represented by CONOUT$ and CONIN$
- STD_OUTPUT_HANDLE and STD_ERROR_HANDLE become handles to the console screen buffer (i.e. to CONOUT$)
- STD_INPUT_HANDLE becomes a handle to the console input buffer (i.e. to CONIN$)
  (the .NET System::Console automatically becomes connected to this new console due to the changed standard handles)

FreeConsole function:
- detaches the calling process from its console (and closes the console if no other process is attached to it)
*/

#ifndef TConsole_H
#define TConsole_H

// Includes:
#define WIN32_LEAN_AND_MEAN		// removes less used parts from windows.h header
#include <windows.h>
#include <string>

// Own includes:
#include "TConsole\IConsole.h"

// Class declaration
class TConsole : public IConsole
{
	// Member variables
protected:
	bool Ready;						// Indicates whether the console was created successfully
	bool Visible;					// Indicates whether the console is visible or hidden

	int Console_Buffer_Rows;		// Number of rows of the console buffer
	int Console_Buffer_Columns;		// Number of columns of the console buffer (= characters per row)
	std::string Console_Title;		// Title of the console window

	FILE* stream_out;				// CRT file stream to new console screen buffer
	FILE* stream_err;				// CRT file stream to new console screen buffer
	FILE* stream_in;				// CRT file stream to new console input buffer

	int fd_save_out;				// CRT file descriptor of the previous stdout
	int fd_save_err;				// CRT file descriptor of the previous stderr
	int fd_save_in;					// CRT file descriptor of the previous stdin

	HANDLE handle_save_out;			// OS handle of the previous standard output device
	HANDLE handle_save_err;			// OS handle of the previous standard error device
	HANDLE handle_save_in;			// OS handle of the previous standard input device

	// Member functions
public:
	TConsole();				// Constructor
	~TConsole();			// Destructor
	void Release();		// Destructor method against memory leaks (instance suicide)

	int Show();										// Show console window
	int Hide();										// Hide console window
	int Status() const;								// Get console status
	int SetBufferSize(int rows, int columns);		// Change size of console screen buffer
	int SetTitle(std::string title);				// Set console window title

protected:
	int StdExceptionHandler(const std::exception& e) const;																											// Global standard exception handler
	int ExceptionHandler() const;																																	// Global exception handler for unknown exceptions
	static void CRTInvalidParameterHandler(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, uintptr_t pReserved);		// CRT error handler
};

#endif