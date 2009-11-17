rd /s /q CoinMP_zip
mkdir CoinMP_zip
cd CoinMP_zip

mkdir src
mkdir vb6
mkdir csharp
cd csharp
mkdir Properties
mkdir bin
cd bin
mkdir Release
cd ..\..

copy ..\MSVisualStudio\v9\release\coinmp.dll
copy ..\MSVisualStudio\v9\release\coinmp.exp
copy ..\MSVisualStudio\v9\release\coinmp.lib
copy ..\MSVisualStudio\v9\release\unittest.exe

copy ..\MSVisualStudio\v9\x64\release\coinmp.dll coinmp_64.dll

copy ..\examples\vb6\CoinMPTestVB6.exe

copy ..\examples\csharp\bin\Release\CoinMPTestCS8.exe
copy ..\examples\csharp\bin\Release\CoinMPTestCS9.exe

copy ..\test\unittest.cpp

copy ..\examples\example.c

copy ..\src\CoinMP.h

copy ..\src\*.cpp      src
copy ..\src\*.c        src
copy ..\src\*.h        src
copy ..\src\coinmp.def src

copy ..\examples\vb6\*.bas              vb6
copy ..\examples\vb6\CoinMPTestVB6.vbp  vb6
copy ..\examples\vb6\CoinMPTestVB6.exe  vb6
copy ..\examples\vb6\FrmCoinMP.frm      vb6


copy ..\examples\csharp\*.cs         csharp
copy ..\examples\csharp\*.csproj     csharp
copy ..\examples\csharp\*.sln        csharp
copy ..\examples\csharp\Form1.resx   csharp

copy ..\examples\csharp\Properties\*.cs                csharp\Properties
copy ..\examples\csharp\Properties\Resources.resx      csharp\Properties
copy ..\examples\csharp\Properties\Settings.settings   csharp\Properties

copy ..\examples\csharp\bin\Release\CoinMPTestCS8.exe  csharp\bin\Release
copy ..\examples\csharp\bin\Release\CoinMPTestCS9.exe  csharp\bin\Release

cd ..


