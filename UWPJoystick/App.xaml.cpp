//
// App.xaml.cpp
// App クラスの実装。
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace UWPJoystick;

using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

/// <summary>
/// 単一アプリケーション オブジェクトを初期化します。これは、実行される作成したコードの
///最初の行であるため、main() または WinMain() と論理的に等価です。
/// </summary>
App::App()
{
    InitializeComponent();
    Suspending += ref new SuspendingEventHandler(this, &App::OnSuspending);

	InitJoystick();
}

/// <summary>
/// アプリケーションがエンド ユーザーによって正常に起動されたときに呼び出されます。他のエントリ ポイントは、
/// アプリケーションが特定のファイルを開くために起動されたときなどに使用されます。
/// </summary>
/// <param name="e">起動の要求とプロセスの詳細を表示します。</param>
void App::OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e)
{
    auto rootFrame = dynamic_cast<Frame^>(Window::Current->Content);

    // ウィンドウに既にコンテンツが表示されている場合は、アプリケーションの初期化を繰り返さずに、
    // ウィンドウがアクティブであることだけを確認してください
    if (rootFrame == nullptr)
    {
        // ナビゲーション コンテキストとして動作するフレームを作成し、
        // SuspensionManager キーに関連付けます
        rootFrame = ref new Frame();

        rootFrame->NavigationFailed += ref new Windows::UI::Xaml::Navigation::NavigationFailedEventHandler(this, &App::OnNavigationFailed);

        if (e->PreviousExecutionState == ApplicationExecutionState::Terminated)
        {
            // TODO: 必要な場合のみ、保存されたセッション状態を復元し、
            // 復元完了後に最後の起動手順をスケジュールします

        }

        if (e->PrelaunchActivated == false)
        {
            if (rootFrame->Content == nullptr)
            {
                // ナビゲーション スタックが復元されない場合は、最初のページに移動します。
                // このとき、必要な情報をナビゲーション パラメーターとして渡して、新しいページを
                // 作成します
                rootFrame->Navigate(TypeName(MainPage::typeid), e->Arguments);
            }
            // フレームを現在のウィンドウに配置します
            Window::Current->Content = rootFrame;
            // 現在のウィンドウがアクティブであることを確認します
            Window::Current->Activate();
        }
    }
    else
    {
        if (e->PrelaunchActivated == false)
        {
            if (rootFrame->Content == nullptr)
            {
                // ナビゲーション スタックが復元されない場合は、最初のページに移動します。
                // このとき、必要な情報をナビゲーション パラメーターとして渡して、新しいページを
                // 構成します
                rootFrame->Navigate(TypeName(MainPage::typeid), e->Arguments);
            }
            // 現在のウィンドウがアクティブであることを確認します
            Window::Current->Activate();
        }
    }
}

/// <summary>
/// アプリケーションの実行が中断されたときに呼び出されます。
/// アプリケーションが終了されるか、メモリの内容がそのままで再開されるかに
/// かかわらず、アプリケーションの状態が保存されます。
/// </summary>
/// <param name="sender">中断要求の送信元。</param>
/// <param name="e">中断要求の詳細。</param>
void App::OnSuspending(Object^ sender, SuspendingEventArgs^ e)
{
    (void) sender;  // 未使用のパラメーター
    (void) e;   // 未使用のパラメーター

    //TODO: アプリケーションの状態を保存してバックグラウンドの動作があれば停止します
}

/// <summary>
/// 特定のページへの移動が失敗したときに呼び出されます
/// </summary>
/// <param name="sender">移動に失敗したフレーム</param>
/// <param name="e">ナビゲーション エラーの詳細</param>
void App::OnNavigationFailed(Platform::Object ^sender, Windows::UI::Xaml::Navigation::NavigationFailedEventArgs ^e)
{
    throw ref new FailureException("Failed to load Page " + e->SourcePageType.Name);
}

void UWPJoystick::App::InitJoystick()
{
	//!< ゲームパッド
	Gamepads = ref new Vector<Gamepad^>();
	//!< この時点で列挙されることは殆ど無い
	//for (auto i : Gamepad::Gamepads) {
	//	Gamepads->Append(i);
	//}
	//!< タイムラグ後、コールバックされる
	Gamepad::GamepadAdded += ref new EventHandler<Gamepad^>([&](Platform::Object^, Gamepad^ args) {
		Gamepads->Append(args);
		UpdateJoystick();
	});
	Gamepad::GamepadRemoved += ref new EventHandler<Gamepad^>([&](Platform::Object^, Gamepad^ args) {
		unsigned int i;
		if (Gamepads->IndexOf(args, &i)) {
			Gamepads->RemoveAt(i);
		}
	});

	//!< アーケードスティック
	ArcadeSticks = ref new Vector<ArcadeStick^>();
	//for (auto i : ArcadeStick::ArcadeSticks) {
	//	ArcadeSticks->Append(i);
	//}
	ArcadeStick::ArcadeStickAdded += ref new EventHandler<ArcadeStick^>([&](Platform::Object^, ArcadeStick^ args) {
		ArcadeSticks->Append(args);
		UpdateJoystick();
	});
	ArcadeStick::ArcadeStickRemoved += ref new EventHandler<ArcadeStick^>([&](Platform::Object^, ArcadeStick^ args) {
		unsigned int i;
		if (ArcadeSticks->IndexOf(args, &i)) {
			ArcadeSticks->RemoveAt(i);
		}
	});

	//!< レーシングホイール
	RacingWheels = ref new Vector<RacingWheel^>();
	//for (auto i : RacingWheel::RacingWheels) {
	//	RacingWheels->Append(i);
	//}
	RacingWheel::RacingWheelAdded += ref new EventHandler<RacingWheel^>([&](Platform::Object^, RacingWheel^ args) {
		RacingWheels->Append(args);
		UpdateJoystick();
	});
	RacingWheel::RacingWheelRemoved += ref new EventHandler<RacingWheel^>([&](Platform::Object^, RacingWheel^ args) {
		unsigned int i;
		if (RacingWheels->IndexOf(args, &i)) {
			RacingWheels->RemoveAt(i);
		}
	});

	//!< フライトスティック
	FlightSticks = ref new Vector<FlightStick^>();
	//for (auto i : FlightStick::FlightSticks) {
	//	FlightSticks->Append(i);
	//}
	FlightStick::FlightStickAdded += ref new EventHandler<FlightStick^>([&](Platform::Object^, FlightStick^ args) {
		FlightSticks->Append(args);
		UpdateJoystick();
	});
	FlightStick::FlightStickRemoved += ref new EventHandler<FlightStick^>([&](Platform::Object^, FlightStick^ args) {
		unsigned int i;
		if (FlightSticks->IndexOf(args, &i)) {
			FlightSticks->RemoveAt(i);
		}
	});
}

