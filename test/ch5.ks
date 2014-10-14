# source: http://llvm.org/releases/3.5.0/docs/tutorial/LangImpl5.html

def fib(x)
  if x < 3 then
    1
  else
    fib(x-1)+fib(x-2);

## 5.3. ‘for’ Loop Expression

extern putchard(char)
def printstar(n)
  for i = 1, i < n, 1.0 in
    putchard(42);  # ascii 42 = '*'

# print 100 '*' characters
printstar(100);

putchard(10);
