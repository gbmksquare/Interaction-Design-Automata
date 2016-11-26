from flask import Flask
from flask_restful import Resource, Api, reqparse
import serial
# import threading

app = Flask(__name__)
api = Api(app)

# Arduino
def send(value):
	# thread = threading.Thread(target=send, args=('2'))
	# thread.start()
	# thread.join()
	data = bytes(value, 'utf-8')
	usb = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
	usb.write(data)
	usb.close()

# Web
class Welcome(Resource):
	def get(self):
		return {'Home': 'Welcome!', 'API': '/blink/:count, /lamp/:isOn'}, 200

class Blink(Resource):
	def get(self, count):
		value = str(count)
		send(value)
		return {'blink': count}, 200

class Lamp(Resource):
	def get(self, isOn):
		return {'result': isOn}, 200

# API
api.add_resource(Welcome, '/')
api.add_resource(Blink, '/blink/<count>')
api.add_resource(Lamp, '/lamp/<isOn>')

# Start app
if __name__ == "__main__":
	# app.run()
	app.run(debug=True)
