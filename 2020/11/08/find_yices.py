from yices import *
import sys
if sys.version_info < (3, 0):
    sys.stdout.write("Sorry, requires Python 3.x or better\n")
    sys.exit(1)
# pip3 install yices
# brew install SRI-CSL/sri-csl/yices2 see https://yices.csl.sri.com
import random
counter = 0
total = 1000
cfg = Config()
cfg.default_config_for_logic('QF_BV')
bv64_t = Types.bv_type(64)
for i in range(total):
  ctx = Context(cfg)
  a = Terms.new_uninterpreted_term(bv64_t, 'a')
  c = Terms.bvconst_integer(64,i)
  t = Terms.bvconst_integer(64,1)
  bigproduct = Terms.bvmul(a , c)
  fmla = Terms.bveq_atom(bigproduct,t)
  ctx.assert_formulas([fmla])
  status = ctx.check_context()
  if((i % 100)==0) :
      print(".",end="",flush=True)
  if status == Status.SAT:
    counter = counter + 1

print(counter*100./total)