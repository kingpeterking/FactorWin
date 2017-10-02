//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"

namespace FactorWin
{

	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
	
	public:
		MainPage();

		Windows::Foundation::IAsyncActionWithProgress<int>^ CreateChidNodesQueueAS();

		void CanvasControl_Draw(Microsoft::Graphics::Canvas::UI::Xaml::CanvasControl^ sender,
			Microsoft::Graphics::Canvas::UI::Xaml::CanvasDrawEventArgs^ args);


	private:
		// Click button
		void FactorButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		
		// Build LN as text is inputted
		void NoAInput_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);


		void Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
