#include "CommInterface.au3"

func ShowError()
	MsgBox(16, "Error", "Error " & @error)
endfunc

func OpenPort()
	local const $iPort = 8
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

func GetX($x)
	return (127 * $x / 1366)
endfunc

func GetY($y)
	return (127 * $y / 768)
endfunc

func SendArduino($hPort, $x, $y, $button)
	local $command[4] = [0xDC, GetX($x), GetY($y), $button]

	_CommAPI_TransmitString($hPort, StringFromASCIIArray($command, 0, UBound($command), 1))

	if @error then ShowError()
endfunc

func ClosePort($hPort)
	_CommAPI_ClosePort($hPort)
	if @error then ShowError()
endfunc

$hWnd = WinGetHandle("[CLASS:MSPaintApp]")
WinActivate($hWnd)
Sleep(200)

$hPort = OpenPort()

SendArduino($hPort, 250, 300, 1)

ClosePort($hPort)
