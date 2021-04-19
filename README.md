# mac2rpi_playback_server
Solution to stream audio from a Mac to a Raspberry Pi.

The playback server runs on the Raspberry Pi and it listens to audio packets sent by [the plugin](https://github.com/betabandido/mac2rpi_coreaudio_plugin) running on the Mac.

## Quick Start

Install dependencies:

```bash
sudo apt update
sudo apt install libasound-dev libboost-all-dev
```

Compile and install [PortAudio](http://www.portaudio.com) with the C++ bindings:

```bash
wget http://www.portaudio.com/archives/pa_stable_v190600_20161030.tgz
tar xf pa_stable_v190600_20161030.tgz
cd portaudio
./configure --enable-cxx --with-alsa --without-oss && make
sudo make install
```

Clone the repository:

```bash
BASEDIR=$(pwd)
git clone https://github.com/betabandido/mac2rpi_playback_server.git
```

Compile the playback server:

```bash
cd ${BASEDIR}/mac2rpi_playback_server
cmake . && make
```



To install the resulting binary run the follow command. The binary file will be installed by default under `/usr/local/bin`. Use [CMAKE_INSTALL_PREFIX](https://cmake.org/cmake/help/latest/variable/CMAKE_INSTALL_PREFIX.html#variable:CMAKE_INSTALL_PREFIX) to override this value.

```bash
sudo make install
```

Finally enable the service so that the playback server runs every time the Raspberry Pi boots:

```bash
sudo cp script/mac2rpi_playback_server.service /etc/systemd/system/
sudo systemctl enable mac2rpi_playback_server.service
sudo systemctl start mac2rpi_playback_server.service
```

**NOTE**: the service file might need to be edited if the installation path has been overridden (update `ExecStart` field).

## HiFiBerry

Follow [this guide](https://www.hifiberry.com/build/documentation/configuring-linux-3-18-x/) to set up a [HiFiBerry](https://www.hifiberry.com/products/) device.

*The playback server does not require a HiFiBerry (any device with ALSA support should work).*

## Development

### Running Unit Tests

To run the unit tests, execute:

```bash
BUILD_ONLY_TESTS=1 cmake . && make && ctest --verbose
```
