# Installation
### Requirement
- C\C++ compiler
- CMake >= 3.25
- Conan2

### How to build
```sh
conan build conanfile.py
```

# Usage
Server and Client controlled by command line arguments (--help for more info) or by the config like this:
## Server
address(number): Modbus address for the current server instance.
port(number): The port number to listen on.
*config_server.json*
```json
{
    "address": 1,
    "port": 8080
}
```
## Client
address(string): The address of the server to connect to. This can be either an IP address or a hostname.
port(number): The port number to connect to on the server.
*config_client.json*
```json
{
    "address": "127.0.0.1",
    "port": 8080
}
```

