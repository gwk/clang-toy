# source: http://llvm.org/releases/3.5.0/docs/tutorial/LangImpl3.html

This code does have a bug, though. Since the PrototypeAST::Codegen can return a previously defined forward declaration, our code can actually delete a forward declaration. There are a number of ways to fix this bug, see what you can come up with! Here is a testcase:

extern foo(a b);     # ok, defines foo.
def foo(a b) c;      # error, 'c' is invalid.
def bar() foo(1, 2); # error, unknown function "foo"

## 3.5. Driver Changes and Closing Thoughts

4+5;

def foo(a b) a*a + 2*a*b + b*b;

def bar(a) foo(a, 4.0) + bar(31337);

extern cos(x);
cos(1.234);
