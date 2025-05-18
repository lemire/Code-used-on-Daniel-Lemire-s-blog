Many programming languages such as the Go programming language are designed to make it easy
to return several values at once from a function. In Go, it is often used to return an optional
error code. The C++ programming language does not have a built-in support for returning several
values. However, several standard types can serve the same purpose. If you need to return two
values, you can use an std::pair instance. If you need to return two or more values, an std::tuple
instance will do. With recent C++ standards, it works really well!

Suppose we want to compute a division with a string error message when one is trying to divided by zero:

<pre class="code_syntax" style="color:#000000;background:#ffffff;"><span class="line_wrapper">std<span style="color:#800080; ">::</span>tuple<span style="color:#808030; ">&lt;</span><span style="color:#800000; font-weight:bold; ">int</span><span style="color:#808030; ">,</span>std<span style="color:#800080; ">::</span>string<span style="color:#808030; ">&gt;</span> divide<span style="color:#808030; ">(</span><span style="color:#800000; font-weight:bold; ">int</span> a<span style="color:#808030; ">,</span> <span style="color:#800000; font-weight:bold; ">int</span> b<span style="color:#808030; ">)</span> <span style="color:#800080; ">{</span></span>
<span class="line_wrapper">    <span style="color:#800000; font-weight:bold; ">if</span> <span style="color:#808030; ">(</span>b <span style="color:#808030; ">=</span><span style="color:#808030; ">=</span> <span style="color:#008c00; ">0</span><span style="color:#808030; ">)</span> <span style="color:#800080; ">{</span></span>
<span class="line_wrapper">        <span style="color:#800000; font-weight:bold; ">return</span> <span style="color:#800080; ">{</span><span style="color:#008c00; ">0</span><span style="color:#808030; ">,</span> <span style="color:#800000; ">"</span><span style="color:#0000e6; ">Error: Division by zero</span><span style="color:#800000; ">"</span><span style="color:#800080; ">}</span><span style="color:#800080; ">;</span></span>
<span class="line_wrapper">    <span style="color:#800080; ">}</span></span>
<span class="line_wrapper">    <span style="color:#800000; font-weight:bold; ">return</span> <span style="color:#800080; ">{</span>a <span style="color:#808030; ">/</span> b<span style="color:#808030; ">,</span> <span style="color:#800000; ">"</span><span style="color:#0000e6; ">Success</span><span style="color:#800000; ">"</span><span style="color:#800080; ">}</span><span style="color:#800080; ">;</span></span>
<span class="line_wrapper"><span style="color:#800080; ">}</span></span>
<span class="line_wrapper"></span></pre>

This approach works nicely. The code is clear and readable. 

You might be concerned that we are fixing the type (int). If you want to write one function for all integer types, you can do so with concepts, like so:

<pre class="code_syntax" style="color:#000000;background:#ffffff;"><span class="line_wrapper"><span style="color:#004a43; ">#</span><span style="color:#004a43; ">include </span><span style="color:#800000; ">&lt;</span><span style="color:#40015a; ">concepts</span><span style="color:#800000; ">&gt;</span></span>
<span class="line_wrapper"></span>
<span class="line_wrapper"></span>
<span class="line_wrapper"><span style="color:#800000; font-weight:bold; ">template</span> <span style="color:#800080; ">&lt;</span><span style="color:#666616; ">std</span><span style="color:#800080; ">::</span>integral int_type<span style="color:#800080; ">&gt;</span></span>
<span class="line_wrapper"><span style="color:#666616; ">std</span><span style="color:#800080; ">::</span>tuple<span style="color:#800080; ">&lt;</span>int_type<span style="color:#808030; ">,</span> <span style="color:#666616; ">std</span><span style="color:#800080; ">::</span><span style="color:#603000; ">string</span><span style="color:#800080; ">&gt;</span> divide<span style="color:#808030; ">(</span>int_type a<span style="color:#808030; ">,</span> int_type b<span style="color:#808030; ">)</span> <span style="color:#800080; ">{</span></span>
<span class="line_wrapper">    <span style="color:#800000; font-weight:bold; ">if</span> <span style="color:#808030; ">(</span>b <span style="color:#808030; ">=</span><span style="color:#808030; ">=</span> <span style="color:#008c00; ">0</span><span style="color:#808030; ">)</span> <span style="color:#800080; ">{</span></span>
<span class="line_wrapper">        <span style="color:#800000; font-weight:bold; ">return</span> <span style="color:#800080; ">{</span><span style="color:#008c00; ">0</span><span style="color:#808030; ">,</span> <span style="color:#800000; ">"</span><span style="color:#0000e6; ">Error: Division by zero</span><span style="color:#800000; ">"</span><span style="color:#800080; ">}</span><span style="color:#800080; ">;</span></span>
<span class="line_wrapper">    <span style="color:#800080; ">}</span></span>
<span class="line_wrapper">    <span style="color:#800000; font-weight:bold; ">return</span> <span style="color:#800080; ">{</span>a <span style="color:#808030; ">/</span> b<span style="color:#808030; ">,</span> <span style="color:#800000; ">"</span><span style="color:#0000e6; ">Success</span><span style="color:#800000; ">"</span><span style="color:#800080; ">}</span><span style="color:#800080; ">;</span></span>
<span class="line_wrapper"><span style="color:#800080; ">}</span></span></pre>

With this definition, you can call divide(10,3), but if you try to call divide(10.0, 3.1323), you will get an error
message such as "'double' does not satisfy 'integral'".

So far so good.


Prior to C++17, there was no elegant way to call this function and immediately capture the values without boilerplate code. Thankfully, we now have structured bindings:

<pre class="code_syntax" style="color:#000000;background:#ffffff;"><span class="line_wrapper"><span style="color:#800000; font-weight:bold; ">auto</span> <span style="color:#808030; ">[</span>result<span style="color:#808030; ">,</span> message<span style="color:#808030; ">]</span> <span style="color:#808030; ">=</span> divide<span style="color:#808030; ">(</span><span style="color:#008c00; ">10</span><span style="color:#808030; ">,</span> <span style="color:#008c00; ">2</span><span style="color:#808030; ">)</span><span style="color:#800080; ">;</span></span>
<span class="line_wrapper">std<span style="color:#800080; ">::</span>println<span style="color:#808030; ">(</span><span style="color:#800000; ">"</span><span style="color:#0000e6; ">Result: {}, Message: {}</span><span style="color:#800000; ">"</span><span style="color:#808030; ">,</span> result<span style="color:#808030; ">,</span> message<span style="color:#808030; ">)</span><span style="color:#800080; ">;</span></span></pre>

As a bonus, in this example, I am using C++23's std::println function which is available with GCC 14 and LLVM 19.


The structured binding approach (the keyword 'auto' followed by brackets) is nice, but what if you care about only one of the return values ? In C++23, you can use the underscore to indicate that you do not care about a value:

<pre class="code_syntax" style="color:#000000;background:#ffffff;"><span class="line_wrapper"><span style="color:#800000; font-weight:bold; ">auto</span> <span style="color:#808030; ">[</span>quotient<span style="color:#808030; ">,</span> _<span style="color:#808030; ">]</span> <span style="color:#808030; ">=</span> divide<span style="color:#808030; ">(</span><span style="color:#008c00; ">15</span><span style="color:#808030; ">,</span> <span style="color:#008c00; ">3</span><span style="color:#808030; ">)</span><span style="color:#800080; ">;</span></span>
<span class="line_wrapper">std<span style="color:#800080; ">::</span>println<span style="color:#808030; ">(</span><span style="color:#800000; ">"</span><span style="color:#0000e6; ">Quotient only: {}</span><span style="color:#800000; ">"</span><span style="color:#808030; ">,</span> quotient<span style="color:#808030; ">)</span><span style="color:#800080; ">;</span></span></pre>

What if you have existing variables, and you do not want to create new variables? You can use std::tie:

<pre class="code_syntax" style="color:#000000;background:#ffffff;"><span class="line_wrapper">std<span style="color:#800080; ">::</span>string msg <span style="color:#808030; ">=</span> <span style="color:#800000; ">"</span><span style="color:#0000e6; ">Hello</span><span style="color:#800000; ">"</span><span style="color:#800080; ">;</span></span>
<span class="line_wrapper"><span style="color:#800000; font-weight:bold; ">int</span> i <span style="color:#808030; ">=</span> <span style="color:#008c00; ">42</span><span style="color:#800080; ">;</span></span>
<span class="line_wrapper">std<span style="color:#800080; ">::</span>tie<span style="color:#808030; ">(</span>i<span style="color:#808030; ">,</span>msg<span style="color:#808030; ">)</span> <span style="color:#808030; ">=</span> divide<span style="color:#808030; ">(</span><span style="color:#008c00; ">3</span><span style="color:#808030; ">,</span> <span style="color:#008c00; ">0</span><span style="color:#808030; ">)</span><span style="color:#800080; ">;</span></span></pre>


You can thus write decent error handling code:

<pre class="code_syntax" style="color:#000000;background:#ffffff;"><span class="line_wrapper"><span style="color:#800000; font-weight:bold; ">if</span> <span style="color:#808030; ">(</span><span style="color:#800000; font-weight:bold; ">auto</span> <span style="color:#808030; ">[</span>res<span style="color:#808030; ">,</span> err<span style="color:#808030; ">]</span> <span style="color:#808030; ">=</span> divide<span style="color:#808030; ">(</span><span style="color:#008c00; ">8</span><span style="color:#808030; ">,</span> <span style="color:#008c00; ">0</span><span style="color:#808030; ">)</span><span style="color:#800080; ">;</span> err <span style="color:#808030; ">!</span><span style="color:#808030; ">=</span> <span style="color:#800000; ">"</span><span style="color:#0000e6; ">Success</span><span style="color:#800000; ">"</span><span style="color:#808030; ">)</span> <span style="color:#800080; ">{</span></span>
<span class="line_wrapper">  std<span style="color:#800080; ">::</span>println<span style="color:#808030; ">(</span><span style="color:#800000; ">"</span><span style="color:#0000e6; ">Error occurred: {}</span><span style="color:#800000; ">"</span><span style="color:#808030; ">,</span> err<span style="color:#808030; ">)</span><span style="color:#800080; ">;</span></span>
<span class="line_wrapper"><span style="color:#800080; ">}</span> <span style="color:#800000; font-weight:bold; ">else</span> <span style="color:#800080; ">{</span></span>
<span class="line_wrapper">  std<span style="color:#800080; ">::</span>println<span style="color:#808030; ">(</span><span style="color:#800000; ">"</span><span style="color:#0000e6; ">Result: {}</span><span style="color:#800000; ">"</span><span style="color:#808030; ">,</span> res<span style="color:#808030; ">)</span><span style="color:#800080; ">;</span></span>
<span class="line_wrapper"><span style="color:#800080; ">}</span></span></pre>

You can do slightly better if the reason for returning two values was strictly to be able to return an error code.
You can use std::expected.

Your division function might be written as...

<pre class="code_syntax" style="color:#000000;background:#ffffff;"><span class="line_wrapper">template <span style="color:#808030; ">&lt;</span>std<span style="color:#800080; ">::</span>integral int_type<span style="color:#808030; ">&gt;</span></span>
<span class="line_wrapper">std<span style="color:#800080; ">::</span>expected<span style="color:#808030; ">&lt;</span>int_type<span style="color:#808030; ">,</span> std<span style="color:#800080; ">::</span>string<span style="color:#808030; ">&gt;</span> divide<span style="color:#808030; ">(</span>int_type a<span style="color:#808030; ">,</span> int_type b<span style="color:#808030; ">)</span> <span style="color:#800080; ">{</span></span>
<span class="line_wrapper">    <span style="color:#800000; font-weight:bold; ">if</span> <span style="color:#808030; ">(</span>b <span style="color:#808030; ">=</span><span style="color:#808030; ">=</span> <span style="color:#008c00; ">0</span><span style="color:#808030; ">)</span> <span style="color:#800080; ">{</span></span>
<span class="line_wrapper">        <span style="color:#800000; font-weight:bold; ">return</span> std<span style="color:#800080; ">::</span>unexpected<span style="color:#808030; ">(</span><span style="color:#800000; ">"</span><span style="color:#0000e6; ">Error: Division by zero</span><span style="color:#800000; ">"</span><span style="color:#808030; ">)</span><span style="color:#800080; ">;</span></span>
<span class="line_wrapper">    <span style="color:#800080; ">}</span></span>
<span class="line_wrapper">    <span style="color:#800000; font-weight:bold; ">return</span> a <span style="color:#808030; ">/</span> b<span style="color:#800080; ">;</span></span>
<span class="line_wrapper"><span style="color:#800080; ">}</span></span></pre>

It allows you to write slightly simpler error-handling code:

<pre class="code_syntax" style="color:#000000;background:#ffffff;"><span class="line_wrapper"><span style="color:#800000; font-weight:bold; ">if</span> <span style="color:#808030; ">(</span><span style="color:#800000; font-weight:bold; ">auto</span> res1 <span style="color:#808030; ">=</span> divide<span style="color:#808030; ">(</span><span style="color:#008c00; ">10</span><span style="color:#808030; ">,</span> <span style="color:#008c00; ">2</span><span style="color:#808030; ">)</span><span style="color:#808030; ">)</span> <span style="color:#800080; ">{</span></span>
<span class="line_wrapper">  std<span style="color:#800080; ">::</span>println<span style="color:#808030; ">(</span><span style="color:#800000; ">"</span><span style="color:#0000e6; ">Result: {:&gt;4}, Message: Success</span><span style="color:#800000; ">"</span><span style="color:#808030; ">,</span> res1<span style="color:#808030; ">.</span>value<span style="color:#808030; ">(</span><span style="color:#808030; ">)</span><span style="color:#808030; ">)</span><span style="color:#800080; ">;</span></span>
<span class="line_wrapper"><span style="color:#800080; ">}</span> <span style="color:#800000; font-weight:bold; ">else</span> <span style="color:#800080; ">{</span></span>
<span class="line_wrapper">  std<span style="color:#800080; ">::</span>println<span style="color:#808030; ">(</span><span style="color:#800000; ">"</span><span style="color:#0000e6; ">Error: {}</span><span style="color:#800000; ">"</span><span style="color:#808030; ">,</span> res1<span style="color:#808030; ">.</span>error<span style="color:#808030; ">(</span><span style="color:#808030; ">)</span><span style="color:#808030; ">)</span><span style="color:#800080; ">;</span></span>
<span class="line_wrapper"><span style="color:#800080; ">}</span></span>
<span class="line_wrapper"></span></pre>

Another benefit of std::expected is that you can let the calling code choose the default value (in case of error):

<pre class="code_syntax" style="color:#000000;background:#ffffff;"><span class="line_wrapper"><span style="color:#800000; font-weight:bold; ">int</span> quotient <span style="color:#808030; ">=</span> divide<span style="color:#808030; ">(</span><span style="color:#008c00; ">15</span><span style="color:#808030; ">,</span> <span style="color:#008c00; ">3</span><span style="color:#808030; ">)</span><span style="color:#808030; ">.</span>value_or<span style="color:#808030; ">(</span><span style="color:#008c00; ">0</span><span style="color:#808030; ">)</span><span style="color:#800080; ">;</span></span>
<span class="line_wrapper">std<span style="color:#800080; ">::</span>println<span style="color:#808030; ">(</span><span style="color:#800000; ">"</span><span style="color:#0000e6; ">Quotient only: {:&gt;4}</span><span style="color:#800000; ">"</span><span style="color:#808030; ">,</span> quotient<span style="color:#808030; ">)</span><span style="color:#800080; ">;</span></span></pre>

If you prefer, you can use std::optional, which works much the same way:

<pre class="code_syntax" style="color:#000000;background:#ffffff;"><span class="line_wrapper">std<span style="color:#800080; ">::</span>optional<span style="color:#808030; ">&lt;</span><span style="color:#800000; font-weight:bold; ">int</span><span style="color:#808030; ">&gt;</span> divide<span style="color:#808030; ">(</span><span style="color:#800000; font-weight:bold; ">int</span> a<span style="color:#808030; ">,</span> <span style="color:#800000; font-weight:bold; ">int</span> b<span style="color:#808030; ">)</span> <span style="color:#800080; ">{</span></span>
<span class="line_wrapper">    <span style="color:#800000; font-weight:bold; ">if</span> <span style="color:#808030; ">(</span>b <span style="color:#808030; ">=</span><span style="color:#808030; ">=</span> <span style="color:#008c00; ">0</span><span style="color:#808030; ">)</span> <span style="color:#800080; ">{</span></span>
<span class="line_wrapper">        <span style="color:#800000; font-weight:bold; ">return</span> std<span style="color:#800080; ">::</span>nullopt<span style="color:#800080; ">;</span></span>
<span class="line_wrapper">    <span style="color:#800080; ">}</span></span>
<span class="line_wrapper">    <span style="color:#800000; font-weight:bold; ">return</span> a <span style="color:#808030; ">/</span> b<span style="color:#800080; ">;</span></span>
<span class="line_wrapper"><span style="color:#800080; ">}</span></span></pre>

However, std::optional is less appropriate for error handling: you cannot pass an error code or an error message.
The std::optional strategy is meant for the simple case where a value can be omitted.


In C++, returning multiple values from functions has evolved significantly, offering developers flexible and elegant solutions. From using std::pair and std::tuple for general-purpose multi-value returns to leveraging std::expected for robust error handling, and std::optional for optional values. By adopting these techniques, C++ programmers can make
their code easier to read and to maintain.
