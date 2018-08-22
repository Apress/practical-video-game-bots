SRandom(@MSEC)
$hWnd = WinGetHandle("[CLASS:Notepad]")
WinActivate($hWnd)
Sleep(200)

while true
	Send("a")
	Sleep(1000)
	if Random(0, 9, 1) < 5 then
		Send("b")
		Sleep(2000)
	endif
	Send("c")
	Sleep(1500)
wend