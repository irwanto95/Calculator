@echo off
echo .
echo ===========================================================
echo Make externals
echo ===========================================================

set DEST_FOLDER=%~dp0\..\extern

echo Making externals .. mufise
echo .
set CONTAINER=mufise
set TAG=%CONTAINER%.tag
set URL=https://github.com/irwanto95/mufise.git/trunk/mufise

if  not exist %DEST_FOLDER%\%TAG% (
	break > %DEST_FOLDER%\%TAG%

	svn propset svn:externals "%CONTAINER% %URL%" %DEST_FOLDER%
	svn update %DEST_FOLDER%
)


echo .
echo Making externals all done