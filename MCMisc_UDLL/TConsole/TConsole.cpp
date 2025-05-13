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

// Declared class:
#include "TConsole\TConsole.h"

// Includes:
#include <iostream>
#include <io.h>

// Own includes:
#include "..\MCJob_UDLL\ErrorCodes.h"
#include "..\MCJob_UDLL\GlobalDefinitions.h"

// ***************** CONSTRUCTOR/DESTRUCTOR/OPERATORS ******************** //

// Constructor
TConsole::TConsole()
{
	// Initialize member variables
	Console_Buffer_Rows = 500;
	Console_Buffer_Columns = 150;
	Console_Title = "Console";
	Ready = false;
	Visible = false;

	stream_out = NULL;
	stream_err = NULL;
	stream_in = NULL;

	fd_save_out = -1;
	fd_save_err = -1;
	fd_save_in = -1;

	handle_save_out = INVALID_HANDLE_VALUE;
	handle_save_err = INVALID_HANDLE_VALUE;
	handle_save_in = INVALID_HANDLE_VALUE;

	try
	{
		// Set error handler for CRT functions
		_set_invalid_parameter_handler(CRTInvalidParameterHandler);

		// Dublicate previous standard file descriptors (flush streams, then dublicate underlying descriptors)
		// -> new descriptors equal -1 if an error occurs (e.g. invalid standard file descriptors)
		fflush(stdout);
		fflush(stderr);
		fflush(stdin);
		fd_save_out = _dup(_fileno(stdout));
		fd_save_err = _dup(_fileno(stderr));
		fd_save_in = _dup(_fileno(stdin));

		// Dublicate previous standard device handles
		// -> new handles equal INVALID_HANDLE_VALUE or NULL if an error occurs (e.g. invalid standard devices)
		handle_save_out = GetStdHandle(STD_OUTPUT_HANDLE);
		handle_save_err = GetStdHandle(STD_ERROR_HANDLE);
		handle_save_in = GetStdHandle(STD_INPUT_HANDLE);

		// Create and open new console
		if (AllocConsole() == 0) return;

		// Remove X-button and "Close" from Pop-Up menu
		HWND hWnd = GetConsoleWindow();
		if (hWnd == NULL) return;
		HMENU hMenu = GetSystemMenu(hWnd, false);
		if (hMenu == NULL) return;
		EnableMenuItem(hMenu, SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);
		RemoveMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);

		// Prevent STRG+C
		SetConsoleCtrlHandler(NULL, true);

		// Set size of the console screen buffer
		CONSOLE_SCREEN_BUFFER_INFO consoleinfo;
		if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleinfo) == 0) return;
		consoleinfo.dwSize.X = Console_Buffer_Columns;
		consoleinfo.dwSize.Y = Console_Buffer_Rows;
		if (SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), consoleinfo.dwSize) == 0) return;

		// Set title
		if (SetConsoleTitleA(Console_Title.c_str()) == 0) return;

		// Redirect standard CRT streams to console buffers
		if (freopen_s(&stream_out, "CONOUT$", "w", stdout) != 0) return;
		if (freopen_s(&stream_err, "CONOUT$", "w", stderr) != 0) return;
		if (freopen_s(&stream_in, "CONIN$", "r", stdin) != 0) return;

		// Make streams unbuffered
		if (setvbuf(stdout, NULL, _IONBF, 0) != 0) return;
		if (setvbuf(stderr, NULL, _IONBF, 0) != 0) return;
		if (setvbuf(stdin, NULL, _IONBF, 0) != 0) return;

		// Synchronize std::cout, std::cin, std::cerr (etc.) with the new stdout, stderr, stdin streams
		std::ios::sync_with_stdio();

		// Remove eventual badbit and failbit flags
		std::cout.clear();
		std::cerr.clear();
		std::cin.clear();

		Ready = true;
		Visible = true;
	}
	catch (std::exception& e)
	{
		Ready = false;
		StdExceptionHandler(e);
		throw;
	}
	catch (...)
	{
		Ready = false;
		ExceptionHandler();
		throw;
	}
}

