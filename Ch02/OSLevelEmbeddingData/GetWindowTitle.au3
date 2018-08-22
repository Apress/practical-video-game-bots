#include <WinAPI.au3>
Sleep(5 * 1000)
$handle = WinGetHandle('[Active]')
MsgBox(0, "", "Title   : " & WinGetTitle($handle) & @CRLF & "Class : " & _WinAPI_GetClassName($handle))