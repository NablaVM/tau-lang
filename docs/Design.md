The TAU language is meant to be a high level language that can be compiled to Nabla Byte Code. This document will serve as documentation for how the language shall work from syntax to nabla implementation. 

Note: This language isn't meant to be anything exceptional. The goal isn't to have anything too fancy or modern (whatever that means), rather, the purpose of TAU is to be a tool to better utilize the NVM. 

# Goals

Create a minimum viable language that will be executable on the NablaVM. The first iteration of TAU will generate nabla assembly and then assemble it to byte code using the libnabla assembler. Once the language
is stable and working, the language will directly generate the byte code.

The language will be no more than what is needed (at first) to design programs that function on the VM. 


## Modules / Functions 

Each logical chunk of code will be separated into "modules" of functionality. These modules will contain their own sets of objects and functions. Each module will be represented by a NVM functional unit, segregating it from
other modules' memory. Utilizing the local stack for variable storage. Comminication between modules will happen via the global stack. This segregation will mean that pointers and references can not be passed between modules
as that would violate memory segregation, and instead, pass by copy or via the data store unit will be required. 

```

--  Comments will be represented by a double dash
--  Everything behind a double dash will be disregarded
--

-- The main module must contain and 'init' function. This is the entry module / function that will be called first during execution
module main
{

    -- The internal block defines variables / structures to that are in use in the main module. These items can only be accessed 
    -- by functions defined in the main module. 
    internal
    {
        an_integer : i8         -- Integers will have the forms :   u8, i8, u16, i16, u32, i32, u64, i64, The largest available primitive in the vm is 64 bits
        a_double   : double     -- The double type will utilize the VM double type, and is the only available floating point type
        a_char     : char       -- A single char, represented by a full VM word (8 bytes). At first only ASCII will be supported, but UTF types are planned for the future
        a_string   : char[100]  -- An array of chars. The first version will not support resizing. This would take 100 words. 
    }

    export
    {
        -- Object definitions (described below) that need to be visible to other modules
    }

    --  The first function ran by main, it is the first function called on execution
    --
    func init() -> none {


        return;
    }

}
```

# Types

| Type     | Meaning                        | Size
|---       |---                             |--- 
| u8       |   Unsigned 8-bit integer       |  1 Byte
| i8       |   Signed 8-bit integer         |  1 Byte
| u16      |   Unsigned 16-bit integer      |  2 Bytes 
| i16      |   Signed 16-bit integer        |  2 Bytes 
| u32      |   Unsigned 32-bit integer      |  4 Bytes 
| i32      |   Signed 32-bit integer        |  4 Bytes 
| u64      |   Unsigned 64-bit integer      |  8 Bytes (1 word)
| i64      |   Signed 64-bit integer        |  8 Bytes (1 word)
| char     |   Character                    |  8 Bytes (1 word)

# Arrays

Using [N] following a type will define an array type. Arrays will be limited to
2-dimensional arrays at first i.e : [X][Y]

# Objects 

Taking the form : 

```

obj some_obj {

    -- Explicit grouping of member objects and their types
    members 
    {
        an_intger : i64
    }

    -- A required method that declares how the object is constructed
    -- The init method can have multiple signatures for different constructions, 
    -- but they must all return the type that they are initializing

    method init() -> some_obj {

        some_obj = { 42 };

        -- Construction in the return not intended to be supported in the first iteration of the language

        return some_obj;
    }

    --  For non-init methods, the name by-which the associated object will be handled
    --  must come after the "method" keyword, and before the method name

    method (so) add(x: int) -> none {

        so.an_integer += x;

        return;
    }
};

```

Object members will be private. Access to them will need to be done via methods, which are all public. Objects aren't meant to be used like that of objects in c++. They will be more similar to structs in that their only purpose is to logically group data and associated functionality. 


# Using modules / import management

Import guards are not required. Absolute paths will be monitored to ensure that double adds of files do not occur, and duplicate module names will flag an error. For the first iteration of the language modules being separated into different files will not be supported. The hope is that modules should only take care of one thing each and not get too large. 

That said, it is okay to add multiple modules to a single file. When compiling, a list of directories to search for include shall be given in the event that required files do not reside in the same directory. 

## File 1
```
module DSStore
{
    func something() -> none {
        return;
    }
}

module IO
{
    export
    {
        obj some_io_obj
        {

            method init() -> some_io_obj {

                some_obj = {};
                return some_obj;
            }
        }
    }


    func something(io_object : some_io_obj) -> none {
        return;
    }
}
```

## File 2
```

-- 'uses' declares that this file will be using the modules from file 1. Ther coule be on separate lines, 
-- but comma separating module names will be supported. 
uses IO, DSStore;

module main
{
    func init() -> none {

        let io_obj : IO.some_io_obj = IO.some_io_obj.init();

        IO.something(io_obj);

        return;
    }
}
```

# Variable Assignment

Variables that are not declared in a space like members or internal can be in-line declared using 'let'. 
```
    let <name> : <type> = <something> ;

    let my_dbl_arr : double[100] = {};  -- 100 doubles will be allocated and instantiated to 0.0;

    let my_char_arr : char[100] = {'a', 'b'} -- 100 chars, first two set, the other 98 will init to 0

    -- With char arrays, using a "" will be allowes to set the chars for ease : 

    let my_char_arr2 : char[100] = "This is another way to do char arrays";
```

# References

Variables can be passed within a module as a reference. They must be declared as a standard non-pointer type, and when passed to a function both the call and the function signature must prefix the item with '&'. 

# Pointers

Pointers represent something stored in the DS Store device, and can only point to things within the DS Store. 

TODO : Describe how to declare a new thing to be stored in DS

# Raw Assembly 

TODO : Explain thoughts on nasm api for injecting assembly

# Back End Design Thoughts

TODO : Think through and describe framework for laying out machine information and how we can use that to generate instructions 
       as-well-as bytecode.