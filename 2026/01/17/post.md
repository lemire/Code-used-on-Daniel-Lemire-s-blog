Irrespective of your programming language of choice, calling C functions is often a necessity. For the longest time, the only standard way to call C was the Java Native Interface (JNI). But it was so painful that few dared to do it. I have heard it said that it was deliberately painful so that people would be enticed to use pure Java as much as possible.

Since Java 22, there is a new approach called the Foreign Function & Memory API in `java.lang.foreign`. Let me go through step by step. 

You need a `Linker` and a `SymbolLookup` instance from which you will build a
`MethodHandle` that will capture the native function you want to call.

The linker is easy:

```java
Linker linker = Linker.nativeLinker();
```

To load the  `SymbolLookup` instance for your library (called `mylibrary`), you may do so as follows:
```java
System.loadLibrary("mylibrary");
SymbolLookup lookup = SymbolLookup.loaderLookup();
```

The native library file should be on your `java.library.path` path, or somewhere on the default library paths. (You can pass it to your java executable as `-Djava.library.path=something`).

Alternatively, you can use `SymbolLookup.libraryLookup` or other means of loading
the library, but `System.loadLibrary` should work well enough.

You have the lookup, you can grab the address of a function like so:

```java
lookup.find("myfunction")
```

This returns an `Optional<MemorySegment>`. You can grab the `MemorySegment` 
like so:
```java
MemorySegment mem = lookup.find("myfunction").orElseThrow()
```

Once you have your `MemorySegment`, you can pass it to your `linker` to get a `MethodHandle` which is close to a callable function:

```java
 MethodHandle myfunc = linker.downcallHandle(
     mem,
     functiondescr
 );
 ```

The `functiondescr` must describe the returned value and the function parameters that your function takes.
If you pass a pointer and get back a `long` value, you might proceed as follows: 

```java
 MethodHandle myfunc = linker.downcallHandle(
     mem,
     FunctionDescriptor.of(
        ValueLayout.JAVA_LONG,
        ValueLayout.ADDRESS
    )
 );
 ```

That is, the first parameter is the returned value.

For function returning nothing, you use `FunctionDescriptor.ofVoid`.

The `MethodHandle` can be called almost like a normal Java function:
`myfunc.invokeExact(parameters)`. It always returns an `Object` which means
that if it should return a `long`, it will return a `Long`.

You can allocate C data structures from Java that you can pass to your native code by using an `Arena`. Let us say that
you want to create an instance like

```java
MemoryLayout mystruct = MemoryLayout.structLayout(
        ValueLayout.JAVA_LONG.withName("age"),
        ValueLayout.JAVA_INT.withName("friends"));
```

You could do it in this manner:

```java
MemorySegment myseg = arena.allocate(mystruct);
```

You can then pass `myseg` as a pointer to a data structure in C.

You often get an array with a `try` clause like so:

```java
try (Arena arena = Arena.ofConfined()) {
       //
}
```

There are many types of arenas: confined, global, automatic, shared. The confined arenas are accessible from a single thread. A shared or global arena is accessible from several threads. The global and automatic arenas are managed by the Java garbage collector whereas the confined and shared arenas are managed explicitly, with a specific lifetime.

So, it is fairly complicated but manageable. Is it fast? To find out, I call from Java a C library I wrote with support for binary fuse filters. They are a fast alternative to Bloom filters. You don't need to know what any of this means, however.
Keep in mind that I wrote a Java library called [jfusebin](https://github.com/FastFilter/jfusebin) which calls a C library. Then I also have a pure [Java implementation](https://github.com/FastFilter/fastfilter_java) and I can compare the speed.

I should first point out that even if calling the C function did not include any overhead, it might still be slower because the Java compiler is unlikely to inline a native function. However, if you have a pure Java function, and it is relatively small, it can get inlined and you get all sorts of nice optimizations like constant folding and so forth.

Thus I can overestimate the cost of the overhead. But that's ok. I just want a ballpark measure.

In my benchmark, I check for the presence of a key in a set. I have one million keys
in the filter. I can ask whether a key is not present in the filter.

I find that the library calling C can issue 44 million calls per second using the 8-bit binary fuse filter. I reach about 400 million calls per second using the pure Java implementation.

| method | time per query in nanoseconds |
|--------|----------------|
| Java-to-C | 22.7 ns |
| Pure Java | 2.5 ns |


Thus I measure an overhead of about 20 ns per C function calls from Java using a macBook (M4 processor).

Obviously, in my case, because the Java library is so fast, the 20 ns becomes too much. But it is otherwise a reasonable overhead.
