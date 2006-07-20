
CoinMP.dll is a Windows dynamic linked library (DLL) that supports most 
of the functionality of COIN-OR LP, COIN-OR Branch-and-Cut and Cut Generation 
Library projects.  This coinMP.dll distribution of the COIN-OR projects for 
Windows has the advantages of being: high portability, does not require any 
compilation, easy to use, and can be used from any other Windows application.  

To build CoinMP see INSTALL file.

To use CoinMP in your own application, link in the CoinMP.lib file in the Release 
folder and copy the DLL file to the same directory as your application.  The header 
file CoinMP.h contains the prototypes for all the functions in the DLL.

The interface to this library designed to be as simple as possible, but still provide
similar functionality as other solver DLL's availble.

The CoinMP.dll file will work as is with the MPL for Windows Modeling System by
Maximal Software.  Simply copy the DLL file into the Mplwin4 installation directory
and restart MPL.

Bjarni Kristjansson
Maximal Software

