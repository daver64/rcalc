# RCalc - Mathematical Expression Calculator

A command-line mathematical expression calculator written in C that supports variables, user-defined functions, programming logic, and a comprehensive range of mathematical operations.

## Features

### Basic Arithmetic Operations
- Addition (`+`)
- Subtraction (`-`)
- Multiplication (`*`)
- Division (`/`)
- Exponentiation (`^`) - right associative

### Transcendental Functions
- **Trigonometric**: `sin`, `cos`, `tan`, `asin`, `acos`, `atan`
- **Hyperbolic**: `sinh`, `cosh`, `tanh`, `asinh`, `acosh`, `atanh`
- **Logarithmic**: `log` (natural log), `ln`, `log10`, `log2`
- **Exponential**: `exp`, `exp2`, `exp10`

### General Mathematical Functions
- `sqrt` - square root
- `cbrt` - cube root
- `abs` - absolute value
- `floor` - floor function
- `ceil` - ceiling function
- `round` - round to nearest integer
- `deg` - convert radians to degrees
- `rad` - convert degrees to radians

### Utility Functions
- `min(a, b)` - returns the smaller of two values
- `max(a, b)` - returns the larger of two values
- `clamp(value, min, max)` - constrains value between min and max
- `lerp(a, b, t)` - linear interpolation between a and b by factor t
- `hypot(x, y)` - hypotenuse calculation (sqrt(x² + y²))

### Programming Functions
- `if(condition, true_value, false_value)` - conditional expression

### Comparison Operators
- `<` - less than
- `>` - greater than
- `<=` - less than or equal to
- `>=` - greater than or equal to
- `==` - equal to (with floating-point tolerance)
- `!=` - not equal to

### Mathematical Constants
- `pi` - π ≈ 3.14159265358979323846
- `e` - Euler's number ≈ 2.71828182845904523536

### Advanced Features
- Parentheses for grouping expressions
- Proper operator precedence
- Unary plus and minus operators
- **Variables** - declare and use variables
- **User-defined functions** - create custom functions
- **Conditional logic** - if/then expressions and comparisons
- **Utility functions** - min, max, clamp, lerp, hypot
- Interactive REPL (Read-Eval-Print Loop)
- Multiline input support
- Built-in help system
- Comprehensive error handling

## Building

### Using GCC (Linux/macOS/MinGW)
```bash
gcc -o rcalc rcalc.c -lm
```

### Using Microsoft Visual C++ (Windows)
```cmd
cl rcalc.c /Fe:rcalc.exe
```

## Usage

Run the calculator to enter interactive mode:
```bash
./rcalc        # Linux/macOS
rcalc.exe      # Windows
```

Or load script files at startup:
```bash
./rcalc script.calc              # Load one script
./rcalc lib1.calc lib2.calc      # Load multiple scripts
```

### Interactive Mode Examples

```
RCalc - Mathematical Expression Calculator
Enter expressions or 'quit' to exit
Supported functions: sin, cos, tan, log, ln, sqrt, exp, etc.
Constants: pi, e

> 2 + 3 * 4
= 14

> (2 + 3) * 4
= 20

> var x = 10;
Variable 'x' = 10

> x * 2 + 5
= 25

> var square(var n) {
...   return n * n;
... }
Function 'square' defined

> square(5)
= 25

> load "example.calc"
Loaded 10 functions, 2 variables from example.calc

> circle_area(10)
= 314.1592654

> kinetic_energy(70, 25)
= 21875

> min(10, 20)
= 10

> max(pi, e)
= 3.141592654

> clamp(15, 0, 10)
= 10

> lerp(0, 100, 0.25)
= 25

> hypot(3, 4)
= 5

> if(5 > 3, 100, 200)
= 100

> var conditional_sqrt(var x) {
...   return if(x >= 0, sqrt(x), 0);
... }
Function 'conditional_sqrt' defined

> conditional_sqrt(16)
= 4

> conditional_sqrt(-4)
= 0

> sin(pi/2)
= 1

> log(e^2)
= 2

> sqrt(16) + 2^3
= 12

> cos(0) * pi
= 3.141592654

> 2^3^2
= 512

> deg(pi/4)
= 45

> rad(90)
= 1.570796327

> abs(-5.5)
= 5.5

> floor(3.7) + ceil(2.1)
= 6

> help
(shows help information)

> quit
Goodbye!
```

