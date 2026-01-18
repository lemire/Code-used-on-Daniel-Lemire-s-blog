import java.lang.foreign.*;
import java.lang.invoke.MethodHandle;
import java.lang.invoke.MethodHandles;
import java.lang.invoke.MethodType;
import java.nio.file.Path;
import java.nio.file.Paths;

public class IntArrayExample {

    public static void main(String[] args) throws Throwable {
        // Load the native library
        System.loadLibrary("sum");

        Linker linker = Linker.nativeLinker();

        // Look up the symbol from loaded libraries
        SymbolLookup lookup = SymbolLookup.loaderLookup();
        MemorySegment sumAddress = lookup.find("sum_array").get();

        // C Signature: int sum_array(int* data, int count)
        MethodHandle sumArray = linker.downcallHandle(
            sumAddress,
            FunctionDescriptor.of(ValueLayout.JAVA_INT, ValueLayout.ADDRESS, ValueLayout.JAVA_INT),
            Linker.Option.critical(true)
        );

        int[] javaArray = {10, 20, 30, 40, 50};

        try (Arena arena = Arena.ofConfined()) {
            MemorySegment heapSegment = MemorySegment.ofArray(javaArray);
            int result = (int) sumArray.invoke(heapSegment, javaArray.length);
            System.out.println("The sum from C is: " + result);
        }
    }
}