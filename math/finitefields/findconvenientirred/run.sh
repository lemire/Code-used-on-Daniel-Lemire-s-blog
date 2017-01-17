echo "generating all polynomials"
python allpoly.py > allpoly.txt
echo "factorizing them"
maxima -b allpoly.txt > allfactors.txt
echo "looking for irreducible ones"
python lookforirred.py
