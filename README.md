# IATHooking [![MIT license](https://img.shields.io/badge/License-MIT-blue.svg)](https://lbesson.mit-license.org/)

## What is this
This is an implementation of IAT Hooking which hooks functions through the [Import Address Table](https://docs.microsoft.com/en-us/windows/win32/debug/pe-format#import-directory-table). Currently this is targeted towards the windows Notepad application and currently hooks two functions [CreateFileW](https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew) and [ReadFile](https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-readfile) however is adaptable to hook any imported functions for most windows PE applications.

## Usage
The project uses [cmake](https://cmake.org/) as its build system, and will output a DLL file which can be injected into the target application. This can be done using any available DLL injector.
## Credits
 - [MSDN](https://docs.microsoft.com/)