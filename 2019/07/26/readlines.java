import java.util.*;
import java.io.*;
import java.util.stream.*; 
import java.nio.file.*;

public class readlines {
    long volume = 0;

    public readlines() {
    }


    public void parseLine(String s) {
        volume += s.length();
    }

    public static String scanFile(String location)  throws IOException {
        return new String(Files.readAllBytes(FileSystems.getDefault().getPath(location)));
    }

    public void readString(String data) throws IOException {				
         StringReader fr = new StringReader(data);
         BufferedReader bf = new BufferedReader(fr);
         bf.lines().forEach(s -> parseLine(s));
         bf.close();
    }

    public static void main(String[] args)  throws IOException  {
      long bef0 = System.currentTimeMillis();
      byte[] b = Files.readAllBytes(FileSystems.getDefault().getPath(args[0])); 
      long aft0 = System.currentTimeMillis();
      String data = scanFile(args[0]);
      System.out.format("init speed: %.3f GB/s\n",b.length/(1024.0*1024.0*1024.0)/((aft0-bef0)/1000.0));
      for(int k = 0; k < 19; k++) {
          readlines d = new readlines();
          long bef = System.currentTimeMillis();
          d.readString(data);
          long aft = System.currentTimeMillis();
          long fz = new File(args[0]).length();
           // we need the leave time for the JIT
           System.out.format("speed: %.3f GB/s\n",fz/(1024.0*1024.0*1024.0)/((aft-bef)/1000.0));
        }
    }

}

