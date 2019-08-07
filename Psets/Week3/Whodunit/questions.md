# Questions

## What's `stdint.h`?

Declares sets of integer types with specified widths and defines macros

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

To preserve the correct numerical value

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

BYTE = 1, DWORD = 4, LONG = 4

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

0x4d42

## What's the difference between `bfSize` and `biSize`?

bfsize is the whole file while biSize is only for the header

## What does it mean if `biHeight` is negative?

Then it is top down with a top-left origin

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

biBitCount

## Why might `fopen` return `NULL` in `copy.c`?

If the program fails to open

## Why is the third argument to `fread` always `1` in our code?

there's only 1 file header

## What value does `copy.c` assign to `padding` if `bi.biWidth` is `3`?

3

## What does `fseek` do?

changes the position indicator of the file stream

## What is `SEEK_CUR`?

origin point for the fseek operation
