global const $kLogFile = "debug.log"

func LogWrite($data)
	FileWrite($kLogFile, $data & chr(10))
endfunc

func ScanProcess($name)
	local $processList = ProcessList($name)

	if $processList[0][0] > 0 then
		LogWrite("Name: " & $processList[1][0] & " PID: " & $processList[1][1])
		MsgBox(0, "Alert", "Clicker bot detected!")
	endif
endfunc

while true
	ScanProcess("AutoHotKey.exe")
	Sleep(5000)
wend