// Destructor
TConsole::~TConsole()
{
	try
	{
		// Close console
		FreeConsole();

		// Close console streams
		fclose(stream_out);
		fclose(stream_err);
		fclose(stream_in);
		stream_out = NULL;
		stream_err = NULL;
		stream_in = NULL;

		// Restore standard file descriptors
		fflush(stdout);
		fflush(stderr);
		fflush(stdin);
		if (fd_save_out != -1) std::ignore = _dup2(fd_save_out, _fileno(stdout));
		if (fd_save_err != -1) std::ignore = _dup2(fd_save_err, _fileno(stderr));
		if (fd_save_in != -1) std::ignore = _dup2(fd_save_in, _fileno(stdin));
		if (fd_save_out != -1) _close(fd_save_out);
		if (fd_save_err != -1) _close(fd_save_err);
		if (fd_save_in != -1) _close(fd_save_in);
		fd_save_out = -1;
		fd_save_err = -1;
		fd_save_in = -1;

		// Restore standard devices
		if ((handle_save_out != INVALID_HANDLE_VALUE) && (handle_save_out != NULL))
			SetStdHandle(STD_OUTPUT_HANDLE, handle_save_out);
		if ((handle_save_err != INVALID_HANDLE_VALUE) && (handle_save_err != NULL))
			SetStdHandle(STD_ERROR_HANDLE, handle_save_err);
		if ((handle_save_in != INVALID_HANDLE_VALUE) && (handle_save_in != NULL))
			SetStdHandle(STD_INPUT_HANDLE, handle_save_in);
		handle_save_out = INVALID_HANDLE_VALUE;
		handle_save_err = INVALID_HANDLE_VALUE;
		handle_save_in = INVALID_HANDLE_VALUE;
	}
	catch (std::exception& e)
	{
		Ready = false;
		StdExceptionHandler(e);
	}
	catch (...)
	{
		Ready = false;
		ExceptionHandler();
	}
}

// **************************** PUBLISHED ********************************* //

// Destructor method against memory leaks (instance suicide)
void TConsole::Release()
{
	try
	{
		delete this;
	}
	catch (std::exception& e)
	{
		StdExceptionHandler(e);
	}
	catch (...)
	{
		ExceptionHandler();
	}
}

// Show console window
int TConsole::Show()
{
	try
	{
		if (Ready != true) return KMCERR_READY_NOT_TRUE;

		HWND hWnd = GetConsoleWindow();
		if (hWnd == NULL)
		{
			Ready = false;
			return KMCERR_NO_CONSOLE;
		}
		ShowWindow(hWnd, SW_SHOW);

		Visible = true;
		return KMCERR_OK;
	}
	catch (std::exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Hide console window
int TConsole::Hide()
{
	try
	{
		if (Ready != true) return KMCERR_READY_NOT_TRUE;

		HWND hWnd = GetConsoleWindow();
		if (hWnd == NULL)
		{
			Ready = false;
			return KMCERR_NO_CONSOLE;
		}
		ShowWindow(hWnd, SW_HIDE);

		Visible = false;
		return KMCERR_OK;
	}
	catch (std::exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Return status of the console
int TConsole::Status() const
{
	if (Ready != true) return KMCERR_READY_NOT_TRUE;		// d.h. Ready == false
	if (Visible == false) return KMCERR_NO_CONSOLE;			// d.h. Ready == true && Visible == false
	return KMCERR_OK;										// d.h. Ready == true && Visible == true
}

// Change size of console screen buffer
int TConsole::SetBufferSize(int rows, int columns)
{
	try
	{
		if (Ready != true) return KMCERR_READY_NOT_TRUE;

		CONSOLE_SCREEN_BUFFER_INFO consoleinfo;
		if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleinfo) == 0)
		{
			Ready = false;
			return KMCERR_NO_CONSOLE;
		}
		consoleinfo.dwSize.X = columns;
		consoleinfo.dwSize.Y = rows;
		if (SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), consoleinfo.dwSize) == 0)
		{
			consoleinfo.dwSize.X = Console_Buffer_Columns;
			consoleinfo.dwSize.Y = Console_Buffer_Rows;
			if (SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), consoleinfo.dwSize) == 0)
			{
				Ready = false;
				return KMCERR_NO_CONSOLE;
			}
			return KMCERR_INVALID_INPUT;
		}
		Console_Buffer_Columns = columns;
		Console_Buffer_Rows = rows;

		return KMCERR_OK;
	}
	catch (std::exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// Set console title
int TConsole::SetTitle(std::string title)
{
	try
	{
		if (Ready != true) return KMCERR_READY_NOT_TRUE;

		if (SetConsoleTitleA(title.c_str()) == 0)
		{
			if (SetConsoleTitleA(Console_Title.c_str()) == 0) return KMCERR_NO_CONSOLE;
			return KMCERR_INVALID_INPUT;
		}
		Console_Title = title;

		return KMCERR_OK;
	}
	catch (std::exception& e)
	{
		return StdExceptionHandler(e);
	}
	catch (...)
	{
		return ExceptionHandler();
	}
}

// ***************************** PUBLIC *********************************** //



// ***************************** PRIVATE ********************************** //

// CRT error handler
void TConsole::CRTInvalidParameterHandler(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, uintptr_t pReserved)
{
	// do nothing
}

// Global standard exception handler
int TConsole::StdExceptionHandler(const std::exception& e) const
{
	std::cerr << KMCERRSTR_STDERR << " " << e.what() << std::endl;
	return KMCERR_EXCEPTION_OCCURED;
}

// Global exception handler for unknown exceptions
int TConsole::ExceptionHandler() const
{
	std::cerr << KMCERRSTR_UNKNOWNERR << std::endl;
	return KMCERR_EXCEPTION_OCCURED;
}