## Operator Precedence

The calculator follows standard mathematical operator precedence:

1. **Parentheses** `()` - highest precedence
2. **Functions** `sin()`, `log()`, etc.
3. **Exponentiation** `^` - right associative
4. **Multiplication and Division** `*`, `/` - left associative
5. **Addition and Subtraction** `+`, `-` - left associative
6. **Comparison** `<`, `>`, `<=`, `>=`, `==`, `!=` - lowest precedence

### Examples of Precedence

- `2 + 3 * 4` evaluates as `2 + (3 * 4) = 14`
- `2^3^2` evaluates as `2^(3^2) = 2^9 = 512` (right associative)
- `sin(pi/2) + cos(0)` evaluates as `1 + 1 = 2`
- `5 > 3 + 1` evaluates as `5 > (3 + 1) = 5 > 4 = 1` (true)

## Error Handling

The calculator provides helpful error messages for:
- Empty expressions
- Division by zero
- Unknown functions or identifiers
- Mismatched parentheses
- Invalid syntax

Example error messages:
```
> 5/0
Error: Division by zero

> sin(
Error: Expected ')' after function argument

> unknown_func(5)
Error: Unknown identifier 'unknown_func'
```

## Variables

Variables can be declared and used in expressions:

```
> var x = 10;
Variable 'x' = 10

> var y = x * 2 + 5;
Variable 'y' = 25

> x + y
= 35
```

Variables can be reassigned:
```
> x = 50;
Variable 'x' = 50

> x + y
= 75
```

## User-Defined Functions

You can define your own functions using C-like syntax:

```
> var square(var n) {
...   return n * n;
... }
Function 'square' defined

> square(5)
= 25

> var power(var base, var exp) {
...   return base ^ exp;
... }
Function 'power' defined

> power(2, 8)
= 256
```

Functions can call other functions and use mathematical operations:
```
> var circle_area(var radius) {
...   return pi * square(radius);
... }
Function 'circle_area' defined

> circle_area(5)
= 78.539816
```

Functions can use conditional logic:
```
> var safe_divide(var a, var b) {
...   return if(b != 0, a / b, 0);
... }
Function 'safe_divide' defined

> safe_divide(10, 2)
= 5

> safe_divide(10, 0)
= 0
```

Complex mathematical functions:
```
> var distance(var x1, var y1, var x2, var y2) {
...   return hypot(x2 - x1, y2 - y1);
... }
Function 'distance' defined

> distance(0, 0, 3, 4)
= 5

> var smooth_step(var edge0, var edge1, var x) {
...   var t = clamp((x - edge0) / (edge1 - edge0), 0, 1);
...   return t * t * (3 - 2 * t);
... }
Function 'smooth_step' defined

> smooth_step(0, 10, 5)
= 0.5
```

## Script Files

You can create reusable libraries of functions and variables in script files with `.calc` or `.rcalc` extensions.

### Script File Format

```rcalc
# This is a comment - lines starting with # are ignored

# Define variables
var gravity = 9.81;
var speed_of_light = 299792458;

# Define functions (same syntax as REPL)
var square(var n) {
  return n * n;
}

var circle_area(var radius) {
  return pi * square(radius);
}

var kinetic_energy(var mass, var velocity) {
  return 0.5 * mass * square(velocity);
}
```

### Loading Scripts

**From command line:**
```bash
./rcalc myscript.calc           # Loads script and starts REPL
./rcalc lib1.calc lib2.calc     # Load multiple scripts
```

**From REPL:**
```
> load "myscript.calc"
Loaded 3 functions, 2 variables from myscript.calc

> load geometry.calc            # Quotes optional for simple filenames
Loaded 5 functions from geometry.calc
```

### Example Scripts

The repository includes example scripts:
- `example.calc` - Math utilities, physics constants, temperature conversion
- `geometry.calc` - Geometry functions for rectangles, triangles, spheres

## Function Reference

### Trigonometric Functions
| Function | Description | Example |
|----------|-------------|---------|
| `sin(x)` | Sine of x (radians) | `sin(pi/2)` → `1` |
| `cos(x)` | Cosine of x (radians) | `cos(0)` → `1` |
| `tan(x)` | Tangent of x (radians) | `tan(pi/4)` → `1` |
| `asin(x)` | Arcsine of x | `asin(1)` → `1.570796` |
| `acos(x)` | Arccosine of x | `acos(0)` → `1.570796` |
| `atan(x)` | Arctangent of x | `atan(1)` → `0.785398` |

