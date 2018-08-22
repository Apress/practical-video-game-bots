#include "FastFind.au3"

#RequireAdmin

Sleep(2000)

global const $LogFile = "debug.log"
	
func LogWrite($data)
	FileWrite($LogFile, $data & chr(10))
endfunc

func IsTargetExist()
	const $SizeSearch = 80
	const $MinNbPixel = 3
	const $OptNbPixel = 10
	const $PosX = 688
	const $PosY = 67
	
	$coords = FFBestSpot($SizeSearch, $MinNbPixel, $OptNbPixel, $PosX, $PosY, 0x871D18, 10)

	const $MaxX = 800
	const $MinX = 575
	const $MaxY = 100
	
	if not @error then
		if $MinX < $coords[0] and $coords[0] < $MaxX and $coords[1] < $MaxY then
			LogWrite("IsTargetExist() - Success, coords = " & $coords[0] & ", " & $coords[1] & " pixels = " & $coords[2])
			return True
		else
			LogWrite("IsTargetExist() - Fail #1")
			return False
		endif
	else
		LogWrite("IsTargetExist() - Fail #2")
		return False
	endif
endfunc

func SelectTarget()
	LogWrite("SelectTarget()")
	while not IsTargetExist()
		Send("{F9}")
		Sleep(200)
	wend
endfunc

func Attack()
	LogWrite("Attack()")
	while IsTargetExist()
		Send("{F1}")
		Sleep(1000)
	wend
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