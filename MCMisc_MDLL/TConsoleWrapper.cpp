// **************************************************************** //
//																	//
//	Class: TConsoleWrapper											//
//	Author: Philipp Hein											//
//  Description:													//
//    Managed C++/CLI assembly (managed DLL) as wrapper for the		//
//	  TConsole class of the unmanaged DLL							//
//																	//
//	Copyright (c) P. Hein, IPC, RWTH Aachen University				//
//  Distributed under GPL v3 license								//
//  (see LICENSE.txt file in the solution root folder)				//
//																	//
// **************************************************************** //

#define KMCERR_INVALID_POINTER -6
#define KMCERR_EXCEPTION_OCCURED -15
#define KMCERRSTR_STDERR "Exception occured:"

// Declared class:
#include "TConsoleWrapper.h"

// *********************** CONSTRUCTOR/DESTRUCTOR ************************* //

// Constructor
TConsoleWrapper::TConsoleWrapper()
{
	try
	{
		m_Console = NULL;
		m_Console = CreateConsole();
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e);
	}
	if (m_Console == NULL)
	{
		throw gcnew System::ApplicationException("Console creation failed (TConsoleWrapper constructor).");
	}
}

// Destructor
TConsoleWrapper::~TConsoleWrapper()
{
	try
	{
		if (m_Console != NULL) m_Console->Release();
		m_Console = NULL;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e);
	}
}

// Finalizer
TConsoleWrapper::!TConsoleWrapper()
{
	try
	{
		if (m_Console != NULL) m_Console->Release();
		m_Console = NULL;
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e);
	}
}

// **************************** PUBLISHED ********************************** //

// Show console window
int TConsoleWrapper::Show()
{
	try
	{
		if (m_Console == NULL) return KMCERR_INVALID_POINTER;

		return m_Console->Show();
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e);
		return KMCERR_EXCEPTION_OCCURED;
	}
}

// Hide console window
int TConsoleWrapper::Hide()
{
	try
	{
		if (m_Console == NULL) return KMCERR_INVALID_POINTER;

		return m_Console->Hide();
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e);
		return KMCERR_EXCEPTION_OCCURED;
	}
}

// Get console status
int TConsoleWrapper::Status()
{
	try
	{
		if (m_Console == NULL) return KMCERR_INVALID_POINTER;

		return m_Console->Status();
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e);
		return KMCERR_EXCEPTION_OCCURED;
	}
}

// Change size of console screen buffer
int TConsoleWrapper::SetBufferSize(int rows, int columns)
{
	try
	{
		if (m_Console == NULL) return KMCERR_INVALID_POINTER;

		return m_Console->SetBufferSize(rows, columns);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e);
		return KMCERR_EXCEPTION_OCCURED;
	}
}

// Set console window title
int TConsoleWrapper::SetTitle(System::String^ title)
{
	try
	{
		if (m_Console == NULL) return KMCERR_INVALID_POINTER;

		std::string U_title = "";
		String_M2U(title, U_title);

		return m_Console->SetTitle(U_title);
	}
	catch (System::Exception^ e)
	{
		System::Console::WriteLine("{0} {1}", KMCERRSTR_STDERR, e);
		return KMCERR_EXCEPTION_OCCURED;
	}
}

// **************************** PROTECTED ********************************** //

// General information for data transfer between unmanaged and managed variables:
// - blittable types (int, double, ...) can be simply transfered by-value, 
//	 only by-reference parameters require a temporary unmanaged copy
// - non-blittable types (string, own classes, ...):
//		- by-value input parameter: copy the managed variable to the unmanaged heap with a Marshal function
//		- by-value output parameter (return value): create a new managed variable with gcnew
//		- by-reference parameter: use by-value input technique to create an unmanaged variable, 
//        then pass it to the unmanaged DLL by-reference and finally use the by-value output technique to 
//		  change the original managed variable

// Transfer of managed String^ to unmanaged std::string
void TConsoleWrapper::String_M2U(System::String^ in_str, std::string& out_str)
{
	const char* cstr = (const char*)(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(in_str)).ToPointer();
	out_str = cstr;
	System::Runtime::InteropServices::Marshal::FreeHGlobal(System::IntPtr((void*)cstr));
}

// Transfer of unmanaged std::string to managed String^
void TConsoleWrapper::String_U2M(std::string in_str, System::String^% out_str)
{
	out_str = gcnew System::String(in_str.c_str());
}