// Opening a window handle
//#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
//#include <T1Help32>
#include <Windows.h>
//#include <tchar>

using namespace std;

int main()
{
	//CONFIGURE AN REGISTER THE WINDOW CLASS
	// Define our const wchar_t variable to name our window handle array elements 
		
	const char Futaba_Webs [] = "FutabaWebs";
	
	//Next declare the window class struct and the main 3 elements that make up the stuct 
	HINSTANCE hInstance;
	WNDCLASS wc = {};

	wc.lpfnWndProc = DefWindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = Futaba_Webs;

	// After configuring the class elements and the handle name, register our class, pointing to the address of our window class
	// We do this using the RegisterClass() in the win32 api
	
	RegisterClass(&wc);
	
	
	//Create our window from the HWND class using the CreatewindowEx()
	
	HWND hwnd = CreateWindowEx(
		0, //Window Style
		Futaba_Webs, //The name of our windwo class
		"FutabaWebs", //Name of the window handle 
		WS_OVERLAPPEDWINDOW, //The window style 
		
		//Positons of the window (Use CW_USERDEFAULT)
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT , CW_USEDEFAULT ,
		
		// null, null, hInstance, null
		NULL,
		NULL,
		hInstance,
		NULL
		);
		
		//When the window closes the value of hwnd is set to zero
		if (hwnd == NULL)
		{
			return 0; //end our function 
			
		}
		
		//Use the ShowWindow() to show our window
		ShowWindow(hwnd,  5);
		system("pause");
		return 0;
	
}
