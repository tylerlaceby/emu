# EMU LANGUAGE
Just a toy language I am working on.
The emu language aims to have a super simple FFI interface and a very easy to use std library to prototype 2d graphic applications. I imagine emu being great for creating 2d graphics and simulation applications with a language that has easy interop with C/C++. Mix of pygame api, js syntax and a dead simple ffi system.

For now emu will be interpreted using a tre-walker method. However as the language gets more mature I am hoping to have emu be emu-bytecode for faster runtime gains and platform portability. I expect that to be a few months off at least!

### Disclaimers
Since Emu is in such a early state, I would suggest and advise against using it unless you expect to run into many runtime crashes and issues on most platforms.
I mainly develop emulang on Fedora 36 and Windows 11. 

Emu is a personal project & toy language and nothing more. I plan to make it better and mopre performant/ stable however I just want to use this to make 2d physics simulations and for fun. I appreciate any help and feedback but I dont have any serious goals with emu.

## Early preview at the syntax
Now that the language has been explained lets see the syntax.

```py

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
