A small utility library to hide all the horrors of JNI API.

Changelog:

===> Version 1.1.0:
* Java array support (arrays like 'int[]')
* Static native methods registration
* LocalReferenceFrame - class to automatically free local references
* Java string utility function (transformations from and to std::string)
* Removed 'this' pointer from 'JavaObjectWrapper' class constructor
* Reworked custom java classes internal representation
* Reworked type deduction system
* Reworked file structure
* Renamed to 'JNIHelper'

===> Version 1.0.1:
* JavaObjectWrapper class

===> Version 1.0.0:
* Basic functionality to call java methods from C++
