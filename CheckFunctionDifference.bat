@echo off

set baseFile=%1
set changedFile=%2

ctags -x --c-types=f --format=1 %baseFile% > baseFunctions.txt
ctags -x --c-types=f --format=1 %changedFile% > changedFunctions.txt

FunctionDifference.exe baseFunctions.txt %baseFile% changedFunctions.txt %changedFile%