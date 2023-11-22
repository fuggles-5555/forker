## Commit Message

### Summary

A tool to enable parallel command execution based on input file which contains a list of commands and thread limit.

### Details

- **Feature**: Parallel execution of commands
- **File**: `forker.c`
- **Compilation Command**: `gcc forker.c -o forker`
- **Parameters**:
  - First argument: File containing a list of commands
  - Second argument: Maximum number of threads

### Description

This tool introduces the capability to execute commands in parallel using a specified input file and maximum thread limit. Compilation is required before using the tool. The compilation command is as follows:

```bash
$ gcc forker.c -o forker

