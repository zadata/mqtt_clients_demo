Download and install mosquitto v1.3.1. We'll be using it's libmosquitto C library:


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


Compile sub_cpp_demo

```
cd ~/sub_cpp_demo_dir
g++ *.cpp -I/usr/local/include/ -L/usr/local/lib -lmosquittopp -o sub_cpp_demo -Wall -pedantic
```

Run sub_demo

```
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib  # you may also add this to .profile

./sub_cpp_demo
```
