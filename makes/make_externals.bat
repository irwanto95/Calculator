@echo off
echo .
echo ===========================================================
echo Make externals
echo ===========================================================

set EXTERNAL_TAG = "externals.txt"
set DEST_FOLDER = %~dp0\..\extern
set CONTAINER = Singleton
set URL = https://github.com/irwanto95/Singleton.git/trunk

echo Dest		: %DEST_FOLDER%
echo Container	: %CONTAINER%
echo url		: %URL%

if  exist %DEST_FOLDER%\%EXTERNAL_TAG% (
	echo .
	echo Externals already sets
	goto :end
)

break > %DEST_FOLDER%\%EXTERNAL_TAG%

svn propset svn:externals "%CONTAINER% %URL%" %DEST_FOLDER%
svn update %DEST_FOLDER%

:end
echo .