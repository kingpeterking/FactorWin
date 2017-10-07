﻿//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include "LongNumber.h"
#include "FactorQueue.h"
#include <atlstr.h>
#include <gdiplus.h>


#include <atomic>
#include <collection.h>


using namespace FactorWin;
using namespace Platform;

// System added before Windows for concurrency 
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

using namespace Windows::UI;
using namespace Microsoft::Graphics::Canvas::UI::Xaml;

// using namespace System::Runtime::InteropServices;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409


// Global variables used to save passing lots of parameters and to aid concurrency 
// Create one, zero as long numbers - global scope so they don't get reinitiatlised for each thread
LongNumber One(1);
LongNumber Zero(1);

// Set the target as a global variable so we don't need to pass it around 
LongNumber LNTarget;
int ResultLen;
int IterCount;

// Create the queue 
FactorQueue FNQueue;
FactorQueue SolvedQueue;
FactorQueue DrawQueue; 

// Counter for number solved - global so doesn't need to be passed around 
int SolvedCount;

// declare the functions we will use later to build the nodes and solve the factor and update the screen
void CreateChidNodesQueue();




MainPage::MainPage()
{
	InitializeComponent();

	// Set up standard long numbers  - one, zero 
	One.SetLongNumber(1, 0);
	Zero.SetLongNumber(0, 0);

	ResultLen = 0;
	IterCount = 0;

}




void FactorWin::MainPage::FactorButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	
	// Create the head of the Queue 
	FactorNode FNHeadofQueue(0, Zero, Zero, false);
	FNQueue.Push(FNHeadofQueue);

	// Clear out anything that was there before
	FactorWin::MainPage::ResultsList->Items->Clear();

	// Set up the progress bar
	FactorWin::MainPage::QueueProgress->Maximum = LNTarget.GetLongNumberLength() * 2000; 

	Windows::Foundation::IAsyncActionWithProgress<int>^ ScanandDisp = FactorWin::MainPage::CreateChidNodesQueueAS();
	ScanandDisp->Progress =
		ref new Windows::Foundation::AsyncActionProgressHandler<int>
		([this](Windows::Foundation::IAsyncActionWithProgress<int>^, int progress)
	{
		
		// Update Queue info
		FactorWin::MainPage::QueueText->Text = progress.ToString();
		FactorWin::MainPage::QueueProgress->Value = progress;

		// Update results as they become available
		while (SolvedQueue.ReturnQueueSize() > 0)
		{
			FactorNode FNItem = SolvedQueue.Pop();
			LongNumber AValuePassed = FNItem.LNGetAValue();
			LongNumber BValuePassed = FNItem.LNGetBValue();
			Platform::String ^ AValueStr = LongNumberChar(AValuePassed);
			Platform::String ^ BValueStr = LongNumberChar(BValuePassed);
			Platform::String ^ Display = "A : " + AValueStr + " B : " + BValueStr;
			FactorWin::MainPage::ResultsList->Items->Append(Display);

		}

	}
	);
	

}



