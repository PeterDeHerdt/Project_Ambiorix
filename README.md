Project_Ambiorix
================

Building
========
export AMX_ROOT_DIR=<path/to/local/repo>

cd <path/to/local/repo>/build
make


Testing
=======
For testing the check unit test framework is needed.
http://check.sourceforge.net/

Test ambiorix libraries:
export AMX_ROOT_DIR=<path/to/local/repo>

cd <path/to/local/repo>/build
make test

Generate documentation
======================
Install doxygen

export AMX_ROOT_DIR=<path/to/local/repo>

cd <path/to/local/repo>/build
make doc
