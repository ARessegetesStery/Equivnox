# Equivnox

Renderer playground under construction.

Currently only offline rendering of very simple objects and of very limited format of models are supported\.

### Build Equivnox

##### Windows - Visual Studio

Equivnox requires Visual Studio 2019. Before building the project please ensure that VS2019 has been properly installed. To obtain the `.sln` files, do the following:

- Download [Premake](https://premake.github.io/download) and move the binary to `vendor/bin/premake`.

- Run `GenerateProject.bat` to get the solution file ready.

Or, after downloading you could alternatively run

```bash
premake5.exe vs2019
# replace 2019 with the edition of your Visual Studio if not applicable
```

##### Other OS or IDEs

Please refer to [Premake Building Options](https://premake.github.io/docs/Building-Premake) for custom building options.