void UWPJoystick::App::UpdateJoystick()
{
	for (auto i : Gamepads) {
		auto Reading = i->GetCurrentReading();

		//!< 必須
		if (GamepadButtons::Menu == (Reading.Buttons & GamepadButtons::Menu)) {}
		if (GamepadButtons::View == (Reading.Buttons & GamepadButtons::View)) {}
		if (GamepadButtons::A == (Reading.Buttons & GamepadButtons::A)) {}
		if (GamepadButtons::B == (Reading.Buttons & GamepadButtons::B)) {}
		if (GamepadButtons::X == (Reading.Buttons & GamepadButtons::X)) {}
		if (GamepadButtons::Y == (Reading.Buttons & GamepadButtons::Y)) {}
		if (GamepadButtons::LeftShoulder == (Reading.Buttons & GamepadButtons::LeftShoulder)) {}
		if (GamepadButtons::RightShoulder == (Reading.Buttons & GamepadButtons::RightShoulder)) {}
		if (GamepadButtons::LeftThumbstick == (Reading.Buttons & GamepadButtons::LeftThumbstick)) {}
		if (GamepadButtons::RightThumbstick == (Reading.Buttons & GamepadButtons::RightThumbstick)) {}
		if (GamepadButtons::DPadUp == (Reading.Buttons & GamepadButtons::DPadUp)) {}
		if (GamepadButtons::DPadDown == (Reading.Buttons & GamepadButtons::DPadDown)) {}
		if (GamepadButtons::DPadLeft == (Reading.Buttons & GamepadButtons::DPadLeft)) {}
		if (GamepadButtons::DPadRight == (Reading.Buttons & GamepadButtons::DPadRight)) {}
		Reading.LeftTrigger;
		Reading.RightTrigger;
		const auto DeadzoneSq = 0.1f * 0.1f;
		const auto LXSq = Reading.LeftThumbstickX * Reading.LeftThumbstickX;
		const auto LYSq = Reading.LeftThumbstickY * Reading.LeftThumbstickY;
		if (LXSq + LYSq > DeadzoneSq) {
			Reading.LeftThumbstickX;
			Reading.LeftThumbstickY;
		}
		const auto RXSq = Reading.RightThumbstickX * Reading.RightThumbstickX;
		const auto RYSq = Reading.RightThumbstickY * Reading.RightThumbstickY;
		if (RXSq + RYSq > DeadzoneSq) {
			Reading.RightThumbstickX;
			Reading.RightThumbstickY;
		}

		//!< 非必須
		if (GamepadButtons::Paddle1 == (Reading.Buttons & GamepadButtons::Paddle1)) {}
		if (GamepadButtons::Paddle2 == (Reading.Buttons & GamepadButtons::Paddle2)) {}
		if (GamepadButtons::Paddle3 == (Reading.Buttons & GamepadButtons::Paddle3)) {}
		if (GamepadButtons::Paddle4 == (Reading.Buttons & GamepadButtons::Paddle4)) {}
	}

	for (auto i : ArcadeSticks) {
		auto Reading = i->GetCurrentReading();
	}

	for (auto i : RacingWheels) {
		auto Reading = i->GetCurrentReading();

		//!< 必須
		if (RacingWheelButtons::PreviousGear == (Reading.Buttons & RacingWheelButtons::PreviousGear)) {}
		if (RacingWheelButtons::NextGear == (Reading.Buttons & RacingWheelButtons::NextGear)) {}
		i->MaxWheelAngle;
		Reading.Wheel;
		Reading.Throttle;
		Reading.Brake;
		
		//!< 非必須
		if (RacingWheelButtons::DPadUp == (Reading.Buttons & RacingWheelButtons::DPadUp)) {}
		if (RacingWheelButtons::DPadDown == (Reading.Buttons & RacingWheelButtons::DPadDown)) {}
		if (RacingWheelButtons::DPadLeft == (Reading.Buttons & RacingWheelButtons::DPadLeft)) {}
		if (RacingWheelButtons::DPadRight == (Reading.Buttons & RacingWheelButtons::DPadRight)) {}
		if (RacingWheelButtons::Button1 == (Reading.Buttons & RacingWheelButtons::Button1)) {}
		//!< ...
		if (RacingWheelButtons::Button16 == (Reading.Buttons & RacingWheelButtons::Button16)) {}
		if (i->HasClutch) {
			Reading.Clutch;
		}
		if (i->HasHandbrake) {
			Reading.Handbrake;
		}
		if (i->HasPatternShifter) {
			i->MaxPatternShifterGear;
			Reading.PatternShifterGear;
		}
	}

	for (auto i : FlightSticks) {
		auto Reading = i->GetCurrentReading();
	}
}
