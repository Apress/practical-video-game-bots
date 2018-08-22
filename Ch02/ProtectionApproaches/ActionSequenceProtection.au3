global const $gKeyHandler = "_KeyHandler"
global const $kLogFile = "debug.log"

global const $gActionTemplate[3] = ['a', 'b', 'c']
global $gActionIndex = 0
global $gCounter = 0

func LogWrite($data)
	FileWrite($kLogFile, $data & chr(10))
endfunc

func _KeyHandler()
	$key_pressed = @HotKeyPressed

	AnalyzeKey($key_pressed)

	HotKeySet($key_pressed)
	Send($key_pressed)
	HotKeySet($key_pressed, $gKeyHandler)
endfunc

func InitKeyHooks($handler)
	for $i = 0 to 256
		HotKeySet(Chr($i), $handler)
	next
endfunc

func Reset()
	$gActionIndex = 0
	$gCounter = 0
endfunc

func AnalyzeKey($key)
	LogWrite("AnalyzeKey() - key = " & $key);

	$indexMax = UBound($gActionTemplate) - 1
	if $gActionIndex <= $indexMax and $key <> $gActionTemplate[$gActionIndex] then
		Reset()
		return
	endif

	if $gActionIndex < $indexMax and $key = $gActionTemplate[$gActionIndex] then
		$gActionIndex += 1
		return
	endif

	if $gActionIndex = $indexMax and $key = $gActionTemplate[$gActionIndex] then
		$gCounter += 1
		$gActionIndex = 0

		if $gCounter = 3 then
			MsgBox(0, "Alert", "Clicker bot detected!")
			Reset()
		endif
	endif
endfunc

InitKeyHooks($gKeyHandler)

while true
	Sleep(10)
wend