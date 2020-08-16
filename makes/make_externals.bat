@echo off
echo .
echo ===========================================================
echo Make externals
echo ===========================================================

set DEST_FOLDER=%~dp0\..\extern

svn propset svn:externals -F externals.list %DEST_FOLDER%
svn update %DEST_FOLDER%

echo .
echo .
echo Making externals all done
echo ===========================================================