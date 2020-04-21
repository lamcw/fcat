# fcat

Extending the idea of [fastcat](https://endler.dev/2018/fastcat/), this is an attempt to implement `cat` with higher throughput by using `splice(2)` to avoid copying between kernel space and user space.

## Build
```console
$ make
```

## Installation
```console
$ make install
```

## Usage
Just like `cat`:

```console
$ fcat *.c
```

## Benchmark
Tested with 10GB file generated with `dd`.
```console
$ dd if=/dev/random of=10gb.txt count=1024 bs=10485760
```

The pipe speed can be checked with [pv](https://www.ivarch.com/programs/pv.shtml).

GNU `cat`
```console
$ cat 10gb.txt | pv -r > /dev/null
[ 500MiB/s]
```

`fcat`
```console
$ fcat 10gb.txt | pv -r > /dev/null
[ 780MiB/s]
```

Interestingly, the speed of `fcat` fluctuates more than `cat`. And sometimes it is slower than `cat`.

## License
[MIT](https://choosealicense.com/licenses/mit/)
