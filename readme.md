This is a Playstation game I made for [32bit Jam 2022](https://itch.io/jam/32bit-jam-2022). The build system is based off [nolibgs_hello_worlds](https://github.com/ABelliqueux/nolibgs_hello_worlds).

Compilation on Debian/Ubuntu
------------

Install the MIPS toolchain.

	sudo apt-get install gcc-mipsel-linux-gnu g++-mipsel-linux-gnu binutils-mipsel-linux-gnu

Clone this repository.

	git clone https://github.com/samfromcadott/32bitjam2022.git --recursive

Download Psy-Q (Playstation SDK).

	cd 32bitjam2022
	wget http://psx.arthus.net/sdk/Psy-Q/psyq-4.7-converted-full.7z
	7z x psyq-4.7-converted-full.7z -o./psyq

Build.

	make