void CreateChidNodesQueue()
{
	if (FNQueue.ReturnQueueSize() != 0)
	{

		// Add new elements to the queue or solve ones that are already there 
		//cout << "Create Children Nodes" << endl;

		// Get the next element from the queue 
		FactorNode FNItem = FNQueue.Pop();

		// Get the details from the FactorNode
		int Level = FNItem.LNGetLevel();
		// cout << "Level : " << Level << endl;
		LongNumber AValuePassed = FNItem.LNGetAValue();
		LongNumber BValuePassed = FNItem.LNGetBValue();
		// cout << " AValuePassed : " << AValuePassed << " BValuePassed : " << BValuePassed << endl;
		LongNumber MultResult(ResultLen * 2);									// Result of A * B 

		int iTarget = LNTarget.GetValue(Level);
		int iRes = 0;

		for (int iCountA = 0; iCountA <= 9; iCountA++)
		{
			int BCountFrom = 0;
			if (Level == 0) { BCountFrom = iCountA; }
			for (int iCountB = BCountFrom; iCountB <= 9; iCountB++)
			{
				AValuePassed.SetLongNumber(iCountA, Level);
				BValuePassed.SetLongNumber(iCountB, Level);

				MultResult = LongNumberMultiply(AValuePassed, BValuePassed);
				// cout << " MultResult : " << MultResult << " AValuePassed : " << AValuePassed << " BValuePassed : " << BValuePassed << endl;
				iRes = MultResult.GetValue(Level);

				if (iRes == iTarget)
				{
					bool FactorComplete = false;
					int CompResult = LongNumberCompare(MultResult, LNTarget);			// compares, returns 1 if >, 0 if equal and -1 if <
																						// cout << "Comp Result : " << CompResult << " MultResult : " << MultResult << " LNTarget : " << *LNTarget << endl;
					switch (CompResult)
					{
					case -1:			// less than
					{FactorNode FNAdd(
						Level + 1,			// LevelPassed
						AValuePassed,		// AValuePassed
						BValuePassed,		// BValuePassed
						false				// FactorCompletePassed
					);
					// PrintFactorNode(&FNAdd);
					FNQueue.Push(FNAdd);
					DrawQueue.Push(FNAdd);
					break;
					}
					case 0:				// solved
					{FactorComplete = true;
					FactorNode FNAdd(
						Level + 1,			// LevelPassed
						AValuePassed,		// AValuePassed
						BValuePassed,		// BValuePassed
						true				// FactorCompletePassed
					);
					SolvedQueue.Push(FNAdd);
					DrawQueue.Push(FNAdd);
					SolvedCount++;
					break;
					}
					case 1: // greater than so exit B loop
						goto ExitBLoop;
						break;
					default:
						break;
					} // end switch


				} // end if result

			} // end B loop
		ExitBLoop:;
			IterCount++;
		} // end A loop

	} // end if queue

} // end function 



void FactorWin::MainPage::NoAInput_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	// first check for a number in NoA
	Platform::String^ NoAIn = NoAInput->Text;
	std::wstring NoAInW(NoAIn->Begin());
	std::string NoAInA(NoAInW.begin(), NoAInW.end());
	const char* NoACharStr = NoAInA.c_str();
	int LenA = NoAIn->Length();

	Messages->Text = "Checking Input\r\n";
	if (NoAIn->Length() == 0)
	{
		Messages->Text = Messages->Text + "No Input\r\n";
		return;
	}
	else
	{
		Messages->Text = Messages->Text + "Length of Arg : " + LenA + "\r\n";
	}

	LongNumber ArgPassed(NoACharStr);
	LNTarget = ArgPassed;
	ResultLen = LNTarget.GetLongNumberLength();
}



Windows::Foundation::IAsyncActionWithProgress<int>^ FactorWin::MainPage::CreateChidNodesQueueAS()
{
	
	return Concurrency::create_async([this](Concurrency::progress_reporter <int> reporter) -> void
		{

			int QueueSize = FNQueue.ReturnQueueSize();
			//returnStruct ReturnReporter; 
			//ReturnReporter.QueueCount = QueueSize; 
			//ReturnReporter.ThreadCount = 0;
			reporter.report(QueueSize);
			SolvedCount = 0;							// reset solved counter

			// kick off the threads that will consume the new queue items
			while (QueueSize > 0)
			{

				QueueSize = FNQueue.ReturnQueueSize();
				//ReturnReporter.QueueCount = QueueSize;
				reporter.report(QueueSize);
				int ThreadCount = int(QueueSize / 100) + 10;

				vector<thread> threads;

				for (int i = 0; i < ThreadCount; i++) {
					threads.push_back(thread(&CreateChidNodesQueue));
				}

				for (int i = 0; i < ThreadCount; i++) 
				{
					threads[i].join();
				}


			}

				


	}); // end of Async



}





