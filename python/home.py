from flask import Flask
from flask_restful import Resource, Api, reqparse
import serial
import time

def openPort():
	global usb
	try:
		usb = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
		time.sleep(2) # Arudino needs time to open the port
	except:
		usb = None

app = Flask(__name__)
api = Api(app)

usb = None
openPort()

# Arduino
def send(value):
	try:
		data = bytes(value, 'utf-8')
		global usb
		if usb == None:
			openPort()
		if usb != None:
			usb.write(data)
		return True
	except:
		# usb.close()
		print("Failed to open new port")
		return False

# Web
class Welcome(Resource):
	def get(self):
		return {'Home': 'Welcome!'}, 200

class LedOn(Resource):
	def get(self):
		if send('led:' + str(1)) == True:
			return {'On': True}, 200
		else:
			return {}, 503

class LedOff(Resource):
	def get(self):
		if send('led:' + str(0)) == True:
			return {'Off': True}, 200
		else:
			return {},  503

class LedBlink(Resource):
	def get(self, count):
		if send('ledblink:' + str(count)) == True:
			return {'Blink': count}, 200
		else:
			return {}, 503

class LedBrightness(Resource):
	def get(self, brightness):
		if send('ledbrightness:' + str(brightness)) == True:
			return {'Brightness': brightness}, 200
		else:
			return {}, 503

class IndicatorOn(Resource):
	def get(self):
		if send('indicator:' + str(1)) == True:
			return {'On': True}, 200
		else:
			return {}, 503

class IndicatorOff(Resource):
	def get(self):
		if send('indicator:' + str(0)) == True:
			return {'Off': True}, 200
		else:
			return {}, 503

class IndicatorBlink(Resource):
	def get(self, count):
		if send('indicatorblink:' + str(count)) == True:
			return {'Blink': count}, 200
		else:
			return {}, 503

class MotorOn(Resource):
	def get(self):
		if send('motor:' + str(1)) == True:
			return {'On': True}, 200
		else:
			return {}, 503

class MotorOff(Resource):
	def get(self):
		if send('motor:' + str(0)) == True:
			return {'Off': True}, 200
		else:
			return {}, 503

class MotorSpeed(Resource):
	def get(self, speed):
		if send('motorspeed:' + str(speed)) == True:
			return {'Speed': speed}, 200
		else:
			return {}, 503

class MotorClockwise(Resource):
	def get(self):
		if send('motorcw:' + str(1)) == True:
			return {'Clockwise': True}, 200
		else:
			return {}, 503

class MotorCounterClockwise(Resource):
	def get(self):
		if send('motorcw:' + str(0)) == True:
			return {'Clockwise': False}, 200
		else:
			return {}, 503

class Heartbeat(Resource):
	def get(self, bpm):
		if send('heartbeat:' + str(bpm)) == True:
			return {'Heartbeat': bpm}, 200
		else:
			return {}, 503

class Step(Resource):
	def get(self, step):
		if send('step:' + str(step)) == True:
			return {'Step': step}, 200
		else:
			return {}, 503

class Automata(Resource):
	def post(self):
		parser = reqparse.RequestParser()
		parser.add_argument('bpm', required=True, type=int)
		parser.add_argument('step', required=True, type=int)
		args = parser.parse_args()

		bpm = args['bpm']
		step = args['step']

		if send('automata:' + str(bpm) + ',' + str(step)) == True:
			return {'Heartbeat': bpm, 'Step': step}, 200
		else:
			return {}, 503

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
api.add_resource(Step, '/step/<step>')
api.add_resource(Automata, '/automata')

# Start app
if __name__ == "__main__":
	# app.run()
	app.run(debug=True)
