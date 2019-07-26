import java.util.*;
import java.io.*;
import java.util.stream.*; 

public class oldschoolreadlines {
    long volume = 0;

    public oldschoolreadlines() {
    }


    public void parseLine(String s) {
        volume += s.length();
    }

    public static StringBuffer scanFile(String location)  throws IOException {
        FileReader fr = new FileReader(location);
        BufferedReader bf = new BufferedReader(fr);
        StringBuffer sb = new StringBuffer();
        bf.lines().forEach(s -> sb.append(s));
        bf.close();
        return sb;
    }

    public void readString(String data) throws IOException {				
         StringReader fr = new StringReader(data);
         BufferedReader bf = new BufferedReader(fr);
         String line;
         while((line = bf.readLine()) != null) {
            volume += line.length();
         }
         bf.close();
    }

    public static void main(String[] args)  throws IOException  {
        String data = scanFile(args[0]).toString();
        for(int k = 0; k < 10; k++) {
          oldschoolreadlines d = new oldschoolreadlines();
          long bef = System.currentTimeMillis();
          d.readString(data);
          long aft = System.currentTimeMillis();
          long fz = new File(args[0]).length();
           // we need the leave time for the JIT
          if(k >= 7)
            System.out.format("speed: %.3f GB/s\n",fz/(1024.0*1024.0*1024.0)/((aft-bef)/1000.0));
        }
    }

}

