/**
* By D. Lemire.
* See
* Use random hashing if you care about security?
* http://lemire.me/blog/archives/2012/01/17/use-random-hashing-if-you-care-about-security/
*/

import java.util.*;
class test {
	public static ArrayList<String> listofstrings(int lengthdividedby3) {
		String[] allcollide = { "Ace","BDe","AdF","BEF"};
		ArrayList<String> al = new ArrayList<String>();
		al.add("");
		if(lengthdividedby3 <= 0 ) return al;
		int mask = 0xFFFF;
		for(int k = 0; k<lengthdividedby3;++k) {
			ArrayList<String> oldarray = al;
			al = new ArrayList<String>();
			for(String s : oldarray)
			  for(String t : allcollide)
			    al.add(s+t);
		}
		System.out.println("generated "+al.size()+" ASCII strings of length "+al.get(0).length());
		int expectedhashcode = al.get(0).hashCode() & mask;
		for (String t: al) 
		  if((t.hashCode() & mask)!= expectedhashcode) throw new RuntimeException("Got it wrong!");
		System.out.println("... and they all collide (on first 16 bits)");		  
		return al;
	}
	static void testme(int lengthdividedby3) {
	    long bef, aft;
	    double average, worse;
	    ArrayList<String> x = listofstrings(lengthdividedby3);
		{
  		  System.out.println("testing how long it usually takes to add "+x.size()+" elements to a hash table");

		  bef = System.currentTimeMillis();
		  Hashtable<String,Integer> testhash = new Hashtable<String,Integer>();
		  for(int k = 0; k< x.size();++k) 
		    testhash.put(Integer.toString(k),1);
		  aft = System.currentTimeMillis();
		  average = (aft-bef)/1000.0;	
		  System.out.println("about "+average+" s");
		}
		{	
  		  System.out.println("testing how long it usually takes to add "+x.size()+" elements to a hash table in our cooked up case");

			// now watch the hash table suffer!
		  bef = System.currentTimeMillis();
			Hashtable<String,Integer> myhash = new Hashtable<String,Integer>();
			for(String s : x)
		  		myhash.put(s,1);
		  aft = System.currentTimeMillis();
		  worse = 	(aft-bef)/1000.0;
		  System.out.println("about "+worse+" s");
		}
		System.out.println("ratio = "+worse/average);		
		{	
  		  System.out.println("testing how long it usually takes to add "+x.size()+" elements to a tree in our cooked up case");

			// now watch the hash table suffer!
		  bef = System.currentTimeMillis();
			TreeMap<String,Integer> myhash = new TreeMap<String,Integer>();
			for(String s : x)
		  		myhash.put(s,1);
		  aft = System.currentTimeMillis();
		  double treetime = 	(aft-bef)/1000.0;
		  System.out.println("about "+treetime+" s");
		}
		System.out.println();
	}
	static public void main(String[] args) {
		testme(6);
		testme(7);
		testme(8);

		testme(6);
		testme(7);
		testme(8);

	}
}