### Hyperbolic Functions
| Function | Description | Example |
|----------|-------------|---------|
| `sinh(x)` | Hyperbolic sine | `sinh(1)` → `1.175201` |
| `cosh(x)` | Hyperbolic cosine | `cosh(0)` → `1` |
| `tanh(x)` | Hyperbolic tangent | `tanh(1)` → `0.761594` |
| `asinh(x)` | Inverse hyperbolic sine | `asinh(1)` → `0.881374` |
| `acosh(x)` | Inverse hyperbolic cosine | `acosh(2)` → `1.316958` |
| `atanh(x)` | Inverse hyperbolic tangent | `atanh(0.5)` → `0.549306` |

### Logarithmic and Exponential Functions
| Function | Description | Example |
|----------|-------------|---------|
| `log(x)` | Natural logarithm | `log(e)` → `1` |
| `ln(x)` | Natural logarithm (alias) | `ln(e^2)` → `2` |
| `log10(x)` | Base-10 logarithm | `log10(100)` → `2` |
| `log2(x)` | Base-2 logarithm | `log2(8)` → `3` |
| `exp(x)` | e^x | `exp(1)` → `2.718282` |
| `exp2(x)` | 2^x | `exp2(3)` → `8` |
| `exp10(x)` | 10^x | `exp10(2)` → `100` |

### Other Mathematical Functions
| Function | Description | Example |
|----------|-------------|---------|
| `sqrt(x)` | Square root | `sqrt(16)` → `4` |
| `cbrt(x)` | Cube root | `cbrt(27)` → `3` |
| `abs(x)` | Absolute value | `abs(-5)` → `5` |
| `floor(x)` | Largest integer ≤ x | `floor(3.7)` → `3` |
| `ceil(x)` | Smallest integer ≥ x | `ceil(3.2)` → `4` |
| `round(x)` | Round to nearest integer | `round(3.7)` → `4` |
| `deg(x)` | Radians to degrees | `deg(pi)` → `180` |
| `rad(x)` | Degrees to radians | `rad(180)` → `3.141593` |

### Utility Functions
| Function | Description | Example |
|----------|-------------|---------|
| `min(a, b)` | Minimum of two values | `min(5, 10)` → `5` |
| `max(a, b)` | Maximum of two values | `max(3.14, 2.71)` → `3.14` |
| `clamp(val, min, max)` | Constrain value to range | `clamp(15, 0, 10)` → `10` |
| `lerp(a, b, t)` | Linear interpolation | `lerp(0, 100, 0.25)` → `25` |
| `hypot(x, y)` | Hypotenuse (√(x²+y²)) | `hypot(3, 4)` → `5` |

### Programming Functions
| Function | Description | Example |
|----------|-------------|---------|
| `if(cond, true_val, false_val)` | Conditional expression | `if(5 > 3, 100, 200)` → `100` |

### Comparison Operators
| Operator | Description | Example |
|----------|-------------|---------|
| `<` | Less than | `3 < 5` → `1` (true) |
| `>` | Greater than | `5 > 3` → `1` (true) |
| `<=` | Less than or equal | `3 <= 3` → `1` (true) |
| `>=` | Greater than or equal | `5 >= 5` → `1` (true) |
| `==` | Equal to | `pi == 3.14159` → `1` (true) |
| `!=` | Not equal to | `2 != 3` → `1` (true) |

## License

MIT.

## Contributing

Contributions are welcome! The calculator now has a solid foundation with:

✅ **Complete mathematical function library**  
✅ **Variables and user-defined functions**  
✅ **Programming logic (conditionals, comparisons)**  
✅ **Utility functions (min, max, clamp, lerp, hypot)**  
✅ **Cross-platform compatibility**  
✅ **Memory safety and error handling**  

Some ideas for further enhancements:
- Complex number support
- Matrix operations
- Statistical functions (mean, median, stddev)
- Additional programming constructs (loops, recursion)
- Unit conversion functions
- File I/O for saving/loading variables and functions
- Command history and tab completion
- Plotting capabilities