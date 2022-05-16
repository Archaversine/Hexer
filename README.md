# Hexer
CLI Hex Editor for Linux in C++

## Example

![Hexer in Use](https://github.com/Archaversine/Hexer/img/sample-use.png)

## Installation

Clone the repository into a directory and run the makefile:
```
git clone https://github.com/Archaversine/Hexer.git && cd Hexer
make && sudo make install
```

## Usage

Syntax:
```
hexer <filename>
```

Example:
```
hexer file.txt
```

## Keybindings:

Use the arrow keys or HJKL to navigate the cursor.

`r` - \[R]eplace the selected value at the cursor's position with a new value.\n
`R` - \[R]eplace multiple values starting at the cursor's position.\n
`i` - \[I]nsert a new value before the cursor's position.\n
`I` - \[I]nsert multiple values before the cursor's position.\n
`a` - \[A]ppend a value after the cursor's position.\n
`A` - \[A]ppend multiple values after the corsor's position.\n
`x` - \[D]elete the selected value.\n
`X` - \[D]elete multiple values starting at the cursor's position.\n
`g` - \[G]o to the byte at the specified offset (offset is in hexadecimal).\n
`w` - \[W]rite all changes to a specified file.\n
