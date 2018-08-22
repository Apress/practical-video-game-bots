global const $gKeyHandler = "_KeyHandler"
global const $kLogFile = "debug.log"

global $gTimeSpanA = -1
global $gPrevTimestampA = -1

func LogWrite($data)
	FileWrite($kLogFile, $data & chr(10))
endfunc

func _KeyHandler()
	$keyPressed = @HotKeyPressed

	LogWrite("_KeyHandler() - asc = " & asc($keyPressed) & " key = " & $keyPressed);
	AnalyzeKey($keyPressed)

	HotKeySet($keyPressed)
	Send($keyPressed)
	HotKeySet($keyPressed, $gKeyHandler)
endfunc

func InitKeyHooks($handler)
	for $i = 0 to 256
		HotKeySet(Chr($i), $handler)
	next
endfunc

func AnalyzeKey($key)
	local $timestamp = (@SEC * 1000 + @MSEC)
	LogWrite("AnalyzeKey() - key = " & $key & " msec = " & $timestamp)
	if $key <> 'a' then
		return
	endif

	if $gPrevTimestampA = -1 then
		$gPrevTimestampA = $timestamp
		return
	endif

	local $newTimeSpan = $timestamp - $gPrevTimestampA
	$gPrevTimestampA = $timestamp

	if $gTimeSpanA = -1 then
		$gTimeSpanA = $newTimeSpan
		return
	endif

	if Abs($gTimeSpanA - $newTimeSpan) < 100 then
		MsgBox(0, "Alert", "Clicker bot detected!")
	endif
endfunc

InitKeyHooks($gKeyHandler)

while true
	Sleep(10)
wend
