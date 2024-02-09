from flask import Flask, request, jsonify, render_template
import sqlite3

app = Flask(__name__)

# SQLite database configuration
DB_NAME = 'data.db'

def create_connection():
   conn = sqlite3.connect(DB_NAME)
   return conn

@app.route('/api/data', methods=['POST'])
def receive_data():
   data = request.json  # Assuming data is sent in JSON format
   temperature = data.get('temperature')
   humidity = data.get('humidity')

   # Store the data in SQLite database
   conn = create_connection()
   cursor = conn.cursor()
   cursor.execute("INSERT INTO sensor_data (temperature, humidity) VALUES (?, ?)", (temperature, humidity))
   conn.commit()
   conn.close()

   # Respond with a success message
   response = {'message': 'Data received successfully', 'temperature': temperature, 'humidity': humidity}
   return jsonify(response), 200

@app.route('/data')
def display_data():
   conn = create_connection()
   cursor = conn.cursor()
   cursor.execute("SELECT temperature, humidity, timestamp FROM sensor_data")
   data = cursor.fetchall()
   conn.close()
   return render_template('data.html', data=data)

if __name__ == '__main__':
   app.run(debug=True)
