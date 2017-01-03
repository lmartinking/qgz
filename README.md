qgz
===

gzip decompression for kdb+/q.

# Build

    $ make {m32,m64,l32,l64}
    $ cp qgz_{m32,m64,l32,l64}.so /path/to/q/bin

# Use

    q) gunzip: (`$"qgz_m64") 2:(`gunzip;1); / change m64 to appropriate platform
    q) gunzip read0 `test.gz / takes a char vector of gzip data to decompress

NOTE: You might need to set `DYLD_LIBRARY_PATH` or `LD_LIBRARY_PATH` environment variables
(Mac and Linux respectively) to the directory where the `.so` lives before running `q`.

# Licence

LGPLv3. See `LICENSE` and `COPYING.LESSER`.

Copyright (c) 2016 Lucas Martin-King.
