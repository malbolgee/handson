# Projeto HandsOn para o curso DevTITANS - Motorola | UFAM



## How to sync and compile Android on your machine

### install Git and Repo

```sh
$ sudo apt update
$ sudo apt install git
$ sudo apt install wget python3 python-is-python3
```

To verify if git was successfully installed

```sh
$ git --version
```

Configure git to use your name and e-mail

```sh
$ git config --global user.name <your name>
$ git config --global user.email <your e-mail>
```

To install repo, on your home directory, do:

```sh
$ mkdir opt; cd bin
$ wget https://storage.googleapis.com/git-repo-downloads/repo
$ chmod 755 repo
```

Now, export this directory to your .bashrc  (or .zshrc, if you're using zsh shell).

```sh
$ echo "export PATH=$PATH:~/opt/" >> ~/.bashrc
```

Close the terminal and open a new one (or do ```source .bashrc``` in your home directory).

Test to see if repo is 'visible'.

```sh
$ repo version
```

If something like this is the output, you're good to go.

```sh
<repo not installed>
repo launcher version 2.21
       (from /home/user/opt/repo)
git 2.25.1
Python 3.8.10 (default, Jun 22 2022, 20:18:18) 
[GCC 9.4.0]
OS Linux 5.15.0-46-generic (#49~20.04.1-Ubuntu SMP Thu Aug 4 19:15:44 UTC 2022)
CPU x86_64 (x86_64)
Bug reports: https://bugs.chromium.org/p/gerrit/issues/entry?template=Repo+tool+issue

```

### Initializing the AOSP Repo

Create a directory to store the AOSP source code

```sh
$ cd ~
$ mkdir aosp
$ cd aosp
```

Initialize the Repo repository inside the directory:

```sh
$ repo init --depth=1 -u https://android.googlesource.com/platform/manifest -b android-12.0.0_r3
```

The --depth=1 configures Git to download only the most up-to-date version of the source code, without having to download all the repository's history. This brings the download down to 'only' 20GB, instead of 80GB.

### Downloading AOSP

The process of leaving your local directory with the same code as the remote server is called "*synchronization*". On first run, sync basically downloads all the AOSP source code. To sync your local repository, run:

```sh
$ repo sync -c -j8
```

The -c flag (current) will synchronize only the current branch. The -j flag indicates how many processos/threads to use during the synchronization.

**NOTE**: Too many threads will actually make the AOSP server complain about too many connections. If your internet is average, this process should not take too long even if with 8 threads.

### Creating a new product

Go inside your newly synched AOSP repository

```sh
$ cd device
$ mkdir devtitans; cd devtitans
$ git clone https://github.com/malbolgee/handson.git
```

The ```handson``` product directory will be created.

Of course, the process of creating a new product is already done by this repository, if you want to create a really new product, follow the steps below

### TBD

## Compiling AOSP

First, lets install some necessary packages

```sh
$ sudo apt install vim git git-core python3 python-is-python3 python-mako openjdk-8-jdk android-tools-adb bc bison build-essential curl flex g++-multilib gcc-multilib gnupg gperf imagemagick lib32ncurses-dev lib32readline-dev lib32z1-dev liblz4-tool libncurses5-dev libsdl1.2-dev libssl-dev libxml2 libxml2-utils lzop pngcrush rsync schedtool squashfs-tools xsltproc yasm zip zlib1g-dev libtinfo5 libncurses5 zip libc6-dev-i386 lib32ncurses5-dev x11proto-core-dev libx11-dev libgl1-mesa-dev screen unzip fontconfig kpartx libcurl4
```

**NOTE**: If you're using Ubuntu, chances are that you'll not be able to install some of these packages, others you'll already have on your machine. You probably only need the following packages:

```sh
$ sudo apt install build-essential libncurses5 zram-config libncurses5-dev android-tools-adb lib32ncurses-dev lib32readline-dev lib32z1-dev libsdl1.2-dev libssl-dev lzop pngcrush schedtool flex kpartx tmux
```

After this installation, restart you machine, the ```zram-config``` package requires it to be activated.

### Configuring the enviroment

Before compiling or using any AOSP specific command, we must first configure the enviroment using the envsetup.sh script

```sh
$ cd ~/aosp
$ source build/envsetup.sh
```

The envsetup.sh is a script that brings a set of useful functions that makes the use of AOSP a little bit easier.

Here are some usefull commands that this script declares:

| **Command** | **Description**                                              |
| ----------- | ------------------------------------------------------------ |
| lunch       | Selects the target product that will be work in the terminal session |
| croot       | Go back to the AOSP root directory                           |
| m           | Compiles Android                                             |

As shown above, one of the declared commands by envsetup.sh is the ```lunch``` command. This command selects which product will be worked on in the current terminal session. Basically, it configures a set of environment variables on your shell session related to the selected device. Just like envsetup.sh, this command needs to be executed in each open shell. To see the list of products, simply do:

```sh
$ lunch
```

| **Product**                    | **Description**                                              |
| ------------------------------ | ------------------------------------------------------------ |
| aosp_barbet-userdebug          | Product to be used in [Google Pixel 5a](https://store.google.com/us/product/pixel_5a_5g?hl=en-US) devices. |
| aosp_car_x86_64-userdebug      | Used to emulate an Android being executed in a vehicle.      |
| aosp_cf_x86_64_phone-userdebug | Android being emulated in [Cuttlefish](https://source.android.com/setup/create/cuttlefish), using a X86 64 bits processor. |
| aosp_cf_x86_64_tv-userdebug    | Used to emulate an Android being executed in a TV.           |
| aosp_flame-userdebug           | Product to be used in a Google Pixel 4 device.               |
| beagle_x15-userdebug           | Product specifically for the [Beagle x15](https://beagleboard.org/x15) board. |
| sdk_phone_x86_64-userdebug     | Product for the emulator executing in a X86 64 bits processor. |
| devtitans_handson-eng          | Product that this repository creates. **We'll use this one**. |

In addition, one can observe the end of the names, notice three compilation variations:

| **Variation** | **Description**                                              |
| ------------- | ------------------------------------------------------------ |
| user          | Android final version that will be executed by users, without debug information a no root access. |
| userdebug     | Same as the previous, but with debug information and root access. |
| eng           | Development version, with extra tools, debug information and root access. |

```sh
$ lunch devtitans_handson-eng
```

### Compiling Android

It is time to compile. To do it:

```sh
$ m -j$(nproc)
```

It could (*will*) take a while if this is the first compilation, but after it is done:

```sh
$ emulator &
```

## The Android Kernel

This project uses a kernel module in order to be able to use the esp32 board that captures the packets in promiscuous mode. We'll recompile the kernel including this module written for this project.

### Downloading, configuring and compiling the kernel

The official Linux kernel is developed and maintained by Linus Torvalds and can be found on [GitHub](https://github.com/torvalds/linux). However, Android includes a set of modules, configurations, and modifications that they do upon the original kernel. To make it easier, this modified kernel is available [here](https://android.googlesource.com/kernel/manifest/+refs).

To compile the kernel, some packages are necessary

```sh
$ sudo apt install kernel-package bzip2 lib32z1 libelf-dev qt5-default qttools5-dev-tools qttools5-dev meld geany gtk+-2.0 libgtk-3-dev libwebkit2gtk-4.0-dev autogen libgtk2.0-dev libglade2-dev
```

Although the kernel source code is available from Google, it is not part of the AOSP source code. So we'll need to create a new directory (outside of AOSP) to store the code

```sh
$ cd ~
$ mkdir kernel
$ cd kernel
```

After that, we'll use repo to download the kernel source code.

**NOTE**: Use a different terminal session to do this, or the kernel repo will overwrite the AOSP repo.

```sh
$ init --depth=1 -u https://android.googlesource.com/kernel/manifest -b common-android12-5.10-lts
$ repo sync --force-sync --no-clone-bundle --no-tags -j$(nproc)
```

Approximately 5 GB will be downloaded. As we are going to copy the compiled files manually to the AOSP directory, we are going to change the kernel configuration file. Put the code below at the ***end*** of the file

```sh
$ vim common-modules/virtual-device/build.config.virtual_device.x86_64
```

```makefile
SKIP_CP_KERNEL_HDR=1
FILES="
arch/x86/boot/bzImage
vmlinux
System.map
"
MAKE_GOALS="
bzImage
modules
"
```

Now, lets compile the kernel

```sh
$ time BUILD_CONFIG=common-modules/virtual-device/build.config.virtual_device.x86_64 build/build.sh -j$(nproc)
```

It might take between 10 to 12 minutes to compile. The final result will be the kernel itself, called ```bzImage``` alongside the modules, which are the ```.ko``` files.

```sh
$ ls -lh out/android12-5.10/dist/bzImage
```

### Copying the new kernel to AOSP

```sh
$ mkdir -p ~/aosp/device/devtitans/handson/kernel/image/
```

Now, lets copy the kernel and its modules to the new directory

```sh
$ cp out/android12-5.10/dist/bzImage ~/aosp/device/devtitans/handson/kernel/image/
$ cp out/android12-5.10/dist/*.ko ~/aosp/device/devtitans/handson/kernel/image/
```

**NOTE**: This repository already comes with a pre-compiled kernel image and modules. Only do this (compile) to test or if you want to update the kernel or its modules.

Next, we need to tell the build system that we are going to use our new kernel. To do this, we'll need to change an emulator configuration file

```sh
$ cd ~/aosp
$ vim device/generic/goldfish/x86_64-kernel.mk
```

Change the lines

```makefile
KERNEL_MODULES_PATH := device/devtitans/handson/kernel/image/
EMULATOR_KERNEL_FILE := device/devtitans/handson/kernel/image/bzImage
```

Next

```sh
$ source build/envsetup.sh
$ lunch devtitans_handson-eng
$ m -j$(nproc)
```

To test if the new kernel is on your Android image

```sh
$ emulator &
$ adb shell
$ cat /proc/version
```

The old kernel version starts with something like this

> *Linux version 5.10.43-android12-9-00031-g5b153f189546-ab7618735 (build-user@build-host) (Android (7284624, based on r416183b) clang version 12.0.5 (https://android.googlesource.com/toolchain/llvm-project c935d99d7cf2016289302412d708641d52d2f7ee), LLD 12.0.5 (/buildbot/src/android/llvm-toolchain/out/llvm-project/lld c935d99d7cf2016289302412d708641d52d2f7ee)) #1 SMP PREEMPT Fri Aug 6 17:53:30 UTC 2021*

And the new one

> *Linux version 5.10.120-android12-9-g2de0a17df44a-dirty (build-user@build-host) (Android (7284624, based on r416183b) clang version 12.0.5 (https://android.googlesource.com/toolchain/llvm-project c935d99d7cf2016289302412d708641d52d2f7ee), LLD 12.0.5 (/buildbot/src/android/llvm-toolchain/out/llvm-project/lld c935d99d7cf2016289302412d708641d52d2f7ee)) #1 SMP PREEMPT Sat Jul 23 14:09:48 UTC 2022*

### Including the sniffer module

For this project, we'll use a custom module that can communicate with the esp32 board, sending commands and managing responses. Because of that, we need to include this module in the compilation process to generate the ```.ko``` file module file that will later be loaded into the kernel.

Let's begin copying the Sniffer code to the kernel directory

```sh
$ cp ~/kernel
$ cp -r -p ~/aosp/device/devtitans/handson/kernel/modules/sniffer-module .
```

Now, let's edit the kernel compiling configuration file to add our module

```makefile
FILES=""
MAKE_GOALS="modules"

SKIP_CP_KERNEL_HDR=1
FILES="
arch/x86/boot/bzImage
vmlinux
System.map
"
MAKE_GOALS="
bzImage
modules
"

EXT_MODULES+=" sniffer-module"
```

Next, let's compile

```sh
$ time BUILD_CONFIG=common-modules/virtual-device/build.config.virtual_device.x86_64 SKIP_MRPROPER=1 build/build.sh -j$(nproc)
```

This time, we do not need to compile the whole kernel again, we use the ```SKIP_MRPROPER``` flag to tell the make command not to remove the files already compiled.

When it's done, the compilation must have generated our compiled module

```sh
ls -lh out/android12-5.10/dist/sniffer.ko
```

Next, copy the module to the AOSP directory

```sh
$ cp out/android12-5.10/dist/sniffer.ko ~/aosp/device/devtitans/handson/kernel/image/
```

Close your emulator, in case it's open, and recompile

```sh
$ cd ~/aosp
$ m -j$(nproc)
```

Finally, lets test if our new module is loaded into the kernel

```sh
$ emulator &
$ adb shell
$ lsmod | grep sniffer
$ dmesg | grep sniffer
```

