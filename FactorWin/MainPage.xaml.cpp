//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include "LongNumber.h"
#include "FactorQueue.h"
#include <atlstr.h>
#include <gdiplus.h>

// Added for UW Concurrency 
#include <ppltasks.h>
using namespace concurrency;

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

// using namespace System::Runtime::InteropServices;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

// Create one, zero as long numbers - global scope so they don't get reinitiatlised for each thread
LongNumber One(1);
LongNumber Zero(1);

// Set the target as a global variable so we don't need to pass it around 
LongNumber LNTarget;
int ResultLen = 0;
int IterCount = 0; 

// Create the queue 
FactorQueue FNQueue;
FactorQueue SolvedQueue;

// Counter for number solved - global so doesn't need to be passed around 
int SolvedCount = 0;

// declare the functions we will use later to build the nodes and solve the factor and update the screen
void CreateChidNodesQueue();



MainPage::MainPage()
{
	InitializeComponent();
}

void FactorWin::MainPage::UpdateScreen()
{
	FactorWin::MainPage::QueueText->Text = FNQueue.ReturnQueueSize().ToString();

}

void FactorWin::MainPage::UpdateList()
{
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


void  FactorWin::MainPage::FactorWork()
{
	SolvedCount = 0;							// reset solved counter

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

	// Set up standard long numbers  - one, zero 
	One.SetLongNumber(1, 0);
	Zero.SetLongNumber(0, 0);

	// clear the text file for node overflow
	remove("QueueNode.txt");

	// Create the head of the Queue 
	FactorNode FNHeadofQueue(0, Zero, Zero, false);
	FNQueue.Push(FNHeadofQueue);

	// Clear out anything that was there before
	FactorWin::MainPage::ResultsList->Items->Clear();

	// kick off the threads that will consume the new queue items
	using namespace placeholders;
	while (FNQueue.ReturnQueueSize() > 0)
	{
		// Print Queue info
		//FactorWin::MainPage::QueueText->Text = FNQueue.ReturnQueueSize().ToString();

		FactorWin::MainPage::UpdateScreen();
		FactorWin::MainPage::UpdateList();

		// Added for UWP concurrency 
		auto NodesChildCreate0 = create_task(CreateChidNodesQueue);
		NodesChildCreate0.then([](void) {});

		auto NodesChildCreate1 = create_task(CreateChidNodesQueue);
		NodesChildCreate1.then([](void) {});

		auto NodesChildCreate2 = create_task(CreateChidNodesQueue);
		NodesChildCreate2.then([](void) {});

		auto NodesChildCreate3 = create_task(CreateChidNodesQueue);
		NodesChildCreate3.then([](void) {});

		// Std C++ Concurrency 
		//thread CreateComsume0(&CreateChidNodesQueue);
		//thread CreateComsume1(&CreateChidNodesQueue);
		//thread CreateComsume2(&CreateChidNodesQueue);
		//thread CreateComsume3(&CreateChidNodesQueue);
		//thread CreateComsume4(&CreateChidNodesQueue);
		//
		//CreateComsume0.join();
		//CreateComsume1.detach();
		//CreateComsume2.detach();
		//CreateComsume3.detach();
		//CreateComsume4.detach();

	}

	FactorWin::MainPage::IterationsText->Text = IterCount.ToString();
	Messages->Text = Messages->Text + "Solved Count : " + SolvedCount + "\r\n";

	FactorWin::MainPage::UpdateList();


}





void FactorWin::MainPage::FactorButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	// Called when Factor Button clicked
	FactorWin::MainPage::FactorWork();
	// auto FactorWorkOP = create_task(FactorWork);


	/*IAsyncOperation<auto>^ deviceOp = FactorWin::MainPage::FactorWork();
	auto FactorWorkOP = create_task(FactorWin::MainPage::FactorWork);
	FactorWorkOP.then([](void) {});
	*/

	//
	//
	//SolvedCount = 0;							// reset solved counter

	//// first check for a number in NoA
	//Platform::String^ NoAIn = NoAInput->Text;
	//std::wstring NoAInW(NoAIn->Begin());
	//std::string NoAInA(NoAInW.begin(), NoAInW.end());
	//const char* NoACharStr = NoAInA.c_str();
	//int LenA = NoAIn->Length();

	//Messages->Text = "Checking Input\r\n";
	//if (NoAIn->Length() == 0)
	//{
	//	Messages->Text = Messages->Text + "No Input\r\n";
	//	return;
	//}
	//else
	//{
	//	Messages->Text = Messages->Text + "Length of Arg : " + LenA + "\r\n";
	//}
	//
	//LongNumber ArgPassed(NoACharStr);
	//LNTarget = ArgPassed; 
	//ResultLen = LNTarget.GetLongNumberLength();

	//// Set up standard long numbers  - one, zero 
	//One.SetLongNumber(1, 0);
	//Zero.SetLongNumber(0, 0);

	//// clear the text file for node overflow
	//remove("QueueNode.txt");

	//// Create the head of the Queue 
	//FactorNode FNHeadofQueue(0, Zero, Zero, false); 
	//FNQueue.Push(FNHeadofQueue);

	//// Clear out anything that was there before
	//FactorWin::MainPage::ResultsList->Items->Clear();

	//// kick off the threads that will consume the new queue items
	//using namespace placeholders;
	//while (FNQueue.ReturnQueueSize() > 0)
	//{
	//	// Print Queue info
	//	//FactorWin::MainPage::QueueText->Text = FNQueue.ReturnQueueSize().ToString();

	//	FactorWin::MainPage::UpdateScreen();
	//	FactorWin::MainPage::UpdateList();

	//	// Added for UWP concurrency 
	//	auto NodesChildCreate0 = create_task(CreateChidNodesQueue);
	//	NodesChildCreate0.then([](void) {});

	//	auto NodesChildCreate1 = create_task(CreateChidNodesQueue);
	//	NodesChildCreate1.then([](void) {});

	//	auto NodesChildCreate2 = create_task(CreateChidNodesQueue);
	//	NodesChildCreate2.then([](void) {});

	//	auto NodesChildCreate3 = create_task(CreateChidNodesQueue);
	//	NodesChildCreate3.then([](void) {});

	//	// Std C++ Concurrency 
	//	//thread CreateComsume0(&CreateChidNodesQueue);
	//	//thread CreateComsume1(&CreateChidNodesQueue);
	//	//thread CreateComsume2(&CreateChidNodesQueue);
	//	//thread CreateComsume3(&CreateChidNodesQueue);
	//	//thread CreateComsume4(&CreateChidNodesQueue);
	//	//
	//	//CreateComsume0.join();
	//	//CreateComsume1.detach();
	//	//CreateComsume2.detach();
	//	//CreateComsume3.detach();
	//	//CreateComsume4.detach();
	//	
	//}

	//FactorWin::MainPage::IterationsText->Text = IterCount.ToString();
	//Messages->Text = Messages->Text + "Solved Count : " + SolvedCount + "\r\n";

	//FactorWin::MainPage::UpdateList();



}

void FactorWin::MainPage::IterationsText_TextChanged(Platform::Object ^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs ^ e)
{
	QueueText->Text = "One";

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




