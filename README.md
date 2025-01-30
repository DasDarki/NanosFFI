# NanosFFI
The NanosFFI Project adds an FFI (foreign function interface) to nanos world. Currently still in development and only for the server available.

## Folder Structure
- **thirdparty/**: Contains important third party libraries used in the project
- **server/**: Contains the server side of the NanosFFI project. (currently the only one)
    - **static/**: Contains a static library containing the real magic. The API and the loader for other plugins
    - **win/**: Contains the loader for the static library through version.dll proxying. This is only for windows!