# mac2rpi_audio
Solution to stream audio from a Mac to a Raspberry Pi.

The playback servers runs on the Raspberry Pi, and it listens to audio packets sent by the plugin running on the Mac.

## Quick Start

Clone the repository:

```bash
BASEDIR=$(pwd)
git clone https://github.com/betabandido/mac2rpi_playback_server.git
```

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

Compile the playback server:

```bash
cd ${BASEDIR}/mac2rpi_playback_server
scons
```

Install and enable the service:

```bash
sudo cp script/mac2rpi_playback_server.service /etc/systemd/system/
sudo systemctl enable mac2rpi_playback_server.service
sudo systemctl start mac2rpi_playback_server.service
```

**NOTE**: the service file should be edited to include the path to the server executable (`ExecStart` field).

## HiFiBerry

Follow [this guide](https://www.hifiberry.com/build/documentation/configuring-linux-3-18-x/) to set up a [HiFiBerry](https://www.hifiberry.com/products/) device.

*The playback server does not require a HiFiBerry (any device with ALSA support should work).*
