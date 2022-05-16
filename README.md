# Hexer
CLI Hex Editor for Linux in C++

## Example

IMAGE GOES HERE

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

`r` - \[R]eplace the selected value at the cursor's position with a new value.
`R` - \[R]eplace multiple values starting at the cursor's position.
`i` - \[I]nsert a new value before the cursor's position.
`I` - \[I]nsert multiple values before the cursor's position.
`a` - \[A]ppend a value after the cursor's position.
`A` - \[A]ppend multiple values after the corsor's position.
`x` - \[D]elete the selected value.
`X` - \[D]elete multiple values starting at the cursor's position.
`g` - \[G]o to the byte at the specified offset (offset is in hexadecimal).
`w` - \[W]rite all changes to a specified file.
