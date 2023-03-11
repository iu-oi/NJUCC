# NJUCC

a C-- compiler based on Nanjing University Compilers Lab 2022

Author: iu_oi

## How to compile

- GCC
- GNU Flex
- GNU Bison

For debian/ubuntu:

`sudo apt-get install build-essential flex bison libbison-dev gcc`

## About NJU C--

See `NJUCC.md` and `example/` for more details.

## Usage

`./njucc <source>`

Optional arguments:

- -a \<file\> Write abstract syntax tree into \<file\>
- -i \<file\> Write 3 addr code into \<file\>
- -s \<file\> Write assembly into \<file\>
- -d/--debug Show compiler debugging info

Debug inter-code:

`python src/njuvm.py <file>`