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
		return {'Home': 'Welcome!',
				'API': {'LED': '/led/:count',
						'Motor': '/motor/:mode (-1: CCW, 0: Stop, 1: CW'}
				}, 200

class Led(Resource):
	def get(self, count):
		send('led:' + str(count))
		return {'led': count}, 200

class Motor(Resource):
	def get(self, mode):
		send('motor:' + str(mode))
		return {'result': mode}, 200

# API
api.add_resource(Welcome, '/')
api.add_resource(Led, '/led/<count>')
api.add_resource(Motor, '/motor/<mode>')

# Start app
if __name__ == "__main__":
	# app.run()
	app.run(debug=True)
