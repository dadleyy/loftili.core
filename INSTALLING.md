# Core library installation

This guide was made to help users get the core library compiled onto their compatible device. We recommend the [raspberry pi 2](http://amzn.com/B00T2U7R7I), with the [ubuntu snappy core](http://developer.ubuntu.com/en/snappy/) os installed. For instruction on getting the operating system installed on your raspberry pi, see [this guide](http://elinux.org/RPi_Easy_SD_Card_Setup), which walks you through the process of burning an image to an SD card, which you'll use as the storage on your device. On OSX, this process looks like:

```
$ diskutil list
$ diskutil unmountDisk /dev/<disk# from diskutil>
$ sudo dd bs=1m if=<your image file>.img of=/dev/<disk# from diskutil>
```

*note: using `/dev/rdisk#` will take less time*

## Package dependencies

After getting the operating system installed on your device, you will need to install a few packages that are prerequisites of our core library. These are tools that will allow you to compile the library into an executable.

```
$ sudo apt-get update
$ sudo apt-get install git libssl-dev vim build-essential autoconf automake libtool wget -y
```

### Packages used by the core during runtime

After installing the tools required to compile the application, you'll need to install a few libraries that are used by the application during runtime. They are:

1. [alsa-lib](http://www.alsa-project.org/main/index.php/Download)
2. [ao](https://www.xiph.org/ao/)
3. [mpg123](http://www.mpg123.de/)

### Installing from source

We prefer to make and install these libraries ourselves. This allows us to maintain control over the versions, which is important in ensuring a smooth sailing. 

The source files can be downloaded from the websites above, and compiling them is straightforward:

```
$ wget <artifact url>/alsa-lib-1.0.28.tar.bz2
$ tar xjf alsa-lib-1.0.28.tar.bz2
$ cd alsa-lib-1.0.28
$ ./configure
$ make
$ make install

$ wget <artifact url>/libao-1.2.0.tar.gz
$ tar xzf libao-1.2.0.tar.gz
$ cd libao-1.2.0
$ ./configure
$ make
$ make install

$ wget <artifact url>/mpg123-1.20.1.tar.bz2
$ tar xjf mpg123-1.20.1.tar.bz2
$ cd mpg123-1.20.1
$ ./configure
$ make
$ make install
```

**important: after installing these from source, you will need to update your system's `LD_LIBRARY_PATH`**
```
export LD_LIBRARY_PATH=/usr/local/lib:/usr/lib
```

### Installing from packages

If you are uncomfortable installing the libraries yourself, you could always install them through you operating system's package manager. On ubuntu snappy, installing these could be done by running:

```
$ sudo apt-get install -y libao-dev libmpg123-dev
```

*note: while installing the library on our own devices, we preferred installing from source*

### Ensuring device's audio capability

While we do suggest using a raspberry pi v2 and ubuntu snappy, there are known issues getting the 3.55mm audio jack to be work with the operating system. 

In order to check your device's audio setup, we recommend installing the `alsa-utils` package, which contains an application called `aplay` that can be used to list the audio output devices your device is aware of:

```
$ aplay -l
```

this will output something along the lines of:

```
**** List of PLAYBACK Hardware Devices ****
card 0: ALSA [bcm2835 ALSA], device 0: bcm2835 ALSA [bcm2835 ALSA]
  Subdevices: 8/8
  Subdevice #0: subdevice #0
  Subdevice #1: subdevice #1
  Subdevice #2: subdevice #2
  Subdevice #3: subdevice #3
  Subdevice #4: subdevice #4
  Subdevice #5: subdevice #5
  Subdevice #6: subdevice #6
  Subdevice #7: subdevice #7
card 0: ALSA [bcm2835 ALSA], device 1: bcm2835 ALSA [bcm2835 IEC958/HDMI]
  Subdevices: 1/1
  Subdevice #0: subdevice #0
card 1: CODEC [USB Audio CODEC], device 0: USB Audio [USB Audio]
  Subdevices: 1/1
  Subdevice #0: subdevice #0
```

If you are planning on using the 3.5mm audio jack instead of HDMI, we recommend purchasing a usb sound card like [this one](http://www.amazon.com/gp/product/B001MSS6CS) which should be automatically picked up by `aplay` after plugging it in. Once you've confirmed the soundcard is understood by the operating system, you will need to edit `/usr/share/alsa/alsa.conf`, changing the default card from 0 to 1:

```
defaults.pcm.card 1
```

## Downloading the source

The source code for the library can be downloaded from our github's [releases](https://github.com/loftili/core/releases) page, or it can be cloned using:

```
$ git clone https://github.com/loftili/core.git ./core
```

Once you have the source code downloaded, you'll need to copy our vendor libraries into a `vendor` director; these libraries are:

1. [spdlog](https://github.com/gabime/spdlog) - a logging library used in favor of "in-house" solution
2. [rapidjson](https://github.com/miloyip/rapidjson) - json parsing library used to get information from api requests

```
$ cd ./core
$ mkdir -p vendor
$ cd vendor
$ git clone https://github.com/miloyip/rapidjson ./rapidjson
$ git clone https://github.com/gabime/spdlog.git ./spdlog
```

## Compiling the source

Once everything is in place, compiling the source code can be done by running:

```
$ cd ./core
$ ./bootstrap
$ ./configure
$ make
```

Be sure to view the output from the `./confgure command` - it will have helpful information regarding the core library's ability to compile properly - including a check that the dependent packages were installed correctly. You should see something like:

![image](https://cloud.githubusercontent.com/assets/1545348/7918818/26f53d96-0862-11e5-9372-10d980fe2954.png)

## Running

After successfully compiling the source code, your executable can be found at `./src/loftili`. Running without any arguments should generate the following output:

![image](https://cloud.githubusercontent.com/assets/1545348/7919013/6a1700ae-0863-11e5-88df-881f39be6fcd.png)

The `serial` parameter should be the serial number assigned to you via our [account request](https://lofti.li/join) form. The rest of the arguments are optional; values default to our production api server and a default log file.
