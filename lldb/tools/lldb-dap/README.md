# LLDB DAP

## Procuring the `lldb-dap` binary

The extension requires the `lldb-dap` (formerly `lldb-vscode`) binary.
This binary is not packaged with the VS Code extension.

There are multiple ways to obtain this binary:

- Use the binary provided by your toolchain (for example `xcrun -f lldb-dap` on macOS) or contact your toolchain vendor to include it.
- Download one of the release packages from the [LLVM release page](https://github.com/llvm/llvm-project/releases/). The `LLVM-19.1.0-{operating_system}.tar.xz` packages contain a prebuilt `lldb-dap` binary.
- Build it from source (see [LLDB's build instructions](https://lldb.llvm.org/resources/build.html)).

By default, the VS Code extension will expect to find `lldb-dap` in your `PATH`.
Alternatively, you can explicitly specify the location of the `lldb-dap` binary using the `lldb-dap.executable-path` setting.

### Usage with other IDEs

The `lldb-dap` binary is a command line tool that implements the [Debug Adapter Protocol](https://microsoft.github.io/debug-adapter-protocol/).
It is used to power the VS Code extension but can also be used with other IDEs and editors that support DAP.
The protocol is easy to run remotely and also can allow other tools and IDEs to get a full featured debugger with a well defined protocol.

## Launching & Attaching to a debugee

Launching or attaching a debugee require you to create a [launch configuration](https://code.visualstudio.com/Docs/editor/debugging#_launch-configurations).
This file defines arguments that get passed to `lldb-dap` and the configuration settings control how the launch or attach happens.

### Launching a debugee

This will launch `/tmp/a.out` with arguments `one`, `two`, and `three` and
adds `FOO=1` and `bar` to the environment:

```javascript
{
  "type": "lldb-dap",
  "request": "launch",
  "name": "Debug",
  "program": "/tmp/a.out",
  "args": [ "one", "two", "three" ],
  "env": {
    "FOO": "1"
    "BAR": ""
  }
}
```

### Attaching to a process

When attaching to a process using LLDB, you can attach in multiple ways:

1. Attach to an existing process using the process ID
2. Attach to an existing process by name
3. Attach by name by waiting for the next instance of a process to launch

#### Attach using PID

This will attach to a process `a.out` whose process ID is 123:

```javascript
{
  "type": "lldb-dap",
  "request": "attach",
  "name": "Attach to PID",
  "program": "/tmp/a.out",
  "pid": 123
}
```

#### Attach by Name

This will attach to an existing process whose base
name matches `a.out`. All we have to do is leave the `pid` value out of the
above configuration:

```javascript
{
  "name": "Attach to Name",
  "type": "lldb-dap",
  "request": "attach",
  "program": "/tmp/a.out",
}
```

If you want to ignore any existing a.out processes and wait for the next instance
to be launched you can add the "waitFor" key value pair:

```javascript
{
  "name": "Attach to Name (wait)",
  "type": "lldb-dap",
  "request": "attach",
  "program": "/tmp/a.out",
  "waitFor": true
}
```

This will work as long as the architecture, vendor and OS supports waiting
for processes. Currently MacOS is the only platform that supports this.

### Loading a Core File

This loads the coredump file `/cores/123.core` associated with the program
`/tmp/a.out`:

```javascript
{
  "name": "Load coredump",
  "type": "lldb-dap",
  "request": "attach",
  "coreFile": "/cores/123.core",
  "program": "/tmp/a.out"
}
```

### Connect to a Debug Server on the Current Machine

This connects to a debug server (e.g. `lldb-server`, `gdbserver`) on
the current machine, that is debugging the program `/tmp/a.out` and listening
locally on port `2345`.

```javascript
{
  "name": "Local Debug Server",
  "type": "lldb-dap",
  "request": "attach",
  "program": "/tmp/a.out",
  "attachCommands": ["gdb-remote 2345"],
}
```

You can also use the `gdb-remote-port` parameter to send an attach request
to a debug server running on the current machine,
instead of using the custom command `attachCommands`.

```javascript
{
  "name": "Local Debug Server",
  "type": "lldb-dap",
  "request": "attach",
  "program": "/tmp/a.out",
  "gdb-remote-port": 2345,
}
```

### Connect to a Debug Server on Another Machine

This connects to a debug server running on another machine with hostname
`hostname`. Which is debugging the program `/tmp/a.out` and listening on
port `5678` of that other machine.

```javascript
{
  "name": "Remote Debug Server",
  "type": "lldb-dap",
  "request": "attach",
  "program": "/tmp/a.out",
  "attachCommands": ["gdb-remote hostname:5678"],
}
```

You can also use the `gdb-remote-hostname` and `gdb-remote-port` parameters
to send an attach request to a debug server running on a different machine,
instead of custom command `attachCommands`.
The default hostname being used `localhost`.

```javascript
{
  "name": "Local Debug Server",
  "type": "lldb-dap",
  "request": "attach",
  "program": "/tmp/a.out",
  "gdb-remote-port": 5678,
  "gdb-remote-hostname": "hostname",
}
```

### Launching via `vscode://` URIs

Debugging sessions can also be starting using special URIs.

The `vscode://llvm-vs-code-extensions.lldb-dap/start?config={launch-config}`
URI accepts a [URL-encoded](https://en.wikipedia.org/wiki/Percent-encoding)
JSON launch config. The most frequently used arguments (`request`, `program`,
`args`, `cwd`, `pid`, ...) can also specified directly, e.g.
`vscode://llvm-vs-code-extensions.lldb-dap/start?request=attach&pid=1234`, or
`vscode://llvm-vs-code-extensions.lldb-dap/start?program=ls&args=-a&args=/etc`.

This is useful for integration with custom scripts to start debugging
sessions. The URI might be printed to the terminal, potentially using
[OSC-8 hyperlinks](https://gist.github.com/egmontkob/eb114294efbcd5adb1944c9f3cb5feda),
or passed to `code --open-url` or `xdg-open`, although mileage may vary depending
on your specific debugging setup. E.g., `code --open-url` will not work when using a
SSH remote session. Furthermore, placeholders such as `${workspaceFolder}` are not
supported within launch URLs.

### Configuration Settings Reference

For both launch and attach configurations, lldb-dap accepts the following `lldb-dap`
specific key/value pairs:

| Parameter                         | Type        | Req |         |
|-----------------------------------|-------------|:---:|---------|
| **name**                          | string      | Y   | A configuration name that will be displayed in the IDE.
| **type**                          | string      | Y   | Must be "lldb-dap".
| **request**                       | string      | Y   | Must be "launch" or "attach".
| **program**                       | string      | Y   | Path to the executable to launch.
| **sourcePath**                    | string      |     | Specify a source path to remap \"./\" to allow full paths to be used when setting breakpoints in binaries that have relative source paths.
| **sourceMap**                     | [string[2]] |     | Specify an array of path re-mappings. Each element in the array must be a two element array containing a source and destination pathname. Overrides sourcePath.
| **debuggerRoot**                  | string      |     | Specify a working directory to use when launching lldb-dap. If the debug information in your executable contains relative paths, this option can be used so that `lldb-dap` can find source files and object files that have relative paths.
| **commandEscapePrefix**           | string      |     | The escape prefix to use for executing regular LLDB commands in the Debug Console, instead of printing variables. Defaults to a backtick. If it's an empty string, then all expression in the Debug Console are treated as regular LLDB commands.
| **customFrameFormat**             | string      |     | If non-empty, stack frames will have descriptions generated based on the provided format. See https://lldb.llvm.org/use/formatting.html for an explanation on format strings for frames. If the format string contains errors, an error message will be displayed on the Debug Console and the default frame names will be used. This might come with a performance cost because debug information might need to be processed to generate the description.
| **customThreadFormat**            | string      |     | Same as `customFrameFormat`, but for threads instead of stack frames.
| **displayExtendedBacktrace**      | bool        |     | Enable language specific extended backtraces.
| **enableAutoVariableSummaries**   | bool        |     | Enable auto generated summaries for variables when no summaries exist for a given type. This feature can cause performance delays in large projects when viewing variables.
| **enableSyntheticChildDebugging** | bool        |     | If a variable is displayed using a synthetic children, also display the actual contents of the variable at the end under a [raw] entry. This is useful when creating synthetic child plug-ins as it lets you see the actual contents of the variable.
| **initCommands**                  | [string]    |     | LLDB commands executed upon debugger startup prior to creating the LLDB target.
| **preRunCommands**                | [string]    |     | LLDB commands executed just before launching/attaching, after the LLDB target has been created.
| **stopCommands**                  | [string]    |     | LLDB commands executed just after each stop.
| **exitCommands**                  | [string]    |     | LLDB commands executed when the program exits.
| **terminateCommands**             | [string]    |     | LLDB commands executed when the debugging session ends.

All commands and command outputs will be sent to the debugger console when they are executed.
Commands can be prefixed with `?` or `!` to modify their behavior:

- Commands prefixed with `?` are quiet on success, i.e. nothing is written to stdout if the command succeeds.
- Prefixing a command with `!` enables error checking: If a command prefixed with `!` fails, subsequent commands will not be run. This is useful if one of the commands depends on another, as it will stop the chain of commands.

For JSON configurations of `"type": "launch"`, the JSON configuration can additionally
contain the following key/value pairs:

| Parameter                         | Type        | Req |         |
|-----------------------------------|-------------|:---:|---------|
| **program**                       | string      | Y   | Path to the executable to launch.
| **args**                          | [string]    |     | An array of command line argument strings to be passed to the program being launched.
| **cwd**                           | string      |     | The program working directory.
| **env**                           | dictionary  |     | Environment variables to set when launching the program. The format of each environment variable string is "VAR=VALUE" for environment variables with values or just "VAR" for environment variables with no values.
| **stopOnEntry**                   | boolean     |     | Whether to stop program immediately after launching.
| **runInTerminal** (deprecated)    | boolean     |     | Launch the program inside an integrated terminal in the IDE. Useful for debugging interactive command line programs.
| **console**                       | string      |     | Specify where to launch the program: internal console (`internalConsole`), integrated terminal (`integratedTerminal`) or external terminal (`externalTerminal`). Supported from lldb-dap 21.0 version.
| **launchCommands**                | [string]    |     | LLDB commands executed to launch the program.

For JSON configurations of `"type": "attach"`, the JSON configuration can contain
the following `lldb-dap` specific key/value pairs:

| Parameter                         | Type        | Req |         |
|-----------------------------------|-------------|:---:|---------|
| **program**                       | string      |     | Path to the executable to attach to. This value is optional but can help to resolve breakpoints prior the attaching to the program.
| **pid**                           | number      |     | The process id of the process you wish to attach to. If **pid** is omitted, the debugger will attempt to attach to the program by finding a process whose file name matches the file name from **program**. Setting this value to `${command:pickMyProcess}` will allow interactive process selection in the IDE.
| **waitFor**                       | boolean     |     | Wait for the process to launch.
| **attachCommands**                | [string]    |     | LLDB commands that will be executed after **preRunCommands** which take place of the code that normally does the attach. The commands can create a new target and attach or launch it however desired. This allows custom launch and attach configurations. Core files can use `target create --core /path/to/core` to attach to core files.

### Configuring `lldb-dap` defaults

User settings can set the default value for the following supported
`launch.json` configuration keys:

| Parameter                         | Type     | Default |
| --------------------------------- | -------- | :-----: |
| **commandEscapePrefix**           | string   | ``"`"`` |
| **customFrameFormat**             | string   |  `""`   |
| **customThreadFormat**            | string   |  `""`   |
| **detachOnError**                 | boolean  | `false` |
| **disableASLR**                   | boolean  | `true`  |
| **disableSTDIO**                  | boolean  | `false` |
| **displayExtendedBacktrace**      | boolean  | `false` |
| **enableAutoVariableSummaries**   | boolean  | `false` |
| **enableSyntheticChildDebugging** | boolean  | `false` |
| **timeout**                       | number   |  `30`   |
| **targetTriple**                  | string   |  `""`   |
| **platformName**                  | string   |  `""`   |
| **initCommands**                  | [string] |  `[]`   |
| **preRunCommands**                | [string] |  `[]`   |
| **postRunCommands**               | [string] |  `[]`   |
| **stopCommands**                  | [string] |  `[]`   |
| **exitCommands**                  | [string] |  `[]`   |
| **terminateCommands**             | [string] |  `[]`   |

To adjust your settings, open the Settings editor via the 
`File > Preferences > Settings` menu or press `Ctrl+`, on Windows/Linux and
`Cmd+`, on Mac.

## Debug Console

The Debug Console allows printing variables / expressions and executing lldb commands.
By default, lldb-dap tries to auto-detect whether a provided command is a variable
name / expression whose values will be printed to the Debug Console or a LLDB command.
To side-step this auto-detection and execute a LLDB command, prefix it with the
`commandEscapePrefix`.

The auto-detection can be disabled using the `lldb-dap repl-mode` command.
The escape character can be adjusted via the `commandEscapePrefix` configuration option.

### lldb-dap specific commands

The `lldb-dap` tool includes additional custom commands to support the Debug
Adapter Protocol features.

#### `lldb-dap start-debugging`

Using the command `lldb-dap start-debugging` it is possible to trigger a
reverse request to the client requesting a child debug session with the
specified configuration. For example, this can be used to attached to forked or
spawned processes. For more information see
[Reverse Requests StartDebugging](https://microsoft.github.io/debug-adapter-protocol/specification#Reverse_Requests_StartDebugging).

The custom command has the following format:

```
lldb-dap start-debugging <launch|attach> <configuration>
```

This will launch a server and then request a child debug session for a client.

```javascript
{
  "program": "server",
  "postRunCommand": [
    "lldb-dap start-debugging launch '{\"program\":\"client\"}'"
  ]
}
```

#### `lldb-dap repl-mode`

Inspect or adjust the behavior of lldb-dap repl evaluation requests. The
supported modes are `variable`, `command` and `auto`.

- `variable` - Variable mode expressions are evaluated in the context of the
  current frame.
- `command` - Command mode expressions are evaluated as lldb commands, as a
  result, values printed by lldb are always stringified representations of the
  expression output.
- `auto` - Auto mode will attempt to infer if the expression represents an lldb
  command or a variable expression. A heuristic is used to infer if the input
  represents a variable or a command.

In all three modes, you can use the `commandEscapePrefix` to ensure an expression
is evaluated as a command.

The initial repl-mode can be configured with the cli flag `--repl-mode=<mode>`
and may also be adjusted at runtime using the lldb command
`lldb-dap repl-mode <mode>`.

#### `lldb-dap send-event`

lldb-dap includes a command to trigger a Debug Adapter Protocol event
from a script.

The event maybe a custom DAP event or a standard event, if the event is not
handled internally by `lldb-dap`.

This command has the format:

```
lldb-dap send-event <name> <body>?
```

For example you can use a launch configuration hook to trigger custom events like:

```json
{
  "program": "exe",
  "stopCommands": [
    "lldb-dap send-event MyStopEvent",
    "lldb-dap send-event MyStopEvent '{\"key\": 321}"
  ]
}
```

[See the specification](https://microsoft.github.io/debug-adapter-protocol/specification#Base_Protocol_Event)
for more details on Debug Adapter Protocol events and the VS Code
[debug.onDidReceiveDebugSessionCustomEvent](https://code.visualstudio.com/api/references/vscode-api#debug.onDidReceiveDebugSessionCustomEvent)
API for handling a custom event from an extension.

## Contributing

`lldb-dap` and `lldb` are developed under the umbrella of the [LLVM project](https://llvm.org/).
The source code is part of the [LLVM repository](https://github.com/llvm/llvm-project/tree/main/lldb/tools/lldb-dap) on Github.
We use Github's [issue tracker](https://github.com/llvm/llvm-project/issues?q=label%3Alldb-dap) and patches can be submitted via [pull requests](https://github.com/llvm/llvm-project/pulls?q=label%3Alldb-dap).
Furthermore, there is a [LLDB category](https://discourse.llvm.org/c/subprojects/lldb/8) on the LLVM discourse forum.

For instructions on how to get started with development on lldb-dap, see the "[Contributing to lldb-dap](https://lldb.llvm.org/resources/lldbdap.html)" guide.
