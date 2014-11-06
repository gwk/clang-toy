# source: http://llvm.org/releases/3.5.0/docs/tutorial/LangImpl4.html

def test(x) (1+2+x)*(x+(1+2));

4+5;

def testfunc(x y) x + y*2;

testfunc(4, 10);

##

extern sin(x);
extern cos(x);
sin(1.0);
def foo(x) sin(x)*sin(x) + cos(x)*cos(x);
foo(4.0);
