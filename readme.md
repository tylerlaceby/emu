# EMU LANGUAGE
Just a toy language I am working on. The focus of emu is to be compatible with C/Rust code over C-ABI and have native support for FFI on Linux, osx & Win32.

## Early preview at the syntax
All of this is subject to change very much :) The language is supposed to feel and work similar to javascript but with support for FFI and macros built in.
The syntax is generated using a recursive-descent parser instead of a parser-generator which will come in handy when implimenting newer features/syntaxex.


```js

# VARIABLES & Base Types
# This is a comment inside Emu
let value = 10

# To declare a variable you must use the null keyword. There is zero support for let n; syntax where its defined as null by default.
let n = null 

# true and false
let t = true;
t = false;

# Object literals

let person = {
  age: 22, color: "Red",
  name: "Tyler",
  isDev: true,
  blank: {},
  nested: {
    foo: "Bar":,
    n, This will capture the value of n defined above.
    # its the same as n: n
  },
}

println (person.name) # Accessing objects is super easy!

# You can also access Modules and namespaces using the .operator.

const x = 20.5
# Constants are also in emu.

# FUNCTIONS

fn add (x, y) {
  x + y
}

# Declare a function that takes in two numbers and returns the sum.
# No need to type return if you dont want too.
 
 
# Using closures we can create classes.
fn Circle (r) {
  # Acts like a private variable.
  let radius = r
  
  fn area () {
    return Math.PI * Math.exp(radius, 2) # Square is a default function in the Math library.
  }
  
  # Public Getter method.
  fn getRadius () { radius }
  fn setRadius (_r) { radius = _r }  
  
  # Then we can export this method and the variable to a make a closure/class.
  return { getRadius, setRadius, area }
}

# Create a circle instance.
const circle = Circle(10)

# Call a public method exposed.
println ("The area of the circle is = ", circle.area())


```

## Development

The language is written in C++ and I want to target a custom bytecode at somepoint. Until then this language will be slow and not very usable for anything really. But check it out and let me know of any feedback you may have. If you want to help develop Emu for some weird reason ? Then make a pull request and I will happily check it out.

Cheers!
