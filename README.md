# Interaction Design - Automata

This is a second Arduino project for course ARDE324_00 Fall semeter of 2016 in Korea University.

## Configuration

* Raspberry Pi 3
  * Raspbian GNU/Linux 8 (jessie)
  * Nginx (1.6.2)
  * uWSGI (2.0.14)
  * Python 3 (3.4.2)
* Arduino Uno

## Build

* Raspberry Pi
  * Install dependencies.
    * `sudo apt-get update`
    * `sudo apt-get upgrade`
    * `sudo apt-get install nginx python3 python3-dev python3-pip`
    * `pip3 install uwsgi flask flask-restful`
  * Open Terminal or connect to Raspberry Pi via ssh.
    *  `ssh pi@<address> ` (address is local address of Raspberry Pi; Make sure that both Raspberry Pi and client are on the same network.)
    * `ssh -R 52698:localhost:52698 pi@<address>` if `rsub` is not working on Raspberry Pi.
  * Move to home directory or any other directory.
    * `cd ~/`
  * Clone current repository.
    * `git clone https://github.com/gbmksquare/Interaction-Design-Automata.git`
  * Move to `python` directory.
    * `cd ./python`
  * Run python application.
    * `uwsgi --socket 0.0.0.0:8080 --enable-threads --protocol=http --ini home.ini`
  * Open any web browser and visit `<address>:8080` to see the application running.
* Arduino
  * Move to `arduino` directory.
    * `cd ./arduino`
  * Open `automata.ino` in Arduino IDE.
  * Connect Arduino Uno to Raspberry Pi via USB.
  * Verify and upload to Arduino Uno.

## License

MIT License

Copyright (c) 2016 BumMo Koo

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.