﻿//------------------------------------------------------------------------------
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
//------------------------------------------------------------------------------
#include "pch.h"

#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BINDING_DEBUG_OUTPUT
extern "C" __declspec(dllimport) int __stdcall IsDebuggerPresent();
#endif

#include "MainPage.xaml.h"

void ::FactorWin::MainPage::InitializeComponent()
{
    if (_contentLoaded)
    {
        return;
    }
    _contentLoaded = true;
    ::Windows::Foundation::Uri^ resourceLocator = ref new ::Windows::Foundation::Uri(L"ms-appx:///MainPage.xaml");
    ::Windows::UI::Xaml::Application::LoadComponent(this, resourceLocator, ::Windows::UI::Xaml::Controls::Primitives::ComponentResourceLocation::Application);
}

void ::FactorWin::MainPage::Connect(int __connectionId, ::Platform::Object^ __target)
{
    switch (__connectionId)
    {
        case 1:
            {
                this->NoAInput = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(this->NoAInput))->TextChanged += ref new ::Windows::UI::Xaml::Controls::TextChangedEventHandler(this, (void (::FactorWin::MainPage::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::Controls::TextChangedEventArgs^))&MainPage::NoAInput_TextChanged);
            }
            break;
        case 2:
            {
                this->NoBInput = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            }
            break;
        case 3:
            {
                this->FactorButton = safe_cast<::Windows::UI::Xaml::Controls::Button^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::Button^>(this->FactorButton))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::FactorWin::MainPage::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&MainPage::FactorButton_Click);
            }
            break;
        case 4:
            {
                this->Messages = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            }
            break;
        case 5:
            {
                this->ResultsList = safe_cast<::Windows::UI::Xaml::Controls::ListBox^>(__target);
            }
            break;
        case 6:
            {
                this->ThreadProgress = safe_cast<::Windows::UI::Xaml::Controls::ProgressBar^>(__target);
            }
            break;
        case 7:
            {
                this->ThreadsText = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            }
            break;
        case 8:
            {
                this->QueueProgress = safe_cast<::Windows::UI::Xaml::Controls::ProgressBar^>(__target);
            }
            break;
        case 9:
            {
                this->QueueText = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            }
            break;
        case 10:
            {
                this->IterationsText = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            }
            break;
        case 11:
            {
                this->UpdateList = safe_cast<::Windows::UI::Xaml::Controls::Button^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::Button^>(this->UpdateList))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::FactorWin::MainPage::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&MainPage::Button_Click);
            }
            break;
    }
    _contentLoaded = true;
}

::Windows::UI::Xaml::Markup::IComponentConnector^ ::FactorWin::MainPage::GetBindingConnector(int __connectionId, ::Platform::Object^ __target)
{
    __connectionId;         // unreferenced
    __target;               // unreferenced
    return nullptr;
}


