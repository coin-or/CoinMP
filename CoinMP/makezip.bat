rd /s /q CoinMP_dist
mkdir CoinMP_dist
cd CoinMP_dist

mkdir bin
mkdir bin64
mkdir include
mkdir src
mkdir test
mkdir examples

cd examples
mkdir vb6
mkdir csharp
cd csharp
mkdir Properties
mkdir bin
cd bin
mkdir x86
cd x86
mkdir Release
cd ..
mkdir x64
cd x64
mkdir Release
cd ..\..\..\..

copy ..\MSVisualStudio\v9\release\coinmp.dll   bin
copy ..\MSVisualStudio\v9\release\coinmp.exp   bin
copy ..\MSVisualStudio\v9\release\coinmp.lib   bin
copy ..\MSVisualStudio\v9\release\unittest.exe bin
copy ..\examples\vb6\CoinMPTestVB6.exe         bin
copy ..\examples\csharp\bin\x86\Release\CoinMPTestCS9.exe bin

copy ..\MSVisualStudio\v9\x64\release\coinmp.dll   bin64
copy ..\MSVisualStudio\v9\x64\release\coinmp.exp   bin64
copy ..\MSVisualStudio\v9\x64\release\coinmp.lib   bin64
copy ..\MSVisualStudio\v9\x64\release\unittest.exe bin64
copy ..\examples\csharp\bin\x64\Release\CoinMPTestCS9.exe bin64

copy ..\src\CoinMP.h  include

copy ..\src\*.cpp  src
copy ..\src\*.c    src
copy ..\src\*.h    src
copy ..\src\*.def  src

copy ..\test\unittest.cpp   test

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

copy ..\examples\csharp\bin\x86\Release\CoinMPTestCS9.exe  examples\csharp\bin\x86\Release
copy ..\examples\csharp\bin\x64\Release\CoinMPTestCS9.exe  examples\csharp\bin\x64\Release

cd ..


