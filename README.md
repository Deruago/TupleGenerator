# TupleGenerator

This is a simple meta function for C++17.

## Usecase

Reconstructing Variable typenames, or constructing tuples.

## Usage

```C++17
TupleGenerator<int, double>
        ::type<2,   1> generatedTuple1; // std::tuple<int, int, double>
TupleGenerator<int, double, char>
        ::type<1,   2,      3> generatedTuple2; // std::tuple<int, double, double, char, char, char>
```

The first type list specifies the available types.
The second integer list specifies the desired amount of the type at the same index.