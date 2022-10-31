# Setting up and using dotnet script

The Half-Life Unified SDK uses dotnet script to run scripts written in C#.

This page describes how to install this tool and use it.

## Install the .NET SDK

Before you can install dotnet script itself you will need to install the .NET SDK. .NET 6.0 or newer is needed to run the tool and the scripts and packages used by existing scripts.

See [Setting up the .NET SDK](/docs/tutorials/setting-up-dotnet-sdk.md)

## Install dotnet script

Next, install dotnet script as a global tool. This process is described here: https://github.com/filipw/dotnet-script#net-core-global-tool

You will need to enter the commands in a command prompt or terminal.

> **Note:**
> Since Visual Studio 2022 is the first 64 bit version of Visual Studio the `PATH` environment variable is modified to point to the 64 bit version of .NET. If you encounter problems adding dotnet script as a tool, check to see if the variable contains the paths to both the 32 bit (`C:\Program Files (x86)\dotnet\`) and 64 bit (`C:\Program Files\dotnet\`) versions of .NET. If so, either move the 32 bit version to behind the 64 bit version or remove it.
> See this article for more information: https://developercommunity.visualstudio.com/t/installing-vs2022-breaks-dotnet-tool/1575432

## Using dotnet script

To run a script using dotnet script, open a command prompt or terminal and enter this command:

```bat
dotnet script <script filename>
```

Note that you must open the command prompt after installing the SDK. The `dotnet` executable's path is added to the `PATH` environment variable on installation, which is not updated for command prompts that are already open.

Scripts may take some time to restore packages and to be compiled before being executed. After this process has been completed once the script should run much faster on subsequent executions provided the contents have not changed.

If you are running the dotnet command for the first time after installing or updating the SDK you may get a welcome message. If you wait a bit the command you entered will execute.

### Passing command line parameters

To pass command line parameters simply append them to the command:
```bat
dotnet script <script filename> --my-parameter my-value
```

### Using scripts with Visual Studio Code

Visual Studio Code can be used to edit, run and debug C# scripts.

First, install the C# extension: https://marketplace.visualstudio.com/items?itemName=ms-dotnettools.csharp

Second, configure the extension to use the latest version of .NET to ensure the extension understands the syntax used in scripts.
You can find out more about this setting here: https://github.com/OmniSharp/omnisharp-vscode/tree/84f1d5ab16718c2e0ad91e732770515500a8113e#using-net-6-builds-of-omnisharp

When editing scripts through Visual Studio Code it is advised to use a `launch.json` file. dotnet script can create one for you using  the `init` command. You can find out more about this command here: https://github.com/filipw/dotnet-script/tree/09d3ff903f717ed7665d498496e96c3b47f55f49#scaffolding

Note that on Windows this currently creates a `launch.json` file that references the absolute path to the dotnet script tool:
```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": ".NET Script Debug",
            "type": "coreclr",
            "request": "launch",
            "program": "dotnet",
            "args": [
                "exec",
                "path/to/dotnet-script.dll",
                "${file}"
            ],
            "cwd": "${workspaceRoot}",
            "stopAtEntry": false
        }
    ]
}
```

Modify the contents of the file to use the global location based on environment variables instead. You can use the contents of this template file as a starting point: https://github.com/filipw/dotnet-script/blob/master/src/Dotnet.Script.Core/Templates/globaltool.launch.json.template

For default generated scripts this should suffice. For scripts that require command line parameters you will need to modify the contents of the `args` array.
