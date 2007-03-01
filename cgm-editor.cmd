@echo off
cgm-editor.exe %1 %2 %3 %4 %5 %6 %7 %8 %9
if not errorlevel 1 goto ende
pause
:ende
