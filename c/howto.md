
Download and install mosquitto v1.3.1, we'll be using it's libmosquitto C library

``` bash
sudo apt-get update
sudo apt-get install -y build-essential libtool m4 autoconf openssl libssl-dev libc-ares-dev
cd
wget http://mosquitto.org/files/source/mosquitto-1.3.1.tar.gz
tar xzf mosquitto-1.3.1.tar.gz
rm mosquitto-1.3.1.tar.gz
cd mosquitto-1.3.1
make
sudo make install
rm -rf mosquitto-1.3.1/
echo 'export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib' >> ~/.bashrc # or ~/.profile
```

Compile sub_demo

``` bash
cd ~/sub_demo_dir
gcc sub_demo.c -I/usr/local/include/ -L/usr/local/lib -lmosquitto -o sub_demo -Wall -pedantic
```

Run sub_demo

``` bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib  # you may also add this to .profile

./sub_demo
```
