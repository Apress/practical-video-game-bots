#include "CommInterface.au3"

func ShowError()
	MsgBox(16, "Error", "Error " & @error)
endfunc

func OpenPort()
	local const $iPort = 10
	local const $iBaud = 9600
	local const $iParity = 0
	local const $iByteSize = 8
	local const $iStopBits = 1

	$hPort = _CommAPI_OpenCOMPort($iPort, $iBaud, $iParity, $iByteSize, $iStopBits)
	if @error then
		ShowError()
		return NULL
	endif
 
	_CommAPI_ClearCommError($hPort)
	if @error then
		ShowError()
		return NULL
	endif
 
	_CommAPI_PurgeComm($hPort)
	if @error then
		ShowError()
		return NULL
	endif

	return $hPort
endfunc

func SendArduinoKeyboard($hPort, $modifier, $key)
	if $modifier == NULL then
		local $command[5] = [0xDC, 0x1, 0xFF, $key, 0xFF]
	else
		local $command[5] = [0xDC, 0x2, $modifier, $key, 0xFF]
	endif

	_CommAPI_TransmitString($hPort, StringFromASCIIArray($command, 0, UBound($command), 1))

	if @error then ShowError()
endfunc

func GetX($x)
	return (127 * $x / 1366)
endfunc

func GetY($y)
	return (127 * $y / 768)
endfunc

func SendArduinoMouse($hPort, $x, $y, $button)
	local $command[5] = [0xDC, 0x3, GetX($x), GetY($y), $button]

	_CommAPI_TransmitString($hPort, StringFromASCIIArray($command, 0, UBound($command), 1))

	if @error then ShowError()
endfunc

func ClosePort($hPort)
	_CommAPI_ClosePort($hPort)
	if @error then ShowError()
endfunc

$hPort = OpenPort()

$hWnd = WinGetHandle("[CLASS:MSPaintApp]")
WinActivate($hWnd)
Sleep(200)

SendArduinoMouse($hPort, 250, 300, 1)

Sleep(1000)

$hWnd = WinGetHandle("[CLASS:Notepad]")
WinActivate($hWnd)
Sleep(200)

SendArduinoKeyboard($hPort, Null, 0x54) ; T
SendArduinoKeyboard($hPort, Null, 0x65) ; e
SendArduinoKeyboard($hPort, Null, 0x73) ; s
SendArduinoKeyboard($hPort, Null, 0x74) ; t

Sleep(1000)

SendArduinoKeyboard($hPort, 0x82, 0xB3) ; Alt+Tab

ClosePort($hPort)