void MainPage::CanvasControl_Draw(CanvasControl^ sender, CanvasDrawEventArgs^ args)
{
	// page offsets
	int XPageLen = 630; 
	int YPageLen = 400; 
	int XOffset = 40;
	int YOffset = 40;
	int XUsable = XPageLen - (2 * XOffset);
	int YUsable = YPageLen - (2 * YOffset);
	int LevelWidth = 50; 
	int CircleSize = 5; 

	int OldXPos = XOffset;
	int OldYPos = (int) (YUsable / 2) ;
	int XPos = OldXPos;
	int YPos = OldYPos;
	Windows::UI::Color CircleColour = Colors::Black;

	// Draw borders and Guidelines
	args->DrawingSession->DrawLine(XOffset, 0, XOffset, YPageLen, Colors::Aqua, 1);								// left vertical
	args->DrawingSession->DrawLine(0, YOffset, XPageLen, YOffset, Colors::Aqua, 1);								// top horizontal
	args->DrawingSession->DrawLine(XPageLen - XOffset, 0, XPageLen - XOffset, YPageLen, Colors::Aqua, 1);		// right vertical 
	args->DrawingSession->DrawLine(0, YPageLen - YOffset, XPageLen, YPageLen - YOffset, Colors::Aqua, 1);		// bottom horizontal
	for (int iCount = 0; iCount <= 10; iCount++)
	{
		args->DrawingSession->DrawLine((iCount * LevelWidth) + XOffset, 0, (iCount * LevelWidth) + XOffset, YPageLen,  Colors::Aqua, 1);
		args->DrawingSession->DrawText("L" + iCount, (iCount * LevelWidth) + XOffset, 2, Colors::Aqua);
	}
	

	// draw head of queue
	args->DrawingSession->DrawEllipse(XPos, YPos, CircleSize, CircleSize, Colors::Black, 1);

	int QueueSize = DrawQueue.ReturnQueueSize();
	int AValLevel = 0; 
	int BValLevel = 0;
	int PreviousAValue = 0; 
	int AValueInt = 0; 
	int AValueInvertedInt = 0;

	while (QueueSize > 0)
	{

		// get the head of the drawing queue
		FactorNode FNItem = DrawQueue.Pop();
		
		// Get the details from the FactorNode
		int Level = FNItem.LNGetLevel();

		if (Level < 10)					// if greater than 10 then don't draw
		{

			// Details for this node
			LongNumber AValuePassed = TrimLongNumber(FNItem.LNGetAValue());
			AValueInt = LongNumbertoInt(AValuePassed); 
			//FactorWin::MainPage::IterationsText->Text = AValueInt.ToString();
			LongNumber AValueInverted = LongNumberInvert(AValuePassed); 
			AValueInvertedInt = LongNumbertoInt(AValueInverted);
			FactorWin::MainPage::IterationsText->Text = AValueInvertedInt.ToString();
			
			// Location of this node
			XPos = (Level * LevelWidth) + XOffset;
			YPos = ((AValueInvertedInt / pow(10, Level)) * YUsable) + YOffset;

			if (Level > 1)
			{
				// Calculate its parent node
				LongNumber AValuePrev = LongNumberReturnPart(AValuePassed, 0, Level);
				LongNumber AValuePrevInverted = LongNumberInvert(AValuePrev); 
				int AValuePrevInvertedInt = LongNumbertoInt(AValuePrevInverted);
				OldXPos = ((Level - 1) * LevelWidth) + XOffset;;
				OldYPos = ((AValuePrevInvertedInt / pow(10, (Level))) * YUsable) + YOffset;
			}
			else
			{
				// old positions = head node
				OldXPos = XOffset;
				OldYPos = (int)(YUsable / 2);
			}

			CircleColour = Colors::Black;
			if (FNItem.GetFactorComplete() == true) 
			{ 
				CircleColour = Colors::Yellow; 
			}

			//args->DrawingSession->DrawLine(OldXPos, OldYPos, XPos, YPos, Colors::Black, 1);
			args->DrawingSession->DrawEllipse(XPos, YPos, 5, 5, CircleColour, 1);
			args->DrawingSession->DrawText("A" + AValueInt, XPos, YPos, Colors::Aqua);
		
		} // end if 

		// check queue size for while loop 
		QueueSize = DrawQueue.ReturnQueueSize();


	}



}


void FactorWin::MainPage::Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	FactorWin::MainPage::Canvas->Invalidate();


}
