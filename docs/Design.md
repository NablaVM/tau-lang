The TAU language is meant to be a high level language that can be compiled to Nabla Byte Code. This document will serve as documentation for how the language shall work from syntax to nabla implementation. 

Note: This language isn't meant to be anything exceptional. The goal isn't to have anything too fancy or modern (whatever that means), rather, the purpose of TAU is to be a tool to better utilize the NVM. 

# Goals

Create a minimum viable language that will be executable on the NablaVM. The first iteration of TAU will generate nabla assembly and then assemble it to byte code using the libnabla assembler. Once the language
is stable and working, the language will directly generate the byte code.

The language will be no more than what is needed (at first) to design programs that function on the VM. 


## C

Throughout recent history whenever someone makes a new pice of hardware they make a C compiler, or a sub-c compiler. I want to follow this somewhat and make tau a C-Like language. That said, I intend on making
some changes to the syntax a little bit more like recent languages. 

# Types

The first types of this language will be 

    int, double, char

These types will rely on their underlying VM representations with chars being represented as ints. This will waste some space at first, but it also allows us some wiggle room when it comes
to expanding the char set to include UTF encodings. 

# Control Flow

If / else if / else will be supported 

while / for loops (do while loops potentially)

enums 

```
// Standard enums
    enum MyEnum {

        MY_VAL,
        MY_OTHER_VAL,
        SOME_OTHER_VAL
    };

// Potentially, 

    enum MyEnum<SomeStruct> {
        MY_VAL,
        OTHER_VAL
    };

    swtich(someMyEnum) {
        case MY_VAL :
            $.structMember = 3;
            break;
        default:
            break;
    }
```

# Structs

```

struct myStruct 
{
    c : int;
    b : int;
};

func new_struct() -> myStruct {

    x : myStruct = 
    { 
        c: 0, 
        b: 0 
    };

    y : myStruct = 
    {
        0,
        0
    };

    return x;
}

func (ms : myStruct *) some_func() {

}

func main() -> int {

    myStruct x;

    x.some_func();

    return 0;
}

```