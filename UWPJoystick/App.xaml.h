//
// App.xaml.h
// App クラスの宣言。
//

#pragma once

#include "App.g.h"

using namespace Platform::Collections;
using namespace Windows::Gaming::Input;

namespace UWPJoystick
{
	/// <summary>
	/// 既定の Application クラスを補完するアプリケーション固有の動作を提供します。
	/// </summary>
	ref class App sealed
	{
	protected:
		virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e) override;

	internal:
		App();

	private:
		void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ e);
		void OnNavigationFailed(Platform::Object ^sender, Windows::UI::Xaml::Navigation::NavigationFailedEventArgs ^e);

	public:
		void InitJoystick();
		void UpdateJoystick();

	private:
		Vector<Gamepad^>^ Gamepads;
		Vector<ArcadeStick^>^ ArcadeSticks;
		Vector<RacingWheel^>^ RacingWheels;
		Vector<FlightStick^>^ FlightSticks;
	};
}
