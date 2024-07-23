
# BitCraft

BitCraft is a simple programming language designed for performing basic mathematical operations and printing results. It supports variable assignments, arithmetic expressions, and custom print formatting.

## Features

- **Variable Assignment**: Assign values to variables.
- **Arithmetic Operations**: Perform addition, subtraction, multiplication, and division.
- **Printing**: Print variables and expressions with customizable formatting.
- **Comments**: Add comments to your source code for clarity.

## Syntax

### Variables and Assignments

Variables can be assigned using the `=` operator. For example:

```plaintext
width = 5;
height = 10;
```
### Arithmetic Expressions
Arithmetic expressions are supported using +, -, *, and / operators. For example:
```
area = width * height;
```
### Printing
Use the print statement to output text and variables. The print statement accepts multiple arguments, including strings and variables, separated by commas. For example:
```
print("The width is ", width, " and the height is ", height, " The area is ", area, ".");
```
### Comments
Single-line comments are supported and start with //. For example:
```
// This is a comment
width = 5;
```
### Parentheses
Parentheses () are used to group expressions. For example:
```
result = (width + height) * 2;
```
## Full example
Here’s a complete example of a source file using BitCraft:
```
// Define variables
width = 5.53;
height = 10.4;

// Calculate area
area = width * height;

// Print results
print("The width is ", width, " and the height is ", height);
print("The area is ", area);
```
## Usage
1 - Create a Source File: Write your BitCraft code in a text file with a .bc extension (e.g., program.bc).

2 - Compile and Run: Use the provided Makefile to compile and execute your interpreter. For example:
```
bitc program.bc
```
# License
This project is licensed under the [MIT](https://choosealicense.com/licenses/mit/) License. See the LICENSE file for more details.

# Contributing
Contributions are welcome! Please submit issues and pull requests via the GitHub repository.
