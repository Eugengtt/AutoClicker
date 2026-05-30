# Universal Auto-Clicker Pro v4

A fast, lightweight, and efficient auto-clicker for Windows, written natively in C++ using the Win32 API. The application runs seamlessly in the background without draining system resources, simulating mouse clicks directly through "old-school" system functions.

## 🚀 Key Features

* **Customizable Interval:** Set the exact delay between clicks in milliseconds (ms).
* **Mouse Button Options:** Full support for both **Left** and **Right** clicks.
* **Repeat Modes:**
  * **Infinite:** Clicks continuously until you manually stop it.
  * **Fixed Count:** Stops automatically after reaching a predefined number of clicks.
* **Global Hotkey:** Press **F3** from anywhere (even while inside a game or another window) to toggle the auto-clicker instantly.
* **Zero Lag:** Clean, native Windows GUI that keeps CPU usage at an absolute minimum.

## 🛠️ How to Use

| Step | Action |
| :--- | :--- |
| **1** | Open the application and set your desired interval in milliseconds (e.g., `10`). |
| **2** | Choose the target mouse button (Left/Right) and select your preferred repeat mode. |
| **3** | Move your cursor to the target area and press **F3** to START clicking. |
| **4** | Press **F3** again (or click the button in the UI) to STOP. |

## 💻 Technical Specifications

* **Language:** C++
* **Interface:** Win32 API (Native Windows GUI)
* **Simulation Method:** `mouse_event` (Classic Windows API)
* **Compatibility:** Windows 10 / Windows 11
