Installl needed packages

```
sudo apt-get update
sudo apt-get install make gcc g++
```

Download and install mosquitto

```
cd ~
wget http://mosquitto.org/files/source/mosquitto-0.15.tar.gz
tar -xvzf mosquitto-0.15.tar.gz 
cd mosquitto-0.15
make
sudo make install
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
