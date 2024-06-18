# Sample OS
This is my simple OS project (SAMPLE_OS), a simple operating system written in C and assembly.
This project includes a minimal C standard library and a custom sysroot to manage dependencies.
Here (in this `README.md`), you are provided with setup instructions, build processes, and an
overview of the project's structure.

## Project Structure
```bash
sample_os/
│
├── sysroot/
│ ├── usr/
│ │ ├── include/
│ │ │ ├── kernel/
│ │ │ │ ├── stdlib.h
│ │ │ │ 
│ │ │ ├── stdlib.h
│ │ │ ├── string.h
│ │ │ ├── io.h
│ │ │ 
│ │ └── lib/
│ │ │ └── libc.a
│
├── kernel/
│ ├── kernel.c
│ └── Makefile
│
├── libc/
│ ├── include/
│ │ ├── stdlib.h
│ │ ├── string.h
│ │ ├── stdio.h
│ │
│ ├── src/
│ │ ├── stdio/
│ │ ├── stdlib/
│ │ ├── string/
│ │
│ └── Makefile
│
└── linker.ld
```


## Requirements
- Ubuntu 22.04 (tested) or some other linux distro
- `i686-elf-gcc` (cross-compiler) - i386 or other ix86 may work (not tested)
- `i686-elf-gdb` (cross-debugger) - i386 or other ix86 may work (not tested)
- `qemu-system-i386` (emulator)? - Since an ISO is created, we can burn it to a disc to run physically
- Make
- Visual Studio Code (VS Code) (tested) or any other IDE
- C/C++ extension for VS Code (tested) or any other IDE and their tools

## Setup

1. **Install the cross-compiler and tools**:
    - Follow the instructions to install `i686-elf-gcc` and `i686-elf-gdb` [here](https://wiki.osdev.org/GCC_Cross-Compiler).
    - Install QEMU: `sudo apt-get install qemu-system-i386`.

2. **Clone the repository**:
    ```bash
    git clone https://github.com/Powerisinschool/sample_os.git
    cd myos
    ```

3. **Configure VS Code**:
    - Install the "C/C++" extension by Microsoft.
    - The `.vscode` folder has aleady been configured for you, but here are the steps:
    - Create `.vscode` directory and add configuration files as described below.

## VS Code Configuration

### `c_cpp_properties.json`

```json
{
    "configurations": [
        {
            "name": "Sample OS",
            "includePath": [
                "${workspaceFolder}/sysroot/usr/include",
                "${workspaceFolder}/libc/include",
                "${workspaceFolder}/kernel/include",
                "${workspaceFolder}/kernel"
            ],
            "defines": [],
            "compilerPath": "~/opt/cross/bin/i686-elf-gcc",
            "cStandard": "gnu99",
            "cppStandard": "gnu++17",
            "intelliSenseMode": "gcc-x86",
            "compilerArgs": [
                "-ffreestanding",
                "-nostdlib",
                "-Wall",
                "-Wextra"
            ],
            "browse": {
                "path": [
                    "${workspaceFolder}/sysroot/usr/include",
                    "${workspaceFolder}/libc/include",
                    "${workspaceFolder}/kernel"
                ],
                "limitSymbolsToIncludedHeaders": true,
                "databaseFilename": ""
            }
        }
    ],
    "version": 4
}
```

### `tasks.json`
```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "Build libc",
            "type": "shell",
            "command": "make",
            "args": [
                "install"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "options": {
                "cwd": "${workspaceFolder}/libc"
            },
            "problemMatcher": []
        },
        {
            "label": "Build Kernel",
            "type": "shell",
            "command": "make",
            "args": [],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "dependsOn": "Build libc",
            "options": {
                "cwd": "${workspaceFolder}/kernel"
            },
            "problemMatcher": []
        },
        {
            "label": "Run QEMU",
            "type": "shell",
            "command": "qemu-system-i386",
            "args": [
                "-kernel",
                "${workspaceFolder}/kernel/kernel.bin"
            ],
            "group": {
                "kind": "test",
                "isDefault": true
            },
            "problemMatcher": []
        }
    ]
}
```
### `launch.json`
```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Debug Kernel",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/kernel/kernel.bin",
            "args": [],
            "stopAtEntry": true,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "miDebuggerPath": "/path/to/i686-elf-gdb",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ],
            "preLaunchTask": "Build Kernel",
            "miDebuggerArgs": "-iex 'set auto-load safe-path /'",
            "logging": {
                "engineLogging": true,
                "trace": true
            }
        }
    ]
}
```

## License
This project is licensed under the MIT License - see the `LICENSE` file for details.

## Acknowledgements
- The OSDev wiki and community for invaluable resources and guidance.
- The authors of the tools and libraries used in this project.

## Contributing
Contributions are welcome! Please feel free to submit a pull request or open an issue.
