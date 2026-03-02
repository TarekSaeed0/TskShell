<h1 align="center">TskShell</h1>

<p align="center"> A simple Unix shell written in C </p>

<div align="center">

<a href="https://github.com/TarekSaeed0/TskShell/blob/main/LICENSE">
 <picture>
    <source media="(prefers-color-scheme: dark)"  srcset="https://img.shields.io/github/license/TarekSaeed0/TskShell?style=for-the-badge&labelColor=%23151b23&color=%234493f8">
    <source media="(prefers-color-scheme: light)" srcset="https://img.shields.io/github/license/TarekSaeed0/TskShell?style=for-the-badge&labelColor=%23f6f8fa&color=%230969da">
    <img alt="GitHub License" src="https://img.shields.io/github/license/TarekSaeed0/TskShell?style=for-the-badge&labelColor=%23151b23&color=%234493f8">
 </picture>
</a>
<a href="https://github.com/TarekSaeed0/TskShell/pulse">
  <picture>
    <source media="(prefers-color-scheme: dark)"  srcset="https://img.shields.io/github/last-commit/TarekSaeed0/TskShell?style=for-the-badge&labelColor=%23151b23&color=%234493f8">
    <source media="(prefers-color-scheme: light)" srcset="https://img.shields.io/github/last-commit/TarekSaeed0/TskShell?style=for-the-badge&labelColor=%23f6f8fa&color=%230969da">
    <img alt="GitHub Last Commit" src="https://img.shields.io/github/last-commit/TarekSaeed0/TskShell?style=for-the-badge&labelColor=%23151b23&color=%234493f8">
  </picture>
</a>
<a href="https://github.com/TarekSaeed0/TskShell/stargazers">
  <picture>
    <source media="(prefers-color-scheme: dark)"  srcset="https://img.shields.io/github/stars/TarekSaeed0/TskShell?style=for-the-badge&labelColor=%23151b23&color=%234493f8">
    <source media="(prefers-color-scheme: light)" srcset="https://img.shields.io/github/stars/TarekSaeed0/TskShell?style=for-the-badge&labelColor=%23f6f8fa&color=%230969da">
    <img alt="GitHub Repo stars" src="https://img.shields.io/github/stars/TarekSaeed0/TskShell?style=for-the-badge&labelColor=%23151b23&color=%234493f8">
  </picture>
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
