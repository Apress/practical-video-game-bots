#include <WinAPI.au3>

global const $kLogFile = "debug.log"
global $gHook

func LogWrite($data)
	FileWrite($kLogFile, $data & chr(10))
endfunc

func _KeyHandler($nCode, $wParam, $lParam)
	if $nCode < 0 then
		return _WinAPI_CallNextHookEx($gHook, $nCode, $wParam, $lParam)
	endIf

	local $keyHooks = DllStructCreate($tagKBDLLHOOKSTRUCT, $lParam)

	LogWrite("_KeyHandler() - keyccode = " & DllStructGetData($keyHooks, "vkCode"));

	local $flags = DllStructGetData($keyHooks, "flags")
	if $flags = $LLKHF_INJECTED then
		MsgBox(0, "Alert", "Clicker bot detected!")
	endif

	return _WinAPI_CallNextHookEx($gHook, $nCode, $wParam, $lParam)
endfunc

func InitKeyHooks($handler)
	local $keyHandler = DllCallbackRegister($handler, "long", "int;wparam;lparam")
	local $hMod = _WinAPI_GetModuleHandle(0)
	$gHook = _WinAPI_SetWindowsHookEx($WH_KEYBOARD_LL, DllCallbackGetPtr($keyHandler), $hMod)
endfunc

InitKeyHooks("_KeyHandler")

while true
	Sleep(10)
wend