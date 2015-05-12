/**
* This little experiment shows that a naive implementation of
* Polya's urn (NaiveUrn) is fine up until about a few millions picks when using 1000 colors.
* For more than 100 million picks, you are better with a constant-memory approach (ConstantUrn).
* Exact results will depend on your hardware.
*
* Note that if you use fewer colors, then the constant-time approach will be relatively faster.
*/

import java.util.*;

public class polyaurn {
    static int N = 100*1000*1000;//*1000;
    static int concepts = 1000;//*1000;

    static void test(List<String> t) {
        long t1 = System.currentTimeMillis();
        NaiveUrn u = new NaiveUrn(t);
        for(int j = 0; j < N; ++j) {
            u.pick();
        }
        System.out.println("ignore:"+u.size());
        long t2 = System.currentTimeMillis();
        ConstantUrn cu = new ConstantUrn(t);
        for(int j = 0; j < N; ++j) {
            cu.pick();
        }
        System.out.println("ignore:"+cu.size());
        long t3 = System.currentTimeMillis();
        System.out.println("naive time: "+(t2-t1)+", fancier time: "+(t3-t2));
    }


    public static void main(String[] args) {
        ArrayList<String> t = new ArrayList<String>();
        for(int i = 0; i < concepts; ++i) {
            t.add(Integer.toString(i));
        }
        System.out.println("calling Polya's urn "+N+" times over "+concepts+" colors");
        for(int k = 0; k< 10 ; ++k ) test(t);

    }

}


// simple Polya's urn with ever increasing memory usage
class NaiveUrn {
    ArrayList<String> back = new ArrayList<String>();
    Random rand = new Random();
    public NaiveUrn(List<String> inputs) {
        back.addAll(inputs);
    }

    String pick() {
        int r = rand.nextInt(back.size());
        String answer = back.get(r);
        back.add(answer);
        return answer;
    }

    int size() {
        return back.size();
    }
}


// simple Polya's urn with constant memory usage
class ConstantUrn {
    final String[] back;
    final int [] counters;
    int N = 0;
    Random rand = new Random();
    public ConstantUrn(List<String> inputs) {
        back = new String[inputs.size()];
        counters = new int[inputs.size()];
        for(int i = 0; i < inputs.size(); ++i) {
            back[i] = inputs.get(i);
            counters[i] = 1;
        }
        N = inputs.size();
    }

    String pick() {
        int r = rand.nextInt(N) ;
        int i = 0;
        for(; i < back.length; ++i) {
            r -= counters[i];
            if(r<0) break;
        }
        N++;
        counters[i] ++;
        return back[i];
    }

    int size() {
        return N;
    }
}
