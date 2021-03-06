# Interaction Design - Automata

This is a second Arduino project for course ARDE324_00 Fall semeter of 2016 in Korea University.

<img src="./images/automata_16.jpg">

For more images, see below.

## Configuration

* Raspberry Pi 3
  * Raspbian GNU/Linux 8 (jessie)
  * Nginx (1.6.2)
  * uWSGI (2.0.14)
  * Python 3 (3.4.2)
  * [Homebridge](https://github.com/nfarina/homebridge) (Optional)
* Arduino Uno

## API

### GET

* `/led/on`: Turn red LED on.
* `/led/off`: Turn red LED off.
* `/led/blink/<count>`: Blink red LED ` <count>` times.
* `/led/brightness/<brightness>`: Change brightness of red LED.
* `/indicator/on`: Turn indicator LED on.
* `/indicator/off`: Turn indicator LED off.
* `/indicator/blink/<count>`: Blink indicator LED ` <count>` times.
* `/motor/on`: Turn motor on.
* `/motor/off`: Turn motor off.
* `/motor/speed/<speed>`: Change motor speed. `<value` is a range between 0 and 100.
* `/motor/cw`: Rotate motor clockwise.
* `/motor/ccw`: Rotate motor counter-clockwise.
* `/heartbeat/<bmp>`: Change pulse of red LED. 0 to stop.
* `/step/<steps>`: Change rotation speed of motor. 0 to stop.

### POST

* `/automata`: `bpm=<int>&step=<int>` Enter any positive integers to blink LED and rotate motor. Use `bpm=0&step=0` to stop blinking LED and rotate motor.

## Build

1. Connect to Raspberry Pi via SSH or open Terminal.
   * `ssh pi@<address>` or `ssh -R 52698:localhost:52698 pi@<address>` to use `rsub`.

2. Install dependencies.
   * `sudo apt-get update`
   * `sudo apt-get upgrade`
   * `sudo apt-get install arduino`
   * `sudo apt-get install nginx python3 python3-dev python3-pip`
   * `sudo pip3 install uwsgi flask flask-restful`
   * To install Hombridge, go to [Wiki](https://github.com/nfarina/homebridge/wiki/Running-HomeBridge-on-a-Raspberry-Pi). (Optional)
     * `curl -sL https://deb.nodesource.com/setup_6.x | sudo -E bash -`
     * ``sudo apt-get install -y nodejs`
     * `sudo apt-get install libavahi-compat-libdnssd-dev`
     * `sudo npm install -g --unsafe-perm homebridge hap-nodejs node-gyp`
     * `cd /usr/lib/node_modules/homebridge/`
     * `sudo npm install --unsafe-perm bignum`
     * `cd /usr/lib/node_modules/hap-nodejs/node_modules/mdns`
     * `sudo node-gyp BUILDTYPE=Release rebuild`
     * `npm install -g homebridge-http`

3. Clone this repository to any directory.
   * `git clone https://github.com/gbmksquare/Interaction-Design-Automata.git`

4. Verify and upload `/arduino/arduino.ino` to Arduino Uno using Arduino IDE.

5. Run `/python/home.py` and test with a web browser or Apple HomeKit (if using Homebridge).
   * `uwsgi --socket 0.0.0.0:8080 --enable-threads --protocol=http --ini home.ini`

6. Make a symbolic link to Homebridge configuration file and run Homebridge. (Optional)

   * `cd ~/.homebridge`


* `ln -s <path_to_project>/homebridge/config.json config.json`
* `homebridge`



## Images

<img src="./images/automata_15.jpg">

<img src="./images/automata_14.jpg">

<img src="./images/automata_13.jpg">

<img src="./images/automata_12.jpg">

<img src="./images/automata_11.jpg">

<img src="./images/automata_10.jpg">

<img src="./images/automata_9.jpg">

<img src="./images/automata_8.jpg">

<img src="./images/automata_7.jpg">

<img src="./images/automata_6.jpg">

<img src="./images/automata_5.jpg">

<img src="./images/automata_4.jpg">

<img src="./images/automata_3.jpg">

<img src="./images/automata_2.jpg">

<img src="./images/automata_1.jpg">

<img src="./images/Mockup-Generated-by-Dunnnk.jpg">

<img src="./images/Mockup-Generated-by-Dunnnk-2.jpg">

## License

MIT License

Copyright (c) 2016 BumMo Koo

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.