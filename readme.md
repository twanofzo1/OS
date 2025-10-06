# about

this is a hobby project to learn more about os programming
it is a simple x86_64 os written in C and assembly  
it is not meant to be a full featured os but rather a learning project
i am folowing a tutorial from [OSdev](https://wiki.osdev.org)


# building
to build the os you need to have a cross compiler for x86_64
you can use [this](https://wiki.osdev.org/GCC_Cross-Compiler) guide to build one
furthermore limine is installed when you call run.sh
to build the os simply run 
```bash
./run.sh
```
this will build the os and run it in qemu
you can also run qemu manually with
```bash
qemu-system-x86_64 -hda image.hdd -m 512M
```