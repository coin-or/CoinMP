rd /s /q CoinMP_zip
mkdir CoinMP_zip
cd CoinMP_zip

mkdir x64
mkdir src
mkdir examples
cd examples
mkdir vb6
mkdir csharp
cd csharp
mkdir Properties
mkdir bin
cd bin
mkdir Release
cd ..\..\..

copy ..\MSVisualStudio\v9\release\coinmp.dll
copy ..\MSVisualStudio\v9\release\coinmp.exp
copy ..\MSVisualStudio\v9\release\coinmp.lib
copy ..\MSVisualStudio\v9\release\unittest.exe

copy ..\src\CoinMP.h

copy ..\examples\vb6\CoinMPTestVB6.exe

copy ..\examples\csharp\bin\Release\CoinMPTestCS9.exe

copy ..\MSVisualStudio\v9\x64\release\coinmp.dll   x64
copy ..\MSVisualStudio\v9\x64\release\coinmp.exp   x64
copy ..\MSVisualStudio\v9\x64\release\coinmp.lib   x64
copy ..\MSVisualStudio\v9\x64\release\unittest.exe x64

copy ..\src\*.cpp      src
copy ..\src\*.c        src
copy ..\src\*.h        src
copy ..\src\coinmp.def src

copy ..\test\unittest.cpp   examples
copy ..\examples\example.c  examples

copy ..\examples\vb6\*.bas              examples\vb6
copy ..\examples\vb6\CoinMPTestVB6.vbp  examples\vb6
copy ..\examples\vb6\CoinMPTestVB6.exe  examples\vb6
copy ..\examples\vb6\FrmCoinMP.frm      examples\vb6


copy ..\examples\csharp\*.cs         examples\csharp
copy ..\examples\csharp\*.csproj     examples\csharp
copy ..\examples\csharp\*.sln        examples\csharp
copy ..\examples\csharp\Form1.resx   examples\csharp

copy ..\examples\csharp\Properties\*.cs                examples\csharp\Properties
copy ..\examples\csharp\Properties\Resources.resx      examples\csharp\Properties
copy ..\examples\csharp\Properties\Settings.settings   examples\csharp\Properties

copy ..\examples\csharp\bin\Release\CoinMPTestCS9.exe  examples\csharp\bin\Release

cd ..


