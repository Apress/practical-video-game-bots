SRandom(@MSEC)
$hWnd = WinGetHandle("[CLASS:Notepad]")
WinActivate($hWnd)
Sleep(200)

while true
	Send("a")
	Sleep(Random(800, 1200))
	Send("b")
	Sleep(Random(1700, 2300))
	Send("c")
	Sleep(Random(1300, 1700))
wend