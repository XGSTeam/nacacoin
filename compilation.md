# Additional instructions for Mac

```console
sudo ./contrib/install_db4.sh /usr/local

export BDB_PREFIX='/usr/local/db4'

brew install automake autoconf pkg-config qt boost openssl libevent

./configure BDB_LIBS="-L${BDB_PREFIX}/lib -ldb_cxx-4.8" BDB_CFLAGS="-I${BDB_PREFIX}/include"
```


If you need to have qt first in your PATH run:
  echo 'export PATH="/usr/local/opt/qt/bin:$PATH"' >> ~/.bash_profile

For compilers to find qt you may need to set:
  export LDFLAGS="-L/usr/local/opt/qt/lib"
  export CPPFLAGS="-I/usr/local/opt/qt/include"

For pkg-config to find qt you may need to set:
  export PKG_CONFIG_PATH="/usr/local/opt/qt/lib/pkgconfig"

export LDFLAGS="-L/usr/local/opt/boost@1.60/lib"
  export CPPFLAGS="-I/usr/local/opt/boost@1.60/include"