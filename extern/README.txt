Please add external to this folder (trunk/libs).
This steps is temporary because we can't commit this folder with externals properties.
This should be fixed on the next time (maybe ?).

External URLs:
	1. 	- path: Singleton
		- url: https://github.com/irwanto95/Singleton.git/trunk

How to do (deprecated):
	1. right-click on trunk/libs
	2. TortoiseSVN > Properties
	3. New > Externals
	4. New
		- Local path: [path]
		- URL: [URL]
	5. Ok
	6. Repeat 1-5 if there is more than 1 externals to add
	7. Ok > Ok
	8. SVN Update on trunk/libs
	
	
How to do:
	- double-click on trunk/makes/make_externals.bat for single command, or
	- double-click on trunk/make_all.bat for execute all makes command