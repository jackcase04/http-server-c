### HTTP Server

#### Build

To run this server, clone the repository on a Linux machine. In the project root, simply run:

```
make
```

#### Run

After building, just run:

```
./a.out
```

If you wish to remove object and executable files and start over, run:

```
make clean
```

#### TODO

- [x] Handle concurrent client requests
- [x] Allow dynamic access to any file in `public` folder
    - Considering security (path traversal, etc)
- [ ] Strengthen parsing of HTTP request
    - Include parsing headers, etc
