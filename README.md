# vey

A high-performance network tool and IP stresser.

## Usage

The application uses a sub-command structure.

### 1. Network Testing (`inet`)

Target a network host and configure traffic parameters.

```bash
vey.exe inet [ip] [port] [protocol] [options]
```

#### Positional Arguments
* `ip`   : Target IP address (IPv4 or IPv6).
* `port` : Target port number (1-65535).

#### Protocol Flags (Required — Mutually Exclusive)
* `-t, --tcp` : Use TCP protocol.
* `-u, --udp` : Use UDP protocol.

#### Optional Flags
* `-s, --socket-count`        : Number of concurrent sockets to use (default: `1`).
* `-p, --packet-size`          : Packet size in bytes (default: `1024`).
* `-rc, --reconnect-count`    : Maximum number of reconnection attempts (default: infinity reconnect).
* `-dw, --delay-ms-per-write` : Delay between consecutive write operations in milliseconds (default: no delay).
* `-v4, --ipv4`               : Force IPv4 addressing.
* `-v6, --ipv6`               : Force IPv6 addressing.
* `--verbose`                 : Enable detailed logging / verbose output.

#### Examples

**Basic UDP test with default values:**
```bash
vey.exe inet 192.168.0.1 80 --udp
```

**Advanced TCP test with reconnection and delays:**
```bash
vey.exe inet 192.168.0.1 152 --tcp -s 5 -p 2048 -rc 10 -dw 50
```

---

### 2. Process Injection (`injec`) — *Windows Only*

Perform process injection operations on a target process ID.

```bash
vey.exe injec --pid [pid] [injection_method] [options]
```

#### Required Arguments
* `-p, --pid` : Target process ID (PID).

#### Injection Method Flags (Required — Mutually Exclusive)
* `-wc, --wsa-cleanup` : Try by shellcode clean WSA.

#### Optional Flags
* `-dp, --enable-debug-privilege` : Try to enable debug privilege (`SeDebugPrivilege`).
* `--verbose`                     : Enable detailed logging / verbose output.

#### Examples

**Basic WSA cleanup injection:**
```bash
vey.exe injec --pid 1234 --wsa-cleanup
```

---

## Build Instructions

```bash
cd vey
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### Build Options (CMake)

You can customize the build by passing variables to CMake via `-D<OPTION>=ON|OFF`.

* `VEY_OPTIMIZE_ON_RELEASE` (Default: `OFF`)  
  Enables aggressive compiler optimization flags when building in Release mode.

* `VEY_STATIC_ON_RELEASE` (Default: `OFF`)  
  Links libraries statically, eliminating external dynamic library dependencies for the release binary.

**Example with all options enabled:**
```bash
cmake .. -DCMAKE_BUILD_TYPE=Release -DVEY_OPTIMIZE_ON_RELEASE=ON -DVEY_STATIC_ON_RELEASE=ON
```
