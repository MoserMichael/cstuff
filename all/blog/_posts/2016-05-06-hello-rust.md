---
layout: post
title: Rust - the new kid in town
---

{{ page.title }}
----------------

<p class="publish_date">
6 February 2016

</p>
Rust is a promising new programming language

-   Rust is similar to C** in terms of runtime costs: memory management is explicit, there is no automatic garbage collector.
-   The main design philosophy (like in C**) is to provide higher level language features that are checked at compile time with no additional runtime cost, all in order to keep runtime complexity as low as possible.
-   Rust tries to provide memory safety: all heap allocated memory has its owner (owning variable), memory of heap allocated data is freed once the owner of this memory goes out of scope. Lifetime of references is enforced by the compiler, a program that passes compilation is supposed to be free of access to freed/invalid memory. This is a really big improvement over C**.
-   In terms of syntax and type system: Rust tries to favor a more functional style of programming. It has closures, a function can return a closure as return value (!); it also has variant enum (a type that can assume any one of a given list of types - that is similar to haskell data definitions) and pattern matching switch statements. (The Rust Option type/error handling is very similar to the Haskell Maybe monad). Rust also has local type inference (like C++11 with auto, go - all newer languages do have this feature).
-   Object oriented programming: A structure can have member functions; all structure fields are 'public' (no data hiding). Rust has interface inheritance (you can implement interfaces), but no implementation inheritance (you can't derive from another structure). Rust has templates (unlike Go).
-   Modularity
-   macros: Rust has quite complex macros, the macro can match entities over the syntax tree and manipulate them (interesting how they will fix slow compilation time while keeping this feature)

How does Rust compare to C** in terms of language complexity? For C** a lot of the complexity comes from the requirement of backward compatibility with C, Rust does not have such a problem. Now Rust does introduce a lot of features that are lacking in C**; these features do add complexity and it doesn't make Rust a simple language. However the promise of safety from memory errors makes it all seem worth the cost.

It is interesting to note that statically typed languages seem to be all the rage these days. Scripting languages were popular in the 90ies and 00's, when the internet/web experienced massive growth and tools that made it easy to develop for the web platform got a lot of attention. Now the party is sort of over as there is no longer the promise of massive growth; also CPUs are not getting much faster as they used to do - the focus now is more efficient use of existing computing resources; the changed realities call for statically typed compiled languages that target native code. Of course an added focus is on security it is too easy to introduce subtle security bugs with C and C**. Rust tries to fit into this niche lets see how fast we will see wide spread adoption.

It will take some time until Rust becomes a mainstream language, it took quite some for C** and Java to become significant players (more than a decade); it takes a lot of time for the language to mature, a 'big' language also needs a significant pool of trained programmers training of a large number of programmers takes time and is very costly.

Also I suspect that it will take quite a while to master this language 'efficiently'/'effectively' (just as with C**).

------------------------------------------------------------------------

My notes on the [Rust book](https://doc.rust-lang.org/book/)

Variables. values / simple types

- builtin types (like stdint.h) i8,i16,i32,i64 signed integers, u8, u16, u32, u64 unsigned integers; isize, usize equivalents for ssize\_t, size\_t (in C). f32, f64 floating point types.

<blockquote>
    <code>
        let x = 5
    </code>

</blockquote>
- x is variable on the stack (by default).

- does type inference; x is immutable (constant) variable; can't assign x = 7
- the full declaration (with type) is

<blockquote>
    <code>
        let x : i32 = 5
    </code>

</blockquote>
- also: you must assign a value to a variable at variable declaration

<blockquote>
    <code>
        let x : 32 // a syntax error
    </code>

</blockquote>
- if you want variable allocated on the heap then

<blockquote>
    <code>
        let x = Box::new(5); // this creates instance of Box<i32> - it points to i32 value allocated on heap (Box<i32> - instantiate a template of type Box that works with i32 (Box is in standard library)
    </code>

</blockquote>
- also: variable lifetime is limited to its scope

<blockquote>
    <code>
        {
            let x = 5
        }
        println!(" {} ", x ) // can't do that ; x is used out of its scope so x is no longer visible.''
    </code>

</blockquote>
also a variable can be redefined (shadowed) in its nested scope, but only the last scope is valid

<blockquote>
    <code>
        let x = 5
        {
            let x = 7 // x is now 7
        }
        // x is 5 again
        let mut x = 5
        x = 7
    </code>

</blockquote>
-

- that's possible because x is now mutable. (by default a declaration is immutable (const))

<blockquote>
    <code>
        let mut x = 5
        let y = & mut x
    </code>

</blockquote>
- you can't assign something else to y because it is not mutable (it is const) ; but you can modify the referenced value and change x !

<blockquote>
    <code>
        * y = 7 // special syntax for changing mutable refs (dereferencing mutable references)
    </code>

</blockquote>
- now the value of x is 7

Functions:

rust program must have a main function (like C); every statement must be in a function

<blockquote>
    <code>
        fn main()
        {

        }
    </code>

</blockquote>
- functions : -&gt; <type> denotes the return value.

<blockquote>
    <code>
        fn add( x : i32 ) -> i32
        {
            x + 1 // block is an expression 
                                    // so last statement in block is the return value (but if the last statement ends in ;
                                    // (semicolon) then syntax error because there is no return value ! 
        }
    </code>

</blockquote>
- can also do return to explicitly return a value

<blockquote>
    <code>
        fn add( x : 32 ) -> i32
        {
            return x + 1 // same as previous  but with explicit return
        }
    </code>

</blockquote>
- closures (anonymous functions)

<blockquote>
    <code>
        let num = 5
        let plus_num = |x : i32 | { x + num } // | <parameter list > | <body> ; also references num  another binding.
    </code>

</blockquote>
- closers can mutate captured variables (but they have to be marked as mutable)

<blockquote>
    <code>
        let mut num = 5;
        let mut add_num = | x |  num += x; // the closure is mutable  and it modifies a mutable binding num
        add_num(5); // after the call the value of num has changed to 10    
    </code>

</blockquote>
but the closure can be made to copy the referenced captured variable (move capture)

<blockquote>
    <code>
        let num = 5
        let mut add_num = move |x | num += x
        add_num(5) // after the call num is still 5 ! - the move capture made a copy of captured variable num
    </code>

</blockquote>
.
data / more complex data structures

- arrays fixed size arrays

<blockquote>
    <code>
        let v = [1,2,3];

        println!( "the first one {} number of entries {}", vec[0], vec.len() ); // prinln! Is a macro call, first {} puts in the string for second param, etc.

        let n = [ 1 ; 20 ];  // twenty entries of value 1
    </code>

</blockquote>
- vectors dynamic vectors allocated on heap. (vec! Is a macro call generated code constructs a vector and fills in the specified data)

<blockquote>
    <code>
        let v = vec![ 1,2, 3 ]
    </code>

</blockquote>
- out of bound access will result in panic (program abort)

<blockquote>
    <code>
        println!("will panic ! {}" , v[4] )
    </code>

</blockquote>
- fixed size string (unicode string UTF8) (string constants on initialized data section)

<blockquote>
    <code>
        let a = "can span 
            multiple lines"
    </code>

</blockquote>
- can't access n'th element directly but have to convert to chars

<blockquote>
    <code>
        let secondLetter = a.chars().nth(1)
    </code>

</blockquote>
- or convert to array of bytes

<blockquote>
    <code>
        let secondByte = a.as_bytes().
    </code>

</blockquote>
- dynamic strings (on Heap) that can be concatenated type String

<blockquote>
    <code>
        let hello = hello.to_string() // to_string turns str to String.

        let world = world // of type str (fixed size string literal)
        let hw = hello + world // note that second variable is fixed size literal !

        Dereferencing of String gets you a str (because operator * is overloaded for String)
    </code>

</blockquote>
- structures

<blockquote>
    <code>
        struct Point {
            x : i32,
            y : i32,
        }
        let point = Point { y : 5 , x ; 4  } // any order of the fields can do  as long as all  fields (name : value pairs) are specified
        println!(" x {} y {}", point.x, point.y )
    </code>

</blockquote>
- tuples: a struct with fields/members that have no name

<blockquote>
    <code>
        struct IntPair { i32, i32 }
        let IntPair tuple = IntPair( 15, 20 )
    </code>

</blockquote>
- can access values as tuple.0 first member of value; tuple.1 second member of value , ...

- or more tricky by means of pattern matching let (assignment)

<blockquote>
    <code>
        let IntPair encoded = IntPair( 15, 20 )
        let IntPair( x, y ) =  encoded // assigning with pattern matching
        println!( "x {} y {} " , x, y );
    </code>

</blockquote>
- interfaces are called traits

<blockquote>
    <code>
    trait Printable 
    {
        fn print( &self );
    }

    struct Circle {
        x : f64,
        y : f64,
        radius : f64
    }

    impl Printable for Circle {
        fn print( &self )
        {
            println!(" x {} y {} ", self.x, self.y )
        }
    }
    </code>

</blockquote>
- traits can inherit (the inherited interface adds additional methods)

<blockquote>
    <code>
    trait Foo {
        fn foo(&self);
    }

    trait FooFoo { // FooFoo has both foo and bar.
        fn bar(&self);
    } 
    </code>

</blockquote>
if a struct implements FooFoo then it has to implement both Foo and FooFoo traits !

template types can be required to support a trait/interface

<blockquote>
    <code>
    fn print_me<T : Printable>( x : T )  { x.print() } // T : Printable  == require type T to implement trait Printable 
    </code>

</blockquote>
- of you can require that a template type supports multiple interfaces

<blockquote>
    <code>
    fn printMe< T : Printable + Debug> ( x : T ) // requires that x supports both Printable and Debug trait
    </code>

</blockquote>
- or equivalent form

<blockquote>
    <code>
    fn PrintMe<T> ( x : T) where T :  Printable + Debug
    </code>

</blockquote>
- that is supposed to be more convenient with multiple template types that have several requirements.

<blockquote>
    <code>
    fn PrintMe<T : Printable, U : Printable + Debug> ( x: T, y: U)

    fn PrintMe<T,U> (x : T, y: U ) where T : Printable, U : Printable + Debug 

    </code>

</blockquote>
- with generic structure implementation you can require the template type to support a given interfaces

<blockquote>
    <code>
    struct Rectangle<T>
    {
        x : T,
        y : T,
        width : T,
        height : T,
    }

    impl<T : PartialEq> Rectable<T> { // require T to support Partial Eq (? why is this requirement not in the type definition ?)

        fn is_square(&self) -> bool {
            self.width == self.height // if T supports ParialEq then it has a overloaded == operator.
        }

    // 
    </code>

</blockquote>
- Object Oriented p. with structs (member functions and constructors)

<blockquote>
    <code>
    struct Circle {
        x : f64,
        y : f64,
        radius : f64
    }

    impl Circle {  // member functions are added in one or more impl  blocks.
        fn new( x : f64, y f64, radius : f64) // new is a static method  it does not require this/self.
        {
            return Circle{ x : x, y : y, radius : radius );
        }
        f64 x(&self) -> f64 { return x.x } // member function  self reference
        Circle x(mut &xelf, x : f64 } -> mut & Circle{ self.x = x; return x; } // mutable reference to self. Returning ref to self allows you to chain calls.
    }
    </code>

</blockquote>
- templates/generics

<blockquote>
    <code>
        enum Option<T> {
            Some(T),
            None
        }
    -   let x = Option(5) // type x inferred to be Option<i32>
        let y = Option(5.0) // type y inferred to be Option<f32>
    </code>

</blockquote>
- function templates

<blockquote>
    <code>
        fn do_it<T, U> ( T : x, Y : y)
    </code>

</blockquote>
- structs with templates

<blockquote>
    <code>
        struct Point<T> {
            T : x,  
            T : y,
        }
        impl<T> Point<T> {
            fn (&mut self, T x } -> Point<T> { self.x = x; return x }
        }
    </code>

</blockquote>
- overriding stuff:

- each type can have a destructor that is called when an instance of the type goes out of scope.
- you get that by implementing the Drop interface for the type.

<blockquote>
    <code>
    struct HasDTor {
        int a;
    }

    impl Drop for HasDTor {
        fn drop(&mut self) {
            println!("calling the dtor ");
        }
    }
    </code>

</blockquote>
control structures:

rust has its own switch statement (called pattern)

<blockquote>
    <code>
        match x {
            1 => println!( "1\n" ),
            2 | 3 => println!( "2 or 3\n" ),
            4 .. 7 => println!( "4 or 5 or 6 or 7  a range as selector\n" ),
             e @ 8 .. 12 => println!( "for range you can get value of the match {} \n", e )
            _  => println!("something else  default\n" ),  // _ => matches all other values  
        }
    </code>

</blockquote>
this match thing most cover all values ; so without \_ =&gt; it gives error that not all values are covered by this switch.

- it can also retrieve structure values

<blockquote>
    <code>
        struct Point { x : i32, y : i32 }

        p = new Point{ x: 1 , y : 2 }

        match p  {
            Point{ x, y } =>  println!("x={} y={}\n", x, y ),   
        }
    </code>

</blockquote>
- more usefully it is used to handle enum values each case is handling a possible type of the enum

- it also works on enums (a union where each field is of different type, but which one of them is in effect is chosen at assignment)

<blockquote>
    <code>
        enum Message {
            MoveIt { x : i32, y : 32 }, // struct with two fields
            ChangeColor( i32, i32, i32 ) // tuple with three fields
        };
        let m : Message = Message::ChangeColor(1,2,3) // we determine that m holds a ChangeColor tuple
    </code>

</blockquote>
- each case deconstructs the type value (fields are extracted in right hand side)

<blockquote>
    <code>
        match m {
            MoveIt( x, y ) => { println!( "x{} y {}" , x, y ) // this case handles MoveIt type
            ChangeColor( x, y, z ) => println( "x {} y {} z {}",  x, y, z)
        }
    </code>

</blockquote>
- structure fields can't be declared mutable its the variable (binding) of type point can be mutable or not.

<blockquote>
    <code>
        struct Point
        {
            x : i32,
            y : y32,
        }
        mut p = new Point { x: 5, y : 7 }
        p.x = 11
        pp = new Point { x : 1, y : 2 }
        pp.x =22 // compilation error:
    </code>

</blockquote>
but no, you can emulate fields that can be modified in non mutable bindings !!! (why?)

<blockquote>
    <code>
        use std::cell::Cell
        struct Point
        {
            x : i32,
            y : Cell<i32>
        }
        let p = new Point( x: 5, y : Cell::new(6) }
        p.y.set(8) // now value of y is 8.
    </code>

</blockquote>
- Enums (not your father's C enum here each member is a type, so it is a union of types, but the compiler knows that one specific type has been chosen as the value and enforces it accordingly)

<blockquote>
    <code>
        enum Message {
            MoveIt { x : i32, y : 32 }, // struct with two fields
            ChangeColor( i32, i32, i32 ) // tuple with three fields
        };

        let x : Message = Message::MoveIt{ x : 10, y : 20 } // constructs an instance of message and sets its implementation type to MoveIt. Value x can't be treated as ChangeColor !!!        
    </code>

</blockquote>
- messages are processed with match keyword (see under control structures)

- member functions (structures and enums can have member functions\_

<blockquote>
    <code>
        impl Point 
        {
            // each new member function can be in its own impl block (or several grouped together) 
            fn distance(&self, Point other) -> f64
            {
                (self.x  other.x) * (self.x  other.x) + (self.y  other.y) * (self.y  other.y) 
            }
        }


            impl Point
            {
                   fn move(& mut self, Point other) -> & mut Point // mutable this  if you want to modify the object  !

                // return value of mutable self  so that you can chain methods 
                //     let &mut pt = Point{ x : 0 , y : 0 }         
                //     pt.move(Point{ x: 10 , y: 20 } ).distance( Point { x : 100, y: 100 } )
                   {
            self.x += other.x;
            self.y += other.y;
            self
            }
        }
    </code>

</blockquote>
- generics: syntax is similar to C** and Java templates.

<blockquote>
    <code>
        struct Point<T> {
            x : T,
            y : T
        }
    </code>

</blockquote>
- one can also parametrize functions:

<blockquote>
    <code>
        fn two_args<T,U> ( x : T, y : U )
    </code>

</blockquote>
Memory management / Ownership

- the main 'killer' feature of Rust; makes sure that only one variable owns the data (one binding)

- Move types : this means that assigning the value moves ownership

<blockquote>
    <code>
        let v = vec![ 1, 2, 3]
        let u = v  // for vector this moves ownership to u; can't use v afterwards
        println!("{}", v[0]) // compiler error: acessing moved value !!!
    </code>

</blockquote>
move semantics are the default: but if type implements Copy trait then it is copied by value (like i32 etc)

<blockquote>
    <code>
        let n = 1
        let m = n
        println!("still can use n {} because n was copied not moved", n )
    </code>

</blockquote>
- also passing a vector to a function also moves ownership from caller of function to function implementation (like assignment); so use references for every function argument that is not easily copied.

- References: (making a reference is called to 'borrow')
- Rule: there can be more than one 'const' reference to a value;

<blockquote>
    <code>
        let num = 32
        let refToNum = &num
        let anotherRefToNum = &num

        println("get a value from ref {}" , *refToNum ) // dereference
    </code>

</blockquote>
- Rule: there can be one mutable reference (where you can change the referred to value)

<blockquote>
    <code>
        let mut num = 32 /
        {
           let & mutRef = & mut num  // & mut creates mutable reference, referred to value must be mutable to begin with !

           *mutRef += 1 // now value of num is 33, you need * only with Mutable references. !!!
        }
        println!( "the value is now {}", num ); // can create new 'const' reference only when there is no active mutable reference !!!
    </code>

</blockquote>
Rule: you can't make a new 'const' reference while a mutable reference exists/
(q: what about dereferencing of existing 'const' reference while mutable reference exists?)
that rule prevents data 'races' access of value while another part of the program may modify the value.

- rational: supposed to prevent iterator invalidation in a loop each iteration creates new const ref; but modification of collection (owner) involves holding mutable reference.

- Lifetimes: goal: prevent access of freed memory via reference.

<blockquote>
    <code>
    fn two_refs_as_args( x : & int32, y &int32 )  ->  & int32 // does not compile !!!!@
    </code>

</blockquote>
- now the function could sometimes the first reference with one lifetime, and sometimes the second one (ambiguity); so it is impossible to infer the lifetime of the returned reference for the general case !

- also if the caller of the function is in a different package then the compiler does not always have the source code of the function;
- therefore need to specify the lifetime of each ref (unless the compiler can't infer it for very simple cases as function with one argument)

<blockquote>
    <code>
    fn two_refs_as_arguments<'a, 'b> ( x : & 'a int32 , x : & 'b int32 ) -> int32 & 'a
    </code>

</blockquote>
- lifetimes are specified as 'a and 'b (both are part of the type signature of the function). They are given as function ; lifetime is given before before type.''

Same applies to structures that hold two references as member fields : a member function of the structure might sometimes return the first structure field (reference) and sometimes the second one, in this case it is impossible to infer the lifetime of the return value.

<blockquote>
    <code>
    struct Foo<'a,'b> {
        x : int32 'a,
        y : in32 'b,
     }

    impl Foo<'a,'b> Foo<'a>  {
        fn x(&self) -> & 'a  i32 { return self.x }
        fn y(&self) -> & 'b i32 { return self.y }
    }
    </code>

</blockquote>