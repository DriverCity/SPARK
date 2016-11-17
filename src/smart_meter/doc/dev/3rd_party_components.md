# 3rd party componens
This document describes what 3rd party components are needed and how to install them.

## SQLite3

1. Install sqlite3 to dev env: 

  ```
  sudo apt-get install sqlite3 libsqlite3-dev
  ```
2. Add source files for cross-compilation:
  - Get amalgamation source package: http://www.sqlite.org/
  - Unzip package.
  - Copy header files to 3rd_party/sqlite3/include (create directory). Copy source files to 3rd_party/sqlite3/src.
  - To view database contents in target system intall sqlite3 there also.
