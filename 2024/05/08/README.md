## Example

```Python
import digit_to_str
print(digit_to_str.all_strings())
print(digit_to_str.all_strings_new())
print(digit_to_str.all_strings_buffered())
print(digit_to_str.digit_to_str(1))
print(digit_to_str.digit_to_str_from(1))
print(digit_to_str.digit_to_str_new(1))
print(digit_to_str.digit_to_str_buffer(1))
```


## Building a testing

We assume that you have a Python development environment (python-dev + C compiler).

In the current directory, type

```
python3 -m pip install . 
```

This should install the package.

Then run:

```
python3 test.py
```