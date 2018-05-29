---
layout: post
title: Effective post-modern C++
---

{{ page.title }}
----------------

<p class="publish_date">
2 December 2014

</p>
C** used to have default constructors, copy constructors, assignment operators, cast conversion constructor, constructors with arguments; each such feature is invoked by means of some non obvious syntactic construct.

With C++11 they added move constructors, move assignment operators, initializer constructor; and optional special constructors for r-rvalue references only. now suddenly one has to be a lawyer in order to understand all possible interactions between these features.

Luckily there is Scott Meyers who can explain the resulting mess of regulations; "Effective Modern C**" (purchase [here](http://shop.oreilly.com/product/0636920033707.do) ) does a great job at explaining how all the new features of C+*11 and c*+14 fit together.

is the added complexity worth it? I don't know, there are two opinions here:

C** critics:

-   many system programmers do not like the exception feature, because it makes it almost impossible to handle error conditions without some kind of subtle leaks.
-   system programmers often decry the a loss of control that comes with C**; compared to C it is harder to tell what code is being generated. (See Linus Thorvalds famous [C** rant](http://harmful.cat-v.org/software/c++/linus) )
-   the many things done implicitly, this implicit nature of things makes it difficult to reason about the problem when things screw up (and they do). As the C++FQA put it [here](http://yosefk.com/c++fqa/mixing.html)

<blockquote>
"C** is not a higher-level language than C. The damage caused by low-level errors is still not limited. You still have to think about pointers and object life cycles and integer endianness and many other things. But on top of that, there's a huge amount of things done implicitly, like global initialization and destruction, stack unwinding, base/derived classes pointer adjustment, and many more things - and all of them combine with the low-level errors into a single deep, wide tar pit with the programmer in the middle.

A good high-level language allows you to forget about many small details of program execution. A good low-level language allows you to control the many small details of program execution. C** is not much of a high-level language, but it's not a very good low-level language either."

</blockquote>
C** fans:

-   application programmers really seem to value the expressiveness of C**;
-   'conservative' approach to software engineering (as Steve Yegge put it [more here](http://mosermichael.github.io/cstuff/all/blog/2014/11/09/steve-yegge-politics.html) ) in these quarters they don't like text based meta programming; however templates are considered to be powerful enough (somehow they don't mind the convoluted nature of C** templates)
-   in most shops they have coding guidelines that pick the desired C** features and formulate 'best practices'; the stated aim here is to create a 'manageable subset' of the language. Well results are mixed (it's usually like this with standardized regulations ...)

On the plus side of things c+*11 and C14 do address real pressing problems of C*+98:

-   the override specifier makes it hard to make errors while overriding virtual functions; now one constructor can call another one (Java had these feature for a long time)
-   move semantics can avoid unnecessary deletion of temporary objects (in C+*98: void foo(string &b,string &c) { string a = b* c; } // b + c used to be a wasted temporary copy, nowadays the data of this copy is moved right into a.
    however this feature comes at the cost of great complexity. at least half of this book is about r-value references and move semantics.
-   auto keyword + type inference significantly reduces the verbosity of C** code. In 1983 the auto keyword used to be a feature of c**, but it was later dropped !!! Later it got adopted by languages like D and Java (since version 7); so the pressure was on C** standard committee to make the language more 'powerful'.
-   there is some form of function invocation/computation at compile time. (but it can't be used for meta-programming)
-   lambda functions are supposed to make stl::algorithms usable, but i doubt that this will happen: try to debug this code; i guess people will still prefer loops over std::algoritm .
    also keep figuring when captured variable is copied by value or by reference (you can even move stuff into the capture !)
-   a richer standard library; std::uniqe\_ptr is nice (shared\_ptr & reference counting is messy and it always uses atomic/lockless reference count, no way to customize that); they added atomic(lockless) integer types, which is nice; the new concurrency API. STL collections add emplace - avoids creation of temporary values in some cases.

I guess C+*11 is the result of 'evolutionary pressure' - over time other programming languages added a lot of features (some sort of type deduction and lambda functions) and the C*+ central committee decided that it had to catch up in order to stay relevant. It is funny that this multitude of features creates many ways of expressing the same thing - just like in Perl, the [postmodern programming language](http://www.wall.org/~larry/pm.html)

the bottom line: for me it is advisable to read this mess in order to remain employable.

Also here is a nice overview of "C+*11":https://en.wikipedia.org/wiki/C14 and "c14":https://en.wikipedia.org/wiki/C*+14 from wikipedia.

------------------------------------------------------------------------

my notes of Scott Meyers: 'Effective Modern C**' (Preview)

(Item1)

different kinds of type deduction in C**:

c++98 deduction for template&lt;&gt; member functions (like template<typename T> void f(const T &arg)
c++11 auto and decltypeg
sc++14 decltype can be used with auto

- the same mechanism for type deduction applies to all three cases.

1) (template is reference (or point))

<blockquote>
    <code>
    template<typename T>
    void f(const T  &arg)
    </code>

</blockquote>
-   can call f with references and value types (ignores if it is reference)
-   if called with reference T is deduced to be type on the left of &.

2) (template type is universal reference)

<blockquote>
    <code>
    template<typename T>
    void f(const T  && arg)
    </code>

</blockquote>
-   called with value type (int); T is inferred to be int &
-   called with rvalue (like constant literal) ; T is inferred to be int &&

3) (template type is value type)

<blockquote>
    <code>
    template<typename T>
    void f(const T   arg)
    </code>

</blockquote>
-   copy by value; if pointer then the pointer value is copied
-   function pointers turn to signature of function (not suprising)
-   arrays treated as pointers (not suprising)

But:

<blockquote>
    <code>
    // return size of array as a compile time constant

    template<typename T, std::size_t N>
    constexpr std::size_t arraySize(T (&)[N])
    {
        return N;
    }

    int a[10];

    arraySize(a); // returns 10 !!!
    </code>

</blockquote>
(Item2) different kinds of auto type deduction

<blockquote>
    <code>
    auto x = 27;         // x is int


    const auto cx = x;
    // cx is const int  (same deduction as in: template<typename t> void f(T param))
    const auto &rx = x;
    // rx is const int (same deduction as in: template<typename t> void f(const T &param))

    auto &&u1 = x;       // u1 is int &
    auto &&u2 = cx;      // u2 is const int &
    auto &&u3 = 27;      // u3 is int &&


    const char name[] = "ha ha ha";

    auto arr = name ;    // arr is const char *
    auto &arr2 = name;   // arr is const char (&) [13]; (arraySize function trick in item#1)

    </code>

</blockquote>
auto type deduction != template type deduction) (special kind for :Universal initializer: (why?))

<blockquote>
    <code>
    auto x = { 1 };
    // x is a std::initializer_list<int> and has an instance with value 27

    auto x = { 1, 2 };
    // x is a std::initializer_list<int> and has an instance with values 1 and 2

    auto x = { 1, 2, 3.0 };
    // error: values in initializer list must be of the same type.

    //C++14 allows function return types to be deduced with auto,
    // but that does not work for this case !!! (why ???)


    //C++ function type can create generic lambda expression !!!!
    //(but you can't instantiate with initializer list (?!))
    </code>

</blockquote>
(Item3): decltype

decltype returns type of argument expression.

auto lambda\_template = \[&v\] (const auto &newValue) { v = newvalue; }

C++11: decltype can be used in trailing return type specifications:
allows to specify return type as returned type of decltype !!!

<blockquote>
    <code>
    template<typename Container, typename index>
    auto foo(Container &c, Index &n) -> decltype(c[i])
    </code>

</blockquote>
!!!! that's only way to have return type conditional on parameter types (as parameter types must have already been parsed when return type is set ! !!!

again, notype deduction for :universal initializers: {1, 2, 3}

C++14: the same trick can be done more concise:

<blockquote>
    <code>
    template<typename Container, typename index>
    decltype(auto) foo(Container &c, Index &n)
    {
        return c[n];
    }
    </code>

</blockquote>
So what is the return value?

<blockquote>
    <code>
    Widget a;
    const Widget &b = a;

    auto rr = b;           // rr is Widget &
    decltype(auto) r = b;  // r is const Widget &  - same as the right hand side.
    </code>

</blockquote>
Therefore the return value of foo is const reference to element of c.

What if you want foo to receive r-value references (to temporary objects?); reference to element here will not outlive the argument; so the return value must be a copy of the contained element.

<blockquote>
    <code>
    template<typename Container, typename index>
    decltype(auto) foo(Container &c, Index &n)
    {
       return std::forward<Container>(c) [n]; // don't we love C++ ?
    }
    </code>

</blockquote>
in C++11 this looks like

<blockquote>
    <code>

    template<typename Container, typename index>
    auto foo(Container &c, Index &n) -> decltype(std::forward<Container>(c) [i])
    {
        return c[i];
    }
    </code>

</blockquote>
watch this in C++14 !!!!

<blockquote>
    <code>

    decltype(auto) f1()  // returns an int
    {
       int x = 2;
       return x;
    }


    // returns an int & (!!!oh my, this will crash when used!!!)
    decltype(auto) f1()
    {
       int x = 2;
       return (x);       // watch the parenthesis !!!!
    }
    </code>

</blockquote>
(item 3) how to know what has been deduced

-   during runtime: typeid(x).name() - gcc returns mangled name (convert with c++filt) ; however it is not the exact type (ha ha ha)
-   IDE gives hints (if you use one) - but it relies on typeid so its not reliable too.
-   platform dependent: gcc has macro: *PRETTY\_FUNCTION* , MSVC has *FUNCSIG*
-   compiler error message includes deduced type; now that is reliable ;-)

(item 5) prefer auto

-   trivia: in 1983 C** had auto, but it was not accepted ! in C++11 it came back
-   (did not mention that Java and D also have auto ;-)
-   advise: always use auto, this way you don't have uninitialized variables (well compiler would tell you that in any case, with optimization ;-)
-   use of auto will avoid downcasting (if lhs type is narrower than actual type); or copying if automatic type conversion is invoked (due to declared type being different to actual type)
-   in c++14: can have auto arguments to functions and assign result to auto - template lambdas !

auto derefLess = \[\](const auto& p1, const auto& p2) { return \*p1 &lt; \*p2; }; // pointer-like

-   auto for lambda types is preferable to std::function; std::function allocates memory and is slower to invoke.
-   auto use is more difficult to read; but is flexible and less error prone.

------------------------------------------------------------------------

(item 7) initialization

std::vector<int> a ={ 1, 2, 3 }; // braced initialization: does a.push\_back(1); a.push\_back(2); a.push\_back(3);
std::vector<int> a{ 1, 2, 3 }; // same as the last case.
std::vector<int> a ={ 1.0 }; // can't do that ! must be the exact type - int, does not allow narrowing/implicit conversion.

is immune from most vexing parse: braced initialization prevents this construct:
Widget v(); // this does not create an object v - this is a forward declaration of function v that returns Widget.

Neat: problem that braced initialization does amazing things with auto:

auto v3{-1}; // type of v3 is not int; it is std::initializer\_list<int>
auto v4 = {-1}; // same here

But! if class Widget has this constructor

<blockquote>
    <code>
    class Widget
    {
    public: Widget(std::initializer_list<bool> il)

    }

    Widget w{ 3, 4.0 };
    // Now the constructor with init list is called; AND IT DOES TYPE CONVERSION !@#@!
    </code>

</blockquote>
It gets worse:

<blockquote>
    <code>

    class Widget {
    public:
      Widget(int i, bool b); // con #1
      Widget(int i, double d); // con #2
      Widget(std::initializer_list<std::string> il); // con #3
    };

    // calls con #2 (exact mach of parameter types)
    Widget w1(10, true); Widget w3(10, 5.0);

    // Can't call #3 - no implicit conversion to std::string. Call con #1 !!!@#@!!
    Widget w2{10, true};

    // Can't call #3 - no implicit conversion to std::string. Call con #2 !!!@#@!!
    Widget w4{10, 5.0};

    // calls default con; there is no empty std::initializer_list !!
    Widget w5{};

    // forces creation of empty std::initializer_list !@#!\
    Widget w5({});

    // same here
    Widget w5({});
    </code>

</blockquote>
! principle: if initializer list constr. is there then it is preferred one; (even if implicit type conversion is required) !
! this is different to normal logic, where you use the constructor that does not require type conversion !

Also with {} copy const and move constr. are called when they are most appropriate (no type conversion required)

(item 8) nullptr

- nullptr is not integral type, but it can convert to pointer types;

<blockquote>
    <code>
    void foo(int);      // #fun1
    void foo(void *a);  // #fun2

    foo(NULL); // NULL is 0; therfore calls #fun1
    foo(nullptr) // does not convert to int; so calls #fun2
    </code>

</blockquote>
-   nullptr helps to read code when variable is declared as auto; with nullptr you know that's supposed to be a pointer.
-   if template argument requires a pointer type, then you have to pass nullptr; 0/NULL would not compiler (type mismatch)

(item 9) alias declarations better than typedef

instead of
typedef std::vector<int> IntVector;

do

using IntVector = std::vector<int>;

Now with using you can also do templated declarations

<blockquote>
    <code>
    template<typename T>
    using MyVec = std::vector<T, MyAlloc<T>>
    </code>

</blockquote>
then can use MyVec<int> instead of std::vector<int> ; (good for nested templates, but oh my)

- and for template writers it helps to get rid of typename:

<blockquote>
    <code>
    template<typename T>
    class MyHack
    {
       typename std::vector<T>::type Vect; // (vector<T>::type is the self type typedef, especially for this purpose !)
                           // so Vect is a 'dependent type' (oh my)

      // if you want to use the template this way you have to use typename.
      Vect imp_;
    };
    </code>

</blockquote>
Now using can help you to get rid of this !!!

<blockquote>
    <code>

    template<typename T>
    using MyVec = std::vector<T, MyAlloc<T>>


    template<typename T>
    class MyHack
    {

       MyVec<T> imp_; // works without typename
    }
    </code>

</blockquote>
(item 10) scoped enums

enums in c++98

<blockquote>
    <code>
    enum Color { red, green };

    // values visible in the same scope as enum declaration ('namespace pollution').
    // Also cast from enum to int value.
    int x = red;
    // downside: more to type and name is longer (have to do scope::)

    //in c++11


    enum class Color = { red, green };

    Color x = Color::red;
    // now must qualify enum value with class name (no namespace pollution);
    //also no implicit type conversion to integer not possible !! (but you can cast explicitly)
    // C++98: comiler will choose the smallest type that can hold the number
    // (based on maximum value); so type can change if maximum value changes !
    // C++11: type to represent enum is always int. (allows for better forwarding of enum;

    // C++11 also alows to change the representation type explicitly. (after : )
    enum class Color : std::uint32_t;

    </code>

</blockquote>
(item 11) functions marked as deleted (prevent declared function from being called)

prevent function from bein called (like copy constructor and assignment op) ?
C++98: mark as private; and do not provide an implementation (so that linker error is forced)

C++11: mark as deleted; this way usage of copy constr. just does not compile.

basic\_ios( const basic\_ios & ) = delete; // c** way to mark the function as not callable ; also convention is to declare this public - compiler will then tell that this is not callable (calling private would be another compilation error)

&gt; Want to prevent some template instantiation for some specific types from happening? use deleted

<blockquote>
    <code>
    template<typename T>
    void doIt(T *);

    //but not for char *
    template<>
    void doIt(char *) = deleted.
    </code>

</blockquote>
(item 12) override qualifier.

C++98: Overriding virtual function works if

-   override marked as virtual
-   function name, arguments types and return types are identical
-   same const specification.

C++11: adds requirement that 'reference qualifiers are the same' (OMG) (can say that virtual function is only good for r-value or l-value)

Now a small difference in type specs will create new overloaded function instead of doing virtual function overriding.

<blockquote>
    <code>
    class Base
    {
      virtual foo() override;
    }

    class Derived
    {
      // this will not compile; override says that foo
      // name must override -> comply with the rules.
      virtual foo(int a);

    //Now more about 'reference qualifiers'

    class Base
    {
       void foo() &; // only callable if this is lvalue
       void foo() &&; // only callable if this is rvalue (OMG)

    </code>

</blockquote>
(item 13) prefer const iterators

New in C++11 - stl collections have cbegin and cend - to return const iterators (and crbegin/crend - for const reverse iterators). (before that usage of const iterators was difficult)

For maximum generality (for example in templates) use std::cbegin(container) - for build in arrays these are provided as global functions !) (in C+*14 std::cbegin©/std::cend© were added, but they already had std::begin©/std::end© in c*+11 !!!)

this one would add cbegin for C++11 too:

<blockquote>
    <code>
    template <class C>
    auto cbegin(const C& container) -> decltype(std::begin(container))
    {
       /// works because const C& returns const iterator !!! (used to work in c++98)
       return std::begin(container);
    }
    </code>

</blockquote>
(item 14) constexpr - magic during compilation

- constexpr specifies value known during compilation (so that it can be placed into read only memory)

<blockquote>
    <code>
    constexpr auto arraySize = 10;
    // arraySize is constant known during compilation

    std::array<int, arraySize> data;
    // the size of an array must be known during compilation, so constexpr is fine
    </code>

</blockquote>
constexpr int pow(int base, int exp); // functions with contexpr specifier: if called with values known during compilations (constexpr) these functions will create constexpr values !!! (that is the function is invoked during compilation and not during runtime) !!!

in C++11 constexpr function must consist of one return statement only; but we can use recursion.

<blockquote>
    <code>
    constexpr int pow(int base, int exp)
    {
       return exp<=0 ? 1 : base * pow(base, exp-1);
    }
    </code>

</blockquote>
in C++14 there are less restrictions on implementation of const expression, so they look like normal functions (as long as no non constexpr data is accessed)

Even classes can look like that!!

<blockquote>
    <code>
    class Point
    {
    public:
       constexpr Point(intx, y) : x_(x), y_(y) {}
       constexpr int getX() { return X_; }
       constexpr int getX() { return y_; }
    </code>

</blockquote>
And so that

constexpr Point x{1.1}; // all happens during compilatioN! and these constexpr point objects can be used in constexpr functions returning constexpr results !!!

However setter functions would break constexpr magic..

(item 15) thread safe const members

In C++98 you can have const member functions that modify state of an object;: example

<blockquote>
    <code>
    class Polynomial
    {
      const float getRoots() const
      {
        if (cacheValid) {
            return cachedValue;
        }
        // compute cachedValue and set cachedValid
        return cachedValue
      }

      mutable bool cacheValid { false };
      mutable bool cachedValue;
    }
    </code>

</blockquote>
this item tells you to properly lock access to mutable variables (if accessed from multiple threads)
C++11 std library adds

<blockquote>
    <code>
    std::mutex a;
    std::lock_guard<std::mutex> guard(m);
    //automatic lock in constructor, unlock in destructor.
    </code>

</blockquote>
std::atomic<int> cnt; // for atomic, non locking primitives. (has overloaded **,--, +=(int) and int operator())

(item 16) noexcept

C++98 added exception specification; this was part of signature and would break linkage if it was changed.

C++11 aproach: function either throws exception or none at all (for these specify noexcept); with noexcept

if exception leaves function marked by noexcept (only if exceptions not disabled)

C++98: 1) unwinding stack to caller of function; 2) some actions (???); exit program.
C++11: a bit different: only unwind stack just befoe program exit (OMG); this provides better opportunities for code optimization

&lt;&lt; didn't read till the end &gt;&gt;&gt;

(Item 17) how to do setter methods?

<blockquote>
    <code>

    class Widget {
    public:
      // copy lvalue; cost: one copy
      void setName(const std::string& newName){  name = newName; }

      // copy rvalue; cost: one move
      void setName(std::string&& newName) { name = std::move(newName); }

      // PROBLEM: now two functions instead of one.

      std::string name;


    class Widget {


      // use universal references; forward: copies l-values and moves r=values;
      // problem: bloated object code, still two implementations (but setter coded once)
      template<typename T>
      void setName(T&& newName) { name = std::forward<T>(newName); }



    class Widget {

      // since std::string has a move constructor that is fast:
      // cost for rvalue: one move constructor + one move operation; (two moves)
      // cost for lvalues: one copy constructor + one move operation
      // problem: slicing problem - in case there are derived classes from std::string,
      // passing derived class inst. will do implicit type conversion to string
      // (and loose derived stuff)

      void setName(std::string newName) { name = std::move(newName); }
    </code>

</blockquote>
(item 18) emplace\_insert in stl collections

-   rvalues do a good job of getting rid of temporary values.
-   problem remains: argument type does not match the parameter type of a function.

<blockquote>
    <code>
    std::list<string> lst;
    lst.push_back("aaaa"); // "aaa" does not match std::string; type conversion creates a temporary instance.

    // this one does the same: just that no temporary is created (used 'perfect' forwarding
    // instead of objects this takes constructor arguments that will be used to construct the instance 'in place' (by placement new)
    lst.emplace_back("aaa");


    lst.emplace_front("bbb"); // the same treatment as push_front;
    lst.emplace(lst.begin(),"ccc"); // same as insert
    </code>

</blockquote>
any problems?
if collection holds shared\_ptr then shared\_ptr with custom delete function can only be created by means of this construct:

std::shared\_ptr<Gadget>( new Gadget(), customDeleter ); // where void customDeleter(Gadget \*); is the deleter.

in this case there is no wayto avoid temporary object ! (also if exceptions are used then deleter is a must: if push\_back throws out of memory exception then deleter is the only way to get rid of newly created Gadget object. (for emplace function there would be no way to delete Gadget because the smart pointer is not yet constructed when new node is allocated - so nothing there to deal with exception)

Gotcha: emplace functions can ignore explicit constructors (this kind of deferred initialization implicit type conversion even if the constructor specifies explicit).

(item 19) default implementations of default constr. / assignment op/ destr.

C++98: default constr; copy constructor; assignment operator and destructor are created inline (unless is is derived class with virtual destr -&gt; default destr is also virtual).
first base class operations are called, then by member copy is invoked. If copy constr is defined but no assignment op then default assignment op is created.

Law of three: if you declare any one of the three (copy constr; assignment op or destructor) then you need to do the other ones. justification: non default destructor indicates that resource management takes place so that member by member copy is not enough (and vice-versa)

C++11: adds default move constructor and move assignment op. these try to move each member (with std::move) if possible (if a member has no move action then it is copied by value).
Unlike copy constr; If move constructor is declared but no move assignment op, then no move assignment op is created !!! (justification: they learned from the law of three)

So default move op is created

-   no copy op declared in class
-   no move op is declared in class
-   no destructor is declared in class (important detail !!!)

Still: possible to specify that a default implementation should be created

<blockquote>
    <code>
    class Widget
    {
      ~Widget();

      // tell it to do a default move constr
      Widget(const Widget &&arg) = default;

      // and move assignment despite specified destructor
      Widget &operator=(Widget &&rhs) = default;
    </code>

</blockquote>
Catch22: a templated constructor would not prevent the compiler to put in a default move constructor/assignment op !!!

<blockquote>
    <code>
    class Widget {
      template<typename T> // copy-construct Widget
      Widget(const T& rhs); // from anything

      template<typename T> // copy-assign Widget
      Widget& operator=(const T& rhs);

    </code>

</blockquote>
.h3 smart pointers

finally there is adequate support in the standard library for smart pointers !

in C+*11: std::auto\_ptr is deprecated (because it is stupid and did move in copy operations; (did not have move semantics in c*+98)

(item 20) std::unique\_ptr this is the only owner of the resource (meaning need to delete the managed object)

-   copy of std::unique\_ptr not allowed (can't have two owners); no copy constructor from pointer to smart pointer (avoids implicit type conversion)
-   but you can transfer ownership to a different instance of the smart pointer.
-   when smart pointe is deleted and it owns something, then owned pointer is deleted too; but if you want different treatment then custom deleter can be supplied.
    (example see further down)
-   size of smart pointer without custom deleter is equal to sizeof(void\*).
    create these with: std::<Foo *>::make\_unique(); // now New any more !!!!
-   size of smart pointer with this deleter adds a second pointer data member (does delete to dispose of Foo when smart pointer obj. is destroyed)

<blockquote>
    <code>
       void deleteF(Foo *foo) { delete foo; printf("deleted foo %p\n", foo); }

       std::unique<Foo, decltype(deleteF)> ptr( new Foo(), deleteF );
    </code>

</blockquote>
- but std::unique\_ptr with deleteras lambdas without data (withoug captured variables) is also of sizeof(void\*) !!!!

<blockquote>
    <code>
       auto delF =[] (Foo *foo) { delete foo; printf("deleted foo\n",foo); }

        std::unique<foo, decltype(delF)> ptr( new Foo, delf );

    </code>

</blockquote>
(item 21) std::shared\_ptr - smart pointer with reference counting (aka shared resource management)

-   reference counter is dynamically allocated (because it is shared between instances), and is on the stack.
-   uses atomic increment/decrement to manage reference counter (for thread safety, but inerlocked exchange is slower)
-   when custom deleter is specified, pointer to deleter is placed in dynamic memory right next to the reference count (no tricks like with unique\_ptr)/.
-   shared\_ptr has a move constructor/assignment op (so moving is without increment/decrement of ref count)
-   sizeof(shardpt\_t) - two pointers; one to owned object, the other to the control block (which consists of ref count and possibly the pointer to custom deleter)
-   control block in addition to that also has a virtual function (WTF ! ); so each control block has ptr to vtable (didn't explain why that is too well)
-   idiom for constructing smart\_ptr without deleter
    auto ptr = std::make\_shared<Foo>();
-   idiom for constructing smart\_ptr with deleter
    std::smart\_ptr<Foo> ptr( new Foo(), deleterFun ); // thie way ptr is never passed to two instances of shared\_ptr -&gt; which would result to twice deletion.
-   if you don't like this rule:

<blockquote>
    <code>

        // the base class always has pointer to the controll block;
        // so it can be asked repeatedly to create smart ptr instance using the same controll block
        // by means of  std::enable_shared_from_this<Foo>::shared_from_this()

        class Foo : public std::enable_shared_from_this<Foo>
        {
        };

        Foo *ptr = new Foo();
        // both ptr1 and ptr2 will point to the same control block; so ptr is deleted only once.
        auto ptr1 = ptr->shard_from_this();
        auto ptr2 = ptr->shared_from_this();
    </code>

</blockquote>
- you can't take ownership away from shared from shared\_ptr (even if ref\_count is 1); no way to do that.

(item22) std::weak\_ptr

- weak pointer can dangle, but you have a chance to detect that.

auto autoptr1 = std::make\_shared<Foo> ()

std::weak\_ptr<Foo> weakptr( autoptr1 ); // weakptr, points to same control block as autoptr1, but does not increase ref count =&gt; weakptr can know when pointer has expired.

std::shared\_ptr<Foo> sm2 = weapktr.lock(); // if autoptr1 still lives then sm2 != nullptr; if autoptr points to expired object: sm2 == nullptr

- used to break refrence cycles (these create impossible problems with reference counting) a~~<span style="text-align:right;">b (pointer via shared pointer); b</span>~~&gt;a (pointer via weak pointer)

- example usage: a cache holds weakptrs, but returns shared\_ptrs; (what about exploding size of cache with invalid entries?)

(item 23) use std::make\_unique (to create unique smart ptr) and std::make\_shared (to create shared smart ptr) instead of doing new directly

- limitation; still can't use these function without new if you need a custom deleter (see previous items)

C++11: add std::make\_shared
C+*14: adds std::make\_unique (oversight that it was not in C11); so it is easy to add for C*+11

<blockquote>
    <code>
    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
    </code>

</blockquote>
-   it is shorter to type
-   std::make\_shared<Foo>( new Foo() ); // two allocations: one for Foo and one for control block
-   std::make\_shared<Foo>(); // both Foo and control block allocated in the same allocation (next in memory)

this does not work if Foo has class specific new/delete; (because make\_shared does placement new on its own chunk that includes both Foo and control block !)

- problem: memory is not freed when you think it is; control block has reference count and counter of outstanding weak references.
if refcount  0 then destructor of owned object (Foo) is invoked, but control block has to exists while counter of weak references is != 0 ;
  when refcount  0 and counter of weak refs is 0 then they can free the chunk that includes both the control block and the memory for Foo instance.

- better for exception safety (!@\#)

userofptr( std::shard\_ptr<Foo> ptr, int a);

userofptr( std::shard\_ptr<Foo>(new Foo), call\_another\_fun() );

possible timeline (alowed by standard): new Foo, call\_another\_func(), constructor of shared\_ptr; if call\_another\_fun throws an exception then Foo \* is left dangling and is not cleaned up.

this problem is so;ved by usage of std::make\_shared<Foo() instead

(item 24) -- i did not understand it too much ---

(Item 25) std::move and std::forward the gory details

- std::move casts unconditionally casts argument to rvalue

<blockquote>

    <code>
      std::string another_string; // global variable.

      void foo(const std::string arg )
      {
        another_string  = std::move( arg );
        // arg is cast to const std::string && ; now string has operator= that receives string && not const string &&
        // therefore assignment results in a full copy and not in a move !!!!
        // never use const argument when expecting to move them !!!
    </code>

</blockquote>
- std::forward is a conditional cast to rvalue;

<blockquote>
    <code>
      template<typename T>
      void forward(T &&param)
      {
        printf("got here\n");
        process(  std::forward<T>(param) );
      }

      Fooo f;

      forward( f );
      // called with lvalue; now std::forward does not cast;
      //forwarding an lvalue creates copies each time that the argument is forwarded.

      forward( std::move(f) );
      // called with rvalue: std::forward casts to rvalue only
      // if forward was called with rvalue !!!!
      // (this is done by clever type deduction and is explained later)
    </code>

</blockquote>
(item 26) universal references and rvalue references

bFoo && ref = new Foo(); // ref is an rvalue reference

void FooMove(Foo && rvalue); // another rvalue reference

Universal reference is a different case: caller can be rvalue or lvalue, const or non const

template<T>
void foo(t &&arg); // arg is universal reference; caller can be rvalue or lvalue, const or non const !!!

auto &&val = value; //val is also a universal reference.

!! The difference: universal reference arises when type deduction comes in. when type is specified it is just a rvalue reference. !!

However this example is an rvalue reference

<blockquote>
    <code>
    template<T>
    void foo(std::vector<T> && arg ); // rvalue reference; as type must be vector.


    template<T>
    void foo(const T && arg ); // also rvalue reference; as const T is specified.

    template<T>
    class Foo
    {
        void add(T && arg );
        // also rvalue reference: this is add of a specific template instance !
    </code>

</blockquote>
so universal reference arises when type deduction to unspecified type T in member template or auto. !!! (very specific case)

(in item 30 they will tell us that things are even more complicated ...) tss. tss!

(Item 27) use std::move on rvalue ref, std::forward on universal ref

std::forward - conditional cast of reference depending on use, makes sense only for universal references; (as rvalues are very unambiguous);

So it is clearer to do std::move on rvalue references and std::forward on universal references.

example setter:

<blockquote>
    <code>

    class Foo
    {
      template<typename T>
      void setName(T &&newName) { name = std::move(newName); }
      std::sting name;
    }
    </code>

</blockquote>
here is a bug:

auto n = get\_string\_value();
Foo foo;
foo.setName(n); // after the call n has been moved and its value is empty !!!!!! the lvalue arg should have been copied; std::forward can take care of it.

<blockquote>
    <code>
    class Foo
    {
      template<typename T>
      void setName(T &&newName) { name = std::forward<T>(newName); } // correct form; copies where l-value reference, moves where r-value reference.
      std::string name;
    }

    </code>

</blockquote>
?when is it advisable to move into the return value?

- when not: if temporary value is returned AND it is the same value as return value then compiler already allocates the temporary in place of the return value.

<blockquote>
    <code>
    Foo makeFoo()
    {
       Foo a;

       return a; // compiler does no copy; this optimisation is called copy ellision.
    }


    Matrix operator+(Matrix&& lhs, const Matrix& rhs)
    {
      lhs += rhs;
      return std::forward<Matrix>(lhs); // here move makes sense (no RVO takes place); lhs is argument.
    }
    </code>

</blockquote>
(item 28) when to avoid universal references

there are problems if the sane function is overloaded on universal ref and different argument.

<blockquote>
    <code>

    template<typename T>
    void logAndAdd(T&& name)
    {
        log(now, "logAndAdd");
        names.emplace(std::forward<T>(name));
    }

    void logAndAdd(int idx)
    {
        auto now = std::chrono::system_clock::now();
        log(now, "logAndAdd");
        names.emplace(nameFromIdx(idx)); // nameFromIdx returns std::string
    }
    </code>

</blockquote>
Now calling logAndIndex with int is ok; but with ushort argument it will go to the first version (universal ref matches better - without type conversion from uint to int)

&lt;&lt;&lt;&lt;&lt;&lt;&gt;&gt;&gt;&gt;&gt;&gt;&gt;
it does not help to add a class that does conversion

<blockquote>
    <code>
    class Person {
    public:
       template<typename T> // this does not prevent the compiler from adding default move ctor !!!
       explicit Person(T&& n);

       explicit Person(int idx);
    }
    </code>

</blockquote>
(item 29) how to fix the previous problem.

- no overloading (have logAndAdd and logAndAddIdx) - but Scott Meyers does not like it. (probably because he has some more pages to fill)

- avoid universal ref. do only const references (not quite as efficient)

<blockquote>
    <code>
       template<typename T>
       void logAndAdd(const T &ref);;

       void logAndAdd(int idx);
    </code>

</blockquote>
- tag dispatch (aka. total brainfuck)

<blockquote>
    <code>

    template<typename T>
    void logAndAdd(T&& name)
    {
        logAndAddImpl(std::forward<T>(name),
            std::is_integral<typename std::remove_reference<T>::type>()
    }

    template<typename T>
    void logAndAddImpl(T&& name, std::false_type)  // non-integral types of logAndAdd
    {
      auto now = std::chrono::system_clock::now();
      log(now, "logAndAdd"); // structure
      names.emplace_back(std::forward<T>(name));
    }

    void logAndAddImpl(int idx, std::true_type) // for integral types of logAndAdd
    {
       logAndAdd(anyFromIdx(idx)); // up name and
    }
    </code>

</blockquote>
The same shit can be done to constructors, but what the fuck ??

(Item 30) std::forward explained by reference collapsing

- you can't take reference of reference;

<blockquote>
    <code>
      int x;
      auto & & R = x; // will not compile.
    </code>

</blockquote>
- but references to references can appear in type deduction

template<typename T>
void func(T&& param);

if T is Foo & then this substitution turns out as

void func(Foo & && param); // reference to reference

- they have rules for shortening this - known as 'reference collapsing'

\* if either ref is an lvalue (& && or && &) -&gt; result collapsed into lvalue
\* else (both refs are rvalue) -&gt; result collapsed into rvalue

- how does std::forward look like?

<blockquote>
    <code>

    template<typename T>
    T&& forward(T&& param)
    {
        return static_cast<T&&>(param);
    }
    </code>

</blockquote>
How does this work?

if T is Foo & (l-value ref)

<blockquote>
    <code>
    Foo& && forward(Foo& &&param)
    {
        return static_cast<Foo& &&>(param);
    }

    // & && -> & (by rule of reference collapsing so result is

    Foo& forward(Foo& param)
    {
        return static_cast<Foo&>(param);
    }
    </code>

</blockquote>
if T is Foo && (r-value ref)

<blockquote>
    <code>
    Foo&& && forward(Foo&& &&param)
    {
        return static_cast<Foo&& &&>(param);
    }

    && && -> && (by rule of reference collapsing so result is

    Foo&& forward(Foo&& param)
    {
        return static_cast<Foo&&>(param);
    }
    </code>

</blockquote>
&lt;&lt;&lt;<blablabla>&gt;&gt;&gt;&gt;&gt;&gt;

(Item 31) limits of moving

- move not always available (it is for std library classes)

- move not always efficient;
std::array is a static array; move copies the content of the array (same as copy)
some strings do 'small string optimization' short strings are kept in the string object and not managed on heap with reference counting, so here move for small strings is copy.

- exception guarantees: if move is not marked as no\_except so compiler may have to copy for move in order to preserve exception semantics (???)

- be aware that move is only relevant for r-values (check that assumption did not change during development ;-)

(Item 32) problems with perfect forwarding

for one argument:

<blockquote>
    <code>
    template<typename T>
    void fwd(T && arg)  // uses universal ref; only here is origin type characteristics (const, ref type) of parameters deducible to forward
    {
        f(std::forward<T> (arg) ); // f is now called with calling type (T & forwarded to T &; T&& forwarded to T &&, etc)
    }
    </code>

</blockquote>
for many arguments.

<blockquote>
    <code>
    template<typename ... T > // ... - ellipsis (variadic template = repeat the same for several types)
    void fwd(T &&arg... arg )
    {
        f(std::forward<T>(arg) ... );
    }
    </code>

</blockquote>
forwarding fails if fwd(<expression>) is not compatible with f(<expression>)

- 0 for null pointer; type deduction find integer type, this is not forwarded to pointer; use nullptr as argument (unambiguous) and it works

void f(void **); fwd(0); // T deduced as int; can't be forwarded to void**.

- void f(std::list<int> arg);
fwd({1, 2, 3}); // ambiguous: converts to std::initializer\_list only if fwd would be known to have signature fwd(std::initializer\_list<int> arg); this is not the case; otherwise would be parsed as f(1,2,3)

workaround:

auto a = {1,2,3}; // a is std::initializer\_list - unambiguous
f(a); // no compilation errors.

- static const

static const std::size Maxsize = 42; // compiler turns this into constant without memory location.

forwarding takes a reference to this (meaning reference to location that is not defined -&gt; linker errors on **some** (?weasel?) compilers

- void f(void (\*fptr) (int) );// f receives function pointer to function with one int argument

void print(int ); // \#1
void print(char \*,int); // \#2

f(print); // can deduce type of f so it picks the right function - \#1;

forwarding of print to fwd creates ambiguous situation -&gt; fails. (explicit cast to correct argument type helps).

- forwarding of bitfield members is verboten.

.h3 Lambda functions / (also known as anonymous functions)

- one line functions can make stl algorithms usable

<blockquote>
    <code>
       std::find_if( v.begin(), v.end(),
        [] (auto val) { return 0 < val && val < 10; } // this is the lambda expression.
         );
    </code>

</blockquote>
- closure class := each lambda expression generates a class that holds the lambda expression as member function. Note: captured variables are data members of the closure class.
- closure := instance of a closure class

can copy closures (aka instances of closure class)

<blockquote>
    <code>

       int x = 42;
       auto c1 = [x] (int y) { return x + y; } // c1 is closure instance ( 'captures' the value of x as data member)

    </code>

</blockquote>
(item 33) default capture mode is evil.

! beware of implicit behavior; closures are good for use in STL containers (used in the same scope as captured data); !
! if you store a lambda then it can point to data that got out of scope (very very bad) !

what to do with non local variables? I found this item very confusing, so i wrote a sample program that makes things clearer.

Different capture modes:

<blockquote>
    <code>

    int aaa;
    auto e = [aaa] (int c) { return c * val_; };
    // must specify captured variables by name in []; copied by value; can only visible locals.


    auto e = [=] (int c) { return c * val_; };
    // copy by value (also ptrs copied by value); can access data member of enclosing class + locals in enclosing scope

    auto e = [&] (int c) { return c * val_; };
    // copy by ref; can access data member of enclosing class + locals in enclosing scope.

    </code>

</blockquote>
! static variables can be used inside lambda's but these are not 'captured' (not copied by value or reference into closure) !

------------------------------------------------------------------------

<blockquote>
    <code>
    <pre><code>
    #include <stdio.h>

    class Foo
    {
    public:

      Foo() : val_(42) {}

      int doo(int arg)
      {
        printf("*** capture of class members ***\n");

        Foo *f = this;
        int vall = 42;

        printf("val_ptr=%p\n", &val_);

        // does not compile; can capture only locals.
        //auto e = [val_] (int c) { return c * val_; };

        // reference of vall_ copied; capture points to class member
        auto c = [=] (int c) { printf("v=%p\n", &val_);  return c * val_; }; //f-> val_; };
        printf("res=%d\n", c( arg ) );

        // reverence to this->vall_ copied; capture points to class member
        auto d = [&] (int c) { printf("v=%p\n", &val_); return c * val_; };

        printf("res=%d\n", d( arg ) );
      }


      int doo_local(int arg)
      {
        printf("*** capture of local variables  ***\n");

        Foo *f = this;
        int vall = 42;
        int *vptr = &vall;

        printf("val_ptr=%p\n", &vall);

        //auto e = [] (int c) { return c * vall; }; 
        // does not compile, must specify the imported variable

        // value vall is copied
        auto e = [vall] (int c) { printf("v=%p\n", &vall); return c * vall; };
        printf("res=%d\n", e( arg ) );

        // pointer is copied by value (capture refers to stack locationa)
        auto h = [vptr] (int c) { printf("v=%p\n", vptr); return c * (*vptr); };
        printf("res=%d\n", h( arg ) );

        // vall is copied by value (capture has its own copy)
        auto c = [=] (int c) { printf("v=%p\n", &vall);  return c * vall; };
        printf("res=%d\n", c( arg ) );

        // pointer is copied by value (capture points to stack location     )
        auto g = [=] (int c) { printf("v=%p\n", vptr);  return c * (*vptr); };
        printf("res=%d\n", g( arg ) );

        // vall copied by reference (capture points to stack location)
        auto d = [&] (int c) { printf("v=%p\n", &vall); return c * vall; };
        printf("res=%d\n", d( arg ) );
      }
    </code>

</code>

</pre>
</blockquote>
(item 34) init capture in C++14

-   c++11 no way to move stuff into closure (either copy by val or copy by ref)
-   C++14 has more flexible mechanism for specifying capture: 'init captures'

<blockquote>
    <code>

    auto pw = std::make_unique<Foo>();

    auto func = [ pw = std::move(pw) ]  // init capture has expression to initalize captured variables (here move unique_ptr into capture)
            { return pw->foo(); }x
    </code>

</blockquote>
- trick to do emulate this in C++11

<blockquote>
    <code>

    auto pw = std::make_unique<Foo>();

    auto pw = std::make_unique<Foo>();
        std::bind( // C++11 emulation
            [](std::unique_ptr<Foo> & data) // of init capture
            { return pw->foo() },

            std::move(data)
        ) ;
    </code>

</blockquote>
-   std::bind returns function object, first part is the 'callable' (lambda); second part argument to be evaluated on call.
-   so when func is called, it copies the vector<double> data from outer scope into captured variable data (it is a member of the closure instance)
-   works because closure has the same lifetime as bind object.

(item 35) '

C++14 has lambdas with auto parameters

auto f = \[\](auto x){ return func(normalize(x)); };

if you want to use forwarding for nromalize

auto f = \[\](auto x){ return func( normalize( std::forward<decltype(x)>( x ) )); }; // type argument to std::forward deduced by decltype

&lt;&lt;&lt;&lt; remaining items are about the concurrency API, i don't think that this is such a stellar thing &gt;&gt;&gt;&gt;&gt;