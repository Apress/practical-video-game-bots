#RequireAdmin

Sleep(2000)

func SelectTarget()
	Send("{F9}")
	Sleep(200)
endfunc

func Attack()
	Send("{F1}")
	Sleep(5000)
endfunc

func Pickup()
	Send("{F8}")
	Sleep(1000)
endfunc

while true
	SelectTarget()
	Attack()
	Pickup()
wend