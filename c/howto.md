Install needed packages

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

Compile sub_demo

```
cd ~/sub_demo_dir
gcc sub_demo.c -I/usr/local/include/ -L/usr/local/lib -lmosquitto -o sub_demo -Wall -pedantic
```

Run sub_demo

```
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib  # you may also add this to .profile
./sub_demo
```
