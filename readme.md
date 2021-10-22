# Archiver

## Huffman

This project uses the Huffman algorithm.
The idea of it is to optimize the overall file size by
constructing a new encoding table that depends on the number
of occurrences of each byte in the file. 

That way, a byte that occurs 10 times more frequently than the other will
tend to have a shorter encoding word length.

## Installation

```
mkdir build
cd build
cmake .. && make 
```

This will generate the executable file MAIN

## Usage:

* `MAIN -c archive_name file1 [file2 ...]` - creates an archive of file1, file2, ...
* `MAIN -d archive_name` - extracts the files from archive and puts them into current directory
* `MAIN -h` - displays the help message.

## Benchmarks

### Compression

|   Type of File   | Initial File Size | Compressed File Size |   Time   |
|------------------|-------------------|----------------------|----------|
| Linux executable |      1.12 MiB     |      877.68 KiB      |  0.451s  |
|    Text File     |      89.6 KiB     |      48.18  KiB      |  0.041s  |
|      Image       |     0.9807 MiB    |      0.9816 MiB      |  0.427s  |
|      Video       |     23.376 MiB    |      23.388 MiB      |  13.310s |

### Decompression

|   Type of File   | Compressed File Size | Decompressed File Size |   Time   |
|------------------|----------------------|------------------------|----------|
| Linux executable |      877.68 KiB      |        1.12 MiB        |  0.262s  |
|    Text File     |      48.18  KiB      |        89.6 KiB        |  0.050s  |
|      Image       |      0.9816 MiB      |       0.9807 MiB       |  0.289s  |
|      Video       |      23.388 MiB      |        23.37 MiB       |  7.506s  |

## License

This software is being distributed under the MIT License
