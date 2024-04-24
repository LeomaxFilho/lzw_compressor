# Leozw compressor
![C++](https://img.shields.io/badge/C%2B%2B-C%2B%2B?style=flat-square&logo=c%2B%2B&logoColor=white&labelColor=blue&color=white)
![Git](https://img.shields.io/badge/Github-github?style=flat-square&logo=github&logoColor=white&labelColor=grey&color=white&link=https%3A%2F%2Fgithub.com%2FLeomaxFilho)
[![FOSSA Status](https://app.fossa.com/api/projects/git%2Bgithub.com%2FLeomaxFilho%2Flzw_compressor.svg?type=small)](https://app.fossa.com/projects/git%2Bgithub.com%2FLeomaxFilho%2Flzw_compressor?ref=badge_small)

###### <em> Leomax da Costa </em> 
###### <em> Federal University of Paraíba </em>

---

A compressor and decompressor that supports variable length codes between 9 and 21 bits by default, but the upper limit is 25 bits, due to the 'write_bit' process.

The code uses the LZW compressor by reference, but actually has changes, to make the code unique.

## Features
- Static size dictionary
- Variable size dictionary
- Commercial speed compression

### Compile

```console
some@you:~$ g++ src/Compressor[...].cpp -o compressor.out
```

---

### Run
### Encode
```console
some@you:~$ ./compressor.out -c <input_file> <output_file_name.leozw>
```
### Decode
```console
some@you:~$ ./compressor.out -d <input_file.leozw> <output_file_name>
```
