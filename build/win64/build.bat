cd ..\..\newhope
zip -r newhope.love *
cd ..\build\win64
copy /b love64.exe+..\..\newhope\newhope.love NewHope.exe
del ..\..\newhope\newhope.love
