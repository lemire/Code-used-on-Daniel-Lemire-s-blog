try:
    import z3
except ImportError:
    from pip._internal import main as pip
    pip(['install', '--user', 'z3-solver'])
    import z3

y = z3.BitVec("y", 32)
s = z3.Solver()
s.add( ( 1 + y ) / 2 >= y )
if(s.check() == z3.sat):
    model = s.model()
    print(model)


y = z3.BitVec("y", 32)
s = z3.Solver()
s.add( ( 1 + y ) / 2 >= y )
s.add( y > 0 )

if(s.check() == z3.sat):
    model = s.model()
    print(model)


s = z3.Solver()
s.add( ( 1 + y ) / 2 >= y )
s.add( y > 1 )

if(s.check() == z3.sat):
    model = s.model()
    print(model)

s = z3.Solver()
s.add( ( 1 + y ) >= y * 2 )
s.add( y > 1 )

if(s.check() == z3.sat):
    model = s.model()
    print(model)


s = z3.Solver()
s.add( ( 1 + y ) >= y * 2 )
s.add( y > 1 )
s.add( y <  2147483647/2)

if(s.check() == z3.sat):
    model = s.model()
    print(model)
