# MicroShell

MicroShell is a simple shell program that provides basic file and directory manipulation functionalities. It allows users to navigate directories, move files, and remove files and directories.

## Features

- **cd Command:** Change the current working directory.
- **mv Command:** Move files or directories to a specified location.
- **rm Command:** Remove files or directories. Supports the `-r` flag for recursive removal.

## Usage

- Use the `cd` command to change directories.
- Use the `mv` command to move files or directories.
- Use the `rm` command to remove files or directories.

## Examples

```bash
$ ./microshell
[username:/path/to/directory]$ cd /new/directory
[username:/new/directory]$ mv file.txt /backup/
[username:/new/directory]$ rm -r old_directory
