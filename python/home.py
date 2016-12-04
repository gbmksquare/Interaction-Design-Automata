from flask import Flask
from flask_restful import Resource, Api, reqparse
import serial

app = Flask(__name__)
api = Api(app)

# Arduino
def send(value):
	data = bytes(value, 'utf-8')
	usb = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
	usb.write(data)
	usb.close()

# Web
class Welcome(Resource):
	def get(self):
		return {'Home': 'Welcome!'}, 200

class LedOn(Resource):
	def get(self):
		send('led:' + str(1))
		return {'On': True}, 200

class LedOff(Resource):
	def get(self):
		send('led:' + str(0))
		return {'Off': True}, 200

class LedBlink(Resource):
	def get(self, count):
		send('ledblink:' + str(count))
		return {'Blink': count}, 200

class LedBrightness(Resource):
	def get(self, brightness):
		send('ledbrightness:' + str(brightness))
		return {'Brightness': brightness}, 200

class IndicatorOn(Resource):
	def get(self):
		send('indicator:' + str(1))
		return {'On': True}, 200

class IndicatorOff(Resource):
	def get(self):
		send('indicator:' + str(0))
		return {'Off': True}, 200

class IndicatorBlink(Resource):
	def get(self, count):
		send('indicatorblink:' + str(count))
		return {'Blink': count}, 200

class MotorOn(Resource):
	def get(self):
		send('motor:' + str(1))
		return {'On': True}, 200

class MotorOff(Resource):
	def get(self):
		send('motor:' + str(0))
		return {'Off': True}, 200

class MotorSpeed(Resource):
	def get(self, speed):
		send('motorspeed:' + str(speed))
		return {'Speed': speed}, 200

class MotorClockwise(Resource):
	def get(self):
		send('motorcw:' + str(1))
		return {'Clockwise': True}, 200

class MotorCounterClockwise(Resource):
	def get(self):
		send('motorcw:' + str(0))
		return {'Clockwise': False}, 200

class Heartbeat(Resource):
	def get(self, bpm):
		send('heartbeat:' + str(bpm))
		return {'Heartbeat': bpm}, 200

# API
api.add_resource(Welcome, '/')
api.add_resource(LedOn, '/led/on')
api.add_resource(LedOff, '/led/off')
api.add_resource(LedBlink, '/led/blink/<count>')
api.add_resource(LedBrightness, '/led/brightness/<brightness>')
api.add_resource(IndicatorOn, '/indicator/on')
api.add_resource(IndicatorOff, '/indicator/off')
api.add_resource(IndicatorBlink, '/indicator/blink/<count>')
api.add_resource(MotorOn, '/motor/on')
api.add_resource(MotorOff, '/motor/off')
api.add_resource(MotorSpeed, '/motor/speed/<speed>')
api.add_resource(MotorClockwise, '/motor/cw')
api.add_resource(MotorCounterClockwise, '/motor/ccw')

api.add_resource(Heartbeat, '/heartbeat/<bpm>')

# Start app
if __name__ == "__main__":
	# app.run()
	app.run(debug=True)
