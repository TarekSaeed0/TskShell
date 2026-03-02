# TskShell

<div align="center" style="margin-bottom: 1em;">
A simple Unix shell written in C
</div>

<div align="center">
    <a href="https://github.com/TarekSaeed0/TskShell/blob/main/LICENSE">
        <img alt="Github License" src="https://img.shields.io/github/license/TarekSaeed0/TskShell?style=for-the-badge&labelColor=%231e1e2e&color=%238bd5ca">
    </a>
    <a href="https://github.com/TarekSaeed0/TskShell/pulse">
        <img alt="Last Commit" src="https://img.shields.io/github/last-commit/TarekSaeed0/TskShell?style=for-the-badge&labelColor=%231e1e2e&color=%238bd5ca">
    </a>
    <a href="https://github.com/TarekSaeed0/TskShell/stargazers">
        <img alt="Repo stars" src="https://img.shields.io/github/stars/TarekSaeed0/TskShell?style=for-the-badge&labelColor=%231e1e2e&color=%238bd5ca">
    </a>
</div>

## Features

- Command execution with arguments.
- Support for built-in commands like `cd`, `echo`, `export`.
- Support for environment variable expansion.
- Background execution using `&`.

## Building

```sh
git clone https://github.com/TarekSaeed0/TskShell.git
cd TskShell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## License

MIT License. See [LICENSE](https://github.com/TarekSaeed0/TskShell/blob/main/LICENSE) for details.
